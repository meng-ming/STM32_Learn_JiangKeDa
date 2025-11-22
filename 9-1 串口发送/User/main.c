#include "Delay.h"
#include "stm32f10x.h"                  // Device header
#include "OLED.h"
#include "Serial.h"

int main(void)
{
	OLED_Init();
	Serial_Init();
	
	Serial_Send_Data('B');
	
	uint8_t test[] = {'A','B','C','D'};
	
	char* string = "Helloworld!";
	
	Serial_Send_String(string);
	
	Serial_Send_Array(test,sizeof(test) / sizeof(test[0]));
	
	Serial_Send_Num(123,3);
	
	printf("Num = %d",123456);

	Serial_Printf("你觉得怎么样");
	
	while(1)
	{
		
	}
}
