/*
  微型四轴源码版权归西电航协研发部门团队所有，未经本团队同意，请勿随意在网上传播本源码。
	与本软件相关参考资料西电航协微型四轴开发指南，内容对本套包含的所有软件以及硬件相关都做了详细的讲解
  如果有同学做了各种有意义的改进或有任何建议，请随时与我们保持联系。
	作者：PhillWeston
	联系邮箱：2436559745@qq.com
*/
#include "Maths.h"
#include "math.h"
#include "Advanced.h"
#include "Control.h"
#include "struct_all.h"

uint32_t newTime=0;
/******************************************************************************
函数原型：	void FailSafe(void)
功    能：	失效保护函数
					失效保护的情景有：侧翻
*******************************************************************************/ 
void FailSafe(void)
{
	//飞机侧翻，检测到过大的pitch或者roll角度，关闭电机，防止电机堵转烧坏
	if(fabs(out_angle.pitch)>80 || fabs(out_angle.roll)>80)
	{
		Motor_Out(0, 0, 0, 0);
		Rc_Lock = 1;	//飞机上锁
	}
	
}
