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
#if 0


typedef QVector <QRenderUnit * >  VECTOR_UNIT;					//���ڻ�������ͼ

class GeneralPolyLineBar : public MeteorologyProducts
{
public:
	GeneralPolyLineBar(void);
	~GeneralPolyLineBar(void);
	int Dump();

protected:
	QRenderable* pRenderable[2];
	char m_Command[256];			//�ű�����
};
#endif