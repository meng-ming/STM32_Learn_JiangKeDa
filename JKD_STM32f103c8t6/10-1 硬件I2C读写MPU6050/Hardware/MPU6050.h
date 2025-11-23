#ifndef __MPU_6050_H
#define __MPU_6050_H

#include "stm32f10x.h"                  // Device header
#include "MPU6050_Reg.h"

typedef enum {
    I2C_OK       = 0,
    I2C_ERROR    = 1,
    I2C_TIMEOUT  = 2
} I2C_Status_e;

void MPU_6050_Init(void);

I2C_Status_e MPU_6050_W_Reg(uint8_t Reg_Addr,uint8_t Write_Data); // 对 MPU6050 指定地址 写入 数据 

I2C_Status_e MPU_6050_Read_Buffer(uint8_t Reg_Addr,uint8_t* Read_Buffer,uint16_t Read_Buffer_Len);// 对 MPU6050 指定地址 读取 一定长度的数据

uint8_t MPU_6050_Get_ID(void); // 获取 MPU_6050 的 ID 值

#endif
