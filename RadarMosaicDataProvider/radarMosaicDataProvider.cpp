#include "radarMosaicDataProvider.h"
#include <QtCore/QCoreApplication>
#include <QtCore/QDateTime>
#include <Qtcore/QMutexLocker>
#include "goGeometry.h"
#include "Layer.h"
#include "Map.h"
#include<shlobj.h>
#include <QtCore/QSettings>
#include <geos/geom/CoordinateArraySequence.h>
#include "Proj4Transform.h"
#include "AreaStatistic.h"
#include  <math.h>
#define  racial 10000.000
#define  km  1000.000


RadarMosaicDataProvider* g_pmRadarProvider = 0;
extern "C"
{
	DataProvider Q_DECL_EXPORT * DataProviderPluginMain()
	{
		g_pmRadarProvider = new RadarMosaicDataProvider;
		return g_pmRadarProvider;
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
		if (filename.contains(QString::fromLocal8Bit("MVIL"))||filename.contains(QString::fromLocal8Bit("MVILD"))||filename.contains(QString::fromLocal8Bit("BREF"))||filename.contains(QString::fromLocal8Bit("CREF"))) 
		{
			return true;
		}
		else
			return false;
	}
}

RadarMosaicDataProvider * RadarMosaicDataProvider::m_pRadarProvider = 0;
/** 
  * @brief    ���캯��   
  * @author   ��־ѧ  
  * @param [in] parameter name   parameter description
  * @param [out] parameter name   parameter description
  * @return   return data
  * @note     detailed description �޸���  �޸�ʱ��  �޸�����
  * @date     create date                                
  * @exception  exception object exception description  
  * @remarks remark text               
  * @deprecated   description            
  * @since when time or version           
  * @see references            
*/ 
RadarMosaicDataProvider::RadarMosaicDataProvider():pDrawMulProduct(0),
	pMosaicGdata(0),gData(0),m_MaxValue(-32767),m_iLayerNO(0)
{


}

/** 
  * @brief    ��������   
  * @author   ��־ѧ  
  * @param [in] parameter name   parameter description
  * @param [out] parameter name   parameter description
  * @return   return data
  * @note     detailed description �޸���  �޸�ʱ��  �޸�����
  * @date     create date                                
  * @exception  exception object exception description  
  * @remarks remark text               
  * @deprecated   description            
  * @since when time or version           
  * @see references            
*/ 
RadarMosaicDataProvider::~RadarMosaicDataProvider()
{
	gData->ClearAll();
	if (NULL != pDrawMulProduct)
	{
		delete pDrawMulProduct;
		pDrawMulProduct = NULL;
	}
}

/** 
  * @brief    ����ṩ��ʵ��    
  * @author   ��־ѧ  
  * @param [in] parameter name   parameter description
  * @param [out] parameter name   parameter description
  * @return   RadarMosaicDataProvider  ���������ṩ��ָ��
  * @note     ��־ѧ
  * @date     2013��3��                               
  * @exception  exception object exception description  
  * @remarks remark text               
  * @deprecated   description            
  * @since when time or version           
  * @see references            
*/ 
RadarMosaicDataProvider * RadarMosaicDataProvider::Instance()
{
	if (!m_pRadarProvider)
	{
		m_pRadarProvider = new RadarMosaicDataProvider();
	}
	return m_pRadarProvider;
}

/** 
  * @brief    �����״�����     
  * @author   ��־ѧ 
  * @param    filename   �ļ���
  * @param    
  * @return   bool  �ɹ�����true��ʧ��false
  * @note     detailed description �޸���  �޸�ʱ��  �޸�����
  * @date     2013��8��                                
  * @exception  exception object exception description  
  * @remarks remark text               
  * @deprecated   description            
  * @since when time or version           
  * @see references            
*/ 
bool RadarMosaicDataProvider::LoadFile( const std::string& filename )
{
	m_fileName = QString(filename.c_str( ));
	GDataFiles* gDataFile = NULL;
	gData = new GenerationData();
	gDataFile  = (GDataFiles*)gData;
 
	if(string::npos != filename.find("MVIL") || string::npos != filename.find("MVILD") || string::npos != filename.find("BREF") || string::npos != filename.find("CREF")||string::npos != filename.find("PSP")||string::npos != filename.find("HM")||string::npos != filename.find("MET"))	
		pDrawMulProduct = (cDrawMultiProduct *)gData;
	else
		return false;


	//QStringList stationInfo = FileNameAnalyes(QString(filename.c_str( )));//�ļ�������
	//QString firstName = stationInfo.at(0);
	//QString firstLetter = firstName.left(1);

	int ret = gDataFile->ReadFrom((char *)filename.c_str());
	if(ret!=0)
	{
		return 0;
	}
	pMosaicGdata = (MosaicDataProvider *)gData;
	m_Rect = *(pMosaicGdata->GetRect());
	//��ȡ������������
	ProductDataProvider(m_pParentLayer,gData,0);
	DumpMosaicAttr(m_pParentLayer);
	m_pParentLayer->InsertLayerAttribute(m_ColorTable,"ColorTable","ColorList");
	InitProductNameTable();

	return true;
}

/** 
  * @brief   �ļ�������    
  * @author  ��־ѧ  
  * @param   �ļ�ȫ·��+����
  * @param [out] parameter name   parameter description
  * @return   QStringList �ļ�������
  * @note     detailed description �޸���  �޸�ʱ��  �޸�����
  * @date     create date                                
  * @exception  exception object exception description  
  * @remarks remark text               
  * @deprecated   description            
  * @since when time or version           
  * @see references            
*/ 
QStringList RadarMosaicDataProvider::FileNameAnalyes( QString fName )
{
	QStringList attribute = QDir(fName).dirName().split("_");
	if (attribute.at(1) == "CHN")
	{
		QFileInfo info(fName);
		if(info.suffix() == "bin")
		{
			QString temp = attribute.last();
			QStringList templist = temp.split("-");
			attribute.clear();
			attribute.append(templist[2]);
			attribute.append(templist[1]);
			temp = templist[4].left(attribute[4].indexOf("."));
			attribute.append(temp);
		}else if (info.suffix() == "bz2")
		{
			QString temp = attribute.last();
			QStringList templist = temp.split("-");
			attribute.clear();
			attribute.append(templist[1]);
			attribute.append(templist[3]);
			temp = templist[4].left(attribute[4].indexOf("."));
			attribute.append(temp);
		}


	}else 
	{
		QString str_time = attribute[3].left(attribute[3].indexOf("."));
		attribute[2].append(str_time);
	}

	return attribute;
}

/** 
  * @brief    �ļ����ݽ����ӿں���     
  * @author   ��־ѧ   
  * @param    pLayer ͼ��ָ�� gData ����ָ�� requestLayer ��Ҫ��ȡ�Ĳ���
  * @param    [out] parameter name   parameter description
  * @return   void
  * @note     detailed description �޸���  �޸�ʱ��  �޸�����
  * @date     2013��6��                                
  * @exception  exception object exception description  
  * @remarks remark text               
  * @deprecated   description            
  * @since when time or version           
  * @see references            
*/ 
void RadarMosaicDataProvider::ProductDataProvider( Layer *pLayer,GenerationData* gData,int requestLayer /*= -1*/ )
{
	m_MaxValue = 0;
	pMosaicGdata = (MosaicDataProvider *)gData;
	InitColorTable();	

	pMosaicGdata->ProdDataProvider(pLayer,requestLayer,m_MaxValue,m_pColorValueIndexHash);
}

/** 
  * @brief    ��ʼ����ɫ��     
  * @author   ��־ѧ  
  * @param [in] parameter name   parameter description
  * @param [out] parameter name   parameter description
  * @return   void
  * @note     detailed description �޸���  �޸�ʱ��  �޸�����
  * @date     create date                                
  * @exception  exception object exception description  
  * @remarks remark text               
  * @deprecated   description            
  * @since when time or version           
  * @see references            
*/ 
void RadarMosaicDataProvider::InitColorTable()
{
	QString appConfigPth = QCoreApplication::applicationDirPath().remove("/bin") + "/config/";
	xml_reader.ReadFile(appConfigPth+"/BasicInfo/m_multi_radar_color.xml");
	m_hColorTableHash = (xml_reader.ColorTable());
	m_ColorTable =  m_hColorTableHash.value(pMosaicGdata->ProductCode());
	//P_COLORNODE_PRODUCT colorq = new COLORNODE_PRODUCT;
	//colorq->fStart = -327.680;
	//colorq->fEnd = -327.660;
	//colorq->nRed = 128;
	//colorq->nGreen = 128;
	//colorq->nBlue = 128;
	//m_ColorTable->push_back(colorq);
	for (int i = 0; i<m_ColorTable->count();i ++)
	{
		P_COLORNODE_PRODUCT color = m_ColorTable->at(i);		
		/*QString key = QString::number(color->fStart) + "-" + QString::number(color->fEnd);*/
		//m_pColorValueHash.insert(key,QVector4D(color->nRed/255.0 ,color->nGreen/255.0,color->nBlue/255.0,1.0));
		m_pColorValueIndexHash.insert(i,QVector4D(color->nRed/255.0 ,color->nGreen/255.0,color->nBlue/255.0,1.0));
	}

}

/** 
  * @brief    ��ò�Ʒ���   
  * @author   ��־ѧ   
  * @param [in] parameter name   parameter description
  * @param [out] parameter name   parameter description
  * @return   int ��Ʒ���
  * @note     detailed description �޸���  �޸�ʱ��  �޸�����
  * @date     create date                                
  * @exception  exception object exception description  
  * @remarks remark text               
  * @deprecated   description            
  * @since when time or version           
  * @see references            
*/ 
int RadarMosaicDataProvider::GetProductCode()
{
	return pMosaicGdata->ProductCode();
}

/** 
  * @brief    ��ʼ����Ʒ���б�    
  * @author   ��־ѧ  
  * @param [in] parameter name   parameter description
  * @param [out] parameter name   parameter description
  * @return   void
  * @note     detailed description �޸���  �޸�ʱ��  �޸�����
  * @date     create date                                
  * @exception  exception object exception description  
  * @remarks remark text               
  * @deprecated   description            
  * @since when time or version           
  * @see references            
*/ 
void RadarMosaicDataProvider::InitProductNameTable()
{
	QString appConfigPth = QCoreApplication::applicationDirPath().replace("/Bin","") + "/Config/";
	CRadReadProduct_XML temp_reader;
	temp_reader.ReadFile(appConfigPth+"/BasicInfo/product_name_map.xml");
	P_PRODUCT tempPro;
	tempPro =  temp_reader.ProductTable().value(pMosaicGdata->ProductCode());
	if (tempPro != NULL)
	{	
		m_ProductNameTable.nID = tempPro->nID;
		m_ProductNameTable.strAbb = tempPro->strAbb;
		m_ProductNameTable.strName = tempPro->strName;
		m_ProductNameTable.strUnit = tempPro->strUnit;
	}
}

/** 
  * @brief    ��ȡ������������    
  * @author   ��־ѧ   
  * @param  layer ͼ��ָ��
  * @param [out] parameter name   parameter description
  * @return   void
  * @note     detailed description �޸���  �޸�ʱ��  �޸�����
  * @date     create date                                
  * @exception  exception object exception description  
  * @remarks remark text               
  * @deprecated   description            
  * @since when time or version           
  * @see references            
*/ 
void RadarMosaicDataProvider::DumpMosaicAttr(Layer *layer )
{
	AreaInfo *tempai = pMosaicGdata->GetAreaInfoP();
	MultiDataHead *tempmdh = pMosaicGdata->GetMultiHeadP();
	MultiLayerHead *tempmlh = pMosaicGdata->GetLayerP(0);
	cDrawMultiProduct *cm = (cDrawMultiProduct*)pMosaicGdata;
	if (cm ==NULL)
	{
		return;
	}
	int pro;
	std::string regionName ,regionSite,proName, proCName;

	QString tempStr;
	QString StrUnit = GetDataUnit();
	mHighHash.insert(tempmlh->SNOfLayer,tempmlh->HeightOfElevation);
	float d = cm->GetMaxValue(0);
	int npCode = GetProductCode();
	m_productcode = npCode;
	QString strproName = GetProductName(npCode);
	layer->InsertLayerAttribute(strproName.toStdString(),std::string("��Ʒ���ƣ�"),"String");

	std::string Arename = cm->TransToRadarName(tempai->AreaCode);
	layer->InsertLayerAttribute(Arename,std::string("��������"),"String");

	QString rese = QString( "%1*%2deg").arg(double(tempmdh->ScaleOfData*0.0001)).arg(double(tempmdh->ScaleOfData*0.0001));
	layer->InsertLayerAttribute(rese.toStdString(),std::string("�ֱ��ʣ�"),"String");

	QDate tDate(1969,12,31);
	tDate = tDate.addDays(static_cast<int>(tempmdh->ProductDate));
	tempStr = tDate.toString("yyyy-MM-dd");
	layer->InsertLayerAttribute(tempStr.toStdString(),std::string("�״��������ڣ�"),"String");

	QTime tTime(0,0,0,0);
	tTime = tTime.addMSecs(tempmdh->ProductTime);
	tempStr = tTime.toString("hh:mm:ss");
	layer->InsertLayerAttribute(tempStr.toStdString(),std::string("�״�����ʱ�䣺"),"String");

	QDate tDateG(1969,12,31);
	tDateG = tDateG.addDays(tempmdh->GenerationDateOfProduct);
	tempStr = tDateG.toString("yyyy-MM-dd");
	layer->InsertLayerAttribute(tempStr.toStdString(),std::string("��Ʒ�������ڣ�"),"String");

	QTime tTimeG(0,0,0,0);
	tTimeG = tTimeG.addMSecs(tempmdh->GenerationTimeOfProduct);
	tempStr = tTimeG.toString("hh:mm:ss");
	layer->InsertLayerAttribute(tempStr.toStdString(),std::string("��Ʒ����ʱ�䣺"),"String");

	if(npCode == 501)   //������BREF��Ʒ
	{
		int s = mHighHash.value(0);
		layer->InsertLayerAttribute((QString( "%1m").arg((s)*10)).toStdString(),std::string("�����߶ȣ�"),"String");
	}

	QString strdu = QString::number(tempai->LongitudeOfWest/10000);
	int ntemp = (tempai->LongitudeOfWest%10000);
	int fen = (int)ntemp*60*0.0001;
	int ns =(  ((double)(ntemp*0.0001*60)-(double)fen)*60     );
	QString strfen = QString::number(fen);
	QString strS = QString::number(ns);
	tempStr = QString( "%1��%2��%3�� E").arg(strdu).arg(strfen).arg(strS);
	layer->InsertLayerAttribute(tempStr.toStdString(),std::string("��ʼ���ȣ�"),"String");

	strdu = QString::number(tempai->LatitudeOfNorth/10000);
	ntemp = (tempai->LatitudeOfNorth%10000);
	fen = (int)ntemp*60*0.0001;
	ns =(  ((double)(ntemp*0.0001*60)-(double)fen)*60     );
	strfen = QString::number(fen);
	strS = QString::number(ns);
	tempStr = QString( "%1��%2��%3�� N").arg(strdu).arg(strfen).arg(strS);
	layer->InsertLayerAttribute(tempStr.toStdString(),std::string("��ʼγ�ȣ�"),"String");


	strdu = QString::number(tempai->LongitudeOfEast/10000);
	ntemp = (tempai->LongitudeOfEast%10000);
	fen = (int)ntemp*60*0.0001;
	ns =(  ((double)(ntemp*0.0001*60)-(double)fen)*60     );
	strfen = QString::number(fen);
	strS = QString::number(ns);
	tempStr = QString( "%1��%2��%3�� E").arg(strdu).arg(strfen).arg(strS);
	layer->InsertLayerAttribute(tempStr.toStdString(),std::string("��ֹ���ȣ�"),"String");

	strdu = QString::number(tempai->LatitudeOfSouth/10000);
	ntemp = (tempai->LatitudeOfSouth%10000);
	fen = (int)ntemp*60*0.0001;
	ns =(  ((double)(ntemp*0.0001*60)-(double)fen)*60     );
	strfen = QString::number(fen);
	strS = QString::number(ns);
	tempStr = QString( "%1��%2��%3�� N").arg(strdu).arg(strfen).arg(strS);
	layer->InsertLayerAttribute(tempStr.toStdString(),std::string("��ֹγ�ȣ�"),"String");

	tempStr = QString::number(d,'f',2);
	QString mav = QString::number(MaxvalHash.value(0),'f',2);
	if (MaxvalHash.value(0) == 0.0)
	{
		mav = tempStr;
	}
	tempStr = QString( "%1%2").arg(mav).arg(StrUnit);
	layer->InsertLayerAttribute(tempStr.toStdString(),std::string("���ֵ��"),"String");
	layer->InsertLayerAttribute(StrUnit.toStdString(),std::string("��λ��"),"String");
	layer->InsertLayerAttribute(*GetRect(),std::string("�߽�"),"RectF");
}

/** 
  * @brief    ��ò�Ʒ��Ӣ����    
  * @author   ��־ѧ   
  * @param     ncode ��Ʒ���
  * @param [out] parameter name   parameter description
  * @return   QString  ��Ʒ����
  * @note     detailed description �޸���  �޸�ʱ��  �޸�����
  * @date     create date                                
  * @exception  exception object exception description  
  * @remarks remark text               
  * @deprecated   description            
  * @since when time or version           
  * @see references            
*/ 
QString RadarMosaicDataProvider::GetProductName( int ncode )
{
	QString temp;
	switch(ncode)
	{
	case 251:
		temp = QString("����������(R)");
		break;
	case 252:
		temp = QString("���������ƵĻ���������(QR)");
		break;
	case 253:
		temp = QString("���������Ļ���������(BREF)");
		break;
	case 254:
		temp = QString("�����ٶ�(V)");
		break;
	case 255:
		temp = QString("���������ƵĻ����ٶ�(QV)");
		break;
	case 256:
		temp = QString("�����׿�(W)");
		break;
	case 257:
		temp = QString("���������ƵĻ����׿�(QW)");
		break;
	case 258:
		temp = QString("�ȸ��淴����(CAP)");
		break;
	case 259:
		temp = QString("ǿ�ȸ߶���ʾ(RHI)");
		break;
	case 260:
		temp = QString("�ٶȸ߶���ʾ(VHI)");
		break;
	case 261:
		temp = QString("ǿ�ȴ�ֱ����(RCS)");
		break;
	case 262:
		temp = QString("���ɨ�跴����(HBR)");
		break;
	case 263:
		temp = QString("��Ϸ�����(CR)");
		break;
	case 264:
		temp = QString("������ʸ߶�(CRH)");
		break;	
	case 265:
		temp = QString("�ز�����(ET)");
		break;
	case 266:
		temp = QString("��ֱ�ۻ�Һ̬ˮ����(VIL)");
		break;
	case 267:
		temp = QString("��ֱ�ۻ�Һ̬ˮ�ܶ�(VILD)");
		break;
	case 268:
		temp = QString("�ٶȷ�λ��ʾ(VAD)");
		break;
	case 269:
		temp = QString("VAD������(VWP)");
		break;
	case 270:
		temp = QString("��ˮ�ʹ���(QPR)");
		break;
	case 271:
		temp = QString("һСʱ�ۻ���ˮ(OHP)");
		break;
	case 272:
		temp = QString("��Сʱ�ۻ���ˮ(WHP)");
		break;
	case 273:
		temp = QString("��Сʱ�ۻ���ˮ(THP)");
		break;
	case 274:
		temp = QString("��Сʱ�ۻ���ˮ(SHP)");
		break;
	case 277:
		temp = QString("�籩ʶ��(SCI)");
		break;
	case 280:
		temp = QString("�ֲ���Ϸ�����(LRM)");
		break;
	case 281:
		temp = QString("�籩����ʶ�����(SBT)");
		break;
	case 283:
		temp = QString("һСʱǿ�ز�(OSR)");
		break;
	case 284:
		temp = QString("��Сʱǿ�ز�(WSR)");
		break;
	case 285:
		temp = QString("��չ�ٶȷ�λ��ʾ����(EVP)");
		break;
	case 291:
		temp = QString("����ָ��(HI)");
		break;
	case 292:
		temp = QString("�г߶�����(M)");
		break;
	case 293:
		temp = QString("������������(TVS)");
		break;
	case 294:
		temp = QString("����б�(CS)");
		break;
	case 301:
		temp = QString("�����(IW)");
		break;
	case 501:
		temp = QString("��������������(BREF)");
		break;
	case 502:
		temp = QString("������Ϸ�����(CREF)");
		break;
	case 503:
		temp = QString("������ֱ�ۻ�Һ̬ˮ����(MVIL)");
		break;
	case 504:
		temp = QString("������ֱ�ۻ�Һ̬ˮ�ܶ�(MVILD)");
		break;
	case 505:
		temp = QString("����(PSP)");
		break;
	case 506:
		temp = QString("����(HM)");
		break;
	case 507:
		temp = QString("�����ز�����(MET)");
		break;
	default:
		break;

	}
	return temp;
}

/** 
  * @brief    ���ݲ�Ʒ��д��ò�Ʒ����     
  * @author   ��־ѧ  
  * @param    ShortName ��Ʒ��д
  * @param [out] parameter name   parameter description
  * @return   QString ��Ʒ��ȫ��
  * @note     detailed description �޸���  �޸�ʱ��  �޸�����
  * @date     create date                                
  * @exception  exception object exception description  
  * @remarks remark text               
  * @deprecated   description            
  * @since when time or version           
  * @see references            
*/ 
QString RadarMosaicDataProvider::GetProductNameByShortName( int productcode )
{
	QMap<int,QString> ProductNameMap;
	{
		ProductNameMap.insert(251,QString("����������(R)"));
		//ProductNameMap.insert(QString("VILT"),QString("��Ȳ����ϴ�ֱ�ۻ�Һ̬ˮ����"));
		//ProductNameMap.insert(QString("VILB"),QString("��Ȳ����´�ֱ�ۻ�Һ̬ˮ����"));

		ProductNameMap.insert(252,QString("���������ƵĻ���������(QR)"));

		ProductNameMap.insert(253,QString("���������Ļ���������(BREF)"));

		ProductNameMap.insert(254,QString("�����ٶ�(V)"));

		ProductNameMap.insert(255,QString("���������ƵĻ����ٶ�(QV)"));

		ProductNameMap.insert(256,QString("�����׿�(W)"));

		ProductNameMap.insert(257,QString("���������ƵĻ����׿�(QW)"));

		ProductNameMap.insert(258,QString("�ȸ��淴����(CAP)"));

		//ProductNameMap.insert(QString( "RHI"),QString( "ǿ�ȸ߶���ʾ(RHI)"));

		//ProductNameMap.insert(QString( "VHI"),QString( "�ٶȸ߶���ʾ(VHI)"));

		//ProductNameMap.insert(QString( "RCS"),QString( "ǿ�ȴ�ֱ����(RCS)"));

		ProductNameMap.insert(262,QString("���ɨ�跴����(HBR)"));

		ProductNameMap.insert(263,QString("��Ϸ�����(CR)"));

		ProductNameMap.insert(264,QString("������ʸ߶�(CRH)"));;	

		ProductNameMap.insert(265,QString("�ز�����(ET)"));

		ProductNameMap.insert(266,QString("��ֱ�ۻ�Һ̬ˮ����(VIL)"));

		ProductNameMap.insert(267,QString("��ֱ�ۻ�Һ̬ˮ�ܶ�(VILD)"));

		ProductNameMap.insert(268,QString("�ٶȷ�λ��ʾ(VAD)"));

		ProductNameMap.insert(269,QString("VAD������(VWP)"));

		ProductNameMap.insert(270,QString("��ˮ�ʹ���(QPR)"));

		ProductNameMap.insert(271,QString("һСʱ�ۻ���ˮ(OHP)"));

		ProductNameMap.insert(272,QString("��Сʱ�ۻ���ˮ(WHP)"));

		ProductNameMap.insert(273,QString("��Сʱ�ۻ���ˮ(THP)"));

		ProductNameMap.insert(274,QString("��Сʱ�ۻ���ˮ(SHP)"));

		ProductNameMap.insert(277,QString("�籩ʶ��(SCI)"));

		ProductNameMap.insert(280,QString("�ֲ���Ϸ�����(LRM)"));

		ProductNameMap.insert(281,QString("�籩����ʶ�����(SBT)"));

		ProductNameMap.insert(283,QString("һСʱǿ�ز�(OSR)"));

		ProductNameMap.insert(284,QString("��Сʱǿ�ز�(WSR)"));

		ProductNameMap.insert(285,QString("��չ�ٶȷ�λ��ʾ����(EVP)"));

		ProductNameMap.insert(291,QString( "����ָ��(HI)"));

		ProductNameMap.insert(292,QString( "�г߶�����(M)"));

		ProductNameMap.insert(293,QString( "������������(TVS)"));

		ProductNameMap.insert(294,QString( "����б�(CS)"));

		//ProductNameMap.insert(QString("IW"),QString( "�����(IW)"));

		ProductNameMap.insert(501,QString( "��������������(BREF)"));

		ProductNameMap.insert(502,QString( "������Ϸ�����(CREF)"));

		ProductNameMap.insert(503,QString( "������ֱ�ۻ�Һ̬ˮ����(MVIL)"));

		ProductNameMap.insert(504,QString( "������ֱ�ۻ�Һ̬ˮ�ܶ�(MVILD)"));

		ProductNameMap.insert(506,QString( "�����ֲ���Ϸ�����(MLRM)"));

		//ProductNameMap.insert(QString("KTP"),QString( "�����ƶ�����Сʱ�ۼƽ�ˮ(KTP)"));

		//ProductNameMap.insert(QString("KWP"),QString( "�����ƶ�����Сʱ�ۼƽ�ˮ(KWP)"));
	}
	return ProductNameMap.value(productcode);
}


/** 
  * @brief    ��������ͼ     
  * @author   ��־ѧ   
  * @param    filepath �ļ�·��
  * @param [out] parameter name   parameter description
  * @return   ����ͼ·��
  * @note     detailed description �޸���  �޸�ʱ��  �޸�����
  * @date     create date                                
  * @exception  exception object exception description  
  * @remarks remark text               
  * @deprecated   description            
  * @since when time or version           
  * @see references            
*/ 

QString RadarMosaicDataProvider::DrawThumbs( QString& filepath )
{
	m_fileName = filepath;
	GDataFiles* gDataFile = NULL;
	gData = new GenerationData();
	gDataFile  = (GDataFiles*)gData;
	std::string filePath = filepath.toStdString();
	if(string::npos != filePath.find("MVIL") || string::npos != filePath.find("MVILD") || string::npos != filePath.find("BREF") || string::npos != filePath.find("CREF")||string::npos != filePath.find("PSP")||string::npos != filePath.find("HM")||string::npos != filePath.find("MET"))	
		pDrawMulProduct = (cDrawMultiProduct *)gData;
	else
		return false;

	int ret = gDataFile->ReadFrom((char *)filePath.c_str());
	if(ret!=0)
	{
		return 0;
		
	}
	pMosaicGdata = (MosaicDataProvider *)gData;
	m_Rect = *(pMosaicGdata->GetRect());
	QString iniPath = QCoreApplication::applicationDirPath()+QString("/Config/Config.ini");
	QSettings *configIni = new QSettings(iniPath, QSettings::IniFormat); 
	QString dbPath = configIni->value("/PICPATH/PICDIR").toString();//���Ŀ¼
	//��ȡ������������
	QFileInfo info (filepath);
	QString picPath;
	if ("bz2" == info.suffix())
	{
		QString name1 = info.fileName();

		QString name2 = info.fileName();
		name2.replace(".bz2",".png");
		picPath =filepath.replace(name1,name2);
	}else if ("bin" == info.suffix())
	{	
		QString name1 = info.fileName();

		QString name2 = info.fileName();
		name2.replace(".bin",".png");
		picPath =filepath.replace(name1,name2);

	
		picPath.replace(picPath.left(2),dbPath);
		QFileInfo infotemp(picPath);

		QString tempPath =infotemp.path();
		SHCreateDirectoryEx(NULL, tempPath.replace("/","\\").utf16(), NULL);

	}

	DrawPic(picPath,gData,0);
	return picPath;
}

/** 
  * @brief    ����ͼƬ     
  * @author   ��־ѧ  
  * @param    filePath �ļ�·�� gData ����ָ�� requestlayer ���Ʋ���
  * @param [out] parameter name   parameter description
  * @return   �ɹ�����true ʧ�ܷ���false
  * @note     detailed description �޸���  �޸�ʱ��  �޸�����
  * @date     create date                                
  * @exception  exception object exception description  
  * @remarks remark text               
  * @deprecated   description            
  * @since when time or version           
  * @see references            
*/ 
bool RadarMosaicDataProvider::DrawPic( QString& filePath,GenerationData* gData,int requestLayer /*= -1*/ )
{
	pMosaicGdata = (MosaicDataProvider *)gData;
	InitColorTable();	
	pMosaicGdata->DrawPic(filePath,requestLayer,m_ColorTable);
	return true;
}

/** 
  * @brief    ������ݷ�Χ    
  * @author   ��־ѧ   
  * @param [in] parameter name   parameter description
  * @param [out] parameter name   parameter description
  * @return   ��������ı�������ָ�루��γ�ȣ�
  * @note     detailed description �޸���  �޸�ʱ��  �޸�����
  * @date     create date                                
  * @exception  exception object exception description  
  * @remarks remark text               
  * @deprecated   description            
  * @since when time or version           
  * @see references            
*/ 
QRectF * RadarMosaicDataProvider::GetRect()
{

	return &m_Rect;
}

/** 
  * @brief   ������ݷ�Χ     
  * @author  ��־ѧ   
  * @param   lat γ�� lon����   ��������λ��
  * @param [out] parameter name   parameter description
  * @return   ��������ı��Σ���λ���ף�
  * @note     detailed description �޸���  �޸�ʱ��  �޸�����
  * @date     create date                                
  * @exception  exception object exception description  
  * @remarks remark text               
  * @deprecated   description            
  * @since when time or version           
  * @see references            
*/ 
QRectF * RadarMosaicDataProvider::GetRect( float lat,float lon )
{
	Proj4Transform * proj = new Proj4Transform;
	proj->setSourceCRS("+proj=longlat +ellps=WGS84 +datum=WGS84 +no_defs");
	proj->setSourceProjName("��ͶӰ");
	proj->setDestCRS("+proj=lcc +lat_1=30 +lat_2=62 +lat_0=0 +lon_0=110 +x_0=0 +y_0=0 +a=6378140 +b=6356755.288157528 +units=m +no_defs");
	proj->setDesProjName("�״�����������ͶӰ");
	osg::Vec3  src(lon,lat,0),des;
	proj->Transform( src,des);

	proj->setSourceCRS("+proj=lcc +lat_1=30 +lat_2=62 +lat_0=0 +lon_0=110 +x_0=0 +y_0=0 +a=6378140 +b=6356755.288157528 +units=m +no_defs");
	proj->setSourceProjName("�״�����������ͶӰ");
	proj->setDestCRS("+proj=longlat +ellps=WGS84 +datum=WGS84 +no_defs");
	proj->setDesProjName("��ͶӰ");

	osg::Vec3  src1(des.x()-5000,des.y()-5000,0),des1;
	osg::Vec3  src2(des.x()+5000,des.y()+5000,0),des2;
	des1 = proj->Transform(src1);
	des2 = proj->Transform(src2);

	QRectF ref;
	ref.setTop(des2.y());
	ref.setBottom(des1.y());
	ref.setLeft(des1.x());
	ref.setRight(des2.x());

	delete proj;
	proj = NULL;

	return &ref;

}

/** 
  * @brief    ��ȡBref�ļ�     
  * @author   ��־ѧ  
  * @param    filename �ļ�·��
  * @param [out] parameter name   parameter description
  * @return   return data
  * @note     detailed description �޸���  �޸�ʱ��  �޸�����
  * @date     create date                                
  * @exception  exception object exception description  
  * @remarks remark text               
  * @deprecated   description            
  * @since when time or version           
  * @see references            
*/ 
bool RadarMosaicDataProvider::LoadRefFile( const std::string& filename )
{
	if (string::npos == filename.find("BREF"))
		return false;

	m_fileName = QString(filename.c_str());
	GDataFiles* gDataFile = NULL;
	gData = new GenerationData();
	gDataFile  = (GDataFiles*)gData;

	int ret = gDataFile->ReadFrom((char *)filename.c_str());
	if (ret != 0)
	{
		return false;
	}

	pMosaicGdata = (MosaicDataProvider *)gData;
	std::vector<SAVEELE_INFO> save_info ;
	pMosaicGdata->LoadWeaEleAlgorithm(save_info);
	for (int ii = 0 ;ii<save_info.size();ii++)
	{
		SAVEELE_INFO info = save_info.at(ii);
		QRectF* rect =  GetRect(info.Lat,info.Lon);
		QList<STATION_INFO>* station_inf = GetStationInfoByRect(rect,info.id);
		setAngle(info.Lat,info.Lon,info.Hgt,*station_inf);
	}
	return true;
}

QList<STATION_INFO>* RadarMosaicDataProvider::GetStationInfoByRect(QRectF *rect,int id)
{
	QList<STATION_INFO> infoList;
	if (!m_db.isOpen())
	{
		m_db.open();
	}
	QSqlQuery query(m_db);

	QString sql;
	sql.append("select code,longitude,latitude from r_work_station where longitude >");
	sql.append(QString::number(rect->left()));
	sql.append(" and longitude <");
	sql.append(QString::number(rect->right()));
	sql.append(" and latitude <");
	sql.append(QString::number(rect->bottom()));
	sql.append(" and latitude >");
	sql.append(QString::number(rect->top()));

	bool bsuccess = query.exec(sql);
	while(query.next())
	{
		STATION_INFO info;
		info.id = id;
		info.code = query.value(0).toInt();
		info.Lon = query.value(1).toDouble();
		info.Lat = query.value(2).toDouble();
		infoList.push_back(info);
	}
	return &infoList;
}

bool RadarMosaicDataProvider::setAngle( float lat,float lon ,float s_hight,QList<STATION_INFO>& station_info )
{
	Proj4Transform * proj = new Proj4Transform;
	proj->setSourceCRS("+proj=longlat +ellps=WGS84 +datum=WGS84 +no_defs");
	proj->setDestCRS("+proj=lcc +lat_1=30 +lat_2=62 +lat_0=0 +lon_0=110 +x_0=0 +y_0=0 +a=6378140 +b=6356755.288157528 +units=m +no_defs");
	osg::Vec3  src(lon,lat,0),des;
	proj->Transform( src,des);

	for(int i = 0 ; i<station_info.size();i++)
	{
		STATION_INFO info = station_info.at(i);
		osg::Vec3  tempsrc(info.Lon,info.Lat,0),tempdes;
		proj->Transform( tempsrc,tempdes);
		double dis = Get_Length(tempdes.x() - des.x(),tempdes.y() - des.y());
		double angle_elev = atan(s_hight /dis);
		info.elevation = angle_elev*180/M_PI;
		info.azimuthal =  180 *Get_Angle_Of_Direction(tempdes.x() - des.x(),tempdes.y() - des.y())/M_PI;
		info.start = QDateTime::currentDateTime();
		info.start = QDateTime::currentDateTime().addSecs(1000);
		info.count = 50;
	}
	return true;
}


double RadarMosaicDataProvider::Get_Angle_Of_Direction( double dx, double dy )
{
	//if ( dx == 0 && dy>0 )
	//{
	//	return 0;
	//}
	if (dx == 0 && dy < 0)
	{
		return M_PI;
	}
	if ( dx>0 && dy==0)
	{
		return M_PI / 2.0;
	}
	if ( dx<0 && dy==0 )
	{
		return M_PI * 3.0 / 2.0;
	}
	if ( dx>0 && dy<0 )
	{
		return atan2(dx,dy);
	}
	if ( dx>0 && dy >0)
	{
		return M_PI - atan2(dx,-dy);
	}
	if ( dx<0 && dy<0 )
	{
		return 2*M_PI - atan2(-dx,dy);
	}
	if ( dx<0 && dy>0 )
	{
		return M_PI + atan2(-dx,-dy);
	}
	return 0;
}

double RadarMosaicDataProvider::Get_Length( double dx, double dy )
{
	return( sqrt(dx*dx + dy*dy) );
}

void RadarMosaicDataProvider::AreaAdd(bool tArea,bool tPie,float tMin,float tMax,osg::Vec3Array* p)
{
	AreaStatistic *pAreaS = new AreaStatistic;
	if (p != 0)
	{
		CoordinateTransform* ct = ParentLayer()->GetParentMap()->GetCoordinateTransform();
		Proj4Transform pj4;
		pj4.setSourceCRS(ct->getDesCRS());
		pj4.setDestCRS(ct->getSourceCRS());

		osg::Vec3 v0 = pj4.Transform(p->at(0));
		osg::Vec3 v1 = pj4.Transform(p->at(1));

		p->clear();
		p->push_back(v0);
		p->push_back(v1);

		pAreaS->SetPoint(p);
	}
	pAreaS->AreaAdd(tArea,tPie,tMin,tMax,this);
}

QString RadarMosaicDataProvider::GetDataUnit()
{
	int productcode = pMosaicGdata->ProductCode();

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

float RadarMosaicDataProvider::GetPositionValue( double lon,double lat,double height )
{
	pMosaicGdata = (MosaicDataProvider*)gData;
	return pMosaicGdata->GetValue(lon,lat,height,m_pParentLayer->CoordTrans());
}

