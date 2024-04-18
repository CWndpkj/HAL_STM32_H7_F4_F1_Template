// GPIO映射，根据自己的连接修改
#include "sys.h"

typedef enum {
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
} GPIO_t;

GPIO_TypeDef* GPIO_get_port( GPIO_t gpio );
uint16_t GPIO_get_pin( GPIO_t gpio );

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
#define SPI2_MISO_CLK_ENABLE()         \
    do {                              \
        __HAL_RCC_GPIOB_CLK_ENABLE(); \
    } while ( 0 );
#define SPI2_MOSI_CLK_ENABLE()         \
    do {                              \
        __HAL_RCC_GPIOB_CLK_ENABLE(); \
    } while ( 0 );

#define SPI3_SCK
#define SPI3_MISO
#define SPI3_MOSI

#define CUSTOM_STM32F103
#ifdef CUSTOM_STM32F103
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
#endif  // CUSTOM_STM32F103

#ifdef CUSTOM_STM32F407

#define SW4_Pin GPIO_PIN_1
#define SW4_GPIO_Port GPIOE
#define SW5_Pin GPIO_PIN_2
#define SW5_GPIO_Port GPIOE
#define SW6_Pin GPIO_PIN_3
#define SW6_GPIO_Port GPIOE
#define SW7_Pin GPIO_PIN_4
#define SW7_GPIO_Port GPIOE
#define SW8_Pin GPIO_PIN_5
#define SW8_GPIO_Port GPIOE
#define SW9_Pin GPIO_PIN_6
#define SW9_GPIO_Port GPIOE

#define LED1_Pin GPIO_PIN_13
#define LED1_GPIO_Port GPIOF
#define LED2_Pin GPIO_PIN_14
#define LED2_GPIO_Port GPIOF
#define LED3_Pin GPIO_PIN_15
#define LED3_GPIO_Port GPIOF
#define LED4_Pin GPIO_PIN_7
#define LED4_GPIO_Port GPIOD
#define LED5_Pin GPIO_PIN_10
#define LED5_GPIO_Port GPIOG
#define LED6_Pin GPIO_PIN_11
#define LED6_GPIO_Port GPIOG
#define LED7_Pin GPIO_PIN_13
#define LED7_GPIO_Port GPIOG
#define LED8_Pin GPIO_PIN_15
#define LED8_GPIO_Port GPIOG

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
