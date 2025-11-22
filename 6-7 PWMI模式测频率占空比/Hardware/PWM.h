#ifndef __PWM_H
#define __PWM_H

void PWM_Init(void); // 初始化 PWM 

void PWM_Set_Compare1(uint16_t Compare);// 设置占空比 CRR

void PWM_Set_Prescaler(uint16_t Prescaler); // 设置 PSC

#endif
