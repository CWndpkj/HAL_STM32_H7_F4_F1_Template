#include <stm32f10x.h>
#include "AD.h"

uint16_t AD_Value[4];
void AD_Init()
{
    // 配置ADC时钟分频
    RCC_ADCCLKConfig(RCC_PCLK2_Div6);
    // 开启时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
    // 初始化GPIO口
    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_AIN;
    GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStruct);

    // 配置ADC通道
    ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_239Cycles5);
    ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 2, ADC_SampleTime_239Cycles5);
    ADC_RegularChannelConfig(ADC1, ADC_Channel_2, 3, ADC_SampleTime_239Cycles5);
    ADC_RegularChannelConfig(ADC1, ADC_Channel_3, 4, ADC_SampleTime_239Cycles5);

    // 初始化ADC
    ADC_InitTypeDef ADC_InitStruct;
    ADC_InitStruct.ADC_ContinuousConvMode = ENABLE;                    // 连续转换模式开启
    ADC_InitStruct.ADC_DataAlign          = ADC_DataAlign_Right;       // 数据对齐模式
    ADC_InitStruct.ADC_ExternalTrigConv   = ADC_ExternalTrigConv_None; // 外部触发选择
    ADC_InitStruct.ADC_Mode               = ADC_Mode_Independent;      // 独立的工作模式
    ADC_InitStruct.ADC_NbrOfChannel       = 4;                         // 需要扫描的通道数
    ADC_InitStruct.ADC_ScanConvMode       = ENABLE;                    // 扫描模式
    ADC_Init(ADC1, &ADC_InitStruct);

    // 校准ADC
    ADC_ResetCalibration(ADC1);
    while (ADC_GetResetCalibrationStatus(ADC1) == RESET);
    ADC_StartCalibration(ADC1);
    while (ADC_GetCalibrationStatus(ADC1) == SET);

    // 初始化DMA
    DMA_InitTypeDef DMA_InitStruct;
    DMA_InitStruct.DMA_BufferSize         = 4;
    DMA_InitStruct.DMA_DIR                = DMA_DIR_PeripheralSRC;           // 方向
    DMA_InitStruct.DMA_M2M                = DMA_M2M_Disable;                 // 硬件触发
    DMA_InitStruct.DMA_MemoryBaseAddr     = (uint32_t)AD_Value;              // 储存器地址
    DMA_InitStruct.DMA_MemoryDataSize     = DMA_MemoryDataSize_HalfWord;     // 以字节宽度转运
    DMA_InitStruct.DMA_MemoryInc          = DMA_MemoryInc_Enable;            // 储存器地址是否自增
    DMA_InitStruct.DMA_Mode               = DMA_Mode_Circular;               // 循环自动重装或普通模式
    DMA_InitStruct.DMA_PeripheralBaseAddr = (uint32_t) & (ADC1->DR);         // 外设地址
    DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord; // 宽度位Byte
    DMA_InitStruct.DMA_PeripheralInc      = DMA_PeripheralInc_Disable;
    DMA_InitStruct.DMA_Priority           = DMA_Priority_Medium; // 优先级
    DMA_Init(DMA1_Channel1, &DMA_InitStruct);

    // 开启ADC_DMA
    ADC_DMACmd(ADC1, ENABLE);
    // 使能ADC
    ADC_Cmd(ADC1, ENABLE);
    // 使能DMA
    DMA_Cmd(DMA1_Channel1, ENABLE);
}

uint16_t *AD_GetValue()
{
    return AD_Value;
}

void AD_StartConvert()
{
    ADC_SoftwareStartConvCmd(ADC1, ENABLE);
}