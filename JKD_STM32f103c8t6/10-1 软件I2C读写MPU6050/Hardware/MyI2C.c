#include "MyI2C.h"
#include "Delay.h"

// 宏定义 I2C 延时时间
#define I2C_Delay_US 5

// 内部辅助函数：根据 GPIOx 获取对应的 RCC 时钟 
static uint32_t My_I2C_Get_RCC(GPIO_TypeDef* Port)
{
	if (Port == GPIOA) return RCC_APB2Periph_GPIOA;
	if (Port == GPIOB) return RCC_APB2Periph_GPIOB;
	if (Port == GPIOC) return RCC_APB2Periph_GPIOC;
	return RCC_APB2Periph_GPIOB; // 默认
}

void My_I2C_Init(I2C_Pin_Set_s *I2C_Pin_Set)
{
	// 软件 I2C 通信不需要使用 I2C 库函数，并且可以将 SCL 和 SDA 接到任意一个 GPIO 口
	// 1. 动态开启时钟
	RCC_APB2PeriphClockCmd(My_I2C_Get_RCC(I2C_Pin_Set->I2C_Port), ENABLE);
	
	// 2. 初始化GPIO
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;//开漏输出，防止主机从机一高一低打通路线
	GPIO_InitStructure.GPIO_Pin = I2C_Pin_Set->I2C_SCL_Pin | I2C_Pin_Set->I2C_SDA_Pin;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(I2C_Pin_Set->I2C_Port,&GPIO_InitStructure);
	
	// 3. 默认输出高电平（空闲状态）
	GPIO_SetBits(I2C_Pin_Set->I2C_Port,I2C_Pin_Set->I2C_SCL_Pin | I2C_Pin_Set->I2C_SDA_Pin);
}

// 主机对 SCL 写
void My_I2C_W_SCL(I2C_Pin_Set_s *I2C_Pin_Set,uint8_t Bit_Value)
{
	GPIO_WriteBit(I2C_Pin_Set->I2C_Port,I2C_Pin_Set->I2C_SCL_Pin,(BitAction)Bit_Value);
	Delay_us(I2C_Delay_US);// 防止后续翻转不及时
}

// 对 SDA 写
void My_I2C_W_SDA(I2C_Pin_Set_s *I2C_Pin_Set,uint8_t Bit_Value)
{
	GPIO_WriteBit(I2C_Pin_Set->I2C_Port,I2C_Pin_Set->I2C_SDA_Pin,(BitAction)Bit_Value);
	Delay_us(I2C_Delay_US);// 防止后续翻转不及时
}

uint8_t My_I2C_R_SDA(I2C_Pin_Set_s *I2C_Pin_Set)
{
	uint8_t SDA_Bit_Value = GPIO_ReadInputDataBit(I2C_Pin_Set->I2C_Port,I2C_Pin_Set->I2C_SDA_Pin);
	Delay_us(I2C_Delay_US);
	return SDA_Bit_Value;
}

void My_I2C_Start(I2C_Pin_Set_s *I2C_Pin_Set)
{
	// 确保 SCL 和 SDA 均为上拉
	My_I2C_W_SDA(I2C_Pin_Set,1);
	My_I2C_W_SCL(I2C_Pin_Set,1);
	
	// 先下拉 SDA，再下拉 SCL
	My_I2C_W_SDA(I2C_Pin_Set,0);
	My_I2C_W_SCL(I2C_Pin_Set,0);
}

void My_I2C_Stop(I2C_Pin_Set_s *I2C_Pin_Set)
{
	// 确保 SCL 和 SDA 均为下拉
	My_I2C_W_SDA(I2C_Pin_Set,0);
	// My_I2C_W_SCL(I2C_Pin_Set,0); // 进行读写操作之后 SCL 默认为 0
	
	// 先上拉 SCL，再上拉 SDA
	My_I2C_W_SCL(I2C_Pin_Set,1);
	My_I2C_W_SDA(I2C_Pin_Set,1);
}

void My_I2C_Send_Byte(I2C_Pin_Set_s *I2C_Pin_Set,uint8_t Byte)
{
	// 1. 依次发送 8 bit 数据(从高位到低位)
	for(int i = 0 ;i < 8 ;i++)
	{
		// 1.1 先写入数据(从到位到低位依次写入)
		My_I2C_W_SDA(I2C_Pin_Set,Byte & (0x80 >> i) );
		// 1.2 拉高 SCL
		My_I2C_W_SCL(I2C_Pin_Set,1); // 此时，从机 会将 SDA 上发送的数据读走
		// 1.3 拉低 SCL
		My_I2C_W_SCL(I2C_Pin_Set,0);
	}
}

uint8_t My_I2C_Receive_Byte(I2C_Pin_Set_s* I2C_Pin_Set)
{
	uint8_t Receive_Byte = 0x00;
	// 1. 主机释放 SDA 的控制权
	My_I2C_W_SDA(I2C_Pin_Set,1);
	
	// 2. 依次从 SDA 上读取数据
	for(int i = 0;i < 8;i++)
	{
		// 2.1 从机写入数据（也是高位先行）
		// 2.2 主机拉高 SCL 
		My_I2C_W_SCL(I2C_Pin_Set,1);
		// 2.3 主机此时能够从 SDA 数据总线上读取从机发送的数据
		if(My_I2C_R_SDA(I2C_Pin_Set) == 1)
			Receive_Byte |= (0x80 >> i);
		// 2.4 主机拉低 SCL 
		My_I2C_W_SCL(I2C_Pin_Set,0);
	}
	return Receive_Byte;
}

void My_I2C_Send_Ack(I2C_Pin_Set_s *I2C_Pin_Set,uint8_t Send_Ack_Bit_Value)
{

	// 1.1 先写入数据(从到位到低位依次写入)
	My_I2C_W_SDA(I2C_Pin_Set,Send_Ack_Bit_Value);
	// 1.2 拉高 SCL
	My_I2C_W_SCL(I2C_Pin_Set,1); // 此时，从机 会将 SDA 上发送的数据读走
	// 1.3 拉低 SCL
	My_I2C_W_SCL(I2C_Pin_Set,0);
}

uint8_t My_I2C_Receive_Ack(I2C_Pin_Set_s* I2C_Pin_Set)
{
	uint8_t Receive_Ack_Bit_Value;
	// 1. 主机释放 SDA 的控制权
	My_I2C_W_SDA(I2C_Pin_Set,1);
	
	// 2. 依次从 SDA 上读取数据
	// 2.1 从机写入数据（也是高位先行）
	// 2.2 主机拉高 SCL 
	My_I2C_W_SCL(I2C_Pin_Set,1);
	// 2.3 主机此时能够从 SDA 数据总线上读取从机发送的数据
	Receive_Ack_Bit_Value = My_I2C_R_SDA(I2C_Pin_Set);
	// 2.4 主机拉低 SCL 
	My_I2C_W_SCL(I2C_Pin_Set,0);
	
	return Receive_Ack_Bit_Value;
}
