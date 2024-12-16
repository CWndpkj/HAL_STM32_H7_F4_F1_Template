#include "key.h"
#include "GPIO.h"

void key_init()
{
    __HAL_RCC_GPIOE_CLK_ENABLE();
    GPIO_InitTypeDef GPIO_InitStruct = { 0 };

    GPIO_InitStruct.Mode  = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull  = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;

    GPIO_InitStruct.Pin = GPIO_get_pin( SW4_Pin );
    HAL_GPIO_Init( GPIO_get_port( SW4_Pin ), &GPIO_InitStruct );

    GPIO_InitStruct.Pin = GPIO_get_pin( SW5_Pin );
    HAL_GPIO_Init( GPIO_get_port( SW5_Pin ), &GPIO_InitStruct );

    GPIO_InitStruct.Pin = GPIO_get_pin( SW6_Pin );
    HAL_GPIO_Init( GPIO_get_port( SW6_Pin ), &GPIO_InitStruct );

    GPIO_InitStruct.Pin = GPIO_get_pin( SW7_Pin );
    HAL_GPIO_Init( GPIO_get_port( SW7_Pin ), &GPIO_InitStruct );

    GPIO_InitStruct.Pin = GPIO_get_pin( SW8_Pin );
    HAL_GPIO_Init( GPIO_get_port( SW8_Pin ), &GPIO_InitStruct );
}

// 用于轮询模式获取按键的状态
uint8_t key_get_state()
{
    uint8_t state = 0;
    if ( HAL_GPIO_ReadPin( GPIO_get_port( SW4_Pin ), GPIO_get_pin( SW4_Pin ) ) == GPIO_PIN_RESET ) {
        state |= 0x1;
    }
    if ( HAL_GPIO_ReadPin( GPIO_get_port( SW5_Pin ), GPIO_get_pin( SW5_Pin ) ) == GPIO_PIN_RESET ) {
        state |= ( 0x1 << 1 );
    }
    if ( HAL_GPIO_ReadPin( GPIO_get_port( SW6_Pin ), GPIO_get_pin( SW6_Pin ) ) == GPIO_PIN_RESET ) {
        state |= ( 0x1 << 2 );
    }
    if ( HAL_GPIO_ReadPin( GPIO_get_port( SW7_Pin ), GPIO_get_pin( SW7_Pin ) ) == GPIO_PIN_RESET ) {
        state |= ( 0x1 << 3 );
    }
    if ( HAL_GPIO_ReadPin( GPIO_get_port( SW8_Pin ), GPIO_get_pin( SW8_Pin ) ) == GPIO_PIN_RESET ) {
        state |= ( 0x1 << 4 );
    }
    if ( HAL_GPIO_ReadPin( GPIO_get_port( SW9_Pin ), GPIO_get_pin( SW9_Pin ) ) == GPIO_PIN_RESET ) {
        state |= ( 0x1 << 5 );
    }
    return state;
}