## Overview

This is the source code of flight control terminal for XDU Aero Association micro quadcopter.

This is the [Keil5](http://www.keil.com/) project.So you could import to Keil5 and run from source code easily.

The main control chip is stm32g030, and the Bluetooth module is JDY-32

At present, the project is in the stage of to be completed. Non producers should not clone the source code of the master branch.

When modifying the code, please create a new feature branch under the master branch, modify the code under the feature branch, and then perform the pull request operation through the branch. Confirm and merge to the main branch.

For bug related and suggestions, please move to the issue column

By phillweston, Hangning Zhang, Dian Zhao

## 概要说明

这是西电航协微型四轴飞行器飞控端的源代码

这是一个[Keil5](http://www.keil.com/)工程，直接导入到Keil5中就可以编译运行。

主控芯片为STM32G030，蓝牙模块为JDY-32

该项目目前出于待完成阶段，非制作人员勿克隆master分支源码

修改代码的时候，请在master分支下面新建一个特性分支，在该特性分支下进行代码的修改然后通过该分支执行pull request操作。确认无误后再合并至主分支。

bug相关和建议请移步至issue栏目

## 四轴功能

该四轴目前基本功能已实现，还差自动降落的部分，以及定高抛飞的部分。

串口接收部分写在主循环中频率为1000Hx的Task函数中，串口内容解析在Connect函数中调用。

## 作者名单

作者：Phillweston, Hangning Zhang, Dian Zhao

## 微型四轴飞行器外形:

         HEAD
	  M4  ↑  M1

	   \     /

	    \   /

	     \ /

	     / \

	    /   \

	   /     \

	  M3     M2
	
## 硬件资源:

![image](https://github.com/uav-operation-system/Drone_Master/raw/master/chip.png)

	1.MCU:STM32G030K8T6 (FLASH:512K, RAM:128K, 系统运行时钟频率:96MHz)

	2.6轴MPU6050连接在IIC1上(IMU_SCL:PB8, IMU_SDA:PB7, 通信方式:模拟IIC)

	3.蓝牙无线通信NFR51822连接在UART1上(RXD:PA3, TXD:PA2, NRF_FLOW_CTRL:PA4)

	4.MOTOR1连接在TIM4_CH2上(PB6)

	5.MOTOR2连接在TIM4_CH1上(PB3)

	6.MOTOR3连接在TIM2_CH3上(PA9/PA11)

	7.MOTOR4连接在TIM2_CH1上(PB8)

	8.MOTOR1对应的绿色LED，连接在PB5上，高电平有效

	9.MOTOR2对应的绿色LED，连接在PB4上，高电平有效

	10.MOTOR3对应的绿色LED，连接在PC8上，高电平有效

	11.MOTOR4对应的绿色LED，连接在PB9上，高电平有效
