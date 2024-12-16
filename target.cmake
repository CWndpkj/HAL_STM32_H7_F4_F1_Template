#设置平台类型
SET(PLATFORM "cortex_m4")
if (${PLATFORM} STREQUAL "cortex_m7")
    INCLUDE("${CMAKE_CURRENT_SOURCE_DIR}/CMake/target/cortex_m7.cmake")
elseif (${PLATFORM} STREQUAL "cortex_m4")
    INCLUDE("${CMAKE_CURRENT_SOURCE_DIR}/CMake/target/cortex_m4.cmake")
elseif (${PLATFORM} STREQUAL "cortex_m3")
    INCLUDE("${CMAKE_CURRENT_SOURCE_DIR}/CMake/target/cortex_m3.cmake")
elseif (${PLATFORM} STREQUAL "cortex_m0")
    INCLUDE("${CMAKE_CURRENT_SOURCE_DIR}/CMake/target/cortex_m0.cmake")
else ()
    MESSAGE(FATAL_ERROR "PLATFORM is not supported")
endif ()

# 针对特定平台的编译flags
include("CMake/flags.cmake")

if (${PLATFORM} STREQUAL "cortex_m7")
    #指定链接文件
    SET(LINKER_SCRIPT "${PATH_WORKSPACE_ROOT}/LinkScripts/STM32H7/stm32h750xbh6_external_flash.ld")
    #指定启动文件
    SET(STARTUP_ASM "${PATH_WORKSPACE_ROOT}/Core/STM32H7/StartUp/startup_stm32h750xx.s")
elseif (${PLATFORM} STREQUAL "cortex_m4")
    SET(LINKER_SCRIPT "${PATH_WORKSPACE_ROOT}/LinkScripts/STM32F4/STM32F407ZGTx_FLASH.ld")
    SET(STARTUP_ASM "${PATH_WORKSPACE_ROOT}/Core/STM32F4/StartUp/startup_stm32f407xx.s")
elseif (${PLATFORM} STREQUAL "cortex_m3")
    SET(LINKER_SCRIPT "${PATH_WORKSPACE_ROOT}/LinkScripts/STM32F1/STM32F103XB_FLASH.ld")
    SET(STARTUP_ASM "${PATH_WORKSPACE_ROOT}/Core/STM32F1/StartUp/startup_stm32f103xb.s")
elseif (${PLATFORM} STREQUAL "cortex_m0")
    #SET(LINKER_SCRIPT "${PATH_WORKSPACE_ROOT}/toolchain/linkscripts/stm32h750xbh6_external_flash.ld")
    #SET(STARTUP_ASM "${PATH_WORKSPACE_ROOT}/System/startup_stm32h750xx.s")
else ()
    MESSAGE(FATAL_ERROR "PLATFORM is not supported")
endif ()
if (${PLATFORM} STREQUAL "cortex_m7")
    ADD_DEFINITIONS(-DSTM32H750xx -DHSE_VALUE=25000000)
elseif (${PLATFORM} STREQUAL "cortex_m4")
    ADD_DEFINITIONS(-DSTM32F407xx -DHSE_VALUE=8000000)
elseif (${PLATFORM} STREQUAL "cortex_m3")
    ADD_DEFINITIONS(-DSTM32F103xB -DHSE_VALUE=8000000)
elseif (${PLATFORM} STREQUAL "cortex_m0")
    ADD_DEFINITIONS(-DSTM32??-DHSE_VALUE=8000000)
else ()
    MESSAGE(FATAL_ERROR "PLATFORM is not supported")
endif ()
ADD_DEFINITIONS(
        -DUSE_HAL_DRIVER
        #-DUSING_NON_RTOS=0 # 不使用 RTOS
        -DUSING_FREERTOS=1 # 使用 FreeRTOS
        #-DUSING_THREADX=2 # 使用 Threadx
        #-DUSING_RTOS=USING_NON_RTOS # 选择使用的 RTOS
)