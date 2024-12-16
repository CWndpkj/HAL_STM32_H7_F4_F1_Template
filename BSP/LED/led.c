#include "led.h"
#include "GPIO.h"

void LED_Init()
{
    GPIO_InitTypeDef GPIO_InitStruct = { 0 };

    __HAL_RCC_GPIOE_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOH_CLK_ENABLE();
    __HAL_RCC_GPIOF_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOD_CLK_ENABLE();
    __HAL_RCC_GPIOG_CLK_ENABLE();

    GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull  = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;

    GPIO_InitStruct.Pin = GPIO_get_pin( LED1_Pin );
    HAL_GPIO_Init( GPIO_get_port( LED1_Pin ), &GPIO_InitStruct );

    GPIO_InitStruct.Pin = GPIO_get_pin( LED2_Pin );
    HAL_GPIO_Init( GPIO_get_port( LED2_Pin ), &GPIO_InitStruct );

    GPIO_InitStruct.Pin = GPIO_get_pin( LED2_Pin );
    HAL_GPIO_Init( GPIO_get_port( LED2_Pin ), &GPIO_InitStruct );

    GPIO_InitStruct.Pin = GPIO_get_pin( LED2_Pin );
    HAL_GPIO_Init( GPIO_get_port( LED2_Pin ), &GPIO_InitStruct );

    GPIO_InitStruct.Pin = GPIO_get_pin( LED2_Pin );
    HAL_GPIO_Init( GPIO_get_port( LED2_Pin ), &GPIO_InitStruct );

    GPIO_InitStruct.Pin = GPIO_get_pin( LED2_Pin );
    HAL_GPIO_Init( GPIO_get_port( LED2_Pin ), &GPIO_InitStruct );

    GPIO_InitStruct.Pin = GPIO_get_pin( LED2_Pin );
    HAL_GPIO_Init( GPIO_get_port( LED2_Pin ), &GPIO_InitStruct );
}

void LED_On( eLED_NUM num )
{
    switch ( num ) {
    case LED1:
        HAL_GPIO_WritePin( GPIO_get_port( LED1_Pin ), GPIO_get_pin( LED1_Pin ), GPIO_PIN_RESET );
        break;
    case LED2:
        HAL_GPIO_WritePin( GPIO_get_port( LED2_Pin ), GPIO_get_pin( LED2_Pin ), GPIO_PIN_RESET );
        break;
    case LED3:
        HAL_GPIO_WritePin( GPIO_get_port( LED3_Pin ), GPIO_get_pin( LED3_Pin ), GPIO_PIN_RESET );
        break;
    case LED4:
        HAL_GPIO_WritePin( GPIO_get_port( LED4_Pin ), GPIO_get_pin( LED4_Pin ), GPIO_PIN_RESET );
        break;
    case LED5:
        HAL_GPIO_WritePin( GPIO_get_port( LED5_Pin ), GPIO_get_pin( LED5_Pin ), GPIO_PIN_RESET );
        break;
    case LED6:
        HAL_GPIO_WritePin( GPIO_get_port( LED6_Pin ), GPIO_get_pin( LED6_Pin ), GPIO_PIN_RESET );
        break;
    case LED7:
        HAL_GPIO_WritePin( GPIO_get_port( LED7_Pin ), GPIO_get_pin( LED7_Pin ), GPIO_PIN_RESET );
        break;
    case LED8:
        HAL_GPIO_WritePin( GPIO_get_port( LED8_Pin ), GPIO_get_pin( LED8_Pin ), GPIO_PIN_RESET );
        break;
    }
}
void LED_Off( eLED_NUM num )
{
    switch ( num ) {
    case LED1:
        HAL_GPIO_WritePin( GPIO_get_port( LED1_Pin ), GPIO_get_pin( LED1_Pin ), GPIO_PIN_SET );
        break;
    case LED2:
        HAL_GPIO_WritePin( GPIO_get_port( LED2_Pin ), GPIO_get_pin( LED2_Pin ), GPIO_PIN_SET );
        break;
    case LED3:
        HAL_GPIO_WritePin( GPIO_get_port( LED3_Pin ), GPIO_get_pin( LED3_Pin ), GPIO_PIN_SET );
        break;
    case LED4:
        HAL_GPIO_WritePin( GPIO_get_port( LED4_Pin ), GPIO_get_pin( LED4_Pin ), GPIO_PIN_SET );
        break;
    case LED5:
        HAL_GPIO_WritePin( GPIO_get_port( LED5_Pin ), GPIO_get_pin( LED5_Pin ), GPIO_PIN_SET );
        break;
    case LED6:
        HAL_GPIO_WritePin( GPIO_get_port( LED6_Pin ), GPIO_get_pin( LED6_Pin ), GPIO_PIN_SET );
        break;
    case LED7:
        HAL_GPIO_WritePin( GPIO_get_port( LED7_Pin ), GPIO_get_pin( LED7_Pin ), GPIO_PIN_SET );
        break;
    case LED8:
        HAL_GPIO_WritePin( GPIO_get_port( LED8_Pin ), GPIO_get_pin( LED8_Pin ), GPIO_PIN_SET );
        break;
    }
}
void LED_Toggle( eLED_NUM num )
{
    switch ( num ) {
    case LED1:
        HAL_GPIO_TogglePin( GPIO_get_port( LED1_Pin ), GPIO_get_pin( LED1_Pin )  );
        break;
    case LED2:
        HAL_GPIO_TogglePin( GPIO_get_port( LED2_Pin ), GPIO_get_pin( LED2_Pin )  );
        break;
    case LED3:
        HAL_GPIO_TogglePin( GPIO_get_port( LED3_Pin ), GPIO_get_pin( LED3_Pin )  );
        break;
    case LED4:
        HAL_GPIO_TogglePin( GPIO_get_port( LED4_Pin ), GPIO_get_pin( LED4_Pin )  );
        break;
    case LED5:
        HAL_GPIO_TogglePin( GPIO_get_port( LED5_Pin ), GPIO_get_pin( LED5_Pin )  );
        break;
    case LED6:
        HAL_GPIO_TogglePin( GPIO_get_port( LED6_Pin ), GPIO_get_pin( LED6_Pin )  );
        break;
    case LED7:
        HAL_GPIO_TogglePin( GPIO_get_port( LED7_Pin ), GPIO_get_pin( LED7_Pin )  );
        break;
    case LED8:
        HAL_GPIO_TogglePin( GPIO_get_port( LED8_Pin ), GPIO_get_pin( LED8_Pin )  );
        break;
    }
}
