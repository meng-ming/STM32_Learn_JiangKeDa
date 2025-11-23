#include "Delay.h"
#include "stm32f10x.h"                  // Device header
#include "OLED.h"
#include "MPU6050.h"

int main(void)
{
	I2C_Pin_Set_s I2C_Pin_Set;
	I2C_Pin_Set.I2C_Port = GPIOB;
	I2C_Pin_Set.I2C_SCL_Pin = GPIO_Pin_10;
	I2C_Pin_Set.I2C_SDA_Pin = GPIO_Pin_11;
	
	uint8_t Raw_Data[14];// 存放原始数据的缓冲 (Acc 6 + Temp 2 + Gyro 6 = 14)
	int16_t AccX, AccY, AccZ, GYROX, GYROY, GYROZ;

	OLED_Init();
	
	MPU_6050_Init(&I2C_Pin_Set);

	// 显示当前 MPU6050 芯片的 ID
	OLED_ShowString(1,1,"ID:");
	
	OLED_ShowHexNum(1,4,MPU_6050_Get_ID(&I2C_Pin_Set),2);
	
	while(1)
	{
		if(MPU_6050_Read_Buffer(&I2C_Pin_Set,MPU_6050_ACCEL_XOUT_H,Raw_Data,14) == I2C_OK)
		{
			// 拼接数据 (高8位 << 8 | 低8位)
			// X、Y、Z 轴的加速度
            AccX = (Raw_Data[0] << 8) | Raw_Data[1];
            AccY = (Raw_Data[2] << 8) | Raw_Data[3];
            AccZ = (Raw_Data[4] << 8) | Raw_Data[5];
			
			// 陀螺仪 X、Y、Z 轴的角速度
			GYROX = (Raw_Data[8] << 8) | Raw_Data[9];
			GYROY = (Raw_Data[10] << 8) | Raw_Data[11];
			GYROZ = (Raw_Data[12] << 8) | Raw_Data[13];
			
			OLED_ShowString(2,1,"AX:");
			OLED_ShowSignedNum(2,4,AccX,4);
			OLED_ShowString(3,1,"AY:");
			OLED_ShowSignedNum(3,4,AccY,4);
			OLED_ShowString(4,1,"AZ:");
			OLED_ShowSignedNum(4,4,AccZ,4);
				
			OLED_ShowString(2,9,"GX:");
			OLED_ShowSignedNum(2,12,GYROX,4);
			OLED_ShowString(3,9,"GY:");
			OLED_ShowSignedNum(3,12,GYROY,4);
			OLED_ShowString(4,9,"GZ:");
			OLED_ShowSignedNum(4,12,GYROZ,4);
		}else{
			OLED_ShowString(2, 1, "I2C Err");
		}
			Delay_ms(50);
	}
}
