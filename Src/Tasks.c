/*
  微型四轴源码版权归西电航协研发部门团队所有，未经本团队同意，请勿随意在网上传播本源码。
	与本软件相关参考资料西电航协微型四轴开发指南，内容对本套包含的所有软件以及硬件相关都做了详细的讲解
  如果有同学做了各种有意义的改进或有任何建议，请随时与我们保持联系。
	作者：PhillWeston
	联系邮箱：2436559745@qq.com
*/
#include "IMU.h"
#include "Tasks.h"
#include "Maths.h"
#include "Filter.h"
#include "Control.h"
#include "Advanced.h"
#include "struct_all.h"

uint8_t Bsp_Int_Ok = 0;

/******************************************************************************
函数原型：	void BSP_Int(void)
功    能：	硬件驱动初始化
*******************************************************************************/ 
void BSP_Int(void)
{
	LED_ON_OFF();//LED闪烁
	TIM_Init();//TIM1四个通道的PWM初始化
	ADC1_Init();//ADC及DMA初始化
	//这里加入蓝牙串口通讯
	
	while( InitMPU6050()!=1 );//若MPU6050初始化不成功，则程序不向下运行
	Calculate_FilteringCoefficient(0.001f,10.f);//计算IIR滤波器参数

	HAL_FLASH_Unlock();//Flash解锁
	EEPROM_INIT();//初始化模拟的EEPROM，读取参数

	Bsp_Int_Ok = 1;
}

/******************************************************************************
函数原型：	void Task_1000HZ(void)
功    能：	主循环中运行频率为1000HZ任务
*******************************************************************************/ 
void Task_1000HZ(void)
{
	if( MPU6050_SequenceRead()==0 )//若连续读取6050数据寄存器失败
	{
		I2C_Erro++;//统计I2C出错次数
		MPU6050_SingleRead();//分次读MPU6050数据寄存器
	}
	MPU6050_Compose();//6050数据合成
	ACC_IIR_Filter(&acc,&filter_acc);//对acc做IIR滤波
	Gyro_Filter(&gyro,&filter_gyro);//对gyro做窗口滤波
	Get_Radian(&filter_gyro,&SI_gyro);//角速度数据转为弧度
	IMUupdate(SI_gyro.x,SI_gyro.y,SI_gyro.z,filter_acc.x,filter_acc.y,filter_acc.z);//姿态解算
	Bluetooth_Connect();//蓝牙连接函数
}

/******************************************************************************
函数原型：	void Task_500HZ(void)
功    能：	主循环中运行频率为500HZ任务
*******************************************************************************/ 
void Task_500HZ(void)
{
	Control_Gyro(&SI_gyro,&Rc,Rc_Lock);//内环控制
}

/******************************************************************************
函数原型：	void Task_250HZ(void)
功    能：	主循环中运行频率为250HZ任务
*******************************************************************************/ 
void Task_250HZ(void)
{
	Get_Eulerian_Angle(&out_angle);//四元数转欧拉角
	Control_Angle(&out_angle,&Rc);//外环控制
}

/******************************************************************************
函数原型：	void Task_10HZ(void)
功    能：	主循环中运行频率为10HZ任务
*******************************************************************************/ 
void Task_10HZ(void)
{
	FailSafe();//异常保护
}
