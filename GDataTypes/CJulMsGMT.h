///////////////////////////////////////////////////////
//CJulMsGMT.h by chenxiang 
//created by chenxiang 20080407
//for����ʱ�䴦��
//�����պͱ�׼���ڻ���ת��
//����ͱ�׼ʱ�以��ת��
//��׼����ʱ��ĸ���ʱ��ͱ���ʱ�以��ת��
//�����պ���ĸ���ʱ��ͱ���ʱ��Ļ���ת��
//��ȡ��ǰ�����պ���
///////////////////////////////////////////////////////

#pragma once

#ifndef		CJULMSGMT_H
#define		CJULMSGMT_H

#include	"GDef.h"
class DLLEXP_GSYS CJulMsGMT
{
public:
	static bool Jul2Greg(gUint16 JulDay,gInt32& Year,gInt32& Month,gInt32& Day);		//ת��Julian����Ϊ�������ڣ�Gregorian��
	static gUint16 Greg2Jul(gInt32 Year,gInt32 Month,gInt32 Day);						//ת���������ڣ�Gregorian��Ϊ������������(Julian)//����Jul2Greg()
	static gInt32 Time2MilliSecond(gInt32 T_Hour,gInt32 T_Minute,gInt32 T_Second,gInt32 T_MilliSecond);//ʱ��ת����
	static bool MilliSecond2Time(gInt32 MilliSecond,gInt32& T_Hour,gInt32& T_Minute,gInt32& T_Second,gInt32& T_MilliSecond);//����תʱ��
	static bool BeijingTime2GMT(gInt32& Year,gInt32& Month,gInt32& Day,gInt32& T_Hour);	//����ʱ��ת����ʱ�� ���ڱ���ת���ֹ�����Сʱ���µ�ʱ�䲻��ı���˲��ṩ������Ȳ���//����Jul2Greg()��Greg2Jul()
	static bool GMT2BeijingTime(gInt32& Year,gInt32& Month,gInt32& Day,gInt32& T_Hour);	//����ʱ��ת����ʱ�� //����Jul2Greg()��Greg2Jul()
	static bool JulMsBJT2GMT(gUint16& JulDay,gInt32& MilliSecond);						//���������պ���ת���������պ���
	static bool JulMsGMT2BJT(gUint16& JulDay,gInt32& MilliSecond);						//���������պ���ת���������պ���
	static bool GetLocalJulMs(gUint16& JulDay,gInt32& MilliSecond);						//��ȡ��ǰ�����պ��� ����"��"//����Greg2Jul()��Time2Millisecond()

private:
	
};

#endif