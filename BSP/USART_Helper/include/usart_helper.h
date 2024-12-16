#pragma once
#include "GPIO.h"
#include "error.h"
#include "sys.h"

#define USART_DEBUG USART3
#define USART_DEBUG_CLK_ENABEL __HAL_RCC_USART3_CLK_ENABLE
#define USART_DEBUG_BAUD 115200
#define USART_DEBUG_TX USART3_TX
#define USART_DEBUG_RX USART3_RX
#define USART_DEBUG_GPIO_AF GPIO_AF7_USART3
#define USART_DEBUG_TX_CLK_ENABLE USART3_TX_GPIO_CLK_ENABLE
#define USART_DEBUG_RX_CLK_ENABLE USART3_RX_GPIO_CLK_ENABLE

#define USART_DEBUG_IRQn USART3_IRQn
#define USART_DEBUG_IRQHandler USART3_IRQHandler

#define USART_REC_LEN 200 /* 定义最大接收字节数 200 */
#define USART_EN_RX 1     /* 使能（1）/禁止（0）串口1接收 */
#define RXBUFFERSIZE 128  /* 缓存大小 */

typedef struct {
    void ( *rcv_complete_cb )( uint8_t* data );  // 长数据接受完成回调函数
    uint8_t* rcv_buffer_0;                       // 长数据接收buffer1
    uint8_t* rcv_buffer_1;                       // 长数据接收buffer2
    uint8_t use_muti_buffer;                     // 是否使用双缓冲
    uint32_t last_transfer_timestamp;            // 上一次传输的时间戳，用于在10s没有数据传输时自动退出数据模式返回指令模式
} Data_rcv_handler_t;
extern Data_rcv_handler_t data_rcv_handler;  // 数据接收句柄,定义在usmart_config.c中

extern uint8_t g_usart_rx_buf[ USART_REC_LEN ]; /* 接收缓冲,最大USART_REC_LEN个字节.末字节为换行符 */
extern uint16_t g_usart_rx_sta;                 /* 接收状态标记 */
extern uint8_t g_rx_buffer[ RXBUFFERSIZE ];     /* HAL库USART接收Buffer */

error_t usart_debug_write( const u8* data, u32 length );

error_t usart_debug_init();