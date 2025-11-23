#include "stm32f10x.h"                  // Device header

static uint16_t GPIO_Pin_Used = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3;

void AD_Init(void)
{
	// 当前有多个通道进行输入，但是未使用DMA将数据转换走，会出现数据覆盖现象
	// 使用单次转换，非扫描模式。每个外设模块对应一个通道，当想查看不同外设现象时，将其对应的通道传入规则组中
	
	// 1. 使能 AD 以及输入 GPIO，
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	// 2. 初始化 GPIO
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN; // GPIO 口无效，防止外设输入对 AD 造成干扰
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_Used;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	// 3. 配置 ADC_Clock
	RCC_ADCCLKConfig(RCC_PCLK2_Div6); // 最高能支持 14MHz,当前时钟为 72M,只能使用 6/8 分频
	
	// 4. 初始化 AD 转换器 (此处需要配置为单次转换，非扫描模式)
	ADC_InitTypeDef ADC_InitStructure;
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right; // 数据右对齐
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None; // 不使用外部触发，使用软件触发
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent; // ADC 各自转换
	ADC_InitStructure.ADC_NbrOfChannel = 1; // 当前已经指定了为非扫描模式，只会转换一个序列内的信号，此处参数无用
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;
	
	ADC_Init(ADC1,&ADC_InitStructure);
	
	// 5. 使能 ADC
	ADC_Cmd(ADC1,ENABLE);
	
	// 6. 校准
	ADC_ResetCalibration(ADC1); // 复位校准
	while(ADC_GetResetCalibrationStatus(ADC1) == SET);
	ADC_StartCalibration(ADC1); // 开始校准
	while(ADC_GetCalibrationStatus(ADC1) == SET);
}

uint16_t AD_Get_Value(uint8_t ADC_Channel)
{
	// 配置规则组
	ADC_RegularChannelConfig(ADC1,ADC_Channel,1,ADC_SampleTime_28Cycles5);
	
	// 使用软件开始转换标志位
	ADC_SoftwareStartConvCmd(ADC1,ENABLE);
	
	// 获取转换标志位状态
	// ADC_FLAG_EOC: 转换结束标志
	while(ADC_GetFlagStatus(ADC1,ADC_FLAG_EOC) == RESET);
	// 获取转换值
	return ADC_GetConversionValue(ADC1);
}
