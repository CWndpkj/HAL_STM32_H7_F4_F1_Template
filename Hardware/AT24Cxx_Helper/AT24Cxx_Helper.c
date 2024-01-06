#include "AT24Cxx_Helper.h"
#include "I2C_Helper.h"
#include "delay.h"
#define AT24Cxx_SlaveAddr 0x50
extern I2C_HandleTypeDef hi2c3;

int8_t AT24Cxx_Helper_Init()
{
    I2C_DEV_t i2c_dev;
    i2c_dev.I2C_x    = I2C_3;
    i2c_dev.SCL_Pin  = GPIO_PIN_8;
    i2c_dev.SDA_Pin  = GPIO_PIN_9;
    i2c_dev.SCL_Port = GPIOA;
    i2c_dev.SDA_Port = GPIOC;
    I2C_Helper_FailRecover( &i2c_dev );
    I2C_Helper_Init( &hi2c3, &i2c_dev );
    return 0;
}
int8_t AT24Cxx_Helper_WriteByte( uint8_t Buff, uint16_t Addr )
{
    int8_t ErrCode = 0;
    // uint8_t SendBuff[ 2 ];
    // SendBuff[ 0 ] = Addr;
    // SendBuff[ 1 ] = Buff;
    // ErrCode       = I2C_DMA_Write_Len( AT24Cxx_SlaveAddr, Addr >> 8, 2, SendBuff );
    ErrCode = HAL_I2C_Mem_Write( &hi2c3, AT24Cxx_SlaveAddr, Addr, 2, &Buff, 1, HAL_MAX_DELAY );
    delay_ms( 10 );
    return ErrCode;
}
uint8_t AT24Cxx_Helper_ReadByte( uint16_t Addr )
{
    uint8_t Buff;
    uint8_t ErrCode;
    // uint8_t SendBuff[ 2 ];
    // SendBuff[ 0 ] = Addr;
    // SendBuff[ 1 ] = Buff;
    // ErrCode = I2C_DMA_ReadByte( AT24Cxx_SlaveAddr, Addr, &Buff );
    ErrCode = HAL_I2C_Mem_Read( &hi2c3, AT24Cxx_SlaveAddr, Addr, 2, &Buff, 1, HAL_MAX_DELAY );
    return ErrCode;
}

/**
 * @brief 最大写一个page的数据
 *
 * @param pSendBuff
 * @param Addr
 * @param WriteLen
 * @return int8_t
 */
int8_t AT24Cxx_Helper_WriteLen( uint8_t* pSendBuff, uint16_t Addr, uint16_t WriteLen )
{
    // return I2C_DMA_Write_Len( AT24Cxx_SlaveAddr, Addr, WriteLen, pSendBuff );
    return HAL_I2C_Mem_Write( &hi2c3, AT24Cxx_SlaveAddr, Addr, 2, pSendBuff, WriteLen, HAL_MAX_DELAY );
}
int8_t AT24Cxx_Helper_ReadLen( uint8_t* pRcvBuff, uint16_t Addr, uint16_t ReadLen )
{
    // return I2C_DMA_Read_Len( AT24Cxx_SlaveAddr, Addr, ReadLen, pRcvBuff );
    return HAL_I2C_Mem_Read( &hi2c3, AT24Cxx_SlaveAddr, Addr, 2, pRcvBuff, ReadLen, HAL_MAX_DELAY );
}
