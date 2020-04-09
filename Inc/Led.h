#ifndef _LED_H_
#define _LED_H_
#include "stm32g0xx_hal.h"
/******************************************************************************
							�궨��
*******************************************************************************/ 
//#define LED2_GPIO	GPIOB
//#define LED2_Pin	GPIO_Pin_3
//#define LED3_GPIO	GPIOC
//#define LED3_Pin	GPIO_Pin_13

//#define LED2_ON 	LED2_GPIO->BRR  = LED2_Pin //��Դ�Ե�ָʾ��
//#define LED2_OFF  	LED2_GPIO->BSRR = LED2_Pin
//#define LED3_ON  	LED3_GPIO->BSRR = LED3_Pin//������Χ��ҹ���	
//#define LED3_OFF 	LED3_GPIO->BRR  = LED3_Pin		

/******************************************************************************
							ȫ�ֺ�������
*******************************************************************************/ 
void Delay_led(uint16_t n);
void LED_ON_OFF(void);
void LED_ON_Frequency(uint8_t Light_Frequency);
void LED_ON(void);
void LED_OFF(void);
#endif
