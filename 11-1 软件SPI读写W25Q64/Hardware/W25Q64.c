#include "W25Q64.h"

void W25Q64_Init(const My_SPI_Pin_Set_s MySPIPinSet)
{
    My_SPI_Init(MySPIPinSet);
}

void W25Q64_Get_ID(uint8_t* Manu_ID, uint16_t* Device_ID)
{
    My_SPI_Start();
    My_SPI_Swap_Byte(W25Q64_JEDEC_ID); // 发送 0x9F
    
    *Manu_ID = My_SPI_Swap_Byte(W25Q64_DUMMY_BYTE);
    
    // 接收高8位
    *Device_ID = My_SPI_Swap_Byte(W25Q64_DUMMY_BYTE);
    *Device_ID <<= 8;
    
    // 接收低8位
    *Device_ID |= My_SPI_Swap_Byte(W25Q64_DUMMY_BYTE);
    
    My_SPI_Stop();
}

void W25Q64_Write_Enable(void)
{
    My_SPI_Start();
    My_SPI_Swap_Byte(W25Q64_WRITE_ENABLE); // 发送 0x06
    My_SPI_Stop();
}

void W25Q64_Wait_Busy(void)
{
    uint32_t Timeout = 100000; // 定义超时防止死机
    
    My_SPI_Start();
    My_SPI_Swap_Byte(W25Q64_READ_STATUS_REGISTER_1); // 发送 0x05 读取状态寄存器1
    
    // 循环发送 Dummy Byte 来读取状态
    // 检查 Bit0 (BUSY) 是否为 1
    while ((My_SPI_Swap_Byte(W25Q64_DUMMY_BYTE) & W25Q64_FLAG_BUSY) == W25Q64_FLAG_BUSY)
    {
        if((Timeout--) == 0)
        {
            break; // 超时跳出
        }
    }
    
    My_SPI_Stop();
}

void W25Q64_Page_Program(uint32_t Addr, uint8_t* Data_Array, uint16_t Data_Len)
{
    uint16_t i;
    
    // 1. 写操作前必须先写使能
    W25Q64_Write_Enable();
    
    // 2. 启动编程
    My_SPI_Start();
    My_SPI_Swap_Byte(W25Q64_PAGE_PROGRAM); // 0x02
    
    // 发送 24位 地址
    My_SPI_Swap_Byte(Addr >> 16);
    My_SPI_Swap_Byte(Addr >> 8);
    My_SPI_Swap_Byte(Addr);
    
    // 发送数据
    for(i = 0; i < Data_Len; i++)
    {
        My_SPI_Swap_Byte(Data_Array[i]);
    }
    My_SPI_Stop();
    
    // 3. 操作结束后等待忙碌结束，确保数据写入完成
    // 这样上层应用调用完这个函数后，可以立即进行下一次操作
    W25Q64_Wait_Busy();
}

void W25Q64_Sector_Erase(uint32_t Addr)
{
    // 1. 写操作前必须先写使能
    W25Q64_Write_Enable();
    
    // 2. 启动擦除
    My_SPI_Start();
    My_SPI_Swap_Byte(W25Q64_SECTOR_ERASE_4KB); // 0x20
    
    // 发送 24位 地址
    My_SPI_Swap_Byte(Addr >> 16);
    My_SPI_Swap_Byte(Addr >> 8);
    My_SPI_Swap_Byte(Addr);
    
    My_SPI_Stop();
    
    // 3. 等待擦除完成 (擦除通常比较慢，必须等)
    W25Q64_Wait_Busy();
}

void W25Q64_Read_Data(uint32_t Addr, uint8_t* Data_Array, uint32_t Data_Len)
{
    // 读取操作通常不需要 Wait_Busy
    
    uint32_t i; // C89 变量声明
    
    My_SPI_Start();
    My_SPI_Swap_Byte(W25Q64_READ_DATA); // 0x03
    
    // 发送 24位 地址
    My_SPI_Swap_Byte(Addr >> 16);
    My_SPI_Swap_Byte(Addr >> 8);
    My_SPI_Swap_Byte(Addr);
    
    // 读取数据
    for(i = 0; i < Data_Len; i++)
    {
        Data_Array[i] = My_SPI_Swap_Byte(W25Q64_DUMMY_BYTE);
    }
    My_SPI_Stop();
}
