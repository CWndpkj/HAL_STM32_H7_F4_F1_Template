#ifndef __EPD_DEFS_H
#define __EPD_DEFS_H

#define EPD_SPI SPI1

#define EPD_NSS_PORT GPIOA
#define EPD_NSS_PIN GPIO_PIN_4

#define EPD_SCK_PORT GPIOA
#define EPD_SCK_PIN GPIO_PIN_5

#define EPD_MOSI_PORT GPIOA
#define EPD_MOSI_PIN GPIO_PIN_7

#define EPD_MISO_PORT GPIOA
#define EPD_MISO_PIN GPIO_PIN_6

#define EPD_CONTROL_GPIO_CLK_ENABEL __HAL_RCC_GPIOG_CLK_ENABLE
#define EPD_CONTROL_PORT GPIOG
#define EPD_RST_PIN GPIO_PIN_9
#define EPD_DC_PIN GPIO_PIN_10
#define EPD_CS_PIN GPIO_PIN_11
#define EPD_BUSY_PIN GPIO_PIN_12

#define EPD_Digital_Write( _pin, _value ) HAL_GPIO_WritePin( EPD_CONTROL_PORT, _pin, _value == 0 ? GPIO_PIN_RESET : GPIO_PIN_SET )
#define EPD_Digital_Read( _pin ) HAL_GPIO_ReadPin( EPD_CONTROL_PORT, _pin )

#define EPD_WIDTH 400
#define EPD_HEIGHT 300

#endif  // !__EPD_DEFS_H