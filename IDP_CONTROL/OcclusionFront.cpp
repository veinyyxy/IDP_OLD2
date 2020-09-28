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
�ļ�����OcclusionFront.h OcclusionFront.cpp 
�� ���� COcclusionFront
�� �ࣺ CAlgorithmInterface
�� �ࣺ �� 
����˵���� �������㷨
����˵���� 

-------------------------------------�汾����------------------------------------------------ 
V 1.1 
�� �ߣ��������ߣ����޸��ߣ�����
������ڣ�
����˵���� 
V 1.0 
ԭ���� ����ѩ
������ڣ�2011��2��24��
============================================================*/

#include "OcclusionFront.h"
#include "VectorAlgorithm.h"


COcclusionFront::COcclusionFront(void)
{
}


COcclusionFront::~COcclusionFront(void)
{
}


void COcclusionFront::SetLineAlgorithmWidth(float width)
{
	m_width = width;
	m_sideLength = 3*width;
}

/************************************************************************/
/* �������ܣ� �������㷨��������������ߵ㼯,���������������㼯       */
/* ����˵��:  ���㷨����ͨ����Ա���������������������εı߳�       		*/
/*            inputLineΪ��������ߵ㼯									*/
/*            outputLineΪ�����������㼯								*/
/* �� �� ֵ:  bool ����ִ�з���true�����򷵻�false                 		*/
/* ��    ��:  renxue													*/
/* ��    ��:  2011 �� 02 �� 24 ��										*/
/************************************************************************/
bool COcclusionFront::SymbolLineAlgorithm(osg::Vec3Array &inputLine,  std::vector<osg::ref_ptr<osg::Vec3Array>> &outputLine)
{
	float width = m_sideLength;
	CVectorAlgorithm linehelper;
	float length = linehelper.distanceOfPolyLine(inputLine);
	float len;
	float h1, h2, h3;

	float occlusionfrontsize = 4*width;//ů���ߵĵ�λ���ȣ����趨ֱ�����ı�
	osg::Vec3 startp = inputLine[0];
	//outputLine<<inputLine;
	outputLine.push_back(osg::ref_ptr<osg::Vec3Array>(&inputLine));

	if (length < 2*width)  return false;//���߳���С��ֱ��������
	else //���Բͻ��Ĺؼ�����
	{ 
		int num, i;
		num = length/occlusionfrontsize;
		i = 0;
		if(num == 0)//���߳��ȴ���ֱ����������С��ֱ�����ı�
		{
			len = (length - 2*width)/2;
			num = 1;
		}
		else
			len = (length - num*occlusionfrontsize)/2;//���߳��ȴ���ֱ�����ı�

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
			linehelper.getPointOnPolyline(p1, nPos1, inputLine, startp, len);
			linehelper.getPointOnPolyline(p3, nPos3, inputLine, startp, len+width);
			linehelper.getPointOnPolyline(p5, nPos5, inputLine, startp, len+2*width);
			linehelper.getMidpoint(p1, p3, M);
			linehelper.getPointToPOnRightHand(p1, p3, M, width/2, 1, p2);			

			//�ռ������ε㼯
			osg::ref_ptr<osg::Vec3Array> triangle(new osg::Vec3Array), semicircle(new osg::Vec3Array);
			tempPos = nPos3;
			//triangle<<p1<<p2<<p3;
			triangle->push_back(p1);
			triangle->push_back(p2);
			triangle->push_back(p3);

			while(tempPos > nPos1)
			{
				triangle->push_back(inputLine[tempPos--]);
			}
			triangle->push_back(p1);

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
			//semicircle<<p3<<QL1<<QL2<<QL3<<p4<<QR3<<QR2<<QR1<<p5;
			semicircle->push_back(p3);
			semicircle->push_back(QL1);
			semicircle->push_back(QL2);
			semicircle->push_back(QL3);
			semicircle->push_back(p4);
			semicircle->push_back(QR3);
			semicircle->push_back(QR2);
			semicircle->push_back(QR1);
			semicircle->push_back(p5);
			while(nPos5 > nPos3)
			{
				semicircle->push_back(inputLine[nPos5--]);
			}
			semicircle->push_back(p3);

			//outputLine<<triangle<<semicircle;
			outputLine.push_back(triangle);
			outputLine.push_back(semicircle);

			len += occlusionfrontsize;
			i++;
		}
	}	
	return true;
}