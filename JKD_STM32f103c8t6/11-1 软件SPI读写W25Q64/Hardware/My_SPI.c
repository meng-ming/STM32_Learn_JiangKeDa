#include "My_SPI.h"

/* 私有全局变量 */
static My_SPI_Pin_Set_s g_SPI_Pin_Set;

void My_SPI_Pin_Set_Init(const My_SPI_Pin_Set_s MySPIPinSet)
{
    g_SPI_Pin_Set = MySPIPinSet;
}

void My_SPI_RCC_Enable_GPIOX_And_SPI(void)
{
    // 1. 使能 GPIO 时钟
    if(g_SPI_Pin_Set.GPIO_X == GPIOA) RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    else if (g_SPI_Pin_Set.GPIO_X == GPIOB) RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    else if (g_SPI_Pin_Set.GPIO_X == GPIOC) RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
    
    // 2. 使能 SPI 时钟 (SPI1在APB2, SPI2/3在APB1)
    if(g_SPI_Pin_Set.SPI_X == SPI1)
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
    else if(g_SPI_Pin_Set.SPI_X == SPI2)
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);
    // else if SPI3 ...
}

void My_SPI_Init(const My_SPI_Pin_Set_s MySPIPinSet)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    SPI_InitTypeDef SPI_InitStructure;

    My_SPI_Pin_Set_Init(MySPIPinSet);
    My_SPI_RCC_Enable_GPIOX_And_SPI();
    
    // --- GPIO 配置 ---
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  
    // 1. SCK, MOSI: 复用推挽输出 (AF_PP)
    // 去掉 Pin_CS，只放硬件控制的引脚
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Pin = g_SPI_Pin_Set.Pin_MOSI | g_SPI_Pin_Set.Pin_SCK;
    GPIO_Init(g_SPI_Pin_Set.GPIO_X, &GPIO_InitStructure);
    
    // 2. MISO: 上拉输入 (IPU) 或 浮空输入 (IN_FLOATING)
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 
    GPIO_InitStructure.GPIO_Pin = g_SPI_Pin_Set.Pin_MISO;
    GPIO_Init(g_SPI_Pin_Set.GPIO_X, &GPIO_InitStructure);
    
    // 3. CS: 通用推挽输出 (Out_PP) - 软件控制
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
    GPIO_InitStructure.GPIO_Pin = g_SPI_Pin_Set.Pin_CS; 
    GPIO_Init(g_SPI_Pin_Set.GPIO_X, &GPIO_InitStructure);
    
    // 初始化后立刻拉高 CS (取消选中)
    GPIO_WriteBit(g_SPI_Pin_Set.GPIO_X, g_SPI_Pin_Set.Pin_CS, Bit_SET);
    
    // --- SPI 配置 ---
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_128; // 速度设置
    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;   // 全双工
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;                        // 主机模式
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;                    // 8位数据
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;                   // 高位先行
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;                            // 软件片选
    SPI_InitStructure.SPI_CRCPolynomial = 7;
    
    // CPOL 和 CPHA 配置
    if(g_SPI_Pin_Set.My_SPI_Mode == MY_SPI_MODE_0 || g_SPI_Pin_Set.My_SPI_Mode == MY_SPI_MODE_1)
        SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
    else
        SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
    
    if(g_SPI_Pin_Set.My_SPI_Mode == MY_SPI_MODE_0 || g_SPI_Pin_Set.My_SPI_Mode == MY_SPI_MODE_2)
        SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
    else
        SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
    
    SPI_Init(g_SPI_Pin_Set.SPI_X, &SPI_InitStructure);
    
    // 使能 SPI
    SPI_Cmd(g_SPI_Pin_Set.SPI_X, ENABLE);
}

void My_SPI_Start(void)
{
    GPIO_WriteBit(g_SPI_Pin_Set.GPIO_X, g_SPI_Pin_Set.Pin_CS, Bit_RESET);
}

void My_SPI_Stop(void)
{
    GPIO_WriteBit(g_SPI_Pin_Set.GPIO_X, g_SPI_Pin_Set.Pin_CS, Bit_SET);
}

uint8_t My_SPI_Swap_Byte(uint8_t Byte_Send)
{
    // 加上超时机制 (Timeout) 防止死循环
    uint32_t Timeout = 10000;

    // 1. 等待 TXE (发送缓冲空)
    while(SPI_I2S_GetFlagStatus(g_SPI_Pin_Set.SPI_X, SPI_I2S_FLAG_TXE) == RESET)
    {
        if((Timeout--) == 0) return 0; // 超时退出
    }
    
    // 2. 发送数据
    SPI_I2S_SendData(g_SPI_Pin_Set.SPI_X, Byte_Send);
    
    // 3. 等待 RXNE (接收缓冲非空)
    Timeout = 10000;
    while(SPI_I2S_GetFlagStatus(g_SPI_Pin_Set.SPI_X, SPI_I2S_FLAG_RXNE) == RESET)
    {
        if((Timeout--) == 0) return 0; // 超时退出
    }
    
    // 4. 读取数据 (这会自动清除 RXNE 标志)
    return SPI_I2S_ReceiveData(g_SPI_Pin_Set.SPI_X);
}