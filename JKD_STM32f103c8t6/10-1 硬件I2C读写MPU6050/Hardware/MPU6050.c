#include "MPU6050.h"

#define MPU_6050_Addr 0xD0

// 定义超时阈值 
#define I2C_TIMEOUT_MAX  10000 

static I2C_TypeDef* I2C_Select = I2C2;

/**
 * @brief  带超时的事件检查
 * @param  I2C_Event: 期望的事件 (如 I2C_EVENT_MASTER_MODE_SELECT)
 * @retval I2C_Status_e: I2C_OK 或 I2C_TIMEOUT
 */
static I2C_Status_e I2C_WaitEvent(I2C_TypeDef* I2Cx, uint32_t I2C_Event)
{
    uint32_t Timeout = I2C_TIMEOUT_MAX;
    
    // 只要事件没发生，就一直检查
    while(I2C_CheckEvent(I2Cx, I2C_Event) != SUCCESS)
    {
		// 检查是否发生 ACK 失败 (AF)
        if (I2C_GetFlagStatus(I2Cx, I2C_FLAG_AF) == SET)
        {
            I2C_ClearFlag(I2Cx, I2C_FLAG_AF); // 清除标志
            return I2C_ERROR; // 返回错误，表示设备不存在或未应答
        }
		
        // 倒计时
        if((Timeout--) == 0)
        {
            // 超时了！
            return I2C_TIMEOUT;
        }
    }
    return I2C_OK;
}

void MPU_6050_Init(void)
{
	// 1. RCC使能 I2C 和 GPIO 时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	
	// 2. 初始化 GPIO
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;//复用开漏输出，输出交由片上外设控制
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
	// 3. 初始化 I2C
	I2C_InitTypeDef I2C_InitSructure;

	I2C_InitSructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit; // 选择 7bits 的确认地址
	I2C_InitSructure.I2C_OwnAddress1 = 0x00;
	I2C_InitSructure.I2C_ClockSpeed = 200000; // if x <= 100KHz,x = 100KHz,else if x <= 400KHz,x = 400KHz 
	I2C_InitSructure.I2C_DutyCycle = I2C_DutyCycle_2; // 低电平：高电平
	I2C_InitSructure.I2C_Mode = I2C_Mode_I2C;
	I2C_InitSructure.I2C_Ack = I2C_Ack_Enable; // 使用应答

	I2C_Init(I2C_Select,&I2C_InitSructure);
	
	// 4. 使能 I2C
	I2C_Cmd(I2C_Select,ENABLE);
	
	
	// 通过写入寄存器的值，初始化 MPU_6050 芯片
	//*MPU_6050寄存器初始化，需要对照MPU_6050手册的寄存器描述配置，此处仅配置了部分重要的寄存器*/
	MPU_6050_W_Reg(MPU_6050_PWR_MGMT_1, 0x01);				//电源管理寄存器1，取消睡眠模式，选择时钟源为X轴陀螺仪
	MPU_6050_W_Reg(MPU_6050_PWR_MGMT_2, 0x00);				//电源管理寄存器2，保持默认值0，所有轴均不待机
	MPU_6050_W_Reg(MPU_6050_SMPLRT_DIV, 0x09);				//采样率分频寄存器，配置采样率
	MPU_6050_W_Reg(MPU_6050_CONFIG, 0x06);					//配置寄存器，配置DLPF
	MPU_6050_W_Reg(MPU_6050_GYRO_CONFIG, 0x18);				//陀螺仪配置寄存器，选择满量程为±2000°/s
	MPU_6050_W_Reg(MPU_6050_ACCEL_CONFIG, 0x18);			//加速度计配置寄存器，选择满量程为±16g
}

I2C_Status_e MPU_6050_W_Reg(uint8_t Reg_Addr,uint8_t Write_Data)
{
	// 根据 主发送器传送序列图依次完成
	
	// 1. 启动 I2C 通信
	I2C_GenerateSTART(I2C_Select,ENABLE);
	if(I2C_WaitEvent(I2C_Select,I2C_EVENT_MASTER_MODE_SELECT) != I2C_OK) goto I2C_Err_Handler; // 等待 EV5 事件完成
	
	// 2. 发送一个字节的时序，为 从机地址（7位） + 1位读写位(0: 写 1: 读)
	I2C_Send7bitAddress(I2C_Select,MPU_6050_Addr,I2C_Direction_Transmitter);
	if(I2C_WaitEvent(I2C_Select,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED) != I2C_OK) goto I2C_Err_Handler; // 等待 发送EV6 事件完成(没有 EV8_1 事件)
	
	// 3. 发送一个字节的时序，为 从机的寄存器地址
	I2C_SendData(I2C_Select,Reg_Addr);
	if(I2C_WaitEvent(I2C_Select,I2C_EVENT_MASTER_BYTE_TRANSMITTING) != I2C_OK) goto I2C_Err_Handler; // 等待 EV8 事件完成
	
	// 4. 发送需要传入的一个字节的数据（可以通过循环传入数组）
	// 当前只模拟发送一个字节的数据，因此也即发送最后一个字节的数据，需要等待 EV8_2 事件完成
	I2C_SendData(I2C_Select,Write_Data);
	if(I2C_WaitEvent(I2C_Select,I2C_EVENT_MASTER_BYTE_TRANSMITTED) != I2C_OK) goto I2C_Err_Handler; // 等待 EV8_2 事件完成
	
	// 5. 不需要继续传输数据直接停止 I2C 通信
	I2C_GenerateSTOP(I2C_Select,ENABLE);
	
	return I2C_OK;
	
I2C_Err_Handler:
	I2C_GenerateSTOP(I2C_Select, ENABLE);
    
    return I2C_TIMEOUT; // 告诉上层出错了
}

I2C_Status_e MPU_6050_Read_Buffer(uint8_t Reg_Addr,uint8_t* Read_Buffer,uint16_t Read_Buffer_Len)
{
	// 1. 启动 I2C 通信
	I2C_GenerateSTART(I2C_Select,ENABLE);
	if(I2C_WaitEvent(I2C_Select,I2C_EVENT_MASTER_MODE_SELECT) != I2C_OK) goto I2C_Err_Handler; // 等待 EV5 事件完成
	
	// 2. 发送一个字节的时序，为 从机地址（7位） + 1位读写位(0: 写 1: 读)
	I2C_Send7bitAddress(I2C_Select,MPU_6050_Addr,I2C_Direction_Transmitter);
	if(I2C_WaitEvent(I2C_Select,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED) != I2C_OK) goto I2C_Err_Handler; // 等待 发送EV6 事件完成
	
	// 3. 发送一个字节的时序，为 从机的寄存器地址
	I2C_SendData(I2C_Select,Reg_Addr);
	if(I2C_WaitEvent(I2C_Select,I2C_EVENT_MASTER_BYTE_TRANSMITTED) != I2C_OK) goto I2C_Err_Handler; // 等待 EV8_2 事件完成，保证重启之前传输完成
	
	// 4. 当前指定了一个 从机的某个寄存器 地址
	// 需要进行读取数据操作的话，需要重启 I2C 通信
	I2C_GenerateSTART(I2C_Select,ENABLE);
	if(I2C_WaitEvent(I2C_Select,I2C_EVENT_MASTER_MODE_SELECT) != I2C_OK) goto I2C_Err_Handler; // 等待 EV5 事件完成
	
	// 5. 发送一个字节的时序，为 从机地址（7位） + 1，并指定为 读操作
	I2C_Send7bitAddress(I2C_Select,MPU_6050_Addr,I2C_Direction_Receiver);
	if(I2C_WaitEvent(I2C_Select,I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED) != I2C_OK) goto I2C_Err_Handler; // 等待 接收EV6 事件完成
	
	// 6. 正常接收数据
	while(Read_Buffer_Len)
	{
		if(Read_Buffer_Len == 1)
		{
			// 如果是最后一个字节
            // 1. 禁止 ACK (发送 NACK)
            I2C_AcknowledgeConfig(I2C_Select, DISABLE);
            // 2. 发送 STOP
            I2C_GenerateSTOP(I2C_Select, ENABLE);
		}
		
		// 6.1 先等待 EV7 事件完成
		if(I2C_WaitEvent(I2C_Select,I2C_EVENT_MASTER_BYTE_RECEIVED) != I2C_OK) goto I2C_Err_Handler; // 等待 接收EV7 事件完成，表示当前数据寄存器已经接收到一个字节的数据
		
		// 6.2 从 数据寄存器读取字节数据
		*Read_Buffer = I2C_ReceiveData(I2C_Select);
		Read_Buffer++;
		Read_Buffer_Len--;
	}
	
	// 7. 恢复 ACK 标志位，保证连续接收字节数据
	I2C_AcknowledgeConfig(I2C_Select,ENABLE);
	
	// 8. 返回 I2C 状态
	return I2C_OK;
	
I2C_Err_Handler:
	I2C_GenerateSTOP(I2C_Select, ENABLE);
    I2C_AcknowledgeConfig(I2C_Select, ENABLE); // 出错也要恢复 ACK
    return I2C_TIMEOUT; // 告诉上层出错了
}

uint8_t MPU_6050_Get_ID(void)
{
	uint8_t data;
	MPU_6050_Read_Buffer(MPU_6050_WHO_AM_I,&data,1);
	return data;
}
