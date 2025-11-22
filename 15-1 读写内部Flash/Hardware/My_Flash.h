#ifndef __MY_FLASH_H
#define __MY_FLASH_H

#include "stm32f10x.h" 

/* --- Flash 读写接口 --- */

/**
 * @brief  读取 Flash 指定地址的一个字 (32-bit)
 * @param  Address: 读取地址 (如 0x0800FC00)
 * @retval 读取到的 32位 数据
 */
uint32_t My_Flash_ReadWord(uint32_t Address);

/**
 * @brief  读取 Flash 指定地址的一个半字 (16-bit)
 * @param  Address: 读取地址
 * @retval 读取到的 16位 数据
 */
uint16_t My_Flash_ReadHalfWord(uint32_t Address);

/**
 * @brief  读取 Flash 指定地址的一个字节 (8-bit)
 * @param  Address: 读取地址
 * @retval 读取到的 8位 数据
 */
uint8_t My_Flash_ReadByte(uint32_t Address);

/**
 * @brief  全擦除 (擦除用户区所有页)
 * @note   !! 慎用 !! 这会清空芯片内所有用户数据，耗时较长。
 * @retval None
 */
void My_Flash_EraseAllPages(void);

/**
 * @brief  擦除指定页
 * @note   STM32F103 的一页通常为 1KB (中容量) 或 2KB (大容量)。
 * 写入数据前，必须确保该页已被擦除（数据为 0xFF），否则写入失败。
 * @param  PageAddress: 该页的起始地址
 * @retval None
 */
void My_Flash_ErasePage(uint32_t PageAddress);

/**
 * @brief  编程写入一个字 (32-bit)
 * @param  Address: 写入地址 (必须 4 字节对齐)
 * @param  Data: 要写入的数据
 * @retval None
 */
void My_Flash_ProgramWord(uint32_t Address, uint32_t Data);

/**
 * @brief  编程写入一个半字 (16-bit)
 * @param  Address: 写入地址 (必须 2 字节对齐)
 * @param  Data: 要写入的数据
 * @retval None
 */
void My_Flash_ProgramHalfWord(uint32_t Address, uint16_t Data);

#endif
