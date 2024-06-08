#include "FreeRTOS.h"
#include "delay.h"
#include "stm32h7xx_hal.h"
// #include "stm32f1xx_hal.h"
// #include "stm32f4xx_hal.h"
#include "sys.h"
#include "u8g2.h"

int main()
{
    HAL_Init();
    Stm32_Clock_Init( 160, 5, 2, 2 );
    delay_init( 480 );
    while (1) {
        delay_ms( 500 );
    }
    return 0;
}