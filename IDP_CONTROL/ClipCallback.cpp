#include <osg/ImageUtils>
#include <osg/ClipPlane>
#include <osg/ShapeDrawable>

#include <osgEarth/ImageUtils>
#include <osgEarth/MapNode>
#include <osgEarth/ModelLayer>
#include <osgEarthUtil/SkyNode>
#include <windows.h>
#include "cDrawProduct.h"
#include "InterpolationRadarData.h"
#include "RadProjTransform.h"
#include "ExtendMap.h"
#include "OsgEarthBridge.h"
#include "VolumeImage.h"
#include "DataProvider.h"
#include "PlotDialog.h"
#include "ClipCallback.h"

ClipCallback::ClipCallback( goto_gis::Map* map )
{
	_map = map;
}


void ClipCallback::DrawPlotGraph(goto_gis::Map* srcMap, osg::Vec3& p1, osg::Vec3& p2, osg::Vec3& p3, osg::Vec3& p4, osg::Node* pClipNode )
{
	Layer * layer0  = srcMap->GetLayer("��1���ǲ�");
	if (!layer0)
	{
		return;
	}
	DataProvider* dataProvider = layer0->GetDataProvider();
	if (dataProvider)
	{
		VolumeImage* pVI = (VolumeImage*)(dataProvider->GetExtendInterface(3));
		if(!pVI) return;

		pVI->Cutting(p1,p2,p3,p4,pClipNode,srcMap->GetCoordinateTransform());
	}
}

void ClipCallback::MoveCompleted( osg::Vec3& p1, osg::Vec3& p2, osg::Vec3& p3, osg::Vec3& p4, osg::Node* pClipNode, osg::Image** ppImage )
{
	static int i = 1;
	if(i == 18) 
		i = 1;

	double dfMinus,dfFreq,dfTime;
	LARGE_INTEGER litmp;
	LONGLONG Qpart1,Qpart2,Useingtime;

	//QueryPerformanceFrequency(&litmp);//ȡ�ø߾������м�������Ƶ��f,��λ��ÿ����ٴΣ�n/s����
	//dfFreq = (double)litmp.QuadPart;
	//QueryPerformanceCounter(&litmp);//ȡ�ø߾������м���������ֵ
	//Qpart1 = litmp.QuadPart; //��ʼ��ʱ

	DrawPlotGraph(_map,p1, p2, p3, p4,pClipNode);

	//QueryPerformanceCounter(&litmp);//ȡ�ø߾������м���������ֵ
	//Qpart2 = litmp.QuadPart; //��ֹ��ʱ
	//dfMinus = (double)(Qpart2 - Qpart1)/1000000;//���������ֵ
	//qDebug()<< "useing:"<<dfMinus;
}


void ClipCallback::DrawPlotGraph(goto_gis::Map* srcMap, QImage* pImage, osg::Vec3& p1, osg::Vec3& p2, osg::Vec3& p3, osg::Vec3& p4)
{
	//ExtendMap* pMap = dynamic_cast<ExtendMap*>(srcMap);
	Map* pMap = srcMap;
	Layer * layer0  = pMap->GetLayer("��1���ǲ�");
	if (!layer0)
	{
		return;
	}
	DataProvider* dataProvider = layer0->GetDataProvider();
	if (dataProvider)
	{
		void* pvoid = dataProvider->GetExtendInterface(0);
		InterpolationRadarData* pRadarInterface = (InterpolationRadarData*)(pvoid);
		if(pRadarInterface)
		{
			RadProjTransform* pRadProj = dynamic_cast<RadProjTransform*>(layer0->CoordTrans());

			osg::Vec3  p1 = pRadProj->xyWorld2DC(p3.x(),p3.y());
			osg::Vec3  p2 = pRadProj->xyWorld2DC(p4.x(),p4.y());

			double dRatio = abs(Get_Length(p1.x()/1000.0-p2.x()/1000.0,p1.y()/1000.0-p2.y()/1000.0))/abs(Get_Length(p3.x()-p4.x(),p3.y()-p4.y()));


			double ProductCenter_X = p3.x()-p1.x()/1000.0/dRatio;
			double ProductCenter_Y = p3.y()+p1.y()/1000.0/dRatio;

			//boost::any  _center , _elevtion;
			//map->GetLayer("��1���ǲ�")->GetLayerAtrributeFromName("���ģ�",_center);
			//map->GetLayer("��1���ǲ�")->GetLayerAtrributeFromName("���ǣ�",_elevtion);
			double _x1 , _y1, _x2, _y2;
			_x1 = p1.x();
			_y1 = p1.y();

			_x2 = p2.x();
			_y2 = p2.y();

			pRadarInterface->SetStartEnd(&_x1,  &_y1,  &_x2,  &_y2,dRatio,ProductCenter_X,ProductCenter_Y);
			pRadarInterface->Profiles();
			QStringList InfoList;
			P_FIELD_MAP layerAttributeDescripe = pMap->GetLayer("��1���ǲ�")->GetLayerAttributeMap();
			std::map<int, GeomField>::iterator layerIter = layerAttributeDescripe->begin();
			while(layerIter != layerAttributeDescripe->end())
			{
				int index = layerIter->first;
				std::string typeName = layerIter->second.typeName();
				std::string name = layerIter->second.name();
				std::string comment = layerIter->second.comment();
				boost::any _any = GetAttributeValue(index,pMap->GetLayer("��1���ǲ�"));
				if (typeName == "String") // �ַ���
				{
					std::string _value =  boost::any_cast<std::string>(_any);
					InfoList.append(QString::fromStdString(_value));
				}
				layerIter++;
			}
			if (pRadarInterface->ds <= 0 || pRadarInterface->dh <= 0)
			{
				//QMessageBox::information(this,tr("��ʾ"),tr("�����쳣��������ѡ�����ݣ�"));
				return;
			}
			PlotDialog pPlotDialog;
			pPlotDialog.GetArray((""),pRadarInterface->ds,pRadarInterface->dh,pRadarInterface->buf,pRadarInterface->max,pRadarInterface->maxH,pRadarInterface->maxAz,pRadarInterface->maxR,dataProvider->GetColorTable(),pRadarInterface->AzList,pRadarInterface->RList,InfoList);
			pPlotDialog.GetImage(pImage);
		}
	}
}