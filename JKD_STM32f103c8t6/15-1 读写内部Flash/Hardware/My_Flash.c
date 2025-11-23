#include "My_Flash.h"

uint32_t My_Flash_ReadWord(uint32_t Address)
{
	return *((__IO uint32_t *)(Address));	//使用指针访问指定地址下的数据并返回
}

uint16_t My_Flash_ReadHalfWord(uint32_t Address)
{
	return *((__IO uint16_t *)(Address));	//使用指针访问指定地址下的数据并返回
}

uint8_t My_Flash_ReadByte(uint32_t Address)
{
	return *((__IO uint8_t *)(Address));	//使用指针访问指定地址下的数据并返回
}

void My_Flash_EraseAllPages(void)
{
	FLASH_Unlock();					//解锁
	FLASH_EraseAllPages();			//全擦除
	FLASH_Lock();					//加锁
}

void My_Flash_ErasePage(uint32_t PageAddress)
{
	FLASH_Unlock();					//解锁
	FLASH_ErasePage(PageAddress);	//页擦除
	FLASH_Lock();					//加锁
}

void My_Flash_ProgramWord(uint32_t Address, uint32_t Data)
{
	FLASH_Unlock();							//解锁
	FLASH_ProgramWord(Address, Data);		//编程字
	FLASH_Lock();							//加锁
}

void My_Flash_ProgramHalfWord(uint32_t Address, uint16_t Data)
{
	FLASH_Unlock();							//解锁
	FLASH_ProgramHalfWord(Address, Data);	//编程半字
	FLASH_Lock();							//加锁
}
