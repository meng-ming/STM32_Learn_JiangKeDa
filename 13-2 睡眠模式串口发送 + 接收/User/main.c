#include "Delay.h"
#include "stm32f10x.h"                  // Device header
#include "OLED.h"
#include "Serial.h"

// 定义全局接收数据结构体
Serial_Rx_t Serial_Rx_Data_Struct;
	
int main(void)
{	
	const uint8_t Send_Data_Len = 4;
	const uint8_t HEX_Data_Packet_Head = 0xFF;
	const uint8_t HEX_Data_Packet_End = 0xFE;
	
	const uint8_t String_Data_Packet_Head = '@';
	const uint8_t String_Data_Packet_End = '\n';
	
	OLED_Init();
	Serial_Init();
	
	// 初始化接收数据结构体
	Serial_Init_Rx_Struct(&Serial_Rx_Data_Struct);
	
	// 设置 包头包尾
	Serial_Set_HEX_Data_Packet_Head_End(HEX_Data_Packet_Head,HEX_Data_Packet_End);

	Serial_Set_String_Data_Packet_Head_End(String_Data_Packet_Head,String_Data_Packet_End);
	
	uint8_t Send_Data[Send_Data_Len] = {0x41,0x42,0x43,0x44};
	
	Serial_Send_Data(Send_Data,Send_Data_Len);
	
//	OLED_ShowString(1,1,"TxData:");
	
	OLED_ShowString(1,1,"RxData:");
	uint8_t count = 0;
	while(1)
	{
		// 打印 HEX 数据
//		if(Serial_Rx_Data_Struct.data_ready_flag)
//		{			
//			// 将结构体中的数据打印出来
//			for(int i = 0;i< Serial_Rx_Data_Struct.expected_len ;i++)
//			{
//				OLED_ShowHexNum(i / 5 + 1,1 + 3 * (i % 5),Serial_Rx_Data_Struct.buffer[i],2);
//				
//			}
//			// 手动恢复标志位，防止当接受完一次数据之后，对下一个接收的字节数据进行判断时，还会打印一遍
//			Serial_Rx_Data_Struct.data_ready_flag = 0;
//		}
		
		// 打印 String 数据
		if(Serial_Rx_Data_Struct.data_ready_flag)
		{
			// 获取接收到的字符串指针
			OLED_ShowNum(2,1,Serial_Rx_Data_Struct.index,2);
			Serial_Rx_Data_Struct.buffer[Serial_Rx_Data_Struct.index] = '\0';
            char* rx_str = (char*)Serial_Rx_Data_Struct.buffer;
			OLED_ShowString(3,1,rx_str);
			Serial_Rx_Data_Struct.data_ready_flag = 0;
		}
		
		// 测试 睡眠模式
		OLED_ShowString(4,1,"Running");
		OLED_ShowString(4,1,"       ");
		
		if (Serial_Rx_Data_Struct.data_ready_flag == 0)
		{
			OLED_ShowNum(1,8,count,4);
			__WFI(); // 只有没事干的时候，才允许睡觉
		}
	}
}

// USART 接收数据中断处理函数
void USART1_IRQHandler(void)
{
	if(USART_GetITStatus(USART1,USART_IT_RXNE) == SET)
	{
		// 获取一个字节
		uint8_t Receive_Byte = USART_ReceiveData(USART1);
		// 交给接收数据状态机处理
//		Serial_Process_Var_Len_Rx_Byte(&Serial_Rx_Data_Struct,Receive_Byte);
		Serial_Process_String_Rx_Byte(&Serial_Rx_Data_Struct,Receive_Byte);
	}
}
