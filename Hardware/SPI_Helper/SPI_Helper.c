#include "SPI_Helper.h"
static SPI_DEV_t* dev;
static SPI_HandleTypeDef* handler;
/**
 *
 * @brief stm32f407zgt6 对应的SPI引脚资源
 * SPI1  NSS PA4,PA15,;SCK PA5,PB3;MISO PA6,PB4;MOSI PA7,PB5;
 * SPI2  NSS PB12,PB9;SCK PB13,PB10;MISO PB14,PC2;MOSI PB15,PC3
 * SPI3  NSS PA4,PA15,SCK PB3,PC10 ;MISO PB4,PC11;MOSI PB5,PC12
 */

void HAL_SPI_MspInit( SPI_HandleTypeDef* SPI_handler )
{
    if ( SPI_handler->Instance == SPI1 ) {
        __HAL_RCC_SPI1_CLK_ENABLE();
        __HAL_RCC_GPIOA_CLK_ENABLE();
        if ( (dev->SCK_Port == GPIOB )| (dev->MISO_Port == GPIOB )| (dev->MOSI_Port == GPIOB ) ){
            __HAL_RCC_GPIOB_CLK_ENABLE();
        }
        GPIO_InitTypeDef GPIO_InitStruct;
        GPIO_InitStruct.Alternate = GPIO_AF5_SPI1;
        GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull      = GPIO_NOPULL;
        GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
        GPIO_InitStruct.Pin       = dev->NSS_Pin;
        HAL_GPIO_Init( GPIOA, &GPIO_InitStruct );
        GPIO_InitStruct.Pin = dev->SCK_Pin;
        if ( (dev->SCK_Port) == GPIOA ) {
            HAL_GPIO_Init( GPIOA, &GPIO_InitStruct );
        }
        else if ( dev->SCK_Port == GPIOB ) {
            HAL_GPIO_Init( GPIOB, &GPIO_InitStruct );
        }

        GPIO_InitStruct.Pin = dev->MISO_Pin;
        if ( dev->MISO_Port == GPIOA ) {
            HAL_GPIO_Init( GPIOA, &GPIO_InitStruct );
        }
        else if ( dev->MISO_Port == GPIOB ) {
            HAL_GPIO_Init( GPIOB, &GPIO_InitStruct );
        }

        GPIO_InitStruct.Pin = dev->MOSI_Pin;
        if ( dev->MOSI_Port == GPIOA ) {
            HAL_GPIO_Init( GPIOA, &GPIO_InitStruct );
        }
        else if ( dev->MOSI_Port == GPIOB ) {
            HAL_GPIO_Init( GPIOB, &GPIO_InitStruct );
        }
    }
    else if ( SPI_handler->Instance == SPI2 ) {
        __HAL_RCC_SPI2_CLK_ENABLE();
        if ( (dev->SCK_Port == GPIOB) | (dev->MISO_Port == GPIOB) | (dev->MOSI_Port == GPIOB) ) {
            __HAL_RCC_GPIOB_CLK_ENABLE();
        }
        if ( (dev->SCK_Port == GPIOA) | (dev->MISO_Port == GPIOA) | (dev->MOSI_Port == GPIOA )) {
            __HAL_RCC_GPIOA_CLK_ENABLE();
        }
        GPIO_InitTypeDef GPIO_InitStruct;
        GPIO_InitStruct.Alternate = GPIO_AF5_SPI2;
        GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull      = GPIO_NOPULL;
        GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;

        GPIO_InitStruct.Pin = dev->NSS_Pin;
        if ( dev->NSS_Port == GPIOA ) {
            HAL_GPIO_Init( GPIOA, &GPIO_InitStruct );
        }
        else if ( dev->NSS_Port == GPIOB ) {
            HAL_GPIO_Init( GPIOB, &GPIO_InitStruct );
        }
        GPIO_InitStruct.Pin = dev->SCK_Pin;
        if ( dev->SCK_Port == GPIOA ) {
            HAL_GPIO_Init( GPIOA, &GPIO_InitStruct );
        }
        else if ( dev->SCK_Port == GPIOB ) {
            HAL_GPIO_Init( GPIOB, &GPIO_InitStruct );
        }

        GPIO_InitStruct.Pin = dev->MISO_Pin;
        if ( dev->MISO_Port == GPIOA ) {
            HAL_GPIO_Init( GPIOA, &GPIO_InitStruct );
        }
        else if ( dev->MISO_Port == GPIOB ) {
            HAL_GPIO_Init( GPIOB, &GPIO_InitStruct );
        }

        GPIO_InitStruct.Pin = dev->MOSI_Pin;
        if ( dev->MOSI_Port == GPIOA ) {
            HAL_GPIO_Init( GPIOA, &GPIO_InitStruct );
        }
        else if ( dev->MOSI_Port == GPIOB ) {
            HAL_GPIO_Init( GPIOB, &GPIO_InitStruct );
        }
    }
    else if ( SPI_handler->Instance == SPI3 ) {
        __HAL_RCC_SPI3_CLK_ENABLE();
        __HAL_RCC_GPIOA_CLK_ENABLE();
        if ( (dev->SCK_Port == GPIOB) | (dev->MISO_Port == GPIOB) | (dev->MOSI_Port == GPIOB )) {
            __HAL_RCC_GPIOB_CLK_ENABLE();
        }
        if ( (dev->SCK_Port == GPIOC) | (dev->MISO_Port == GPIOC) | (dev->MOSI_Port == GPIOC )) {
            __HAL_RCC_GPIOC_CLK_ENABLE();
        }
        GPIO_InitTypeDef GPIO_InitStruct;
        GPIO_InitStruct.Alternate = GPIO_AF6_SPI3;
        GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull      = GPIO_NOPULL;
        GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
        GPIO_InitStruct.Pin       = dev->NSS_Pin;
        HAL_GPIO_Init( GPIOA, &GPIO_InitStruct );
        GPIO_InitStruct.Pin = dev->SCK_Pin;
        if ( dev->SCK_Port == GPIOB ) {
            HAL_GPIO_Init( GPIOB, &GPIO_InitStruct );
        }
        else if ( dev->SCK_Port == GPIOC ) {
            HAL_GPIO_Init( GPIOC, &GPIO_InitStruct );
        }

        GPIO_InitStruct.Pin = dev->MISO_Pin;
        if ( dev->MISO_Port == GPIOB ) {
            HAL_GPIO_Init( GPIOB, &GPIO_InitStruct );
        }
        else if ( dev->MISO_Port == GPIOC ) {
            HAL_GPIO_Init( GPIOC, &GPIO_InitStruct );
        }

        GPIO_InitStruct.Pin = dev->MOSI_Pin;
        if ( dev->MOSI_Port == GPIOB ) {
            HAL_GPIO_Init( GPIOB, &GPIO_InitStruct );
        }
        else if ( dev->MOSI_Port == GPIOC ) {
            HAL_GPIO_Init( GPIOC, &GPIO_InitStruct );
        }
    }
}
void SPI_Helper_Init( SPI_HandleTypeDef* SPI_handler, SPI_DEV_t* SPI_dev )
{
    dev     = SPI_dev;
    handler = SPI_handler;

    handler->Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_16;
    handler->Init.CLKPhase          = SPI_PHASE_1EDGE;
    handler->Init.CLKPolarity       = SPI_POLARITY_HIGH;
    handler->Init.CRCCalculation    = SPI_CRCCALCULATION_DISABLE;
    handler->Init.CRCPolynomial     = 7;  // default
    handler->Init.DataSize          = SPI_DATASIZE_8BIT;
    handler->Init.Direction         = SPI_DIRECTION_2LINES;
    handler->Init.FirstBit          = SPI_FIRSTBIT_MSB;
    handler->Init.Mode              = SPI_MODE_MASTER;
    handler->Init.NSS               = SPI_NSS_SOFT;
    handler->Init.TIMode            = SPI_TIMODE_DISABLE;
    if ( SPI_dev->SPI_x == SPI_1 ) {
        handler->Instance = SPI1;
    }
    else if ( SPI_dev->SPI_x == SPI_2 ) {
        handler->Instance = SPI2;
    }
    else if ( SPI_dev->SPI_x == SPI_3 ) {
        handler->Instance = SPI3;
    }
    HAL_SPI_Init( handler );
}