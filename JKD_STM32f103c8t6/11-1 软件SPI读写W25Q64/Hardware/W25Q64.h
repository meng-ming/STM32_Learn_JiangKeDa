#ifndef __W25Q64_H
#define __W25Q64_H

#include "stm32f10x.h"
#include "My_SPI.h"
#include "W25Q64_Reg.h"

/* --- 状态定义 --- */
#define W25Q64_FLAG_BUSY  0x01 // Status Register 1 的最低位 (S0)

/**
 * @brief  初始化 W25Q64 及其底层的 SPI 引脚
 * @param  MySPIPinSet: SPI 引脚配置结构体
 * @retval None
 */
void W25Q64_Init(const My_SPI_Pin_Set_s MySPIPinSet);

/**
 * @brief  读取芯片制造商和设备 ID (JEDEC ID)
 * @param  Manu_ID:   [Out] 存储制造商 ID 的指针 (Winbond = 0xEF)
 * @param  Device_ID: [Out] 存储设备 ID 的指针 (e.g. 0x4017)
 * @retval None
 */
void W25Q64_Get_ID(uint8_t* Manu_ID, uint16_t* Device_ID);

/**
 * @brief  发送写使能指令 (Write Enable)
 * @note   在页编程、扇区擦除、块擦除、芯片擦除、写状态寄存器之前必须调用
 * @retval None
 */
void W25Q64_Write_Enable(void);

/**
 * @brief  阻塞等待芯片忙碌状态结束 (Wait for Busy)
 * @note   包含超时机制，防止死循环
 * @retval None
 */
void W25Q64_Wait_Busy(void);

/**
 * @brief  页编程 (Page Program)
 * @param  Addr:       写入的起始地址 (24-bit)
 * @param  Data_Array: 要写入的数据缓冲区指针
 * @param  Data_Len:   写入数据的长度 (最大 256 字节，注意不要跨页)
 * @retval None
 */
void W25Q64_Page_Program(uint32_t Addr, uint8_t* Data_Array, uint16_t Data_Len);

/**
 * @brief  扇区擦除 (Sector Erase 4KB)
 * @param  Addr: 扇区内的任意地址 (24-bit)，芯片会自动对齐到 4KB 扇区首地址
 * @retval None
 */
void W25Q64_Sector_Erase(uint32_t Addr);

/**
 * @brief  读取数据 (Read Data)
 * @param  Addr:       读取的起始地址 (24-bit)
 * @param  Data_Array: [Out] 存储读取数据的缓冲区
 * @param  Data_Len:   读取长度
 * @retval None
 */
void W25Q64_Read_Data(uint32_t Addr, uint8_t* Data_Array, uint32_t Data_Len);

#endif /* __W25Q64_H */
