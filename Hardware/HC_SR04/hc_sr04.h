#ifndef __HC_SR04_H
#define __HC_SR04_H

#include "stm32f4xx_hal.h"

#define ECHO_PORT GPIOD
#define ECHO_PORT_CLK_ENABLE __HAL_RCC_GPIOD_CLK_ENABLE
#define ECHO_PIN GPIO_PIN_11
#define TRIG_PORT GPIOD
#define TRIG_PORT_CLK_ENABLE __HAL_RCC_GPIOD_CLK_ENABLE
#define TRIG_PIN GPIO_PIN_12
#define HC_SR04_TIM TIM1
#define HS_SR04_TIM_CLK_ENABLE __HAL_RCC_TIM1_CLK_ENABLE
#define HC_SR04_TIM_CHANNEL TIM_CHANNEL_1
#define HC_SR04_TIM_CHANNEL_GPIO_CLK_ENABLE __HAL_RCC
void hc_sr04_init();
void hc_sr04_set_cb( void ( *cb )( uint32_t dis ) );

void hc_sr04_trig();
float hc_sr04_get_dis();

#endif  // !__HC_SR04_H
