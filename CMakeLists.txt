SET(CMAKE_SYSTEM_NAME Generic)
SET(CMAKE_SYSTEM_VERSION 1)
CMAKE_MINIMUM_REQUIRED(VERSION 3.20)
SET(PATH_WORKSPACE_ROOT ${CMAKE_SOURCE_DIR})
#设置项目的名称
SET(RROJECT_NAME "project")
SET(PROJECT_VERSION 1.0)
#可通过修改下面的值爱修改输出的文件名
SET(OUTPUT_EXE_NAME ${RROJECT_NAME})
include("CMake/toolchain/arm-none-eabi.cmake")

PROJECT(
        ${RROJECT_NAME}
        VERSION ${PROJECT_VERSION}
        LANGUAGES C CXX ASM
)

include("target.cmake")
# #################################################################
# 1、工程信息
# #################################################################

MESSAGE(STATUS "*** Building project: ${CMAKE_PROJECT_NAME} ,Version: ${CMAKE_PROJECT_VERSION}***")

#项目底层公共头文件路径
INCLUDE_DIRECTORIES("${PATH_WORKSPACE_ROOT}/Driver/CMSIS/Core/Include")

# ###############################################
# 3、预定义宏；
# ###############################################

# TODO:采用更灵活的方式来管理平台类型以支持更多的MCU型号，以及不同的HSE频率
#平台相关定义

# ##################################################
# 4、差异化构建配置；
# ##################################################

#设置一个Debug调试选项
OPTION(OPEN_LOG_OMN_DEBUG "Open log output for debug" OFF)

#优化级别的差异配置
IF ("${CMAKE_BUILD_TYPE}" STREQUAL "Release")
    ADD_DEFINITIONS()
ELSEIF ("${CMAKE_BUILD_TYPE}" STREQUAL "RelWithDebInfo")
    ADD_DEFINITIONS()
ELSEIF ("${CMAKE_BUILD_TYPE}" STREQUAL "MinSizeRel")
    ADD_DEFINITIONS()
ELSE ()
    IF (OPEN_LOG_OMN_DEBUG)
        ADD_DEFINITIONS(-DLOG_BACKEND=LOG_BACKEND_NONE)
    ELSE ()
        ADD_DEFINITIONS(-DLOG_BACKEND=LOG_BACKEND_NONE)
    ENDIF ()
ENDIF ()

MESSAGE(STATUS "*** Build for ${CMAKE_BUILD_TYPE} ***")

# ###########################################################
# 5、设置文件输出路径；
#############################################################

#设置库的输出路径

SET(LIBRARY_OUTPUT_PATH "${PATH_WORKSPACE_ROOT}/build/obj")

SET(ELF_FILE "${PATH_WORKSPACE_ROOT}/build/${OUTPUT_EXE_NAME}.elf")
SET(HEX_FILE "${PATH_WORKSPACE_ROOT}/build/${OUTPUT_EXE_NAME}.hex")
SET(BIN_FILE "${PATH_WORKSPACE_ROOT}/build/${OUTPUT_EXE_NAME}.bin")

# ###################################################################
# 6、组织公共库源文件；
# ####################################################################


# ####################################################################
# 7、组织用户源文件；
# ####################################################################

# #####################################################################
# 8、编译、连接，生成可执行文件
# #####################################################################

# 定义连接器参数； --gc-sections：指示链接器去掉不用的 section
#Og -Wall -g --specs=nosys.specs --specs=nano.specs -u _printf_float -ffunction-sections -fdata-sections
SET(CMAKE_EXE_LINKER_FLAGS
        "${CMAKE_EXE_LINKER_FLAGS} -T ${LINKER_SCRIPT} -Wl,-Map=${PROJECT_BINARY_DIR}/${OUTPUT_EXE_NAME}.map -Wl,--gc-sections,--print-memory-usage"
)
#生成可执行文件
ADD_EXECUTABLE(${PROJECT_NAME} ${COMMON_SERVICES_SOURCE} ${USER_SOURCE} ${LINKER_SCRIPT} ${STARTUP_ASM})
SET_TARGET_PROPERTIES(${PROJECT_NAME} PROPERTIES SUFFIX ".elf")
#添加依赖
if (${PLATFORM} STREQUAL "cortex_m7")
    ADD_SUBDIRECTORY(${PATH_WORKSPACE_ROOT}/Driver/STM32H7xx_HAL_Driver ${LIBRARY_OUTPUT_PATH}/Driver)
    ADD_SUBDIRECTORY(${PATH_WORKSPACE_ROOT}/Core/STM32H7 ${LIBRARY_OUTPUT_PATH}/Core)
elseif (${PLATFORM} STREQUAL "cortex_m4")
    ADD_SUBDIRECTORY(${PATH_WORKSPACE_ROOT}/Driver/STM32F4xx_HAL_Driver ${LIBRARY_OUTPUT_PATH}/Driver)
    ADD_SUBDIRECTORY(${PATH_WORKSPACE_ROOT}/Core/STM32F4 ${LIBRARY_OUTPUT_PATH}/Core)
elseif (${PLATFORM} STREQUAL "cortex_m3")
    ADD_SUBDIRECTORY(${PATH_WORKSPACE_ROOT}/Driver/STM32F1xx_HAL_Driver/ ${LIBRARY_OUTPUT_PATH}/Driver)
    ADD_SUBDIRECTORY(${PATH_WORKSPACE_ROOT}/Core/STM32F1 ${LIBRARY_OUTPUT_PATH}/Core)
elseif (${PLATFORM} STREQUAL "cortex_m0")
    #ADD_SUBDIRECTORY(${PATH_WORKSPACE_ROOT}/Driver/STM32H7xx_HAL_Driver/ ${LIBRARY_OUTPUT_PATH}/Driver)
    #ADD_SUBDIRECTORY(${PATH_WORKSPACE_ROOT}/System/ ${LIBRARY_OUTPUT_PATH}/System)
else ()
    MESSAGE(FATAL_ERROR "PLATFORM is not supported")
endif ()
ADD_SUBDIRECTORY(${PATH_WORKSPACE_ROOT}/User ${LIBRARY_OUTPUT_PATH}/User)
ADD_SUBDIRECTORY(${PATH_WORKSPACE_ROOT}/BSP ${LIBRARY_OUTPUT_PATH}/BSP)
ADD_SUBDIRECTORY(${PATH_WORKSPACE_ROOT}/App ${LIBRARY_OUTPUT_PATH}/App)
ADD_SUBDIRECTORY(${PATH_WORKSPACE_ROOT}/Middlewares ${LIBRARY_OUTPUT_PATH}/Middlewares)
ADD_SUBDIRECTORY(${PATH_WORKSPACE_ROOT}/Utils ${LIBRARY_OUTPUT_PATH}/Utils)
# ###################################################################
# 9、生成 hex 和 bin 文件
# ###################################################################
ADD_CUSTOM_COMMAND(
        TARGET "${PROJECT_NAME}"
        POST_BUILD
        #生成.hex .bin
        COMMAND ${OBJCOPY} -Obinary "${PROJECT_NAME}.elf" "${OUTPUT_EXE_NAME}.bin"
        COMMAND ${OBJCOPY} -Oihex "${PROJECT_NAME}.elf" "${OUTPUT_EXE_NAME}.hex"
        #显示大小
        COMMAND ${SIZE} --format=berkeley "${PROJECT_NAME}.elf"
        #转换为.elf
        #objcopy -I ihex input.hex -O elf32-littlearm output.elf
        #        COMMAND ${OBJCOPY} -I ihex "${PROJECT_NAME}" -O elf32-littlearm "${OUTPUT_EXE_NAME}.elf"
        #        COMMENT "hex to elf"

        #        显示.elf的文件信息
        #        objdump -h output.elf
        #        COMMAND ${OBJDUMP} -h "${OUTPUT_EXE_NAME}.elf"
        #        COMMENT "objdum result"
)