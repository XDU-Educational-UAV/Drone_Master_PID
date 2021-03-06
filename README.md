# 西电航协微型四轴PID版本代码

[![Codacy Badge](https://api.codacy.com/project/badge/Grade/677411ebae7c4a25a40263f6fc33ee4c)](https://app.codacy.com/gh/xdu-aero-association/Drone_Master_PID?utm_source=github.com&utm_medium=referral&utm_content=xdu-aero-association/Drone_Master_PID&utm_campaign=Badge_Grade_Dashboard)

![logo](./logo.jpg)

## Status

![stars](https://img.shields.io/github/stars/xdu-aero-association/Drone_Master_PID.svg) ![forks](https://img.shields.io/github/forks/xdu-aero-association/Drone_Master_PID.svg) ![tag](https://img.shields.io/github/tag/xdu-aero-association/Drone_Master_PID.svg) ![release](https://img.shields.io/github/release/xdu-aero-association/Drone_Master_PID.svg) ![issues](https://img.shields.io/github/issues/xdu-aero-association/Drone_Master_PID.svg)

## Overview

This is the source code of flight control terminal for XDU Aero Association micro quadcopter.

This is the [Keil5](http://www.keil.com/) project.So you could import to Keil5 and run from source code easily.

The main control chip is stm32g030, and the Bluetooth module is JDY-32

At present, the project is in the stage of to be completed. Non producers should not clone the source code of the master branch.

When modifying the code, please create a new feature branch under the master branch, modify the code under the feature branch, and then perform the pull request operation through the branch. Confirm and merge to the main branch.

For bug related and suggestions, please move to the issue column

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

## 项目进度

|Tag|功能|完成日期|
|:-:|:-:|:-:|
|0.1.0|移植烈火四轴源码到CubeMX框架上|2020.02.28|
|0.1.1|代码编译调试，消除警告与错误|2020.03.31|
|0.1.2|实现与APP的通信|2020.05.01|
|0.1.3|添加额外功能（无头模式、故障保护）|待定|

## 微型四轴飞行器外形

         HEAD
	  M2  ↑  M3

	   \     /

	    \   /

	     \ /

	     / \

	    /   \

	   /     \

	  M1     M4
	
## 电机和桨安装顺序

请按照图示顺序正确安装飞控板方向以及电机线序：（注意：**安装错误的方向可能导致飞行器出现严重故障**）

![chip3d](./chip3d.png)

桨叶颜色与电机线序：

电机线有两种颜色：白黑和蓝红

|电机线颜色|红色桨叶|黑色桨叶|
|-|-|-|
|白黑|白正黑负|白负黑正|
|蓝红|蓝正红负|蓝负红正|

M1和M3为逆时针旋转，M2和M4为顺时针旋转

## 上锁和解锁方法

左手油门手的摇杆打向左下角，右手摇杆不动，上锁

左手油门手的摇杆打向右下角，右手摇杆不动，解锁

（Beta：未来可以使用APP上面的解锁/上锁按钮来进行解锁/上锁）

## 陀螺仪和加速度计校准方法

左手油门手的摇杆打向左下角，右手摇杆俯仰方向拉到最低，校正陀螺仪

左手油门手的摇杆打向左下角，右手摇杆俯仰方向拉到最低，校正加速度计

（Beta：未来可以使用APP上面的校正陀螺仪/加速度计按钮来进行校正陀螺仪/加速度计）

## 硬件资源

![chip](./chip.png)

### MCU和传感器模块接口

- MCU:STM32G030K8T6 (FLASH:512K, RAM:128K, 系统运行时钟频率:96MHz)

- 6轴MPU6050连接在IIC1上(IMU_SCL:PB8, IMU_SDA:PB7, 通信方式:模拟IIC)

- 蓝牙无线通信NFR51822连接在UART1上(RXD:PA3, TXD:PA2, NRF_FLOW_CTRL:PA4)

### 空心杯电机接口

- MOTOR1连接在TIM4_CH2上(PB6)

- MOTOR2连接在TIM4_CH1上(PB3)

- MOTOR3连接在TIM2_CH3上(PA9/PA11)

- MOTOR4连接在TIM2_CH1上(PB8)

### 空心杯电机与对应的LED接口

- MOTOR1对应的绿色LED，连接在PB5上，高电平有效

- MOTOR2对应的绿色LED，连接在PB4上，高电平有效

- MOTOR3对应的绿色LED，连接在PC8上，高电平有效

- MOTOR4对应的绿色LED，连接在PB9上，高电平有效

## 作者名单

作者：Phillweston, Hangning Zhang, Dian Zhao
