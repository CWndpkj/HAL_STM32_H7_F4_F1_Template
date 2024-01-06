#include "MPU6500.h"
#include "I2C_Helper.h"
#include "UART.h"
#include "delay.h"
#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h"
#include "math.h"
#include "math_fun.h"
// 定义MPU6500寄存器地址
#define MPU6500_SELF_TEST_X_GYRO 0x00
#define MPU6500_SELF_TEST_Y_GYRO 0x01
#define MPU6500_SELF_TEST_Z_GYRO 0x02
#define MPU6500_SELF_TEST_X_ACCEL 0x0D
#define MPU6500_SELF_TEST_Y_ACCEL 0x0E
#define MPU6500_SELF_TEST_Z_ACCEL 0x0F
#define MPU6500_XG_OFFSET_H 0x13
#define MPU6500_XG_OFFSET_L 0x14
#define MPU6500_YG_OFFSET_H 0x15
#define MPU6500_YG_OFFSET_L 0x16
#define MPU6500_ZG_OFFSET_H 0x17
#define MPU6500_ZG_OFFSET_L 0x18
#define MPU6500_SMPLRT_DIV 0x19
#define MPU6500_CONFIG 0x1A
#define MPU6500_GYRO_CONFIG 0x1B
#define MPU6500_ACCEL_CONFIG 0x1C
#define MPU6500_ACCEL_CONFIG2 0x1D
#define MPU6500_FIFO_EN 0x23
#define MPU6500_INT_PIN_CFG 0x37
#define MPU6500_INT_ENABLE 0x38
#define MPU6500_INT_STATUS 0x3A
#define MPU6500_ACCEL_XOUT_H 0x3B
#define MPU6500_ACCEL_XOUT_L 0x3C
#define MPU6500_ACCEL_YOUT_H 0x3D
#define MPU6500_ACCEL_YOUT_L 0x3E
#define MPU6500_ACCEL_ZOUT_H 0x3F
#define MPU6500_ACCEL_ZOUT_L 0x40
#define MPU6500_TEMP_OUT_H 0x41
#define MPU6500_TEMP_OUT_L 0x42
#define MPU6500_GYRO_XOUT_H 0x43
#define MPU6500_GYRO_XOUT_L 0x44
#define MPU6500_GYRO_YOUT_H 0x45
#define MPU6500_GYRO_YOUT_L 0x46
#define MPU6500_GYRO_ZOUT_H 0x47
#define MPU6500_GYRO_ZOUT_L 0x48
#define MPU6500_PWR_MGMT_1 0x6B
#define MPU6500_PWR_MGMT_2 0x6C
#define MPU6500_WHO_AM_I 0x75
#define MPU6500_ADDR 0x68

// MPU输出速率(最大不超过200Hz)
#define DEFAULT_MPU_HZ 100  // 100Hz

extern I2C_HandleTypeDef hi2c3;
// 设置传感器的方向
/* Platform-specific information. Kinda like a boardfile. */
struct platform_data_s {
    signed char orientation[ 9 ];
};

/* The sensors can be mounted onto the board in any orientation. The mounting
 * matrix seen below tells the MPL how to rotate the raw data from the
 * driver(s).
 * TODO: The following matrices refer to the configuration on internal test
 * boards at Invensense. If needed, please modify the matrices to match the
 * chip-to-body matrix for your particular set up.
 */
static struct platform_data_s gyro_pdata = { .orientation = { 0, -1, 0, 1, 0, 0, 0, 0, 1 } };

/*    Axis Transformation matrix
|r11  r12  r13| |vx|     |v'x|
|r21  r22  r23| |vy|  =  |v'y|
|r31  r32  r33| |vz|     |v'z|
v'x  = {(r11  * vx) +( r12 * vy) +( r13 * vz)}
v'y  = {(r21  * vx) +( r22 * vy) +( r23 * vz)}
v'z  = {(r31  * vx) +( r32 * vy) +( r33 * vz)}
*/

/**
 * @brief MPU6500初始化函数
 */
void MPU6500_Init()
{
    I2C_DEV_t i2c_dev;
    i2c_dev.I2C_x    = I2C_3;
    i2c_dev.SCL_Pin  = GPIO_PIN_8;
    i2c_dev.SDA_Pin  = GPIO_PIN_9;
    i2c_dev.SCL_Port = GPIOA;
    i2c_dev.SDA_Port = GPIOC;
    I2C_Helper_FailRecover( &i2c_dev );
    I2C_Helper_Init( &hi2c3, &i2c_dev );

    uint8_t ret= MPU6500_DMP_Init();
    printf( "init ret:%d\r\n", ret );
}

/**
 * @brief MPU6500获取器件ID,通常用于测试
 * @return uint8_t 返回器件地址，固定为0x70若返回值不一致，说明通讯错误
 */
uint8_t MPU6500_GetDeviceID()
{
    uint8_t Addr;
    HAL_I2C_Mem_Read( &hi2c3, MPU6500_ADDR << 1, MPU6500_WHO_AM_I, 1, &Addr, 1, HAL_MAX_DELAY );
    return Addr;
}

/**
 * @brief MPU6500,DMP初始化函数
 * @return uint8_t 返回初始化结果 返回0为初始化成功
 */
uint8_t MPU6500_DMP_Init( void )
{
    struct int_param_s int_param;
    int result;
    result = mpu_init( &int_param );
    if ( result )
        return 1;
    result = mpu_set_sensors( INV_XYZ_GYRO | INV_XYZ_ACCEL );  // 设置所需要的传感器
    if ( result )
        return 2;
    result = mpu_configure_fifo( INV_XYZ_GYRO | INV_XYZ_ACCEL );  // 设置FIFO
    if ( result )
        return 3;
    result = mpu_set_sample_rate( DEFAULT_MPU_HZ );  // 设置采样率
    if ( result )
        return 4;
    result = dmp_load_motion_driver_firmware();  // 加载dmp固件
    if ( result )
        return 5;
    result = dmp_set_orientation( inv_orientation_matrix_to_scalar( gyro_pdata.orientation ) );  // 设置陀螺仪方向
    if ( result )
        return 6;
    result =
        dmp_enable_feature( DMP_FEATURE_TAP | DMP_FEATURE_ANDROID_ORIENT | DMP_FEATURE_6X_LP_QUAT | DMP_FEATURE_GYRO_CAL | DMP_FEATURE_SEND_RAW_ACCEL | DMP_FEATURE_SEND_RAW_GYRO );  // 设置dmp功能
    if ( result )
        return 7;
    result = dmp_set_fifo_rate( DEFAULT_MPU_HZ );  // 设置DMP输出速率(最大不超过200Hz)
    if ( result )
        return 8;
    result = MPU6500_run_self_test();  // 自检
    if ( result )
        return 9;
    result = mpu_set_dmp_state( 1 );  // 使能DMP
    if ( result )
        return 10;
    result = dmp_set_interrupt_mode( DMP_INT_CONTINUOUS );  // 设置中断产生方式
    if ( result )
        return 11;
    return 0;
}

// 得到dmp处理后的数据(注意,本函数需要比较多堆栈,局部变量有点多)
// pitch:俯仰角 精度:0.1°   范围:-90.0° <---> +90.0°
// roll:横滚角  精度:0.1°   范围:-180.0°<---> +180.0°
// yaw:航向角   精度:0.1°   范围:-180.0°<---> +180.0°
// 返回值:0,正常
//     其他,失败
uint8_t MPU6500_dmp_get_euler_angle( short* accel, short* gyro, float* pitch, float* roll, float* yaw )
{
// q30格式,long转float时的除数.
#define Q30 ( ( 1 << 30 ) * 1.0f )

    float q0 = 1.0f, q1 = 0.0f, q2 = 0.0f, q3 = 0.0f;
    unsigned long sensor_timestamp;
    short sensors;
    unsigned char more;
    long quat[ 4 ];
    uint8_t result = 0;
    result         = dmp_read_fifo( gyro, accel, quat, &sensor_timestamp, &sensors, &more );
    if ( result )
        return 1;
    /* Gyro and accel data are written to the FIFO by the DMP in chip frame and hardware units.
     * This behavior is convenient because it keeps the gyro and accel outputs of dmp_read_fifo and mpu_read_fifo consistent.
     **/
    /*if (sensors & INV_XYZ_GYRO )
    send_packet(PACKET_TYPE_GYRO, gyro);
    if (sensors & INV_XYZ_ACCEL)
    send_packet(PACKET_TYPE_ACCEL, accel); */
    /* Unlike gyro and accel, quaternions are written to the FIFO in the body frame, q30.
     * The orientation is set by the scalar passed to dmp_set_orientation during initialization.
     **/
    if ( sensors & INV_WXYZ_QUAT ) {
        q0 = quat[ 0 ] / Q30;  // q30格式转换为浮点数
        q1 = quat[ 1 ] / Q30;
        q2 = quat[ 2 ] / Q30;
        q3 = quat[ 3 ] / Q30;
        // 计算得到俯仰角/横滚角/航向角
        *pitch = asin( -2 * q1 * q3 + 2 * q0 * q2 ) * 57.3;                                         // pitch
        *roll  = atan2( 2 * q2 * q3 + 2 * q0 * q1, -2 * q1 * q1 - 2 * q2 * q2 + 1 ) * 57.3;         // roll
        *yaw   = atan2( 2 * ( q1 * q2 + q0 * q3 ), q0 * q0 + q1 * q1 - q2 * q2 - q3 * q3 ) * 57.3;  // yaw
    }
    else
        return 2;
    return 0;
}

// 设置MPU6050陀螺仪传感器满量程范围
// fsr:0,±250dps;1,±500dps;2,±1000dps;3,±2000dps
// 返回值:0,设置成功
//     其他,设置失败
uint8_t MPU6500_Set_Gyro_Fsr( uint8_t fsr )
{
    uint8_t buff = fsr << 3;
    return HAL_I2C_Mem_Write( &hi2c3, MPU6500_ADDR << 1, MPU6500_GYRO_CONFIG, 1, &buff, 1, HAL_MAX_DELAY );
}

// 设置MPU6050加速度传感器满量程范围
// fsr:0,±2g;1,±4g;2,±8g;3,±16g
// 返回值:0,设置成功
//     其他,设置失败
uint8_t MPU6500_Set_Accel_Fsr( uint8_t fsr )
{
    uint8_t buff = fsr << 3;
    return HAL_I2C_Mem_Write( &hi2c3, MPU6500_ADDR << 1, MPU6500_ACCEL_CONFIG, 1, &buff, 1, HAL_MAX_DELAY );
}

// 设置MPU6050的采样率(假定Fs=1KHz)
// rate:4~1000(Hz)
// 返回值:0,设置成功
//     其他,设置失败
uint8_t MPU6500_Set_Rate( uint16_t rate )
{
    uint8_t data;
    if ( rate > 1000 )
        rate = 1000;
    if ( rate < 4 )
        rate = 4;
    data = 1000 / rate - 1;
    // 设置数字低通滤波器
    HAL_I2C_Mem_Write( &hi2c3, MPU6500_ADDR << 1, MPU6500_SMPLRT_DIV, 1, &data, 1, HAL_MAX_DELAY );
    // 自动设置LPF为采样率的一半
    return MPU6500_Set_LPF( rate / 2 );
}

// 设置MPU6050的数字低通滤波器
// lpf:数字低通滤波频率(Hz)
// 返回值:0,设置成功
//     其他,设置失败
uint8_t MPU6500_Set_LPF( uint16_t lpf )
{
    uint8_t data = 0;
    if ( lpf >= 188 )
        data = 1;
    else if ( lpf >= 98 )
        data = 2;
    else if ( lpf >= 42 )
        data = 3;
    else if ( lpf >= 20 )
        data = 4;
    else if ( lpf >= 10 )
        data = 5;
    else
        data = 6;
    // 设置数字低通滤波器
    return HAL_I2C_Mem_Write( &hi2c3, MPU6500_ADDR << 1, MPU6500_CONFIG, 1, &data, 1, HAL_MAX_DELAY );
}

// MPU6050自测试
// 返回值:0,正常
//     其他,失败
uint8_t MPU6500_run_self_test( void )
{
    int result;
    // char test_packet[4] = {0};
    long gyro[ 3 ], accel[ 3 ];
    result = mpu_run_self_test( gyro, accel );
    if ( result == 0x07 ) {
        /* Test passed. We can trust the gyro data here, so let's push it down
         * to the DMP.
         */
        float gyro_sens;
        unsigned short accel_sens;
        mpu_get_gyro_sens( &gyro_sens );
        gyro[ 0 ] = ( long )( gyro[ 0 ] * gyro_sens );
        gyro[ 1 ] = ( long )( gyro[ 1 ] * gyro_sens );
        gyro[ 2 ] = ( long )( gyro[ 2 ] * gyro_sens );
        dmp_set_gyro_bias( gyro );
        mpu_get_accel_sens( &accel_sens );
        accel[ 0 ] *= accel_sens;
        accel[ 1 ] *= accel_sens;
        accel[ 2 ] *= accel_sens;
        dmp_set_accel_bias( accel );
        return 0;
    }
    else
        return 1;
}

// /**********为了匿名四轴上位机的协议定义的变量****************************/
// // cup为小端模式存储，也就是在存储的时候，低位被存在0字节，高位在1字节
// #define BYTE0(dwTemp) (*(char *)(&dwTemp))       // 取出int型变量的低字节
// #define BYTE1(dwTemp) (*((char *)(&dwTemp) + 1)) //	取存储在此变量下一内存字节的内容，高字节
// #define BYTE2(dwTemp) (*((char *)(&dwTemp) + 2))
// #define BYTE3(dwTemp) (*((char *)(&dwTemp) + 3))

// uint8_t MPU6500_Send2Host(uint16_t ACCEL_XOUT, uint16_t ACCEL_YOUT, uint16_t ACCEL_ZOUT, uint16_t GYRO_XOUT, uint16_t GYRO_YOUT, uint16_t GYRO_ZOUT)
// {
//     uint8_t BUFF[40];
//     uint8_t sumcheck  = 0;
//     uint8_t addcheck  = 0;
//     uint8_t _cnt      = 0;
//     BUFF[_cnt++] = 0xAA;              // 帧头
//     BUFF[_cnt++] = 0xFF;              // 目标地址
//     BUFF[_cnt++] = 0XF1;              // 功能码
//     BUFF[_cnt++] = 12;                // 数据长度
//     BUFF[_cnt++] = BYTE0(ACCEL_XOUT); // 数据内容,小段模式，低位在前
//     BUFF[_cnt++] = BYTE1(ACCEL_XOUT); // 需要将字节进行拆分，调用上面的宏定义即可。
//     BUFF[_cnt++] = BYTE0(ACCEL_YOUT);
//     BUFF[_cnt++] = BYTE1(ACCEL_YOUT);
//     BUFF[_cnt++] = BYTE0(ACCEL_ZOUT);
//     BUFF[_cnt++] = BYTE1(ACCEL_ZOUT);
//     BUFF[_cnt++] = BYTE0(GYRO_XOUT);
//     BUFF[_cnt++] = BYTE1(GYRO_XOUT);
//     BUFF[_cnt++] = BYTE0(GYRO_YOUT);
//     BUFF[_cnt++] = BYTE1(GYRO_YOUT);
//     BUFF[_cnt++] = BYTE0(GYRO_ZOUT);
//     BUFF[_cnt++] = BYTE1(GYRO_ZOUT);
//     // SC和AC的校验直接抄最上面上面简介的即可
//     for (uint8_t i = 0; i < BUFF[3] + 4; i++) {
//         sumcheck += BUFF[i];
//         addcheck += sumcheck;
//     }
//     BUFF[_cnt++] = sumcheck;
//     BUFF[_cnt++] = addcheck;

//     return USART_Helper_SendLen(BUFF, _cnt);
// }