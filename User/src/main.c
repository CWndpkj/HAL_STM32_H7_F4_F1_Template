#include "FreeRTOS.h"
#include "Handlers.h"
#include "delay.h"
#include "stm32h7xx_hal.h"
//#include "stm32f1xx_hal.h"
//#include "stm32f4xx_hal.h"
#include "sys.h"
#include "u8g2.h"
int main()
{
    HAL_Init();
    //sys_stm32_clock_init( );
    delay_init( 168 );
    while ( 1 ) {
        delay_ms( 500 );
    }

    return 0;
}