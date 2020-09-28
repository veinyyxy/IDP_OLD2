/*============================================================ 
项目名： 人机交互平台
单 位： 中国软件与技术服务股份有限公司
最后修改时间： 2011年？月？日
版权声明： 版权所有2010-2018 中软气象事业部
保留所有权利
Copyright (c) 2010-2018  CHINA NATIONAL SOFTWARE &SERVICE CO.,LTD.
All rights reserved 
重要声明： 1. 类库使用范围为气象事业部软件开发组成员，未经许可不得擅自传播。
           2. 类库使用者不得修改本文件内容，否则会导致类库使用错误。
------------------------------------------------------------------------------------- 
文件名：MathHelper.h MathHelper.cpp 
类 名： CMathHelper
父 类： 无
子 类： 无 
功能说明：辅助数学算法
调用说明：

-------------------------------------版本更新------------------------------------------------ 
V 1.1 
作 者：输入作者（或修改者）名字
完成日期：
更新说明： 
V 1.0 
原作者 ：renxue
完成日期：2011年02月01日
============================================================*/

#include "MathHelper.h"

CMathHelper::CMathHelper(void)
{}

CMathHelper::~CMathHelper(void)
{}

void CMathHelper::myRoundf(float &Data)//用四舍五入法去掉小数点后的尾数
{
	int temp;
	if(Data < 1e-6) 
	{
		temp =(int)(abs(Data)+0.5);
		Data = temp *(-1.0);
	}
	else
	{
		temp = (int)(Data + 0.5);
		Data = temp * 1.0;
	}
}

/************************************************************************/
/* 函数功能： 获得一元二次方程的根     							    	*/
/* 参数说明:  equation是输入的一元二次方程的系数						*/
/*            evalue是方程的根			            					*/
/*            int 返回的解的个数		            					*/
/* 返 回 值:  double p到line线的距离             						*/
/* 作    者:  renxue					    							*/
/* 日    期:  2011 年 04 月 11 日										*/
/************************************************************************/
int CMathHelper::getValueOfquadraticEquation(osg::Vec3 &equation, float *evalue1, float *evalue2)
{// delta做的特殊处理，即使delta小于零，也要将其进行取绝对值的处理，确保能够得到两个根
	double delta = equation.y()*equation.y() - 4*equation.x()*equation.z();
	
	if (delta > 1e-6)
	{
		delta = sqrt(delta);
		*evalue1 =  (delta-equation.y())/(2*equation.x());
		*evalue2 = -(delta+equation.y())/(2*equation.x());		
		return 2;
	} 
	else 
	{
		*evalue1 = *evalue2 = -equation.y()/(2*equation.x());
		return 1;
	}
}