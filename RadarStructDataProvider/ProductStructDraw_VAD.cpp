#include "ProductStructDraw_VAD.h"
#include "Struct_285_EVP_269_VWP_268_VAD.h"
#include "goQuadrilateral.h"
#include <math.h>
#include <osgText/Text>
//#include "maptreewidgetapp.h"
#include <QtCore/QCoreApplication>
#include "QtXml/QXmlStreamReader"
#include <QtCore/QFile>
#include <QDir>
//#include "QualityControl_VelocityDealiasing.h"
#include <QDate>
//#include "QEText.h"                            写文字！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！
#include <qtcore/QVector>
CProductStructDraw_VAD::CProductStructDraw_VAD(void)
{
	//pDrawProduct = NULL;
//	m_requestLayer = 0;
		Max_Vel  = -100.0;
		m_ID = 0;
}
CProductStructDraw_VAD::~CProductStructDraw_VAD(void)
{
	/*if (NULL != pDrawProduct)
	{
	delete pDrawProduct;
	pDrawProduct = NULL;
	}*/
}

STRUCT_DATA_TYPE CProductStructDraw_VAD::GetTypeOfProduct()
{
	STRUCT_DATA_TYPE n = STRUCT_VAD;
	return n ;
}

bool CProductStructDraw_VAD::DumpAll(Layer* layer,CProductRead* lpProduct, P_COLORTABLE_VECTOR pColorTable,QString filename,int requestLayer)
{
	bool bRet=false;
	PDBlock* blok = lpProduct->GetHeadOfProduct();	
	RadarInfor * pRadarInfo = lpProduct->GetRadarInfoP(); 
	try
	{
		for (int i = requestLayer ; i < blok->NumberOfLayers ; i++)
		{
			DILayer* layerr = lpProduct->GetHeadOfLayer(i);
			//wxBitmap bitmp;
			//wxImage imageLayer;
			for (int j = 0;j < layerr->NumberOfBlocks ; j++)
			{
				DLIBlock* dblock = lpProduct->GetHeadOfBlock(i,j);
				if (dblock->TypeOfDataPackets == BLOCKTYPE_STRUCT)
				{				
					StructHead * structHead=lpProduct->GetHeadOfBlockStruct(i,j);
					gInt16	NumbersOfStruct=structHead->NumbersOfData; //结构个数
					gInt16	LengthOfEachStruct=structHead->LengthOfDataType;//每个结构体字节数
					void * p = lpProduct->GetDataOfBlockStruct(i, j);
					if(blok->ProductCode==268)//画VAD 产品
					{
						int iAbscissa[8] = {45,90,135,180,225,270,315,360}; //8个横坐标
						Draw_VAD_Background(p, layer,iAbscissa);	

						Draw_VAD_Line(p,layer,NumbersOfStruct,pColorTable/*,lpParam*/);//画曲线
					}
				}
			}
			break;
		}
		bRet=true;
		CreatAttributeList(layer,lpProduct,filename);
	}	
	catch(...)
	{
		bRet=false;
	}
	return bRet;
}

void CProductStructDraw_VAD::Draw_VAD_Background( void *p,Layer* layer,int iAbscissa[8] )
{
	Layer::MgoGeometry* pMG = layer->GetMgoGeometry();
	goQuadrilateral * pLine = new goQuadrilateral[22];
	goQuadrilateral * pTempLine = pLine;

	osg::ref_ptr <osg::Vec3Array> temp_array = new osg::Vec3Array  ;//数据数组
	osg::ref_ptr <osg::Vec4Array> temp_color_array = new osg::Vec4Array;//颜色数组

	temp_color_array->push_back(osg::Vec4(1.0, 1.0,1.0,255));
	temp_color_array->push_back(osg::Vec4(1.0, 1.0,1.0,255));

	temp_array->push_back(osg::Vec3(100, -40, 0.51));
	temp_array->push_back(osg::Vec3(100, -590, 0.51));

	pTempLine->SetBTransform(false);
	pTempLine->setGlenum(osg::PrimitiveSet::LINES);
	pTempLine->SetCoordinateTransform(layer->CoordTrans());
	pTempLine->SetOsgArray(temp_array);
	pTempLine->SetOsgColorArray(*temp_color_array);
	pTempLine->SetParentLayer(layer);
	pMG->insert(Layer::MgoGeometryPair(pMG->size(),pTempLine));
	//m_ID++;

	pTempLine = pLine +1;
	osg::ref_ptr <osg::Vec3Array> temp_array1 = new osg::Vec3Array  ;//数据数组

	temp_array1->push_back(osg::Vec3(1, -550, 0.51));
	temp_array1->push_back(osg::Vec3(801, -550, 0.51));

	pTempLine->SetBTransform(false);
	pTempLine->setGlenum(osg::PrimitiveSet::LINES);
	pTempLine->SetCoordinateTransform(layer->CoordTrans());
	pTempLine->SetOsgArray(temp_array1);
	pTempLine->SetOsgColorArray(*temp_color_array);
	pTempLine->SetParentLayer(layer);
	pMG->insert(Layer::MgoGeometryPair(pMG->size(),pTempLine));
	//m_ID++;

	int altitude=-40;

	pTempLine = pLine +2;
	osg::ref_ptr <osg::Vec3Array> temp_array2 = new osg::Vec3Array  ;//数据数组

	temp_array2->push_back(osg::Vec3(100, -548, 0.51));
	temp_array2->push_back(osg::Vec3(801, -548, 0.51));

	pTempLine->SetBTransform(false);
	pTempLine->setGlenum(osg::PrimitiveSet::LINES);
	pTempLine->SetCoordinateTransform(layer->CoordTrans());
	pTempLine->SetOsgArray(temp_array2);
	pTempLine->SetOsgColorArray(*temp_color_array);
	pTempLine->SetParentLayer(layer);
	pMG->insert(Layer::MgoGeometryPair(pMG->size(),pTempLine));
	//m_ID++;

	//画横线
	//memDC.DrawLine(100,548,801,548);
	for(int i=3;i<=11;i++)
	{
		pTempLine = pLine +i;

		osg::ref_ptr <osg::Vec3Array> temp_array2 = new osg::Vec3Array  ;//数据数组
		osg::ref_ptr <osg::Vec4Array> temp_color_array = new osg::Vec4Array;//颜色数组

		temp_color_array->push_back(osg::Vec4(1.0, 1.0,1.0,255));
		temp_color_array->push_back(osg::Vec4(1.0, 1.0,1.0,255));

		temp_array2->push_back(osg::Vec3(100, -548+(i-2)*50, 0.51));
		temp_array2->push_back(osg::Vec3(801, -548+(i-2)*50, 0.51));

		pTempLine->SetBTransform(false);
		pTempLine->setGlenum(osg::PrimitiveSet::LINES);
		pTempLine->SetCoordinateTransform(layer->CoordTrans());
		pTempLine->SetOsgArray(temp_array2);
		pTempLine->SetOsgColorArray(*temp_color_array);
		pTempLine->SetParentLayer(layer);
		pMG->insert(Layer::MgoGeometryPair(pMG->size(),pTempLine));

		QString Alt=QString::number(altitude);
		osg::Geode* pGeo = layer->GetLayerNode()->asGeode();
		if(pGeo)
			pGeo->addDrawable(DrawText(Alt.toStdString(), osg::Vec3(70, -548+(i-2)*50,0.51),18));

		altitude+=10;
		//m_ID++;

	}

	//画纵线
	for(int i=12;i<20;i++)
	{
		pTempLine = pLine +i;
		osg::ref_ptr <osg::Vec3Array> temp_array2 = new osg::Vec3Array  ;//数据数组
		osg::ref_ptr <osg::Vec4Array> temp_color_array = new osg::Vec4Array;//颜色数组
		temp_array2->push_back(osg::Vec3(100+(i-11)*80,-40, 0.51));
		temp_array2->push_back(osg::Vec3(100+(i-11)*80, -550, 0.51));

		temp_color_array->push_back(osg::Vec4(1.0, 1.0,1.0,255));
		temp_color_array->push_back(osg::Vec4(1.0, 1.0,1.0,255));

		pTempLine->SetBTransform(false);
		pTempLine->setGlenum(osg::PrimitiveSet::LINES);
		pTempLine->SetCoordinateTransform(layer->CoordTrans());
		pTempLine->SetOsgArray(temp_array2);
		pTempLine->SetOsgColorArray(*temp_color_array);
		pTempLine->SetParentLayer(layer);

		pMG->insert(Layer::MgoGeometryPair(pMG->size(),pTempLine));
		//m_ID++;
	}

	//AS 组织公式 FIT  =   CF0  -   cos(  Az  -  Du  )  *  U
	VAD_ELEMENTS Vad=((VAD_ELEMENTS *)p)[0];
	gInt16 CF0 = Vad.CF0;
	gInt16 Du = Vad.Du;
	gInt16 U = Vad.U;

	QString strCF0 = QString::number(CF0/100.0,'g',3); 
	QString strDu = QString::number(Du/10.0,'g',3);  
	QString strU = QString::number(U/100.0,'g',3); 
	QString strFormat = "FIT = ";
	strFormat.append(strCF0);
	strFormat.append(" - cos( Az - ");
	strFormat.append(strDu);
	strFormat.append(" )  * ");

	if(U<0)
	{
		strFormat.append("(");
	}

	strFormat.append(strU);

	if(U<0)
	{
		strFormat.append(" )");
	}

	osg::Geode* pGeo = layer->GetLayerNode()->asGeode();
	if(pGeo == 0) return;
	// AE 组织公式 100+(i-11)*80,-40, 0.51
	pGeo->addDrawable(DrawText("方位角", osg::Vec3(10,-565,0.51),18));    //横坐标意义
	pGeo->addDrawable(DrawText("(度)", osg::Vec3(10,-585,0.51),18));    
	// memDC.DrawText(wxT("FIT  =   CF0  -   cos(  Az  -  Du  )  *  U"),wxPoint(110,570));
	if(CF0 != -32768)
		pGeo->addDrawable(DrawText(strFormat.toStdString(),osg::Vec3(110,-570,0.51),18));
	pGeo->addDrawable(DrawText( "速度(V)", osg::Vec3(10,-548+(10)*50,0.51),18));     //纵坐标意义
	pGeo->addDrawable(DrawText("(m/s)", osg::Vec3(10,-548+(9.5)*50,0.51),18));
	/***************************************************************************/
#if 0


	QEText* pText = new QEText[20];
	QEText* pTempText = pText;
	pTempText->SetColor(QColor(65,226,218,255));
	pTempText->SetFont(QFont(tr("宋体"), 9));
	
	QString tempStr;//产品单位
	tempStr = QString(tr("azimuth"));
	//tempStr = QString::number((float)pAttr->altitude*0.1,'f',2);
	pTempText->SetString(tempStr);	
	pTempText->GetPositionArray()->push_back(QVector3D(70,-570,0.51));
	//pText->SetOriPosition(QEText::LEFT_BOTTOM);
	container->InsertRenderable(pTempText);

	pTempText = pText++;
	pTempText->SetColor(QColor(65,226,218,255));
	pTempText->SetFont(QFont(tr("宋体"), 9));
	tempStr = QString(tr("velocity"));
	//tempStr = QString::number((float)pAttr->altitude*0.1,'f',2);
	pTempText->SetString(tempStr);	
	pTempText->GetPositionArray()->push_back(QVector3D(50,-50,0.51));
	//pText->SetOriPosition(QEText::LEFT_TOP);
	container->InsertRenderable(pTempText);
	
	//colorAttribute->Recover();

	for ( int i =0; i<8;i++)//添加横坐标
	{
		pTempText = pText++;
		pTempText->SetColor(QColor(65,226,218,255));
		pTempText->SetFont(QFont(tr("宋体"), 9));
		tempStr = QString::number((float)iAbscissa[i],'f',2);
		pTempText->SetString(tempStr);	

		pTempText->GetPositionArray()->push_back(QVector3D(100+(i+1)*87.5, -570, 0.51));
		container->InsertRenderable(pTempText);
	}
	int iVercoor[9] = {-40,-30,-20,-10,0,10,20,30,40}; //9个纵坐标
	for ( int i =0; i<8;i++)//添加纵坐标
	{
		pTempText = pText++;
		pTempText->SetColor(QColor(65,226,218,255));
		pTempText->SetFont(QFont(tr("宋体"), 9));
		tempStr = QString::number((float)iVercoor[i],'f',2);
		//tempStr = QString::number((float)pAttr->altitude*0.1,'f',2);
		pTempText->SetString(tempStr);	

		pTempText->GetPositionArray()->push_back(QVector3D(100, -550+(i+1)*56, 0.51));
		//pText->SetOriPosition(QEText::LEFT_TOP);
		container->InsertRenderable(pTempText);

	}
	/***************************************************************************/
	#endif
}


osg::Drawable* CProductStructDraw_VAD::DrawText(const std::string& text,osg::Vec3 positionOffset,float size)
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
	t->setCharacterSize(16);//100000  
	t->setAxisAlignment( osgText::Text::XY_PLANE );
	t->setAlignment( osgText::Text::LEFT_CENTER );
	osgText::Font* font = osgText::readFontFile("Fonts/simhei.ttf");
	t->setFont(font);

	t->setBackdropColor( osg::Vec4( 0.3 ,  0.3 ,  0.3 , 1 ) );
	t->setBackdropType( osgText::Text::OUTLINE );

	return t;
}

void CProductStructDraw_VAD::Draw_VAD_Line( void *p,Layer* layer, int NumbersOfStruct,P_COLORTABLE_VECTOR pColorTable/*,CProductDrawParam* lpParam*/ )
{
	Layer::MgoGeometry* pMG = layer->GetMgoGeometry();

	goQuadrilateral * pLine = new goQuadrilateral;
	osg::ref_ptr <osg::Vec3Array> temp_array2 = new osg::Vec3Array  ;//数据数组

	osg::ref_ptr <osg::Vec4Array> temp_color_array = new osg::Vec4Array;//颜色数组
	temp_color_array->push_back(osg::Vec4(1.0, 1.0,1.0,255));
	temp_color_array->push_back(osg::Vec4(1.0, 1.0,1.0,255));

	for(int i=0;i<NumbersOfStruct/*1*/;i++)
	{
		VAD_ELEMENTS Vad=((VAD_ELEMENTS *)p)[i];
		int x = 0;
		int y = 0;
		//计算360个点
		double fit[360];
		double fit2[360];             //用double型提高精度
		for(int i = 0;i<360 ;i++)
		{
			if(Vad.CF0 == -32768)
			{
				continue;
			}
			//FIT=CF0-cos(Az-Du)*U;  （单位：0.01m/s）
			double n = (double)cos((((double)i-(double)Vad.Du/10))/180*3.1415926);
			fit2[i]= Vad.CF0-n*Vad.U;
			fit[i] = fit2[i]/100;

			x = 100+ i/45*80 + i%45*(((double)80/(double)45));    //这个算的准不准确有很大影响

			if((fit[i]>= -0.000001) && (fit[i] <= 0.000001))
			{
				y = 298;
			}
			else 
			{
				y = 298 - (fit[i]/10*50);
			}
			if ((-y) > Max_Vel)
				Max_Vel = -y;
			temp_array2->push_back(osg::Vec3(x, -y,0.51));
		}
	}
	pLine->SetBTransform(false);
	pLine->setGlenum(osg::PrimitiveSet::LINES);
	pLine->SetCoordinateTransform(layer->CoordTrans());
	pLine->SetOsgArray(temp_array2);
	pLine->SetOsgColorArray(*temp_color_array);
	pLine->SetParentLayer(layer);
	pMG->insert(Layer::MgoGeometryPair(pMG->size(),pLine));
	//m_ID++;

	//画离散的点 360个

	for(int i=0;i<1/*NumbersOfStruct*/;i++)
	{

		VAD_ELEMENTS Vad=((VAD_ELEMENTS *)p)[i];
		int yposition;
		int xposition;
		for(int i = 0;i<360 ;i++)
		{
			osg::ref_ptr <osg::Vec3Array> temp_array2 = new osg::Vec3Array  ;//数据数组
			osg::ref_ptr <osg::Vec4Array> temp_color_array = new osg::Vec4Array;//颜色数组

			if(Vad.Vel[i] == -32767)
			{
				continue;
			}
			xposition = 100+ i/45*80 + i%45*(((double)80/(double)45));    //这个算的准不准确有很大影响
			yposition = 298 - (Vad.Vel[i]/100/10*50) - (Vad.Vel[i]/100%10*(50/10));
			//判断是否多普勒速度最大值
			

			temp_array2->push_back(osg::Vec3(xposition-1,-yposition-1,0.51));
			temp_array2->push_back(osg::Vec3(xposition+1,-yposition-1,0.51));
			temp_array2->push_back(osg::Vec3(xposition+1,-yposition+1,0.51));
			temp_array2->push_back(osg::Vec3(xposition-1,-yposition+1,0.51));

			for (int h = 0 ; h < pColorTable->count() ; h++)
			{	
				float a = pColorTable->at(h)->fStart;
				float b = pColorTable->at(h)->fEnd;	
				if (Vad.RD[i]/100 >= a && Vad.RD[i]/100 <= b )
				{
					osg::Vec4 tempcolor =  osg::Vec4(pColorTable->at(h)->nRed /255.0,pColorTable->at(h)->nGreen /255.0,pColorTable->at(h)->nBlue /255.0,1);
					temp_color_array->push_back(tempcolor);
					temp_color_array->push_back(tempcolor);
					temp_color_array->push_back(tempcolor);
					temp_color_array->push_back(tempcolor);
					// memDC.DrawPoint(xposition,yposition);
					break;
				}
				else if (-32767 == Vad.RD[i]/100 )
				{
					osg::Vec4 tempcolor =  osg::Vec4(128/255.0,0,128/255.0,1);
					temp_color_array->push_back(tempcolor);
					temp_color_array->push_back(tempcolor);
					temp_color_array->push_back(tempcolor);
					temp_color_array->push_back(tempcolor);
				}
				  // Vad.RD[i]=-32767,用特殊颜色表示Vad.Vel[i]
			}
			if (temp_color_array->size() == 0)
			{
				temp_color_array->push_back(osg::Vec4(128/255.0,0,128/255,1));
				temp_color_array->push_back(osg::Vec4(128/255.0,0,128/255,1));
			}
			goQuadrilateral* gpl = new goQuadrilateral;
			gpl->SetBTransform(false);
			gpl->setGlenum(osg::PrimitiveSet::QUADS);
			gpl->SetCoordinateTransform(layer->CoordTrans());
			gpl->SetOsgArray(temp_array2);
			gpl->SetOsgColorArray(*temp_color_array);
			gpl->SetParentLayer(layer);
			pMG->insert(Layer::MgoGeometryPair(pMG->size(),gpl));
			//m_ID++;
			//temppoly->SetColorArray();
			//container->InsertRenderable(temppoly);
		}
	}
}

void CProductStructDraw_VAD::CreatAttributeList(Layer* layer,CProductRead * pProductData,QString fileName)
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
	layer->InsertLayerAttribute(QString("%1m").arg(tempStr).toStdString(),std::string("海拔高度："),"String");

	QString strdu = QString::number(pAttr->longitude/10000);
	int ntemp = (pAttr->longitude%10000);
	int fen = (int)ntemp*60*0.0001;
	int ns =(  ((double)(ntemp*0.0001*60)-(double)fen)*60     );
	QString strfen = QString::number(fen);
	QString strS = QString::number(ns);
	layer->InsertLayerAttribute((QString("%1°%2′%3″ E").arg(strdu).arg(strfen).arg(strS)).toStdString(),std::string("经度："),"String");

	strdu = QString::number(pAttr->latitude/10000);
	int temp = (int)(pAttr->latitude%10000);
	fen = temp*0.0001*60;
	strfen = QString::number(fen);
	ns = (int)(  ((double)(temp*0.0001*60)-(double)fen)*60     );
	strS = QString::number(ns);
	layer->InsertLayerAttribute((QString("%1°%2′%3″ N").arg(strdu).arg(strfen).arg(strS)).toStdString(),std::string("纬度："),"String");

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


	if (pAttr->vcp_mode == 0)
	{
		tempStr =QString(tr("VCP 11"));
	}
	else if (pAttr->vcp_mode == 1)
	{
		tempStr =QString(tr("VCP 21"));
	}
	else if (pAttr->vcp_mode == 2)
	{
		tempStr =QString(tr("VCP 31"));
	}
	else if (pAttr->vcp_mode == 3)
	{
		tempStr =QString(tr("VCP 32"));
	}
	layer->InsertLayerAttribute(tempStr.toStdString(),std::string("体扫模式："),"String");

	tempStr = QString::number(Max_Vel,'f',2);
	layer->InsertLayerAttribute(QString("%1 m/s").arg(tempStr).toStdString(),std::string("多普勒速度最大值："),"String");

	float hight = (double)pAttr->HeightOfElevation*0.1;
	tempStr = QString::number(hight,'f',2);
	layer->InsertLayerAttribute(QString("%1 m").arg(tempStr).toStdString(),std::string("VAD高度："),"String");

	
	delete pAttr;
	pAttr = NULL;
}

void CProductStructDraw_VAD::GetStationNameAndPName(QString &fileName,QString &stationName,
	QString &productName)
{
	QStringList stationInfo = FileNameAnalyes(fileName);
	CRadReadStationFile pRead ;
	QString appConfigPth = QCoreApplication::applicationDirPath().replace("/bin","") + "/config/";
	pRead.ReadFile(appConfigPth+"CINRAD_classB.dat");
	RADSTATION_HASH stationHash = pRead.RadStaion();

	//qgsLayer = new QgsVectorLayer(appConfigPth, stationInfo, "VAD");

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
	ProductNameMap.insert(QString(tr("VAD")),QString(tr("速度方位显示(VAD)")));
	productName = ProductNameMap.value(stationInfo.at(1));
}

QStringList CProductStructDraw_VAD::FileNameAnalyes( QString fName )
{
	QStringList attribute = QDir(fName).dirName().split("_");
	QString str_time = attribute[3].left(attribute[3].indexOf("."));
	attribute[2].append(str_time);
	return attribute;
}


QImage CProductStructDraw_VAD::DrawThumbs(CProductRead* lpProduct,P_COLORTABLE_VECTOR pColorTable,float imsize)
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
	for(int i=0;i<1;i++)
	{
		VAD_ELEMENTS Vad=((VAD_ELEMENTS *)p)[i];
		int x = 0;
		int y = 0;
		//计算360个点
		double fit[360];
		double fit2[360];             //用double型提高精度
		for(int i = 0;i<360 ;i++)
		{
			if(Vad.CF0 == -32768)
			{
				continue;
			}
			//FIT=CF0-cos(Az-Du)*U;  （单位：0.01m/s）
			double n = (double)cos((((double)i-(double)Vad.Du/10))/180*3.1415926);
			fit2[i]= Vad.CF0-n*Vad.U;
			fit[i] = fit2[i]/100;

			x = 100+ i/45*80 + i%45*(((double)80/(double)45));    //这个算的准不准确有很大影响

			if((fit[i]>= -0.000001) && (fit[i] <= 0.000001))
			{
				y = 298;
			}
			else 
			{
				y = 298 - (fit[i]/10*50);
			}
			if ((-y) > Max_Vel)
				Max_Vel = -y;
			value = qRgb(247, 91, 20);
			image.setPixel((x-100)/rule+5,85/2+(y-200)/rule,value);
		}
	}

	//画离散的点 360个

	for(int i=0;i<1/*NumbersOfStruct*/;i++)
	{

		VAD_ELEMENTS Vad=((VAD_ELEMENTS *)p)[i];
		int yposition;
		int xposition;
		for(int i = 0;i<360 ;i++)
		{
			if(Vad.Vel[i] == -32767)
			{
				continue;
			}
			xposition = 100+ i/45*80 + i%45*(((double)80/(double)45));    //这个算的准不准确有很大影响
			yposition = 298 - (Vad.Vel[i]/100/10*50) - (Vad.Vel[i]/100%10*(50/10));
			//判断是否多普勒速度最大值

			for (int h = 0 ; h < pColorTable->count() ; h++)
			{	
				float a = pColorTable->at(h)->fStart;
				float b = pColorTable->at(h)->fEnd;	
				if (Vad.RD[i]/100 >= a && Vad.RD[i]/100 <= b )
				{
					QVector4D tempcolor =  QVector4D(pColorTable->at(h)->nRed /255.0,pColorTable->at(h)->nGreen /255.0,pColorTable->at(h)->nBlue /255.0,1);
					
					value = qRgb(tempcolor.x()*255, tempcolor.y()*255, tempcolor.z()*255);
					image.setPixel((xposition-100)/rule+5,85/2+(yposition-200)/rule,value);
					break;
				}
				else if (-32767 == Vad.RD[i]/100 )
				{
					QVector4D tempcolor =  QVector4D(128/255.0,0,128/255.0,1);
					
					value = qRgb(tempcolor.x()*255, tempcolor.y()*255, tempcolor.z()*255);
					image.setPixel((xposition-100)/rule+5,85/2+(yposition-200)/rule,value);
				}
				// Vad.RD[i]=-32767,用特殊颜色表示Vad.Vel[i]
			}
		}
	}
	value = qRgb(128, 128, 128);
	for(int i = 5;i<93;i++)
	{
		image.setPixel(5,i,value);
		image.setPixel(30,i,value);
		image.setPixel(52,i,value);
		image.setPixel(73,i,value);
		image.setPixel(i,30,value);
		image.setPixel(i,60,value);
		image.setPixel(i,92,value);
	}

	return image;
}
