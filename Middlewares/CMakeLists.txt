ADD_SUBDIRECTORY(${CMAKE_CURRENT_LIST_DIR}/U8G2 ${LIBRARY_OUTPUT_PATH}/U8G2)
ADD_SUBDIRECTORY(${CMAKE_CURRENT_LIST_DIR}/FreeRTOS ${LIBRARY_OUTPUT_PATH}/FreeRTOS)
ADD_SUBDIRECTORY(${CMAKE_CURRENT_LIST_DIR}/lvgl ${LIBRARY_OUTPUT_PATH}/lvgl)
TARGET_LINK_LIBRARIES(${PROJECT_NAME} freertos_kernel)
TARGET_LINK_LIBRARIES(${PROJECT_NAME} lvgl)
TARGET_LINK_LIBRARIES(${PROJECT_NAME} lvgl_demos)
TARGET_LINK_LIBRARIES(lvgl freertos_kernel)