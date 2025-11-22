#ifndef __ENCODER_H
#define __ENCODER_H

// 用于旋转编码器计数
extern volatile int16_t Encoder_Count;

// 初始化 旋转编码器 的中断
void Encoder_Init(void);

#endif
