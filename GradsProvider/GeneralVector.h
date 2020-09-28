/**************************************************************************************/
/*��Ŀ���� �˻�����ƽ̨                                                               */
/*�� �ߣ� ������                                                                      */
/*�� λ�� �й�����뼼������ɷ����޹�˾                                              */
/*����޸�ʱ�䣺 2011�ꣿ�£���                                                       */
/*��Ȩ������ ��Ȩ����2010-2018 ����������ҵ����������Ȩ��                             */
/*Copyright (c) 2010-2018  CHINA NATIONAL SOFTWARE &SERVICE CO.,LTD.All rights reserved */
/*��Ҫ������ 1. ���ʹ�÷�ΧΪ������ҵ������������Ա��δ����ɲ������Դ�����        */
/*           2. ���ʹ���߲����޸ı��ļ����ݣ�����ᵼ�����ʹ�ô���                */
/**************************************************************************************/
/*�ļ�����GeneralBarb.h GeneralBarb.cpp                                 */
/*�� ���� GeneralBarb                                                   */
/*�� �ࣺ MeteorologyProducts                                           */
/*�� �ࣺ ��                                                            */
/*����˵������Ʒ�㷨����                                                */
/*����˵����                                                            */
/************************************************************************/
/*V 1.0                                                                 */
/*ԭ���� ��WANGJL                                                       */
/*������ڣ�2011��6��22��                                               */
/************************************************************************/
#pragma once
#include "meteorologyproducts.h"

#include "gxX.h"
#include "gauser.h"

class GeneralVector : public MeteorologyProducts
{
public:
	GeneralVector(void);
	~GeneralVector(void);

protected:
	void InitCommand();
	void ScaleDataLine();

	char m_Command[256];			//�ű�����

	bool m_flag4ClearVolume;		//�Ƿ������ı�ʾ
	QString m_strArriChanged;		//�ı�����Եļ�¼

	QColor m_cBitColor;

	//CDivisionData m_divObject;							//�����и����
};

