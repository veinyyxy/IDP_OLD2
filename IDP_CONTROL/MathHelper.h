/*============================================================ 
项目名： 人机交互平台
作 者： 任雪
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
功能说明：数学辅助算法
调用说明：

-------------------------------------版本更新------------------------------------------------ 
V 1.2 
作 者：输入作者（或修改者）名字
完成日期：
更新说明： 
V 1.1 
作 者：输入作者（或修改者）名字
完成日期：
更新说明： 
V 1.0 
原作者 ：任雪
完成日期：2011年03月02日
============================================================*/

#pragma once
#include <osg/Vec3>
#include <stdlib.h>
#include <math.h>

class CMathHelper
{
public:
	CMathHelper(void);
	~CMathHelper(void);
	void myRoundf(float &Data);//用四舍五入法去掉小数点后的尾数
	int getValueOfquadraticEquation(osg::Vec3& equation, float *evalue1, float *evalue2);//获得一元二次方程的根,有几个解返回几，并在evalue中填入相应的值  
	// getValueOfquadraticEquation的delta做的特殊处理，即使delta小于零，也要将其进行取绝对值的处理，确保能够得到两个根
};