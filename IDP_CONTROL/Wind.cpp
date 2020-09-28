/*============================================================ 
�ļ�����Wind.cpp 
�� ���� CWind
�� �ࣺ CAlgorithmInterface
�� �ࣺ �� 
����˵���� ����㷨
����˵���� 

-------------------------�汾����---------------------------
V 1.1 
�� �ߣ��������ߣ����޸��ߣ�����
������ڣ�
����˵���� 
V 1.0 
ԭ���� ��renxue
������ڣ�2011��08��23��
============================================================*/

#include "Wind.h"
#include "VectorAlgorithm.h"
#include "MathHelper.h"


CWind::CWind(int iWidth)
{
	CAlgorithmInterface::m_width = iWidth;
	m_nRatio = 6;
}

CWind::~CWind(void)
{
}

void CWind::SetLineAlgorithmWidth(float width)
{
	m_width = width;
	m_sideLength = 3*width;
}

/************************************************************************/
// �������ܣ�����ĺ���
/************************************************************************/
bool CWind::SymbolLineAlgorithm(osg::Vec3Array &inputLine,  std::vector<osg::ref_ptr<osg::Vec3Array>> &outputLine)
{
	return false;
}

/************************************************************************/
/* �������ܣ� ����㷨�����������������,��������ķ��㼯             */
/* ����˵��:  ���㷨����ͨ����Ա�������÷���Ϻ��ߵı߳�         		*/
/*            P�Ƿ���λ�ã�fAngle�Ƿ���fSpeed�Ƿ���
              outputLine�Ƿ�������㼯                                */
/* �� �� ֵ:  bool ����ִ�з���true�����򷵻�false                 		*/
/* ��    ��:  renxue													*/
/* ��    ��:  2011 �� 08 �� 23 ��										*/
/************************************************************************/
bool CWind::WindAlgorithm(float fSpeed, std::vector<osg::ref_ptr<osg::Vec3Array>> &outputLine)
{
	CVectorAlgorithm linehelper;

	osg::Vec3  StartPoint(0, 0, 0), EndPoint(0, m_sideLength*2.0, 0);
	osg::ref_ptr<osg::Vec3Array> sharft_real(new osg::Vec3Array), barb_real(new osg::Vec3Array);
	osg::Vec3Array& shaft = *sharft_real;
	osg::Vec3Array& barb = *barb_real;//shaft�Ƿ���ϵĵ㣬barb�Ƿ����ϵĵ�
	//shaft<<StartPoint<<EndPoint;
	shaft.push_back(StartPoint);
	shaft.push_back(EndPoint);
	//outputLine<<shaft;
	outputLine.push_back(sharft_real);

	int nType = WindScaleBasedSpeed(fSpeed);
	if(nType<0)
	{
		return false;
	}
	if(nType<=36)
	{
		m_nRatio = 6;
	}
	else if(nType>36 && nType<200)
	{
		m_nRatio = 10;
	}
	else if(nType>200)
	{
		m_nRatio = 20;
	}

	//������
	osg::Vec3 tempA, tempB;
	if(nType == 0)   //�޷�
	{
		linehelper.getPointInVectorABToP(StartPoint, EndPoint, StartPoint, m_width*(m_nRatio-2), tempA);
		linehelper.getPointToPOnRightHand(StartPoint, EndPoint, tempA, m_width, 0, tempB);
		//barb<<EndPoint<<tempB;
		barb.push_back(EndPoint);
		barb.push_back(tempB);
		//outputLine<<barb;
		outputLine.push_back(barb_real);
	}
	else 
	{
		CaculateBarb(shaft, nType, outputLine);
	}		
	return true;
}

/************************************************************************/
/* �������ܣ� ���ݷ���ȷ����ļ���                                      */
/* ����˵��:  fSpeed��float�͵ķ���       	                        	*/
/*            array�Ƿ�˵ĵ㼯��nType�Ƿ缶,outputLine�Ƿ���ĵ㼯     */
/* �� �� ֵ:  int ����ִ�з��ط缶�����򷵻�-1                  		*/
/* ��    ��:  renxue													*/
/* ��    ��:  2011 �� 09 �� 01 ��										*/
/************************************************************************/
int CWind::WindScaleBasedSpeed( float fSpeed )
{
	if(fSpeed<0) return -1;
	return (int)fSpeed/2;
}

/************************************************************************/
/* �������ܣ� �����㷨�����������������,��������ķ���㼯             */
/* ����˵��:  ���㷨����ͨ����Ա�������÷����Ϻ��ߵı߳�         		*/
/*            array�Ƿ�˵ĵ㼯��nType�Ƿ缶,outputLine�Ƿ���ĵ㼯     */
/* �� �� ֵ:  bool ����ִ�з���true�����򷵻�false                 		*/
/* ��    ��:  renxue													*/
/* ��    ��:  2011 �� 09 �� 01 ��										*/
/************************************************************************/
bool CWind::CaculateBarb( osg::Vec3Array &array, int nType, std::vector<osg::ref_ptr<osg::Vec3Array>> &outputLine )
{
	CVectorAlgorithm linehelper;
	osg::Vec3 tempA, tempB, tempC;
	float     fLongWidth = m_width * 3.0;//�����ߵĳ���


	int nShaftRange, nRange;
	nRange = 1.5;
	nShaftRange = m_nRatio + nRange;
	int i=0;
	while(nType)
	{
		osg::ref_ptr<osg::Vec3Array> barb_real(new osg::Vec3Array);
		osg::Vec3Array& barb = *barb_real;
		if(nType>=10)
		{
			linehelper.getPointInVectorABToP(array[0], array[1], array[0], m_width*(nShaftRange-i*nRange), tempA);
			linehelper.getPointToPOnRightHand(array[0], array[1], tempA, fLongWidth, 0, tempB);
			linehelper.getPointInVectorABToP(array[0], array[1], array[0], m_width*(nShaftRange-i*nRange-nRange), tempC);
			//barb<<tempC<<tempA<<tempB<<tempC;
			//outputLine<<barb;
			barb.push_back(tempC);
			barb.push_back(tempA);
			barb.push_back(tempB);
			barb.push_back(tempC);
			outputLine.push_back(barb_real);

			nType -= 10;
		}
		else if(nType>=2)
		{
			linehelper.getPointInVectorABToP(array[0], array[1], array[0], m_width*(nShaftRange-i*nRange), tempA);
			linehelper.getPointToPOnRightHand(array[0], array[1], tempA, fLongWidth, 0, tempB);
			linehelper.getPointInVectorABToP(array[0], array[1], array[0], m_width*(nShaftRange-i*nRange-nRange), tempC);
			//barb<<tempC<<tempB;
			//outputLine<<barb;
			barb.push_back(tempC);
			barb.push_back(tempB);
			outputLine.push_back(barb_real);

			nType -= 2;
		}
		else if(nType>0)
		{
			linehelper.getPointInVectorABToP(array[0], array[1], array[0], m_width*(nShaftRange-i*nRange), tempA);
			linehelper.getPointToPOnRightHand(array[0], array[1], tempA, fLongWidth, 0, tempB);
			linehelper.getPointInVectorABToP(array[0], array[1], array[0], m_width*(nShaftRange-i*nRange-nRange), tempC);
			linehelper.getMidpoint(tempB, tempC, tempA);
			//barb<<tempC<<tempA;
			//outputLine<<barb;
			barb.push_back(tempC);
			barb.push_back(tempA);

			outputLine.push_back(barb_real);

			nType -= 1;
		}
		i++;
	}
	
	return true;
}
