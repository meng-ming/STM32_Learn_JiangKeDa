#ifndef __MY_SPI_H
#define __MY_SPI_H

#include "stm32f10x.h"                  // Device header

// CPOL (极性) 和 CPHA (相位)
typedef enum
{
	MY_SPI_MODE_0, // CPOL=0, CPHA=0
	MY_SPI_MODE_1, // CPOL=0, CPHA=1
	MY_SPI_MODE_2, // CPOL=1, CPHA=0
	MY_SPI_MODE_3  // CPOL=1, CPHA=1
} My_SPI_Mode_e;

typedef struct
{
	SPI_TypeDef* SPI_X;
	GPIO_TypeDef* GPIO_X;
	uint16_t Pin_CS;
	uint16_t Pin_MISO;
	uint16_t Pin_MOSI;
	uint16_t Pin_SCK;
	My_SPI_Mode_e My_SPI_Mode;
}My_SPI_Pin_Set_s;

void My_SPI_Init(const My_SPI_Pin_Set_s MySPIPinSet);// 初始化 SPI_Pin 结构体

void My_SPI_Start(void); // 开始 SPI

void My_SPI_Stop(void); // 停止 SPI

uint8_t My_SPI_Swap_Byte(uint8_t Byte_Send); // 主从机交换 1字节 数据

#endif
