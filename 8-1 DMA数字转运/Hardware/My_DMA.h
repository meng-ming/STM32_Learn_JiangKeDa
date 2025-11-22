#ifndef __MY_DMA
#define __MY_DMA

void My_DMA_Init(uint32_t MemoryBaseAddr, uint32_t PeripheralBaseAddr,uint32_t BufferSize);

void Start_DMA_Transfer(uint16_t BufferSize);// 手动启动 DMA 转运

#endif
