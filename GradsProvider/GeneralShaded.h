/**************************************************************************************/
/*��Ŀ���� �˻�����ƽ̨                                                               */
/*�� �ߣ� ����ʥ                                                                      */
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
/*ԭ���� ������ʥ                                                       */
/*������ڣ�2011��6��22��                                               */
/************************************************************************/
#pragma once
#include "meteorologyproducts.h"

#include "gxX.h"
#include "gauser.h"

//#include "DivisionData.h"
#include <QFile>
#include <QByteArray>
#include <QCoreApplication>
#include <QtCore/QMutex>

//#include "Interpolation_Triangle.h"

class GeneralShaded : public MeteorologyProducts
{
public:
	GeneralShaded(void);
	~GeneralShaded(void);
	int Dump(goto_gis::Layer* pLayer, int iDirect = 0);

protected:
	void InitCommand();
	void ScaleDataShade();
	int Draw(const std::string& strCmd);
	char m_Command[65535];			// �ű�����
	char m_Temp[65535];
	QMap<double,double> m_ZlevelIndex;
};
