/*
  微型四轴源码版权归西电航协研发部门团队所有，未经本团队同意，请勿随意在网上传播本源码。
	与本软件相关参考资料西电航协微型四轴开发指南，内容对本套包含的所有软件以及硬件相关都做了详细的讲解
  如果有同学做了各种有意义的改进或有任何建议，请随时与我们保持联系。
	作者：PhillWeston
	联系邮箱：2436559745@qq.com
*/
#include "struct_all.h"

/******************************************************************************
函数原型：	static void Delay_led(u16 n)
功    能：	延时
*******************************************************************************/ 
void Delay_led(uint16_t n)
{	
	uint16_t i,j;
	for(i=0;i<n;i++)
		for(j=0;j<8500;j++);
}

/******************************************************************************
函数原型：	void LED_ON(void)
功    能：	四个红色LED点亮
*******************************************************************************/ 
void LED_ON(void)
{
	//GPIO_PIN_RESET相当于置低，由于LED设置为上拉，因此此处为亮灯
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, GPIO_PIN_RESET);
}
/******************************************************************************
函数原型：	void LED_ON_OFF(void)
功    能：	四个红色LED熄灭
*******************************************************************************/ 
void LED_OFF(void)
{
	//GPIO_PIN_SET相当于置高，由于LED设置为上拉，因此此处为灭灯
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, GPIO_PIN_SET);
}
/******************************************************************************
函数原型：	void LED_ON_OFF(void)
功    能：	LED开机闪烁
*******************************************************************************/ 
void LED_ON_OFF(void)
{
	uint8_t i ;
	for(i=0;i<3;i++)
	{
		LED_ON();
		Delay_led(200);
		LED_OFF();
		Delay_led(200);	
	}
	for(i=0;i<3;i++)
	{
		LED_ON();
		Delay_led(100);
		LED_OFF();
		Delay_led(100);	
	}
	Delay_led(100);
}
/******************************************************************************
函数原型：	void LED_ON_Frequency(uint8_t Light_Frequency)
功    能：	让LED3按照某个频率闪烁（需要循环扫描该函数）
参    数：   Light_Frequency，闪烁频率（HZ）
*******************************************************************************/ 
void LED_ON_Frequency(uint8_t Light_Frequency)
{
	uint16_t time_count;
	static uint8_t Light_On;
	static uint32_t Last_time,Now_time;
	
	Now_time = Timer3_Count;
	if( Light_On )
	{
		time_count = (uint16_t)(Timer3_Frequency / Light_Frequency / 2);	
		
		if( Now_time - Last_time >= time_count)
		{
			LED_OFF();
			Light_On=0;
			Last_time = Timer3_Count;
		}
	}
	else
	{
		time_count = (uint16_t)(Timer3_Frequency / Light_Frequency / 2);
		
		if( Now_time - Last_time >= time_count)
		{
			LED_ON();
			Light_On=1;
			Last_time = Timer3_Count;
		}
	}
}

