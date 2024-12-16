# 要连接到构建目标的源文件；
TARGET_SOURCES(
    ${PROJECT_NAME}
    PRIVATE
    #Begin target source
    ${CMAKE_CURRENT_LIST_DIR}/usmart_config.c
    ${CMAKE_CURRENT_LIST_DIR}/usmart_port.c
    ${CMAKE_CURRENT_LIST_DIR}/usmart_str.c
    ${CMAKE_CURRENT_LIST_DIR}/usmart.c
    #${CMAKE_CURRENT_LIST_DIR}/usmart_data_rcv.c
    #end target sorurce
)

# 将模块头文件路径添加到目标；
TARGET_INCLUDE_DIRECTORIES(${PROJECT_NAME} 
    PUBLIC 
    ${CMAKE_CURRENT_LIST_DIR}
)