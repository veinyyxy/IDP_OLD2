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
�ļ�����WarmFront.h WarmFront.cpp 
�� ���� CWarmFront
�� �ࣺ CAlgorithmInterface
�� �ࣺ �� 
����˵���� ů���㷨
����˵���� 

-------------------------------------�汾����------------------------------------------------ 
V 1.1 
�� �ߣ��������ߣ����޸��ߣ�����
������ڣ�
����˵���� 
V 1.0 
ԭ���� ��renxue
������ڣ�2011��2��23��
============================================================*/

#include "WarmFront.h"
#include "VectorAlgorithm.h"

#define LINE_DEBUG_OUT 0

CWarmFront::CWarmFront(void)
{
}


CWarmFront::~CWarmFront(void)
{
}

void CWarmFront::SetLineAlgorithmWidth(float width)
{
	m_width = width;
	m_sideLength = 3*width;
}

/************************************************************************/
/* �������ܣ� ů�����㷨��������������ߵ㼯,���������ů���ߵ㼯       */
/* ����˵��:  ���㷨����ͨ����Ա��������ů���ϰ�Բ��ֱ��        		*/
/*            inputLineΪ��������ߵ㼯									*/
/*            outputLineΪ�����ů���ߵ㼯								*/
/* �� �� ֵ:  bool ����ִ�з���true�����򷵻�false                 		*/
/* ��    ��:  renxue													*/
/* ��    ��:  2011 �� 02 �� 23 ��										*/
/************************************************************************/
bool CWarmFront::SymbolLineAlgorithm(osg::Vec3Array &inputLine,  std::vector<osg::ref_ptr<osg::Vec3Array>> &outputLine)//����һ�����߷��̵ĵ㼯���ů���ߵĵ㼯,width��ů������Բ��ֱ��
{
	//************************************************
#if LINE_DEBUG_OUT
	qDebug("******************WarmFront: e-6 = %f, e-7 = %f*********************",1e-6, 1e-7);
	for(int i=0; i<inputLine.size(); i++)//debug--nirissa--20110301
	{
		qDebug("inputLine[%d]:x:%f, y:%f", i, inputLine[i].x(), inputLine[i].y());
	}
#endif
	//************************************************

	float width = m_sideLength;	
	CVectorAlgorithm linehelper;
	float length = linehelper.distanceOfPolyLine(inputLine);
	float len, h1, h2, h3;

	float warmfrontsize = 4*width;//ů���ߵĵ�λ���ȣ����趨ֱ�����ı�
	osg::Vec3 startp = inputLine[0];

	osg::ref_ptr<osg::Vec3Array> inputLineCopy = new osg::Vec3Array;
	*inputLineCopy = inputLine;
	outputLine.push_back(inputLineCopy);
	if (length < 2*width)  return false;//���߳���С��ֱ��������
	else //���Բͻ��Ĺؼ�����
	{ 
		int num, i;
		num = length/warmfrontsize;	
		i = 0;
		if(num == 0)//���߳��ȴ���ֱ����������С��ֱ�����ı�
		{
			len = (length - 2*width)/2;
			num = 1;
		}
		else
			len = (length - num*warmfrontsize)/2;//���߳��ȴ���ֱ�����ı�

		//�����������Ͼ���ĳ��Ϊwidth�ĵ������
		h1 = width*sqrt(5.0)/8;
		h2 = width*sqrt(3.0)/4;
		h3 = width*sqrt(15.0)/8;
		osg::Vec3 p1, p2, p3, p4, p5, M;	
		osg::Vec3 ml1, ml2, ml3, mr1, mr2, mr3;
		osg::Vec3 QL1, QL2, QL3, QR1, QR2, QR3;
		int nPos1, nPos3, nPos5, tempPos;
		while(i<num)
		{
#if LINE_DEBUG_OUT
			qDebug("d:%f, h1:%f, h2:%f, h3:%f, i=%d", width/2, h1, h2, h3, i);//debug--nirissa
#endif
			
			linehelper.getPointOnPolyline(p1, nPos1, inputLine, startp, len);
			linehelper.getPointOnPolyline(p3, nPos3, inputLine, startp, len+width);
			linehelper.getPointOnPolyline(p5, nPos5, inputLine, startp, len+2*width);
			//***************Debug--nirissa*****************
#if LINE_DEBUG_OUT
			float tResult;
			qDebug("p1: x:%f, y:%f����%f ", p1.x(), p1.y(), width);
			tResult =  sqrt((p3.x()-p1.x())*(p3.x()-p1.x())+ (p3.y()-p1.y())*(p3.y()-p1.y()));
			qDebug("p3: x:%f, y:%f����%f ", p3.x(), p3.y(), tResult);			
			tResult =  sqrt((p5.x()-p3.x())*(p5.x()-p3.x())+ (p5.y()-p3.y())*(p5.y()-p3.y()));
			qDebug("p5: x:%f, y:%f����%f ", p5.x(), p5.y(), tResult);
#endif  
   			//****************Debug--nirissa***************
			linehelper.getMidpoint(p1, p3, M);

#if LINE_DEBUG_OUT
			qDebug("M:%f, %f", M.x(), M.y());
			qDebug("**********p1, p3*********");
#endif
			linehelper.getPointToPOnRightHand(p1, p3, M, width/2, 1, p2);

			linehelper.getMidpoint(p1, M, ml2);
			linehelper.getMidpoint(p1, ml2, ml1);
			linehelper.getMidpoint(ml2, M, ml3);
			linehelper.getMidpoint(M, p3, mr2);
			linehelper.getMidpoint(M, mr2, mr3);
			linehelper.getMidpoint(mr2, p3, mr1);
			linehelper.getPointToPOnRightHand(p1, p3, ml1, h1, 1, QL1);
			linehelper.getPointToPOnRightHand(p1, p3, ml2, h2, 1, QL2);
			linehelper.getPointToPOnRightHand(p1, p3, ml3, h3, 1, QL3);
			linehelper.getPointToPOnRightHand(p1, p3, mr1, h1, 1, QR1);
			linehelper.getPointToPOnRightHand(p1, p3, mr2, h2, 1, QR2);
			linehelper.getPointToPOnRightHand(p1, p3, mr3, h3, 1, QR3);
			
			//�ռ���Բ�㼯
			osg::ref_ptr<osg::Vec3Array> semicircle1(new osg::Vec3Array);
			osg::ref_ptr<osg::Vec3Array> semicircle2(new osg::Vec3Array);
			//semicircle1<<p1<<QL1<<QL2<<QL3<<p2<<QR3<<QR2<<QR1<<p3;
			semicircle1->push_back(p1);
			semicircle1->push_back(QL1);
			semicircle1->push_back(QL2);
			semicircle1->push_back(QL3);
			semicircle1->push_back(p2);
			semicircle1->push_back(QR3);
			semicircle1->push_back(QR2);
			semicircle1->push_back(QR1);
			semicircle1->push_back(p3);
			tempPos = nPos3;
			while(tempPos > nPos1)
			{
				semicircle1->push_back(inputLine[tempPos--]);
			}
			semicircle1->push_back(p1);

			linehelper.getMidpoint(p3, p5, M);
			linehelper.getPointToPOnRightHand(p3, p5, M, width/2, 1, p4);
			linehelper.getMidpoint(p3, M, ml2);
			linehelper.getMidpoint(p3, ml2, ml1);
			linehelper.getMidpoint(ml2, M, ml3);
			linehelper.getMidpoint(M, p5, mr2);
			linehelper.getMidpoint(M, mr2, mr3);
			linehelper.getMidpoint(mr2, p5, mr1);
			linehelper.getPointToPOnRightHand(p3, p5, ml1, h1, 1, QL1);
			linehelper.getPointToPOnRightHand(p3, p5, ml2, h2, 1, QL2);
			linehelper.getPointToPOnRightHand(p3, p5, ml3, h3, 1, QL3);
			linehelper.getPointToPOnRightHand(p3, p5, mr1, h1, 1, QR1);
			linehelper.getPointToPOnRightHand(p3, p5, mr2, h2, 1, QR2);
			linehelper.getPointToPOnRightHand(p3, p5, mr3, h3, 1, QR3);

			//�ռ���Բ�㼯
			//semicircle2<<p3<<QL1<<QL2<<QL3<<p4<<QR3<<QR2<<QR1<<p5;
			semicircle2->push_back(p3);
			semicircle2->push_back(QL1);
			semicircle2->push_back(QL2);
			semicircle2->push_back(QL3);
			semicircle2->push_back(p4);
			semicircle2->push_back(QR3);
			semicircle2->push_back(QR2);
			semicircle2->push_back(QR1);
			semicircle2->push_back(p5);

			while(nPos5 > nPos3)
			{
				semicircle2->push_back(inputLine[nPos5--]);
			}
			semicircle2->push_back(p3);

			//outputLine<<semicircle1<<semicircle2;
			outputLine.push_back(semicircle1);
			outputLine.push_back(semicircle2);
			len += warmfrontsize;
			i++;
		}
	}	
	return true;
}
