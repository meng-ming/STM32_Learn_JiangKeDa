#include "LightSensor.h"

// 定义包含所有 LED 的数组
static Light_Sensor_Definition_t Light_Sensors[] =
{
	{GPIOB,GPIO_Pin_13,Dark},
};

#define NUM_LIGHT_SENSOR_S sizeof(Light_Sensors)/sizeof(Light_Sensors[0])

void Light_Sensor_Init(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin_Used)
{
	if (GPIOx == GPIOA)
    {
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    }
    else if (GPIOx == GPIOB)
    {
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    }
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;// 上拉
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_Used;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOx,&GPIO_InitStructure);

	// 设置使用的引脚初始化为高电平，防止默认打开LED
	GPIO_SetBits(GPIOx,GPIO_Pin_Used);
}

void Light_Sensor_Init_All(void)
{
	for (int i = 0; i < NUM_LIGHT_SENSOR_S; i++)
	{
		Light_Sensor_Init(Light_Sensors[i].port,Light_Sensors[i].pin);
	}
}

Light_Sensor_State_e Get_Light_Sensor_State(Light_Sensor_Num_e Light_Sensor_Num)
{
    if(GPIO_ReadInputDataBit(Light_Sensors[Light_Sensor_Num].port,Light_Sensors[Light_Sensor_Num].pin) == 1)
	{
		Light_Sensors[Light_Sensor_Num].state = Dark;
		return Dark;
	}
    else
	{
		Light_Sensors[Light_Sensor_Num].state = Light;
		return Light;
	}

}
