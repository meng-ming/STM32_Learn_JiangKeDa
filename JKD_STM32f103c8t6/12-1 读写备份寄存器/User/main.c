#include "Delay.h"
#include "stm32f10x.h"                  // Device header
#include "OLED.h"
#include "Key.h"

int main(void)
{
	uint16_t Write_Data[2] = {0x1234,0x5678};
	uint16_t Read_Data[2];
	
	OLED_Init();

	Key_Init(GPIOB,GPIO_Pin_0);
	
	// 1. 使能电源和备份接口时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);

	// 2. 允许访问备份域
	PWR_BackupAccessCmd(ENABLE);
	
	OLED_ShowString(1,1,"W:");
	OLED_ShowString(3,1,"R:");
	
	while(1)
	{
		BKP_WriteBackupRegister(BKP_DR1,Write_Data[0]);
		BKP_WriteBackupRegister(BKP_DR2,Write_Data[1]);
		
		OLED_ShowHexNum(1,3,Write_Data[0],4);
		OLED_ShowHexNum(1,8,Write_Data[1],4);
		
		if(Key_GetPressed_Num() == Key_1)
		{
			Write_Data[0]++;
			Write_Data[1]++;
		}
		
		OLED_ShowHexNum(3,3,BKP_ReadBackupRegister(BKP_DR1),4);
		OLED_ShowHexNum(3,8,BKP_ReadBackupRegister(BKP_DR2),4);
	}
}
