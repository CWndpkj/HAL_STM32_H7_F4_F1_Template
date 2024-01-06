#include "I2C_Helper.h"

static I2C_HandleTypeDef* handler;
I2C_DEV_t* dev;

/**
 *
 * @brief stm32f407zgt6 对应的I2C引脚资源
 * I2C1  SDA PB7,SCL PB6;SDA PB9,SCL PB8
 * I2C2  SDA PF0,SCL PF1;SDA PB11,SCL PB10
 * I2C3  SDA PC9,SCL PA8;
 */

void HAL_I2C_MspInit( I2C_HandleTypeDef* hi2c )
{
    if ( hi2c->Instance == I2C1 ) {
        __HAL_RCC_I2C1_CLK_ENABLE();
        __HAL_RCC_GPIOB_CLK_ENABLE();
        GPIO_InitTypeDef GPIO_InitStruct;
        GPIO_InitStruct.Alternate = GPIO_AF4_I2C1;
        GPIO_InitStruct.Mode      = GPIO_MODE_AF_OD;
        GPIO_InitStruct.Pull      = GPIO_PULLUP;
        GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
        // I2C1  SDA PB7,PB9;SCL PB6,PB8
        GPIO_InitStruct.Pin = dev->SDA_Pin | dev->SCL_Pin;
        HAL_GPIO_Init( GPIOB, &GPIO_InitStruct );
    }
    else if ( hi2c->Instance == I2C2 ) {
        __HAL_RCC_I2C2_CLK_ENABLE();
        GPIO_InitTypeDef GPIO_InitStruct;
        GPIO_InitStruct.Alternate = GPIO_AF4_I2C2;
        GPIO_InitStruct.Mode      = GPIO_MODE_AF_OD;
        GPIO_InitStruct.Pull      = GPIO_PULLUP;
        GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
        // I2C2  SDA PF0,PB11;SCL PF1,SCL PB10
        GPIO_InitStruct.Pin = dev->SDA_Pin;
        if ( dev->SDA_Port == GPIOF ) {
            __HAL_RCC_GPIOF_CLK_ENABLE();
            HAL_GPIO_Init( GPIOF, &GPIO_InitStruct );
        }
        else if ( dev->SDA_Port == GPIOB ) {
            __HAL_RCC_GPIOB_CLK_ENABLE();
            HAL_GPIO_Init( GPIOB, &GPIO_InitStruct );
        }
        GPIO_InitStruct.Pin = dev->SCL_Pin;
        if ( dev->SCL_Port == GPIOF ) {
            __HAL_RCC_GPIOF_CLK_ENABLE();
            HAL_GPIO_Init( GPIOF, &GPIO_InitStruct );
        }
        else if ( dev->SCL_Port == GPIOB ) {
            __HAL_RCC_GPIOB_CLK_ENABLE();
            HAL_GPIO_Init( GPIOB, &GPIO_InitStruct );
        }
    }
    else if ( hi2c->Instance == I2C3 ) {
        __HAL_RCC_I2C3_CLK_ENABLE();
        GPIO_InitTypeDef GPIO_InitStruct;
        GPIO_InitStruct.Alternate = GPIO_AF4_I2C3;
        GPIO_InitStruct.Mode      = GPIO_MODE_AF_OD;
        GPIO_InitStruct.Pull      = GPIO_PULLUP;
        GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
        // i2C3 SDA PC9,SCL PA8;
        __HAL_RCC_GPIOC_CLK_ENABLE();
        __HAL_RCC_GPIOA_CLK_ENABLE();
        GPIO_InitStruct.Pin = GPIO_PIN_8;
        HAL_GPIO_Init( GPIOA, &GPIO_InitStruct );
        GPIO_InitStruct.Pin = GPIO_PIN_9;
        HAL_GPIO_Init( GPIOC, &GPIO_InitStruct );
    }
}

void I2C_Helper_Init( I2C_HandleTypeDef* i2c_handler, I2C_DEV_t* i2c_dev )
{
    handler = i2c_handler;
    dev     = i2c_dev;
    // 开启GPIO时钟
    handler->Init.AddressingMode  = I2C_ADDRESSINGMODE_7BIT;
    handler->Init.ClockSpeed      = 200000;
    handler->Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
    handler->Init.DutyCycle       = I2C_DUTYCYCLE_16_9;
    handler->Init.GeneralCallMode = I2C_GENERALCALL_ENABLE;
    handler->Init.NoStretchMode   = I2C_NOSTRETCH_DISABLE;
    handler->Init.OwnAddress1     = 0x88;
    handler->Init.OwnAddress2     = 0x89;
    if ( dev->I2C_x == I2C_1 ) {
        handler->Instance = I2C1;
    }
    else if ( dev->I2C_x == I2C_2 ) {
        handler->Instance = I2C2;
    }
    else if ( dev->I2C_x == I2C_3 ) {
        handler->Instance = I2C3;
    }
    HAL_I2C_Init( handler );
}
void I2C_Helper_FailRecover( I2C_DEV_t* i2c_dev )
{
    if ( i2c_dev->I2C_x == I2C_1 ) {
        __HAL_RCC_GPIOB_CLK_ENABLE();
        GPIO_InitTypeDef GPIO_InitStruct;
        GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
        GPIO_InitStruct.Pull  = GPIO_NOPULL;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;

        GPIO_InitStruct.Pin = dev->SCL_Pin | dev->SDA_Pin;
        HAL_GPIO_Init( GPIOB, &GPIO_InitStruct );
        // I2C1  SDA PB7,SCL PB6
        for ( uint8_t i = 0; i < 10; i++ ) {
            HAL_GPIO_WritePin( GPIOB, dev->SCL_Pin, GPIO_PIN_RESET );
            HAL_Delay( 1 );
            HAL_GPIO_WritePin( GPIOB, dev->SCL_Pin, GPIO_PIN_SET );
            HAL_Delay( 1 );
        }
    }
    else if ( i2c_dev->I2C_x == I2C_2 ) {
        GPIO_InitTypeDef GPIO_InitStruct;
        GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
        GPIO_InitStruct.Pull  = GPIO_NOPULL;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
        // I2C2  SDA PF0,PB11;SCL PF1,SCL PB10
        GPIO_InitStruct.Pin = dev->SCL_Pin;
        if ( dev->SCL_Port == GPIOF ) {
            __HAL_RCC_GPIOF_CLK_ENABLE();
            HAL_GPIO_Init( GPIOF, &GPIO_InitStruct );
            for ( uint8_t i = 0; i < 10; i++ ) {
                HAL_GPIO_WritePin( GPIOF, dev->SCL_Pin, GPIO_PIN_RESET );
                HAL_Delay( 1 );
                HAL_GPIO_WritePin( GPIOF, dev->SCL_Pin, GPIO_PIN_SET );
                HAL_Delay( 1 );
            }
        }
        else if ( dev->SCL_Port == GPIOB ) {
            __HAL_RCC_GPIOB_CLK_ENABLE();
            HAL_GPIO_Init( GPIOB, &GPIO_InitStruct );
            for ( uint8_t i = 0; i < 10; i++ ) {
                HAL_GPIO_WritePin( GPIOB, dev->SCL_Pin, GPIO_PIN_RESET );
                HAL_Delay( 1 );
                HAL_GPIO_WritePin( GPIOB, dev->SCL_Pin, GPIO_PIN_SET );
                HAL_Delay( 1 );
            }
        }
    }
    else if ( i2c_dev->I2C_x == I2C_3 ) {
        GPIO_InitTypeDef GPIO_InitStruct;
        GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
        GPIO_InitStruct.Pull  = GPIO_NOPULL;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
        // i2C3 SDA PC9,SCL PA8;
        __HAL_RCC_GPIOA_CLK_ENABLE();
        GPIO_InitStruct.Pin = GPIO_PIN_8;
        HAL_GPIO_Init( GPIOA, &GPIO_InitStruct );
        for ( uint8_t i = 0; i < 10; i++ ) {
            HAL_GPIO_WritePin( GPIOA, GPIO_PIN_8, GPIO_PIN_RESET );
            HAL_Delay( 1 );
            HAL_GPIO_WritePin( GPIOA, GPIO_PIN_8, GPIO_PIN_SET );
            HAL_Delay( 1 );
        }
    }
}
