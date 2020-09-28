/*============================================================ 
��Ŀ���� �˻�����ƽ̨
�� λ�� �й�����뼼������ɷ����޹�˾
����޸�ʱ�䣺 2011�ꣿ�£���
��Ȩ������ ��Ȩ����2010-2018 ����������ҵ��
��������Ȩ��
Copyright (c) 2010-2018  CHINA NATIONAL SOFTWARE &SERVICE CO.,LTD.
All rights reserved 
��Ҫ������ 1. ���ʹ�÷�ΧΪ������ҵ������������Ա��δ����ɲ������Դ�����
           2. ���ʹ���߲����޸ı��ļ����ݣ�����ᵼ�����ʹ�ô���
------------------------------------------------------------------------------------- 
�ļ�����MathHelper.h MathHelper.cpp 
�� ���� CMathHelper
�� �ࣺ ��
�� �ࣺ �� 
����˵����������ѧ�㷨
����˵����

-------------------------------------�汾����------------------------------------------------ 
V 1.1 
�� �ߣ��������ߣ����޸��ߣ�����
������ڣ�
����˵���� 
V 1.0 
ԭ���� ��renxue
������ڣ�2011��02��01��
============================================================*/

#include "MathHelper.h"

CMathHelper::CMathHelper(void)
{}

CMathHelper::~CMathHelper(void)
{}

void CMathHelper::myRoundf(float &Data)//���������뷨ȥ��С������β��
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
/* �������ܣ� ���һԪ���η��̵ĸ�     							    	*/
/* ����˵��:  equation�������һԪ���η��̵�ϵ��						*/
/*            evalue�Ƿ��̵ĸ�			            					*/
/*            int ���صĽ�ĸ���		            					*/
/* �� �� ֵ:  double p��line�ߵľ���             						*/
/* ��    ��:  renxue					    							*/
/* ��    ��:  2011 �� 04 �� 11 ��										*/
/************************************************************************/
int CMathHelper::getValueOfquadraticEquation(osg::Vec3 &equation, float *evalue1, float *evalue2)
{// delta�������⴦����ʹdeltaС���㣬ҲҪ�������ȡ����ֵ�Ĵ���ȷ���ܹ��õ�������
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