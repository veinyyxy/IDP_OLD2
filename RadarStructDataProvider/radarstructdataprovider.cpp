#include <QtCore/QCoreApplication>
#include <QtCore/QDateTime>
#include "goGeometry.h"
#include <geos/geom/CoordinateArraySequence.h>
#include "Draw_SBT_Product.h"
#include "Draw_M_Product.h"
#include "ProductStructDraw_HI.h"
#include "ProductStructDraw_VAD.h"
#include "ProductDraw_TITAN.h"
#include "Draw_VWP_Product.h"
#include "Draw_LRM_Product.h"
#include "ProductDraw_SWP.h"
#include "ProductStructDraw_SCI.h"
#include "ProductStructDraw_SCT.h"
#include "ProductStructDraw_EVP.h"
#include "ProductStructDraw_TVS.h"
#include "ProductDraw_MSBT.h"
#include "radarstructdataprovider.h"

#define  racial 10000.000
#define  km  1000.000

extern "C"
{
	DataProvider Q_DECL_EXPORT * DataProviderPluginMain()
	{
		g_pRadarProvider = new RadarStructDataProvider;
		return g_pRadarProvider;
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
		if (filename.contains(QString::fromLocal8Bit("VWP"))||filename.contains(QString::fromLocal8Bit("VAD"))||filename.contains(QString::fromLocal8Bit("SBT"))
			||filename.contains(QString::fromLocal8Bit("EVP"))||filename.contains(QString::fromLocal8Bit("_M_"))||filename.contains(QString::fromLocal8Bit("SCI"))||filename.contains(QString::fromLocal8Bit("HI")))
		{
			return true;
		}
		else
			return false;
	}
}

RadarStructDataProvider * RadarStructDataProvider::m_pStructProvider = 0;

RadarStructDataProvider::RadarStructDataProvider()
{
	m_pStructData = new RadarStructData;
	m_ColorTable = new COLORTABLE_VECTOR;
	m_requestLayer = 0;
	//m_pDraw = NULL;
}

RadarStructDataProvider::~RadarStructDataProvider()
{

}

RadarStructDataProvider * RadarStructDataProvider::Instance()
{
	if (!m_pStructProvider)
	{
		m_pStructProvider = new RadarStructDataProvider();
	}
	return m_pStructProvider;
}

bool RadarStructDataProvider::LoadFile( const std::string& filename )
{
	m_fileName= QString(filename.c_str());
	bool mosiacRequest = false;
	if (!(
		m_fileName.  contains("VWP")||m_fileName.contains("VAD")||m_fileName.contains  ("SCT")||m_fileName.contains  ("SCI")
		||m_fileName.contains("LRM")||m_fileName.contains("SBT")||m_fileName.contains  ("EVP") || m_fileName.contains("MSBT")
		||m_fileName.contains("SWP")||m_fileName.contains("HI" )||m_fileName.contains  ("_M_")
		||m_fileName.contains("CL" )||m_fileName.contains("TITAN")||m_fileName.contains("TVS"))
		)
	{
		return 0;
	}
	GDataFiles* gDataFile = NULL;
	gData = new GenerationData();
	gDataFile  = (GDataFiles*)gData;

	if (m_fileName.contains("MVIL")||m_fileName.contains("MVILD")||m_fileName.contains("BREF")||m_fileName.contains("CREF")) 
		pDrawMulProduct = (cDrawMultiProduct *)gData;
	else
		pDrawProduct = (cDrawProduct *)gData;//去掉这句的话右边的属性会出错 

	QByteArray bytearray = m_fileName.toLocal8Bit();
	const char *c_str2 = bytearray.data();

	//QStringList stationInfo = FileNameAnalyes(m_fileName);
	//QString firstName = stationInfo.at(0);
	//QString firstLetter = firstName.left(1);
	//if(firstLetter =="Z")//根据文件名第一个字母，判断 是单站还是组网
	//{
	//	mosiacRequest = false;
	//	m_bType = true;
	//}
	//else if(firstLetter == "X")
	//{
	//	m_bType = false;
	//	mosiacRequest = true;
	//}
	int ret = gDataFile->ReadFrom((char *)c_str2);
	if(ret!=0)
	{
		return 0;
	}
	gInt16 nProductCode = 0;
	pDrawProduct->GetProductCode(nProductCode);
	m_productcode = nProductCode;
	switch(nProductCode)
	{
	case 268:
		m_pStructData->type = STRUCT_VAD; 
		break;
	case 269:
		m_pStructData->type = STRUCT_VWP;  
		break;
	case 277:
		m_pStructData->type = STRUCT_SCI;  
		break;
	case 278:
		m_pStructData->type = STRUCT_SCT;  
		break;
	case 280:
		m_pStructData->type = STRUCT_LRM;  
		break;
	case 281:
		m_pStructData->type = STRUCT_SBT;  
		break;
	case 285:
		m_pStructData->type = STRUCT_EVP;  
		break;
	case 288:
		m_pStructData->type = STRUCT_SWP;  
		break;
	case 291:
		m_pStructData->type = STRUCT_HI;  
		break;
	case 292:
		m_pStructData->type = STRUCT_M;  
		break;
	case 295:
		m_pStructData->type = STRUCT_CL;  
		break;
	case 296:
		m_pStructData->type = STRUCT_TITAN;  
		break;
	case 293:
		m_pStructData->type = STRUCT_TVS;  
		break;
	case 516:
		m_pStructData->type = STRUCT_MSBT;
	default:
		m_pStructData->type = UNKONW_TYPE; 
		break;
	}
	m_pStructData->pData = (CProductRead*)pDrawProduct;
	RadarStructProductionDraw *pDraw = NULL;
	if (m_pStructData->type != STRUCT_EVP && m_pStructData->type != STRUCT_SCI )
	{
		InitColorTable();
	}
	pDraw = GetProductDrawInstance(m_pStructData->type);
	if (pDraw != NULL)
	{
		pDraw->DumpAll(m_pParentLayer,m_pStructData->pData,m_ColorTable,m_fileName,m_requestLayer);
		//添加中心点坐标********************************************************//
		//QVector3D tmpCentural;
		//tmpCentural = stationPosition();
		//setRadarCentral(tmpCentural);
		ReleaseProductDrawInstance(pDraw);
	}

	
	return true;
}

QStringList RadarStructDataProvider::FileNameAnalyes( QString fName )
{
	QStringList attribute = QDir(fName).dirName().split("_");
	QString str_time = attribute[3].left(attribute[3].indexOf("."));
	attribute[2].append(str_time);
	return attribute;
}

void RadarStructDataProvider::InitColorTable()
{
	QString appConfigPth = QCoreApplication::applicationDirPath().remove("/bin") + "/config/";

	xml_reader.ReadFile(appConfigPth+"/BasicInfo/m_single_radar_color.xml");
	m_hColorTableHash = (xml_reader.ColorTable());
	gInt16 productcode = 0 ;
	m_pStructData->pData->GetProductCode(productcode);
	m_ColorTable =  m_hColorTableHash.value(productcode);
	//if(254==pSingleGdata->ProductCode()||255==pSingleGdata->ProductCode()||301==pSingleGdata->ProductCode()){
	//	for (int ii = 0; ii<m_ColorTable->count();ii++){
	//		m_ColorTable->at(ii)->fStart=(m_ColorTable->at(ii)->fStart)/100*m_nParam;
	//		m_ColorTable->at(ii)->fEnd=(m_ColorTable->at(ii)->fEnd)/100*m_nParam;
	//		m_hColorTableHash.value(pSingleGdata->ProductCode())->at(ii)->fStart=m_ColorTable->at(ii)->fStart;
	//		m_hColorTableHash.value(pSingleGdata->ProductCode())->at(ii)->fEnd=m_ColorTable->at(ii)->fEnd;
	//	}
	//}
	if (m_ColorTable->size()==0)
	{
		return;
	}
	for (int i = 0; i<m_ColorTable->count();i ++)
	{
		P_COLORNODE_PRODUCT color = m_ColorTable->at(i);		
		QString key = QString::number(color->fStart) + "-" + QString::number(color->fEnd);
		m_pColorValueHash.insert(key,QVector4D(color->nRed/255.0 ,color->nGreen/255.0,color->nBlue/255.0,1.0));
	}
}

void RadarStructDataProvider::InitProductNameTable()
{

}


RadarStructProductionDraw* RadarStructDataProvider::GetProductDrawInstance( STRUCT_DATA_TYPE nProductCode )
{
	RadarStructProductionDraw* p=NULL;
	switch(nProductCode)
	{
	case STRUCT_VAD:
		p=new CProductStructDraw_VAD;   //added by hdm 20090211
		break;
	case STRUCT_M:
		p = new CProductStructDraw_M;
		break;
	case STRUCT_VWP:
		p=new Draw_VWP_Product;
		break;
	case STRUCT_SCT:
		p=new CProductStructDraw_SCT;
		break;
	case STRUCT_SCI:
		p=new CProductStructDraw_SCI;
		break;
	case STRUCT_LRM:
		p=new CProductStructDraw_LRM;
		break;
	case  STRUCT_MSBT:
		p= new CProductStructDraw_MSBT;
		break;
	case STRUCT_SBT:
		p=new CProductStructDraw_SBT;
		break;
	case STRUCT_HI:
		p=new CProductStructDraw_HI;
		break;
	case STRUCT_EVP:
		p=new CProductStructDraw_EVP;
		break;
	case STRUCT_SWP:
		p=new CProductStructDraw_SWP;
		break;
	/*case 295:
		p=new CProductStructDraw_CL;
		break;
		*/
	case STRUCT_TITAN:
		p=new CProductStructDraw_TITAN;
		break;
	case STRUCT_TVS:
		p=new CProductStructDraw_TVS;
		break;
	default:
		p=NULL;
		break;
	}
	return p;
}

void RadarStructDataProvider::ReleaseProductDrawInstance( RadarStructProductionDraw* lpInstance )
{
	RadarStructProductionDraw* p= lpInstance;
	if(p!=NULL)
	{
		delete p;
		p=NULL;
	}
}

QVector3D RadarStructDataProvider::stationPosition()
{
	double CenterLat,CenterLon,CenterAltitude;
	RadarInfor * pRadarInfo = m_pStructData->pData->GetRadarInfoP(); 
	CenterLat  = pRadarInfo->Latitude*0.001;      //中心纬度
	CenterLon  = pRadarInfo->Longitude*0.001;     //中心经度
	CenterAltitude  = pRadarInfo->Altitude*0.001; //雷达海拔高度，单位：米
	return QVector3D(CenterLat,CenterLon,CenterAltitude);
}

QString RadarStructDataProvider::DrawThumbs( QString& filepath )
{
	//m_fileName= QString(filename.c_str());
	QImage image;
	if (filepath.contains("SBT"))
	{
		m_pDraw = new CProductStructDraw_SBT;
		//InitColorTable();
		AnalysisFile(filepath.toStdString());
		image = m_pDraw->DrawThumbs(m_pStructData->pData,m_ColorTable,400);
	}

	if (filepath.contains("HI"))
	{
		m_pDraw = new CProductStructDraw_HI;
		//InitColorTable();
		AnalysisFile(filepath.toStdString());
		image = m_pDraw->DrawThumbs(m_pStructData->pData,m_ColorTable,400);
	}
	filepath.replace(QFileInfo(filepath).suffix(),"png");
	image.save(filepath);
	return filepath;
}

void RadarStructDataProvider::GetRect( QRectF &rect )
{
	m_pDraw->GetRect(rect);
}

void RadarStructDataProvider::AnalysisFile( const std::string& filename )
{
	m_fileName= QString(filename.c_str());
	bool mosiacRequest = false;
	if (!(
		m_fileName.  contains("VWP")||m_fileName.contains("VAD")||m_fileName.contains  ("SCT")
		||m_fileName.contains("LRM")||m_fileName.contains("SBT")||m_fileName.contains  ("EVP")
		||m_fileName.contains("SWP")||m_fileName.contains("HI" )||m_fileName.contains  ("_M_")
		||m_fileName.contains("CL" )||m_fileName.contains("TITAN")||m_fileName.contains("TVS"))
		)
	{
		return ;
	}
	GDataFiles* gDataFile = NULL;
	gData = new GenerationData();
	gDataFile  = (GDataFiles*)gData;

	if (m_fileName.contains("MVIL")||m_fileName.contains("MVILD")||m_fileName.contains("BREF")||m_fileName.contains("CREF")) 
		pDrawMulProduct = (cDrawMultiProduct *)gData;
	else
		pDrawProduct = (cDrawProduct *)gData;//去掉这句的话右边的属性会出错 

	QByteArray bytearray = m_fileName.toLocal8Bit();
	const char *c_str2 = bytearray.data();

	int ret = gDataFile->ReadFrom((char *)c_str2);
	if(ret!=0)
	{
		return ;
	}
	gInt16 nProductCode = 0;
	pDrawProduct->GetProductCode(nProductCode);

	switch(nProductCode)
	{
	case 268:
		m_pStructData->type = STRUCT_VAD; 
		break;
	case 269:
		m_pStructData->type = STRUCT_VWP;  
		break;
	case 278:
		m_pStructData->type = STRUCT_SCT;  
		break;
	case 280:
		m_pStructData->type = STRUCT_LRM;  
		break;
	case 281:
		m_pStructData->type = STRUCT_SBT;  
		break;
	case 285:
		m_pStructData->type = STRUCT_EVP;  
		break;
	case 288:
		m_pStructData->type = STRUCT_SWP;  
		break;
	case 291:
		m_pStructData->type = STRUCT_HI;  
		break;
	case 292:
		m_pStructData->type = STRUCT_M;  
		break;
	case 295:
		m_pStructData->type = STRUCT_CL;  
		break;
	case 296:
		m_pStructData->type = STRUCT_TITAN;  
		break;
	case 293:
		m_pStructData->type = STRUCT_TVS;  
		break;
	default:
		m_pStructData->type = UNKONW_TYPE; 
		break;
	}
	m_pStructData->pData = (CProductRead*)pDrawProduct;
	RadarStructProductionDraw *pDraw = NULL;
	InitColorTable();
	pDraw = GetProductDrawInstance(m_pStructData->type);
}

void RadarStructDataProvider::AreaAdd( bool tArea,bool tPie,float tMin,float tMax,osg::Vec3Array* p)
{

}

QString RadarStructDataProvider::GetDataUnit()
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

float RadarStructDataProvider::GetPositionValue( double lon,double lat,double height )
{
	return 0.0;
}
