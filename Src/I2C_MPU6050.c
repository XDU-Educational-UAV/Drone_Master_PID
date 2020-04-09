/*
  微型四轴源码版权归西电航协研发部门团队所有，未经本团队同意，请勿随意在网上传播本源码。
	与本软件相关参考资料西电航协微型四轴开发指南，内容对本套包含的所有软件以及硬件相关都做了详细的讲解
  如果有同学做了各种有意义的改进或有任何建议，请随时与我们保持联系。
	作者：PhillWeston
	联系邮箱：2436559745@qq.com
*/
#include "struct_all.h"

/******************************************************************************
							宏定义
*******************************************************************************/ 
/**I2C1 GPIO Configuration    
    PB7     ------> I2C1_SDA
    PB8     ------> I2C1_SCL 
    */

#define	SCL_H   	{GPIOB->BSRR = SCL_Pin;I2C_delay(4);}	 //SCL高电平
#define	SCL_L   	{GPIOB->BRR  = SCL_Pin;I2C_delay(4);}	 //SCL低电平
#define	SDA_H   	{GPIOB->BSRR = SDA_Pin;I2C_delay(4);}	 //SDA高电平
#define	SDA_L   	{GPIOB->BRR  = SDA_Pin;I2C_delay(4);}	 //SDA低电平
#define	SDA_Read	GPIOB->IDR  & SDA_Pin	 //SDA读数据

/******************************************************************************
							全局变量
*******************************************************************************/ 
uint8_t	GYRO_Offset = 0;//不自动校正
uint8_t	ACC_Offset  = 0;
uint32_t I2C_Erro=0;
/******************************************************************************
							局部变量
*******************************************************************************/ 
static uint8_t	MPU6050_Buffer[14];	//I2C读取数据缓存

/******************************************************************************
函数原型:	static void I2C_delay(int delay)
功　　能:	I2C总线延时
*******************************************************************************/ 
static void I2C_delay(int delay)
{
    while (delay)
        delay--;
}

/******************************************************************************
函数原型：	static uint8_t I2C_Start(void)	
功    能：	I2C总线开始信号
返 回 值：   1成功；0失败   
*******************************************************************************/ 
static uint8_t I2C_Start(void)	
{
	SCL_L;					
	SDA_H;
	SCL_H;
	
	if(!SDA_Read)
		return 0;	//SDA线为低电平则总线忙,退出
	SDA_L;
	if(SDA_Read) 
		return 0;	//SDA线为高电平则总线出错,退出
	
	SDA_L;
	return 1;
}

/******************************************************************************
函数原型：	static void I2C_Stop(void)	
功    能：	I2C总线停止信号
*******************************************************************************/ 
static void I2C_Stop(void)
{
	SCL_L;					
	SDA_L;
	SCL_H;
	SDA_H;
}

/******************************************************************************
函数原型：	static void I2C_ACK(void)	
功    能：	I2C总线Master发送ACK信号
*******************************************************************************/ 
static void I2C_ACK(void)
{
	SCL_L; 	
	__nop();
	SDA_L;	//写应答信号
	__nop();
    SCL_H; 
	__nop();
    SCL_L;      
}

/******************************************************************************
函数原型：	static void I2C_NACK(void)	
功    能：	I2C总线Master发送NACK信号
*******************************************************************************/ 
static void I2C_NACK(void)
{
	SCL_L; 		
//	__nop();
	SDA_H;	//不写应答信号
//	__nop();
    SCL_H; 
//	__nop();
    SCL_L;  
}

/******************************************************************************
函数原型：	static uint8_t I2C_WaitAck(void)	
功    能：	I2C总线Master接收Slave应答
返 回 值：	1有收到ACK；0收到NACK  
*******************************************************************************/ 
static uint8_t I2C_WaitAck(void)
{
	SCL_L;				
	SDA_H;
	SCL_H;
		
	if(SDA_Read)
	{
		SCL_L;
		return 0;	
	}
	
	SCL_L;
	return 1;	  
}
/******************************************************************************
函数原型：	static void I2C_WriteByte(unsigned char SendByte) 	
功    能：	I2C写一个字节数据,先写高位
*******************************************************************************/ 
static void I2C_WriteByte(unsigned char SendByte) 
{
	uint8_t i=8; 
	
	while(i--)
	{
		SCL_L;
		if( SendByte&0x80 ) 
			SDA_H
		else 
			SDA_L
		SendByte <<= 1;
		SCL_H;
	}   
	SCL_L;  
}

/******************************************************************************
函数原型：	static uint8_t I2C_ReadByte(void)	
功    能：	I2C读一个字节数据,先读高位
返 回 值：	返回读出的一个字节数据
*******************************************************************************/ 
static uint8_t I2C_ReadByte(void)
{
	uint8_t i=8;
	uint8_t ReceiveByte =0;
	
	SDA_H;
	while(i--)
	{
		SCL_L;  
		__nop();
		SCL_H; 		
		ReceiveByte <<= 1;
		if(SDA_Read) 
			ReceiveByte |= 0x01;
	}
	SCL_L;
	return ReceiveByte;
}

/******************************************************************************
函数原型：	static uint8_t Single_WriteI2C(unsigned char Regs_Addr,unsigned char Regs_Data) 	
功    能：	写MPU6050寄存器
返 回 值：	0出错；1成功
*******************************************************************************/ 
static uint8_t Single_WriteI2C(unsigned char Regs_Addr,unsigned char Regs_Data) 
{  
	if( !I2C_Start() )
		return 0;	//I2C起始错误，返回
	I2C_WriteByte(MPU6050Address); //写Slave地址，并配置成写模式
	if( !I2C_WaitAck() )
	{
		I2C_Stop(); 
		return 0;	//无ACK，返回
	}
	I2C_WriteByte(Regs_Addr);    //写寄存器地址
	I2C_WaitAck();
	I2C_WriteByte(Regs_Data);    //写寄存器数据
	I2C_WaitAck();
	I2C_Stop();         
	return 1;
}
/******************************************************************************
函数原型：	static uint8_t Single_ReadI2C(unsigned char Regs_Addr) 	
功    能：	读MPU6050单字节
返 回 值：	寄存器值
*******************************************************************************/ 
static uint8_t Single_ReadI2C(unsigned char Regs_Addr) 
{
	uint8_t ret;

	if( !I2C_Start() )
		return 0;	//I2C起始错误，返回
	I2C_WriteByte(MPU6050Address); 	//写Slave地址，并配置成写模式
	if( !I2C_WaitAck() )
	{
		I2C_Stop(); 
		return 0;	//无ACK，返回
	}
	
	I2C_WriteByte(Regs_Addr);    	//写寄存器地址
	I2C_WaitAck();
	I2C_Start();
	I2C_WriteByte(MPU6050Address+1);//写Slave地址，并配置成读模式
	I2C_WaitAck();
	
	ret=I2C_ReadByte();				//从传感器中读出数据

	I2C_NACK();						//无应答
	I2C_Stop();                  	//结束本段IIC进程 
	return ret;	
} 

/******************************************************************************
函数原型：	uint8_t MPU6050_SequenceRead(void) 	
功    能：	连续读MPU6050数据寄存器
返 回 值：	0出错；1成功
*******************************************************************************/ 
uint8_t MPU6050_SequenceRead(void)
{
	uint8_t index=14;
	
	if( !I2C_Start() )
		return 0;	//I2C起始错误，返回
	I2C_WriteByte(MPU6050Address); 	//写Slave地址，并配置成写模式
	if( !I2C_WaitAck() )
	{
		I2C_Stop(); 
		return 0;	//无ACK，返回
	}
	I2C_WriteByte(ACCEL_XOUT_H);    //写寄存器地址
	I2C_WaitAck();
	
	I2C_Start();
	I2C_WriteByte(MPU6050Address+1);//写Slave地址，并配置成读模式
	I2C_WaitAck();

	while(--index)	//连读13位寄存器
	{
		MPU6050_Buffer[13 - index] = I2C_ReadByte();
		I2C_ACK();
	}

	MPU6050_Buffer[13] = I2C_ReadByte();	//读第14寄存器
	I2C_NACK();	
	I2C_Stop();	
	
	return 1;
}
/******************************************************************************
函数原型：	void MPU6050_SingleRead(void)
功    能：	分次读MPU6050数据寄存器
*******************************************************************************/ 
void MPU6050_SingleRead(void)
{
		MPU6050_Buffer[0] = Single_ReadI2C(ACCEL_XOUT_H);
		MPU6050_Buffer[1] = Single_ReadI2C(ACCEL_XOUT_L);		
		MPU6050_Buffer[2] = Single_ReadI2C(ACCEL_YOUT_H);	
		MPU6050_Buffer[3] = Single_ReadI2C(ACCEL_YOUT_L);
		MPU6050_Buffer[4] = Single_ReadI2C(ACCEL_ZOUT_H);
		MPU6050_Buffer[5] = Single_ReadI2C(ACCEL_ZOUT_L);
		
		MPU6050_Buffer[8]  = Single_ReadI2C(GYRO_XOUT_H);
		MPU6050_Buffer[9]  = Single_ReadI2C(GYRO_XOUT_L);
		MPU6050_Buffer[10] = Single_ReadI2C(GYRO_YOUT_H);
		MPU6050_Buffer[11] = Single_ReadI2C(GYRO_YOUT_L);
		MPU6050_Buffer[12] = Single_ReadI2C(GYRO_ZOUT_H);
		MPU6050_Buffer[13] = Single_ReadI2C(GYRO_ZOUT_L);
}
/******************************************************************************
函数原型：	uint8_t MPU6050_SingleRead(void)
功    能：	初始化MPU6050
返 回 值：	0出错；1成功
*******************************************************************************/ 
uint8_t InitMPU6050(void)
{
	if( Single_ReadI2C(WHO_AM_I) != 0x68)//检查MPU6050是否正常
	{
		return 0;
	}
	
	//PrintString("\r\n MPU6050  正常！");
	Single_WriteI2C(PWR_MGMT_1, 0x00);	//电源管理，典型值：0x00，正常模式
	I2C_delay(20000); //约2.5ms延时
	Single_WriteI2C(SMPLRT_DIV, 0x00);	//陀螺仪采样率，典型值：0x00，不分频（8KHZ）
	I2C_delay(20000);            
	Single_WriteI2C(CONFIG2, 0x00);   	//低通滤波频率，典型值：0x00，不启用MPU6050自带滤波
	I2C_delay(20000);
	Single_WriteI2C(GYRO_CONFIG, 0x18);	//陀螺仪自检及测量范围，典型值：0x18(不自检，2000deg/s)
	I2C_delay(20000);
	Single_WriteI2C(ACCEL_CONFIG, 0x1F);//加速计自检、测量范围及高通滤波频率，典型值：0x1F(不自检，16G)
	I2C_delay(20000);
	
	//PrintString("\r\n MPU6050  初始化完成！");
	return 1;
}
/******************************************************************************
函数原型：	void Do_ACC_Offset(void)
功    能：	MPU6050加速度零偏校正
*******************************************************************************/ 
void Do_ACC_Offset(void)
{
	ACC_Offset=1;
}

/******************************************************************************
函数原型：	void Do_GYRO_Offset(void)
功    能：	MPU6050角速度零偏校正
*******************************************************************************/ 
void Do_GYRO_Offset(void)
{
	GYRO_Offset=1;
}

/******************************************************************************
函数原型：	void MPU6050_Offset(void)
功    能：	MPU6050零偏校正
*******************************************************************************/ 
void MPU6050_Offset(void)
{
	if(ACC_Offset)
	{
		static int32_t ACC_X=0,ACC_Y=0,ACC_Z=0;
		static uint8_t count_acc=0;
		if(count_acc==0)
		{
			offset_acc.x = 0;
			offset_acc.y = 0;
			offset_acc.z = 0;
			ACC_X = 0;
			ACC_Y = 0;
			ACC_Z = 0;
			count_acc = 1;
			return;
		}
		else
		{
			count_acc++;
			ACC_X += acc.x;
			ACC_Y += acc.y;
			ACC_Z += acc.z;
		}
		if(count_acc==251)
		{
			count_acc--;
			offset_acc.x = ACC_X / count_acc;
			offset_acc.y = ACC_Y / count_acc;
			offset_acc.z = ACC_Z / count_acc - 2048;
			count_acc = 0;
			ACC_Offset = 0;
			EEPROM_SAVE_ACC_OFFSET();
			
		}
	}
	
	if(GYRO_Offset)
	{
		static int32_t GYRO_X=0,GYRO_Y=0,GYRO_Z=0;
		static uint8_t count_gyro=0;
		if(count_gyro==0)
		{
			offset_gyro.x = 0;
			offset_gyro.y  = 0;
			offset_gyro.z   = 0;
			GYRO_X = 0;
			GYRO_Y = 0;
			GYRO_Z = 0;
			count_gyro = 1;
			return;
		}
		else
		{
			count_gyro++;
			GYRO_X += gyro.x;
			GYRO_Y += gyro.y;
			GYRO_Z += gyro.z;
		}
		if(count_gyro==251)
		{
			count_gyro--;
			offset_gyro.x = GYRO_X / count_gyro;
			offset_gyro.y = GYRO_Y / count_gyro;
			offset_gyro.z = GYRO_Z / count_gyro;
			count_gyro = 0;
			GYRO_Offset = 0;
			EEPROM_SAVE_GYRO_OFFSET();
		}
	}
}

/******************************************************************************
函数原型：	void MPU6050_Compose(void)
功    能：	合成MPU6050的16位数据
*******************************************************************************/ 
void MPU6050_Compose(void)
{
	acc.x  = ((((int16_t)MPU6050_Buffer[0]) << 8) | MPU6050_Buffer[1]) - offset_acc.x;	//减去零偏
	acc.y  = ((((int16_t)MPU6050_Buffer[2]) << 8) | MPU6050_Buffer[3]) - offset_acc.y;
	acc.z  = ((((int16_t)MPU6050_Buffer[4]) << 8) | MPU6050_Buffer[5]) - offset_acc.z;

	gyro.x = ((((int16_t)MPU6050_Buffer[8])  << 8) | MPU6050_Buffer[9])  - offset_gyro.x;
	gyro.y = ((((int16_t)MPU6050_Buffer[10]) << 8) | MPU6050_Buffer[11]) - offset_gyro.y;
	gyro.z = ((((int16_t)MPU6050_Buffer[12]) << 8) | MPU6050_Buffer[13]) - offset_gyro.z;
	
	MPU6050_Offset();
}

/******************************************************************************
函数原型：	void MPU6050_Printf(void)
功    能：	输出MPU6050的原始数据
*******************************************************************************/ 
/*
void MPU6050_Printf(void)
{
	//PrintString("\r\nMPU6050 acc:\r\n");
	PrintS16(acc.x);
	PrintS16(acc.y);
	PrintS16(acc.z);
	//PrintString("\r\nMPU6050 gyro\r\n");
	PrintS16(gyro.x);
	PrintS16(gyro.y);
	PrintS16(gyro.z);
}
*/
