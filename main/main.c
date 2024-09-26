/*
 * SPDX-FileCopyrightText: 2022-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "esp_err.h"
#include "esp_log.h"
#include "esp_check.h"
#include "driver/i2c.h"
#include "driver/gpio.h"
#include "driver/spi_master.h"
#include "esp_lcd_panel_io.h"
#include "esp_lcd_panel_vendor.h"
#include "esp_lcd_panel_ops.h"
#include "esp_lvgl_port.h"
#include "esp_lcd_ili9341.h"

#include "esp_lcd_touch_xpt2046.h"


#include "lv_demos.h"
#include "lv_examples.h"
#include "gui_app/ui.h"

/* LCD size */
#define EXAMPLE_LCD_H_RES   (240)
#define EXAMPLE_LCD_V_RES   (320)

/* LCD settings */
#define EXAMPLE_LCD_SPI_NUM         (SPI3_HOST)
#define EXAMPLE_LCD_PIXEL_CLK_HZ    (40 * 1000 * 1000)
#define EXAMPLE_LCD_CMD_BITS        (8)
#define EXAMPLE_LCD_PARAM_BITS      (8)
#define EXAMPLE_LCD_COLOR_SPACE     (ESP_LCD_COLOR_SPACE_BGR)
#define EXAMPLE_LCD_BITS_PER_PIXEL  (16)
#define EXAMPLE_LCD_DRAW_BUFF_DOUBLE (1)
#define EXAMPLE_LCD_DRAW_BUFF_HEIGHT (50)
#define EXAMPLE_LCD_BL_ON_LEVEL     (1)

/* LCD pins */
#define EXAMPLE_LCD_GPIO_SCLK       (GPIO_NUM_7)
#define EXAMPLE_LCD_GPIO_MOSI       (GPIO_NUM_6)
#define EXAMPLE_LCD_GPIO_RST        (GPIO_NUM_48)
#define EXAMPLE_LCD_GPIO_DC         (GPIO_NUM_4)
#define EXAMPLE_LCD_GPIO_CS         (GPIO_NUM_5)
#define EXAMPLE_LCD_GPIO_BL         (GPIO_NUM_45)

/* Touch settings */
#define EXAMPLE_TOUCH_SPI_NUM       (SPI2_HOST)
#define EXAMPLE_TOUCH_SPI_CLK_HZ    (40 * 1000 * 1000)

/* LCD touch pins */
#define EXAMPLE_TOUCH_GPIO_SCLK     (GPIO_NUM_42) 
#define EXAMPLE_TOUCH_GPIO_MOSI     (GPIO_NUM_40)   //DIN
#define EXAMPLE_TOUCH_GPIO_MISO     (GPIO_NUM_41)   //DO
#define EXAMPLE_TOUCH_GPIO_CS       (GPIO_NUM_38) 
#define EXAMPLE_TOUCH_GPIO_IRQ      (GPIO_NUM_37) 

/*
    FSPI通常被称为SPI0，HSPI通常被称为SPI2，而VSPI则是SPI3
    VSPI（Virtual SPI）
    MOSI: GPIO23
    MISO: GPIO19
    SCK: GPIO18
    CS: GPIO5（可选）

    HSPI（Hardware SPI）
    MOSI: GPIO13
    MISO: GPIO12
    SCK: GPIO14
    CS: GPIO15（可选）

*/


static const char *TAG = "EXAMPLE";

// // LVGL image declare
// LV_IMG_DECLARE(esp_logo)

/* LCD IO and panel */
static esp_lcd_panel_io_handle_t lcd_io = NULL;
static esp_lcd_panel_handle_t lcd_panel = NULL;
static esp_lcd_touch_handle_t touch_handle = NULL;

/* LVGL display and touch */
static lv_display_t *lvgl_disp = NULL;
static lv_indev_t *lvgl_touch_indev = NULL;

static esp_err_t app_lcd_init(void)
{
    esp_err_t ret = ESP_OK;

    /* LCD backlight */
    gpio_config_t bk_gpio_config = {
        .mode = GPIO_MODE_OUTPUT,
        .pin_bit_mask = 1ULL << EXAMPLE_LCD_GPIO_BL
    };
    ESP_ERROR_CHECK(gpio_config(&bk_gpio_config));

    /* LCD initialization */
    ESP_LOGD(TAG, "Initialize SPI bus");
    const spi_bus_config_t buscfg = {
        .sclk_io_num = EXAMPLE_LCD_GPIO_SCLK,
        .mosi_io_num = EXAMPLE_LCD_GPIO_MOSI,
        .miso_io_num = GPIO_NUM_NC,
        .quadwp_io_num = GPIO_NUM_NC,
        .quadhd_io_num = GPIO_NUM_NC,
        .max_transfer_sz = EXAMPLE_LCD_H_RES * EXAMPLE_LCD_DRAW_BUFF_HEIGHT * sizeof(uint16_t),
    };
    ESP_RETURN_ON_ERROR(spi_bus_initialize(EXAMPLE_LCD_SPI_NUM, &buscfg, SPI_DMA_CH_AUTO), TAG, "SPI init failed");

    ESP_LOGD(TAG, "Install panel IO");
    const esp_lcd_panel_io_spi_config_t io_config = {
        .dc_gpio_num = EXAMPLE_LCD_GPIO_DC,
        .cs_gpio_num = EXAMPLE_LCD_GPIO_CS,
        .pclk_hz = EXAMPLE_LCD_PIXEL_CLK_HZ,
        .lcd_cmd_bits = EXAMPLE_LCD_CMD_BITS,
        .lcd_param_bits = EXAMPLE_LCD_PARAM_BITS,
        .spi_mode = 0,
        .trans_queue_depth = 10,
    };
    ESP_GOTO_ON_ERROR(esp_lcd_new_panel_io_spi((esp_lcd_spi_bus_handle_t)EXAMPLE_LCD_SPI_NUM, &io_config, &lcd_io), err, TAG, "New panel IO failed");

    ESP_LOGD(TAG, "Install LCD driver");
    const esp_lcd_panel_dev_config_t panel_config = {
        .reset_gpio_num = EXAMPLE_LCD_GPIO_RST,
        .color_space = EXAMPLE_LCD_COLOR_SPACE,
        .bits_per_pixel = EXAMPLE_LCD_BITS_PER_PIXEL,
    };
    ESP_GOTO_ON_ERROR(esp_lcd_new_panel_ili9341(lcd_io, &panel_config, &lcd_panel), err, TAG, "New panel failed");

    esp_lcd_panel_reset(lcd_panel);
    esp_lcd_panel_init(lcd_panel);
    esp_lcd_panel_mirror(lcd_panel, true, false);
    esp_lcd_panel_disp_on_off(lcd_panel, true);

    /* LCD backlight on */
    ESP_ERROR_CHECK(gpio_set_level(EXAMPLE_LCD_GPIO_BL, EXAMPLE_LCD_BL_ON_LEVEL));

    return ret;

err:
    if (lcd_panel) {
        esp_lcd_panel_del(lcd_panel);
    }
    if (lcd_io) {
        esp_lcd_panel_io_del(lcd_io);
    }
    spi_bus_free(EXAMPLE_LCD_SPI_NUM);
    return ret;
}

static esp_err_t app_touch_init(void)
{

    /* Initilize SPI */
    const spi_bus_config_t buscfg = {
        .sclk_io_num = EXAMPLE_TOUCH_GPIO_SCLK,
        .mosi_io_num = EXAMPLE_TOUCH_GPIO_MOSI,
        .miso_io_num = EXAMPLE_TOUCH_GPIO_MISO,
        .quadwp_io_num = GPIO_NUM_NC,
        .quadhd_io_num = GPIO_NUM_NC,
        .max_transfer_sz = EXAMPLE_LCD_H_RES * EXAMPLE_LCD_DRAW_BUFF_HEIGHT * sizeof(uint16_t),
        // .max_transfer_sz = EXAMPLE_LCD_H_RES * 80 * sizeof(uint16_t),
    };
    ESP_RETURN_ON_ERROR(spi_bus_initialize(EXAMPLE_TOUCH_SPI_NUM, &buscfg, SPI_DMA_CH_AUTO), TAG, "SPI init failed");


    /* Initialize touch HW */

    const esp_lcd_touch_config_t tp_cfg = {
        .x_max = EXAMPLE_LCD_H_RES,
        .y_max = EXAMPLE_LCD_V_RES,
        .rst_gpio_num = GPIO_NUM_NC, // Shared with LCD reset
        .int_gpio_num = GPIO_NUM_NC,
        .levels = {
            .reset = 0,
            .interrupt = 0,
        },
        .flags = {
            .swap_xy = 0,
            .mirror_x = 0,
            .mirror_y = 1,
        },
    };
    esp_lcd_panel_io_handle_t tp_io_handle = NULL;
    const esp_lcd_panel_io_spi_config_t tp_io_config = ESP_LCD_TOUCH_IO_SPI_XPT2046_CONFIG(EXAMPLE_TOUCH_GPIO_CS);
    ESP_RETURN_ON_ERROR(esp_lcd_new_panel_io_spi((esp_lcd_spi_bus_handle_t)EXAMPLE_TOUCH_SPI_NUM, &tp_io_config, &tp_io_handle), TAG, "");
    return esp_lcd_touch_new_spi_xpt2046(tp_io_handle, &tp_cfg, &touch_handle);
}

static esp_err_t app_lvgl_init(void)
{
    /* Initialize LVGL */
    const lvgl_port_cfg_t lvgl_cfg = {
        .task_priority = 4,         /* LVGL task priority */
        .task_stack = 4096,         /* LVGL task stack size */
        .task_affinity = -1,        /* LVGL task pinned to core (-1 is no affinity) */
        .task_max_sleep_ms = 500,   /* Maximum sleep in LVGL task */
        .timer_period_ms = 5        /* LVGL timer tick period in ms */
    };
    ESP_RETURN_ON_ERROR(lvgl_port_init(&lvgl_cfg), TAG, "LVGL port initialization failed");

    /* Add LCD screen */
    ESP_LOGD(TAG, "Add LCD screen");
    const lvgl_port_display_cfg_t disp_cfg = {
        .io_handle = lcd_io,
        .panel_handle = lcd_panel,
        .buffer_size = EXAMPLE_LCD_H_RES * EXAMPLE_LCD_DRAW_BUFF_HEIGHT,
        .double_buffer = EXAMPLE_LCD_DRAW_BUFF_DOUBLE,
        .hres = EXAMPLE_LCD_H_RES,
        .vres = EXAMPLE_LCD_V_RES,
        .monochrome = false,
#if LVGL_VERSION_MAJOR >= 9
        .color_format = LV_COLOR_FORMAT_RGB565,
#endif
        .rotation = {
            .swap_xy = false,
            .mirror_x = true,
            .mirror_y = false,
        },
        .flags = {
            .buff_dma = true,
#if LVGL_VERSION_MAJOR >= 9
            .swap_bytes = true,
#endif
        }
    };
    lvgl_disp = lvgl_port_add_disp(&disp_cfg);

    /* Add touch input (for selected screen) */
    const lvgl_port_touch_cfg_t touch_cfg = {
        .disp = lvgl_disp,
        .handle = touch_handle,
    };
    lvgl_touch_indev = lvgl_port_add_touch(&touch_cfg);

    return ESP_OK;
}

static void _app_button_cb(lv_event_t *e)
{
    lv_disp_rotation_t rotation = lv_disp_get_rotation(lvgl_disp);
    rotation++;
    if (rotation > LV_DISPLAY_ROTATION_270) {
        rotation = LV_DISPLAY_ROTATION_0;
    }

    /* LCD HW rotation */
    lv_disp_set_rotation(lvgl_disp, rotation);
}

static void app_main_display(void)
{
    lv_obj_t *scr = lv_scr_act();

    /* Task lock */
    lvgl_port_lock(0);

//     /* Your LVGL objects code here .... */

    // /* Create image */
    // lv_obj_t *img_logo = lv_img_create(scr);
    // lv_img_set_src(img_logo, &esp_logo);
    // lv_obj_align(img_logo, LV_ALIGN_TOP_MID, 0, 20);

//     /* Label */
//     lv_obj_t *label = lv_label_create(scr);
//     lv_obj_set_width(label, EXAMPLE_LCD_H_RES);
//     lv_obj_set_style_text_align(label, LV_TEXT_ALIGN_CENTER, 0);
// #if LVGL_VERSION_MAJOR == 8
//     lv_label_set_recolor(label, true);
//     lv_label_set_text(label, "#FF0000 "LV_SYMBOL_BELL" Hello world Espressif and LVGL "LV_SYMBOL_BELL"#\n#FF9400 "LV_SYMBOL_WARNING" For simplier initialization, use BSP "LV_SYMBOL_WARNING" #");
// #else
//     lv_label_set_text(label, LV_SYMBOL_BELL" Hello world Espressif and LVGL "LV_SYMBOL_BELL"\n "LV_SYMBOL_WARNING" For simplier initialization, use BSP "LV_SYMBOL_WARNING);
// #endif
//     lv_obj_align(label, LV_ALIGN_CENTER, 0, 20);

//     /* Button */
//     lv_obj_t *btn = lv_btn_create(scr);
//     label = lv_label_create(btn);
//     lv_label_set_text_static(label, "Rotate screen");
//     lv_obj_align(btn, LV_ALIGN_BOTTOM_MID, 0, -30);
//     lv_obj_add_event_cb(btn, _app_button_cb, LV_EVENT_CLICKED, NULL);

    void lv_demo_music(void);
    lv_demo_music();

    /* Task unlock */
    lvgl_port_unlock();
}




void app_main(void)
{
    /* LCD HW initialization */
    ESP_ERROR_CHECK(app_lcd_init());

    /* Touch initialization */
    ESP_ERROR_CHECK(app_touch_init());

    /* LVGL initialization */
    ESP_ERROR_CHECK(app_lvgl_init());

    /* Show LVGL objects */
    app_main_display();
    
}



