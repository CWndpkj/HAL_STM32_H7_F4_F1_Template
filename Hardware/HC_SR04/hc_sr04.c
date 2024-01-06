#include "hc_sr04.h"
#include "uart.h"

// static hc_sr04_timestamp_rise = 0, hc_sr04_timestamp_fall = 0;
static void ( *fresh_cb )( uint32_t dis );
static float distance;
TIM_HandleTypeDef tim_handler;

// void HAL_TIM_IC_MspInit( TIM_HandleTypeDef* htim )
// {
//     if ( htim->Instance == TIM1 ) {
//         __HAL_RCC_TIM1_CLK_ENABLE();
//         __HAL_RCC_GPIOA_CLK_ENABLE();
//         GPIO_InitTypeDef GPIO_InitStruct;
//         GPIO_InitStruct.Mode  = GPIO_MODE_INPUT;
//         GPIO_InitStruct.Pin   = ECHO_PIN;
//         GPIO_InitStruct.Pull  = GPIO_PULLDOWN;
//         GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
//         HAL_GPIO_Init( ECHO_PORT, &GPIO_InitStruct );

//         HAL_NVIC_SetPriority( TIM1_UP_TIM10_IRQn, 1, 2 );
//         HAL_NVIC_EnableIRQ( TIM1_UP_TIM10_IRQn );

//         HAL_NVIC_SetPriority( TIM1_CC_IRQn, 1, 1 );
//         HAL_NVIC_EnableIRQ( TIM1_CC_IRQn );
//     }
// }
void HAL_TIM_Base_MspInit( TIM_HandleTypeDef* htim )
{
    if ( htim->Instance == HC_SR04_TIM ) {
        HS_SR04_TIM_CLK_ENABLE();

        HAL_NVIC_SetPriority( TIM1_UP_TIM10_IRQn, 2, 2 );
        HAL_NVIC_EnableIRQ( TIM1_UP_TIM10_IRQn );
    }
}
void hc_sr04_init()
{
    ECHO_PORT_CLK_ENABLE();
    TRIG_PORT_CLK_ENABLE();
    GPIO_InitTypeDef GPIO_InitStruct = { 0 };
    GPIO_InitStruct.Mode             = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pin              = TRIG_PIN;
    GPIO_InitStruct.Pull             = GPIO_PULLDOWN;
    GPIO_InitStruct.Speed            = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init( TRIG_PORT, &GPIO_InitStruct );

    GPIO_InitStruct.Pin  = ECHO_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING_FALLING;
    GPIO_InitStruct.Pull = GPIO_PULLDOWN;
    HAL_GPIO_Init( ECHO_PORT, &GPIO_InitStruct );
    HAL_NVIC_SetPriority( EXTI15_10_IRQn, 2, 2 );
    HAL_NVIC_EnableIRQ( EXTI15_10_IRQn );

    tim_handler.Instance               = HC_SR04_TIM;
    tim_handler.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
    tim_handler.Init.ClockDivision     = TIM_CLOCKDIVISION_DIV1;
    tim_handler.Init.CounterMode       = TIM_COUNTERMODE_UP;
    tim_handler.Init.Period            = 0xFFFF;
    tim_handler.Init.Prescaler         = 168 - 1;  // 1MHz
    tim_handler.Init.RepetitionCounter = 0;
    HAL_TIM_Base_Init( &tim_handler );

    // TIM_IC_InitTypeDef TIM_IC_InitStruct = { 0 };
    // TIM_IC_InitStruct.ICFilter           = 0xF;
    // TIM_IC_InitStruct.ICPolarity         = TIM_ICPOLARITY_RISING;
    // TIM_IC_InitStruct.ICPrescaler        = TIM_ICPSC_DIV1;
    // TIM_IC_InitStruct.ICSelection        = TIM_ICSELECTION_DIRECTTI;  // 控制输入捕获是否反向的,配置为不反向
    // HAL_TIM_IC_ConfigChannel( &htim1, &TIM_IC_InitStruct, TIM_CHANNEL_4 );

    // 开启更新中断
    __HAL_TIM_ENABLE_IT( &tim_handler, TIM_IT_UPDATE );  // 在溢出时触发中断,处理时间不够的情况
    //__HAL_TIM_ENABLE_IT( &htim1, TIM_IT_CC4 );
    // HAL_TIM_IC_Start_IT( &htim1, TIM_CHANNEL_4 );  // 开启输入捕获中断
}

/**
 * @brief       通用定时器TIMX 通道Y 输入捕获 初始化函数
 * @note
 *              通用定时器的时钟来自APB1,当PPRE1 ≥ 2分频的时候
 *              通用定时器的时钟为APB1时钟的2倍, 而APB1为36M, 所以定时器时钟 = 72Mhz
 *              定时器溢出时间计算方法: Tout = ((arr + 1) * (psc + 1)) / Ft us.
 *              Ft=定时器工作频率,单位:Mhz
 *
 *              这里参数设定为arr =0; psc = 0; Tick = 1/72M,Tout = 65536/72M
 */

// 采用定时器输入捕获读取高电平的时间
// 输入捕获状态(IC_sta)
// [7]  :0,没有捕获到上升沿;1,成功捕获到上升沿，等待下降沿的到来.在捕获到一次高电平后复原
// [6]  :0,还没捕获到高电平;1,已经捕获到高电平了.
// [5:0]:捕获高电平后溢出的次数,最多溢出63次,所以最长捕获值 = 63*65536 + 65535 = 4194303
//       注意:为了通用,我们默认ARR和CCRy都是16位寄存器,对于32位的定时器(如:TIM5),也只按16位使用
//       按1us的计数频率,最长溢出时间为:4194303 us, 约4.19秒
//
//      (说明一下：正常32位定时器来说,1us计数器加1,溢出时间:4294秒)

uint8_t ECHO_sta    = 0;  // 捕获状态
uint32_t ECHO_value = 0;  // 捕获高电平的值

// void TIM1_CC_IRQHandler()
// {
//     HAL_TIM_IRQHandler( &htim1 );
// }

void TIM1_UP_TIM10_IRQHandler()
{
    HAL_TIM_IRQHandler( &tim_handler );
}
/**
 * @brief       定时器输入捕获中断处理回调函数
 * @param       htim:定时器句柄指针
 * @note        该函数在HAL_TIM_IRQHandler中会被调用
 * @retval      无
 */
// void HAL_TIM_IC_CaptureCallback( TIM_HandleTypeDef* htim )
// {
//     if ( htim->Instance == TIM1 ) {
//         if ( IC_sta & 0x80 )  // 已捕获上升沿
//         {
//             // 成功捕获到高电平
//             IC_sta |= 0x40;
//             IC_sta &= 0x7F;
//             // 读取捕获的值
//             IC_value = HAL_TIM_ReadCapturedValue( htim, TIM_CHANNEL_4 );
//             distance = ( ( float )( IC_sta & 0x3F ) * 65536 / 72000000 + ( float )IC_value / 72000000 ) * 17000;//cm
//             printf( "\r\nRepeat:%d,IC_value:%ld\r\n", (IC_sta & 0x3F), IC_value );
//             printf( "distance:" );
//             PrintFloat( distance );
//             //  一定要先清除原来的设置
//             TIM_RESET_CAPTUREPOLARITY( htim, TIM_CHANNEL_4 );
//             TIM_SET_CAPTUREPOLARITY( htim, TIM_CHANNEL_4, TIM_ICPOLARITY_RISING );
//         }
//         else {
//             // 捕获到上升沿
//             IC_sta   = 0;  // 清零移除计数
//             IC_value = 0;
//             IC_sta |= 0x80;
//             // 关闭定时器
//             __HAL_TIM_DISABLE( htim );
//             __HAL_TIM_SET_COUNTER( htim, 0 );
//             TIM_RESET_CAPTUREPOLARITY( htim, TIM_CHANNEL_4 );
//             TIM_SET_CAPTUREPOLARITY( htim, TIM_CHANNEL_4, TIM_ICPOLARITY_FALLING );
//             // 开始定时器
//             __HAL_TIM_ENABLE( htim );
//         }
//     }
// }
void EXTI15_10_IRQHandler()
{
    HAL_GPIO_EXTI_IRQHandler( ECHO_PIN );
}

void HAL_GPIO_EXTI_Callback( uint16_t GPIO_Pin )
{
    if ( GPIO_Pin == ECHO_PIN ) {
        if ( HAL_GPIO_ReadPin( ECHO_PORT, ECHO_PIN ) == GPIO_PIN_SET && !( ECHO_sta & 0x80 ) ) {
            ECHO_sta |= 0x80;
            // 第一次高电平触发，开启定时器
            __HAL_TIM_SET_COUNTER( &tim_handler, 0 );
            HAL_TIM_Base_Start_IT( &tim_handler );
            __HAL_TIM_ENABLE( &tim_handler );
        }
        else if ( HAL_GPIO_ReadPin( ECHO_PORT, ECHO_PIN ) == GPIO_PIN_RESET && ( ECHO_sta & 0x80 ) ) {
            HAL_TIM_Base_Stop_IT( &tim_handler );
            ECHO_value = __HAL_TIM_GET_COUNTER( &tim_handler );
            __HAL_TIM_DISABLE( &tim_handler );
            ECHO_sta &= 0x7F;
            ECHO_sta |= 0x40;
            // printf( "value:%ld\r\n", ( ( ECHO_sta & 0x3F ) * 65536 + ECHO_value ) * 1 * 17000 );
            PrintFloat( ( ( float )( ECHO_sta & 0x3F ) * 65536 + ECHO_value ) * 17000 / 1000000 );
            printf( "\r\n" );
            ECHO_sta = 0;
        }
    }
}
/**
 * @brief       定时器更新中断回调函数,计数溢出时调用
 * @param        htim:定时器句柄指针
 * @note        此函数会被定时器中断函数共同调用的
 * @retval      无
 */
void HAL_TIM_PeriodElapsedCallback( TIM_HandleTypeDef* htim )
{
    if ( htim->Instance == HC_SR04_TIM ) {
        if ( ECHO_sta & 0x80 ) {
            // 表明定时器捕获到一个上升沿,在溢出时仍未捕获到下降沿
            ECHO_sta++;  // 溢出计数自增
        }
    }
}

void hc_sr04_set_cb( void ( *cb )( uint32_t dis ) )
{
    fresh_cb = cb;
}
void hc_sr04_trig()
{
    HAL_GPIO_WritePin( TRIG_PORT, TRIG_PIN, GPIO_PIN_SET );
    HAL_Delay( 1 );
    HAL_GPIO_WritePin( TRIG_PORT, TRIG_PIN, GPIO_PIN_RESET );
}
float hc_sr04_get_dis()
{
    return distance;
}