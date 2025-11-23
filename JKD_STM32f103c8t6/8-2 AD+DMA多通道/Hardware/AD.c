#include "stm32f10x.h"                  // Device header

static uint16_t GPIO_Pin_Used = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3;

const uint8_t Buffer_Size = 4;

uint16_t Memory_Data[4] = {0};

void AD_Init(void)
{
	// 使用 AD 触发 + DMA 转运的方式，防止数据丢失
	// 当前使用 AD 软件触发 + 连续转换 + 扫描模式，连续不断将当前 模拟信号 转换为 数字信号
	// DMA 使用 硬件触发（AD） + 循环转运，转运完成之后不用重新设置 Buffer_Size，会自动开启下一次转运
	
	// 1. 使能 AD 、输入 GPIO 以及 DMA 外设
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);
	
	// 2. 初始化 GPIO
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN; // GPIO 口无效，防止外设输入对 AD 造成干扰
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_Used;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	// 3. 配置 ADC_Clock
	RCC_ADCCLKConfig(RCC_PCLK2_Div6); // 最高能支持 14MHz,当前时钟为 72M,只能使用 6/8 分频
	
	// 4. 配置规则组
	ADC_RegularChannelConfig(ADC1,ADC_Channel_0,1,ADC_SampleTime_28Cycles5);
	ADC_RegularChannelConfig(ADC1,ADC_Channel_1,2,ADC_SampleTime_28Cycles5);
	ADC_RegularChannelConfig(ADC1,ADC_Channel_2,3,ADC_SampleTime_28Cycles5);
	ADC_RegularChannelConfig(ADC1,ADC_Channel_3,4,ADC_SampleTime_28Cycles5);
	
	// 5. 初始化 AD 转换器 
	ADC_InitTypeDef ADC_InitStructure;
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE; // 是否连续转换
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None; // 不使用外部触发，使用软件触发
	ADC_InitStructure.ADC_NbrOfChannel = 4; // 
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right; // 数据右对齐
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent; // ADC 各自转换

	ADC_Init(ADC1,&ADC_InitStructure);
	
	// 6. 初始化 DMA 结构体
	DMA_InitTypeDef DMA_InitStructure;

	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)Memory_Data;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable; // 转运一次之后是否自增
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&ADC1->DR;// 起始地址为 ADC->DR 的地址
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;// 不自增	
	DMA_InitStructure.DMA_BufferSize = Buffer_Size; // 转运次数，软件触发会不断将此数减 1，直到 0
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular; // 是否单次转运，即当设定的转运次数减为 0 之后，停止DMA转运
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC; // 转运方向，外设区域为源头，即 外设->存储器
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable; // 设定为软件触发还是硬件触发
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium; // 当前只有通道1，没有其它通道与其竞争，随便什么优先级都可以
	
	DMA_Init(DMA1_Channel1,&DMA_InitStructure);// 只能使用通道1，ADC1 只对应 通道1
	
	// 7. 使能 ADC 、DMA
	ADC_Cmd(ADC1,ENABLE);
	ADC_DMACmd(ADC1,ENABLE);// 开启 ADC 到 DMA 的输出
	DMA_Cmd(DMA1_Channel1,ENABLE);
	
	// 8. 校准
	ADC_ResetCalibration(ADC1); // 复位校准
	while(ADC_GetResetCalibrationStatus(ADC1) == SET);
	ADC_StartCalibration(ADC1); // 开始校准
	while(ADC_GetCalibrationStatus(ADC1) == SET);
	
	// 9. 开启软件触发 AD 转换
	ADC_SoftwareStartConvCmd(ADC1,ENABLE);
}

// 当使用 ADC 使用单次转换，DMA 不使用循环转运时
void AD_Get_Value(void)
{
	// 1. 重新启动 DMA 转运
	DMA_Cmd(DMA1_Channel1,DISABLE);
	DMA_SetCurrDataCounter(DMA1_Channel1,Buffer_Size);
	DMA_Cmd(DMA1_Channel1,ENABLE);
	
	// 2. 使用软件开始转换标志位
	ADC_SoftwareStartConvCmd(ADC1,ENABLE);
	
	// 3. 由于 DMA 转运总是在 ADC 转换之后，所以只需要等待 DMA 转运完即可
	while(DMA_GetFlagStatus(DMA1_FLAG_TC1) == RESET);
	// 4. 手动清除标志位
	DMA_ClearFlag(DMA1_FLAG_TC1);
}
