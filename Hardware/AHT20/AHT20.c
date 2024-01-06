#include "AHT20.h"
#include "I2C_Helper.h"
#include "delay.h"

extern I2C_HandleTypeDef hi2c3;
uint8_t AHT20_Init()
{
    I2C_DEV_t i2c_dev;
    i2c_dev.I2C_x    = I2C_3;
    i2c_dev.SCL_Pin  = GPIO_PIN_8;
    i2c_dev.SDA_Pin  = GPIO_PIN_9;
    i2c_dev.SCL_Port = GPIOA;
    i2c_dev.SDA_Port = GPIOC;
    I2C_Helper_FailRecover( &i2c_dev );
    I2C_Helper_Init( &hi2c3, &i2c_dev );

    uint8_t Command[ 3 ] = { AHT20_INIT_REG, 0x08, 0x00 };  // 初始化命令
    return HAL_I2C_Master_Transmit( &hi2c3, AHT20_ADDR, Command, 3, HAL_MAX_DELAY );
}
uint8_t ATH20_Read_Status()  // 读取AHT10的状态寄存器
{
    uint8_t buff;
    HAL_I2C_Mem_Read( &hi2c3, AHT20_ADDR, ( uint16_t )AHT20_STATUS_REG, 1, &buff, 1, HAL_MAX_DELAY );
    return buff;
}
uint8_t AHT20_StartAquire()
{
    uint8_t Command[ 3 ] = { AHT20_TrigMeasure_REG, 0x33, 0x00 };
    return HAL_I2C_Master_Transmit( &hi2c3, AHT20_ADDR, Command, 3, HAL_MAX_DELAY );
}

uint8_t AHT20_GetValue( AHT20_value_t* value )
{
    uint8_t Buff[ 7 ] = { 0, 0, 0, 0, 0, 0, 0 };
    uint32_t tmp      = 0;
    if ( 0 == AHT20_StartAquire() ) {
        delay_ms( 100 );
        //  读取结果
        if ( HAL_OK == HAL_I2C_Mem_Read( &hi2c3, AHT20_ADDR, 0x00, 1, Buff, 7, HAL_MAX_DELAY ) ) {
            tmp |= Buff[ 1 ];
            tmp <<= 8;
            tmp |= Buff[ 2 ];
            tmp <<= 8;
            tmp |= Buff[ 3 ];
            tmp >>= 4;
            value->Humidity = ( float )tmp / 10485.76;

            tmp = 0;
            tmp |= ( Buff[ 3 ] & 0x0F );
            tmp <<= 8;
            tmp |= Buff[ 4 ];
            tmp <<= 8;
            tmp |= Buff[ 5 ];
            value->Temperature = ( float )tmp / 1048576 * 200 - 50;
            return 0;
        }
        return 1;
    }
    return 1;
}