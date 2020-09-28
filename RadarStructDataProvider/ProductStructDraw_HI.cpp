
//#include "../Arithmetics/Arith_291_HI/struct_291_HI.h"
//#include "ProductStructDraw_HI.h"
//#include <wx/wx.h>
#include "ProductStructDraw_HI.h"
#include "Struct_291_HI.h"
#include "goPoint.h"
//#include "goLineString.h"
//#include "goPolygon.h"
#include "goQuadrilateral.h"
#include <math.h>
//#include "qvectoe3d.h"
#include "RadProjTransform.h"

#include <QtCore/QCoreApplication>
#include "QtXml/QXmlStreamReader"
#include <QtCore/QFile>
#include <QDir>
#include <QDate>

#define  racial 10000.000
#define  km  1000.000

CProductStructDraw_HI::~CProductStructDraw_HI()
{
}

STRUCT_DATA_TYPE CProductStructDraw_HI::GetTypeOfProduct()
{
	STRUCT_DATA_TYPE n = STRUCT_HI;
	return n ;
}

int CProductStructDraw_HI::GetCountOfLayer(CProductRead* lpProduct)
{
	int nCount=0;
	PDBlock* blok = lpProduct->GetHeadOfProduct();
	if(blok!=NULL )
	{
		nCount = blok->NumberOfLayers;
	}
	return nCount;
}

bool CProductStructDraw_HI::DumpAll(Layer* layer,CProductRead* lpProduct, 
	/*CProductDrawParam* lpParam,*/P_COLORTABLE_VECTOR pColorTable,QString filename,int requestLayer/* = 0*/)
{
	bool bRet=false;
	
	 osg::Vec3 vp(0,0,0);
	CRadReadStationFile pRead ;
	QString appConfigPth = QCoreApplication::applicationDirPath().remove("/bin") + "/config/";
	pRead.ReadFile(appConfigPth+"CINRAD_classB.dat");
	RADSTATION_HASH stationHash = pRead.RadStaion();
	QStringList stationInfo = FileNameAnalyes(filename);
	QString id = stationInfo.at(0);
	P_CINRAD_STATIONINFO info = stationHash.value(id);
	if (info == NULL)
	{
		 osg::Vec3 p(0,0,0);
		 vp = p;
	}
	osg::Vec3 p((info->nLat)/racial,(info->nLon)/racial,(info->nHei)/10);
	vp = p;
	proj = 0;
	proj = new RadProjTransform;
	QString destproj = "RADARPROJ" ;
	destproj.append(" ");
	destproj.append(QString::number(vp.y()));
	destproj.append(" ");
	destproj.append( QString::number(vp.x()));
	destproj.append(" ");
	destproj.append(QString::number(vp.z()));

	proj->setSourceCRS(destproj.toStdString());
	proj->setSourceProjName("雷达中心兰勃特投影");

	proj->setDesProjName("无投影");
	proj->setDestCRS("+proj=longlat +ellps=WGS84 +datum=WGS84 +no_defs");
	proj->Elevation(true);

	try{
		/*if(lpParam->m_nCount>=4)
		{*/
			//****************************************************//
			//这里需要修改各种冰水六相的颜色
			m_ColorHail             = osg::Vec4(pColorTable->at(0)->nRed /255,pColorTable->at(0)->nGreen /255,pColorTable->at(0)->nBlue /255,1);
			m_ColorHail_Big         = osg::Vec4(pColorTable->at(1)->nRed /255,pColorTable->at(1)->nGreen /255,pColorTable->at(1)->nBlue /255,1);
			m_ColorStrongHail       = osg::Vec4(pColorTable->at(2)->nRed /255,pColorTable->at(2)->nGreen /255,pColorTable->at(2)->nBlue /255,1);
			m_ColorStrongHail_Big   = osg::Vec4(pColorTable->at(3)->nRed /255,pColorTable->at(3)->nGreen /255,pColorTable->at(3)->nBlue /255,1);
		/*pdhi.m_ColorHail             = QVector4D(1.0, 1.0,1.0,255);
		pdhi.m_ColorHail_Big         = QVector4D(1.0, 1.0,1.0,255);
		pdhi.m_ColorStrongHail       = QVector4D(1.0, 1.0,1.0,255);
		pdhi.m_ColorStrongHail_Big   = QVector4D(1.0, 1.0,1.0,255);*/
			/*pdhi.m_ColorHail           = lpParam->m_colorRibbon[0];
			pdhi.m_ColorHail_Big         = lpParam->m_colorRibbon[1];
			pdhi.m_ColorStrongHail       = lpParam->m_colorRibbon[2];
			pdhi.m_ColorStrongHail_Big   = lpParam->m_colorRibbon[3];*/
			//****************************************************//
		//}
		////pdhi.SetParam(lpParam);

		DrawHI(lpProduct,layer);
		//pdhi.DrawHI(lpProduct,*container);
		bRet=true;
		CreatAttributeList(layer,lpProduct,filename);
	}
	catch(...)
	{
		bRet=false;
	}
	//lpParam->SetRatio(pdhi.m_pParam->m_ratio);
	return bRet;
}

QImage CProductStructDraw_HI::DrawThumbs(CProductRead* lpProduct,P_COLORTABLE_VECTOR pColorTable,float imsize)
{
	void * p = lpProduct->GetDataOfBlockStruct(0, 0);
	int maxdis = 650;
	float rule = maxdis/95.0;
	int image_size = maxdis/rule;
	QImage image(image_size,image_size, QImage::Format_RGB32);
	//QImage image(650,650, QImage::Format_RGB32);
	QRgb value;
	value = qRgb(255,255,255);
	image.fill(value);
	image.save("D:\\2.png");

	return image;

}

//////////////////////////////////////////////////////////////////////////////////////////////////////

CProductStructDraw_HI::CProductStructDraw_HI()
{
//	m_pParam = NULL;

	/*m_ColorHail.Set(0xFF0000);
	m_ColorHail_Big.Set(0x0000FF);
	m_ColorStrongHail.Set(0x00FF00);
	m_ColorStrongHail_Big.Set(0xCCFF00);
	*/
//	QVector4D(0.96, 0.35, 0.08, 255)
	m_ColorHail = osg::Vec4(255, 0, 0, 255);
	m_ColorHail_Big= osg::Vec4(0, 0, 0255, 255);
	m_ColorStrongHail= osg::Vec4(0, 255, 0, 255);
	m_ColorStrongHail_Big= osg::Vec4(186, 0, 0, 255);
	//m_ColorHail.Set(0xFF000001);
	//m_ColorHail_Big.Set(0x0000FF0101);
	//m_ColorStrongHail.Set(0x00FF00);
	//m_ColorStrongHail_Big.Set(0xCCFF0001);

	m_size1 = 0.04;//4;//000;
	m_size2 = 0.06;//6;//000;
	m_size3 = 0.08;//8;//000;
	m_size4 = 0.08;//8;//000;
	m_ID = 0;
}

//void CProductStructDraw_HI::SetParam(CProductDrawParam* lpParam)
//{
//	m_pParam = lpParam;
//}

//CProductDrawParam* CProductStructDraw_HI::GetParam()
//{
//	return m_pParam;
//}

void CProductStructDraw_HI::DrawHI(ProductBase* pProduct ,Layer* lay/*wxBitmap &bmp*/)
{
	bool bRet = false;
	/*wxBitmap bitmp;
	wxMemoryDC memDC;
	int nDPixel;*/
	PDBlock* blok;
	CProductRead* pThisProduct=(CProductRead*) pProduct;

	blok = pThisProduct->GetHeadOfProduct();

	//gFloat fKmPerPixel = m_pParam->m_ratio * 0.001;  //像素比
	//gFloat fR_km = 460.0;   

	/*int nR = fR_km/fKmPerPixel*2;

	bitmp.Create( nR, nR, -1);
	bmp.Create( nR, nR, -1);

	memDC.SelectObject( bitmp );
	memDC.Clear();
	memDC.SetBackgroundMode(wxTRANSPARENT);
	memDC.SetBrush( *wxBLACK_BRUSH );
	memDC.SetBackground(*wxBLACK);
	memDC.DrawRectangle(0,0,bitmp.GetWidth(),bitmp.GetHeight());*/

	try
	{
		int i = 0;
		for ( i = 0; i < blok->NumberOfLayers ; i++)//i
		{
			DILayer* layer = pThisProduct->GetHeadOfLayer(i);		
			if(2== layer->NumberOfBlocks )
			{
				HAIL_HEAD* tHeadData = (HAIL_HEAD*)pThisProduct->GetDataOfBlockStruct(i,0);//j=0
				HAIL_INDEX* tIndexData = (HAIL_INDEX*)pThisProduct->GetDataOfBlockStruct(i,1);//j=1

				DrawLayer(tHeadData,tIndexData/*, &memDC*/,lay);

				bRet=true;
			}
			else
			{
				bRet=false;
				break;
			}
		}
		if(i==blok->NumberOfLayers)
		{
			bRet=true;
		}
	}
	catch(...)
	{
		bRet=false;
	}
	if(bRet==true)
	{
		//bmp=bitmp;
	}
}

void CProductStructDraw_HI::DrawLayer(const void* pBlockHead,const void* pBlockData/*,wxDC* pDC*/,Layer* layer)
{
	HAIL_HEAD* tHeadData = (HAIL_HEAD*)pBlockHead;//j=0
	HAIL_INDEX* tIndexData = (HAIL_INDEX*)pBlockData;//j=1

	m_POH_MinThreshold  = tHeadData->POH_MinThreshold;
	m_POH_Threshold     = tHeadData->POH_Threshold;
	m_POSH_MinThreshold = tHeadData->POSH_MinThreshold;
	m_POSH_Threshold    = tHeadData->POSH_Threshold;
		
	for (int k = 0; k < tHeadData->NumOfStormCells ; k++)//k
	{
		gFloat fPOH  = ((float)(tIndexData[k].POH))/100.0;		// POH是整型，30--〉30%
		gFloat fPOSH = ((float)(tIndexData[k].POSH))/100.0;		//
		gFloat fMEHS = (float)(tIndexData[k].MEHS);

		if( fPOH<=m_POH_Threshold  && fPOH>=m_POH_MinThreshold )  //可能发生冰雹
		{
           DrawHail(tIndexData[k].r, tIndexData[k].az/*,pDC*/,layer);
		}
		else if(fPOH>=m_POH_Threshold  && fPOSH<=m_POSH_MinThreshold) //发生冰雹可能性很大
		{
           DrawHail_Big(tIndexData[k].r, tIndexData[k].az/*,pDC*/,layer);
		}
		else if(fPOSH>=m_POSH_MinThreshold  && fPOSH<=m_POSH_Threshold && fMEHS<=25.4)  //可能发生强冰雹, 1英寸==25.4mm
		{
		   DrawStrongHail(tIndexData[k].r, tIndexData[k].az/*,pDC*/,layer);
		}
		else if(fPOSH>=m_POSH_Threshold && fMEHS>=25.4)  //发生强冰雹可能 性很大
		{
		   DrawStrongHail_Big(tIndexData[k].r, tIndexData[k].az/*,pDC*/,layer);
		}
	}

}

void CProductStructDraw_HI::DrawHail(gFloat r, gFloat az/*,wxDC* pDC*/,Layer* layer)
{   
	osg::Vec3 pt_center = TransARtoXy(r,az);//(r,az,0.51); // 
    DrawTriangle(pt_center, m_size1, m_ColorHail ,layer);
}

void CProductStructDraw_HI::DrawHail_Big(gFloat r, gFloat az/*,wxDC* pDC*/,Layer* layer)
{     
	osg::Vec3 pt_center= TransARtoXy(r,az); //(r,az,0.51);// 
	DrawTriangle(pt_center, m_size2, m_ColorHail_Big, layer);

    Layer::MgoGeometry* pMG = layer->GetMgoGeometry();

	osg::ref_ptr <osg::Vec4Array> temp_color_array = new osg::Vec4Array;//颜色数组
	temp_color_array->push_back(m_ColorHail_Big);

	goQuadrilateral * pLine = new goQuadrilateral[3];
	goQuadrilateral * pTempLine = pLine;
	osg::Vec3 pt_Trig[3];
	pt_Trig[0] = osg::Vec3(pt_center.x(),pt_center.y()+m_size2,0.51);   //三角形三个顶点      0
	pt_Trig[1] = osg::Vec3(pt_center.x()-m_size2,pt_center.y()-m_size2,0.51);//             1   2
	pt_Trig[2] = osg::Vec3(pt_center.x()+m_size2,pt_center.y()-m_size2,0.51);

	osg::Vec3 pt1 =  osg::Vec3((pt_Trig[0].x() +  pt_Trig[2].x())/2, (pt_Trig[0].y() +  pt_Trig[2].y())/2,0.51);   //斜线填充
	osg::Vec3 pt11 = osg::Vec3((pt_Trig[1].x() +  pt_Trig[2].x())/2, pt_Trig[1].y(),0.51);

	osg::ref_ptr <osg::Vec3Array> temp_array1 = new osg::Vec3Array  ;//数据数组

	temp_array1->push_back(osg::Vec3((pt_Trig[0].x() +  pt_Trig[2].x())/2, (pt_Trig[0].y() +  pt_Trig[2].y())/2, 0.51));
	temp_array1->push_back(osg::Vec3((pt_Trig[1].x() +  pt_Trig[2].x())/2, pt_Trig[1].y(), 0.51));
	pTempLine->setGlenum(osg::PrimitiveSet::LINES);
	pTempLine->SetCoordinateTransform(layer->CoordTrans());
	pTempLine->SetOsgArray(temp_array1);
	pTempLine->SetOsgColorArray(*temp_color_array);
	pTempLine->SetParentLayer(layer);
	pMG->insert(Layer::MgoGeometryPair(pMG->size(),pTempLine));

	pTempLine = pLine +1;
	osg::ref_ptr <osg::Vec3Array> temp_array2= new osg::Vec3Array  ;//数据数组

	temp_array2->push_back(osg::Vec3((pt_Trig[0].x() +  pt1.x())/2, (pt_Trig[0].y() +  pt1.y())/2, 0.51));
	temp_array2->push_back(osg::Vec3((pt_Trig[1].x() +  pt11.x())/2, pt_Trig[1].y(), 0.51));
	pTempLine->setGlenum(osg::PrimitiveSet::LINES);
	pTempLine->SetCoordinateTransform(layer->CoordTrans());
	pTempLine->SetOsgArray(temp_array2);
	pTempLine->SetOsgColorArray(*temp_color_array);
	pTempLine->SetParentLayer(layer);
	pMG->insert(Layer::MgoGeometryPair(pMG->size(),pTempLine));
	
	pTempLine = pLine +2;
	osg::ref_ptr <osg::Vec3Array> temp_array3= new osg::Vec3Array  ;//数据数组
	temp_array3->push_back(osg::Vec3((pt_Trig[2].x() +  pt1.x())/2, (pt_Trig[2].y() +  pt1.y())/2, 0.51));
	temp_array3->push_back(osg::Vec3((pt_Trig[2].x() +  pt11.x())/2, pt_Trig[1].y(), 0.51));

	pTempLine->setGlenum(osg::PrimitiveSet::LINES);
	pTempLine->SetCoordinateTransform(layer->CoordTrans());
	pTempLine->SetOsgArray(temp_array3);
	pTempLine->SetOsgColorArray(*temp_color_array);
	pTempLine->SetParentLayer(layer);
	pMG->insert(Layer::MgoGeometryPair(pMG->size(),pTempLine));

}

void CProductStructDraw_HI::DrawStrongHail(gFloat r, gFloat az/*,wxDC* pDC*/,Layer* layer)
{     
	osg::Vec3 pt_center = TransARtoXy(r,az); //(r,az,0.51);//
    DrawTriangle(pt_center, m_size3, m_ColorStrongHail/*, pDC*/,layer);

	Layer::MgoGeometry* pMG = layer->GetMgoGeometry();
    pt_center.set(pt_center.x(),pt_center.y()+4,pt_center.z()); 

	goQuadrilateral * pLine = new goQuadrilateral[4];
	goQuadrilateral * pTempLine = pLine;

	osg::ref_ptr <osg::Vec3Array> temp_array = new osg::Vec3Array  ;//数据数组
	osg::ref_ptr <osg::Vec4Array> temp_color_array = new osg::Vec4Array;//颜色数组

	temp_color_array->push_back(osg::Vec4(1.0, 1.0,1.0,255));

	temp_array->push_back(osg::Vec3(pt_center.x()-0.04,pt_center.y()-0.04, 0.51));//三角形三个顶点
	temp_array->push_back(osg::Vec3(pt_center.x()+0.04,pt_center.y()+0.04, 0.51));
	pTempLine->setGlenum(osg::PrimitiveSet::LINES);
	pTempLine->SetCoordinateTransform(layer->CoordTrans());
	pTempLine->SetOsgArray(temp_array);
	pTempLine->SetOsgColorArray(*temp_color_array);
	pTempLine->SetParentLayer(layer);
	pMG->insert(Layer::MgoGeometryPair(pMG->size(),pTempLine));

	pTempLine = pLine +1;
	osg::ref_ptr <osg::Vec3Array> temp_array1 = new osg::Vec3Array  ;//数据数组
	temp_array1->push_back(osg::Vec3(pt_center.x()+0.04,pt_center.y()-0.04, 0.51));
	temp_array1->push_back(osg::Vec3(pt_center.x()-0.04,pt_center.y()+0.04, 0.51));
	pTempLine->setGlenum(osg::PrimitiveSet::LINES);
	pTempLine->SetCoordinateTransform(layer->CoordTrans());
	pTempLine->SetOsgArray(temp_array1);
	pTempLine->SetOsgColorArray(*temp_color_array);
	pTempLine->SetParentLayer(layer);
	pMG->insert(Layer::MgoGeometryPair(pMG->size(),pTempLine));

	pTempLine = pLine +2;
	osg::ref_ptr <osg::Vec3Array> temp_array2 = new osg::Vec3Array  ;//数据数组
	temp_array2->push_back(osg::Vec3(pt_center.x(),pt_center.y()-0.04, 0.51));
	temp_array2->push_back(osg::Vec3(pt_center.x(),pt_center.y()+0.04, 0.51));
	pTempLine->setGlenum(osg::PrimitiveSet::LINES);
	pTempLine->SetCoordinateTransform(layer->CoordTrans());
	pTempLine->SetOsgArray(temp_array2);
	pTempLine->SetOsgColorArray(*temp_color_array);
	pTempLine->SetParentLayer(layer);
	pMG->insert(Layer::MgoGeometryPair(pMG->size(),pTempLine));

	pTempLine = pLine +3;
	osg::ref_ptr <osg::Vec3Array> temp_array3 = new osg::Vec3Array  ;//数据数组
	temp_array3->push_back(osg::Vec3(pt_center.x()-0.04,pt_center.y(), 0.51));
	temp_array3->push_back(osg::Vec3(pt_center.x()+0.05,pt_center.y(), 0.51));
	pTempLine->setGlenum(osg::PrimitiveSet::LINES);
	pTempLine->SetCoordinateTransform(layer->CoordTrans());
	pTempLine->SetOsgArray(temp_array3);
	pTempLine->SetOsgColorArray(*temp_color_array);
	pTempLine->SetParentLayer(layer);
	pMG->insert(Layer::MgoGeometryPair(pMG->size(),pTempLine));
}

void CProductStructDraw_HI::DrawStrongHail_Big(gFloat r, gFloat az/*,wxDC* pDC*/,Layer* layer)   //发生强冰雹的可能性很大
{
   osg::Vec3 pt_center = TransARtoXy(r,az);//(r,az,0.51);// 
   DrawTriangle(pt_center, m_size4, m_ColorStrongHail_Big/*, pDC*/,layer);

   Layer::MgoGeometry* pMG = layer->GetMgoGeometry();
   goQuadrilateral * pLine = new goQuadrilateral[2];
   goQuadrilateral * pTempLine = pLine;

   osg::ref_ptr <osg::Vec3Array> temp_array = new osg::Vec3Array  ;//数据数组
   osg::ref_ptr <osg::Vec4Array> temp_color_array = new osg::Vec4Array;//颜色数组

   temp_color_array->push_back(osg::Vec4(1.0, 1.0,1.0,255));

   temp_array->push_back(osg::Vec3(pt_center.x()-0.04,pt_center.y(), 0.51));//三角形三个顶点
   temp_array->push_back(osg::Vec3(pt_center.x()+0.04,pt_center.y()+0.06, 0.51));

   pTempLine->setGlenum(osg::PrimitiveSet::LINES);
   pTempLine->SetCoordinateTransform(layer->CoordTrans());
   pTempLine->SetOsgArray(temp_array);
   pTempLine->SetOsgColorArray(*temp_color_array);
   pTempLine->SetParentLayer(layer);
   pMG->insert(Layer::MgoGeometryPair(pMG->size(),pTempLine));

   pTempLine = pLine +1;
   osg::ref_ptr <osg::Vec3Array> temp_array1 = new osg::Vec3Array  ;//数据数组
   temp_array1->push_back(osg::Vec3(pt_center.x()+0.04,pt_center.y(), 0.51));
   temp_array1->push_back(osg::Vec3(pt_center.x()-0.04,pt_center.y()+0.06, 0.51));
   pTempLine->setGlenum(osg::PrimitiveSet::LINES);
   pTempLine->SetCoordinateTransform(layer->CoordTrans());
   pTempLine->SetOsgArray(temp_array1);
   pTempLine->SetOsgColorArray(*temp_color_array);
   pTempLine->SetParentLayer(layer);
   pMG->insert(Layer::MgoGeometryPair(pMG->size(),pTempLine));
}


void CProductStructDraw_HI::DrawTriangle(osg::Vec3 pt_center, double size,osg::Vec4 BrushColor ,Layer* layer)   //画三角形符号
{    
	Layer::MgoGeometry* pMG = layer->GetMgoGeometry();

	osg::ref_ptr <osg::Vec3Array> temp_array = new osg::Vec3Array  ;//数据数组
	osg::ref_ptr <osg::Vec4Array> temp_color_array = new osg::Vec4Array;//颜色数组

	temp_array->push_back(osg::Vec3(pt_center.x(),pt_center.y()+size,0.51));
	temp_array->push_back(osg::Vec3(pt_center.x()-size,pt_center.y()-size,0.51));

	temp_array->push_back(osg::Vec3(pt_center.x()-size,pt_center.y()-size,0.51));
	temp_array->push_back(osg::Vec3(pt_center.x()+size,pt_center.y()-size,0.51));

	temp_array->push_back(osg::Vec3(pt_center.x()+size,pt_center.y()-size,0.51));
	temp_array->push_back(osg::Vec3(pt_center.x(),pt_center.y()+size,0.51));

	temp_color_array->push_back(BrushColor);

	goQuadrilateral* gpl = new goQuadrilateral;
	gpl->setGlenum(osg::PrimitiveSet::LINE_LOOP);
	gpl->SetCoordinateTransform(layer->CoordTrans());
	gpl->SetOsgArray(temp_array);
	gpl->SetOsgColorArray(*temp_color_array);
	gpl->SetParentLayer(layer);
	pMG->insert(Layer::MgoGeometryPair(pMG->size(),gpl));
}

osg::Vec3 CProductStructDraw_HI::TransARtoXy(gFloat fA,gFloat fR)
{
	osg::Vec3 src(fA*1000*sin(fR*M_PI/180),fA*1000*cos(fR*M_PI/180),0);
	osg::Vec3 res;
	proj->Transform(src, res);

	double x = res.x();
	double y = res.y();
	double z = 0.51;
	res.set(x,y,z);

	return res;
}

void CProductStructDraw_HI::CreatAttributeList(Layer* layer,CProductRead * pProductData,QString fileName)
{
	
	proAttr *pAttr = new proAttr;
	cDrawProduct*pDrawProduct = (cDrawProduct*)pProductData;
	if (pDrawProduct)
	{
		pDrawProduct->ReadInfo(pAttr,0);
	}
	//-------------用最大不模糊速度修改  速度产品颜色表fstart fend值

	//if (pAttr->pro_code == 254 || pAttr->pro_code == 255 || pAttr->pro_code == 301) //301-IW-逆风区
	//{
	//	m_nParam = pAttr->param1;
	//}

	//-------------end

	QString StrUnit;//产品单位
	StrUnit = QString("dBZ");
	//JudgeProdctUnit(fileName,StrUnit);
	// fileName = QString(tr("VAD"));
	QString tempStr;

	QString StrStationName,strProName;
	GetStationNameAndPName(fileName,StrStationName,strProName);

	layer->InsertLayerAttribute(strProName.toStdString(),std::string("产品名称："),"String");
	layer->InsertLayerAttribute(StrStationName.toStdString(),std::string("雷达站点："),"String");

	QDate tDate(1969,12,31);
	tDate = tDate.addDays(static_cast<int>(pAttr->scan_date));
	tempStr = tDate.toString("yyyy-MM-dd");
	layer->InsertLayerAttribute(tempStr.toStdString(),std::string("体扫开始日期："),"String");

	QTime tTime(0,0,0,0);
	tTime = tTime.addMSecs(pAttr->scan_time);
	tempStr = tTime.toString("hh:mm:ss");
	layer->InsertLayerAttribute(tempStr.toStdString(),std::string("体扫开始时间："),"String");

	QDate tDateG(1969,12,31);
	tDateG = tDateG.addDays(pAttr->generation_date);
	tempStr = tDateG.toString("yyyy-MM-dd");
	layer->InsertLayerAttribute(tempStr.toStdString(),std::string("产品生成日期："),"String");

	QTime tTimeG(0,0,0,0);
	tTimeG = tTimeG.addMSecs(pAttr->generation_time);
	tempStr = tTimeG.toString("hh:mm:ss");
	layer->InsertLayerAttribute(tempStr.toStdString(),std::string("产品生成时间："),"String");

	tempStr = QString::number((float)pAttr->altitude*0.1,'f',2);
	layer->InsertLayerAttribute(tempStr.toStdString(),std::string("海拔高度："),"String");

	QString strdu = QString::number(pAttr->longitude/10000);
	int ntemp = (pAttr->longitude%10000);
	int fen = (int)ntemp*60*0.0001;
	int ns =(  ((double)(ntemp*0.0001*60)-(double)fen)*60     );
	QString strfen = QString::number(fen);
	QString strS = QString::number(ns);
	tempStr = QString( "%1°%2′%3″ E").arg(strdu).arg(strfen).arg(strS);
	layer->InsertLayerAttribute(tempStr.toStdString(),std::string("经度："),"String");


	strdu = QString::number(pAttr->latitude/10000);
	int temp = (int)(pAttr->latitude%10000);
	fen = temp*0.0001*60;
	strfen = QString::number(fen);
	ns = (int)(  ((double)(temp*0.0001*60)-(double)fen)*60     );
	strS = QString::number(ns);
	tempStr = QString( "%1°%2′%3″ N").arg(strdu).arg(strfen).arg(strS);
	layer->InsertLayerAttribute(tempStr.toStdString(),std::string("纬度："),"String");

	tempStr = QString("维护");

	if (pAttr->operation_mode == 1)
	{
		tempStr = QString("晴空");
	}		
	if (pAttr->operation_mode == 2)
	{
		tempStr = QString("降水");
	}

	layer->InsertLayerAttribute(tempStr.toStdString(),std::string("工作模式："),"String");

	/*double nwidth = GetStructProductRadius(0)/1000.0;
	tempStr = QString::number(nwidth,'f',3);
	propertylist.push_back(QString(tr("有效范围：%1Km")).arg(tempStr));*/

	HAIL_INDEX * pHead = (HAIL_INDEX*)(pProductData->GetDataOfBlockStruct(0,0)); //轮廓头信息
	tempStr = QString::number(pHead->r);
	layer->InsertLayerAttribute(QString("%1Km").arg(tempStr).toStdString(),std::string("有效范围："),"String");

	if (pAttr->vcp_mode == 0)
	{
		tempStr =QString("VCP 11");
	}
	else if (pAttr->vcp_mode == 1)
	{
		tempStr =QString("VCP 21");
	}
	else if (pAttr->vcp_mode == 2)
	{
		tempStr =QString("VCP 31");
	}
	else if (pAttr->vcp_mode == 3)
	{
		tempStr =QString("VCP 32");
	}
	layer->InsertLayerAttribute(tempStr.toStdString(),std::string("体扫模式："),"String");

	tempStr = QString::number(0);
	tempStr = QString("中心：%1deg %2Km").arg(tempStr).arg(tempStr);
	layer->InsertLayerAttribute(tempStr.toStdString(),std::string("中心："),"String");

	//HAIL_INDEX * pHead = (HAIL_INDEX*)(pProductData->GetDataOfBlockStruct(0,0)); //轮廓头信息
	tempStr = QString::number(50);/*(pHead->r);*/
	layer->InsertLayerAttribute(QString("%1km 30 deg").arg(tempStr).toStdString(),std::string("距离圈："),"String");

	//tagBoundFileHeader * pHead = (tagBoundFileHeader*)(pProductData->GetDataOfBlockStruct(0,0)); //轮廓头信息
	//tempStr = QString::number(pHead->ikuchang);
	//propertylist.push_back(QString(tr("距离圈：%1m")).arg(tempStr));

	//m_fileDeclaration.push_back(StrUnit);
	delete pAttr;
	pAttr = NULL;
}

void CProductStructDraw_HI::GetStationNameAndPName(QString &fileName,QString &stationName,
	QString &productName)
{
	QStringList stationInfo = FileNameAnalyes(fileName);
	CRadReadStationFile pRead ;
	QString appConfigPth = QCoreApplication::applicationDirPath().replace("/bin","") + "/config/";
	pRead.ReadFile(appConfigPth+"CINRAD_classB.dat");
	RADSTATION_HASH stationHash = pRead.RadStaion();
	if (stationInfo.size()<2)
	{
		return;
	}
	QString id = stationInfo.at(0);
	P_CINRAD_STATIONINFO info = stationHash.value(id);
	if (info == 0)
	{
		stationName = "";
	}
	else
	{
		stationName = info->cName;
	}
	//productName = GetProductNameByShortName(stationInfo.at(1));
	QMap<QString,QString> ProductNameMap;
	ProductNameMap.insert(QString(tr("HI")),QString("冰雹指数(HI)"));
	productName = ProductNameMap.value(stationInfo.at(1));
}
QStringList CProductStructDraw_HI::FileNameAnalyes( QString fName )
{
	QStringList attribute = QDir(fName).dirName().split("_");
	QString str_time = attribute[3].left(attribute[3].indexOf("."));
	attribute[2].append(str_time);
	return attribute;
}