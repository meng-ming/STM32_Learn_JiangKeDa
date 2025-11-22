#include "Delay.h"
#include "stm32f10x.h"                  // Device header
#include "OLED.h"
#include "W25Q64.h"

int main(void)
{
	uint8_t Manu_ID;
	uint16_t Device_ID;
	
	uint8_t Array_Write[] = {0x53, 0x02, 0x78, 0x04};
	uint8_t Array_Read[4];
	
	My_SPI_Pin_Set_s My_SPI_Pin_Set;
	My_SPI_Pin_Set.GPIO_X = GPIOA;
	My_SPI_Pin_Set.My_SPI_Mode = MY_SPI_MODE_0;
	My_SPI_Pin_Set.Pin_CS = GPIO_Pin_4;
	My_SPI_Pin_Set.Pin_SCK = GPIO_Pin_5;
	My_SPI_Pin_Set.Pin_MISO = GPIO_Pin_6;
	My_SPI_Pin_Set.Pin_MOSI = GPIO_Pin_7;
	My_SPI_Pin_Set.SPI_X = SPI1;
	
	OLED_Init();
	W25Q64_Init(My_SPI_Pin_Set);
	
	OLED_ShowString(1, 1, "MID:   DID:");
	OLED_ShowString(2, 1, "W:");
	OLED_ShowString(3, 1, "R:");
	
	W25Q64_Get_ID(&Manu_ID, &Device_ID);
	OLED_ShowHexNum(1, 5, Manu_ID, 2);
	OLED_ShowHexNum(1, 12, Device_ID, 4);
	
	W25Q64_Sector_Erase(0x000000);
	W25Q64_Page_Program(0x000000, Array_Write, 4);
	
	W25Q64_Read_Data(0x000000, Array_Read, 4);
	
	OLED_ShowHexNum(2, 3, Array_Write[0], 2);
	OLED_ShowHexNum(2, 6, Array_Write[1], 2);
	OLED_ShowHexNum(2, 9, Array_Write[2], 2);
	OLED_ShowHexNum(2, 12, Array_Write[3], 2);
	
	OLED_ShowHexNum(3, 3, Array_Read[0], 2);
	OLED_ShowHexNum(3, 6, Array_Read[1], 2);
	OLED_ShowHexNum(3, 9, Array_Read[2], 2);
	OLED_ShowHexNum(3, 12, Array_Read[3], 2);
	
	while(1)
	{

	}
}
