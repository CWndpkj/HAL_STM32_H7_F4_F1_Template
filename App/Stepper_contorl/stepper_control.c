#include "stepper_control.h"
#include <stdlib.h>

// 控制使能引脚
/* 带参宏，可以像内联函数一样使用 */
#define MOTOR_PUL( port, pin, x ) HAL_GPIO_WritePin( port, pin, x )
#define MOTOR_DIR( port, pin, x ) HAL_GPIO_WritePin( port, pin, x )
#define MOTOR_OFFLINE( port, pin, x ) HAL_GPIO_WritePin( port, pin, x )
#define MOTOR_START( tim, channel, status ) TIM_CCxChannelCmd( tim, channel, status )
#define MOTOR_STOP( tim, channel, status ) TIM_CCxChannelCmd( tim, channel, status )

/* 步进电机结构体数组  bsp_stepper_init.c */
Stepper_TypeDef step_motor[ 2 ] = {
    { X_MOTOR_PUL_PIN, X_MOTOR_DIR_PIN, X_MOTOR_EN_PIN, X_MOTOR_PUL_CHANNEL, X_MOTOR_PUL_PORT, X_MOTOR_DIR_GPIO_PORT, X_MOTOR_EN_GPIO_PORT },
    { Y_MOTOR_PUL_PIN, Y_MOTOR_DIR_PIN, Y_MOTOR_EN_PIN, Y_MOTOR_PUL_CHANNEL, Y_MOTOR_PUL_PORT, Y_MOTOR_DIR_GPIO_PORT, Y_MOTOR_EN_GPIO_PORT },
};
TIM_HandleTypeDef TIM_StepperHandle;
Axis_TypeDef axis;
LinearInterpolation_TypeDef interpolation_para = { 0 };
static enum eShape current_shape;

static void TIMx_NVIC_Configuration( void )
{
    /* 外设中断配置 */
    HAL_NVIC_SetPriority( MOTOR_PUL_IRQn, 0, 0 );
    HAL_NVIC_EnableIRQ( MOTOR_PUL_IRQn );
}

/**
 * @brief  配置TIM复用输出PWM时用到的I/O
 * @param  无
 * @retval 无
 */
static void Stepper_GPIO_Config( void )
{
    GPIO_InitTypeDef GPIO_InitStruct;

    /* 获取数组元素个数 */
    uint8_t member_count = sizeof( step_motor ) / sizeof( Stepper_TypeDef );

    X_MOTOR_DIR_GPIO_CLK_ENABLE();
    X_MOTOR_EN_GPIO_CLK_ENABLE();
    X_MOTOR_PUL_GPIO_CLK_ENABLE();

    Y_MOTOR_DIR_GPIO_CLK_ENABLE();
    Y_MOTOR_EN_GPIO_CLK_ENABLE();
    Y_MOTOR_PUL_GPIO_CLK_ENABLE();

    for ( uint8_t i = 0; i < member_count; i++ ) {
        /*选择要控制的GPIO引脚*/
        GPIO_InitStruct.Pin = step_motor[ i ].dir_pin;
        /*设置引脚的输出类型为推挽输出*/
        GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
        GPIO_InitStruct.Pull = GPIO_PULLUP;
        /*设置引脚速率为高速 */
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
        /*Motor 方向引脚 初始化*/
        HAL_GPIO_Init( step_motor[ i ].dir_port, &GPIO_InitStruct );
        MOTOR_DIR( step_motor[ i ].dir_port, step_motor[ i ].dir_pin, CW );

        /*Motor 使能引脚 初始化*/
        GPIO_InitStruct.Pin = step_motor[ i ].en_pin;
        HAL_GPIO_Init( step_motor[ i ].en_port, &GPIO_InitStruct );
        MOTOR_OFFLINE( step_motor[ i ].en_port, step_motor[ i ].en_pin, LOW );

        /* 定时器输出通道功能引脚IO初始化 */
        /*设置输出类型*/
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        /*设置引脚速率 */
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
        /*设置复用*/
        GPIO_InitStruct.Alternate = MOTOR_PUL_GPIO_AF;
        /*设置复用*/
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        /*选择要控制的GPIO引脚*/
        GPIO_InitStruct.Pin = step_motor[ i ].pul_pin;
        /*Motor 脉冲引脚 初始化*/
        HAL_GPIO_Init( step_motor[ i ].pul_port, &GPIO_InitStruct );
        MOTOR_PUL( step_motor[ i ].pul_port, step_motor[ i ].pul_pin, LOW );
    }
}
static void TIM_PWMOUTPUT_Config( void )
{
    TIM_OC_InitTypeDef TIM_OCInitStructure;

    /* 获取数组元素个数 */
    uint8_t member_count = sizeof( step_motor ) / sizeof( Stepper_TypeDef );

    /*使能定时器*/
    MOTOR_PUL_CLK_ENABLE();

    TIM_StepperHandle.Instance = MOTOR_PUL_TIM;
    /* 累计 TIM_Period个后产生一个更新或者中断*/
    // 当定时器从0计数到TIM_PERIOD，即为TIM_PERIOD次，为一个定时周期
    TIM_StepperHandle.Init.Period = TIM_PERIOD;
    // 通用控制定时器时钟源TIMxCLK = HCLK = 168MHz
    // 设定定时器频率为=TIMxCLK/(TIM_Prescaler+1)=28MHz
    TIM_StepperHandle.Init.Prescaler = TIM_PRESCALER - 1;

    /*计数方式*/
    TIM_StepperHandle.Init.CounterMode = TIM_COUNTERMODE_UP;
    /*采样时钟分频*/
    TIM_StepperHandle.Init.ClockDivision     = TIM_CLOCKDIVISION_DIV1;
    TIM_StepperHandle.Init.RepetitionCounter = 0;
    /*初始化定时器为输出比较模式*/
    HAL_TIM_Base_Init( &TIM_StepperHandle );

    /*PWM模式配置--这里配置为PWM模式2*/
    TIM_OCInitStructure.OCMode = TIM_OCMODE_PWM2;
    /*比较输出的计数值*/
    TIM_OCInitStructure.Pulse = TIM_PERIOD;
    /*当定时器计数值小于CCR1_Val时为高电平*/
    TIM_OCInitStructure.OCPolarity = TIM_OCPOLARITY_LOW;
    /*设置互补通道输出的极性*/
    TIM_OCInitStructure.OCNPolarity = TIM_OCNPOLARITY_LOW;
    /*快速模式设置*/
    TIM_OCInitStructure.OCFastMode = TIM_OCFAST_DISABLE;
    /*空闲电平*/
    TIM_OCInitStructure.OCIdleState = TIM_OCIDLESTATE_RESET;
    /*互补通道设置*/
    TIM_OCInitStructure.OCNIdleState = TIM_OCNIDLESTATE_RESET;

    for ( uint8_t i = 0; i < member_count; i++ ) {
        /* 配置输出比较通道 */
        HAL_TIM_OC_ConfigChannel( &TIM_StepperHandle, &TIM_OCInitStructure, step_motor[ i ].pul_channel );
        TIM_CCxChannelCmd( MOTOR_PUL_TIM, step_motor[ i ].pul_channel, TIM_CCx_DISABLE );
    }
}

/**
 * @brief  直线增量插补运动
 * @param  inc_x：终点坐标X的增量
 * @param  inc_y：终点坐标Y的增量
 * @param  speed：进给速度
 * @retval 无
 */
void InterPolation_Move( uint32_t inc_x, uint32_t inc_y, uint16_t speed )
{
    /* 偏差清零 */
    interpolation_para.deviation = 0;

    /* 设置终点坐标 */
    interpolation_para.endpoint_x = inc_x;
    interpolation_para.endpoint_y = inc_y;
    /* 所需脉冲数为X、Y坐标增量之和 */
    interpolation_para.endpoint_pulse = inc_x + inc_y;

    /* 第一步进给的活动轴为X轴 */
    interpolation_para.active_axis = x_axis;
    /* 计算偏差 */
    interpolation_para.deviation -= interpolation_para.endpoint_y;

    /* 设置速度 */
    __HAL_TIM_SET_COMPARE( &TIM_StepperHandle, step_motor[ x_axis ].pul_channel, speed );
    __HAL_TIM_SET_COMPARE( &TIM_StepperHandle, step_motor[ y_axis ].pul_channel, speed );
    __HAL_TIM_SET_AUTORELOAD( &TIM_StepperHandle, speed * 2 );

    /* 使能主输出 */
    __HAL_TIM_MOE_ENABLE( &TIM_StepperHandle );
    /* 开启X轴比较通道输出 */
    TIM_CCxChannelCmd( MOTOR_PUL_TIM, step_motor[ interpolation_para.active_axis ].pul_channel, TIM_CCx_ENABLE );
    HAL_TIM_Base_Start_IT( &TIM_StepperHandle );
    interpolation_para.motionstatus = 1;
}
/**
 * @brief  任意直线插补运动
 * @param  coordi_x：终点坐标X的增量
 * @param  coordi_y：终点坐标Y的增量
 * @param  speed：进给速度，定时器计数值
 * @retval 无
 */
void Linear_Interpolation( int32_t coordi_x, int32_t coordi_y, uint16_t speed )
{
    current_shape = Rect;
    /* 判断当前是否正在做插补运动 */
    if ( interpolation_para.motionstatus != 0 )
        return;

    /* 判断坐标正负，以此决定各轴的运动方向 */
    if ( coordi_x < 0 ) {
        interpolation_para.dir_x = CCW;
        coordi_x                 = -coordi_x;
        MOTOR_DIR( step_motor[ x_axis ].dir_port, step_motor[ x_axis ].dir_pin, CCW );
    }
    else {
        interpolation_para.dir_x = CW;
        MOTOR_DIR( step_motor[ x_axis ].dir_port, step_motor[ x_axis ].dir_pin, CW );
    }

    if ( coordi_y < 0 ) {
        interpolation_para.dir_y = CCW;
        coordi_y                 = -coordi_y;
        MOTOR_DIR( step_motor[ y_axis ].dir_port, step_motor[ y_axis ].dir_pin, CCW );
    }
    else {
        interpolation_para.dir_y = CW;
        MOTOR_DIR( step_motor[ y_axis ].dir_port, step_motor[ y_axis ].dir_pin, CW );
    }

    /* 开始插补运动 */
    InterPolation_Move( coordi_x, coordi_y, speed );
}
/**
 * @brief  定时器比较中断回调函数
 * @param  htim：定时器句柄指针
 * @note   无
 * @retval 无
 */
void HAL_TIM_PeriodElapsedCallback( TIM_HandleTypeDef* htim )
{
    uint32_t last_axis = 0;
    if ( current_shape == Rect ) {
        /* 记录上一步的进给活动轴 */
        last_axis = interpolation_para.active_axis;

        /* 根据上一步的偏差，判断的进给方向，并计算下一步的偏差 */
        if ( interpolation_para.deviation >= 0 ) {
            /* 偏差>0，在直线上方，进给X轴，计算偏差 */
            interpolation_para.active_axis = x_axis;
            interpolation_para.deviation -= interpolation_para.endpoint_y;
        }
        else {
            /* 偏差<0，在直线下方，进给Y轴，计算偏差 */
            interpolation_para.active_axis = y_axis;
            interpolation_para.deviation += interpolation_para.endpoint_x;
        }

        /* 下一步的活动轴与上一步的不一致时，需要换轴 */
        if ( last_axis != interpolation_para.active_axis ) {
            TIM_CCxChannelCmd( htim->Instance, step_motor[ last_axis ].pul_channel, TIM_CCx_DISABLE );
            TIM_CCxChannelCmd( htim->Instance, step_motor[ interpolation_para.active_axis ].pul_channel, TIM_CCx_ENABLE );
        }

        /* 进给总步数减1 */
        interpolation_para.endpoint_pulse--;

        /* 判断是否完成插补 */
        if ( interpolation_para.endpoint_pulse == 0 ) {
            /* 关闭定时器 */
            TIM_CCxChannelCmd( htim->Instance, step_motor[ last_axis ].pul_channel, TIM_CCx_DISABLE );
            TIM_CCxChannelCmd( htim->Instance, step_motor[ interpolation_para.active_axis ].pul_channel, TIM_CCx_DISABLE );
            __HAL_TIM_MOE_DISABLE( htim );
            HAL_TIM_Base_Stop_IT( htim );
            interpolation_para.motionstatus = 0;
        }
    }
    else if ( current_shape == Circle ) {
        /* 记录上一步的进给活动轴 */
        last_axis = circular_para.active_axis;

        /* 根据进给方向刷新坐标 */
        switch ( last_axis ) {
        case x_axis:
            switch ( circular_para.dir_x ) {
            case CCW:
                circular_para.startpoint[ x_axis ]--;
                break;
            case CW:
                circular_para.startpoint[ x_axis ]++;
                break;
            }
            break;
        case y_axis:
            switch ( circular_para.dir_y ) {
            case CCW:
                circular_para.startpoint[ y_axis ]--;
                break;
            case CW:
                circular_para.startpoint[ y_axis ]++;
                break;
            }
            break;
        }

        /* 根据上一次进给的偏差，判断新的进给活动轴 */
        if ( circular_para.deviation > 0 ) {
            switch ( circular_para.dir_interpo ) {
            case CW: /* 顺时针 */
                switch ( circular_para.crood_pos ) {
                case quadrant_1st:
                case quadrant_3rd:
                    circular_para.active_axis = y_axis;
                    break;
                case quadrant_2nd:
                case quadrant_4th:
                    circular_para.active_axis = x_axis;
                    break;
                }
                break;
            case CCW: /* 逆时针 */
                switch ( circular_para.crood_pos ) {
                case quadrant_1st:
                case quadrant_3rd:
                    circular_para.active_axis = x_axis;
                    break;
                case quadrant_2nd:
                case quadrant_4th:
                    circular_para.active_axis = y_axis;
                    break;
                }
                break;
            }
        }
        else /* 偏差小于0，向圆外进给 */
        {
            switch ( circular_para.dir_interpo ) {
            case CW: /* 顺时针 */
                switch ( circular_para.crood_pos ) {
                case quadrant_1st:
                case quadrant_3rd:
                    circular_para.active_axis = x_axis;
                    break;
                case quadrant_2nd:
                case quadrant_4th:
                    circular_para.active_axis = y_axis;
                    break;
                }
                break;
            case CCW: /* 逆时针 */
                switch ( circular_para.crood_pos ) {
                case quadrant_1st:
                case quadrant_3rd:
                    circular_para.active_axis = y_axis;
                    break;
                case quadrant_2nd:
                case quadrant_4th:
                    circular_para.active_axis = x_axis;
                    break;
                }
                break;
            }
        }
        /* 根据插补运动方向和进给方向计算出新的偏差 */
        circular_para.deviation += 2 * circular_para.devi_sign[ circular_para.active_axis ] * circular_para.startpoint[ circular_para.active_axis ] + 1;

        /* 下一步的活动轴与上一步的不一致时，需要换轴 */
        if ( last_axis != circular_para.active_axis ) {
            TIM_CCxChannelCmd( htim->Instance, step_motor[ last_axis ].pul_channel, TIM_CCx_DISABLE );
            TIM_CCxChannelCmd( htim->Instance, step_motor[ circular_para.active_axis ].pul_channel, TIM_CCx_ENABLE );
        }

        /* 进给总步数减1 */
        circular_para.endpoint_pulse--;

        /* 判断是否完成插补 */
        if ( circular_para.endpoint_pulse == 0 ) {
            /* 关闭定时器 */
            TIM_CCxChannelCmd( htim->Instance, step_motor[ last_axis ].pul_channel, TIM_CCx_DISABLE );
            TIM_CCxChannelCmd( htim->Instance, step_motor[ circular_para.active_axis ].pul_channel, TIM_CCx_DISABLE );
            __HAL_TIM_MOE_DISABLE( htim );
            HAL_TIM_Base_Stop_IT( htim );
            circular_para.motionstatus = 0;
        }
    }
}
/**
 * @brief  步进电机初始化
 * @param  无
 *	@note   无
 * @retval 无
 */
void stepper_Init()
{
    /*电机IO配置*/
    Stepper_GPIO_Config();
    /*定时器PWM输出配置*/
    TIM_PWMOUTPUT_Config();
    /*中断配置*/
    TIMx_NVIC_Configuration();
}

Quadrant_TypeDef quadrant;
CircularInterpolation_TypeDef circular_para = { 0 };
/**
 * @brief  设置进给方向
 * @param  coord_x
 * @param  coord_y
 * @retval 无
 */
static void Set_Feed_DIR( int32_t coord_x, int32_t coord_y, uint8_t dir )
{
    /* 记录插补运动方向 */
    circular_para.dir_interpo = dir;

    if ( dir == CW ) {
        if ( coord_x > 0 ) /* x正半轴 */
        {
            if ( coord_y > 0 ) /* 第一象限 */
            {
                circular_para.crood_pos           = quadrant_1st;
                circular_para.dir_x               = CW;
                circular_para.dir_y               = CCW;
                circular_para.devi_sign[ x_axis ] = 1;
                circular_para.devi_sign[ y_axis ] = -1;
                MOTOR_DIR( step_motor[ x_axis ].dir_port, step_motor[ x_axis ].dir_pin, CW );
                MOTOR_DIR( step_motor[ y_axis ].dir_port, step_motor[ y_axis ].dir_pin, CCW );
            }
            else /* 第四象限 */
            {
                circular_para.crood_pos           = quadrant_4th;
                circular_para.dir_x               = CCW;
                circular_para.dir_y               = CCW;
                circular_para.devi_sign[ x_axis ] = -1;
                circular_para.devi_sign[ y_axis ] = -1;
                MOTOR_DIR( step_motor[ x_axis ].dir_port, step_motor[ x_axis ].dir_pin, CCW );
                MOTOR_DIR( step_motor[ y_axis ].dir_port, step_motor[ y_axis ].dir_pin, CCW );
            }
        }
        else if ( coord_x < 0 ) /* x负半轴 */
        {
            if ( coord_y >= 0 ) /* 第二象限 */
            {
                circular_para.crood_pos           = quadrant_2nd;
                circular_para.dir_x               = CW;
                circular_para.dir_y               = CW;
                circular_para.devi_sign[ x_axis ] = 1;
                circular_para.devi_sign[ y_axis ] = 1;
                MOTOR_DIR( step_motor[ x_axis ].dir_port, step_motor[ x_axis ].dir_pin, CW );
                MOTOR_DIR( step_motor[ y_axis ].dir_port, step_motor[ y_axis ].dir_pin, CW );
            }
            else /* 第三象限 */
            {
                circular_para.crood_pos           = quadrant_3rd;
                circular_para.dir_x               = CCW;
                circular_para.dir_y               = CW;
                circular_para.devi_sign[ x_axis ] = -1;
                circular_para.devi_sign[ y_axis ] = 1;
                MOTOR_DIR( step_motor[ x_axis ].dir_port, step_motor[ x_axis ].dir_pin, CCW );
                MOTOR_DIR( step_motor[ y_axis ].dir_port, step_motor[ y_axis ].dir_pin, CW );
            }
        }
        else if ( coord_x == 0 ) /* x=0，当前点在Y轴上 */
        {
            if ( coord_y > 0 ) /* 第一象限 */
            {
                circular_para.crood_pos           = quadrant_1st;
                circular_para.dir_x               = CW;
                circular_para.dir_y               = CCW;
                circular_para.devi_sign[ x_axis ] = 1;
                circular_para.devi_sign[ y_axis ] = -1;
                MOTOR_DIR( step_motor[ x_axis ].dir_port, step_motor[ x_axis ].dir_pin, CW );
                MOTOR_DIR( step_motor[ y_axis ].dir_port, step_motor[ y_axis ].dir_pin, CCW );
            }
            else if ( coord_y < 0 ) /* 第三象限 */
            {
                circular_para.crood_pos           = quadrant_3rd;
                circular_para.dir_x               = CCW;
                circular_para.dir_y               = CW;
                circular_para.devi_sign[ x_axis ] = -1;
                circular_para.devi_sign[ y_axis ] = 1;
                MOTOR_DIR( step_motor[ x_axis ].dir_port, step_motor[ x_axis ].dir_pin, CCW );
                MOTOR_DIR( step_motor[ y_axis ].dir_port, step_motor[ y_axis ].dir_pin, CW );
            }
        }
    }
    else {
        if ( coord_x > 0 ) /* x正半轴 */
        {
            if ( coord_y >= 0 ) /* 第一象限 */
            {
                circular_para.crood_pos           = quadrant_1st;
                circular_para.dir_x               = CCW;
                circular_para.dir_y               = CW;
                circular_para.devi_sign[ x_axis ] = -1;
                circular_para.devi_sign[ y_axis ] = 1;
                MOTOR_DIR( step_motor[ x_axis ].dir_port, step_motor[ x_axis ].dir_pin, CCW );
                MOTOR_DIR( step_motor[ y_axis ].dir_port, step_motor[ y_axis ].dir_pin, CW );
            }
            else /* 第四象限 */
            {
                circular_para.crood_pos           = quadrant_4th;
                circular_para.dir_x               = CW;
                circular_para.dir_y               = CW;
                circular_para.devi_sign[ x_axis ] = 1;
                circular_para.devi_sign[ y_axis ] = 1;
                MOTOR_DIR( step_motor[ x_axis ].dir_port, step_motor[ x_axis ].dir_pin, CW );
                MOTOR_DIR( step_motor[ y_axis ].dir_port, step_motor[ y_axis ].dir_pin, CW );
            }
        }
        else if ( coord_x < 0 ) /* x负半轴 */
        {
            if ( coord_y > 0 ) /* 第二象限 */
            {
                circular_para.crood_pos           = quadrant_2nd;
                circular_para.dir_x               = CCW;
                circular_para.dir_y               = CCW;
                circular_para.devi_sign[ x_axis ] = -1;
                circular_para.devi_sign[ y_axis ] = -1;
                MOTOR_DIR( step_motor[ x_axis ].dir_port, step_motor[ x_axis ].dir_pin, CCW );
                MOTOR_DIR( step_motor[ y_axis ].dir_port, step_motor[ y_axis ].dir_pin, CCW );
            }
            else /* 第三象限 */
            {
                circular_para.crood_pos           = quadrant_3rd;
                circular_para.dir_x               = CW;
                circular_para.dir_y               = CCW;
                circular_para.devi_sign[ x_axis ] = 1;
                circular_para.devi_sign[ y_axis ] = -1;
                MOTOR_DIR( step_motor[ x_axis ].dir_port, step_motor[ x_axis ].dir_pin, CW );
                MOTOR_DIR( step_motor[ y_axis ].dir_port, step_motor[ y_axis ].dir_pin, CCW );
            }
        }
        else if ( coord_x == 0 ) /* x=0，当前点在Y轴上 */
        {
            if ( coord_y > 0 ) /* 第二象限 */
            {
                circular_para.crood_pos           = quadrant_2nd;
                circular_para.dir_x               = CCW;
                circular_para.dir_y               = CCW;
                circular_para.devi_sign[ x_axis ] = -1;
                circular_para.devi_sign[ y_axis ] = -1;
                MOTOR_DIR( step_motor[ x_axis ].dir_port, step_motor[ x_axis ].dir_pin, CCW );
                MOTOR_DIR( step_motor[ y_axis ].dir_port, step_motor[ y_axis ].dir_pin, CCW );
            }
            else if ( coord_y < 0 ) /* 第四象限 */
            {
                circular_para.crood_pos           = quadrant_4th;
                circular_para.dir_x               = CW;
                circular_para.dir_y               = CW;
                circular_para.devi_sign[ x_axis ] = 1;
                circular_para.devi_sign[ y_axis ] = 1;
                MOTOR_DIR( step_motor[ x_axis ].dir_port, step_motor[ x_axis ].dir_pin, CW );
                MOTOR_DIR( step_motor[ y_axis ].dir_port, step_motor[ y_axis ].dir_pin, CW );
            }
        }
    }
}

/**
 * @brief  任意象限顺圆插补运动
 * @param  start_x：圆弧起点坐标X
 * @param  start_y：圆弧起点坐标Y
 * @param  stop_x：圆弧终点坐标X
 * @param  stop_y：圆弧终点坐标Y
 * @param  speed：进给速度
 * @param  dir：进给方向
 * @retval 无
 */
void Circular_InterPolation( int32_t start_x, int32_t start_y, int32_t stop_x, int32_t stop_y, uint16_t speed, uint8_t dir )
{
    current_shape = Circle;
    /* 判断当前是否正在做插补运动 */
    if ( circular_para.motionstatus != 0 )
        return;

    /* 检查起点、终点坐标是否在同一个圆上 */
    if ( ( ( start_x * start_x ) + ( start_y * start_y ) ) != ( ( stop_x * stop_x ) + ( stop_y * stop_y ) ) )
        return;

    /* 偏差清零 */
    circular_para.deviation = 0;

    /* 起点坐标 */
    circular_para.startpoint[ x_axis ] = start_x;
    circular_para.startpoint[ y_axis ] = start_y;
    /* 终点坐标 */
    circular_para.endpoint_x = stop_x;
    circular_para.endpoint_y = stop_y;
    /* 所需脉冲数是从起点到终点的脉冲数之和 */
    circular_para.endpoint_pulse = abs( stop_x - start_x ) + abs( stop_y - start_y );

    /* 根据坐标确定插补方向和X、Y运动方向 */
    Set_Feed_DIR( circular_para.startpoint[ x_axis ], circular_para.startpoint[ y_axis ], dir );

    /* 起点坐标x=0，说明起点在y轴上，直接向x轴进给可减小误差 */
    if ( circular_para.startpoint[ x_axis ] == 0 ) {
        /* 偏差方程：F = F ± 2 * x + 1*/
        circular_para.active_axis = x_axis;
        circular_para.deviation += 2 * circular_para.devi_sign[ x_axis ] * circular_para.startpoint[ x_axis ] + 1;
    }
    else {
        /* 偏差方程：F = F ± 2 * y + 1*/
        circular_para.active_axis = y_axis;
        circular_para.deviation += 2 * circular_para.devi_sign[ y_axis ] * circular_para.startpoint[ y_axis ] + 1;
    }

    /* 设置速度 */
    __HAL_TIM_SET_COMPARE( &TIM_StepperHandle, step_motor[ x_axis ].pul_channel, speed );
    __HAL_TIM_SET_COMPARE( &TIM_StepperHandle, step_motor[ y_axis ].pul_channel, speed );
    __HAL_TIM_SET_AUTORELOAD( &TIM_StepperHandle, speed * 2 );

    /* 使能主输出 */
    __HAL_TIM_MOE_ENABLE( &TIM_StepperHandle );
    /* 开启活动轴比较通道输出 */
    TIM_CCxChannelCmd( MOTOR_PUL_TIM, step_motor[ circular_para.active_axis ].pul_channel, TIM_CCx_ENABLE );
    HAL_TIM_Base_Start_IT( &TIM_StepperHandle );

    circular_para.motionstatus = 1;
}
