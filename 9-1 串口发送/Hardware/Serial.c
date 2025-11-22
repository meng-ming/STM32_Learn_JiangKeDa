#include "stm32f10x.h"                  // Device header
#include "Serial.h"
#include <stdarg.h>

void Serial_Init(void)
{
	// 当前只测试 USART1 的 TX 功能，对应 PA9 引脚
	// 1.使能 USART 和对应的 GPIO引脚
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	// 2. 初始化 GPIO
	GPIO_InitTypeDef GPIO_InitStructure;
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; // USART 传输数据使用 复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
 	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	// 3. 初始化 USART
	USART_InitTypeDef USART_InitStrucutre;
	USART_InitStrucutre.USART_BaudRate = 115200;
	USART_InitStrucutre.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStrucutre.USART_Mode = USART_Mode_Tx; // 当前只发送
	USART_InitStrucutre.USART_Parity = USART_Parity_No; // 校验位
	USART_InitStrucutre.USART_StopBits = USART_StopBits_1;
	USART_InitStrucutre.USART_WordLength = USART_WordLength_8b;
	
	USART_Init(USART1,&USART_InitStrucutre);
	
	// 4. 使能 USART
	USART_Cmd(USART1,ENABLE);
}

void Serial_Send_Data(uint8_t Byte)
{
	USART_SendData(USART1,Byte);
	// 等待发送完成
	while(USART_GetFlagStatus(USART1,USART_FLAG_TXE) == RESET);
}

void Serial_Send_String(char* String)
{
	for(int i = 0;String[i] != '\0'; i++)
	{
		Serial_Send_Data(String[i]);
	}
}

void Serial_Send_Array(uint8_t *Array,uint8_t Length)
{
	for(uint8_t i = 0;i < Length;i++)
	{
		Serial_Send_Data(Array[i]);
	}
}

uint32_t Pow(uint32_t x,uint32_t y)
{
	uint32_t result = 1;
	while(y--)
	{
		result *= x;
	}
	return result;
}

void Serial_Send_Num(uint32_t Num,uint8_t Length)
{
	
	for(uint8_t i = 0;i < Length; i++)
	{
		Serial_Send_Data(Num / Pow(10,Length - i - 1) % 10 + '0');
	}	
}

// 重定向 fputc(),使 printf 输出到 串口上
int fputc(int ch,FILE *f)
{
	Serial_Send_Data(ch);
	return ch;
}

void Serial_Printf(char* format,...)
{
	char String[100];
	va_list arg;
	va_start(arg,format);
	vsprintf(String,format,arg);
	va_end(arg);
	Serial_Send_String(String);
}
