#ifndef __SERIAL_H
#define __SERIAL_H
#include <stdio.h>

void Serial_Init(void);

void Serial_Send_Data(uint8_t Byte); // 发送一个 8bits 的数据

void Serial_Send_String(char* String); // 发送一个字符串

void Serial_Send_Array(uint8_t *Array,uint8_t Length); // 发送一个数组

uint32_t Pow(uint32_t x,uint32_t y); // 求取 x^y 

void Serial_Send_Num(uint32_t Num,uint8_t Length); // 发送一个十进制数字

void Serial_Printf(char* format,...); // 任意的 USART 均能使用格式化打印

#endif
