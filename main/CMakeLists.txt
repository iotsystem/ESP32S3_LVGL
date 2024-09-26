# set(UI_DIR ./gui_app)
# file(GLOB_RECURSE UI_SRCS ${UI_DIR}/*.c)

# idf_component_register(SRCS "main.c" ${UI_SRCS}
#                         INCLUDE_DIRS "."  ${UI_DIR}                        
#                       )

                      
idf_component_register(
    SRCS "main.c"
    INCLUDE_DIRS "."                        
)
