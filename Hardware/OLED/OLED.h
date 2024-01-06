#ifndef __OLED_H
#define __OLED_H
#include "stm32f4xx_hal.h"
#include "u8g2.h"
#include "u8x8.h"
extern u8g2_t u8g2;

#define OLED_PORT GPIOB
#define OLED_SCL GPIO_PIN_6
#define OLED_SDA GPIO_PIN_7

void OLED_Init();

#endif