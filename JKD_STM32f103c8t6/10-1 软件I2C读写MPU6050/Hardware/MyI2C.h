#ifndef __MYI2C_H
#define __MYI2C_H

#include "stm32f10x.h"                  // Device header

typedef struct
{
	GPIO_TypeDef* I2C_Port;
	uint16_t I2C_SCL_Pin;
	uint16_t I2C_SDA_Pin;
}I2C_Pin_Set_s;

void My_I2C_Init(I2C_Pin_Set_s* I2C_Pin_Set);

void My_I2C_W_SCL(I2C_Pin_Set_s *I2C_Pin_Set,uint8_t Bit_Value); // 对 SCL 写

void My_I2C_W_SDA(I2C_Pin_Set_s *I2C_Pin_Set,uint8_t Bit_Value);// 对 SDA 写

uint8_t My_I2C_R_SDA(I2C_Pin_Set_s *I2C_Pin_Set);// 读取 SDA

void My_I2C_Start(I2C_Pin_Set_s *I2C_Pin_Set);// 启动 I2C

void My_I2C_Stop(I2C_Pin_Set_s *I2C_Pin_Set);// 停止 I2C

void My_I2C_Send_Byte(I2C_Pin_Set_s *I2C_Pin_Set,uint8_t Byte);// I2C 发送一个字节

uint8_t My_I2C_Receive_Byte(I2C_Pin_Set_s* I2C_Pin_Set);// I2C 接收一个字节

void My_I2C_Send_Ack(I2C_Pin_Set_s *I2C_Pin_Set,uint8_t Send_Ack_Bit_Value);// 主机 接收 到从机发送的字节消息后，发送的应答

uint8_t My_I2C_Receive_Ack(I2C_Pin_Set_s* I2C_Pin_Set);// 主机发送字节消息后，接收到从机的应答

#endif
