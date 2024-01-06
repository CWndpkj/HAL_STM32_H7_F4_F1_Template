#ifndef __AHT20_H
#define __AHT20_H

#include "stm32f4xx_hal.h"
#define AHT20_ADDR 0x70

#define	AHT20_STATUS_REG		0x00	//状态字 寄存器地址
#define	AHT20_INIT_REG			0xBE	//初始化 寄存器地址
#define	AHT20_SoftReset			0xBA	//软复位 单指令
#define	AHT20_TrigMeasure_REG	0xAC	//触发测量 寄存器地址

typedef struct  {
    float Humidity;
    float Temperature;
} AHT20_value_t;
uint8_t AHT20_Init();
uint8_t AHT20_StartAquire();
uint8_t ATH20_Read_Status();
uint8_t AHT20_GetValue(AHT20_value_t *value);
#endif // !_AH