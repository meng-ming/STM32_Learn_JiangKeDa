#ifndef __MPU_6050_H
#define __MPU_6050_H

#include "MPU6050_Reg.h"
#include "MyI2C.h"

typedef enum {
    I2C_OK       = 0,
    I2C_ERROR    = 1,
} I2C_Status_e;

void MPU_6050_Init(I2C_Pin_Set_s* I2C_Pin_Struct);

void MPU_6050_W_Reg(I2C_Pin_Set_s* I2C_Pin_Struct,uint8_t Reg_Addr,uint8_t Write_Data); // 对 MPU6050 指定地址 写入 数据 

I2C_Status_e MPU_6050_Read_Buffer(I2C_Pin_Set_s* I2C_Pin_Struct,uint8_t Reg_Addr,uint8_t* Read_Buffer,uint16_t Read_Buffer_Len);// 对 MPU6050 指定地址 读取 数据

uint8_t MPU_6050_Get_ID(I2C_Pin_Set_s* I2C_Pin_Struct); // 获取 MPU_6050 的 ID 值

#endif
