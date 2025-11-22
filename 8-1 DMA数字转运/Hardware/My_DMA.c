#include "stm32f10x.h"                  // Device header

void My_DMA_Init(uint32_t MemoryBaseAddr, uint32_t PeripheralBaseAddr,uint32_t BufferSize)
{
	// 使用 DMA1_Channel1 进行 M2M 的 DMA 转运
	// 将一个数组内部的数据转运到另外一个数组
	
	// 1. 使能 DMA 外设
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);
	
	// 2. 初始化 DMA 结构体
	DMA_InitTypeDef DMA_InitStructure;

	DMA_InitStructure.DMA_MemoryBaseAddr = MemoryBaseAddr;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable; // 转运一次之后是否自增
	DMA_InitStructure.DMA_PeripheralBaseAddr = PeripheralBaseAddr;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Enable;	
	DMA_InitStructure.DMA_BufferSize = BufferSize; // 转运次数，软件触发会不断将此数减 1，直到 0
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal; // 单次转运，即当设定的转运次数减为 0 之后，停止DMA转运
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC; // 转运方向，外设区域为源头，即 外设->存储器
	DMA_InitStructure.DMA_M2M = DMA_M2M_Enable; // 设定为软件触发还是硬件触发
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium; // 当前只有通道1，没有其它通道与其竞争，随便什么优先级都可以
	
	DMA_Init(DMA1_Channel1,&DMA_InitStructure);
	
	// 3. 使能 DMA
	// DMA_Cmd(DMA1_Channel1,ENABLE); // DMA 使能之后会立马开始转运
}

void Start_DMA_Transfer(uint16_t BufferSize)
{
	// 1. 想要 DMA 转运之后再次进行转运，必须先将对应的 DMA 失能
	DMA_Cmd(DMA1_Channel1,DISABLE);
	// 2. 重新设置转运次数
	DMA_SetCurrDataCounter(DMA1_Channel1,BufferSize);
	// 3. 重新使 DMA 使能
	DMA_Cmd(DMA1_Channel1,ENABLE); 
	// 4. 等待 DMA 转运成功，后续进行改进，否则会阻塞主进程
	while(DMA_GetFlagStatus(DMA1_FLAG_TC1) == RESET);
	// 5. 清除标志位
	DMA_ClearFlag(DMA1_FLAG_TC1);
}
