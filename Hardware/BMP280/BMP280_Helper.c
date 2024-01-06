#include "BMP280_Helper.h"
#include "Handlers.h"
#include "I2C_Helper.h"
#include "bmp280.h"
#include "delay.h"
#include <stdio.h>

extern I2C_HandleTypeDef hi2c3;
/*!
 *  @brief Prints the execution status of the APIs.
 *
 *  @param[in] api_name : name of the API whose execution status has to be printed.
 *  @param[in] rslt     : error code returned by the API whose execution status has to be printed.
 *
 *  @return void.
 */
void print_rslt( const char api_name[], int8_t rslt )
{
    if ( rslt != BMP280_OK ) {
        printf( "%s\t", api_name );
        if ( rslt == BMP280_E_NULL_PTR ) {
            printf( "Error [%d] : Null pointer error\r\n", rslt );
        }
        else if ( rslt == BMP280_E_COMM_FAIL ) {
            printf( "Error [%d] : Bus communication failed\r\n", rslt );
        }
        else if ( rslt == BMP280_E_IMPLAUS_TEMP ) {
            printf( "Error [%d] : Invalid Temperature\r\n", rslt );
        }
        else if ( rslt == BMP280_E_DEV_NOT_FOUND ) {
            printf( "Error [%d] : Device not found\r\n", rslt );
        }
        else {
            /* For more error codes refer "*_defs.h" */
            printf( "Error [%d] : Unknown error code\r\n", rslt );
        }
    }
}
int8_t bmp_read( uint8_t dev_id, uint8_t reg_addr, uint8_t* data, uint16_t len )
{
    return HAL_I2C_Mem_Read( &hi2c3, dev_id << 1, reg_addr, 1, data, len, HAL_MAX_DELAY );
}
int8_t bmp_write( uint8_t dev_id, uint8_t reg_addr, uint8_t* data, uint16_t len )
{
    return HAL_I2C_Mem_Write( &hi2c3, dev_id << 1, reg_addr, 1, data, len, HAL_MAX_DELAY );
}
void BMP280_Helper_Init()
{
    I2C_DEV_t i2c_dev;
    i2c_dev.I2C_x    = I2C_3;
    i2c_dev.SCL_Pin  = GPIO_PIN_8;
    i2c_dev.SDA_Pin  = GPIO_PIN_9;
    i2c_dev.SCL_Port = GPIOA;
    i2c_dev.SDA_Port = GPIOC;
    I2C_Helper_FailRecover( &i2c_dev );
    I2C_Helper_Init( &hi2c3, &i2c_dev );
}

void BMP_Mesure()
{
    int8_t rslt;
    struct bmp280_dev bmp;
    struct bmp280_config conf;
    struct bmp280_uncomp_data ucomp_data;
    uint32_t pres32, pres64;
    double pres;

    /* Map the delay function pointer with the function responsible for implementing the delay */
    bmp.delay_ms = delay_ms;

    /* Assign device I2C address based on the status of SDO pin (GND for PRIMARY(0x76) & VDD for SECONDARY(0x77)) */
    bmp.dev_id = BMP280_I2C_ADDR_SEC;

    /* Select the interface mode as I2C */
    bmp.intf = BMP280_I2C_INTF;

    /* Map the I2C read & write function pointer with the functions responsible for I2C bus transfer */
    bmp.read  = bmp_read;
    bmp.write = bmp_write;

    /* To enable SPI interface: comment the above 4 lines and uncomment the below 4 lines */

    /*
     * bmp.dev_id = 0;
     * bmp.read = spi_reg_read;
     * bmp.write = spi_reg_write;
     * bmp.intf = BMP280_SPI_INTF;
     */
    rslt = bmp280_init( &bmp );
    print_rslt( " bmp280_init status", rslt );

    /* Always read the current settings before writing, especially when
     * all the configuration is not modified
     */
    rslt = bmp280_get_config( &conf, &bmp );
    print_rslt( " bmp280_get_config status", rslt );

    /* configuring the temperature oversampling, filter coefficient and output data rate */
    /* Overwrite the desired settings */
    conf.filter = BMP280_FILTER_COEFF_16;

    /* Pressure oversampling set at 4x */
    conf.os_pres = BMP280_OS_16X;

    // 设置温度超采样
    conf.os_temp = BMP280_OS_2X;

    /* Setting the output data rate as 8HZ(125ms) */
    conf.odr = BMP280_ODR_125_MS;
    rslt     = bmp280_set_config( &conf, &bmp );
    print_rslt( " bmp280_set_config status", rslt );

    /* Always set the power mode after setting the configuration */
    rslt = bmp280_set_power_mode( BMP280_NORMAL_MODE, &bmp );
    print_rslt( " bmp280_set_power_mode status", rslt );
    while ( 1 ) {
        /* Reading the raw data from sensor */
        rslt = bmp280_get_uncomp_data( &ucomp_data, &bmp );

        /* Getting the compensated pressure using 32 bit precision */
        rslt = bmp280_get_comp_pres_32bit( &pres32, ucomp_data.uncomp_press, &bmp );

        /* Getting the compensated pressure using 64 bit precision */
        rslt = bmp280_get_comp_pres_64bit( &pres64, ucomp_data.uncomp_press, &bmp );

        /* Getting the compensated pressure as floating point value */
        rslt = bmp280_get_comp_pres_double( &pres, ucomp_data.uncomp_press, &bmp );
        printf( "UP: %ld, P32: %ld, P64: %ld, P64N: %ld, P: %f\r\n", ucomp_data.uncomp_press, pres32, pres64, pres64 / 256, pres );
        bmp.delay_ms( 1000 ); /* Sleep time between measurements = BMP280_ODR_1000_MS */
    }
}