#include "uart.h"

extern UART_HandleTypeDef huart3;
/*************************************************************************************************
 *	�� �� ��:	HAL_UART_MspInit
 *	��ڲ���:	huart3 - UART_HandleTypeDef����ı���������ʾ����Ĵ���
 *	�� �� ֵ:	��
 *	��������:	��ʼ����������
 *	˵    ��:	��
 *************************************************************************************************/

void HAL_UART_MspInit( UART_HandleTypeDef* huart3 )
{
    GPIO_InitTypeDef GPIO_InitStruct = { 0 };
    if ( huart3->Instance == USARTx ) {
        UART_CLK_ENABLE();  // ���� USART ʱ��

        UART_TX_GPIO_CLK_ENABLE();  // ���� USART1 TX ���ŵ� GPIO ʱ��
        UART_RX_GPIO_CLK_ENABLE();  // ���� USART1 RX ���ŵ� GPIO ʱ��

        GPIO_InitStruct.Pin       = UART_TX_PIN;      // TX����
        GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;  // �����������
        GPIO_InitStruct.Alternate = GPIO_AF7_USART3;
        GPIO_InitStruct.Pull  = GPIO_PULLUP;           // ����
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;  // �ٶȵȼ�
        HAL_GPIO_Init( UART_TX_PORT, &GPIO_InitStruct );

        GPIO_InitStruct.Pin  = UART_RX_PIN;  // RX����
        HAL_GPIO_Init( UART_RX_PORT, &GPIO_InitStruct );
    }
}

/*************************************************************************************************
 *	�� �� ��:	USART1_Init
 *	��ڲ���:	��
 *	�� �� ֵ:	��
 *	��������:	��ʼ����������
 *	˵    ��:	��
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
    HAL_UART_Transmit( &huart3, ( uint8_t* )pBuffer, size, HAL_MAX_DELAY );  // ���͵��ֽ�����
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