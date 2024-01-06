#ifndef __EPD_HELPER_H
#define __EPD_HELPER_H

#include "EPD_defs.h"
#include "stm32f4xx_hal.h"
#include <stdint.h>
#include <stdio.h>


// #define EPD_2IN36

// #if defined( EPD_1IN02 )
// #include "./EPD_2in02/EPD_1in02d.h"
// #elif defined( EPD_1IN54 )
// #include "./EPD_1in54/EPD_1in54.h"
// #elif defined( EPD_1IN64 )
// #include "./EPD_1in64/EPD_1in64g.h"
// #elif defined( EPD_2IN7 )
// #include "./EPD_2in7/EPD_2in7.h"
// #elif defined( EPD_2IN9 )
// #include "./EPD_2in02/EPD_2in9.h"
// #elif defined( EPD_2IN13 )
// #include "./EPD_2in7/EPD_2in13.h"
// #elif defined( EPD_2IN36 )
// #include "./EPD_2in36/EPD_2in36g.h"
// #elif defined( EPD_2IN66 )
// #include "./EPD_2in66/EPD_2in66.h"
// #elif defined( EPD_3IN0 )
// #include "./EPD_3in0/EPD_3in0g.h"
// #elif defined( EPD_3IN7 )
// #include "./EPD_3in7/EPD_3in7.h"
// #elif defined( EPD_3IN52 )
// #include "./EPD_3in52/EPD_3in52.h"
// #elif defined( EPD_4IN01 )
// #include "./EPD_2in02/EPD_4in01f.h"
// #elif defined( EPD_4IN2 )
// #include "./EPD_4in2/EPD_4in2.h"
// #elif defined( EPD_4IN37 )
// #include "./EPD_4in37/EPD_4in37g.h"
// #elif defined( EPD_5IN65 )
// #include "./EPD_5in56/EPD_5in65f.h"
// #elif defined( EPD_5IN83 )
// #include "./EPD_5in83/EPD_5in83.h"
// #elif defined( EPD_7IN3 )
// #include "./EPD_7in3/EPD_7in3f.h"
// #elif defined( EPD_7IN5 )
// #include "./EPD_7in5/EPD_7in5.h"
// #else
// #error "Please select the size of e-ink"
// #endif

void EPD_Init();
uint8_t EPD_SPI_WriteByte( uint8_t buff );
#endif