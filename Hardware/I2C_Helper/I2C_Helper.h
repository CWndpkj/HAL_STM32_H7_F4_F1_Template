#ifndef __I2C_HELPER_H
#define __I2C_HELPER_H

#include "stm32f407xx.h"
#include "stm32f4xx_hal.h"

/**
 *
 * @brief stm32f407zgt6 对应的I2C引脚资源
 * I2C1  SDA PB7,PB9;SCL PB6,PB8
 * I2C2  SDA PF0,PB11;SCL PF1,SCL PB10
 * I2C3  SDA PC9,SCL PA8
 */

// 配置结构体
// 对应上述的资源，I2Cx可以为 i2C1,I2C2,I2C3 ,SDA和SCL见上述资源表
enum I2C_ID {
    I2C_1 = 0x1,
    I2C_2 = 0x2,
    I2C_3 = 0x4,
};

typedef struct  {
    enum I2C_ID I2C_x;
    GPIO_TypeDef*SDA_Port;
    GPIO_TypeDef*SCL_Port;
    uint16_t SDA_Pin;
    uint16_t SCL_Pin;
} I2C_DEV_t;

void I2C_Helper_Init( I2C_HandleTypeDef* i2c_handler, I2C_DEV_t* i2c_dev );
void I2C_Helper_FailRecover( I2C_DEV_t* i2c_dev );

#endif  // !__I2C_HELPER_H