#include "Delay.h"
#include "stm32f10x.h"                  // Device header
#include "OLED.h"
#include "MY_DMA.h"

int main(void)
{
	OLED_Init();
	
	const uint16_t Buffer_Size = 4;
	
	uint8_t Periph_Data[Buffer_Size] = {0x01,0x02,0x03,0x04};
	uint8_t Memory_Data[Buffer_Size] = {0};
	
	My_DMA_Init((uint32_t)Memory_Data,(uint32_t)Periph_Data,Buffer_Size);
	
	OLED_ShowString(1,1,"Periph");
	OLED_ShowString(3,1,"Memory");
	
	OLED_ShowNum(1,8,(uint32_t)Periph_Data,8);
	OLED_ShowNum(3,8,(uint32_t)Memory_Data,8);
	
	while(1)
	{
		// 显示转运前的 Periph_Data 和 Memory_Data
		OLED_ShowHexNum(2,1,Periph_Data[0],2);
		OLED_ShowHexNum(2,4,Periph_Data[1],2);
		OLED_ShowHexNum(2,7,Periph_Data[2],2);
		OLED_ShowHexNum(2,10,Periph_Data[3],2);
		OLED_ShowHexNum(4,1,Memory_Data[0],2);
		OLED_ShowHexNum(4,4,Memory_Data[1],2);
		OLED_ShowHexNum(4,7,Memory_Data[2],2);
		OLED_ShowHexNum(4,10,Memory_Data[3],2);
		
		Delay_ms(1000);
		
		// 启动 DMA 转运
		Start_DMA_Transfer(Buffer_Size);
		
		OLED_ShowHexNum(2,1,Periph_Data[0],2);
		OLED_ShowHexNum(2,4,Periph_Data[1],2);
		OLED_ShowHexNum(2,7,Periph_Data[2],2);
		OLED_ShowHexNum(2,10,Periph_Data[3],2);
		OLED_ShowHexNum(4,1,Memory_Data[0],2);
		OLED_ShowHexNum(4,4,Memory_Data[1],2);
		OLED_ShowHexNum(4,7,Memory_Data[2],2);
		OLED_ShowHexNum(4,10,Memory_Data[3],2);
		
		Delay_ms(1000);
		
		Periph_Data[0]++;
		Periph_Data[1]++;
		Periph_Data[2]++;
		Periph_Data[3]++;
		
	}
}
