// This file was generated by SquareLine Studio
// SquareLine Studio version: SquareLine Studio 1.4.2
// LVGL version: 9.1.0
// Project name: Smart_Gadget

#include "../ui.h"


// COMPONENT Scrolldots

lv_obj_t * ui_Scrolldots_create(lv_obj_t * comp_parent)
{

    lv_obj_t * cui_Scrolldots;
    cui_Scrolldots = lv_obj_create(comp_parent);
    lv_obj_set_width(cui_Scrolldots, 65);
    lv_obj_set_height(cui_Scrolldots, 12);
    lv_obj_set_x(cui_Scrolldots, 0);
    lv_obj_set_y(cui_Scrolldots, -8);
    lv_obj_set_align(cui_Scrolldots, LV_ALIGN_BOTTOM_MID);
    lv_obj_remove_flag(cui_Scrolldots, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_bg_color(cui_Scrolldots, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(cui_Scrolldots, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_t * cui_d1;
    cui_d1 = lv_obj_create(cui_Scrolldots);
    lv_obj_set_width(cui_d1, 8);
    lv_obj_set_height(cui_d1, 8);
    lv_obj_set_align(cui_d1, LV_ALIGN_LEFT_MID);
    lv_obj_remove_flag(cui_d1, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_radius(cui_d1, 20, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(cui_d1, lv_color_hex(0xB4B6E6), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(cui_d1, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_t * cui_d2;
    cui_d2 = lv_obj_create(cui_Scrolldots);
    lv_obj_set_width(cui_d2, 4);
    lv_obj_set_height(cui_d2, 4);
    lv_obj_set_x(cui_d2, 15);
    lv_obj_set_y(cui_d2, 0);
    lv_obj_set_align(cui_d2, LV_ALIGN_LEFT_MID);
    lv_obj_remove_flag(cui_d2, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_radius(cui_d2, 20, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(cui_d2, lv_color_hex(0xB4B6E6), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(cui_d2, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_t * cui_d3;
    cui_d3 = lv_obj_create(cui_Scrolldots);
    lv_obj_set_width(cui_d3, 4);
    lv_obj_set_height(cui_d3, 4);
    lv_obj_set_x(cui_d3, 25);
    lv_obj_set_y(cui_d3, 0);
    lv_obj_set_align(cui_d3, LV_ALIGN_LEFT_MID);
    lv_obj_remove_flag(cui_d3, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_radius(cui_d3, 20, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(cui_d3, lv_color_hex(0xB4B6E6), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(cui_d3, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_t * cui_d4;
    cui_d4 = lv_obj_create(cui_Scrolldots);
    lv_obj_set_width(cui_d4, 4);
    lv_obj_set_height(cui_d4, 4);
    lv_obj_set_x(cui_d4, 35);
    lv_obj_set_y(cui_d4, 0);
    lv_obj_set_align(cui_d4, LV_ALIGN_LEFT_MID);
    lv_obj_remove_flag(cui_d4, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_radius(cui_d4, 20, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(cui_d4, lv_color_hex(0xB4B6E6), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(cui_d4, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_t * cui_d5;
    cui_d5 = lv_obj_create(cui_Scrolldots);
    lv_obj_set_width(cui_d5, 4);
    lv_obj_set_height(cui_d5, 4);
    lv_obj_set_x(cui_d5, 45);
    lv_obj_set_y(cui_d5, 0);
    lv_obj_set_align(cui_d5, LV_ALIGN_LEFT_MID);
    lv_obj_remove_flag(cui_d5, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_radius(cui_d5, 20, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(cui_d5, lv_color_hex(0xB4B6E6), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(cui_d5, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_t * cui_d6;
    cui_d6 = lv_obj_create(cui_Scrolldots);
    lv_obj_set_width(cui_d6, 4);
    lv_obj_set_height(cui_d6, 4);
    lv_obj_set_x(cui_d6, 55);
    lv_obj_set_y(cui_d6, 0);
    lv_obj_set_align(cui_d6, LV_ALIGN_LEFT_MID);
    lv_obj_remove_flag(cui_d6, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_radius(cui_d6, 20, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(cui_d6, lv_color_hex(0xB4B6E6), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(cui_d6, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_t ** children = lv_malloc(sizeof(lv_obj_t *) * _UI_COMP_SCROLLDOTS_NUM);
    children[UI_COMP_SCROLLDOTS_SCROLLDOTS] = cui_Scrolldots;
    children[UI_COMP_SCROLLDOTS_D1] = cui_d1;
    children[UI_COMP_SCROLLDOTS_D2] = cui_d2;
    children[UI_COMP_SCROLLDOTS_D3] = cui_d3;
    children[UI_COMP_SCROLLDOTS_D4] = cui_d4;
    children[UI_COMP_SCROLLDOTS_D5] = cui_d5;
    children[UI_COMP_SCROLLDOTS_D6] = cui_d6;
    lv_obj_add_event_cb(cui_Scrolldots, get_component_child_event_cb, LV_EVENT_GET_COMP_CHILD, children);
    lv_obj_add_event_cb(cui_Scrolldots, del_component_child_event_cb, LV_EVENT_DELETE, children);
    ui_comp_Scrolldots_create_hook(cui_Scrolldots);
    return cui_Scrolldots;
}

