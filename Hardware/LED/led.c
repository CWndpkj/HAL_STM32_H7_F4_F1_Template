/***
    *************************************************************************************************
    *	@file  	main.c
    *	@version V1.0
    *  @date    2022-7-7
    *	@author  反客科技
    *	@brief   LED闪烁实验
   *************************************************************************************************
   *  @description
    *
    *	实验平台：反客STM32H750XBH6核心板 （型号：FK750M5-XBH6）
    *	淘宝地址：https://shop212360197.taobao.com
    *	QQ交流群：536665479
    *
>>>>> 文件说明：
    *
    *	初始化LED的IO口，配置为推挽输出、上拉、速度等级2M。
    *
    ************************************************************************************************
***/

#include "led.h"

void LED_Init()
{
    GPIO_InitTypeDef GPIO_InitStruct = { 0 };

    /* GPIO Ports Clock Enable */
    __HAL_RCC_GPIOE_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOH_CLK_ENABLE();
    __HAL_RCC_GPIOF_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOD_CLK_ENABLE();
    __HAL_RCC_GPIOG_CLK_ENABLE();

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin( GPIOF, LED1_Pin | LED2_Pin | LED3_Pin, GPIO_PIN_SET );

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin( LED4_GPIO_Port, LED4_Pin, GPIO_PIN_SET );

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin( GPIOG, LED5_Pin | LED6_Pin | LED7_Pin | LED8_Pin, GPIO_PIN_SET );

    /*Configure GPIO pins : PEPin PEPin PEPin PEPin
                             PEPin PEPin */
    GPIO_InitStruct.Pin  = SW5_Pin | SW6_Pin | SW7_Pin | SW8_Pin | SW9_Pin | SW4_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init( GPIOE, &GPIO_InitStruct );

    /*Configure GPIO pins : PFPin PFPin PFPin */
    GPIO_InitStruct.Pin   = LED1_Pin | LED2_Pin | LED3_Pin;
    GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull  = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init( GPIOF, &GPIO_InitStruct );

    /*Configure GPIO pin : PtPin */
    GPIO_InitStruct.Pin   = LED4_Pin;
    GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull  = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init( LED4_GPIO_Port, &GPIO_InitStruct );

    /*Configure GPIO pins : PGPin PGPin PGPin PGPin */
    GPIO_InitStruct.Pin   = LED5_Pin | LED6_Pin | LED7_Pin | LED8_Pin;
    GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull  = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init( GPIOG, &GPIO_InitStruct );
}

void LED_Shine()
{
    HAL_GPIO_TogglePin( LED1_GPIO_Port, LED1_Pin );
    //delay_ms( 50 );
    //HAL_GPIO_TogglePin( LED2_GPIO_Port, LED2_Pin );
    // delay_ms( 50 );
    // HAL_GPIO_TogglePin( LED3_GPIO_Port, LED3_Pin );
    // delay_ms( 50 );
    // HAL_GPIO_TogglePin( LED4_GPIO_Port, LED4_Pin );
    // delay_ms( 50 );
    // HAL_GPIO_TogglePin( LED5_GPIO_Port, LED5_Pin );
    // delay_ms( 50 );
    // HAL_GPIO_TogglePin( LED6_GPIO_Port, LED6_Pin );
    // delay_ms( 50 );
    // HAL_GPIO_TogglePin( LED7_GPIO_Port, LED7_Pin );
    // delay_ms( 50 );
    // HAL_GPIO_TogglePin( LED8_GPIO_Port, LED8_Pin );
    //delay_ms( 50 );
}
