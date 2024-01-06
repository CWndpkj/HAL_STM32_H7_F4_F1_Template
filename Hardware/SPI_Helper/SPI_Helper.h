#ifndef __SPI_HELPER_H
#define __SPI_HELPER_H

#include "stm32f407xx.h"
#include "stm32f4xx_hal.h"
/**
 *
 * @brief stm32f407zgt6 对应的SPI引脚资源
 * SPI1  NSS PA4,PA15,;SCK PA5,PB3;MISO PA6,PB4;MOSI PA7,PB5;
 * SPI2  NSS PB12,PB9;SCK PB13,PB10;MISO PB14,PC2;MOSI PB15,PC3
 * SPI3  NSS PA4,PA15,SCK PB3,PC10 ;MISO PB4,PC11;MOSI PB5,PC12
 */

// 配置结构体
// 对应上述的资源，SPIx可以为 SPI1,SPI2,SPI3 或者它们的任意组合,Alter可以为0或者1,0对应前一个引脚配置，1对应后者
enum SPI_ID {
    SPI_1 = 0x1,
    SPI_2 = 0x2,
    SPI_3 = 0x4,
};

typedef struct SPI_DEV {
    enum SPI_ID SPI_x;
    GPIO_TypeDef* NSS_Port;
    GPIO_TypeDef* SCK_Port;
    GPIO_TypeDef* MISO_Port;
    GPIO_TypeDef* MOSI_Port;
    uint16_t NSS_Pin;
    uint16_t SCK_Pin;
    uint16_t MISO_Pin;
    uint16_t MOSI_Pin;
} SPI_DEV_t;

void SPI_Helper_Init( SPI_HandleTypeDef* SPI_handler, SPI_DEV_t* SPI_dev );

#endif  // !__SPI_HELPER_H