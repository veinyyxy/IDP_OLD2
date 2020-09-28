#include "Draw_SBT_Product.h"
#include "Struct_281_SBT.h"
//#include "qvectoe3d.h"
#include "RadProjTransform.h"
#include "goLineString.h"
#include "goQuadrilateral.h"
#include "goPolygon.h"
//#include "GraticuleCoordinates.h"
//#include "Point.h"
//#include "Polygon.h"
#include <math.h>
#include <QtCore/QCoreApplication>
#include "QtXml/QXmlStreamReader"
#include <QtCore/QFile>
#include <QDir>
#include <QDate>

//#include "gis/Gis_Api.h"
//#include "CommonMethod.h"
//
//#include "GPM.h"
//#include "GDataFiles.h"
//#include "DirSeparator.h"

CProductStructDraw_SBT::CProductStructDraw_SBT()
{
	m_CenterLon       = 0.0;
	m_CenterLat       = 0.0;
	m_CenterAltitude  = 0.0;
	m_ProductNO       = 0;
	m_BitmapWidth     = 0;
	m_BitmapHeight    = 0;
	m_ProductNO       = 0;
	m_Elevation       = 0.0;
	m_Resolution      = 1.0;
};

CProductStructDraw_SBT::~CProductStructDraw_SBT()
{
};

//判断产品绘图类型
STRUCT_DATA_TYPE CProductStructDraw_SBT::GetTypeOfProduct()
{
	STRUCT_DATA_TYPE n=STRUCT_SBT;
	return n;
};

bool CProductStructDraw_SBT::DumpAll(Layer* layer,CProductRead* lpProduct, P_COLORTABLE_VECTOR pColorTable,QString filename,int requestLayer/* = 0*/)
{
	float maxdis = 0;
	tagBoundFileHeader * pHead = (tagBoundFileHeader*)(lpProduct->GetDataOfBlockStruct(0,0));
	if (pHead == NULL)
	{
		return false;
	}
	
	bool bRet = false;
	PDBlock* blok = lpProduct->GetHeadOfProduct();

	proAttr *pAttr = new proAttr;
	cDrawProduct*pDrawProduct = (cDrawProduct*)lpProduct;
	if (pDrawProduct)
	{
		pDrawProduct->ReadInfo(pAttr,0);
	}
	double strlon = QString::number(pAttr->longitude/10000.0).toDouble();
	double strlat = QString::number(pAttr->latitude/10000.0).toDouble();
	BoundHead * pDataBound0 = (BoundHead*)lpProduct->GetDataOfBlockStruct(0,1);
	try
	{
		int polyNum = 0 ;
		for (int i = requestLayer ; i < blok->NumberOfLayers ; i++)
		{
			tagBoundFileHeader * pHead = (tagBoundFileHeader*)(lpProduct->GetDataOfBlockStruct(i,0)); //轮廓头信息
			
			BoundHead * pDataBound = (BoundHead*)lpProduct->GetDataOfBlockStruct(i,1);
			l_Point * pDataBound11 = (l_Point*)lpProduct->GetDataOfBlockStruct(i,2);

			//double iCenterLon = strlon;
			//double iCenterLat = strlat;

			Layer::MgoGeometry* pMG = layer->GetMgoGeometry();
			for(int j = 0; j< pHead->cntBound; j++)
			{
				double iCenterLon = pDataBound[j].cen_lon*0.0001;
				double iCenterLat = pDataBound[j].cen_lat*0.0001;

				RadProjTransform* pRadProjTransform = new RadProjTransform;
				pRadProjTransform->Set(iCenterLon,iCenterLat,0.51);
				//goQuadrilateral *ln = new goQuadrilateral;
				//ln->setGlenum(osg::PrimitiveSet::LINE_LOOP);
				//osg::ref_ptr <osg::Vec3Array> temp_array = new osg::Vec3Array  ;//数据数组
				//osg::ref_ptr <osg::Vec4Array> temp_color_array = new osg::Vec4Array;//颜色数组

				//ln->SetCoordinateTransform(layer->CoordTrans());
				//ln->SetOsgArray(temp_array);
				//ln->SetOsgColorArray(*temp_color_array);
				//ln->SetParentLayer(layer);
				//pMG->insert(Layer::MgoGeometryPair(j,ln));

				//ln->SetCoordinateTransform(layer->CoordTrans());
				//ln->GetOsgGeometry()->setColorArray(temp_color_array);
				//ln->SetParentLayer(layer);
				//pMG->insert(Layer::MgoGeometryPair(j,ln));

				goQuadrilateral *pPolygon = new goQuadrilateral;
				pPolygon->setGlenum(osg::PrimitiveSet::LINE_LOOP);
				osg::ref_ptr <osg::Vec3Array> temp_parray = new osg::Vec3Array  ;//数据数组
				osg::ref_ptr <osg::Vec4Array> temp_color_parray = new osg::Vec4Array;//颜色数组

				for(int k = 0; k< pDataBound[j].len; k++)
				{
					double  lon = pDataBound11[pDataBound[j].OffsetOfPoint+k].lon/10000.0;
					double lat =  pDataBound11[pDataBound[j].OffsetOfPoint+k].lat/10000.0;

					osg::Vec3 temppoint = pRadProjTransform->xyWorld2DC(lon,lat);
					double temppointx = temppoint.x();
					double temppointy = temppoint.y();
					double mt = Get_Length(temppointx,temppointy);
					if (mt>maxdis)
					{
						maxdis = mt;
					}
					temp_parray->push_back(osg::Vec3(lon,lat ,0.51));
					//pOVectex->push_back(QVector3D(temppointx, temppointy,0.51));
					osg::Vec4 tempcolor =  osg::Vec4(pColorTable->at(i)->nRed /255,pColorTable->at(i)->nGreen /255,pColorTable->at(i)->nBlue /255,1);
					temp_color_parray->push_back(tempcolor);
				}	
				pPolygon->SetCoordinateTransform(layer->CoordTrans());
				pPolygon->SetOsgArray(temp_parray);
				pPolygon->SetOsgColorArray(*temp_color_parray);
				pPolygon->SetParentLayer(layer);
				pMG->insert(Layer::MgoGeometryPair(polyNum,pPolygon));
				polyNum++;
			}
		}
		CreatAttributeList(layer,lpProduct,filename);
 	}	

	catch(...)
	{
		bRet=false;
	}
	return bRet;

};

void CProductStructDraw_SBT::CreatAttributeList(Layer* layer,CProductRead * pProductData,QString fileName)
{
	
	proAttr *pAttr = new proAttr;
	cDrawProduct*pDrawProduct = (cDrawProduct*)pProductData;
	if (pDrawProduct)
	{
		pDrawProduct->ReadInfo(pAttr,0);
	}

	QString StrUnit;//产品单位
	StrUnit = QString(tr("dBZ"));
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
	tempStr = QString("%1°%2′%3″ E").arg(strdu).arg(strfen).arg(strS);
	layer->InsertLayerAttribute(tempStr.toStdString(),std::string("经度："),"String");

	strdu = QString::number(pAttr->latitude/10000);
	int temp = (int)(pAttr->latitude%10000);
	fen = temp*0.0001*60;
	strfen = QString::number(fen);
	ns = (int)(  ((double)(temp*0.0001*60)-(double)fen)*60     );
	strS = QString::number(ns);
	tempStr = QString("%1°%2′%3″ N").arg(strdu).arg(strfen).arg(strS);
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

	/*double nwidth = GetStructProductRadius(m_requestLayer)/1000.0;
	tempStr = QString::number(nwidth,'f',3);
	propertylist.push_back(QString(tr("有效范围：%1Km")).arg(tempStr));*/

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
    tempStr = QString(tr("%1deg %2Km")).arg(tempStr).arg(tempStr);
	layer->InsertLayerAttribute(tempStr.toStdString(),std::string("中心："),"String");

	tagBoundFileHeader * pHead = (tagBoundFileHeader*)(pProductData->GetDataOfBlockStruct(0,0)); //轮廓头信息
	tempStr = QString::number(pHead->ikuchang);
	layer->InsertLayerAttribute(QString("%1m").arg(tempStr).toStdString(),std::string("距离圈："),"String");

	//m_fileDeclaration.push_back(StrUnit);
	delete pAttr;
	pAttr = NULL;
}

void CProductStructDraw_SBT::GetStationNameAndPName(QString &fileName,QString &stationName,
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
	ProductNameMap.insert(QString(tr("SBT")),QString(tr("风暴轮廓识别跟踪(SBT)")));
	productName = ProductNameMap.value(stationInfo.at(1));
}
QStringList CProductStructDraw_SBT::FileNameAnalyes( QString fName )
{
	QStringList attribute = QDir(fName).dirName().split("_");
	QString str_time = attribute[3].left(attribute[3].indexOf("."));
	attribute[2].append(str_time);
	return attribute;
}

double CProductStructDraw_SBT::Get_Length( double dx, double dy )
{
	return( sqrt(dx*dx + dy*dy) );
}

QImage CProductStructDraw_SBT::DrawThumbs(CProductRead* lpProduct,P_COLORTABLE_VECTOR pColorTable,float imsize)
{
	maxdis = 0;//半径
	QList<QVector3D> m_DrawPoint,m_DrawCol;
	//QVector3DArray m_DrawPoint,m_DrawCol;

	bool bRet = false;
	PDBlock* blok = lpProduct->GetHeadOfProduct();

	proAttr *pAttr = new proAttr;
	cDrawProduct*pDrawProduct = (cDrawProduct*)lpProduct;
	if (pDrawProduct)
	{
		pDrawProduct->ReadInfo(pAttr,0);
	}
	double strlon = QString::number(pAttr->longitude/10000.0).toDouble();
	double strlat = QString::number(pAttr->latitude/10000.0).toDouble();
	BoundHead * pDataBound0 = (BoundHead*)lpProduct->GetDataOfBlockStruct(0,1);
	{
		for (int i = 0 ; i < blok->NumberOfLayers ; i++)
		{
			tagBoundFileHeader * pHead = (tagBoundFileHeader*)(lpProduct->GetDataOfBlockStruct(i,0)); //轮廓头信息

			BoundHead * pDataBound = (BoundHead*)lpProduct->GetDataOfBlockStruct(i,1);
			l_Point * pDataBound11 = (l_Point*)lpProduct->GetDataOfBlockStruct(i,2);

			iCenterLon = strlon;
			iCenterLat = strlat;

			for(int j = 0; j< pHead->cntBound; j++)
			{
				RadProjTransform* pRadProjTransform = new RadProjTransform;
				pRadProjTransform->Set(iCenterLon,iCenterLat,0.51);
				for(int k = 0; k< pDataBound[j].len; k++)
				{
					lon = pDataBound11[pDataBound[j].OffsetOfPoint+k].lon*0.0001;
					lat = pDataBound11[pDataBound[j].OffsetOfPoint+k].lat*0.0001;

					QVector3D temppoint;
					osg::Vec3 tp = pRadProjTransform->xyWorld2DC(lon,lat);
					temppoint.setX(tp.x());
					temppoint.setY(tp.y());
					temppoint.setZ(0);
					double temppointx = temppoint.x()/10000.0;
					double temppointy = temppoint.y()/10000.0;
					double mt = Get_Length(temppointx,temppointy);
					if (mt>maxdis)
					{
						maxdis = mt;
					}
					QVector4D tempcolor =  QVector4D(pColorTable->at(i)->nRed ,pColorTable->at(i)->nGreen ,pColorTable->at(i)->nBlue ,1);
					/*************************************************************************************/
					//画缩略图用
					m_DrawPoint.push_back(QVector3D(temppointx, temppointy,0));
					m_DrawCol.push_back(QVector3D(tempcolor.x(),tempcolor.y(),tempcolor.z()));
					/*************************************************************************************/
				}				
			}
		}
		
	}
	maxdis = maxdis*2;
	float rule = maxdis/imsize;
	int image_size = maxdis/rule+1;

	QString backPic =  QCoreApplication::applicationDirPath() + "/Config/BackGround.png";
	QImage backimg(backPic);
	QImage image = backimg.copy(0,0,image_size,image_size);

	//QImage image(image_size,image_size, QImage::Format_RGB32);
	QRgb value;
	value = qRgb(255,255,255);
	//image.fill(value);
	for (int i = 0;i<m_DrawCol.size();i++)
	{
		value = qRgb(m_DrawCol.at(i).x(),m_DrawCol.at(i).y(),m_DrawCol.at(i).z());
		image.setPixel((maxdis/2+m_DrawPoint.at(i).x())/rule,(maxdis/2-m_DrawPoint.at(i).y())/rule, value);
	}
	int cR = maxdis;
// 	value = qRgb(247, 91, 20);
// 	image.save("D:\\1.png");

	int picx = abs(int((maxdis/2+m_DrawPoint.last().x())/rule)-200);
	int picy = abs(int((maxdis/2+m_DrawPoint.last().y())/rule)-200);
	piclat = (iCenterLat-lat)/picy;
	piclon = (iCenterLon-lon)/picx;

// 	iCenterLon-piclon*200//左
// 	iCenterLat+piclat*200//上
// 	iCenterLon+piclon*200//右
// 	iCenterLat-piclon*200//下
	//double piclength = Get_Length(((maxdis/2+m_DrawPoint.last().x())/rule-200),((maxdis/2+m_DrawPoint.last().y())/rule-200));
	return image;
}

float CProductStructDraw_SBT::GetRadius(CProductRead* lpProduct)
{
	//maxdis = 0;//半径
	//PDBlock* blok = lpProduct->GetHeadOfProduct();

	//proAttr *pAttr = new proAttr;
	//cDrawProduct*pDrawProduct = (cDrawProduct*)lpProduct;
	//if (pDrawProduct)
	//{
	//	pDrawProduct->ReadInfo(pAttr,0);
	//}
	//double strlon = QString::number(pAttr->longitude/10000.0).toDouble();
	//double strlat = QString::number(pAttr->latitude/10000.0).toDouble();
	//BoundHead * pDataBound0 = (BoundHead*)lpProduct->GetDataOfBlockStruct(0,1);
	//{
	//	for (int i = 0 ; i < blok->NumberOfLayers ; i++)
	//	{
	//		tagBoundFileHeader * pHead = (tagBoundFileHeader*)(lpProduct->GetDataOfBlockStruct(i,0)); //轮廓头信息

	//		BoundHead * pDataBound = (BoundHead*)lpProduct->GetDataOfBlockStruct(i,1);
	//		l_Point * pDataBound11 = (l_Point*)lpProduct->GetDataOfBlockStruct(i,2);

	//		double iCenterLon = strlon;
	//		double iCenterLat = strlat;

	//		for(int j = 0; j< pHead->cntBound; j++)
	//		{
	//			RadProjTransform* pRadProjTransform = new RadProjTransform;
	//			pRadProjTransform->Set(iCenterLon,iCenterLat,0.51);
	//			for(int k = 0; k< pDataBound[j].len; k++)
	//			{
	//				double lon = pDataBound11[pDataBound[j].OffsetOfPoint+k].lon*0.0001;
	//				double lat = pDataBound11[pDataBound[j].OffsetOfPoint+k].lat*0.0001;

	//				QVector3D temppoint = pRadProjTransform->xyWorld2DC(lon,lat);
	//				double temppointx = temppoint.x();
	//				double temppointy = temppoint.y();
	//				double mt = Get_Length(temppointx,temppointy);
	//				if (mt>maxdis)
	//				{
	//					maxdis = mt;
	//				}
	//			}				
	//		}
	//	}

	//}
	//return maxdis;
	return 0;
}

void CProductStructDraw_SBT::GetRect( QRectF &rect )
{
	rect.setTop (iCenterLat+piclat*200) ;
	rect.setLeft(iCenterLon-piclon*200) ;

	rect.setBottom (iCenterLat-piclon*200) ;
	rect.setRight(iCenterLon+piclon*200) ;
}
