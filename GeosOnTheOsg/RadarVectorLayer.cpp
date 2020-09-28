#include "goQuadrilateral.h"
#include "DataProvider.h"
#include "RadarVectorLayer.h"
#include <Windows.h>
#include <QDebug>
GOTO_NAMESPACE_START


RadarVectorLayer::RadarVectorLayer( DataProvider* data /*= 0*/, CoordinateTransform* ct /*= 0*/ ):Layer(data, ct)
{
	LayerType("RadarVectorLayer");

	//m_pNode->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
}


RadarVectorLayer::~RadarVectorLayer(void)
{
}

void RadarVectorLayer::TransformLayer()
{
	std::map<int, std::auto_ptr<goGeometry>>::iterator goGeomIter = m_pgoGeometry->begin();
	while(goGeomIter != m_pgoGeometry->end())
	{
		goGeometry* goGeom = goGeomIter->second.get();
		if(goGeom)
		{
			goGeom->SetCoordinateTransform(m_coordTrans);
			goGeom->TransformGeometry();
		}
		goGeomIter++;
	}
}

int RadarVectorLayer::CreateLayer( const std::string& filename )
{
	if(!m_dataProvider) 
	{
		return false;
	}
	//double dfMinus,dfFreq,dfTime;
	//LARGE_INTEGER litmp;
	//LONGLONG Qpart1,Qpart2,Useingtime;
	//QueryPerformanceFrequency(&litmp);//ȡ�ø߾������м�������Ƶ��f,��λ��ÿ����ٴΣ�n/s����
	//dfFreq = (double)litmp.QuadPart;
	//QueryPerformanceCounter(&litmp);//ȡ�ø߾������м���������ֵ
	//Qpart1 = litmp.QuadPart; //��ʼ��ʱ

	m_dataProvider->LoadFile(filename);

	//QueryPerformanceCounter(&litmp);//ȡ�ø߾������м���������ֵ
	//Qpart2 = litmp.QuadPart; //��ֹ��ʱ
	//dfMinus = (double)(Qpart2 - Qpart1)/1000000;//���������ֵ
	//qDebug()<< "step read useing:"<<dfMinus;
	//
	return true;
}

void RadarVectorLayer::CreateLayerData()
{
	if (b_trans)
	{	
		std::map<int, std::auto_ptr<goGeometry>>::iterator goGeomIter = m_pgoGeometry->begin();
		while(goGeomIter != m_pgoGeometry->end())
		{
			goGeometry* goGeom = goGeomIter->second.get();
			if(goGeom)
			{
				goGeom->SetCoordinateTransform(m_coordTrans);
				goGeom->SetParentLayer(this);
				goGeom->CreateOsgFromGeos(m_pNode);
			}
			goGeomIter++;
		}
	}
}

void RadarVectorLayer::operator=( const RadarVectorLayer& other )
{
	this->m_bVisible = other.m_bVisible;

	this->m_PendingFieldMap = other.m_PendingFieldMap;
	this->m_LayerAttributeDescripe = other.m_LayerAttributeDescripe;
	this->m_pLayerAttribteTable = other.m_pLayerAttribteTable;

	//this->m_pKSMapGeometry.reset(other.m_pKSMapGeometry.get());

	int count = 0;
	std::map<int, std::auto_ptr<goGeometry>>::iterator goGeomIter = other.m_pgoGeometry->begin();
	while(goGeomIter != other.m_pgoGeometry->end())
	{
		goGeometry* goGeom = goGeomIter->second.get();
		goQuadrilateral* thisGeometry = new goQuadrilateral();
		*thisGeometry = *dynamic_cast<goQuadrilateral*>(goGeom);
		thisGeometry->SetParentLayer(this);
		thisGeometry->SetCoordinateTransform(this->CoordTrans());
		//thisGeometry->CreateOsgFromGeos(this->m_pNode);
		this->m_pgoGeometry->insert(Layer::MgoGeometryPair(count,thisGeometry));
		count++;
		goGeomIter++;
	}
}

GOTO_NAMESPACE_END