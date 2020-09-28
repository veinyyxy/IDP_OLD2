#include "GDALProvider.h"
#include "goConfig.h"
#include "goGeometry.h"
#include "goMark.h"
#include <QString>
#include <QStringList>
#include <QtCore/QFileInfo>
using namespace goto_gis;

extern "C"
{
	DataProvider Q_DECL_EXPORT * DataProviderPluginMain()
	{
		sqliteProvider = new GDALProvider;
		return sqliteProvider;
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

		QString strSuffix("shp");
		QString strSuffix1 = fileInfo.suffix();
		if(strSuffix1 == strSuffix)
			return true;
		else
			return false;
	}
}

int GDALProvider::gcount = 0;
GDALProvider* GDALProvider::m_pGDALProvider = 0;

GDALProvider::GDALProvider(): DataProvider()
{
	
}


GDALProvider::~GDALProvider(void)
{

}

GDALProvider * GDALProvider::Instance()
{
	if (!m_pGDALProvider)
	{
		m_pGDALProvider = new GDALProvider();
	}
	return m_pGDALProvider;
}

bool GDALProvider::LoadFile(const std::string& filename)
{
	gcount = 0;
	OGRRegisterAll();//注册所有要读取的格式    
	// 对中文路径的支持  
	CPLSetConfigOption("GDAL_FILENAME_IS_UTF8", "NO");  
	CPLSetConfigOption("SHAPE_ENCODING","");
	OGRDataSource* poDS = OGRSFDriverRegistrar::Open(filename.c_str(), FALSE );   
	if( poDS == NULL )   
	{   
		//WriteLog("Open failed" );
		return false;   
	}
	//FILE *fp;   
	//string fileName;
	//fileName = Getymdhms("yyyy-MM-dd hh:mm:ss")+string(".wkt");
	//fp = fopen(fileName.c_str(),"w+");   

	//int Layercount = poDS->GetLayerCount();    
	OGRLayer * poLayer = poDS->GetLayer(0);
	OGRFeature *poFeature;       
	poLayer->ResetReading();     //调用Layer::ResetReading()来确保是从层的开头开始。    
	char** wkt = new char*[poLayer->GetFeatureCount()];//分配空间    
	//unsigned char* wkb = new unsigned char[poLayer->GetFeatureCount()];//分配空间    

	//int count = 0;
	Layer::MgoGeometry* pMG = m_pParentLayer->GetMgoGeometry();
	while( (poFeature = poLayer->GetNextFeature()) != NULL )   
	{
		OGRGeometry* poGeometry = poFeature->GetGeometryRef();
		if( poGeometry != NULL)    
		{
#if 1
			poGeometry->exportToWkt(wkt);
			geos::io::WKTReader wktReader;
			geom::Geometry* pGeometry = wktReader.read(string(*wkt));
#else	
			poGeometry->exportToWkb(wkbNDR, wkb);
			geos::io::WKBReader wkbReader;

			std::ifstream ifile(filename.data());
			geom::Geometry* pGeometry = wkbReader.read(ifile);

#endif
			switch (wkbFlatten(poGeometry->getGeometryType()))   
			{  
			case wkbPoint:   
				{
					goPoint* gp = new goPoint(pGeometry, m_pParentLayer->CoordTrans(), m_pParentLayer);
					LoadField(poLayer,poFeature,pGeometry,gp);       
					pMG->insert(Layer::MgoGeometryPair(pMG->size(),gp));
				}break;
			case wkbLineString:   
				{   
					goLineString* gls = new goLineString(pGeometry, m_pParentLayer->CoordTrans(), m_pParentLayer);
					LoadField(poLayer,poFeature,pGeometry,gls); 
					pMG->insert(Layer::MgoGeometryPair(pMG->size(),gls));
				}break;
			case wkbPolygon:   
				{   
					goPolygon* gpl = new goPolygon(pGeometry, m_pParentLayer->CoordTrans(), m_pParentLayer);
					LoadField(poLayer,poFeature,pGeometry,gpl);        
					pMG->insert(Layer::MgoGeometryPair(pMG->size(),gpl));
				}break;
			case  wkbMultiPoint:
				{
					goMultiPoint* gmp = new goMultiPoint(pGeometry, m_pParentLayer->CoordTrans(), m_pParentLayer);
					LoadField(poLayer,poFeature,pGeometry,gmp);        
					pMG->insert(Layer::MgoGeometryPair(pMG->size(),gmp));
				}break;
			case  wkbMultiLineString:
				{
					//QStringList lines = (QString::fromLocal8Bit(*wkt).remove("MULTILINESTRING ((").remove("))")).split("),(",QString::SkipEmptyParts);
					//foreach(QString line , lines)
					//{
					//	QString lineWkt = "LINESTRING ("+line+")";
					//	geom::Geometry* pGeometry = wktReader.read(lineWkt.toStdString());

					//	goLineString* gls = new goLineString(pGeometry, m_pParentLayer->CoordTrans(), m_pParentLayer);
					//	LoadField(poLayer,poFeature,gls); 
					//	pMG->insert(Layer::MgoGeometryPair(count,gls));
					//}

					goMultiLineString* gmls = new goMultiLineString(pGeometry, m_pParentLayer->CoordTrans(), m_pParentLayer);
					LoadField(poLayer,poFeature,pGeometry,gmls);        
					pMG->insert(Layer::MgoGeometryPair(pMG->size(),gmls));
				}break;
			case  wkbMultiPolygon:
				{
					//QStringList polygons = (QString::fromLocal8Bit(*wkt).remove("MULTIPOLYGON (((").remove(")))")).split(")),((",QString::SkipEmptyParts);
					//foreach(QString polygon , polygons)
					//{
					//	QString polygonWkt = "POLYGON (("+polygon+"))";
					//	geom::Geometry* pGeometry = wktReader.read(polygonWkt.toStdString());

					//	goPolygon* gpl = new goPolygon(pGeometry, m_pParentLayer->CoordTrans(), m_pParentLayer);
					//	LoadField(poLayer,poFeature,gpl,count);        
					//	pMG->insert(Layer::MgoGeometryPair(count,gpl));
					//}
					goMultiPolygon* gmp = new goMultiPolygon(pGeometry, m_pParentLayer->CoordTrans(), m_pParentLayer);
					LoadField(poLayer,poFeature,pGeometry,gmp);        
					pMG->insert(Layer::MgoGeometryPair(pMG->size(),gmp));
				}break;
			case  wkbLinearRing:
				{
					goLinearRing* glr = new goLinearRing(pGeometry, m_pParentLayer->CoordTrans(), m_pParentLayer);
					LoadField(poLayer,poFeature,pGeometry,glr);        
					pMG->insert(Layer::MgoGeometryPair(pMG->size(),glr));
				}break;
			}
			//count++;
		}   
		OGRFeature::DestroyFeature( poFeature );   
	}       
	OGRDataSource::DestroyDataSource( poDS );   
	//fclose(fp);
	return true;
}


void GDALProvider::LoadField(OGRLayer *poLayer ,  OGRFeature *poFeature  ,geom::Geometry* goGeom, goGeometry* goGeo)
{
	//char ss[256];
	//const char* layerName = poLayer->GetName();
	//sprintf(ss, "%s,", layerName );
	OGRFeatureDefn *poFDefn = poLayer->GetLayerDefn();  
	Layer::MgoGeometry* pMG = m_pParentLayer->GetMgoGeometry();
	//char sss[256];
	//const char* name = poFDefn->GetName();
	//sprintf(sss, "%s,", name);

	for(int  iField = 0; iField < poFDefn->GetFieldCount(); iField++ )   
	{   
		GeomField geomFiled;
		OGRFieldDefn *poFieldDefn = poFDefn->GetFieldDefn( iField );
		if( poFieldDefn->GetType() == OFTInteger )
		{
			int field = poFeature->GetFieldAsInteger(iField);
			char s[256];
			sprintf(s, "%d", field); 
			geomFiled.setComment(s);
		}
		else if( poFieldDefn->GetType() == OFTReal )
		{
			double field = poFeature->GetFieldAsDouble(iField);
			char s[256];
			sprintf(s, "%d", field); 
			geomFiled.setComment(s);
		}
		else if( poFieldDefn->GetType() == OFTString )
		{
			const char* field= poFeature->GetFieldAsString(iField);
			geomFiled.setComment(field);
		}
		//else if (poFieldDefn->GetType() == OFTIntegerList)
		//{
		//	const int* field = poFeature->GetFieldAsIntegerList(iField,&nItemCount);
		//}
		//else if (poFieldDefn->GetType() == OFTRealList)
		//{
		//	const double* field = poFeature->GetFieldAsDoubleList(iField,&nItemCount);
		//}
		//else if (poFieldDefn->GetType() == OFTStringList)
		//{
		//	char** field = poFeature->GetFieldAsStringList(iField);
		//}
		//else if (poFieldDefn->GetType() == OFTWideString)
		//{
		//	
		//}
		//else if (poFieldDefn->GetType() == OFTWideStringList)
		//{
		//}
		//else if (poFieldDefn->GetType() == OFTBinary)
		//{
		//	GByte * field = poFeature->GetFieldAsBinary(iField,&nItemCount);
		//}
		//else if (poFieldDefn->GetType() == OFTDate)
		//{
		//}
		//else if (poFieldDefn->GetType() == OFTTime)
		//{
		//}
		//else if (poFieldDefn->GetType() == OFTDateTime)
		//{
		//	int *pnYear; int *pnMonth; int *pnDay;int *pnHour;int *pnMinute; int *pnSecond; 	int *pnTZFlag;
		//	int field = poFeature->GetFieldAsDateTime(iField,pnYear,pnMonth,pnDay,pnHour,pnMinute,pnSecond,pnTZFlag);
		//}
		//else if (poFieldDefn->GetType() == OFTMaxType)
		//{
		//}
		else
		{
			const char* field = poFeature->GetFieldAsString(iField);
			geomFiled.setComment(field);
		}
		geomFiled.setName(poFieldDefn->GetNameRef());
		geomFiled.setLength(poFDefn->GetFieldCount());
		geomFiled.setPrecision(poFieldDefn->GetPrecision());
		geomFiled.setType(poFieldDefn->GetType());
		geomFiled.setTypeName(poFieldDefn->GetFieldTypeName(poFieldDefn->GetType()));

		geomFiled.setX(goGeom->getCoordinate()->x);
		geomFiled.setY(goGeom->getCoordinate()->y);
		geomFiled.setZ(m_pParentLayer->LayerZ());
		gcount++;
		m_pParentLayer->GetFieldMap()->insert(FIELD_MAPPair(m_pParentLayer->GetFieldMap()->size(),geomFiled));
		if (geomFiled.name() == m_pParentLayer->ShowFiedName())
		{
			goGeo->GeometryName(geomFiled.comment());
			goMark* gm = new goMark(geomFiled.comment(),osg::Vec3(goGeom->getCoordinate()->x,goGeom->getCoordinate()->y,m_pParentLayer->LayerZ()),m_pParentLayer);
			pMG->insert(Layer::MgoGeometryPair(pMG->size(),gm));
		}
	}
	
}

//wchar_t* GDALProvider::CharToWchar(const char* c)  
//{  
//	int len = MultiByteToWideChar(CP_ACP,0,c,strlen(c),NULL,0);  
//	wchar_t* m_wchar=new wchar_t[len+1];  
//	MultiByteToWideChar(CP_ACP,0,c,strlen(c),m_wchar,len);  
//	m_wchar[len]='\0';  
//	return m_wchar;  
//}  

string GDALProvider::Getymdhms(string format)
{
	//yyyy-MM-dd hh:mm:ss
	time_t timeStruct;   
	struct tm *p;   
	time(&timeStruct);
	p=localtime(&timeStruct);   

	char year [20];;
	_itoa(1900+p->tm_year, year, 10);
	char mon [20];;
	_itoa(1+p->tm_mon, mon, 10);
	char day [20];
	_itoa(p->tm_mday, day, 10);

	if (format == "yyyy-MM-dd hh:mm:ss")
	{
		char hour[20];
		_itoa(p->tm_hour, hour, 10);
		char min[20];
		_itoa(p->tm_min, min, 10);
		char sec[20];
		_itoa(p->tm_sec, sec, 10);

		return string (year)+"-"+string(mon)+"-"+string(day)+"_"+string(hour)+"-"+string(min)+"-"+string(sec);
	}
	else if (format == "yyyy-MM-dd")
	{
		return string (year)+"-"+string(mon)+"-"+string(day);
	}

	//cout<<1900+p->tm_year<<'-'<<1+p->tm_mon<<'-'<<p->tm_mday<<endl;   
	////seconds=   time((time_t*)NULL);   是以1900-01-01   00:00:00   为标准的
	////p->tm_year是一个相对1900的相对变量间隔时间  （）相关参考下面time.h文件
	//cout<<p->tm_hour<<':'<<p->tm_min<<':'<<p->tm_sec<<endl<<endl;
	return string (year)+"-"+string(mon)+"-"+string(day);
}

bool GDALProvider::CreateFile( const std::string& filename ,OGRwkbGeometryType geomType)
{
	//OGRRegisterAll();//注册所有要读取的格式    
	//// 对中文路径的支持  
	//CPLSetConfigOption("GDAL_FILENAME_IS_UTF8", "NO");  
	//CPLSetConfigOption("SHAPE_ENCODING","");
	//QFileInfo info(QString::fromStdString(filename));

	//OGRSFDriver* poDriver = OGRSFDriverRegistrar::GetRegistrar()->GetDriverByName("ESRI Shapefile");
	//OGRDataSource* poDS = poDriver->CreateDataSource( filename.c_str(), NULL );
	//OGRLayer* poLayer= poDS->CreateLayer(info.baseName().toStdString().c_str(), NULL, geomType, NULL );


	//
	//OGRFieldDefn oField1("GeoObjNum",OFTString);// 字符串
	//oField1.SetWidth(8);
	//if( poLayer->CreateField( &oField1 ) != OGRERR_NONE )
	//{

	//}
	//OGRFieldDefn oField2("LBTG",OFTReal);// 浮点数
	//oField2.SetPrecision(3);
	//if( poLayer->CreateField( &oField2 ) != OGRERR_NONE )
	//{
	//
	//}	
	//OGRFieldDefn oField3("Number",OFTInteger);//// 整型
	//if( poLayer->CreateField( &oField3 ) != OGRERR_NONE )
	//{

	//}

	//OGRFeature *poFeature;
	//poFeature =new OGRFeature( poLayer->GetLayerDefn() );

	//poFeature->SetField( "GeoObjNum", strGeoObjNum );
	//poFeature->SetField( "LBTG", fLBTG );
	//poFeature->SetField( "Number", number );

	//OGRLineString *poLine =new OGRLineString();

	//poLine->setNumPoints(2);
	//poLine->setPoint(0,startX,startY, 0.0);
	//poLine->setPoint(1,endX,endY, 0.0);

	//poFeature->SetGeometryDirectly( poLine ); 
	//if( poLayer->CreateFeature( poFeature ) != OGRERR_NONE )
	//{

	//}
	//OGRDataSource::DestroyDataSource( poDS );
	//OGRCleanupAll();
	return true;
}
