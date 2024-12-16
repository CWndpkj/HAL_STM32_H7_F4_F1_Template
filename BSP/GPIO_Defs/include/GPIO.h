#pragma once
// GPIO映射，根据自己的连接修改
#include "sys.h"

#define STM32F103C8T6_BLUE_PILL 0
#define STM32F407ZGT6_DIY 1
#define STM32H750XBH6_FK 2

// TODO: Change this board define
#define USE_BOARD STM32F407ZGT6_DIY

typedef enum {
#if USE_BOARD > -1
    A00,
    A01,
    A02,
    A03,
    A04,
    A05,
    A06,
    A07,
    A08,
    A09,
    A10,
    A11,
    A12,
    A13,
    A14,
    A15,
    B00,
    B01,
    B02,
    B03,
    B04,
    B05,
    B06,
    B07,
    B08,
    B09,
    B10,
    B11,
    B12,
    B13,
    B14,
    B15,
    C00,
    C01,
    C02,
    C03,
    C04,
    C05,
    C06,
    C07,
    C08,
    C09,
    C10,
    C11,
    C12,
    C13,
    C14,
    C15,
#endif
#if USE_BOARD > 0
    D00,
    D01,
    D02,
    D03,
    D04,
    D05,
    D06,
    D07,
    D08,
    D09,
    D10,
    D11,
    D12,
    D13,
    D14,
    D15,
    E00,
    E01,
    E02,
    E03,
    E04,
    E05,
    E06,
    E07,
    E08,
    E09,
    E10,
    E11,
    E12,
    E13,
    E14,
    E15,
    F00,
    F01,
    F02,
    F03,
    F04,
    F05,
    F06,
    F07,
    F08,
    F09,
    F10,
    F11,
    F12,
    F13,
    F14,
    F15,
    G00,
    G01,
    G02,
    G03,
    G04,
    G05,
    G06,
    G07,
    G08,
    G09,
    G10,
    G11,
    G12,
    G13,
    G14,
    G15,
#endif
} GPIO_t;

GPIO_TypeDef* GPIO_get_port( GPIO_t gpio );
uint16_t GPIO_get_pin( GPIO_t gpio );

#if USE_BOARD == 0
// pin map
#define I2C1_SCL B06
#define I2C1_SDA B07
#define I2C1_SCL_CLK_ENABLE()         \
    do {                              \
        __HAL_RCC_GPIOB_CLK_ENABLE(); \
    } while ( 0 )
#define I2C1_SDA_CLK_ENABLE()         \
    do {                              \
        __HAL_RCC_GPIOB_CLK_ENABLE(); \
    } while ( 0 )

#define I2C2_SCL
#define I2C2_SDA

#define I2C3_SCL
#define I2C3_SDA

#define SPI1_SCK
#define SPI1_MISO
#define SPI1_MOSI

#define SPI2_SCK B13
#define SPI2_MISO B14
#define SPI2_MOSI B15
#define SPI2_SCK_CLK_ENABLE()         \
    do {                              \
        __HAL_RCC_GPIOB_CLK_ENABLE(); \
    } while ( 0 );
#define SPI2_MISO_CLK_ENABLE()        \
    do {                              \
        __HAL_RCC_GPIOB_CLK_ENABLE(); \
    } while ( 0 );
#define SPI2_MOSI_CLK_ENABLE()        \
    do {                              \
        __HAL_RCC_GPIOB_CLK_ENABLE(); \
    } while ( 0 );

#define SPI3_SCK
#define SPI3_MISO
#define SPI3_MOSI

// NRF24L01
#define NRF_CE4 B09
#define NRF_SCK SPI2_SCK
#define NRF_MISO SPI2_MISO
#define NRF_MOSI SPI2_MOSI
#define NRF_IRQ B08
#define NRF_CS A08
// WS2812
#define WS2812_MOSI B15
// OLED
#define OLED_SCL I2C1_SCL
#define OLED_SDA I2C1_SDA
// W25Q64
#define W25Q64_CS B12
#define W25Q64_MISO SPI2_MISO
#define W25Q64_MOSI SPI2_MOSI
#define W25Q64_SCK SPI3_SCK

// 74HC165D
#define _4175_KEY1_BIT 0
#define _4175_KEY2_BIT 1
#define _4175_KEY3_BIT 2
#define _4175_KEY4_BIT 3
#define SWITCH_BIT 4
#define EC11_KEY_BIT 5
#define _4175_KEY5_BIT 6

#define NDS1_BIT 8
#define NDS2_BIT 9
#define NDS3_BIT 10
#define NDS4_BIT 11

#define AUX1_BIT 12
#define AUX2_BIT 13
#define AUX3_BIT 14
#define AUX4_BIT 15

#define _74HC165D_LOAD B03
#define _74HC165D_CLK B04
#define _74HC165D_SERIAL_DATA B05

// ADC
#define LEFT_ROCKER_LR_ADC A02  // 左摇杆，左右通道
#define LEFT_ROCKER_UD_ADC A01  // 左摇杆，上下通道

#define RIGHT_ROCKER_LR_ADC A04
#define RIGHT_ROCKER_UD_ADC A03

#define BAT_VOLTAGE_ADC A00  // 电池电压ADC

// USART1
#define USART1_TX A09
#define USART1_RX A10
#define USART1_TX_GPIO_CLK_ENABLE()   \
    do {                              \
        __HAL_RCC_GPIOA_CLK_ENABLE(); \
    } while ( 0 )

#define USART1_RX_GPIO_CLK_ENABLE()   \
    do {                              \
        __HAL_RCC_GPIOA_CLK_ENABLE(); \
    } while ( 0 )
/* 接收引脚时钟使能 */

// USART3
#define USART3_TX B10
#define USART3_RX B11
#define USART3_TX_GPIO_CLK_ENABLE()   \
    do {                              \
        __HAL_RCC_GPIOB_CLK_ENABLE(); \
    } while ( 0 )

#define USART3_RX_GPIO_CLK_ENABLE()   \
    do {                              \
        __HAL_RCC_GPIOB_CLK_ENABLE(); \
    } while ( 0 )
// TIM
#define ENCODER_CH1 A06
#define ENCODER_CH2 A07
#endif

// #############################

#if USE_BOARD == 1
#define SW4_Pin E01
#define SW5_Pin E02
#define SW6_Pin E03
#define SW7_Pin E04
#define SW8_Pin E05
#define SW9_Pin E06

#define LED1_Pin F13
#define LED2_Pin F14
#define LED3_Pin F15
#define LED4_Pin D07
#define LED5_Pin G10
#define LED6_Pin G11
#define LED7_Pin G13
#define LED8_Pin G15
#endif

// USART3
#define USART3_TX B10
#define USART3_RX B11
#define USART3_TX_GPIO_CLK_ENABLE()   \
    do {                              \
        __HAL_RCC_GPIOB_CLK_ENABLE(); \
    } while ( 0 )

#define USART3_RX_GPIO_CLK_ENABLE()   \
    do {                              \
        __HAL_RCC_GPIOB_CLK_ENABLE(); \
    } while ( 0 )
// ####################################################################

#if USE_BOARD == 2
/*----------------------------------------------- 引脚配置宏 ------------------------------------------*/

#define QUADSPI_CLK_PIN GPIO_PIN_10                           // QUADSPI_CLK 引脚
#define QUADSPI_CLK_PORT GPIOF                                // QUADSPI_CLK 引脚端口
#define QUADSPI_CLK_AF GPIO_AF9_QUADSPI                       // QUADSPI_CLK IO口复用
#define GPIO_QUADSPI_CLK_ENABLE __HAL_RCC_GPIOF_CLK_ENABLE()  // QUADSPI_CLK 引脚时钟

#define QUADSPI_BK1_NCS_PIN GPIO_PIN_6                            // QUADSPI_BK1_NCS 引脚
#define QUADSPI_BK1_NCS_PORT GPIOG                                // QUADSPI_BK1_NCS 引脚端口
#define QUADSPI_BK1_NCS_AF GPIO_AF10_QUADSPI                      // QUADSPI_BK1_NCS IO口复用
#define GPIO_QUADSPI_BK1_NCS_ENABLE __HAL_RCC_GPIOG_CLK_ENABLE()  // QUADSPI_BK1_NCS 引脚时钟

#define QUADSPI_BK1_IO0_PIN GPIO_PIN_8                            // QUADSPI_BK1_IO0 引脚
#define QUADSPI_BK1_IO0_PORT GPIOF                                // QUADSPI_BK1_IO0 引脚端口
#define QUADSPI_BK1_IO0_AF GPIO_AF10_QUADSPI                      // QUADSPI_BK1_IO0 IO口复用
#define GPIO_QUADSPI_BK1_IO0_ENABLE __HAL_RCC_GPIOF_CLK_ENABLE()  // QUADSPI_BK1_IO0 引脚时钟

#define QUADSPI_BK1_IO1_PIN GPIO_PIN_9                            // QUADSPI_BK1_IO1 引脚
#define QUADSPI_BK1_IO1_PORT GPIOF                                // QUADSPI_BK1_IO1 引脚端口
#define QUADSPI_BK1_IO1_AF GPIO_AF10_QUADSPI                      // QUADSPI_BK1_IO1 IO口复用
#define GPIO_QUADSPI_BK1_IO1_ENABLE __HAL_RCC_GPIOF_CLK_ENABLE()  // QUADSPI_BK1_IO1 引脚时钟

#define QUADSPI_BK1_IO2_PIN GPIO_PIN_7                            // QUADSPI_BK1_IO2 引脚
#define QUADSPI_BK1_IO2_PORT GPIOF                                // QUADSPI_BK1_IO2 引脚端口
#define QUADSPI_BK1_IO2_AF GPIO_AF9_QUADSPI                       // QUADSPI_BK1_IO2 IO口复用
#define GPIO_QUADSPI_BK1_IO2_ENABLE __HAL_RCC_GPIOF_CLK_ENABLE()  // QUADSPI_BK1_IO2 引脚时钟

#define QUADSPI_BK1_IO3_PIN GPIO_PIN_6                            // QUADSPI_BK1_IO3 引脚
#define QUADSPI_BK1_IO3_PORT GPIOF                                // QUADSPI_BK1_IO3 引脚端口
#define QUADSPI_BK1_IO3_AF GPIO_AF9_QUADSPI                       // QUADSPI_BK1_IO3 IO口复用
#define GPIO_QUADSPI_BK1_IO3_ENABLE __HAL_RCC_GPIOF_CLK_ENABLE()  // QUADSPI_BK1_IO3 引脚时钟
#endif