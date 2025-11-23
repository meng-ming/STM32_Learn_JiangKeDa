#include "stm32f10x.h"                  // Device header
#include "Serial.h"
#include <stdarg.h>
#include <string.h>

uint8_t HEX_Data_Packet_HEAD;
uint8_t HEX_Data_Packet_END;

uint8_t String_Data_Packet_HEAD;
uint8_t String_Data_Packet_END;

// 发送固定字节数的 HEX 数据包

void Serial_Init(void)
{
	// 当前测试 USART1 的 TX + RX 功能，对应 PA9 PA10 引脚
	// 当串口收到信息时，进入处理信息中断
	
	// 1.使能 USART 和对应的 GPIO引脚
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	// 2. 初始化 GPIO
	GPIO_InitTypeDef GPIO_InitStructure;
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; // USART 传输数据使用 复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
 	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; // USART 接收数据使用 上拉输入
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	// 3. 初始化 USART
	USART_InitTypeDef USART_InitStrucutre;
	USART_InitStrucutre.USART_BaudRate = 115200;
	USART_InitStrucutre.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStrucutre.USART_Mode = USART_Mode_Tx | USART_Mode_Rx; // 发送 + 接收
	USART_InitStrucutre.USART_Parity = USART_Parity_No; // 校验位
	USART_InitStrucutre.USART_StopBits = USART_StopBits_1;
	USART_InitStrucutre.USART_WordLength = USART_WordLength_8b;
	
	USART_Init(USART1,&USART_InitStrucutre);
	
	// 4. 打开 USART 到 NVIC 中断控制
	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);
	
	// 5. 对优先级进行分组
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	// 6. 初始化 NVIC
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	
	NVIC_Init(&NVIC_InitStructure);
	
	// 4. 使能 USART
	USART_Cmd(USART1,ENABLE);
}

void Serial_Send_Byte(uint8_t Byte)
{
	USART_SendData(USART1,Byte);
	// 等待发送完成
	while(USART_GetFlagStatus(USART1,USART_FLAG_TXE) == RESET);
}

void Serial_Send_String(char* String)
{
	for(int i = 0;String[i] != '\0'; i++)
	{
		Serial_Send_Byte(String[i]);
	}
}

void Serial_Send_Array(uint8_t *Array,uint8_t Length)
{
	for(uint8_t i = 0;i < Length;i++)
	{
		Serial_Send_Byte(Array[i]);
	}
}

uint32_t Pow(uint32_t x,uint32_t y)
{
	uint32_t result = 1;
	while(y--)
	{
		result *= x;
	}
	return result;
}

void Serial_Send_Num(uint32_t Num,uint8_t Length)
{
	
	for(uint8_t i = 0;i < Length; i++)
	{
		Serial_Send_Byte(Num / Pow(10,Length - i - 1) % 10 + '0');
	}	
}

// 重定向 fputc(),使 printf 输出到 串口上
int fputc(int ch,FILE *f)
{
	Serial_Send_Byte(ch);
	return ch;
}

void Serial_Printf(char* format,...)
{
	char String[100];
	va_list arg;
	va_start(arg,format);
	vsprintf(String,format,arg);
	va_end(arg);
	Serial_Send_String(String);
}

void Serial_Set_HEX_Data_Packet_Head_End(uint8_t HEXDataPacketHead,uint8_t HEXDataPacketEnd)
{
	HEX_Data_Packet_HEAD = HEXDataPacketHead;
	HEX_Data_Packet_END = HEXDataPacketEnd;
}

void Serial_Set_String_Data_Packet_Head_End(uint8_t StringDataPacketHead,uint8_t StringDataPacketEnd)
{
	String_Data_Packet_HEAD = StringDataPacketHead;
	String_Data_Packet_END = StringDataPacketEnd;
}

void Serial_Send_Data(uint8_t *Send_Data,uint8_t Send_Data_Len)
{
	Serial_Send_Byte(HEX_Data_Packet_HEAD);
	Serial_Send_Array(Send_Data,Send_Data_Len);
	Serial_Send_Byte(HEX_Data_Packet_END);
}

void Serial_Init_Rx_Struct(Serial_Rx_t* Serial_Rx_Struct)
{	
	memset(Serial_Rx_Struct->buffer,0,RX_BUFFER_SIZE);
	Serial_Rx_Struct->state = WAIT_PACKET_HEAD;
	Serial_Rx_Struct->data_ready_flag = 0;
	Serial_Rx_Struct->expected_len = 0;
	Serial_Rx_Struct->index = 0;
}

void Serial_Process_Var_Len_Rx_Byte(Serial_Rx_t* Serial_Rx_Struct,uint8_t Rx_Byte)
{
	switch(Serial_Rx_Struct->state)
	{
		case WAIT_PACKET_HEAD:
			if(Rx_Byte == HEX_Data_Packet_HEAD)
			{
				Serial_Rx_Struct->index = 0;
				// 转到等 数据包 长度
				Serial_Rx_Struct->state = WAIT_PACKET_LEN;
				Serial_Rx_Struct->data_ready_flag = 0;
			}
			break;	
		case WAIT_PACKET_LEN:
			// 判断该数值是否合法
			if(Rx_Byte > RX_BUFFER_SIZE)
			{
				Serial_Rx_Struct->state = WAIT_PACKET_HEAD;
				return;
			}else if(Rx_Byte == 0){
				Serial_Rx_Struct->state = WAIT_PACKET_END;
			}else{
				// 赋值期望接收数据长度
				Serial_Rx_Struct->expected_len = Rx_Byte;
				Serial_Rx_Struct->state = RECEIVE_DATA;
			}
			break;
		case RECEIVE_DATA:
			// 将接收到的信息存在 buffer 中
			Serial_Rx_Struct->buffer[Serial_Rx_Struct->index++] = Rx_Byte; 
		
			// 如果缓存区存储了 expected_len 个数据
			if(Serial_Rx_Struct->index == Serial_Rx_Struct->expected_len)
			{
				Serial_Rx_Struct->state = WAIT_PACKET_END;
			}
			break;
		case WAIT_PACKET_END:
			if(Rx_Byte == HEX_Data_Packet_END)
			{
				Serial_Rx_Struct->data_ready_flag = 1;	
			}else
				Serial_Rx_Struct->data_ready_flag = 0;
			Serial_Rx_Struct->state = WAIT_PACKET_HEAD;
			break;
		default:
			Serial_Rx_Struct->state = WAIT_PACKET_HEAD;
			break;
	}
}

void Serial_Process_String_Rx_Byte(Serial_Rx_t* Serial_Rx_Struct, uint8_t Rx_Byte)
{
	switch(Serial_Rx_Struct->state)
	{
		
		case WAIT_PACKET_HEAD:
			if(Rx_Byte == String_Data_Packet_HEAD)
			{
				Serial_Rx_Struct->index = 0;
				// 转到等 数据包 长度
				Serial_Rx_Struct->state = RECEIVE_DATA;
				Serial_Rx_Struct->data_ready_flag = 0;
			}
			break;	
		case RECEIVE_DATA:
			// 如果是回车符 \r，直接忽略，不存储也不结束，直接跳过
			if (Rx_Byte == '\r') 
            {
                return; 
            }
		
			// 判断是否是包尾
			if(Rx_Byte == String_Data_Packet_END)
			{
				Serial_Rx_Struct->buffer[Serial_Rx_Struct->index] = '\0';
				Serial_Rx_Struct->state = WAIT_PACKET_HEAD;
				Serial_Rx_Struct->data_ready_flag = 1;
				return;
			}
		
			// 普通字符，存入缓冲区; -1 是为了留一个位置给 '\0'
			if(Serial_Rx_Struct->index < RX_BUFFER_SIZE - 1)
			{
				Serial_Rx_Struct->buffer[Serial_Rx_Struct->index++] = Rx_Byte; 
			}else{
					Serial_Rx_Struct->state = WAIT_PACKET_HEAD;
					return;
			}
			break;
		default:
			Serial_Rx_Struct->state = WAIT_PACKET_HEAD;
			break;
	}
}
