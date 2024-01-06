#include "OLED.h"
#include "I2C_Helper.h"
#include "delay.h"
u8g2_t u8g2;
extern I2C_HandleTypeDef hi2c1;
uint8_t u8x8_byte_i2c( u8x8_t* u8x8, uint8_t msg, uint8_t arg_int, void* arg_ptr );
uint8_t u8x8_gpio_and_delay( u8x8_t* u8x8, uint8_t msg, uint8_t arg_int, void* arg_ptr );

// void HAL_I2C_MspInit( I2C_HandleTypeDef* hi2c )
// {
//     GPIO_InitTypeDef GPIO_InitStruct = { 0 };
//     if ( hi2c->Instance == I2C1 ) {
//         __HAL_RCC_GPIOB_CLK_ENABLE();
//         __HAL_RCC_I2C1_CLK_ENABLE();
//         HAL_GPIO_WritePin( OLED_PORT, OLED_SCL, GPIO_PIN_SET );
//         HAL_GPIO_WritePin( OLED_PORT, OLED_SDA, GPIO_PIN_SET );
//         GPIO_InitStruct.Mode      = GPIO_MODE_AF_OD;
//         GPIO_InitStruct.Pin       = OLED_SCL | OLED_SDA;
//         GPIO_InitStruct.Alternate = GPIO_AF4_I2C1;
//         GPIO_InitStruct.Pull      = GPIO_PULLUP;
//         GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
//         HAL_GPIO_Init( OLED_PORT, &GPIO_InitStruct );
//     }
// }

void OLED_Init()
{
    I2C_DEV_t i2c_dev;
    i2c_dev.I2C_x    = I2C_1;
    i2c_dev.SCL_Port = GPIOB;
    i2c_dev.SCL_Pin  = GPIO_PIN_6;
    i2c_dev.SDA_Port = GPIOB;
    i2c_dev.SDA_Pin  = GPIO_PIN_7;
    I2C_Helper_FailRecover( &i2c_dev );
    I2C_Helper_Init( &hi2c1, &i2c_dev );
    // hi2c1.Instance             = I2C1;
    // hi2c1.Init.AddressingMode  = I2C_ADDRESSINGMODE_7BIT;
    // hi2c1.Init.ClockSpeed      = 1000000;
    // hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
    // hi2c1.Init.DutyCycle       = I2C_DUTYCYCLE_16_9;
    // hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_ENABLE;
    // hi2c1.Init.NoStretchMode   = I2C_NOSTRETCH_DISABLE;
    // hi2c1.Init.OwnAddress1     = 0x88;
    // HAL_I2C_Init( &hi2c1 );
    // __HAL_I2C_ENABLE( &hi2c1 );

    u8g2_Setup_ssd1306_i2c_128x64_noname_f( &u8g2, U8G2_R0, u8x8_byte_i2c, u8x8_gpio_and_delay );
    u8g2_InitDisplay( &u8g2 );
    u8g2_SetPowerSave( &u8g2, 0 );
    u8g2_ClearDisplay( &u8g2 );
}

uint8_t u8x8_gpio_and_delay( u8x8_t* u8x8, uint8_t msg, uint8_t arg_int, void* arg_ptr )
{
    switch ( msg ) {
    case U8X8_MSG_GPIO_AND_DELAY_INIT:  // called once during init phase of u8g2/u8x8
        break;                          // can be used to setup pins
    case U8X8_MSG_DELAY_NANO:           // delay arg_int * 1 nano second
        __NOP();
        break;
    case U8X8_MSG_DELAY_100NANO:  // delay arg_int * 100 nano seconds
        __NOP();
        break;
    case U8X8_MSG_DELAY_10MICRO:  // delay arg_int * 10 micro seconds
        __NOP();
        break;
    case U8X8_MSG_DELAY_MILLI:  // delay arg_int * 1 milli second
        delay_ms( 1 );
        break;
    case U8X8_MSG_DELAY_I2C:  // arg_int is the I2C speed in 100KHz, e.g. 4 = 400 KHz
        delay_ms( 1 );
        break;                     // arg_int=1: delay by 5us, arg_int = 4: delay by 1.25us
    case U8X8_MSG_GPIO_I2C_CLOCK:  // arg_int=0: Output low at I2C clock pin
        break;                     // arg_int=1: Input dir with pullup high for I2C clock pin
    case U8X8_MSG_GPIO_I2C_DATA:   // arg_int=0: Output low at I2C data pin
        break;                     // arg_int=1: Input dir with pullup high for I2C data pin
    case U8X8_MSG_GPIO_MENU_SELECT:
        u8x8_SetGPIOResult( u8x8, /* get menu select pin state */ 0 );
        break;
    case U8X8_MSG_GPIO_MENU_NEXT:
        u8x8_SetGPIOResult( u8x8, /* get menu next pin state */ 0 );
        break;
    case U8X8_MSG_GPIO_MENU_PREV:
        u8x8_SetGPIOResult( u8x8, /* get menu prev pin state */ 0 );
        break;
    case U8X8_MSG_GPIO_MENU_HOME:
        u8x8_SetGPIOResult( u8x8, /* get menu home pin state */ 0 );
        break;
    default:
        u8x8_SetGPIOResult( u8x8, 1 );  // default return value
        break;
    }
    return 1;
}

uint8_t u8x8_byte_i2c( u8x8_t* u8x8, uint8_t msg, uint8_t arg_int, void* arg_ptr )
{
    static uint8_t buffer[ 32 ]; /* u8g2/u8x8 will never send more than 32 bytes between START_TRANSFER and END_TRANSFER */
    static uint8_t buf_idx;
    uint8_t* data;

    switch ( msg ) {
    case U8X8_MSG_BYTE_SEND:
        data = ( uint8_t* )arg_ptr;
        while ( arg_int > 0 ) {
            buffer[ buf_idx++ ] = *data;
            data++;
            arg_int--;
        }
        break;
    case U8X8_MSG_BYTE_INIT:
        /* add your custom code to init i2c subsystem */
        break;
    case U8X8_MSG_BYTE_SET_DC:
        /* ignored for i2c */
        break;
    case U8X8_MSG_BYTE_START_TRANSFER:
        buf_idx = 0;
        break;
    case U8X8_MSG_BYTE_END_TRANSFER:
        HAL_I2C_Master_Transmit( &hi2c1, 0x78, buffer, buf_idx, 5000 );
        break;
    default:
        return 0;
    }
    return 1;
}
