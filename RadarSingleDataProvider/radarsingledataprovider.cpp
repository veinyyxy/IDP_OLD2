#include "radarsingledataprovider.h"
#include <QtCore/QCoreApplication>
#include <QtCore/QDateTime>
#include "goGeometry.h"
#include "goLinearRing.h"
#include "goLineString.h"
#include <geos/geom/CoordinateArraySequence.h>
#include "InterpolationRadarData.h"
#include "RadarMarchingCubesImp.h"
#include "VolumeImagePrepar.h"
#include <QDebug>
#include "Map.h"
#include "DrawLrmImageImp.h"
#include "LrmAlg.h"
#include "AreaStatistic.h"
#include "SymbolLibraryInterfaceImp.h"
#include "RadarSingleLegend.h"
//////////////////////////////////////////////////////
#include "InterpolationAlgorithm.h"
/////////////////////////////////////////////////////

#define  racial 10000.000
#define  km  1000.000


RadarSingleDataProvider* g_psRadarProvider = 0;

extern "C"
{
	DataProvider Q_DECL_EXPORT * DataProviderPluginMain()
	{
		g_psRadarProvider = new RadarSingleDataProvider;
		return g_psRadarProvider;
	}
}

extern "C"
{
	bool Q_DECL_EXPORT MineData(std::string& stFileName)
	{
		QString strFileName;
		strFileName = QString::fromStdString(stFileName);
		QFileInfo fileInfo;
		fileInfo.setFile(strFileName);
		QString filename = fileInfo.fileName();

		QString strSuffix("bz2");
		QString strSuffix1 = fileInfo.suffix();
		if(strSuffix1 != strSuffix)
			return false;
		else if (filename.contains(QString::fromLocal8Bit("MVIL"))||filename.contains(QString::fromLocal8Bit("MVILD"))||filename.contains(QString::fromLocal8Bit("BREF"))||filename.contains(QString::fromLocal8Bit("CREF"))) 
		{
			return false;
		}
		else if (filename.contains(QString::fromLocal8Bit("VWP"))||filename.contains(QString::fromLocal8Bit("VAD"))||filename.contains(QString::fromLocal8Bit("SBT"))
			||filename.contains(QString::fromLocal8Bit("EVP"))||filename.contains(QString::fromLocal8Bit("_M_"))||filename.contains(QString::fromLocal8Bit("SCI"))||filename.contains(QString::fromLocal8Bit("HI")))
		{
			return false;
		}
		else
			return true;
	}
}

RadarSingleDataProvider * RadarSingleDataProvider::m_pSingleProvider = 0;

RadarSingleDataProvider::RadarSingleDataProvider():pDrawProduct(0),m_btransform(true),
	pSingleGdata(0),gData(0),m_MaxValue(-32767),m_nParam(100), m_ExtendInterface(0), m_MarchinCubesExtend(0),m_iLayerNO(0)
{
	m_pSymbolLibraryInterface = new SymbolLibraryInterfaceImp;
}

RadarSingleDataProvider::~RadarSingleDataProvider()
{
	gData->ClearAll();
	if (!m_pSymbolLibraryInterface)
	{
		delete m_pSymbolLibraryInterface;
		m_pSymbolLibraryInterface = NULL;
	}
	
	if (NULL != pDrawProduct)
	{
		delete pDrawProduct;
		pDrawProduct = NULL;
	}

}

RadarSingleDataProvider * RadarSingleDataProvider::Instance()
{
	if (!m_pSingleProvider)
	{
		m_pSingleProvider = new RadarSingleDataProvider();
	}
	return m_pSingleProvider;
}

bool RadarSingleDataProvider::LoadFile( const std::string& filename )
{
	m_fileName = QString(filename.c_str());
	bool mosiacRequest = false;
	if (m_fileName.contains("VWP")||m_fileName.contains("VAD")||m_fileName.contains("SBT")
		||m_fileName.contains("EVP")||m_fileName.contains("_M_")||m_fileName.contains("SCI")||m_fileName.contains("HI"))
	{
		return 0;
	}
	GDataFiles* gDataFile = NULL;
	gData = new GenerationData();
	gDataFile  = (GDataFiles*)gData;

	if(string::npos == filename.find("MVIL") || string::npos == filename.find("MVILD") || string::npos == filename.find("BREF") || string::npos == filename.find("CREF"))	
		pDrawProduct = (cDrawProduct *)gData;//去掉这句的话右边的属性会出错 
	else
		return false;


	int ret = gDataFile->ReadFrom((char *)filename.c_str());
	if(ret!=0)
	{
		return 0;
	}

	m_layers = GetCountOfLayer();

	proj = NULL;
	osg::Vec3 p = stationPosition();
	if (m_btransform )
	{
		proj = new RadProjTransform;
		QString destproj = "RADARPROJ" ;
		destproj.append(" ");
		destproj.append(QString::number(p.y()));
		destproj.append(" ");
		destproj.append( QString::number(p.x()));
		destproj.append(" ");
		destproj.append(QString::number(p.z()));

		proj->setSourceCRS(destproj.toStdString());
		proj->setSourceProjName("雷达中心兰勃特投影");
		proj->setDestCRS("+proj=longlat +ellps=WGS84 +datum=WGS84 +no_defs");
		proj->setDesProjName("无投影");
		proj->Elevation(true);
	}


	//RadProjTransform *proj2 = new RadProjTransform;
	//QString destproj = "RADARPROJ" ;
	//destproj.append(" ");
	//destproj.append(QString::number(p.y()));
	//destproj.append(" ");
	//destproj.append( QString::number(p.x()));
	//destproj.append(" ");
	//destproj.append(QString::number(p.z()));

	////proj->setSourceCRS(destproj.toStdString());
	////proj->setSourceProjName("雷达中心兰勃特投影");
	////proj->setDestCRS("+proj=longlat +ellps=WGS84 +datum=WGS84 +no_defs");
	////proj->setDesProjName("无投影");
	//proj2->setDestCRS(destproj.toStdString());
	//proj2->setDesProjName("雷达中心兰勃特投影");
	//proj2->setSourceCRS("+proj=longlat +ellps=WGS84 +datum=WGS84 +no_defs");
	//proj2->setSourceProjName("无投影");
	//proj2->Elevation(true);

	if (!InitColorTable())
	{
		return false;
	}
	//double dfMinus,dfFreq,dfTime;
	//LARGE_INTEGER litmp;
	//LONGLONG Qpart1,Qpart2,Useingtime;
	//QueryPerformanceFrequency(&litmp);//取得高精度运行计数器的频率f,单位是每秒多少次（n/s），
	//dfFreq = (double)litmp.QuadPart;
	//QueryPerformanceCounter(&litmp);//取得高精度运行计数器的数值
	//Qpart1 = litmp.QuadPart; //开始计时
	for (int i = 0 ; i< m_layers;i++)
	{
		if (0 == i)
		{
			pSingleGdata = (SingleDataProvider*)gData;
			double an = pSingleGdata->GetRadiaElevation(i)*0.01;
			proj->setProductAngle(an);
			osg::StateSet* pStatSet = m_pParentLayer->GetLayerNode()->getOrCreateStateSet();
			m_pParentLayer->InsertLayerAttribute(m_ColorTable,"ColorTable","ColorList");
			CoordinateTransform * cood = m_pParentLayer->CoordTrans()/*GetParentMap()->SetCoordinateTransform(proj2)*/;
			cood->setCenter(p.y(), p.x());
			/*DumpLayers(m_pParentLayer,proj);*/
			ProductDataProvider(gData,m_pParentLayer,proj,i);
			
			QString layerName = "第1仰角层";
			pStatSet->setRenderBinDetails(m_layers + 3 - i,layerName.toStdString());
			pStatSet->setMode(GL_BLEND,osg::StateAttribute::ON);
			pStatSet->setMode(GL_DEPTH_TEST,osg::StateAttribute::OFF);

			RadarVectorLayer* vLayer = new RadarVectorLayer();
			vLayer->SetDataProvider(0);
			vLayer->CoordTrans(m_pParentLayer->CoordTrans());

			proj->setProductAngle(0);
			DrawCircle(vLayer,0,0,p.z(), GetSingleProductRadius(0),1000.0,proj);
			DumpSingleAttr(vLayer,0);
			DumpSingleAttr(m_pParentLayer,i);
			//vLayer->CreateLayerData();

			Map * mp = m_pParentLayer->GetParentMap();

			vLayer->SetParentMap(mp);
			layerName = "距离圈" ;
			vLayer->SetSymbolLibrary(m_pSymbolLibraryInterface);
			mp->AddLayer(layerName.toStdString(),vLayer);
		}
		else
		{
			RadarVectorLayer* vLayer0 = new RadarVectorLayer();
			vLayer0->SetDataProvider(0);
			osg::StateSet* pStatSet = vLayer0->GetLayerNode()->getOrCreateStateSet();
			vLayer0->InsertLayerAttribute(m_ColorTable,"ColorTable","ColorList");
			vLayer0->CoordTrans(m_pParentLayer->CoordTrans());
			double an = pSingleGdata->GetRadiaElevation(i) *0.01;
			proj->setProductAngle(an);
			ProductDataProvider(gData,vLayer0,proj,i);
			DumpSingleAttr(vLayer0,i);
			//vLayer0->CreateLayerData();
			Map * mp = m_pParentLayer->GetParentMap();
			vLayer0->SetParentMap(mp);
			QString layerName = QString("第%1仰角层").arg(i+1);;
			pStatSet->setRenderBinDetails(m_layers + 3 - i, layerName.toStdString());
			pStatSet->setMode(GL_BLEND,osg::StateAttribute::ON);
			pStatSet->setMode(GL_DEPTH_TEST,osg::StateAttribute::OFF);

			vLayer0->SetSymbolLibrary(m_pSymbolLibraryInterface);
			mp->AddLayer(layerName.toStdString(),vLayer0);
		}
	}
	//QueryPerformanceCounter(&litmp);//取得高精度运行计数器的数值
	//Qpart2 = litmp.QuadPart; //终止计时
	//dfMinus = (double)(Qpart2 - Qpart1)/1000000;//计算计数器值
	//qDebug()<< "step dump useing:"<<dfMinus;
	//-------------------初始化雷达扩展----------------
	InterpolationRadarData* pIR = new InterpolationRadarData;
	m_ExtendInterface = pIR;
	if(m_ExtendInterface)
	{
		DataProvider::m_ExtendInterface.insert(DataProvider::INDEX_INTERFACE_PAIR(0, m_ExtendInterface));
	}

	RadarMarchingCubesImp* pRMCI = new RadarMarchingCubesImp((GenerationData*)gData);
	m_MarchinCubesExtend = pRMCI;
	pRMCI->SetColorTable(*m_ColorTable);
	if(m_MarchinCubesExtend)
	{
		DataProvider::m_ExtendInterface.insert(DataProvider::INDEX_INTERFACE_PAIR(1, m_MarchinCubesExtend));
	}

	LrmAlg* pLrmAlg = new LrmAlg;
	pLrmAlg->Initialize(pDrawProduct, GetColorTable());
	DrawLrmImage* pDrawImage = new DrawLrmImageImp(pLrmAlg);

	if(pDrawImage)
	{
		DataProvider::m_ExtendInterface.insert(DataProvider::INDEX_INTERFACE_PAIR(2, pDrawImage));
	}

	VolumeImagePrepar* pVolumeImg = new VolumeImagePrepar;
	//pVolumeImg->SetColorTable(GetColorTable());
	pVolumeImg->SetColorHash(m_pColorValueIndexHash);
	pVolumeImg->SetDrawProdunt(pDrawProduct);
	if (pVolumeImg)
	{
		DataProvider::m_ExtendInterface.insert(DataProvider::INDEX_INTERFACE_PAIR(3,pVolumeImg));
	}
	//------------------------------------------------------
	pIR->SetDrawProdunt(pDrawProduct);
	//InitProductNameTable();
	//m_radius = GetSingleProductRadius(0);
	return true;
}

//QStringList RadarSingleDataProvider::FileNameAnalyes( QString fName )
//{
//	QMap<QString,QStringList> nameMap;
//	QString tempstring;
//	QStringList tempList;
//
//	tempstring = "RADA_CHN_DOR_L3_ST_QC-R";
//	tempList.append("1");//产品名
//	tempList.append("2");//站名
//	tempList.append("4");//时间
//	nameMap.insert(tempstring,tempList);
//
//	tempstring = "RADA_CHN_DOR_L3_ST_QC-V";
//	nameMap.insert(tempstring,tempList);
//
//	tempstring = "RADA_CHN_DOR_L3_ST_QC-W";
//	nameMap.insert(tempstring,tempList);
//
//	tempstring = "RADA_CHN_DOR_L3_ST_QC-CR";
//	nameMap.insert(tempstring,tempList);
//
//	tempstring = "RADA_CHN_DOR_L3_ST_QC-CRH";
//	nameMap.insert(tempstring,tempList);
//
//	tempstring = "RADA_CHN_DOR_L3_ST_QC-CAP";
//	nameMap.insert(tempstring,tempList);
//
//	tempstring = "RADA_CHN_DOR_L3_ST_QC-ET";
//	nameMap.insert(tempstring,tempList);
//
//	tempstring = "RADA_CHN_DOR_L3_ST_QC-VIL";
//	nameMap.insert(tempstring,tempList);
//
//	tempstring = "RADA_CHN_DOR_L3_ST_QC-VILD";
//	nameMap.insert(tempstring,tempList);
//
//	tempstring = "RADA_CHN_DOR_L3_ST_QC-HBR";
//	nameMap.insert(tempstring,tempList);
//
//	tempstring = "RADA_CHN_DOR_L3_ST_QC-QPR";
//	nameMap.insert(tempstring,tempList);
//
//	tempstring = "RADA_CHN_DOR_L3_ST_QC-OHP";
//	nameMap.insert(tempstring,tempList);
//
//	tempstring = "RADA_CHN_DOR_L3_ST_QC-WHP";
//	nameMap.insert(tempstring,tempList);
//
//	tempstring = "RADA_CHN_DOR_L3_ST_QC-THP";
//	nameMap.insert(tempstring,tempList);
//
//	tempstring = "RADA_CHN_DOR_L3_ST_QC-SHP";
//	nameMap.insert(tempstring,tempList);
//
//	tempstring = "RADA_CHN_DOR_L3_ST_QC-SCI";
//	nameMap.insert(tempstring,tempList);
//
//	tempstring = "RADA_CHN_DOR_L3_ST_QC-SCT";
//	nameMap.insert(tempstring,tempList);
//
//	tempstring = "RADA_CHN_DOR_L3_ST_QC-SBT";
//	nameMap.insert(tempstring,tempList);
//
//	tempstring = "RADA_CHN_DOR_L3_ST_QC-VAD";
//	nameMap.insert(tempstring,tempList);
//
//	tempstring = "RADA_CHN_DOR_L3_ST_QC-VWP";
//	nameMap.insert(tempstring,tempList);
//
//	tempstring = "RADA_CHN_DOR_L3_ST_QC-EVP";
//	nameMap.insert(tempstring,tempList);
//
//	tempstring = "RADA_CHN_DOR_L3_ST_QC-TSR";
//	nameMap.insert(tempstring,tempList);
//
//	tempstring = "RADA_CHN_DOR_L3_ST_QC-OSR";
//	nameMap.insert(tempstring,tempList);
//
//	tempstring = "RADA_CHN_DOR_L3_ST_QC-WSR";
//	nameMap.insert(tempstring,tempList);
//
//	tempstring = "RADA_CHN_DOR_L2_O_UF_QC";
//	tempList.clear();
//	tempList.append("1");//产品名
//	tempList.append("3");//站名
//	tempList.append("4");//时间
//	nameMap.insert(tempstring,tempList);
//
//	tempstring = "RADA_CHN_DOR_L3_ST_QC";
//	tempList.clear();
//	tempList.append("1");//产品名
//	tempList.append("2");//站名
//	tempList.append("4");//时间
//	nameMap.insert(tempstring,tempList);
//
//	tempstring = "RADA_CHN_DOR_L2_O_UF";
//	tempList.clear();
//	tempList.append("3");//产品名
//	tempList.append("1");//站名
//	tempList.append("4");//时间
//	nameMap.insert(tempstring,tempList);
//
//	tempstring = "RADA_CHN_DOR_L3_MST_QC";
//	tempList.clear();
//	tempList.append("1");//产品名
//	tempList.append("0");//站名
//	tempList.append("2");//时间
//	nameMap.insert(tempstring,tempList);
//
//	tempstring = "RADA_CHN_DOR_L3_MST_QC-CREF";
//	nameMap.insert(tempstring,tempList);
//
//	tempstring = "RADA_CHN_DOR_L3_MST_QC-MVIL";
//	nameMap.insert(tempstring,tempList);
//
//	tempstring = "RADA_CHN_DOR_L3_MST_QC-MVILD";
//	nameMap.insert(tempstring,tempList);
//
//	QStringList attribute = QDir(fName).dirName().split("-");
//	if (QStringList() != nameMap.value(attribute.at(0)))
//	{
//
//		//QFileInfo info(fName);
//		//if(info.suffix() == "bin")
//		//{
//		//	QString temp = attribute.last();
//		QStringList templist = attribute;
//		QStringList temp = nameMap.value(attribute.at(0));
//		int i = temp.at(0).toInt();//产品名
//		int j = temp.at(1).toInt();//站名
//		int k = temp.at(2).toInt();//时间
//		attribute.clear();
//		attribute.append(templist.at(j));
//		attribute.append(templist.at(i));
//		QString tempstr = templist[k].left(templist[k].indexOf("."));
//		attribute.append(tempstr);
//
//	}else 
//	{
//		QStringList temlist = attribute[0].split("_");
//		QString str_time = temlist[3].left(temlist[3].indexOf("."));
//		attribute.clear();
//		attribute = temlist;
//		attribute[2].append(str_time);
//	}
//
//	return attribute;
//}

void RadarSingleDataProvider::ProductDataProvider( GenerationData* gData,Layer* layer,RadProjTransform *proj,int requestLayer  )
{
	m_MaxValue = 0;

	pSingleGdata = (SingleDataProvider*)gData;

	//	m_edataType = RADAR_SINGLE;
	pSingleGdata->ProdDataProvider(requestLayer,m_MaxValue,m_pColorValueIndexHash, *layer,proj);
}

bool RadarSingleDataProvider::InitColorTable()
{
	QString appConfigPth = QCoreApplication::applicationDirPath().remove("/bin") + "/config/";
	 
	COLORTABLE_HASH        m_hColorTableHash;
	int productcode = pSingleGdata->ProductCode();
	xml_reader.ReadFile(appConfigPth+"/BasicInfo/m_single_radar_color.xml");

	m_hColorTableHash = (xml_reader.ColorTable());

	m_ColorTable =  m_hColorTableHash.value(pSingleGdata->ProductCode());

	if (m_ColorTable == NULL)
	{
		m_ColorTable = m_hColorTableHash.value(251);
		return false;
	}
	if(254==productcode||255==productcode||301==productcode)
	{
		proAttr pAttributeHaHa;
		if (pDrawProduct != NULL)
		{
			pDrawProduct->ReadInfo(&pAttributeHaHa,0);
		}
		//-------------用最大不模糊速度修改  速度产品颜色表fstart fend值
		if (pAttributeHaHa.pro_code == 254 || pAttributeHaHa.pro_code == 255 || pAttributeHaHa.pro_code == 301) //301-IW-逆风区
		{
			m_nParam = pAttributeHaHa.param1;
		}
		for (int ii = 0; ii<m_ColorTable->count();ii++){
			m_ColorTable->at(ii)->fStart=(m_ColorTable->at(ii)->fStart)/100*m_nParam;
			m_ColorTable->at(ii)->fEnd=(m_ColorTable->at(ii)->fEnd)/100*m_nParam;
			m_hColorTableHash.value(pSingleGdata->ProductCode())->at(ii)->fStart=m_ColorTable->at(ii)->fStart;
			m_hColorTableHash.value(pSingleGdata->ProductCode())->at(ii)->fEnd=m_ColorTable->at(ii)->fEnd;
		}
	}

	for (int i = 0; i<m_ColorTable->count();i ++)
	{
		P_COLORNODE_PRODUCT color = m_ColorTable->at(i);	
		double fstart = color->fStart;
		int key;float step = 0.0;

		if (251==productcode||252==productcode||258==productcode||262==productcode||263==productcode||280==productcode||282==productcode||283==productcode
			||284==productcode||288==productcode||278==productcode||291==productcode||292==productcode||294==productcode||293==productcode)
		{
			step = 5.0;
			key= (int)(fstart/step);
		}else if (254==productcode||255==productcode||301==productcode)
		{
			step = 15*m_nParam/100;
			key = (int)(fstart/step );
		}else if (256==productcode||257==productcode)
		{
			step = 1;
			key = (int)(fstart/ step);
		}else if (267 == productcode)
		{
			step = 0.5;
			key = (int)(fstart/ step);
		}else if (269 == productcode)
		{
			step = 2;
			key = (int)(fstart/ step);
		}else 
		{
			key = SingleDataProvider::GetColorIndexKey(productcode,fstart);
		}
		m_pColorValueIndexHash.insert(key,QVector4D(color->nRed/255.0 ,color->nGreen/255.0,color->nBlue/255.0,1.0));
	}
	//出使唤符号库中的色卡
	RadarSingleLegend* pLegend = new RadarSingleLegend;

	pLegend->SetColorTable(m_ColorTable);

	m_pSymbolLibraryInterface->AddLegend(pLegend);

	return true;
}

void RadarSingleDataProvider::InitProductNameTable()
{
	QString appConfigPth = QCoreApplication::applicationDirPath().replace("/Bin","") + "/Config/";
	CRadReadProduct_XML temp_reader;
	temp_reader.ReadFile(appConfigPth+"/BasicInfo/product_name_map.xml");
	P_PRODUCT tempPro;

	tempPro =  temp_reader.ProductTable().value(pSingleGdata->ProductCode());

	if (tempPro != NULL)
	{	
		m_ProductNameTable.nID = tempPro->nID;
		m_ProductNameTable.strAbb = tempPro->strAbb;
		m_ProductNameTable.strName = tempPro->strName;
		m_ProductNameTable.strUnit = tempPro->strUnit;
	}
}

int RadarSingleDataProvider::GetCountOfLayer()
{
	pSingleGdata = (SingleDataProvider*)gData;
	return pSingleGdata->GetCountOfLayer();
}

osg::Vec3 RadarSingleDataProvider::stationPosition()
{
	
	proAttr pAttributeHaHa;
	if (pDrawProduct != NULL)
	{
		pDrawProduct->ReadInfo(&pAttributeHaHa,0);
	}
	return osg::Vec3(pAttributeHaHa.latitude/10000.0,pAttributeHaHa.longitude /10000.0,(float)pAttributeHaHa.altitude*0.1);

	//CRadReadStationFile pRead ;
	//QString appConfigPth = QCoreApplication::applicationDirPath().remove("/bin") + "/config/";
	//pRead.ReadFile(appConfigPth+"CINRAD_classB.dat");
	//RADSTATION_HASH stationHash = pRead.RadStaion();
	//QStringList stationInfo = FileNameAnalyes(m_fileName);
	//QString id = stationInfo.at(0);
	//P_CINRAD_STATIONINFO info = stationHash.value(id);
	//if (info == NULL
	//{
	//	return osg::Vec3(0,0,0);
	//}
	//
	//return osg::Vec3((info->nLat)/racial,(info->nLon)/racial,(info->nHei)/10);
}

double RadarSingleDataProvider::GetSingleProductRadius( int ilayer )
{
	return pSingleGdata->GetDataRadius(ilayer);
}

int RadarSingleDataProvider::GetSingleProductCode()
{
	return pSingleGdata->ProductCode();
}

void RadarSingleDataProvider::GetStationNameAndPName(QString &fileName,QString &stationName,
	QString &productName)
{
	CRadReadStationFile pRead ;
	QString appConfigPth = QCoreApplication::applicationDirPath().replace("/bin","") + "/config/";
	pRead.ReadFile(appConfigPth+"CINRAD_classB.dat");
	RADSTATION_HASH stationHash = pRead.RadStaion();

	P_CINRAD_STATIONINFO info = stationHash.value(fileName);
	if (info == 0)
	{
		stationName = "";
	}
	else
	{
		stationName = info->cName;
	}
}
void RadarSingleDataProvider::DumpSingleAttr( Layer *layer,int requestlayer )
{
	proAttr pAttributeHaHa;
	if (pDrawProduct != NULL)
	{
		pDrawProduct->ReadInfo(&pAttributeHaHa,requestlayer);
	}
	m_productcode = pAttributeHaHa.pro_code;
	//-------------用最大不模糊速度修改  速度产品颜色表fstart fend值
	if (pAttributeHaHa.pro_code == 254 || pAttributeHaHa.pro_code == 255 || pAttributeHaHa.pro_code == 301) //301-IW-逆风区
	{
		m_nParam = pAttributeHaHa.param1;
	}
	//-------------end
	QString StrUnit = GetDataUnit();//产品单位
	
	QString tempStr;

	QString StrStationName,strProName;
	GetStationNameAndPName(QString(pAttributeHaHa.site_code),StrStationName,strProName);
	strProName = GetProductNameByShortName(pAttributeHaHa.pro_code);
	//StrStationName = QString(pAttributeHaHa.staName);
	layer->InsertLayerAttribute(strProName.toStdString(),std::string("产品名称："),"String");
	layer->InsertLayerAttribute(StrStationName.toStdString(),std::string("雷达站点："),"String");

	QDate tDate(1969,12,31);
	tDate = tDate.addDays(static_cast<int>(pAttributeHaHa.scan_date));
	tempStr = tDate.toString("yyyy-MM-dd");
	if (m_fileName.contains( "OHP")||m_fileName.contains( "WHP")||m_fileName.contains( "THP")
		||m_fileName.contains( "SHP"))
	{
		layer->InsertLayerAttribute(tempStr.toStdString(),std::string("降水累积日期："),"String");
	}else
	{
		layer->InsertLayerAttribute(tempStr.toStdString(),std::string("体扫开始日期："),"String");
	}

	QTime tTime(0,0,0,0);
	tTime = tTime.addMSecs(pAttributeHaHa.scan_time);
	tempStr = tTime.toString("hh:mm:ss");

	if (m_fileName.contains( "OHP")||m_fileName.contains( "WHP")||m_fileName.contains( "THP")
		||m_fileName.contains( "SHP"))
	{
		layer->InsertLayerAttribute(tempStr.toStdString(),std::string("降水累积结束时间："),"String");
	}else
	{
		layer->InsertLayerAttribute(tempStr.toStdString(),std::string("体扫开始时间："),"String");	
	}
	QDate tDateG(1969,12,31);
	tDateG = tDateG.addDays(pAttributeHaHa.generation_date);
	tempStr = tDateG.toString("yyyy-MM-dd");
	layer->InsertLayerAttribute(tempStr.toStdString(),std::string("产品生成日期："),"String");	

	QTime tTimeG(0,0,0,0);
	tTimeG = tTimeG.addMSecs(pAttributeHaHa.generation_time);
	tempStr = tTimeG.toString("hh:mm:ss");
	layer->InsertLayerAttribute(tempStr.toStdString(),std::string("产品生成时间："),"String");	

	tempStr = QString::number((float)pAttributeHaHa.altitude*0.1,'f',2);
	layer->InsertLayerAttribute(tempStr.toStdString(),std::string("海拔高度："),"String");

	tempStr = GetDataUnit();
	layer->InsertLayerAttribute(tempStr.toStdString(),std::string("单位："),"String");

	QString strdu = QString::number(pAttributeHaHa.longitude/10000);
	int ntemp = (pAttributeHaHa.longitude%10000);
	int fen = (int)ntemp*60*0.0001;
	int ns =(  ((double)(ntemp*0.0001*60)-(double)fen)*60     );
	QString strfen = QString::number(fen);
	QString strS = QString::number(ns);
	tempStr = QString( "%1°%2′%3″ E").arg(strdu).arg(strfen).arg(strS);
	layer->InsertLayerAttribute(tempStr.toStdString(),std::string("经度："),"String");	

	strdu = QString::number(pAttributeHaHa.latitude/10000);
	int temp = (int)(pAttributeHaHa.latitude%10000);
	fen = temp*0.0001*60;
	strfen = QString::number(fen);
	ns = (int)(  ((double)(temp*0.0001*60)-(double)fen)*60     );
	strS = QString::number(ns);
	tempStr = QString( "%1°%2′%3″ N").arg(strdu).arg(strfen).arg(strS);
	layer->InsertLayerAttribute(tempStr.toStdString(),std::string("纬度："),"String");	

	tempStr = QString( "维护");

	if (pAttributeHaHa.operation_mode == 1)
	{
		tempStr = QString( "晴空");
	}		
	if (pAttributeHaHa.operation_mode == 2)
	{
		tempStr = QString( "降水");
	}

	layer->InsertLayerAttribute(tempStr.toStdString(),std::string("工作模式："),"String");	

	double nwidth = GetSingleProductRadius(requestlayer)/1000.0;
	tempStr = QString::number(nwidth,'f',3);
	layer->InsertLayerAttribute((QString("%1Km").arg(tempStr)).toStdString(),std::string("有效范围："),"String");	

	if (pAttributeHaHa.vcp_mode == 0)
	{
		tempStr =QString( "VCP 11");
	}
	else if (pAttributeHaHa.vcp_mode == 1)
	{
		tempStr =QString( "VCP 21");
	}
	else if (pAttributeHaHa.vcp_mode == 2)
	{
		tempStr =QString( "VCP 31");
	}
	else if (pAttributeHaHa.vcp_mode == 3)
	{
		tempStr =QString( "VCP 32");
	}

	layer->InsertLayerAttribute(tempStr.toStdString(),std::string("体扫模式："),"String");	

	osg::Vec3 center = stationPosition();
	tempStr = QString::number(0);
	layer->InsertLayerAttribute(center,std::string("中心："),"osg::Vec3");	

	if (0.0 != pAttributeHaHa.elevation)
	{	
		tempStr = QString::number(pAttributeHaHa.elevation,'f',2);
		layer->InsertLayerAttribute(QString( "%1 deg").arg(tempStr).toStdString(),std::string("仰角："),"String");	
	}

	if (strProName.contains(QString( "CAP")))
	{
		double dh = (double)pAttributeHaHa.HeightOfElevation*0.1;
		tempStr = QString::number(dh,'f',2);
		layer->InsertLayerAttribute((QString( "%1 m").arg(tempStr)).toStdString(),std::string("CAPPI高度："),"String");	
	}


	tempStr = QString::number(m_MaxValue,'f',2);
	QString mav = QString::number(MaxvalHash.value(requestlayer)/100.0,'f',2);
	if (MaxvalHash.value(requestlayer) != 0.0)
	{
		tempStr = mav;
	}
	layer->InsertLayerAttribute((QString( "%1%2").arg(tempStr).arg(StrUnit)).toStdString(),std::string("最大值："),"String");	

	tempStr = GetCircleRange(GetSingleProductCode());
	layer->InsertLayerAttribute(tempStr.toStdString(),std::string("距离圈："),"String");	

	osg::Vec3 center1 = m_box.value("UP");
	layer->InsertLayerAttribute(center1,std::string("UP"),"osg::Vec3");	
	osg::Vec3 center2 = m_box.value("DOWN");
	layer->InsertLayerAttribute(center2,std::string("DOWN"),"osg::Vec3");
	osg::Vec3 center3 = m_box.value("LEFT");
	layer->InsertLayerAttribute(center3,std::string("LEFT"),"osg::Vec3");
	osg::Vec3 center4 = m_box.value("RIGHT");
	layer->InsertLayerAttribute(center4,std::string("RIGHT"),"osg::Vec3");
	QRectF rect;
	GetRect(rect);
	layer->InsertLayerAttribute(rect,std::string("边界"),"RectF");
	//m_fileDeclaration.push_back(StrUnit);

}

QString RadarSingleDataProvider::GetProductNameByShortName( int productcode )
{
	QMap<int,QString> ProductNameMap;
	{
		ProductNameMap.insert(251,QString("基本反射率(R)"));
		//ProductNameMap.insert(QString("VILT"),QString("零度层以上垂直累积液态水含量"));
		//ProductNameMap.insert(QString("VILB"),QString("零度层以下垂直累积液态水含量"));

		ProductNameMap.insert(252,QString("经质量控制的基本反射率(QR)"));

		ProductNameMap.insert(253,QString("用于组网的基本反射率(BREF)"));

		ProductNameMap.insert(254,QString("基本速度(V)"));

		ProductNameMap.insert(255,QString("经质量控制的基本速度(QV)"));

		ProductNameMap.insert(256,QString("基本谱宽(W)"));

		ProductNameMap.insert(257,QString("经质量控制的基本谱宽(QW)"));

		ProductNameMap.insert(258,QString("等高面反射率(CAP)"));

		//ProductNameMap.insert(QString( "RHI"),QString( "强度高度显示(RHI)"));

		//ProductNameMap.insert(QString( "VHI"),QString( "速度高度显示(VHI)"));

		//ProductNameMap.insert(QString( "RCS"),QString( "强度垂直剖面(RCS)"));

		ProductNameMap.insert(262,QString("混合扫描反射率(HBR)"));

		ProductNameMap.insert(263,QString("组合反射率(CR)"));

		ProductNameMap.insert(264,QString("最大反射率高度(CRH)"));;	

		ProductNameMap.insert(265,QString("回波顶高(ET)"));

		ProductNameMap.insert(266,QString("垂直累积液态水含量(VIL)"));

		ProductNameMap.insert(267,QString("垂直累积液态水密度(VILD)"));

		ProductNameMap.insert(268,QString("速度方位显示(VAD)"));

		ProductNameMap.insert(269,QString("VAD风廓线(VWP)"));

		ProductNameMap.insert(270,QString("降水率估计(QPR)"));

		ProductNameMap.insert(271,QString("一小时累积降水(OHP)"));

		ProductNameMap.insert(272,QString("两小时累积降水(WHP)"));

		ProductNameMap.insert(273,QString("三小时累积降水(THP)"));

		ProductNameMap.insert(274,QString("六小时累积降水(SHP)"));

		ProductNameMap.insert(277,QString("风暴识别(SCI)"));

		ProductNameMap.insert(280,QString("分层组合反射率(LRM)"));

		ProductNameMap.insert(281,QString("风暴轮廓识别跟踪(SBT)"));

		ProductNameMap.insert(283,QString("一小时强回波(OSR)"));

		ProductNameMap.insert(284,QString("两小时强回波(WSR)"));

		ProductNameMap.insert(285,QString("扩展速度方位显示廓线(EVP)"));

		ProductNameMap.insert(291,QString( "冰雹指数(HI)"));

		ProductNameMap.insert(292,QString( "中尺度气旋(M)"));

		ProductNameMap.insert(293,QString( "龙卷涡旋特征(TVS)"));

		ProductNameMap.insert(294,QString( "组合切变(CS)"));

		//ProductNameMap.insert(QString("IW"),QString( "逆风区(IW)"));

		ProductNameMap.insert(501,QString( "组网基本反射率(BREF)"));

		ProductNameMap.insert(502,QString( "组网组合反射率(CREF)"));

		ProductNameMap.insert(503,QString( "组网垂直累积液态水含量(MVIL)"));

		ProductNameMap.insert(504,QString( "组网垂直累积液态水密度(MVILD)"));

		ProductNameMap.insert(506,QString( "组网分层组合反射率(MLRM)"));

		//ProductNameMap.insert(QString("KTP"),QString( "雨量计订正三小时累计降水(KTP)"));

		//ProductNameMap.insert(QString("KWP"),QString( "雨量计订正两小时累计降水(KWP)"));
	}
	return ProductNameMap.value(productcode);
}

QString RadarSingleDataProvider::GetCircleRange(int ncode)
{
	QString tempStr;
	switch(ncode)
	{
	case 278:
	case 296:
	case 281:
	case 285:		
	case 268:		
	case 269:
	case 280:
	case 291:
	case 292:
	case 293: //by hdm
	case 277:
	case 258:
	default:
		tempStr = QString("50Km 30deg");
		break;
	}
	return tempStr;
}

bool RadarSingleDataProvider::DrawCircle( Layer * pLayer,double X,double Y,double h,double R,double Res,RadProjTransform *proj )
{
	int c_resolution = 1000/Res;
	int foot = 50*c_resolution*1000;
	int numofcircle = (int)((R*1000/foot)/Res);
	Layer::MgoGeometry* pMG = pLayer->GetMgoGeometry();
	int pNum = 0;
	//画圆
	for (int i = 1 ;i <numofcircle+1; i++)
	{
		goQuadrilateral *ln1 = new goQuadrilateral;
		
		osg::ref_ptr <osg::Vec3Array> temp_array = new osg::Vec3Array  ;//数据数组
		osg::ref_ptr <osg::Vec4Array> temp_color_array = new osg::Vec4Array;//颜色数组
		ln1->setGlenum(osg::PrimitiveSet::LINES);

		osg::ref_ptr <osg::Vec3Array> circlearray = new osg::Vec3Array;
		double r_p = foot*i;
		for (int p=1; p<=360; p++)
		{
			osg::Vec3 circle;
			float angle = p-1;
			circle.set(X+r_p * sin(M_PI / 180 * angle),Y+r_p * cos(M_PI / 180 * angle),h);
			circlearray->push_back(circle);
		}

		for (int m = 0 ;m<circlearray->size();m++)
		{
			osg::Vec3 src = circlearray->at(m),res;
			if(!(proj->Transform(circlearray->at(m), res)))
			{
				res.set(res.x(),res.y(),(res.z())*1000.0+h);
				temp_color_array->push_back(osg::Vec4(0.96, 0.0, 0.08, 1.0));
				temp_array->push_back(res);
			}
		}
		ln1->SetCoordinateTransform(pLayer->CoordTrans());
		ln1->SetOsgArray(temp_array);
		ln1->SetOsgColorArray(*temp_color_array);
		ln1->SetParentLayer(pLayer);
		pMG->insert(Layer::MgoGeometryPair(pNum,ln1));
		pNum++;

		/*if (numofcircle == i)
		{
		goQuadrilateral *CirclePlane = new goQuadrilateral;
		*CirclePlane = *ln1;
		osg::ref_ptr<osg::Vec4Array> pColors = new osg::Vec4Array;
		pColors->push_back(osg::Vec4(0, 0, 0, 0.5));
		CirclePlane->SetOsgColorArray(*pColors);
		pMG->insert(Layer::MgoGeometryPair(pNum,ln1));

		CirclePlane->setGlenum(osg::PrimitiveSet::TRIANGLE_FAN);
		pMG->insert(Layer::MgoGeometryPair(++pNum, CirclePlane));
		}*/
	}

	if (int(R*1000/Res) % foot)
	{
		goQuadrilateral *ln2 = new goQuadrilateral;	
		osg::ref_ptr <osg::Vec3Array> temp_array = new osg::Vec3Array  ;//数据数组
		osg::ref_ptr <osg::Vec4Array> temp_color_array = new osg::Vec4Array;//颜色数组
		ln2->setGlenum(osg::PrimitiveSet::LINES);
		osg::ref_ptr <osg::Vec3Array> circlearray = new osg::Vec3Array;

		for (int q=1; q<=360;q++)
		{
			osg::Vec3 circle;
			float angle = q-1;
			circle.set(X+R * sin(M_PI / 180 * angle),Y+R * cos(M_PI / 180 * angle),h);
			circlearray->push_back(circle);
		}
		for (int n = 0 ;n<circlearray->size();n++)
		{
			osg::Vec3 res;
			if(!(proj->Transform(circlearray->at(n), res)))
			{
				res.set(res.x(),res.y(),(res.z())*1000.0+h);
				temp_color_array->push_back(osg::Vec4(0.96, 0.0, 0.08, 1.0));
				temp_array->push_back(res);
			}
		}
		ln2->SetCoordinateTransform(pLayer->CoordTrans());
		ln2->SetOsgArray(temp_array);
		ln2->SetOsgColorArray(*temp_color_array);
		ln2->SetParentLayer(pLayer);

		

		pMG->insert(Layer::MgoGeometryPair(pNum,ln2));
		pNum++;

		goQuadrilateral *CirclePlane = new goQuadrilateral;
		*CirclePlane = *ln2;
		osg::ref_ptr<osg::Vec4Array> pColors = new osg::Vec4Array;
		pColors->push_back(osg::Vec4(0, 0, 0, 1.0));
		CirclePlane->SetOsgColorArray(*pColors);
		CirclePlane->setGlenum(osg::PrimitiveSet::TRIANGLE_FAN);
		pMG->insert(Layer::MgoGeometryPair(++pNum, CirclePlane));
	}
	//画线
	for (int j = 0 ;j < 6;j++)
	{
		goQuadrilateral *ln = new goQuadrilateral;
		osg::ref_ptr <osg::Vec3Array> temp_array = new osg::Vec3Array  ;//数据数组
		osg::ref_ptr <osg::Vec4Array> temp_color_array = new osg::Vec4Array;//颜色数组

		ln->setGlenum(osg::PrimitiveSet::LINES);

		double x1 =  (R*1000.0/Res) * cos((30*j)*PI/180)/60;
		double y1 =  (R*1000.0/Res) * sin((30*j)*PI/180)/60;
		double x2 = -(R*1000.0/Res) * cos((30*j)*PI/180)/60;
		double y2 = -(R*1000.0/Res) * sin((30*j)*PI/180)/60;

		double tempx,tempy;

		tempx = x1*60;tempy = y1*60; 

		for(int k = 1;k<60;k++)
		{
			osg::Vec3 res;
			if(!(proj->Transform(osg::Vec3((tempx),(tempy),h), res)))
			{
				res.set(res.x(),res.y(),(res.z())*1000.0+h);
				temp_color_array->push_back(osg::Vec4(0.96, 0.0, 0.08, 1.0));
				temp_array->push_back(res);
			}
			tempx = tempx-x1;
			tempy = tempy-y1;
		}
		tempx = x2;tempy = y2;
		for (int l = 1;l<60;l++)
		{
			osg::Vec3 res;
			if(!(proj->Transform(osg::Vec3((tempx),(tempy),h), res)))
			{
				res.set(res.x(),res.y(),(res.z())*1000.0+h);
				temp_color_array->push_back(osg::Vec4(0.96, 0.35, 0.08, 1.0));
				temp_array->push_back(res);
			}
			tempx = tempx+x2;
			tempy = tempy+y2;
		}
		ln->SetCoordinateTransform(pLayer->CoordTrans());
		ln->SetOsgArray(temp_array);
		ln->SetOsgColorArray(*temp_color_array);
		ln->SetParentLayer(pLayer);
		pMG->insert(Layer::MgoGeometryPair(pNum,ln));
		pNum++;
	}
	osg::Vec3 resu;
	if(!(proj->Transform(osg::Vec3((0),(R),h), resu)))
	{
		resu.set(resu.x(),resu.y(),(resu.z())*1000.0+h);
		m_box.insert("UP",resu);
	}
	osg::Vec3 resd;
	if(!(proj->Transform(osg::Vec3((0),(-R),h), resd)))
	{
		resd.set(resd.x(),resd.y(),(resd.z())*1000.0+h);
		m_box.insert("DOWN",resd);
	}
	osg::Vec3 resl;
	if(!(proj->Transform(osg::Vec3((-R),(0),h), resl)))
	{
		resl.set(resl.x(),resl.y(),(resl.z())*1000.0+h);
		m_box.insert("LEFT",resl);
	}
	osg::Vec3 resr;
	if(!(proj->Transform(osg::Vec3((R),(0),h), resr)))
	{
		resr.set(resr.x(),resr.y(),(resr.z())*1000.0+h);
		m_box.insert("RIGHT",resr);
	}
	return true;
}

QStringList RadarSingleDataProvider::DrawThumbs( QString& filepath )
{QStringList filelist;
	m_fileName =filepath;
	bool mosiacRequest = false;
	if (m_fileName.contains("VWP")||m_fileName.contains("VAD")||m_fileName.contains("SBT")
		||m_fileName.contains("EVP")||m_fileName.contains("_M_")||m_fileName.contains("SCI")||m_fileName.contains("HI"))
	{
		return filelist;
	}
	GDataFiles* gDataFile = NULL;
	gData = new GenerationData();
	gDataFile  = (GDataFiles*)gData;

	int ret = gDataFile->ReadFrom((char *)(filepath.toStdString().c_str()));
	if(ret!=0)
	{
		return filelist;
	}
	m_layers = GetCountOfLayer();
	
	if (!InitColorTable())
	{
		return filelist;
	}
	for (int i = 0 ; i< m_layers;i++)
	{
		QFileInfo info (filepath);
		QString tempFilePath = filepath;
		QString name1 = info.fileName();
		QString name2 = info.fileName();
		if (info.suffix().toLower() == "bin")
		{
			name2.replace(".bin","_Layer"+QString::number(i) + ".png");
		}
		else if (info.suffix().toLower() == "bz2")
		{
			name2.replace(".bz2","_Layer"+QString::number(i) + ".png");
		}
		QString picPath = tempFilePath.replace(name1,name2);
		tempFilePath = filepath;
		DrawPic(picPath,gData,i);
		filelist.push_back(picPath);
	}

	return filelist;
}

bool RadarSingleDataProvider::DrawPic( QString& filePath,GenerationData* gData,int requestLayer /*= -1*/ )
{
	pSingleGdata = (SingleDataProvider*)gData;

	pSingleGdata->DrawPic(filePath,requestLayer,m_ColorTable);
	return true;
}

void RadarSingleDataProvider::GetRect(QRectF &rect )
{
	RadProjTransform *proj2 = NULL;
	osg::Vec3 p = stationPosition();
	if (m_btransform )
	{
		proj2 = new RadProjTransform;
		QString destproj = "RADARPROJ" ;
		destproj.append(" ");
		destproj.append(QString::number(p.y()));
		destproj.append(" ");
		destproj.append( QString::number(p.x()));
		destproj.append(" ");
		destproj.append(QString::number(p.z()));

		proj2->setSourceCRS(destproj.toStdString());
		proj2->setSourceProjName("雷达中心兰勃特投影");

		proj2->setDestCRS("+proj=longlat +ellps=WGS84 +datum=WGS84 +no_defs");
		proj2->setDesProjName("无投影");

		proj2->Elevation(true);
	}
	double X = p.y();
	double Y = p.x();
	double h = p.z();
	double R= GetSingleProductRadius(0);
	double Res=1000.0;

	int c_resolution = 1000/Res;
	int foot = 50*c_resolution*1000;
	int numofcircle = (int)((R*1000/foot)/Res);
	/*Layer::MgoGeometry* pMG = pLayer->GetMgoGeometry();*/
	int pNum = 0;
	 ;

	float angle = 270;
	osg::Vec3 circle;
	circle.set(X+R * sin(M_PI / 180 * angle),Y+R * cos(M_PI / 180 * angle),h);

	osg::Vec3 topleft = osg::Vec3(circle.x(),circle.y()-R,circle.z());
	osg::Vec3 bottomright = osg::Vec3(circle.x()+2*R,circle.y()+R,circle.z());
	

	osg::Vec3 res1;
	if(!(proj2->Transform(topleft, res1)))
	{
		rect.setTop (res1.y()) ;
		rect.setLeft(res1.x()) ;
	}

	osg::Vec3 res2;
	if(!(proj2->Transform(bottomright, res2)))
	{
		rect.setBottom (res2.y()) ;
		rect.setRight(res2.x()) ;
	}


}

P_COLORTABLE_VECTOR RadarSingleDataProvider::GetColorTable()
{
	return m_ColorTable;
}

RadProjTransform* RadarSingleDataProvider::Coords()
{
	return proj;
}


void RadarSingleDataProvider::AreaAdd( bool tArea,bool tPie,float tMin,float tMax ,osg::Vec3Array* p)
{
	qDebug()<<"RadarSingleDataProvider0";
	AreaStatistic *pAreaS = new AreaStatistic;
	if (p != 0)
	{
		qDebug()<<"RadarSingleDataProvider1";
		pAreaS->SetPoint(p);
	}
	pAreaS->AreaAdd(tArea,tPie,tMin,tMax,this);
}

QString RadarSingleDataProvider::GetDataUnit()
{
	proAttr pAttributeHaHa;
	if (pDrawProduct != NULL)
	{
		pDrawProduct->ReadInfo(&pAttributeHaHa,0);
	}
	//-------------用最大不模糊速度修改  速度产品颜色表fstart fend值
	int productcode = pAttributeHaHa.pro_code;

	if (261 == productcode||262== productcode||251 == productcode||252== productcode||253== productcode||258== productcode||259== productcode)
	{
		return "dBZ";
	}

	if (254 == productcode||255== productcode||256== productcode||257== productcode||260== productcode||275== productcode)
	{
		return "m/s";
	}

	if (263== productcode||276== productcode||280 == productcode||282== productcode||283== productcode||284== productcode)
	{
		return "dBZ";
	}

	if (264 == productcode||265== productcode)
	{
		return "km";
	}

	if (266 == productcode||503 == productcode)
	{
		return "kg/m2";
	}
	if (267 == productcode||504 == productcode)
	{
		return "g/m3";
	}
	if (271 == productcode||272== productcode||273== productcode||274== productcode||286 == productcode||278== productcode)
	{
		return "mm";
	}
	if (501 == productcode||502== productcode||506== productcode||508== productcode)
	{
		return "dBZ";
	}
	if (510 == productcode||511== productcode||512== productcode||513== productcode)
	{
		return "mm";
	}
	if (509 == productcode)
	{
		return "mm/hr";
	}
	return "";
}

float RadarSingleDataProvider::GetPositionValue( double lon,double lat,double height )
{
	pSingleGdata = (SingleDataProvider*)gData;
	
	return pSingleGdata->GetValue(lon,lat,height,proj);
}

