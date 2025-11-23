#ifndef __PWM_LED_H
#define __PWM_LED_H

typedef struct
{
	uint16_t PSC;
	uint16_t ARR;
	uint16_t CCR;
}PWM_LED_t;

void PWM_LED_Init(void);

// 根据PWM设定值初始化PSR,ARR,CRR对应的参数
// 参数对应单位：
// PWM_Frequency: x Hz
// PWM_Resolution：x %
// PWM_Duty_Ratio: x %
void Calcuelate_Param_Init(uint16_t PWM_Frequency,uint16_t PWM_Resolution,uint8_t PWM_Duty_Ratio);

#endif
