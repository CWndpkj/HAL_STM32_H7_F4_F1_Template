#ifndef __UART_H
#define __UART_H

#include "stdio.h"
#include "stm32f4xx_hal.h"

#define UART_BaudRate 115200

#define USARTx USART3
#define UART_TX_PORT GPIOB
#define UART_TX_PIN GPIO_PIN_10

#define UART_RX_PORT GPIOB
#define UART_RX_PIN GPIO_PIN_11

#define UART_CLK_ENABLE __HAL_RCC_USART3_CLK_ENABLE
#define UART_TX_GPIO_CLK_ENABLE __HAL_RCC_GPIOB_CLK_ENABLE
#define UART_RX_GPIO_CLK_ENABLE __HAL_RCC_GPIOB_CLK_ENABLE

void UART_Init( void );
void PrintFloat( float value );

#endif  //__USART_H
