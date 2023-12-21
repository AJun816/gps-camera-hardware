#include "stm32f10x.h" // Device header
#include "Delay.h"
#include "LED.h"
#include "Key.h"
#include "OLED.h"
#include "OV7670.h"
#include "Serial.h"

uint8_t KeyNum; // 定义用于接收按键键码的变量

int main(void)
{
	/*模块初始化*/
	LED_Init();	 // LED初始化
	Key_Init();	 // 按键初始化
	OLED_Init(); // OLED初始化
	OV7670_Init();
	Serial_Init(); // 波特率9600 (在SYSCLOCK为72MHz时正常传输)

	while (1)
	{
		OV7670_GetPic();

		KeyNum = Key_GetNum(); // 获取按键键码

		if (KeyNum == 1) // 按键1按下
		{
			LED1_Turn(); // LED1翻转
			OLED_ShowString(2, 3, "Key1 press");
		}

		if (KeyNum == 2) // 按键2按下
		{
			LED2_Turn(); // LED2翻转
			OLED_ShowString(2, 3, "key2 press");
		}
	}
}
