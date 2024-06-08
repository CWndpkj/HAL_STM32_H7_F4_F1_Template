# 定义通用编译器参数；
# ${MCPU_FLAGS}   处理器内核信息
# ${VFP_FLAGS}    浮点运算单元类型
# ${SYSTEM_PATH}  编译器头文件路径
# 以上参数与对应的MCU平台有关，在target/*.cmake中被设置

SET(CFCOMMON "${MCPU_FLAGS} ${VFP_FLAGS} ${SYSTEM_PATH} --specs=nano.specs -u _printf_float --specs=nosys.specs -Wall -fmessage-length=0 -ffunction-sections -fdata-sections")

# 定义最快运行速度发行模式的编译参数
SET(CMAKE_C_FLAGS_RELEASE "-Os ${CFCOMMON}")
SET(CMAKE_CXX_FLAGS_RELEASE "-Os -g ${CFCOMMON} -fno-exceptions")
SET(CMAKE_ASM_FLAGS_RELEASE "${MPU_FLAGS} ${VFP_FLAGS} -x assembler-with-cpp")

# 定义最小尺寸且包含调试信息的编译参数；
SET(CMAKE_C_FLAGS_RELWITHDEBINFO "-Os -g  ${CFCOMMON}")
SET(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-Os -g  ${CFCOMMON} -fno-exceptions")
SET(CMAKE_ASM_FLAGS_RELWITHDEBINFO "${MCPU_FLAGS} ${VFP_FLAGS} -x assembler-with-cpp")

# 定义最小尺寸的编译参数；
SET(CMAKE_C_FLAGS_MINSIZEREL "-Os  ${CFCOMMON}")
SET(CMAKE_CXX_FLAGS_MINSIZEREL "-Os  ${CFCOMMON} -fno-exceptions")
SET(CMAKE_ASM_FLAGS_MINSIZEREL "${MCPU_FLAGS} ${VFP_FLAGS} -x assembler-with-cpp")

# 定义调试模式编译参数；
SET(CMAKE_C_FLAGS_DEBUG "-O0 -g  ${CFCOMMON}")
SET(CMAKE_CXX_FLAGS_DEBUG "-O0 -g  ${CFCOMMON} -fno-exceptions")
SET(CMAKE_ASM_FLAGS_DEBUG "${MCPU_FLAGS} ${VFP_FLAGS} -x assembler-with-cpp")

IF ("${CMAKE_BUILD-TYPE}" STREQUAL "Release")
    MESSAGE(STATUS "****** Maximum optimization for speed ******")
ELSEIF ("${CMAKE_BUILD_TYPE}" STREQUAL "RelWithDebInfo")
    MESSAGE(STATUS "******Maximum optimization for size,debug info included******")
ELSEIF ("${CMAKE_BUILD_TYPE}" STREQUAL "MinSizeRel")
    MESSAGE(STATUS "******Maximum optimization for size******")
ELSE ()
    MESSAGE(STATUS "******No optimization,debug info included******")
ENDIF ()