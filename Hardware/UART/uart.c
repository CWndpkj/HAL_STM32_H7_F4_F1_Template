#include "uart.h"

extern UART_HandleTypeDef huart3;
/*************************************************************************************************
 *	函 数 名:	HAL_UART_MspInit
 *	入口参数:	huart3 - UART_HandleTypeDef定义的变量，即表示定义的串口
 *	返 回 值:	无
 *	函数功能:	初始化串口引脚
 *	说    明:	无
 *************************************************************************************************/

void HAL_UART_MspInit( UART_HandleTypeDef* huart3 )
{
    GPIO_InitTypeDef GPIO_InitStruct = { 0 };
    if ( huart3->Instance == USARTx ) {
        UART_CLK_ENABLE();  // 开启 USART 时钟

        UART_TX_GPIO_CLK_ENABLE();  // 开启 USART1 TX 引脚的 GPIO 时钟
        UART_RX_GPIO_CLK_ENABLE();  // 开启 USART1 RX 引脚的 GPIO 时钟

        GPIO_InitStruct.Pin       = UART_TX_PIN;      // TX引脚
        GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;  // 复用推挽输出
        GPIO_InitStruct.Alternate = GPIO_AF7_USART3;
        GPIO_InitStruct.Pull  = GPIO_PULLUP;           // 上拉
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;  // 速度等级
        HAL_GPIO_Init( UART_TX_PORT, &GPIO_InitStruct );

        GPIO_InitStruct.Pin  = UART_RX_PIN;  // RX引脚
        HAL_GPIO_Init( UART_RX_PORT, &GPIO_InitStruct );
    }
}

/*************************************************************************************************
 *	函 数 名:	USART1_Init
 *	入口参数:	无
 *	返 回 值:	无
 *	函数功能:	初始化串口配置
 *	说    明:	无
 *************************************************************************************************/

void UART_Init()
{
    huart3.Instance          = USARTx;
    huart3.Init.BaudRate     = UART_BaudRate;
    huart3.Init.WordLength   = UART_WORDLENGTH_8B;
    huart3.Init.StopBits     = UART_STOPBITS_1;
    huart3.Init.Parity       = UART_PARITY_NONE;
    huart3.Init.Mode         = UART_MODE_TX_RX;
    huart3.Init.HwFlowCtl    = UART_HWCONTROL_NONE;
    huart3.Init.OverSampling = UART_OVERSAMPLING_16;
    HAL_UART_Init( &huart3 );
    __HAL_UART_ENABLE( &huart3 );
}

int _write( int fd, char* pBuffer, int size )
{
    HAL_UART_Transmit( &huart3, ( uint8_t* )pBuffer, size, HAL_MAX_DELAY );  // 发送单字节数据
    return size;
}

void PrintFloat( float value )
{
    int tmp, tmp1, tmp2, tmp3, tmp4, tmp5, tmp6;
    tmp  = ( int )value;
    tmp1 = ( int )( ( value - tmp ) * 10 ) % 10;
    tmp2 = ( int )( ( value - tmp ) * 100 ) % 10;
    tmp3 = ( int )( ( value - tmp ) * 1000 ) % 10;
    tmp4 = ( int )( ( value - tmp ) * 10000 ) % 10;
    tmp5 = ( int )( ( value - tmp ) * 100000 ) % 10;
    tmp6 = ( int )( ( value - tmp ) * 1000000 ) % 10;
    printf( "%d.%d%d%d%d%d%d\r\n", tmp, tmp1, tmp2, tmp3, tmp4, tmp5, tmp6 );
}