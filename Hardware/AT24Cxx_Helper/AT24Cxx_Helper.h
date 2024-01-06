#pragma once
#include "stm32f4xx_hal.h"
#include <stdint.h>
int8_t AT24Cxx_Helper_Init();
int8_t AT24Cxx_Helper_WriteByte(uint8_t Buff,uint16_t Addr);
uint8_t AT24Cxx_Helper_ReadByte(uint16_t Addr);
int8_t AT24Cxx_Helper_WriteLen(uint8_t*pSendBuff,uint16_t Addr,uint16_t WriteLen);
int8_t AT24Cxx_Helper_ReadLen(uint8_t*pRcvBuff,uint16_t Addr,uint16_t ReadLen);