/*
  微型四轴源码版权归西电航协研发部门团队所有，未经本团队同意，请勿随意在网上传播本源码。
	与本软件相关参考资料西电航协微型四轴开发指南，内容对本套包含的所有软件以及硬件相关都做了详细的讲解
  如果有同学做了各种有意义的改进或有任何建议，请随时与我们保持联系。
	作者：PhillWeston
	联系邮箱：2436559745@qq.com
*/
#include "Control.h"
#include "Advanced.h"
#include "struct_all.h"

/******************************************************************************
							宏定义
*******************************************************************************/ 

/******************************************************************************
							变量定义
*******************************************************************************/ 
uint8_t Bluetooth_RXDATA[32];//Bluetooth接收到的数据
uint8_t Bluetooth_TXDATA[32];//Bluetooth需要发送的数据
//static uint8_t TX_ADDRESS[5]= {0x1A,0x3B,0x5C,0x7D,0x9E};//本地地址
//static uint8_t RX_ADDRESS[5]= {0x1A,0x3B,0x5C,0x7D,0x9E};//接收地址
static uint16_t Bluetooth_Erro;

/******************************************************************************
函数原型：	static void Bluetooth_Analyse(void)
功    能：	分析Bluetooth收到的数据帧
*******************************************************************************/
static void Bluetooth_Analyse(void)
{
	uint8_t sum = 0,i;
	uint8_t len = Bluetooth_RXDATA[3] + 5;
	for(i=3;i<len;i++)
		sum ^= Bluetooth_RXDATA[i];
	if( sum!=Bluetooth_RXDATA[len] )	return;	//数据校验
	if( Bluetooth_RXDATA[0] != '$' )	return;	//数据校验
	if( Bluetooth_RXDATA[1] != 'M' )	return;	//数据校验
	if( Bluetooth_RXDATA[2] != '<' )	return;	//上位机及遥控下发给飞控数据
	
	if( Bluetooth_RXDATA[4] == MSP_RC )//功能桢标志
	{
		Rc.ROLL 	= ( (uint16_t)(Bluetooth_RXDATA[6])  << 8 ) | Bluetooth_RXDATA[5];
		Rc.PITCH	= ( (uint16_t)(Bluetooth_RXDATA[8])  << 8 ) | Bluetooth_RXDATA[7];
		Rc.YAW 		= ( (uint16_t)(Bluetooth_RXDATA[10]) << 8 ) | Bluetooth_RXDATA[9];
		Rc.THROTTLE = ( (uint16_t)(Bluetooth_RXDATA[12]) << 8 ) | Bluetooth_RXDATA[11];

		Rc.AUX1 	= ( (uint16_t)(Bluetooth_RXDATA[14]) << 8 ) | Bluetooth_RXDATA[13];
		Rc.AUX2		= ( (uint16_t)(Bluetooth_RXDATA[16]) << 8 ) | Bluetooth_RXDATA[15];
		Rc.AUX3		= ( (uint16_t)(Bluetooth_RXDATA[18]) << 8 ) | Bluetooth_RXDATA[17];
		Rc.AUX4 	= ( (uint16_t)(Bluetooth_RXDATA[20]) << 8 ) | Bluetooth_RXDATA[19];	
		
		RC_Limit(&Rc);	
		RC_LOCK();
	}
	if( Bluetooth_RXDATA[4] == MSP_SET_PID )//功能桢标志
	{
		roll.kp  = (float)Bluetooth_RXDATA[5]/10.f;	
		roll.ki  = (float)Bluetooth_RXDATA[6]/1000.0f;
		roll.kd  = (float)Bluetooth_RXDATA[7];
		pitch.kp = roll.kp;
		pitch.ki = roll.ki;
		pitch.kd = roll.kd;

		gyro_roll.kp  = (float)Bluetooth_RXDATA[8]/10.f;
		gyro_roll.ki  = (float)Bluetooth_RXDATA[9]/1000.f;
		gyro_roll.kd  = (float)Bluetooth_RXDATA[10];
		gyro_pitch.kp = gyro_roll.kp;
		gyro_pitch.ki = gyro_roll.ki;
		gyro_pitch.kd = gyro_roll.kd;
		
		gyro_yaw.kp = (float)Bluetooth_RXDATA[11]/10.f;
		gyro_yaw.ki = (float)Bluetooth_RXDATA[12]/1000.f;
		gyro_yaw.kd = (float)Bluetooth_RXDATA[13];
		
		EEPROM_SAVE_PID();
	}
	if( Bluetooth_RXDATA[4] == MSP_ACC_CALIBRATION )//功能桢标志
	{
		Do_ACC_Offset();//校正加速度计
	}
	if( Bluetooth_RXDATA[4] == MSP_MAG_CALIBRATION )//功能桢标志
	{
		Do_GYRO_Offset();//校正陀螺仪
	}
	if( Bluetooth_RXDATA[4] == MSP_RESET_CONF )//功能桢标志
	{
		PID_Reset();//重置PID参数
	}
}

/******************************************************************************
函数原型：	void Send_Data_Back(void)
功    能：	向遥控发送数据
*******************************************************************************/
void Send_Data_Back(void)
{
	static uint8_t turn_count;
	turn_count++;
	switch(turn_count)
	{
		case 1: Print_MSP_RAW_IMU();	break;
		case 2: Print_MSP_ATTITUDE();	break;
		case 3: Print_MSP_FLY_DATA();	
				turn_count=0;			break;
		default:turn_count=0;
	}
}

/******************************************************************************
函数原型：	void Bluetooth_Send_TX(uint8_t * tx_buf, uint8_t len)
功    能：	Bluetooth发送数据包
*******************************************************************************/
void Bluetooth_Send_TX(uint8_t * tx_buf, uint8_t len)
{
	HAL_UART_Transmit(&huart2, tx_buf, sizeof(tx_buf), 0xffff);
}

/******************************************************************************
函数原型：	void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
功    能：	UART2中断
*******************************************************************************/
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	//HAL_UART_Transmit(&huart2, Bluetooth_RXDATA, 1, 100);    // 把收到的字节原样发送出去(调试的时候可能需要)
	HAL_UART_Receive_IT(&huart2, Bluetooth_RXDATA, 1);       // 重新注册一次，要不然下次收不到了
}

/******************************************************************************
函数原型：	void Bluetooth_Connect(void)
功    能：	Bluetooth连接函数
*******************************************************************************/
void Bluetooth_Connect(void)//1KHZ
{
	Bluetooth_Erro ++;
	if(Bluetooth_Erro==1)
	{
		Bluetooth_Analyse();//分析蓝牙串口收到的数据帧
		Send_Data_Back();  //向遥控发送数据
	}
	if(Bluetooth_Erro%100==0)  //0.1s未接收蓝牙数据 ，试图连接遥控
	{	
		//NRF24L01_IRQ();    //清除中断标志位
	}
	if(Bluetooth_Erro>=1000)   //1s未接收蓝牙数据 ,锁定四轴，电机停转，防止意外发生
	{
		Bluetooth_Erro = 1;
		//FailSafe();
		Rc_Lock=1;  //上锁
	}
	
	Battery_Fly=(int16_t)ADC_Value;
	if(Battery_Fly>330)
	{
		if(Rc.AUX4 & Led_Mode)  //夜间模式
			LED_ON();
		else
			LED_OFF();
	}
	else
	{
		LED_ON_Frequency(2);
	}
}
