#include "EPD_Helper.h"
#include "SPI_Helper.h"

extern SPI_HandleTypeDef hspi1;

void EPD_Init()
{
    // 初始化GPIO
    EPD_CONTROL_GPIO_CLK_ENABEL();
    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pin   = EPD_DC_PIN | EPD_CS_PIN | EPD_RST_PIN;
    GPIO_InitStruct.Pull  = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init( EPD_CONTROL_PORT, &GPIO_InitStruct );
    GPIO_InitStruct.Pin  = EPD_BUSY_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    HAL_GPIO_Init( EPD_CONTROL_PORT, &GPIO_InitStruct );
    SPI_DEV_t spi_dev;
    if ( EPD_SPI == SPI1 ) {
        spi_dev.SPI_x = SPI_1;
    }
    else if ( EPD_SPI == SPI2 ) {
        spi_dev.SPI_x = SPI_2;
    }
    else if ( EPD_SPI == SPI3 ) {
        spi_dev.SPI_x = SPI_3;
    }
    spi_dev.NSS_Port  = EPD_NSS_PORT;
    spi_dev.NSS_Pin   = EPD_NSS_PIN;
    spi_dev.SCK_Port  = EPD_SCK_PORT;
    spi_dev.SCK_Pin   = EPD_SCK_PIN;
    spi_dev.MOSI_Port = EPD_MOSI_PORT;
    spi_dev.MOSI_Pin  = EPD_MOSI_PIN;
    spi_dev.MISO_Port = EPD_MISO_PORT;
    spi_dev.MISO_Pin  = EPD_MISO_PIN;
    SPI_Helper_Init( &hspi1, &spi_dev );
}

uint8_t EPD_SPI_WriteByte( uint8_t buff )
{
    return HAL_SPI_Transmit( &hspi1, &buff, 1, 100 );
}
