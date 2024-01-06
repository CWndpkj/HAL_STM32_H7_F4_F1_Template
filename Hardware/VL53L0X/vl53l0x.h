#ifndef __VL53L0X_H
#define __VL53L0X_H
#include "stm32f4xx_hal.h"
#include "vl53l0x_api.h"
#include "vl53l0x_platform.h"

extern I2C_HandleTypeDef hi2c3;
extern VL53L0X_Dev_t vl53l0x_dev;

typedef enum {
    LONG_RANGE    = 0, /*!< Long range mode */
    HIGH_SPEED    = 1, /*!< High speed mode */
    HIGH_ACCURACY = 2, /*!< High accuracy mode */
} RangingConfig_e;
void vl53l0x_init();

float vl53l0x_get_dis();

#endif  // !__VL53L0X_H
