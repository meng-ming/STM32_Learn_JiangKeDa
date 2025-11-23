#include "My_RTC.h"

void My_RTC_Init(Time_s* Time_Structure)
{
    // 1. 开启 PWR 和 BKP 时钟 (必须先开这个，才能访问备份域)
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);
    
    // 2. 使能备份域访问
    PWR_BackupAccessCmd(ENABLE);
    
    if (BKP_ReadBackupRegister(BKP_DR1) != 0xB5A5)
    {
        // --- 第一次上电 (或电池没电了) ---
        
        // 3. 开启 LSE
        RCC_LSEConfig(RCC_LSE_ON);
        
        // 等待 LSE 起振 (增加超时机制，防止死机)
        uint32_t Timeout = 0;
        while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET)
        {
            Timeout++;
            if(Timeout > 0x0FFFFFFF) return; // 硬件故障，直接返回
        }
        
        // 4. 选择 RTC 时钟源
        RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);
        RCC_RTCCLKCmd(ENABLE);
        
        // 5. 等待同步
        RTC_WaitForSynchro();
        RTC_WaitForLastTask();
        
        // 6. 配置分频 (32.768KHz -> 1Hz)
        RTC_SetPrescaler(32768 - 1);
        RTC_WaitForLastTask();
        
        if (Time_Structure != NULL)
        {
            // 使用用户传入的时间
            My_RTC_Set_Time(Time_Structure);
        }
        else
        {
            // 如果用户传了 NULL，使用默认出厂时间 (例如 2023-01-01 12:00:00)
            // 手动构造一个默认值
            Time_s DefaultTime;
            DefaultTime.tm_year = 2025 - 1900;
            DefaultTime.tm_mon  = 11 - 1;
            DefaultTime.tm_mday = 21;
            DefaultTime.tm_hour = 12;
            DefaultTime.tm_min  = 0;
            DefaultTime.tm_sec  = 0;
            My_RTC_Set_Time(&DefaultTime);
        }
		
        RTC_WaitForLastTask();
        
        // 8. 写入标志位，表示初始化已完成
        BKP_WriteBackupRegister(BKP_DR1, 0xB5A5);
    }
    else
    {
        // --- 系统复位，但 RTC 正在运行 ---
        // 不需要重新配置 LSE 和 Prescaler，也不要重置时间
        // 只需要等待同步即可
        RTC_WaitForSynchro();
        RTC_WaitForLastTask();
    }
}

void My_RTC_Set_Time(Time_s *Time_Structure)
{
    // mktime 将 struct tm 转换为 秒数 (time_t)
    // 注意：中国时区是 UTC+8，mktime 通常按 UTC 计算
    // 如果发现相差 8 小时，需要手动 +/- 8*3600
    time_t time_cnt = mktime(Time_Structure) - 8 * 3600;
    
    // 写入之前必须允许访问备份域
    PWR_BackupAccessCmd(ENABLE);
    RTC_SetCounter(time_cnt);
    RTC_WaitForLastTask();
}

void My_RTC_Get_Time(Time_s* Time_Structure)
{
    // 获取当前计数
    time_t time_cnt = RTC_GetCounter() + 8 * 3600;
    
    // localtime 将 秒数 转换为 struct tm
    struct tm* temp_tm = localtime(&time_cnt);
    
    // 拷贝数据
    if (temp_tm != NULL && Time_Structure != NULL)
    {
        *Time_Structure = *temp_tm;
    }
}