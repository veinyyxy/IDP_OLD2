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
�ļ�����ColdFront.h ColdFront.cpp 
�� ���� CColdFront
�� �ࣺ CAlgorithmInterface
�� �ࣺ �� 
����˵���� ������㷨
����˵���� 

-------------------------------------�汾����-----------------------------------
V 1.1 
�� �ߣ��������ߣ����޸��ߣ�����
������ڣ�
����˵���� 
V 1.0 
ԭ���� ��renxue
������ڣ�2011�� 2��24��
============================================================*/

#include "ColdFront.h"
#include "VectorAlgorithm.h"
#include "MathHelper.h"


CColdFront::CColdFront(void)
{
}

CColdFront::~CColdFront(void)
{
}

void CColdFront::SetLineAlgorithmWidth(float width)
{
	m_width = width;
	m_sideLength = 3*width;
}

/************************************************************************/
/* �������ܣ� ������㷨��������������ߵ㼯,�������������ߵ㼯       */
/* ����˵��:  ���㷨����ͨ����Ա������������������εı߳�       		*/
/*            inputLineΪ��������ߵ㼯									*/
/*            outputLineΪ���������ߵ㼯								*/
/* �� �� ֵ:  bool ����ִ�з���true�����򷵻�false                 		*/
/* ��    ��:  renxue													*/
/* ��    ��:  2011 �� 02 �� 24 ��										*/
/************************************************************************/
bool CColdFront::SymbolLineAlgorithm(osg::Vec3Array &inputLine,  std::vector<osg::ref_ptr<osg::Vec3Array>> &outputLine)
{
	float width = m_sideLength;
	CVectorAlgorithm linehelper;
	float length = linehelper.distanceOfPolyLine(inputLine);	
	float coldfrontsize = 4 * width;//����ߵĵ�λ���ȣ����趨Ϊ�����α߳����ı�
	
	if (length < width * 2)  return false;//���߳���С�������α߳�������
	else //��˫�����ε��������
	{
		osg::ref_ptr<osg::Vec3Array> inputLineCopy(&inputLine);
		osg::ref_ptr<osg::Vec3Array> lineVertex = new osg::Vec3Array;
		*lineVertex = inputLine;

		outputLine.push_back(lineVertex);//��ԭ�������ߵ㼯���뵽���line��
		int num, i;
		float len;
		num = length/coldfrontsize;
		if(num == 0)
		{
			len = (length - 2*width)/2;
			num = 1;
		}
		else
			len = (length - num*coldfrontsize)/2;		
		
		//�����������Ͼ���ĳ��Ϊwidth�ĵ������
		i = 0;
		osg::Vec3 mid1, mid2, p1, p2, p3, p4, p5;
		osg::Vec3 startp = inputLine[0];
		int nPos1, nPos3, nPos5, tempPos;
		while(i<num)
		{
			linehelper.getPointOnPolyline(p1, nPos1, inputLine, startp, len);
			linehelper.getPointOnPolyline(p3, nPos3, inputLine, startp, len+width);
			linehelper.getPointOnPolyline(p5, nPos5, inputLine, startp, len+2*width);			
			
			linehelper.getMidpoint(p1, p3, mid1);
			linehelper.getPointToPOnRightHand(p1, p3, mid1, width/2.0, 1, p2);
			
			linehelper.getMidpoint(p3, p5, mid2);
			linehelper.getPointToPOnRightHand(p3, p5, mid2, width/2.0, 1, p4);
			tempPos = nPos3;
			osg::ref_ptr<osg::Vec3Array> triangle1(new osg::Vec3Array), triangle2(new osg::Vec3Array);
			//triangle1<<p1<<p2<<p3;//������ϵ�������
			triangle1->push_back(p1);
			triangle1->push_back(p2);
			triangle1->push_back(p3);

			while(tempPos > nPos1)
			{
				//triangle1<<inputLine[tempPos--];
				triangle1->push_back(inputLine[tempPos--]);
			}
			//triangle1<<p1;
			triangle1->push_back(p1);

			//triangle2<<p3<<p4<<p5;
			triangle2->push_back(p3);
			triangle2->push_back(p4);
			triangle2->push_back(p5);

			while(nPos5 > nPos3)
			{
				//triangle2<<inputLine[nPos5--];
				triangle2->push_back(inputLine[nPos5--]);
			}
			//triangle2<<p3;
			triangle2->push_back(p3);

			//outputLine<<triangle1<<triangle2;
			outputLine.push_back(triangle1);
			outputLine.push_back(triangle2);

			len += coldfrontsize;
			i++;
		}
	}	
	return true;
}