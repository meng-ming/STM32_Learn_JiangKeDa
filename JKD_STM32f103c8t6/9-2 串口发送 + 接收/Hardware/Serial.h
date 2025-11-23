#ifndef __SERIAL_H
#define __SERIAL_H
#include <stdio.h>

#define RX_BUFFER_SIZE 128

typedef enum{
	WAIT_PACKET_HEAD, // 数据包头
	WAIT_PACKET_LEN, // 等待长度字节
	RECEIVE_DATA,
	WAIT_PACKET_END
}Receive_State_e;

typedef struct {
    Receive_State_e state;          // 当前状态
    uint8_t buffer[RX_BUFFER_SIZE]; // 数据缓冲区
    uint16_t index;                 // 当前缓冲区索引
    uint8_t data_ready_flag;        // 接收完成标志 (1: 完成, 0: 未完成)
	uint8_t expected_len;			// 该包应收字节数
} Serial_Rx_t;

void Serial_Init(void);

void Serial_Send_Byte(uint8_t Byte); // 发送一个 8bits 的数据

void Serial_Send_String(char* String); // 发送一个字符串

void Serial_Send_Array(uint8_t *Array,uint8_t Length); // 发送一个数组

uint32_t Pow(uint32_t x,uint32_t y); // 求取 x^y 

void Serial_Send_Num(uint32_t Num,uint8_t Length); // 发送一个十进制数字

void Serial_Printf(char* format,...); // 任意的 USART 均能使用格式化打印

uint8_t Serial_Get_Receive_Flag(void); // 获取是否接收到数据标志位

uint8_t Serial_Receive_Data(void);// 获取串口接收到的数据

// -------------- 发送和接收 有包头包尾 的数据

void Serial_Set_HEX_Data_Packet_Head_End(uint8_t HEXDataPacketHead,uint8_t HEXDataPacketEnd); // 指定串口接发 HEX 数据的包头包尾

void Serial_Set_String_Data_Packet_Head_End(uint8_t StringDataPacketHead,uint8_t StringDataPacketEnd); // 指定串口接发 String 数据的包头包尾

void Serial_Init_Rx_Struct(Serial_Rx_t* Serial_Rx_Struct);// 初始化接收数据结构体

// 发送指定包头、包尾的包长数据，使用 HEX 发送
void Serial_Send_Data(uint8_t *Send_Data,uint8_t Send_Data_Len);

void Serial_Process_Var_Len_Rx_Byte(Serial_Rx_t* Serial_Rx_Struct,uint8_t Rx_Byte);// 接收任意长度的包含包头包尾的数据

// 接收字符串数据
void Serial_Process_String_Rx_Byte(Serial_Rx_t* Serial_Rx_Struct, uint8_t Rx_Byte);

#endif
