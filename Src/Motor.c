/*
  微型四轴源码版权归西电航协研发部门团队所有，未经本团队同意，请勿随意在网上传播本源码。
	与本软件相关参考资料西电航协微型四轴开发指南，内容对本套包含的所有软件以及硬件相关都做了详细的讲解
  如果有同学做了各种有意义的改进或有任何建议，请随时与我们保持联系。
	作者：PhillWeston
	联系邮箱：2436559745@qq.com
*/
#include "struct_all.h"

/******************************************************************************
函数原型：	static void Tim2_init(void)
功    能：	tim2初始化
*******************************************************************************/ 
//PWM频率 = 54000000 / 3 / 1000 = 18Khz
//PWM计数上限	
//设置用来作为TIM2时钟频率除数的预分频值，3分频
//设置时钟分割:TDTS = Tck_tim
//TIMx向上计数模式
/*static void Tim2_init(void)
{
	TIM_TimeBaseInitTypeDef		TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  			TIM_OCInitStructure;
	
 	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	
	//PWM频率 = 72000000 / 4 / 1000 = 18Khz
	TIM_TimeBaseStructure.TIM_Period = 1000 - 1; //PWM计数上限	 
	TIM_TimeBaseStructure.TIM_Prescaler = 4 - 1; //设置用来作为TIM2时钟频率除数的预分频值，4分频
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //TIMx向上计数模式
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); //根据TIM_TimeBaseStructure中指定的参数初始化外设TIM2
	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //选择定时器模式
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure.TIM_Pulse = 0; //设置待装入捕获比较寄存器的脉冲值
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //输出极性:TIM输出比较极性高
	
	TIM_OC1Init(TIM2, &TIM_OCInitStructure); 
	TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Enable); //使能TIM2在CCR1上的预装载寄存器
	TIM_OC2Init(TIM2, &TIM_OCInitStructure);
	TIM_OC2PreloadConfig(TIM2, TIM_OCPreload_Enable); //使能TIM2在CCR2上的预装载寄存器
	TIM_OC3Init(TIM2, &TIM_OCInitStructure);
	TIM_OC3PreloadConfig(TIM2, TIM_OCPreload_Enable); //使能TIM2在CCR3上的预装载寄存器
	TIM_OC4Init(TIM2, &TIM_OCInitStructure);
	TIM_OC4PreloadConfig(TIM2, TIM_OCPreload_Enable); //使能TIM2在CCR4上的预装载寄存器
	
	TIM_ARRPreloadConfig(TIM2, ENABLE); //使能TIM2在ARR上的预装载寄存器
	TIM_Cmd(TIM2, ENABLE);  //使能TIM2外设
}*/

/******************************************************************************
函数原型：	void Motor_Out(int16_t duty1,int16_t duty2,int16_t duty3,int16_t duty4)
功    能：	电机驱动
*******************************************************************************/ 
void Motor_Out(int16_t duty1,int16_t duty2,int16_t duty3,int16_t duty4)
{
	if(duty1>1000)	duty1=1000;
	if(duty1<0)		duty1=0;
	if(duty2>1000)	duty2=1000;
	if(duty2<0)		duty2=0;
	if(duty3>1000)	duty3=1000;
	if(duty3<0)		duty3=0;
	if(duty4>1000)	duty4=1000;
	if(duty4<0)		duty4=0;
	
	TIM1->CCR1 = duty1;
	TIM1->CCR2 = duty2;
	TIM1->CCR3 = duty3;
	TIM1->CCR4 = duty4;
}
