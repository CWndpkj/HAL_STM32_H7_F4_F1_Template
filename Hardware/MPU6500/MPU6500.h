#ifndef __MPU6500_H
#define __MPU6500_H
#include "stm32f4xx_hal.h"

/**
 * @brief 注意调节采样输出频率和获取FIFO数据的频率,当获取FIFO数据小于采样输出频率时，
 * FIFO将发生溢出，导致读取失败，读取到的数全为0
 */
void MPU6500_Init();
uint8_t MPU6500_Set_LPF( uint16_t lpf );
uint8_t MPU6500_run_self_test( void );
uint8_t MPU6500_Set_Rate( uint16_t rate );
uint8_t MPU6500_DMP_Init( void );
uint8_t MPU6500_dmp_get_euler_angle( short* accel, short* gyro, float* pitch, float* roll, float* yaw );
uint8_t MPU6500_GetDeviceID();
uint8_t MPU6500_Set_Gyro_Fsr( uint8_t fsr );
uint8_t MPU6500_Set_Accel_Fsr( uint8_t fsr );
uint8_t MPU6500_Set_Rate( uint16_t rate );
uint8_t MPU6500_Set_LPF( uint16_t lpf );
uint8_t MPU6500_run_self_test( void );

// uint8_t MPU6500_Send2Host(uint16_t ACCEL_XOUT, uint16_t ACCEL_YOUT, uint16_t ACCEL_ZOUT, uint16_t GYRO_XOUT, uint16_t GYRO_YOUT, uint16_t GYRO_ZOUT);
#endif  // !_MPU6500_H_
