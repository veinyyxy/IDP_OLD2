#include "ProductDraw_TITAN.h"
#include "Struct_296_TITAN.h"

#include "goQuadrilateral.h"
#include <math.h>

#include "RadProjTransform.h"
#include <QtCore/QCoreApplication>
#include "QtXml/QXmlStreamReader"
#include <QtCore/QFile>
#include <QDir>
#include <QDate>

CProductStructDraw_TITAN::CProductStructDraw_TITAN()
{
	
}

CProductStructDraw_TITAN::~CProductStructDraw_TITAN()
{
}


STRUCT_DATA_TYPE CProductStructDraw_TITAN::GetTypeOfProduct()
{
	STRUCT_DATA_TYPE n = STRUCT_TITAN;
	return n;
}

bool CProductStructDraw_TITAN::DumpAll(Layer* layer,CProductRead* lpProduct, P_COLORTABLE_VECTOR pColorTable,QString filename ,int requestLayer)
{
	bool bRet=false;




	osg::Vec4 tempcolor =  osg::Vec4(1,0,0,1);

	m_ClrStorm = tempcolor;
	m_ClrStormForecast = tempcolor;
	m_ClrTrackStorm = tempcolor;
	m_ClrTrackForecast = tempcolor;

	try
	{
		DrawTitan(lpProduct,layer);
	}
	catch(...)
	{
		bRet=false;
	}
	return bRet;
}

//int CProductStructDraw_TITAN::GetCountOfLayer(CProductRead* lpProduct)
//{
//	int nCount=0;
//
//	nCount = 1;
//
//	return nCount;
//}


//
////------------------------------------------------------------------------------------
////------------------------------------------------------------------------------------
//
//
//CProductDraw_TITAN::CProductDraw_TITAN()
//{
//	m_pParam=NULL;
//
//    m_Elevation = 0.0;
//	m_CenterLon       = 0.0;
//	m_CenterLat       = 0.0;
//	m_CenterAltitude  = 0.0;
//	
//	m_BitmapWidth     = 0;
//	m_BitmapHeight    = 0;
//    m_Resolution= 1.0;
//    m_fKmPerPixel = 1.0;
//    m_fR_km = 460.0;

//};
//	
//CProductDraw_TITAN::~CProductDraw_TITAN()
//{
//	
//}
//
//
//void CProductDraw_TITAN::SetParam(CProductDrawParam* lpParam)
//{
//	m_pParam = lpParam;
//}

void CProductStructDraw_TITAN::DrawTitan(ProductBase* pProduct ,Layer* layer)
{
	bool bRet=false;

	//wxBitmap bitmp;
	//wxMemoryDC memDC;

	CProductRead* pThisProduct=(CProductRead*) pProduct;
	PDBlock* blok = pThisProduct->GetHeadOfProduct();
	
	//gFloat fKmPerPixel = m_pParam->m_ratio * 0.001;  //像素比
	//gFloat fR_km = 460.0;   
	//if(blok!=NULL)
	//{
	//	int wight=blok->ProductDependent[1];
	//	int realwight=blok->ProductDependent[3];
	//	realwight = realwight > wight ? realwight : wight;
	//	CaculateLayout(realwight);
	//	wight*=1000;
	//	
	//}
	//int nDPixel = m_fR_km/m_fKmPerPixel*2;

	//bitmp.Create( nDPixel,nDPixel,-1);
	//bmp.Create( nDPixel,nDPixel,-1);

	//m_BitmapWidth  = bitmp.GetWidth();
	//m_BitmapHeight = bitmp.GetHeight();
	

	RadarInfor * pRadarInfo = pThisProduct->GetRadarInfoP(); 
	m_CenterLat  = pRadarInfo->Latitude*0.001;      //中心纬度
	m_CenterLon  = pRadarInfo->Longitude*0.001;     //中心经度
	m_CenterAltitude  = pRadarInfo->Altitude*0.001; //雷达海拔高度，单位：米

	//bool isDefaultRatio = (m_pParam->GetRatio()==1000)?true:false;
	//if (isDefaultRatio)
	//{
	//	m_Resolution = 1.0;
	//	//m_Resolution = /*0.5**/((double)(pHead->Height)/(double)(bitmp.GetHeight()));      
	//}
	//else
	//{
	//	m_Resolution = 1.0;
	//	//m_Resolution = lpParam->GetRatio()/(pHead->ikuchang);
	//}

	//memDC.SelectObject( bitmp );
	//memDC.Clear();
	//memDC.SetBackgroundMode(wxTRANSPARENT);
	//memDC.SetBrush( *wxBLACK_BRUSH );
	//memDC.SetBackground(*wxBLACK);
	//memDC.DrawRectangle(0,0,bitmp.GetWidth(),bitmp.GetHeight());
		
	try
	{
		DILayer* _layer = pThisProduct->GetHeadOfLayer(0);		
		if(5== _layer->NumberOfBlocks )
		{
			STITAN_OutputStorm* OutputData = (STITAN_OutputStorm*)pThisProduct->GetDataOfBlockStruct(0,0);//j=0
			STITAN_StormForecast* ForecastData = (STITAN_StormForecast*)pThisProduct->GetDataOfBlockStruct(0,1);//j=1


			STITAN_Track* TrackData = (STITAN_Track*)pThisProduct->GetDataOfBlockStruct(0,2);
			StructHead* pTrackHead = (StructHead*)pThisProduct->GetHeadOfBlockStruct(0,2);
			int TrackNumbers = pTrackHead->NumbersOfData;


			STITAN_TrackForecast* TrackForecastData = (STITAN_TrackForecast*)pThisProduct->GetDataOfBlockStruct(0,3);
			StructHead* pForecastTrackHead = (StructHead*)pThisProduct->GetHeadOfBlockStruct(0,3);
			int ForecastTrackcnt = pForecastTrackHead->NumbersOfData;


			m_LonlatData = (LonLatSPCE*)pThisProduct->GetDataOfBlockStruct(0,4);

            DrawOutputStorm(OutputData,layer);//识别出的风暴体信息
            DrawStormForecast(ForecastData,layer);//预测的一小时的风暴体信息
            //DrawTrackStorm(TrackData,TrackNumbers,layer);//追踪识别出的风暴体信息
            //DrawTrackForecast(TrackForecastData,ForecastTrackcnt,layer);//追踪预测的一小时的风暴体信息

			bRet=true;
		}
		else
		{
			bRet=false;
		}
	}
	catch(...)
	{
		bRet=false;
	}
	//memDC.SelectObject(wxNullBitmap);
	if(bRet==true)
	{
		//wxImage temp = bitmp.ConvertToImage();
		//bmp = wxBitmap(temp);
	}
}


//识别出的风暴体信息
void CProductStructDraw_TITAN::DrawOutputStorm(STITAN_OutputStorm* OutputData,Layer* layer)
{		
	int cnt = OutputData->m_nStormCount;
	for(int i=0; i<cnt; i++)
	{
		for(int j=0; j<73; j++)
		{
			float lon = OutputData->m_storm[i].lonlat[j].lon;
			float lat = OutputData->m_storm[i].lonlat[j].lat;

			Layer::MgoGeometry* pMG = layer->GetMgoGeometry();
			osg::ref_ptr <osg::Vec3Array> temp_array1 = new osg::Vec3Array  ;//数据数组
			temp_array1->push_back(osg::Vec3(lon,lat,0.51));
			osg::ref_ptr <osg::Vec4Array> temp_color_array = new osg::Vec4Array;//颜色数组
			temp_color_array->push_back(m_ClrStorm);

			goQuadrilateral * pTempLine = new goQuadrilateral;
			pTempLine->setGlenum(osg::PrimitiveSet::POINTS);
			pTempLine->SetCoordinateTransform(layer->CoordTrans());
			pTempLine->SetOsgArray(temp_array1);
			pTempLine->SetOsgColorArray(*temp_color_array);
			pTempLine->SetParentLayer(layer);
			pMG->insert(Layer::MgoGeometryPair(pMG->size(),pTempLine));
		}
	}
}


void CProductStructDraw_TITAN::DrawStormForecast(STITAN_StormForecast* ForecastData,Layer* layer)
{	
	int cnt = ForecastData->m_nStormCount;
	for(int i=0; i<cnt; i++)
	{
		for(int j=0; j<73; j++)
		{
			float lon = ForecastData->m_storm[i].lonlat[j].lon;
			float lat = ForecastData->m_storm[i].lonlat[j].lat;

           // wxPoint pnt = TransToPix(lon,lat);
			Layer::MgoGeometry* pMG = layer->GetMgoGeometry();
			osg::ref_ptr <osg::Vec3Array> temp_array1 = new osg::Vec3Array  ;//数据数组
			temp_array1->push_back(osg::Vec3(lon,lat,0.51));
			osg::ref_ptr <osg::Vec4Array> temp_color_array = new osg::Vec4Array;//颜色数组
			temp_color_array->push_back(m_ClrStormForecast);

			goQuadrilateral * pTempLine = new goQuadrilateral;
			pTempLine->setGlenum(osg::PrimitiveSet::POINTS);
			pTempLine->SetCoordinateTransform(layer->CoordTrans());
			pTempLine->SetOsgArray(temp_array1);
			pTempLine->SetOsgColorArray(*temp_color_array);
			pTempLine->SetParentLayer(layer);
			pMG->insert(Layer::MgoGeometryPair(pMG->size(),pTempLine));
			
		}
	}
}


void CProductStructDraw_TITAN::DrawTrackStorm(STITAN_Track *TrackData,int num,Layer* layer)
{
	Layer::MgoGeometry* pMG = layer->GetMgoGeometry();
	osg::ref_ptr <osg::Vec4Array> temp_color_array = new osg::Vec4Array;//颜色数组
	temp_color_array->push_back(m_ClrStorm);
	for(int j=0; j<num; j++)
	{
		int cnt = TrackData[j].m_nPointNum;
		osg::ref_ptr <osg::Vec3Array> temp_array = new osg::Vec3Array  ;//数据数组
		for(int i=0; i<cnt; i++)
		{
			float lon = TrackData[j].m_point[i].m_LonLat.lon;
			float lat = TrackData[j].m_point[i].m_LonLat.lat;
			temp_array->push_back(osg::Vec3(lon,lat,0.51));

			if(i>0)
			{
				temp_color_array->push_back(m_ClrTrackStorm);
				osg::ref_ptr <osg::Vec3Array> temp_array0 = new osg::Vec3Array  ;//数据数组
				temp_array0->push_back(temp_array->at(i-1));
				temp_array0->push_back(temp_array->at(i));

				goQuadrilateral * pTempLine = new goQuadrilateral;
				pTempLine->setGlenum(osg::PrimitiveSet::LINE_LOOP);
				pTempLine->SetCoordinateTransform(layer->CoordTrans());
				pTempLine->SetOsgArray(temp_array0);
				pTempLine->SetOsgColorArray(*temp_color_array);
				pTempLine->SetParentLayer(layer);
				pMG->insert(Layer::MgoGeometryPair(pMG->size(),pTempLine));

				//pDC->DrawLine(pnts[i-1],pnts[i]);
			}	
			
			//pDC->SetPen(wxPen(wxColor(255,255,255)));
	  //      pDC->SetBrush(wxBrush(wxColor(255,255,255)));
			//pDC->DrawCircle(pnt,2);

		}
	}
}


void CProductStructDraw_TITAN::DrawTrackForecast(STITAN_TrackForecast* TrackForecastData,int num,Layer* layer)
{	
	Layer::MgoGeometry* pMG = layer->GetMgoGeometry();
	osg::ref_ptr <osg::Vec4Array> temp_color_array = new osg::Vec4Array;//颜色数组
	temp_color_array->push_back(m_ClrTrackForecast);

	for(int j=0; j<num; j++)
	{
		int cnt = TrackForecastData->m_nPointNum;
		osg::ref_ptr <osg::Vec3Array> temp_array = new osg::Vec3Array  ;//数据数组
		for(int i=0; i<cnt; i++)
		{
			float lon = TrackForecastData[j].m_point[i].m_LonLat.lon;
			float lat = TrackForecastData[j].m_point[i].m_LonLat.lat;

			temp_array->push_back(osg::Vec3(lon,lat,0.51));

			if(i>0)
			{
				temp_color_array->push_back(m_ClrTrackForecast);
				osg::ref_ptr <osg::Vec3Array> temp_array0 = new osg::Vec3Array  ;//数据数组
				temp_array0->push_back(temp_array->at(i-1));
				temp_array0->push_back(temp_array->at(i));

				goQuadrilateral * pTempLine = new goQuadrilateral;
				pTempLine->setGlenum(osg::PrimitiveSet::LINE_LOOP);
				pTempLine->SetCoordinateTransform(layer->CoordTrans());
				pTempLine->SetOsgArray(temp_array0);
				pTempLine->SetOsgColorArray(*temp_color_array);
				pTempLine->SetParentLayer(layer);
				pMG->insert(Layer::MgoGeometryPair(pMG->size(),pTempLine));
			}	
			
			//pDC->SetPen(*wxRED_PEN);
	  //      pDC->SetBrush(*wxRED_BRUSH);
			//pDC->DrawCircle(pnt,2);

		}

		//delete []pnts;

	}
}

QImage CProductStructDraw_TITAN::DrawThumbs( CProductRead* lpProduct,P_COLORTABLE_VECTOR pColorTable,float imsize )
{
	QImage image;
	return image;
}



//wxPoint  CProductDraw_TITAN::TransToPix(float lon, float lat)
//{
//	gFloat fKmPerPixel = m_pParam->m_ratio * 0.001;  //像素比
//	gFloat fR_km = 460.0;   
//
//	int nDPixel = fR_km/fKmPerPixel*2;
//
//   wxPoint pt;
//   pt.x = nDPixel*(lon-m_LonlatData->m_Cat_West)/(m_LonlatData->m_Cat_East - m_LonlatData->m_Cat_West);
//   pt.y = nDPixel*(m_LonlatData->m_Cat_North-lat)/(m_LonlatData->m_Cat_North - m_LonlatData->m_Cat_South);
//
//   return pt;
//
//}

//int	CProductStructDraw_TITAN::xWorld2DC(double x, double y)            // 经度转化为屏幕坐标
//{
//	int xx;
//	double direction,range,height;
//	Shapes temp;
//	temp.Get_Az_L(m_CenterLon,m_CenterLat,m_CenterAltitude,x,y,m_Elevation,direction,range,height);
//
//	xx = m_BitmapWidth*0.5 + range * sin(direction*M_PI/180)/m_Resolution;       //m_Resolution已经乘了仰角的cos值
//	return (xx);
//
//}

//int	CProductDraw_TITAN::yWorld2DC(double y,double x)          // 纬度转化为屏幕坐标,y
//{
//	int yy;
//	double direction,range,height;
//	Shapes temp;
//	temp.Get_Az_L(m_CenterLon,m_CenterLat,m_CenterAltitude,x,y,0.0,direction,range,height);
//
//	yy = m_BitmapHeight*0.5 - range * cos(direction*M_PI/180)/m_Resolution;
//	return (yy);
//	
//}

//
//void CProductDraw_TITAN::CaculateLayout(gFloat fRMax)
//{
//	m_fR_km=fRMax;//230;
//	wxSize sz(460,460);
//	if(m_pParam!=NULL)
//	{
//		sz=m_pParam->m_size;
//	}
//
//	if(m_fR_km<=0)
//	{
//		m_fR_km=460;//here need modify.
//	}
//	else if(m_fR_km>1000.0)
//	{
//		m_fKmPerPixel=m_fR_km*2/1000.0;
//	}
//	else if(m_fR_km>100)
//	{
//		m_fKmPerPixel=1;
//	}						
//	else if(m_fR_km>10)
//	{
//		m_fKmPerPixel=0.1;
//	}
//	else
//	{			
//		m_fKmPerPixel=0.01;
//	}
//	int n=sz.GetWidth()<sz.GetHeight()?sz.GetWidth():sz.GetHeight();
//
//	m_fKmPerPixel=(m_fR_km*2)/n;
//};
