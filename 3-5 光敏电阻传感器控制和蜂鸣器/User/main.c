#include "Delay.h"
#include "Buzzer.h"
#include "LightSensor.h"

int main(void)
{
	Buzzer_Init_All();
	Light_Sensor_Init_All();
	while(1)
	{
		// 如果当前光敏电阻传感器为 暗 状态，则蜂鸣器响
		if(Get_Light_Sensor_State(Light_Sensor_1) == Dark)
			Set_Buzzer_State(Buzzer_1,Sound);
		else
			Set_Buzzer_State(Buzzer_1,No_Sound);
	}
}
