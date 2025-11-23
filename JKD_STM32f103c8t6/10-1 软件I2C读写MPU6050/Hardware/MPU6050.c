#include "MPU6050.h"

#define MPU_6050_Addr 0xD0

void MPU_6050_Init(I2C_Pin_Set_s* I2C_Pin_Struct)
{
	My_I2C_Init(I2C_Pin_Struct);
	
	// 通过写入寄存器的值，初始化 MPU_6050 芯片
	/*MPU_6050寄存器初始化，需要对照MPU_6050手册的寄存器描述配置，此处仅配置了部分重要的寄存器*/
	MPU_6050_W_Reg(I2C_Pin_Struct,MPU_6050_PWR_MGMT_1, 0x01);				//电源管理寄存器1，取消睡眠模式，选择时钟源为X轴陀螺仪
	MPU_6050_W_Reg(I2C_Pin_Struct,MPU_6050_PWR_MGMT_2, 0x00);				//电源管理寄存器2，保持默认值0，所有轴均不待机
	MPU_6050_W_Reg(I2C_Pin_Struct,MPU_6050_SMPLRT_DIV, 0x09);				//采样率分频寄存器，配置采样率
	MPU_6050_W_Reg(I2C_Pin_Struct,MPU_6050_CONFIG, 0x06);					//配置寄存器，配置DLPF
	MPU_6050_W_Reg(I2C_Pin_Struct,MPU_6050_GYRO_CONFIG, 0x18);				//陀螺仪配置寄存器，选择满量程为±2000°/s
	MPU_6050_W_Reg(I2C_Pin_Struct,MPU_6050_ACCEL_CONFIG, 0x18);			//加速度计配置寄存器，选择满量程为±16g
}

void MPU_6050_W_Reg(I2C_Pin_Set_s* I2C_Pin_Struct,uint8_t Reg_Addr,uint8_t Write_Data)
{
	// 1. 启动 I2C 通信
	My_I2C_Start(I2C_Pin_Struct);
	
	// 2. 发送一个字节的时序，为 从机地址（7位） + 1位读写位(0: 写 1: 读)，并接收从机的应答
	My_I2C_Send_Byte(I2C_Pin_Struct,MPU_6050_Addr);
	if(My_I2C_Receive_Ack(I2C_Pin_Struct) == 1) goto I2C_Error; // 跳转到错误处理
	
	// 3. 发送一个字节的时序，为 从机的寄存器地址，并接收从机的应答
	My_I2C_Send_Byte(I2C_Pin_Struct,Reg_Addr);
	if(My_I2C_Receive_Ack(I2C_Pin_Struct) == 1) goto I2C_Error; 
	
	// 4. 发送需要传入的一个字节的数据（可以通过循环传入数组）
	My_I2C_Send_Byte(I2C_Pin_Struct,Write_Data);
	if(My_I2C_Receive_Ack(I2C_Pin_Struct) == 1) goto I2C_Error; 
	
	// 5. 不需要继续传输数据直接停止 I2C 通信
	My_I2C_Stop(I2C_Pin_Struct);
	return;
	
I2C_Error:
    My_I2C_Stop(I2C_Pin_Struct); // 出错时必须发送 STOP 释放总线
	return;
}

I2C_Status_e MPU_6050_Read_Buffer(I2C_Pin_Set_s* I2C_Pin_Struct,uint8_t Reg_Addr,uint8_t* Read_Buffer,uint16_t Read_Buffer_Len)
{
	// 1. 启动
	My_I2C_Start(I2C_Pin_Struct);
	
	// 2. 发送一个字节的时序，为 从机地址（7位） + 1位读写位(0: 写 1: 读)，并接收从机的应答
	My_I2C_Send_Byte(I2C_Pin_Struct,MPU_6050_Addr);
	if(My_I2C_Receive_Ack(I2C_Pin_Struct) == 1) goto I2C_Error; // 跳转到错误处理
	
	// 3. 发送一个字节的时序，为 从机的寄存器地址，并接收从机的应答
	My_I2C_Send_Byte(I2C_Pin_Struct,Reg_Addr);
	if(My_I2C_Receive_Ack(I2C_Pin_Struct) == 1) goto I2C_Error;
	
	// 4. 当前指定了一个 从机的某个寄存器 地址
	// 需要进行读取数据操作的话，需要重启 I2C 通信
	My_I2C_Start(I2C_Pin_Struct);
	
	// 5. 发送一个字节的时序，为 从机地址（7位） + 1，并接收从机的应答，指定为 读操作
	My_I2C_Send_Byte(I2C_Pin_Struct,MPU_6050_Addr | 0x01);
	if(My_I2C_Receive_Ack(I2C_Pin_Struct) == 1) goto I2C_Error;
	
	// 接收指定长度的字节数据
	while(Read_Buffer_Len)
	{
		*Read_Buffer = My_I2C_Receive_Byte(I2C_Pin_Struct);
		// 如果读到最后一个字节
		if(Read_Buffer_Len == 1)
			My_I2C_Send_Ack(I2C_Pin_Struct,1);
		else
			My_I2C_Send_Ack(I2C_Pin_Struct,0);
		
		Read_Buffer++;
		Read_Buffer_Len--;
	}
	
	// 7. 不需要继续读，则直接停止 I2C 通信
	My_I2C_Stop(I2C_Pin_Struct);
	
	return I2C_OK;

I2C_Error:
    My_I2C_Stop(I2C_Pin_Struct); // 出错时必须发送 STOP 释放总线
	return I2C_ERROR;
}

uint8_t MPU_6050_Get_ID(I2C_Pin_Set_s* I2C_Pin_Struct)
{
	uint8_t data;
	MPU_6050_Read_Buffer(I2C_Pin_Struct,MPU_6050_WHO_AM_I,&data,1);
	return data;
}
