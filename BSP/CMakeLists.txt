# 要连接到构建目标的源文件；
file(GLOB ALL_SUB_DIRS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "*")
foreach (SUB_DIR ${ALL_SUB_DIRS})
    if (IS_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/${SUB_DIR})
        LIST(APPEND INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/${SUB_DIR})
    endif ()
endforeach ()
message(STATUS "include directories:${INCLUDE_DIRS}")

file(GLOB_RECURSE ADD_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/*.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
list(APPEND SRCS ${ADD_SRCS})
list(FILTER SRCS EXCLUDE REGEX "._template\\.c$")
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