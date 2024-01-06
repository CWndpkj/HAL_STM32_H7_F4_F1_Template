#include "exti.h"
#include "led.h"

void exti_init()
{
    LED_Init();
    // 开启时钟
    __HAL_RCC_GPIOC_CLK_ENABLE();
    // 初始化GPIO配置
    GPIO_InitTypeDef GPIO_Init_Struct = { 0 };
    GPIO_Init_Struct.Mode             = GPIO_MODE_IT_RISING;  // 下降沿触发
    GPIO_Init_Struct.Pin              = GPIO_PIN_15;
    GPIO_Init_Struct.Pull             = GPIO_PULLDOWN;
    GPIO_Init_Struct.Speed            = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init( GPIOC, &GPIO_Init_Struct );

    // 设置NVIC
    HAL_NVIC_EnableIRQ( EXTI15_10_IRQn );
    HAL_NVIC_SetPriority( EXTI15_10_IRQn, 2, 2 );
}

// 重写硬件中断函数
void EXTI15_10_IRQHandler()
{
    // 在此调用HAL库定义的中断集中处理函数
    HAL_GPIO_EXTI_IRQHandler( GPIO_PIN_15 );
}

// 重写HAL库中断回调函数
void HAL_GPIO_EXTI_Callback( uint16_t GPIO_PINx )
{
    // 判断是否是PIN15
    if ( GPIO_PINx == GPIO_PIN_15 ) {
        // 检测按键是否按下
        HAL_Delay( 20 );  // 消抖
        if ( HAL_GPIO_ReadPin( GPIOC, GPIO_PIN_15 ) == GPIO_PIN_SET ) {
            // 执行相关的中断代码
        }
        // 虽然HAL的集中处理函数已经清除中断挂起位，在此为防止按键抖动带来的因影响，再次清除标志位
        __HAL_GPIO_EXTI_CLEAR_IT( GPIO_PIN_15 );
    }
}