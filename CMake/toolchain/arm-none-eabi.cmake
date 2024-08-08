#编译器参数
#arm_none_eabi_toolchain的bin路径
#查找arm_none_eabi_gcc的路径
SET(TOOL_CHAIN_PREFIX "arm-none-eabi-")
FIND_PROGRAM(ARM_NONE_EABI_GCC_EXECUTABLE NAMES ${TOOL_CHAIN_PREFIX}gcc)
if (ARM_NONE_EABI_GCC_EXECUTABLE)
    SET(CMAKE_C_COMPILER "${ARM_NONE_EABI_GCC_EXECUTABLE}")
else ()
    MESSAGE(FATAL_ERROR "arm-none-eabi-gcc not found ,check whether your toolchain path has been added to PATH")
endif ()
#查找arm_none_eabi_g++的路径
FIND_PROGRAM(ARM_NONE_EABI_GXX_EXECUTABLE NAMES ${TOOL_CHAIN_PREFIX}g++)
if (ARM_NONE_EABI_GXX_EXECUTABLE)
    SET(CMAKE_CXX_COMPILER "${ARM_NONE_EABI_GXX_EXECUTABLE}")
else ()
    MESSAGE(WARNING "arm-none-eabi-g++ not found ,check whether your toolchain path has been added to PATH,use gcc instead")
endif ()

#设置相关工具路径
get_filename_component(ARM_NONE_EABI_TOOLCHAIN_PATH ${ARM_NONE_EABI_GCC_EXECUTABLE} DIRECTORY)
#SET(ARM_NONE_EABI_TOOLCHAIN_PATH "D:/install/arm-none-eabi/bin")
#arm_none_eabi_include路径
#D:\install\arm-none-eabi\lib\gcc\arm-none-eabi\12.3.1\include
SET(ARM_NONE_EABI_INCLUDE_PATH ${ARM_NONE_EABI_TOOLCHAIN_PATH}/../lib/gcc/arm-none-eabi/12.3.1/include)
SET(AS "${ARM_NONE_EABI_TOOLCHAIN_PATH}/${TOOL_CHAIN_PREFIX}as")
SET(AR "${ARM_NONE_EABI_TOOLCHAIN_PATH}/${TOOL_CHAIN_PREFIX}ar")
SET(OBJCOPY "${ARM_NONE_EABI_TOOLCHAIN_PATH}/${TOOL_CHAIN_PREFIX}objcopy")
SET(SIZE "${ARM_NONE_EABI_TOOLCHAIN_PATH}/${TOOL_CHAIN_PREFIX}size")
SET(OBJDUMP "${ARM_NONE_EABI_TOOLCHAIN_PATH}/${TOOL_CHAIN_PREFIX}objdump")

#使用的c语法
SET(CMAKE_C_STANDARD 11)
#使用的c++语法
SET(CMAKE_CXX_STANDARD 11)
#指定生成compile_commands.json
SET(CMAKE_EXPORT_COMPILE_COMMANDS True)
#彩色日志输出
SET(CMAKE_COLOR_DIAGNOSTICS True)
#设置CMake在查找可执行程序时不使用交叉编译环境的根路径。这意味着CMake将使用默认的系统路径来查找可执行程序。
SET(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
#设置CMake在查找库文件时只使用交叉编译环境的根路径。这意味着CMake将只在交叉编译环境的根路径中查找库文件，而不会使用默认的系统路径。
SET(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
#设置CMake在查找头文件时只使用交叉编译环境的根路径。这意味着CMake将只在交叉编译环境的根路径中查找头文件，而不会使用默认的系统路径。
SET(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
#设置CMake在查找包时只使用交叉编译环境的根路径。这意味着CMake将只在交叉编译环境的根路径中查找包，而不会使用默认的系统路径。
SET(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)

#指定生成库为静态库
# this makes the test compiles use static library option so that we don't need to pre-set linker flags and scripts
SET(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)
#设置系统包含路径
MESSAGE(STATUS "getting gcc system include path...")
execute_process(
        COMMAND echo |
        #TODO:windows and linux suffix differece
        COMMAND arm-none-eabi-gcc -E -Wp,-v -
        ERROR_VARIABLE STDERR
)
STRING(REGEX MATCH "#include \"...\" search starts here:(.*)#include <...> search starts here:" MATCHED_CONTENT ${STDERR})
STRING(REPLACE "\\" "/" SYS_INCLUDE_DIRS ${CMAKE_MATCH_1})
STRING(REPLACE "\n" ";" SYS_INCLUDE_DIRS ${SYS_INCLUDE_DIRS})
foreach (INCLUDE_PATH ${SYS_INCLUDE_DIRS})
    STRING(APPEND SYSTEM_PATH "-isystem ${INCLUDE_PATH} ")
endforeach ()
LIST(APPEND SYSTEM_PATH ${SYS_INCLUDE_DIRS})
STRING(REGEX MATCH "#include <...> search starts here:(.*)End of search list."
        MATCHED_CONTENT ${STDERR})
STRING(REPLACE "\\" "/" SYS_INCLUDE_DIRS ${CMAKE_MATCH_1})
STRING(REPLACE "\n" ";" SYS_INCLUDE_DIRS ${SYS_INCLUDE_DIRS})
foreach (INCLUDE_PATH ${SYS_INCLUDE_DIRS})
    STRING(APPEND SYSTEM_PATH "-isystem ${INCLUDE_PATH} ")
endforeach ()
message("system include path:${SYSTEM_PATH}")
