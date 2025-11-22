#include "My_SPI.h"

/* 私有全局变量 (static 限制作用域) */
static My_SPI_Pin_Set_s g_SPI;

/* 内部辅助函数：写 SCK */
static void SPI_W_SCK(uint8_t BitVal)
{
    GPIO_WriteBit(g_SPI.GPIO_X, g_SPI.Pin_SCK, (BitAction)BitVal);
}

/* 内部辅助函数：写 MOSI */
static void SPI_W_MOSI(uint8_t BitVal)
{
    GPIO_WriteBit(g_SPI.GPIO_X, g_SPI.Pin_MOSI, (BitAction)BitVal);
}

/* 内部辅助函数：读 MISO */
static uint8_t SPI_R_MISO(void)
{
    return GPIO_ReadInputDataBit(g_SPI.GPIO_X, g_SPI.Pin_MISO);
}

void My_SPI_Pin_Set_Init(const My_SPI_Pin_Set_s MySPIPinSet)
{
    g_SPI = MySPIPinSet; // 结构体直接赋值
}

void My_SPI_RCC_Enable_GPIOX(void)
{
    if(g_SPI.GPIO_X == GPIOA) RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    else if (g_SPI.GPIO_X == GPIOB) RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    else if (g_SPI.GPIO_X == GPIOC) RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
}

void My_SPI_Init(const My_SPI_Pin_Set_s MySPIPinSet)
{
    My_SPI_Pin_Set_Init(MySPIPinSet);
    My_SPI_RCC_Enable_GPIOX();
    
    GPIO_InitTypeDef GPIO_InitStructure;
    
    // 1. 输出引脚配置 (SCK, MOSI, CS)
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Pin = g_SPI.Pin_CS | g_SPI.Pin_MOSI | g_SPI.Pin_SCK;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(g_SPI.GPIO_X, &GPIO_InitStructure);
    
    // 2. 输入引脚配置 (MISO)
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 
    GPIO_InitStructure.GPIO_Pin = g_SPI.Pin_MISO;
    GPIO_Init(g_SPI.GPIO_X, &GPIO_InitStructure);
    
    // 3. 默认电平设置
    My_SPI_Stop(); // CS 拉高
    
    // 根据 CPOL 设置 SCK 空闲电平
    // Mode 0/1: CPOL=0 (空闲低)
    if(g_SPI.My_SPI_Mode == MY_SPI_MODE_0 || g_SPI.My_SPI_Mode == MY_SPI_MODE_1)
    {
        SPI_W_SCK(0);
    }
    else // Mode 2/3: CPOL=1 (空闲高)
    {
        SPI_W_SCK(1);
    }
}

void My_SPI_Start(void)
{
    GPIO_WriteBit(g_SPI.GPIO_X, g_SPI.Pin_CS, Bit_RESET);
}

void My_SPI_Stop(void)
{
    GPIO_WriteBit(g_SPI.GPIO_X, g_SPI.Pin_CS, Bit_SET);
}

uint8_t My_SPI_Swap_Byte(uint8_t Byte_Send)
{
    uint8_t i; // 【修复】C89 变量声明
    uint8_t Receive_Byte = 0x00;
    
    // ---------------------------------------------------------
    // 场景 1: CPHA = 0 (Mode 0, Mode 2)
    // 特征: 第 1 个边沿采样，第 2 个边沿移出
    // ---------------------------------------------------------
    if(g_SPI.My_SPI_Mode == MY_SPI_MODE_0 || g_SPI.My_SPI_Mode == MY_SPI_MODE_2)
    {
        for(i = 0; i < 8; i++)
        {
            // 1. 主机写数据 (在边沿发生前准备好)
            SPI_W_MOSI(!!(Byte_Send & (0x80 >> i)));
            
            // 2. 产生第 1 个边沿 (采样沿)
            if(g_SPI.My_SPI_Mode == MY_SPI_MODE_0) SPI_W_SCK(1); // 0->1
            else SPI_W_SCK(0); // 1->0
            
            // 3. 主机读数据
            if(SPI_R_MISO() == 1) Receive_Byte |= (0x80 >> i);
            
            // 4. 产生第 2 个边沿 (恢复沿)
            if(g_SPI.My_SPI_Mode == MY_SPI_MODE_0) SPI_W_SCK(0); // 1->0
            else SPI_W_SCK(1); // 0->1
        }
    }
    // ---------------------------------------------------------
    // 场景 2: CPHA = 1 (Mode 1, Mode 3)
    // 特征: 第 1 个边沿移出，第 2 个边沿采样
    // ---------------------------------------------------------
    else 
    {
        for(i = 0; i < 8; i++)
        {
            // 1. 产生第 1 个边沿 (移出沿)
            if(g_SPI.My_SPI_Mode == MY_SPI_MODE_1) SPI_W_SCK(1); // 0->1
            else SPI_W_SCK(0); // 1->0
            
            // 2. 主机写数据 (移出)
            SPI_W_MOSI(!!(Byte_Send & (0x80 >> i)));
            
            // 3. 产生第 2 个边沿 (采样沿)
            if(g_SPI.My_SPI_Mode == MY_SPI_MODE_1) SPI_W_SCK(0); // 1->0
            else SPI_W_SCK(1); // 0->1
            
            // 4. 主机读数据 (采样)
            if(SPI_R_MISO() == 1) Receive_Byte |= (0x80 >> i);
        }
    }
    
    return Receive_Byte;
}
