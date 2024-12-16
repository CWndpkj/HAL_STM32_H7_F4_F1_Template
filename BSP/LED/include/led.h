#ifndef __LED_H
#define __LED_H
#include "GPIO.h"

typedef enum {
    LED1=0,
    LED2,
    LED3,
    LED4,
    LED5,
    LED6,
    LED7,
    LED8,
} eLED_NUM;

void LED_Init();
void LED_On( eLED_NUM num );
void LED_Off( eLED_NUM num );
void LED_Toggle( eLED_NUM num );

#endif  //__LED_H
