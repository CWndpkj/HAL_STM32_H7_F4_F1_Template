#ifdef __cplusplus
extern "C" {
#endif  // __cplusplus
#include "FreeRTOS.h"
#include "Handlers.h"
#include "delay.h"
#include "led.h"
//#include "stm32h7xx_hal.h"
#include "stm32f4xx_hal.h"
#include "sys.h"
#include "task.h"
#include "u8g2.h"
#include "uart.h"
TaskHandle_t handler;
void task_led_shine( void* parameter )
{
    while ( 1 ) {
        printf( "LED Shining\r\n" );
        LED_Shine();
        vTaskDelay( 500 );
    }
}
void start_task()
{
    printf( "start_task\r\n" );
    xTaskCreate( task_led_shine, "led_shine", 128, NULL, 1, &handler );
    vTaskStartScheduler();
}

int main()
{
    HAL_Init();
    //Cache_Enable();
    sys_stm32_clock_init( 336, 8, 2, 7 );
    LED_Init();
    UART_Init();
    delay_init( 168 );
    start_task();
    // QSPI_Helper w25q64;
    // w25q64.init();
    // w25q64.erase( Erase_type::chip_erase, 0x0 );
    // w25q64.volatile_SR_write_enable();
    // w25q64.write_reg( 3, 0x0 );
    // w25q64.enable_qpi_mode();
    // w25q64.enable_mem_map();
    // w25q64.disable_mem_map();
    // u8 buff[ 20 ] = { 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x30 };
    //  u8 reg_value  = w25q64.read_reg( 2 );
    //  w25q64.write_enable();
    // w25q64.erase( erase_type::setor_erase, 253 );
    //   w25q64.write_auto_shift_page( buff, 253, 20 );
    //   w25q64.write_auto_shift_page( buff, 266, 20 );
    // w25q64.write_len( buff, 0, 20 );
    // w25q64.write_no_check( buff, 0, 20 );
    // w25q64.erase( Erase_type::setor_erase, 0x0 );
    // w25q64.write_len( gImage_test1, 0, 64800 );

    // u8 buff_r[ 256 ];
    // w25q64.read_len( buff_r, 0, 256 );
    // printf( "************************\r\n" );
    // for (u32 i = 0; i < 256; i++)
    // {
    //     printf( "%02x ", buff_r[ i ] );
    // }
    // printf( "\r\n" );

    // for ( size_t i = 0; i < 256/4; i++ ) {
    //     u8 bit1 = ( *( ( u32* )0x90000000 + i ) ) >> 24;
    //     u8 bit2 = ( *( ( u32* )0x90000000 + i ) ) >> 16;
    //     u8 bit3 = ( *( ( u32* )0x90000000 + i ) ) >> 8;
    //     u8 bit4 = ( *( ( u32* )0x90000000 + i ) );
    //     printf( "%02x %02x %02x %02x ", bit4,bit3,bit2,bit1);
    // }
    while ( 1 ) {
        LED_Shine();
        delay_ms( 500 );
    }

    return 0;
}

#ifdef __cplusplus
}
#endif  // __cplusplus
