/*
  微型四轴源码版权归西电航协研发部门团队所有，未经本团队同意，请勿随意在网上传播本源码。
	与本软件相关参考资料西电航协微型四轴开发指南，内容对本套包含的所有软件以及硬件相关都做了详细的讲解
  如果有同学做了各种有意义的改进或有任何建议，请随时与我们保持联系。
	作者：PhillWeston
	联系邮箱：2436559745@qq.com
*/
#include "Maths.h"
#include "struct_all.h"

/******************************************************************************
函数原型：	void Print_MSP_RAW_IMU(void)
功    能：	发送传感器原始数据
*******************************************************************************/ 
void Print_MSP_RAW_IMU(void)
{
	uint8_t	data[32];
	int16_t send_data;
	
	data[0] = '$';
	data[1] = 'M';
	data[2] = '>';//发给上位机
	data[3] = 18;
	data[4] = MSP_RAW_IMU;
	
	send_data = (int16_t)(acc.x/4);
	data[5] =  send_data & 0xFF ;
	data[6] = (send_data >> 8) & 0xFF;
	send_data = (int16_t)(acc.y/4);
	data[7] =  send_data & 0xFF ;
	data[8] = (send_data >> 8) & 0xFF;
	send_data = (int16_t)(acc.z/4);
	data[9] =  send_data & 0xFF ;
	data[10] = (send_data >> 8) & 0xFF;

	send_data = (int16_t)(gyro.y);
	data[11] =  send_data & 0xFF ;
	data[12] = (send_data >> 8) & 0xFF;
	send_data = (int16_t)(gyro.x);
	data[13] =  send_data & 0xFF ;
	data[14] = (send_data >> 8) & 0xFF;
	send_data = (int16_t)(-gyro.z);
	data[15] =  send_data & 0xFF ;
	data[16] = (send_data >> 8) & 0xFF;

	send_data = (int16_t)(filter_acc.x*0.75f);
	data[17] =  send_data & 0xFF ;
	data[18] = (send_data >> 8) & 0xFF;
	send_data = (int16_t)(filter_acc.y*0.75f);
	data[19] =  send_data & 0xFF ;
	data[20] = (send_data >> 8) & 0xFF;
	send_data = (int16_t)(filter_acc.z*0.75f);
	data[21] =  send_data & 0xFF ;
	data[22] = (send_data >> 8) & 0xFF;
	
	data[23] = Get_Checksum(data);
	
	Bluetooth_Send_TX(data, 32);
}

/******************************************************************************
函数原型：	void Print_MSP_ATTITUDE(void)
功    能：	发送姿态数据
*******************************************************************************/ 
void Print_MSP_ATTITUDE(void)
{
	uint8_t	data[32];
	int16_t send_data;

	data[0] = '$';
	data[1] = 'M';
	data[2] = '>';//发给上位机
	data[3] = 6;
	data[4] = MSP_ATTITUDE;
	//data[5]和data[6]合在一起发送十六位的姿态解算-roll值
	send_data = (int16_t)(out_angle.roll * 10);
	data[5] =  send_data & 0xFF ;
	data[6] = (send_data >> 8) & 0xFF;
	//data[7]和data[8]合在一起发送十六位的姿态解算-pitch值
	send_data = (int16_t)(out_angle.pitch * 10);
	data[7] =  send_data & 0xFF ;
	data[8] = (send_data >> 8) & 0xFF;
	//data[9]和data[10]合在一起发送十六位的姿态解算-yaw值，注意这里是负号
	send_data = (int16_t)(-out_angle.yaw);
	data[9] =  send_data & 0xFF ;
	data[10] = (send_data >> 8) & 0xFF;
	
	data[11] = Get_Checksum(data);
	
	Bluetooth_Send_TX(data, 32);
}

/******************************************************************************
函数原型：	void Print_MSP_FLY_DATA(void)
功    能：	发送飞控数据（自定义通讯）
*******************************************************************************/ 
void Print_MSP_FLY_DATA(void)
{
	uint8_t	data[32];
	int16_t send_data;

	data[0] = '$';
	data[1] = 'M';
	data[2] = '>';//发给遥控器
	data[3] = 23;
	data[4] = MSP_FLY_DATA;
	//data[5]和data[6]合在一起发送十六位的电池电压值
	send_data = Battery_Fly;//电池电压值的100倍
	data[5] =  send_data & 0xFF ;
	data[6] = (send_data >> 8) & 0xFF;
	//data[7]和data[8]合在一起发送十六位的MOS1的PWM值
	data[7] =   (TIM1->CCR1+1000) & 0xFF;
	data[8] =  ((TIM1->CCR1+1000) >> 8) & 0xFF;
	//data[9]和data[10]合在一起发送十六位的MOS2的PWM值
	data[9] =   (TIM1->CCR2+1000) & 0xFF;
	data[10] =  ((TIM1->CCR2+1000) >> 8) & 0xFF;
	//data[11]和data[12]合在一起发送十六位的MOS3的PWM值
	data[11] =    (TIM1->CCR3+1000) & 0xFF;
	data[12] =  ((TIM1->CCR3+1000) >> 8) & 0xFF;
	//data[12]和data[14]合在一起发送十六位的MOS4的PWM值
	data[13] =   (TIM1->CCR4+1000) & 0xFF;
	data[14] =  ((TIM1->CCR4+1000) >> 8) & 0xFF;
	//发送pid解算后的roll结构体的kp,ki,kd值
	data[15] = (uint8_t)(roll.kp*10);
	data[16] = (uint8_t)(roll.ki*1000);
	data[17] = (uint8_t)(roll.kd);
	//发送陀螺仪解算后的roll结构体的kp,ki,kd值
	data[18]  = (uint8_t)(gyro_roll.kp*10);
	data[19]  = (uint8_t)(gyro_roll.ki*1000);
	data[20] = (uint8_t)(gyro_roll.kd);
	//发送陀螺仪解算后的yaw结构体的kp,ki,kd值
	data[21] = (uint8_t)(gyro_yaw.kp*10);
	data[22] = (uint8_t)(gyro_yaw.ki*1000);
	data[23] = (uint8_t)(gyro_yaw.kd);
////////////////////////////////////////////////////
	data[24] =  I2C_Erro & 0xFF;
	data[25] = (I2C_Erro>>8) & 0xFF;
	data[26] = (I2C_Erro>>16) & 0xFF;
	data[27] = (I2C_Erro>>24) & 0xFF;
	
	data[28] = Get_Checksum(data);

	Bluetooth_Send_TX(data, 32);
}
