#include "usart_helper.h"

UART_HandleTypeDef huart_debug;
#if USART_EN_RX /* 如果使能了接收 */

/* 接收缓冲, 最大USART_REC_LEN个字节. */
uint8_t g_usart_rx_buf[ USART_REC_LEN ];

/*  接收状态
 *  bit15，      接收完成标志
 *  bit14，      接收到0x0d
    bit13        当前接受模式 0：单字节中断模式,1：长数据DMA模式
 *  bit12~0，    接收到的有效字节数目
 */
uint16_t g_usart_rx_sta = 0;

uint8_t g_rx_buffer[ RXBUFFERSIZE ]; /* HAL库使用的串口接收缓冲 */
#endif
error_t usart_debug_write( const u8* data, u32 length )
{
    return HAL_UART_Transmit( &huart_debug, data, length, HAL_MAX_DELAY );
}

void HAL_UART_MspInit( UART_HandleTypeDef* huart )
{
    GPIO_InitTypeDef GPIO_InitStruct = { 0 };

    if ( huart->Instance == USART_DEBUG ) {
        USART_DEBUG_CLK_ENABEL();
        USART_DEBUG_TX_CLK_ENABLE();
        USART_DEBUG_RX_CLK_ENABLE();

        GPIO_InitStruct.Pin       = GPIO_get_pin( USART_DEBUG_TX );  // TX引脚
        GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;                 // 复用推挽输出
        GPIO_InitStruct.Pull      = GPIO_PULLUP;                     // 上拉
        GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;       // 速度等级
        GPIO_InitStruct.Alternate = USART_DEBUG_GPIO_AF;             // 复用为USART1
        HAL_GPIO_Init( GPIO_get_port( USART_DEBUG_TX ), &GPIO_InitStruct );
        GPIO_InitStruct.Pin = GPIO_get_pin( USART_DEBUG_RX );  // RX引脚
        HAL_GPIO_Init( GPIO_get_port( USART_DEBUG_RX ), &GPIO_InitStruct );
#if USART_EN_RX
        HAL_NVIC_EnableIRQ( USART_DEBUG_IRQn );         /* 使能USART1中断通道 */
        HAL_NVIC_SetPriority( USART_DEBUG_IRQn, 3, 3 ); /* 抢占优先级3，子优先级3 */
#endif
    }
}
#if USART_EN_RX
void USART_UX_IRQHandler( void )
{
    HAL_UART_IRQHandler( &huart_debug ); /* 调用HAL库中断处理公用函数 */
}
/**
 * @brief       Rx传输回调函数
 * @param       huart: UART句柄类型指针
 * @retval      无
 */

void HAL_UART_RxCpltCallback( UART_HandleTypeDef* huart )
{
    if ( huart->Instance == USART_DEBUG ) /* 如果是串口1 */
    {
        if ( ( g_usart_rx_sta & 0x2000 ) == 0 ) {
            if ( ( g_usart_rx_sta & 0x8000 ) == 0 ) /* 接收未完成 */
            {
                if ( g_usart_rx_sta & 0x4000 ) /* 接收到了0x0d */
                {
                    if ( g_rx_buffer[ 0 ] != 0x0a ) {
                        g_usart_rx_sta = 0; /* 接收错误,重新开始 */
                    }
                    else {
                        g_usart_rx_sta |= 0x8000; /* 接收完成了 */
                    }
                }
                else /* 还没收到0X0D */
                {
                    if ( g_rx_buffer[ 0 ] == 0x0d ) {
                        g_usart_rx_sta |= 0x4000;
                    }
                    else {
                        g_usart_rx_buf[ g_usart_rx_sta & 0X3FFF ] = g_rx_buffer[ 0 ];
                        g_usart_rx_sta++;
                        if ( g_usart_rx_sta > ( USART_REC_LEN - 1 ) ) {
                            g_usart_rx_sta = 0; /* 接收数据错误,重新开始接收 */
                        }
                    }
                }
            }
            HAL_UART_Receive_IT( &huart_debug, ( uint8_t* )g_rx_buffer, 1 );
        }
        else {
            data_rcv_handler.last_transfer_timestamp = HAL_GetTick();
        }
    }
}
#endif

error_t usart_debug_init()
{
    huart_debug.Instance        = USART_DEBUG;
    huart_debug.Init.BaudRate   = USART_DEBUG_BAUD;
    huart_debug.Init.WordLength = UART_WORDLENGTH_8B;
    huart_debug.Init.StopBits   = UART_STOPBITS_1;
    huart_debug.Init.Parity     = UART_PARITY_NONE;
    huart_debug.Init.Mode       = UART_MODE_TX_RX;

    HAL_UART_Init( &huart_debug );
    return HAL_UART_Receive_IT( &huart_debug, ( uint8_t* )g_rx_buffer, 1 );
}
