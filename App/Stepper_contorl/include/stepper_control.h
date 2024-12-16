#pragma once
#include "sys.h"
/*******************************************************/
#define MOTOR_PUL_TIM TIM8
#define MOTOR_PUL_IRQn TIM8_UP_TIM13_IRQn
#define MOTOR_PUL_IRQHandler TIM8_UP_TIM13_IRQHandler
#define MOTOR_PUL_CLK_ENABLE() __TIM8_CLK_ENABLE()
#define MOTOR_PUL_GPIO_AF GPIO_AF3_TIM8

/*********************X轴电机引脚定义*******************/
// Motor 方向
#define X_MOTOR_DIR_PIN GPIO_PIN_4
#define X_MOTOR_DIR_GPIO_PORT GPIOB
#define X_MOTOR_DIR_GPIO_CLK_ENABLE() __HAL_RCC_GPIOD_CLK_ENABLE()

// Motor 使能
#define X_MOTOR_EN_PIN GPIO_PIN_0
#define X_MOTOR_EN_GPIO_PORT GPIOE
#define X_MOTOR_EN_GPIO_CLK_ENABLE() __HAL_RCC_GPIOE_CLK_ENABLE()

// Motor 脉冲
#define X_MOTOR_PUL_PORT GPIOC
#define X_MOTOR_PUL_PIN GPIO_PIN_7
#define X_MOTOR_PUL_GPIO_CLK_ENABLE() __HAL_RCC_GPIOC_CLK_ENABLE()

// 定时器通道
#define X_MOTOR_PUL_CHANNEL TIM_CHANNEL_2

/*********************Y轴电机引脚定义*******************/
// Motor 方向
#define Y_MOTOR_DIR_PIN GPIO_PIN_5
#define Y_MOTOR_DIR_GPIO_PORT GPIOB
#define Y_MOTOR_DIR_GPIO_CLK_ENABLE() __HAL_RCC_GPIOD_CLK_ENABLE()

// Motor 使能
#define Y_MOTOR_EN_PIN GPIO_PIN_4
#define Y_MOTOR_EN_GPIO_PORT GPIOE
#define Y_MOTOR_EN_GPIO_CLK_ENABLE() __HAL_RCC_GPIOE_CLK_ENABLE()

// Motor 脉冲
#define Y_MOTOR_PUL_PORT GPIOC
#define Y_MOTOR_PUL_PIN GPIO_PIN_9
#define Y_MOTOR_PUL_GPIO_CLK_ENABLE() __HAL_RCC_GPIOC_CLK_ENABLE()

// 定时器通道
#define Y_MOTOR_PUL_CHANNEL TIM_CHANNEL_4

/*频率相关参数*/
// 定时器实际时钟频率为：168MHz/TIM_PRESCALER
// 其中 高级定时器的 频率为168MHz,其他定时器为84MHz
// 168/TIM_PRESCALER=28MHz
// 具体需要的频率可以自己计算
#define TIM_PRESCALER 6
// 定义定时器周期，输出比较模式周期设置为0xFFFF
#define TIM_PERIOD 0xFFFF

/************************************************************/
#define HIGH GPIO_PIN_SET   // 高电平
#define LOW GPIO_PIN_RESET  // 低电平
#define CW HIGH             // 顺时针
#define CCW LOW             // 逆时针

/* 步进电机结构体 */
typedef struct {
    uint16_t pul_pin;        // 脉冲引脚
    uint16_t dir_pin;        // 方向引脚
    uint16_t en_pin;         // 使能引脚
    uint32_t pul_channel;    // 输出脉冲的定时器通道
    GPIO_TypeDef* pul_port;  // 脉冲引脚端口结构体
    GPIO_TypeDef* dir_port;  // 方向引脚端口结构体
    GPIO_TypeDef* en_port;   // 使能引脚端口结构体
} Stepper_TypeDef;

/* 坐标轴枚举 */
typedef enum { x_axis = 0U, y_axis } Axis_TypeDef;

/* 直线插补参数结构体 */
typedef struct {
    __IO uint32_t endpoint_x;       // 终点坐标X
    __IO uint32_t endpoint_y;       // 终点坐标Y
    __IO uint32_t endpoint_pulse;   // 到达终点位置需要的脉冲数
    __IO uint32_t active_axis;      // 当前运动的轴
    __IO int32_t deviation;         // 偏差参数
    __IO uint8_t motionstatus : 1;  // 插补运动状态
    __IO uint8_t dir_x : 1;         // X轴运动方向
    __IO uint8_t dir_y : 1;         // Y轴运动方向
} LinearInterpolation_TypeDef;

#define F 32

extern TIM_HandleTypeDef TIM_StepperHandle;

void stepper_Init();

void Linear_Interpolation( int32_t coordi_x, int32_t coordi_y, uint16_t speed );

/* 坐标轴象限枚举 */
typedef enum { quadrant_1st = 0U, quadrant_2nd, quadrant_3rd, quadrant_4th } Quadrant_TypeDef;

/* 圆弧插补参数结构体 */
typedef struct {
    __IO int32_t startpoint[ 2 ];   // 起点坐标X、Y
    __IO int32_t endpoint_x;        // 终点坐标X
    __IO int32_t endpoint_y;        // 终点坐标Y
    __IO uint32_t endpoint_pulse;   // 到达终点位置需要的脉冲数
    __IO uint32_t active_axis;      // 当前运动的轴
    __IO int32_t deviation;         // 偏差参数F
    __IO int8_t devi_sign[ 2 ];     // 偏差方程的运算符号，正负
    __IO uint8_t motionstatus : 1;  // 插补运动状态
    __IO uint8_t dir_x : 1;         // X轴运动方向
    __IO uint8_t dir_y : 1;         // Y轴运动方向
    __IO uint8_t dir_interpo : 1;   // 插补整体运动方向
    __IO uint8_t crood_pos : 2;     // 起点坐标所在的象限
} CircularInterpolation_TypeDef;

extern CircularInterpolation_TypeDef circular_para;

void Circular_InterPolation( int32_t start_x, int32_t start_y, int32_t stop_x, int32_t stop_y, uint16_t speed, uint8_t dir );

enum eShape { Rect, Circle };