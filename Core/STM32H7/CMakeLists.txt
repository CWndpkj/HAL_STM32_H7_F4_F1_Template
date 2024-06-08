MESSAGE(STATUS "current directory:${CMAKE_CURRENT_SOURCE_DIR}")
# 要连接到构建目标的源文件；
file(GLOB ALL_SUB_DIRS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "*")
foreach (SUB_DIR ${ALL_SUB_DIRS})
    if (IS_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/${SUB_DIR})
        LIST(APPEND INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/${SUB_DIR})
    endif ()
endforeach ()
# TODO:排除不需要包含的头文件路径
list(REMOVE_ITEM ${INCLUDE_DIRS})
message(STATUS "include directories:${INCLUDE_DIRS}")

file(GLOB_RECURSE ADD_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/*.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
list(APPEND SRCS ${ADD_SRCS})
# 默认排除以_template.c文件结尾的源文件
list(FILTER SRCS EXCLUDE REGEX "._template\\.c$")
# TODO:添加需要被排除的源文件
list(REMOVE_ITEM ${SRCS})
message(STATUS "add srcs:${SRCS}")


TARGET_SOURCES(
        ${PROJECT_NAME}
        PRIVATE
        ${SRCS}
)
# 将模块头文件路径添加到目标；
TARGET_INCLUDE_DIRECTORIES(${PROJECT_NAME}
        PUBLIC
        ${INCLUDE_DIRS}
)