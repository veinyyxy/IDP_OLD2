#include "Struct_292_M.h"

#include "RadProjTransform.h"
#include "goLineString.h"
#include "goQuadrilateral.h"
#include "goPolygon.h"

#include <osgText/Text>

#include <math.h>
#include <QtCore/QCoreApplication>
#include "QtXml/QXmlStreamReader"
#include <QtCore/QFile>
#include <QDir>
#include <QDate>


#include "Draw_M_Product.h"

CProductStructDraw_M::CProductStructDraw_M()
{

};

CProductStructDraw_M::~CProductStructDraw_M()
{

};

//判断产品绘图类型
STRUCT_DATA_TYPE CProductStructDraw_M::GetTypeOfProduct()
{
	STRUCT_DATA_TYPE n = STRUCT_M;
	return n;
};
//判断产品绘图类型7

//画产品图片，指定某张图片
bool CProductStructDraw_M::DumpAll(/*wxBitmap* lpBitmap*/Layer* layer,CProductRead* lpProduct, /*CProductDrawParam* lpParam,*/P_COLORTABLE_VECTOR pColorTable, QString filename,int requestLayer)
{
	MHead * pMHead = (MHead*)(lpProduct->GetDataOfBlockStruct(0, 0));

	VortexStruct * pVortex = (VortexStruct*)(lpProduct->GetDataOfBlockStruct(0, 1));
	MesocStruct  * pMesoc = (MesocStruct*)(lpProduct->GetDataOfBlockStruct(0, 2));

	proAttr *pAttr = new proAttr;
	cDrawProduct*pDrawProduct = (cDrawProduct*)lpProduct;
	if (pDrawProduct)
	{
		pDrawProduct->ReadInfo(pAttr,0);
	}

	double iWidth = pAttr->longitude/10000;//pMHead->Width;
	double iHeight = pAttr->latitude/10000;//pMHead->Height;
	bool bRet=false;
	try
	{
		Layer::MgoGeometry* pMG = layer->GetMgoGeometry();

		goQuadrilateral *pPolygon = new goQuadrilateral;
		pPolygon->SetBTransform(false);
		pPolygon->setGlenum(osg::PrimitiveSet::LINE_LOOP);

		osg::ref_ptr <osg::Vec3Array> temp_parray = new osg::Vec3Array  ;//数据数组
		osg::ref_ptr <osg::Vec4Array> temp_color_parray = new osg::Vec4Array;//颜色数组

		osg::Vec3 tempPoint(iWidth, iHeight,0.51);
		osg::Vec3 StartingPoint;
		layer->CoordTrans()->Transform(tempPoint,StartingPoint);

		int iColSpecial = 1100000; //第一列宽度	
		int iColWidth = 1000000;   //其他列宽度
		int iRowHeight = 300000;   //每一行高度
		//osg::Vec3 tempwhPoint(iColSpecial,iRowHeight,0.51);
		//osg::Vec3 whPoint;
		//layer->CoordTrans()->Transform(tempwhPoint,whPoint);
		//iColSpecial = iColWidth = whPoint.x();
		//iRowHeight = whPoint.y();

		//temp_parray->push_back(osg::Vec3(0,0 ,0.51));
		//temp_parray->push_back(osg::Vec3(iWidth,iHeight ,0.51));

		temp_parray->push_back(StartingPoint);
		temp_parray->push_back(osg::Vec3(StartingPoint.x(),StartingPoint.y()+(4+1)*iRowHeight ,0.51));

		temp_parray->push_back(osg::Vec3(StartingPoint.x(),StartingPoint.y()+(4+1)*iRowHeight ,0.51));
		temp_parray->push_back(osg::Vec3(StartingPoint.x()+(pMHead->VORTEXCnt)*iColWidth+iColSpecial, StartingPoint.y()+(4+1)*iRowHeight ,0.51));

		temp_parray->push_back(osg::Vec3(StartingPoint.x()+(pMHead->VORTEXCnt)*iColWidth+iColSpecial, StartingPoint.y()+(4+1)*iRowHeight ,0.51));
		temp_parray->push_back(osg::Vec3(StartingPoint.x()+(pMHead->VORTEXCnt)*iColWidth+iColSpecial, StartingPoint.y(), 0.51));

		temp_parray->push_back(osg::Vec3(StartingPoint.x()+(pMHead->VORTEXCnt)*iColWidth+iColSpecial, StartingPoint.y(), 0.51));
		temp_parray->push_back(StartingPoint);

		/*temp_parray->push_back(osg::Vec3(iWidth,iHeight ,0.51));*/
		//temp_parray->push_back(osg::Vec3(iColSpecial+(pMHead->VORTEXCnt)*iColWidth,iRowHeight*5 ,0.51));

		osg::Vec4 tempcolor =  osg::Vec4(pColorTable->at(0)->nRed /255,pColorTable->at(0)->nGreen /255,pColorTable->at(0)->nBlue /255,1);
		temp_color_parray->push_back(tempcolor);

		pPolygon->SetCoordinateTransform(layer->CoordTrans());
		pPolygon->SetOsgArray(temp_parray);
		pPolygon->SetOsgColorArray(*temp_color_parray);
		pPolygon->SetParentLayer(layer);
		pMG->insert(Layer::MgoGeometryPair(pMG->size(),pPolygon));
		//////////////////////////////////////////////////////////////////////////横线
		for (int i=0; i<4; i++)
		{
			goQuadrilateral *pPolygon = new goQuadrilateral;
			pPolygon->SetBTransform(false);
			pPolygon->setGlenum(osg::PrimitiveSet::LINE_LOOP);
			osg::ref_ptr <osg::Vec3Array> temp_parray = new osg::Vec3Array  ;//数据数组
			temp_parray->push_back(osg::Vec3(StartingPoint.x(),StartingPoint.y()+(i+1)*iRowHeight ,0.51));
			temp_parray->push_back(osg::Vec3(StartingPoint.x()+(pMHead->VORTEXCnt)*iColWidth+iColSpecial,StartingPoint.y()+(i+1)*iRowHeight ,0.51));

			pPolygon->SetCoordinateTransform(layer->CoordTrans());
			pPolygon->SetOsgArray(temp_parray);
			pPolygon->SetOsgColorArray(*temp_color_parray);
			pPolygon->SetParentLayer(layer);
			pMG->insert(Layer::MgoGeometryPair(pMG->size(),pPolygon));
		}

		pPolygon = new goQuadrilateral;
		pPolygon->SetBTransform(false);
		pPolygon->setGlenum(osg::PrimitiveSet::LINE_LOOP);
		temp_parray = new osg::Vec3Array  ;//数据数组

		temp_parray->push_back(osg::Vec3(StartingPoint.x()+iColSpecial, StartingPoint.y(), 0.51));
		temp_parray->push_back(osg::Vec3(StartingPoint.x()+iColSpecial, StartingPoint.y()+1500000 ,0.51));////////////////////////////////////////

		pPolygon->SetCoordinateTransform(layer->CoordTrans());
		pPolygon->SetOsgArray(temp_parray);
		pPolygon->SetOsgColorArray(*temp_color_parray);
		pPolygon->SetParentLayer(layer);
		pMG->insert(Layer::MgoGeometryPair(pMG->size(),pPolygon));

		for (int i=0; i<pMHead->VORTEXCnt-1; i++)
		{
			goQuadrilateral *pPolygon = new goQuadrilateral;
			pPolygon->SetBTransform(false);
			pPolygon->setGlenum(osg::PrimitiveSet::LINE_LOOP);
			osg::ref_ptr <osg::Vec3Array> temp_parray = new osg::Vec3Array  ;//数据数组
			temp_parray->push_back(osg::Vec3(StartingPoint.x()+iColSpecial+(i+1)*iColWidth , StartingPoint.y() ,0.51));
			temp_parray->push_back(osg::Vec3(StartingPoint.x()+iColSpecial+(i+1)*iColWidth , StartingPoint.y()+1500000 ,0.51));///////////////////////////////////////

			pPolygon->SetCoordinateTransform(layer->CoordTrans());
			pPolygon->SetOsgArray(temp_parray);
			pPolygon->SetOsgColorArray(*temp_color_parray);
			pPolygon->SetParentLayer(layer);
			pMG->insert(Layer::MgoGeometryPair(pMG->size(),pPolygon));
		}

		osg::Geode* pGeo = layer->GetLayerNode()->asGeode();
		if(pGeo)
		{
			pGeo->addDrawable(DrawText("风暴代号", osg::Vec3(StartingPoint.x()+5, StartingPoint.y()+80000,0.51),1000));		   //风暴代号
			pGeo->addDrawable(DrawText("特征", osg::Vec3(StartingPoint.x()+5, StartingPoint.y()+80000+1*iRowHeight,0.51),1000)); //特征(0:中尺度气旋，1:三维切变相关区)
			pGeo->addDrawable(DrawText("方位距离", osg::Vec3(StartingPoint.x()+5, StartingPoint.y()+80000+2*iRowHeight,0.51),1000));  //方位距离
			pGeo->addDrawable(DrawText("顶和底的高度", osg::Vec3(StartingPoint.x()+5, StartingPoint.y()+80000+3*iRowHeight,0.51),1000));			//发生的顶和底的高度
			pGeo->addDrawable(DrawText("径向和切向直径",osg::Vec3( StartingPoint.x()+5, StartingPoint.y()+80000+4*iRowHeight,0.51),1000)); //径向和切向直径
		}

	
		for (int i=0; i<pMHead->VORTEXCnt; i++) 
		{
			int iDiff_X = pVortex[i].CenterX;
			int iDiff_Y = pVortex[i].CenterY;
			int iRadiux = pVortex[i].Radius;
			int iCenX = iWidth/2;
			int iCenY = iHeight/2;
			iCenX = iCenX + iDiff_X;
			iCenY = iCenY + iDiff_Y;

			QString szTemp = "";
			szTemp =  QString::number(i+1);

			if(pGeo)
			{
				pGeo->addDrawable(DrawText(szTemp.toStdString(), osg::Vec3(iCenX+iRadiux+50000, iCenY-80000,0.51),1000));
				pGeo->addDrawable(DrawText(szTemp.toStdString(), osg::Vec3(StartingPoint.x()+iColSpecial+450000, StartingPoint.y()+80000,0.51),1000));  //风暴代号
			}

			if (pVortex->VORTEXType == 0)	   //中尺度气旋
			{	
				goQuadrilateral *pPolygon = new goQuadrilateral;
				pPolygon->SetBTransform(false);
				pPolygon->setGlenum(osg::PrimitiveSet::POINTS);//TRIANGLE_STRIP
				osg::ref_ptr <osg::Vec3Array> temp_parray = new osg::Vec3Array  ;//数据数组
				temp_parray->push_back(osg::Vec3(iCenX, iCenY,0.51));
				pPolygon->SetCoordinateTransform(layer->CoordTrans());
				pPolygon->SetOsgColorArray(*temp_color_parray);
				pPolygon->SetOsgArray(temp_parray);
				pPolygon->SetParentLayer(layer);
				pMG->insert(Layer::MgoGeometryPair(pMG->size(),pPolygon));
				if(pGeo)
					pGeo->addDrawable(DrawText("MESO", osg::Vec3(StartingPoint.x()+iColSpecial+i*iColWidth+300000, StartingPoint.y()+1*iRowHeight+80000,0.51),1000)); //中尺度气旋				
			}
			else if (pVortex->VORTEXType == 1) //三维相关切变区
			{
				goQuadrilateral *pPolygon = new goQuadrilateral;
				pPolygon->SetBTransform(false);
				pPolygon->setGlenum(osg::PrimitiveSet::POINTS);//TRIANGLE_STRIP
				osg::ref_ptr <osg::Vec3Array> temp_parray = new osg::Vec3Array  ;//数据数组
				temp_parray->push_back(osg::Vec3(iCenX, iCenY,0.51));
				pPolygon->SetCoordinateTransform(layer->CoordTrans());
				pPolygon->SetOsgArray(temp_parray);
				pPolygon->SetOsgColorArray(*temp_color_parray);
				pPolygon->SetParentLayer(layer);
				pMG->insert(Layer::MgoGeometryPair(pMG->size(),pPolygon));
				if(pGeo)
					pGeo->addDrawable(DrawText("3DC SHR", osg::Vec3(StartingPoint.x()+iColSpecial+i*iColWidth+200000, StartingPoint.y()+80000+1*iRowHeight,0.51),1000)); //三维相关切变区
			}	
		//	lpParam->SetRatio(m_Resolution*(pHead->ikuchang)); //SBT产品的分辨率特殊

			float fRange = pVortex[i].SMFeature.FCP.range;   //斜距
			float fAzimu = pVortex[i].SMFeature.FCP.azi;     //方位角
			szTemp = QString::number(fRange,'f',2);
			if(pGeo)
				pGeo->addDrawable(DrawText(szTemp.toStdString(), osg::Vec3(StartingPoint.x()+iColSpecial+i*iColWidth+100000, StartingPoint.y()+2*iRowHeight+80000,0.51),1000));
			szTemp = QString::number( fAzimu,'f',2);
			if(pGeo)
				pGeo->addDrawable(DrawText(szTemp.toStdString(), osg::Vec3(StartingPoint.x()+iColSpecial+i*iColWidth+550000, StartingPoint.y()+2*iRowHeight+80000,0.51),1000));

			float fTop = pVortex[i].HighestHFC; //发生的顶
			float fEnd = pVortex[i].LowestHFC;  //发生的底
			szTemp = QString::number(fTop,'f',2);
			if(pGeo)
				pGeo->addDrawable(DrawText(szTemp.toStdString(), osg::Vec3(StartingPoint.x()+iColSpecial+i*iColWidth+100000, StartingPoint.y()+3*iRowHeight+80000,0.51),1000));
			szTemp = QString::number(fEnd,'f',2);
			if(pGeo)
				pGeo->addDrawable(DrawText(szTemp.toStdString(), osg::Vec3(StartingPoint.x()+iColSpecial+i*iColWidth+550000, StartingPoint.y()+3*iRowHeight+80000,0.51),1000));

			float fRDiameter =  pVortex[i].SMFeature.DAR*2; //切向直径
			float fTDiameter =  pVortex[i].SMFeature.DAA*2; //经向直径
			szTemp = QString::number(fRDiameter,'f',2);
			if(pGeo)
				pGeo->addDrawable(DrawText(szTemp.toStdString(), osg::Vec3(StartingPoint.x()+iColSpecial+i*iColWidth+100000, StartingPoint.y()+4*iRowHeight+80000,0.51),1000));
			szTemp = QString::number(fTDiameter,'f',2);
			if(pGeo)
				pGeo->addDrawable(DrawText(szTemp.toStdString(), osg::Vec3(StartingPoint.x()+iColSpecial+i*iColWidth+550000, StartingPoint.y()+4*iRowHeight+80000,0.51),1000));	

/*			if(i==0)
			{
				imageLayer=bitmap.ConvertToImage();
			}
			else
			{
				wxImage imageCur = bitmap.ConvertToImage();
				imageCur.SetMaskColour(0,0,0);
				imageLayer.Paste(imageCur,0,0);
			}	*/		
		}		

		//memDC.SelectObject(wxNullBitmap);
		//*lpBitmap = wxBitmap(imageLayer);

		CreatAttributeList(layer,lpProduct,filename);
	}	

	catch(...)
	{
		bRet=false;
	}

	return bRet;
};
//画产品图片，指定某张图片

//判断产品绘图数量，共几张图
//int CProductStructDraw_M::GetCountOfLayer(CProductRead* lpProduct)
//{
//	int nCount=0;
//
//	PDBlock* blok = lpProduct->GetHeadOfProduct();
//	if(blok!=NULL )
//	{
//		nCount=blok->NumberOfLayers;
//	}
//	return nCount;
//};
//判断产品绘图数量，共几张图


osg::Drawable* CProductStructDraw_M::DrawText(const std::string& text,osg::Vec3 positionOffset,float size)
{
	osgText::Text* t = new osgText::Text();
	setlocale( LC_ALL, "chs" );
	int requiredSize = mbstowcs(NULL, text.c_str(), 0); 
	wchar_t* wtext = new wchar_t[requiredSize+1]; 
	mbstowcs(wtext, text.c_str(), requiredSize+1); 
	t->setText(wtext); 
	delete [] wtext; 
	wtext = NULL; 

	t->setPosition( positionOffset );
	t->setAutoRotateToScreen( false );
	t->setCharacterSizeMode( osgText::Text::OBJECT_COORDS_WITH_MAXIMUM_SCREEN_SIZE_CAPPED_BY_FONT_HEIGHT );
	t->setCharacterSize(100000);//100000  
	t->setAxisAlignment( osgText::Text::XY_PLANE );
	t->setAlignment( osgText::Text::LEFT_CENTER );
	//t->setAxisAlignment( osgText::Text::XZ_PLANE );
	//1115247.1
	//unsigned int  colour = TextColor();
	////int ir, ig, ib, ia;
	//osg::Vec4 colorVec4;
	//unsigned char ia = colour >> 24 & 0xFF;
	//unsigned char ir = colour >> 16 & 0xFF;
	//unsigned char ig = colour >> 8 & 0xFF;
	//unsigned char ib = colour & 0xFF;
	//colorVec4.set(ir,ig,ib,ia);
	//t->setColor(colorVec4);
	osgText::Font* font = osgText::readFontFile("Fonts/simhei.ttf");
	t->setFont(font);

	t->setBackdropColor( osg::Vec4( 0.3 ,  0.3 ,  0.3 , 1 ) );
	t->setBackdropType( osgText::Text::OUTLINE );

	return t;
}

QImage CProductStructDraw_M::DrawThumbs(CProductRead* lpProduct,P_COLORTABLE_VECTOR pColorTable,float imsize)
{
	QImage image;
	return image;
}

void CProductStructDraw_M::CreatAttributeList( Layer* layer,CProductRead * pProductData,QString fileName )
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

	MHead * pHead = (MHead*)(pProductData->GetDataOfBlockStruct(0,0)); //轮廓头信息
	tempStr = QString::number(50/*pHead->VORTEXCnt*/);
	layer->InsertLayerAttribute(QString("%1km").arg(tempStr).toStdString(),std::string("距离圈："),"String");

	//m_fileDeclaration.push_back(StrUnit);
	delete pAttr;
	pAttr = NULL;
}

void CProductStructDraw_M::GetStationNameAndPName( QString &fileName,QString &stationName,QString &productName )
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
	ProductNameMap.insert(QString(tr("M")),QString(tr("中尺度气旋(M)")));
	productName = ProductNameMap.value(stationInfo.at(1));
}

QStringList CProductStructDraw_M::FileNameAnalyes( QString fName )
{
	QStringList attribute = QDir(fName).dirName().split("_");
	QString str_time = attribute[3].left(attribute[3].indexOf("."));
	attribute[2].append(str_time);
	return attribute;
}

