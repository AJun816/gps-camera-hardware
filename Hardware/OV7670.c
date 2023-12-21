#include "stm32f10x.h"                  // Device header
#include "SCCB.h"
#include "OLED.h"
#include "Delay.h"
#include "Serial.h"

/* 0100 0010 --- 写地址   ***   0100 0011 ---读地址 */
#define OV7670_ADDRESS 0x42

/* @brief 下面几个函数主要是用于便捷读取串口电平 */
uint8_t OV7670_VS(void)
{
	return GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_12);
}
uint8_t OV7670_HREF(void)
{
	return GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_13);
}
uint8_t OV7670_PCLK(void)
{
	return GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_14);
}
/*
  * @brief	OV7670引脚初始化函数，在OV7670_Init() 里触发，不需外部调用
  * @param  无
  * @retval 无
*/
void OV7670_Pin_Init()
{	
	/* VS HREF 和 PCLK IO口初始化 */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);        /* GPIOB使能 */
	
	GPIO_InitTypeDef GPIO_InitStruct;                   /* 结构体定义 */
    GPIO_InitStruct.GPIO_Pin=GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14;  /* PB12,PB13,PB14 */
    GPIO_InitStruct.GPIO_Mode=GPIO_Mode_IPU;                   /* 上拉输入 */
    GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;                   /*  50MHz*/
    GPIO_Init(GPIOB,&GPIO_InitStruct);
	
	/* D0-D7 IO口初始化 */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);        /* GPIOA使能 */
	
    GPIO_InitStruct.GPIO_Pin=0xFF;                   /* PA0-PA7 */
    GPIO_InitStruct.GPIO_Mode=GPIO_Mode_IN_FLOATING;                   /* 上拉输入 */
    GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;                   /*  50MHz*/
    GPIO_Init(GPIOA,&GPIO_InitStruct); 
}
/*
  * @brief	OV7670写寄存器
  * @param  写入寄存器的地址
  * @param  写入的内容（一个字节数据）
  * @retval 无
*/
void OV7670_WriteReg(uint8_t RegAddress, uint8_t Data)
{
	SCCB_Start();
	
	SCCB_SendByte(OV7670_ADDRESS);
	SCCB_ReceiveAck();			//SCCB_ReceiveAck()==0说明上一步执行成功，下面的两个也是
	
	SCCB_SendByte(RegAddress);
	SCCB_ReceiveAck();
	
	SCCB_SendByte(Data);
	SCCB_ReceiveAck();
	
	SCCB_Stop();
}
/*
  * @brief	OV7670读取寄存器
  * @param  读取寄存器的地址
  * @retval 相应地址的寄存器的数据
*/
uint8_t OV7670_ReadReg(uint8_t RegAddress)
{
	uint8_t Data;
	
	SCCB_Start();
	SCCB_SendByte(OV7670_ADDRESS);
	SCCB_ReceiveAck();			//SCCB_ReceiveAck()==0说明上一步执行成功，下面的两个也是
	SCCB_SendByte(RegAddress);
	SCCB_ReceiveAck();
	SCCB_Stop();		//这里的STOP很必要！！！！是SCCB不同于I2C的地方！！！
	
	SCCB_Start();
	SCCB_SendByte(OV7670_ADDRESS | 0x01);
	SCCB_ReceiveAck();
	Data = SCCB_ReceiveByte();
	SCCB_SendNA();
	SCCB_Stop();
	
	return Data;
}
/* 
  * @brief 参照其他教程的寄存器设置
*/
void OV7670_RegExample(void)
{
	OV7670_WriteReg(0x3a, 0x04);
	OV7670_WriteReg(0x40, 0xd0);
	OV7670_WriteReg(0x12, 0x14);

	OV7670_WriteReg(0x32, 0x80);
	OV7670_WriteReg(0x17, 0x16);
	OV7670_WriteReg(0x18, 0x04);
	OV7670_WriteReg(0x19, 0x02);
	OV7670_WriteReg(0x1a, 0x7b);
	OV7670_WriteReg(0x03, 0x06);

	OV7670_WriteReg(0x0c, 0x00);
	OV7670_WriteReg(0x15, 0x00);
	OV7670_WriteReg(0x3e, 0x00);
	OV7670_WriteReg(0x70, 0x3a);
	OV7670_WriteReg(0x71, 0x35);
	OV7670_WriteReg(0x72, 0x11);
	OV7670_WriteReg(0x73, 0x00);

	OV7670_WriteReg(0xa2, 0x02);
	OV7670_WriteReg(0x11, 0x81);
	OV7670_WriteReg(0x7a, 0x20);
	OV7670_WriteReg(0x7b, 0x1c);
	OV7670_WriteReg(0x7c, 0x28);

	OV7670_WriteReg(0x7d, 0x3c);
	OV7670_WriteReg(0x7e, 0x55);
	OV7670_WriteReg(0x7f, 0x68);
	OV7670_WriteReg(0x80, 0x76);
	OV7670_WriteReg(0x81, 0x80);

	OV7670_WriteReg(0x82, 0x88);
	OV7670_WriteReg(0x83, 0x8f);
	OV7670_WriteReg(0x84, 0x96);
	OV7670_WriteReg(0x85, 0xa3);
	OV7670_WriteReg(0x86, 0xaf);

	OV7670_WriteReg(0x87, 0xc4);
	OV7670_WriteReg(0x88, 0xd7);
	OV7670_WriteReg(0x89, 0xe8);
	OV7670_WriteReg(0x13, 0xe0);
	OV7670_WriteReg(0x00, 0x00);

	OV7670_WriteReg(0x10, 0x00);
	OV7670_WriteReg(0x0d, 0x00);
	OV7670_WriteReg(0x14, 0x28);
	OV7670_WriteReg(0xa5, 0x05);
	OV7670_WriteReg(0xab, 0x07);

	OV7670_WriteReg(0x24, 0x75);
	OV7670_WriteReg(0x25, 0x63);
	OV7670_WriteReg(0x26, 0xA5);
	OV7670_WriteReg(0x9f, 0x78);
	OV7670_WriteReg(0xa0, 0x68);

	OV7670_WriteReg(0xa1, 0x03);
	OV7670_WriteReg(0xa6, 0xdf);
	OV7670_WriteReg(0xa7, 0xdf);
	OV7670_WriteReg(0xa8, 0xf0);
	OV7670_WriteReg(0xa9, 0x90);

	OV7670_WriteReg(0xaa, 0x94);
	OV7670_WriteReg(0x13, 0xe5);
	OV7670_WriteReg(0x0e, 0x61);
	OV7670_WriteReg(0x0f, 0x4b);
	OV7670_WriteReg(0x16, 0x02);

	OV7670_WriteReg(0x1e, 0x37);
	OV7670_WriteReg(0x21, 0x02);
	OV7670_WriteReg(0x22, 0x91);
	OV7670_WriteReg(0x29, 0x07);
	OV7670_WriteReg(0x33, 0x0b);

	OV7670_WriteReg(0x35, 0x0b);
	OV7670_WriteReg(0x37, 0x1d);
	OV7670_WriteReg(0x38, 0x71);
	OV7670_WriteReg(0x39, 0x2a);
	OV7670_WriteReg(0x3c, 0x78);

	OV7670_WriteReg(0x4d, 0x40);
	OV7670_WriteReg(0x4e, 0x20);
	OV7670_WriteReg(0x69, 0x00);
	OV7670_WriteReg(0x6b, 0x40);
	OV7670_WriteReg(0x74, 0x19);
	OV7670_WriteReg(0x8d, 0x4f);

	OV7670_WriteReg(0x8e, 0x00);
	OV7670_WriteReg(0x8f, 0x00);
	OV7670_WriteReg(0x90, 0x00);
	OV7670_WriteReg(0x91, 0x00);
	OV7670_WriteReg(0x92, 0x00);

	OV7670_WriteReg(0x96, 0x00);
	OV7670_WriteReg(0x9a, 0x80);
	OV7670_WriteReg(0xb0, 0x84);
	OV7670_WriteReg(0xb1, 0x0c);
	OV7670_WriteReg(0xb2, 0x0e);

	OV7670_WriteReg(0xb3, 0x82);
	OV7670_WriteReg(0xb8, 0x0a);
	OV7670_WriteReg(0x43, 0x14);
	OV7670_WriteReg(0x44, 0xf0);
	OV7670_WriteReg(0x45, 0x34);

	OV7670_WriteReg(0x46, 0x58);
	OV7670_WriteReg(0x47, 0x28);
	OV7670_WriteReg(0x48, 0x3a);
	OV7670_WriteReg(0x59, 0x88);
	OV7670_WriteReg(0x5a, 0x88);

	OV7670_WriteReg(0x5b, 0x44);
	OV7670_WriteReg(0x5c, 0x67);
	OV7670_WriteReg(0x5d, 0x49);
	OV7670_WriteReg(0x5e, 0x0e);
	OV7670_WriteReg(0x64, 0x04);
	OV7670_WriteReg(0x65, 0x20);

	OV7670_WriteReg(0x66, 0x05);
	OV7670_WriteReg(0x94, 0x04);
	OV7670_WriteReg(0x95, 0x08);
	OV7670_WriteReg(0x6c, 0x0a);
	OV7670_WriteReg(0x6d, 0x55);


	OV7670_WriteReg(0x4f, 0x80);
	OV7670_WriteReg(0x50, 0x80);
	OV7670_WriteReg(0x51, 0x00);
	OV7670_WriteReg(0x52, 0x22);
	OV7670_WriteReg(0x53, 0x5e);
	OV7670_WriteReg(0x54, 0x80);

	OV7670_WriteReg(0x09, 0x03);

	OV7670_WriteReg(0x6e, 0x11);
	OV7670_WriteReg(0x6f, 0x9f);
	OV7670_WriteReg(0x55, 0x00);
	OV7670_WriteReg(0x56, 0x40);
	OV7670_WriteReg(0x57, 0x40);
	OV7670_WriteReg(0x6a, 0x40);
	OV7670_WriteReg(0x01, 0x40);
	OV7670_WriteReg(0x02, 0x40);
	OV7670_WriteReg(0x13, 0xe7);
	OV7670_WriteReg(0x15, 0x00);  
	
		
	OV7670_WriteReg(0x58, 0x9e);
	
	OV7670_WriteReg(0x41, 0x08);
	OV7670_WriteReg(0x3f, 0x00);
	OV7670_WriteReg(0x75, 0x05);
	OV7670_WriteReg(0x76, 0xe1);
	OV7670_WriteReg(0x4c, 0x00);
	OV7670_WriteReg(0x77, 0x01);
	OV7670_WriteReg(0x3d, 0xc2);	
	OV7670_WriteReg(0x4b, 0x09);
	OV7670_WriteReg(0xc9, 0x60);
	OV7670_WriteReg(0x41, 0x38);
	
	OV7670_WriteReg(0x34, 0x11);
	OV7670_WriteReg(0x3b, 0x02); 

	OV7670_WriteReg(0xa4, 0x89);
	OV7670_WriteReg(0x96, 0x00);
	OV7670_WriteReg(0x97, 0x30);
	OV7670_WriteReg(0x98, 0x20);
	OV7670_WriteReg(0x99, 0x30);
	OV7670_WriteReg(0x9a, 0x84);
	OV7670_WriteReg(0x9b, 0x29);
	OV7670_WriteReg(0x9c, 0x03);
	OV7670_WriteReg(0x9d, 0x4c);
	OV7670_WriteReg(0x9e, 0x3f);
	OV7670_WriteReg(0x78, 0x04);
	
	OV7670_WriteReg(0x79, 0x01);
	OV7670_WriteReg(0xc8, 0xf0);
	OV7670_WriteReg(0x79, 0x0f);
	OV7670_WriteReg(0xc8, 0x00);
	OV7670_WriteReg(0x79, 0x10);
	OV7670_WriteReg(0xc8, 0x7e);
	OV7670_WriteReg(0x79, 0x0a);
	OV7670_WriteReg(0xc8, 0x80);
	OV7670_WriteReg(0x79, 0x0b);
	OV7670_WriteReg(0xc8, 0x01);
	OV7670_WriteReg(0x79, 0x0c);
	OV7670_WriteReg(0xc8, 0x0f);
	OV7670_WriteReg(0x79, 0x0d);
	OV7670_WriteReg(0xc8, 0x20);
	OV7670_WriteReg(0x79, 0x09);
	OV7670_WriteReg(0xc8, 0x80);
	OV7670_WriteReg(0x79, 0x02);
	OV7670_WriteReg(0xc8, 0xc0);
	OV7670_WriteReg(0x79, 0x03);
	OV7670_WriteReg(0xc8, 0x40);
	OV7670_WriteReg(0x79, 0x05);
	OV7670_WriteReg(0xc8, 0x30);
	OV7670_WriteReg(0x79, 0x26); 
	OV7670_WriteReg(0x09, 0x00);
}
/*
  * @brief	寄存器初始化
*/
void OV7670_Configure(void)
{
	OLED_ShowString(1,1,"Init...");
	OV7670_WriteReg(0x12,0x80);//寄存器复位,所有寄存器复位为初始默认值
	Delay_s(3);
	OLED_Clear();
	
	OV7670_RegExample();/* 参照其他教程的寄存器设置，具体寄存器对应的功能暂未弄清 */

	OV7670_WriteReg(0x11, 0x9F);/* 输入时钟32分频 */
	OV7670_WriteReg(0x6b, 0x00);/* 设置输入时钟倍频为1 */
	/* PCLK频率：F=4MHz/32*1=0.125MHz */
	/* 经初步测试，PCLK<=0.8MHz时能够正常获取图像数据不丢失 */
	
	/*设置测试图案输出  这里设置的是输出八色彩条*/
	OV7670_WriteReg(0x70, 0x3A);
	OV7670_WriteReg(0x71, 0xB5);
	/*应当是数据手册有误，数据手册里写的是：
	(0x70[7],0x71[7])=(1,0)输出的是八色彩条		×
	(0x70[7],0x71[7])=(0,1)输出的是Shifting “1”	×
	而实际上是反过来的
	(0x70[7],0x71[7])=(1,0)输出的是Shifting “1”	✓
	(0x70[7],0x71[7])=(0,1)输出的是八色彩条		✓
	*/
}
/*
  * @brief	OV7670初始化
  * @param  无
  * @retval 无
*/
void OV7670_Init(void)
{
	SCCB_Init();//SCCB初始化
	OV7670_Pin_Init();//引脚初始化
	OV7670_Configure();//寄存器预设
}
/*
  * @brief	读取图像信息并发送至电脑显示
  * @param  无
  * @retval 无
*/
uint8_t frame[320*40];
void OV7670_GetPic(void)
{
	uint16_t i,j;
	OLED_ShowString(2,1,"GetPics...");
	
	while(OV7670_VS()==0);/* 保证进入一个新的帧时序，而不是在帧时序的一半进入 */
	while(OV7670_VS()==1);
	for(i=0;i<240;i++)
	{		
		while(OV7670_HREF()==0);
		for(j=0;j<320*2;j++)
		{
			while(OV7670_PCLK()==0);
			frame[640*i+j]=GPIOA->IDR&0xFF;
			while(OV7670_PCLK()==1);
		}
		if(i==19) break;
		while(OV7670_HREF()==1);
	}
	while(OV7670_VS()==0);
	
	OLED_ShowString(2,1,"Sending... ");
	Serial_SendByte(0x01);
	Serial_SendByte(0xFE);
	for(i=0;i<320*40;i++)
	{
		Serial_SendByte(frame[i]);
	}
	Serial_SendByte(0xFE);
	Serial_SendByte(0x01);
	
	OLED_ShowString(4,1,"SUCCESS  ");
	Delay_ms(1000);
}
