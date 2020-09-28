#include <osg/Billboard>
#include "Layer.h"
#include "earthquakedataprovider.h"
#include <QTextStream>
#include <QtCore/QFile>
#include <QtCore/QFileInfo>

#include "On_MECA1_Draw.h"
#include "On_MECA2_Draw.h"
#include "On_MECA3_Draw.h"
#include "On_MECA4_Draw.h"
#include "On_MECA5_Draw.h"
#include "On_MECA6_Draw.h"
#include "On_Profile_Draw.h"
#include "OpenCubeData.h"

goto_gis::DataProvider* g_EarthquakeDataProvider = 0;
extern "C"
{
	DataProvider Q_DECL_EXPORT * DataProviderPluginMain()
	{
		g_EarthquakeDataProvider = new EarthquakeDataProvider;
		return g_EarthquakeDataProvider;
	}
}

EarthquakeDataProvider::EarthquakeDataProvider()
{
	pProductDraw = NULL;
}

EarthquakeDataProvider::~EarthquakeDataProvider()
{

}

/** 
  * @brief   加载地震数据文件    
  * @author  陈志学   
  * @param   filename 数据文件全路径
  * @param [out] parameter name   parameter description
  * @return   bool 打开成功放回true 失败 false
  * @note     detailed description 修改人  修改时间  修改内容
  * @date     create date                                
  * @exception  exception object exception description  
  * @remarks remark text               
  * @deprecated   description            
  * @since when time or version           
  * @see references            
*/ 
bool EarthquakeDataProvider::LoadFile( const std::string& filename )
{
	m_fileName = QString(filename.c_str());
	QString suffix = QFileInfo(m_fileName).suffix();

	QFile file(m_fileName);
	
	if (!file.open(QIODevice::ReadOnly))
	{
		return 0;
	}
	pProductDraw = GetProductDrawInstance(GetFileType(file));
	pProductDraw->LoadFile(m_fileName);
	DumpProduct(m_pParentLayer);
	//osg::HeightField *field = osgDB::readHeightFieldFile("E:\\DATA\\DEM\\as_dem_ProjectRaster11.img");
	return true;
}

/** 
  * @brief    绘制地震数据   
  * @author   陈志学   
  * @param    pLayer 数据索要绘制的图层
  * @param [out] parameter name   parameter description
  * @return   void
  * @note     detailed description 修改人  修改时间  修改内容
  * @date     create date                                
  * @exception  exception object exception description  
  * @remarks remark text               
  * @deprecated   description            
  * @since when time or version           
  * @see references            
*/ 

void EarthquakeDataProvider::DumpProduct( Layer *pLayer )
{
	pProductDraw->DumpAll(pLayer);
}

/** 
  * @brief    获得地震文件的解析绘制实例   
  * @author   陈志学   
  * @param    nProductCode 数据类型
  * @param [out] parameter name   parameter description
  * @return   EQProductionDraw 绘制基类实例指针
  * @note     detailed description 修改人  修改时间  修改内容
  * @date     create date                                
  * @exception  exception object exception description  
  * @remarks remark text               
  * @deprecated   description            
  * @since when time or version           
  * @see references            
*/ 
EQProductionDraw* EarthquakeDataProvider::GetProductDrawInstance( EARTHQUAKE_DATA_TYPE nProductCode )
{
	EQProductionDraw* p=NULL;

	switch(nProductCode)
	{
	case STRUCT_MECA1:
		p= new On_MECA1_Draw;   //added by hdm 20090211
		break;
	case STRUCT_MECA2:
		p = new On_MECA2_Draw;
		break;
	case STRUCT_MECA3:
		p= new On_MECA3_Draw;
		break;
	case STRUCT_MECA4:
		p = new On_MECA4_Draw;
		break;
	case STRUCT_MECA5:
		p = new On_MECA5_Draw;
		break;
	case STRUCT_MECA6:
		p = new On_MECA6_Draw;
		break;
	case STRUCT_MECA8:
		p= NULL;
		break;
	case STRUCT_MECA9:
		p=NULL;
		break;
	case STRUCT_PROFILE:
		p = new On_Profile_Draw;
		break;
	case STRUCT_RASTER_ASC:
		p = new On_Profile_Draw;
		break;
	case STRUCT_RASTER_XYZ:
		p = new On_Profile_Draw;
		break;
	case STRUCT_CUBE_DATA:
		p = new OpenCubeData;
		break;
	case UNKONW_TYPE:
		p=NULL;
		break;

	default:
		p=NULL;
		break;
	}
	return p;
}

/** 
  * @brief    销毁数据文件绘制实例    
  * @author   陈志学   
  * @param     lpInstance   数据文件绘制实例指针
  * @param [out] parameter name   parameter description
  * @return   return data
  * @note     detailed description 修改人  修改时间  修改内容
  * @date     create date                                
  * @exception  exception object exception description  
  * @remarks remark text               
  * @deprecated   description            
  * @since when time or version           
  * @see references            
*/ 
void EarthquakeDataProvider::ReleaseProductDrawInstance( EQProductionDraw* lpInstance )
{
	EQProductionDraw* p= lpInstance;
	if(p!=NULL)
	{
		delete p;
		p=NULL;
	}
}

/** 
  * @brief   解析文件类型     
  * @author  陈志学   
  * @param   f 文件对象
  * @param [out] parameter name   parameter description
  * @return   MECA_DATA_TYPE 文件类型枚举
  * @note     detailed description 修改人  修改时间  修改内容
  * @date     create date                                
  * @exception  exception object exception description  
  * @remarks remark text               
  * @deprecated   description            
  * @since when time or version           
  * @see references            
*/ 
EARTHQUAKE_DATA_TYPE EarthquakeDataProvider::GetFileType( QFile &f )
{
	QFileInfo fileInfo(f);

	if(fileInfo.suffix() == "in")
		return STRUCT_CUBE_DATA;

	QRegExp rx("\\s+|\\t");  //至少一个空格和只有一个制表符
	QTextStream in(&f);

	QString head = in.readLine();
	if (head.contains("\n"))
	{
		head.replace("\n","");
	}
	QStringList headlist = head.split(rx,QString::SkipEmptyParts);
	if (headlist.count() == 0  )
	{
		return UNKONW_TYPE;
	}
	
	/*
	else if (headlist.count()==2 && headlist.at(1).toInt()==1)
	{
		return STRUCT_PROFILE;
	}
	*/
	if (headlist.size()==2 && headlist.at(0) == "#")
	{
		if ("1" == headlist.at(1))
		{
			return STRUCT_MECA1;
		}
		else if ("2" == headlist.at(1))
		{
			return STRUCT_MECA2;
		}
		else if ("3" == headlist.at(1))
		{
			return STRUCT_MECA3;
		}
		else if ("4" == headlist.at(1))
		{
			return STRUCT_MECA4;
		}
		else if ("5" == headlist.at(1))
		{
			return STRUCT_MECA5;
		}
		else if ("6" == headlist.at(1))
		{
			return STRUCT_MECA6;
		}
		else if ("7" == headlist.at(1))
		{
			return STRUCT_MECA7;
		}
		else if ("8" == headlist.at(1))
		{
			return STRUCT_MECA8;
		}
	}
	else if (headlist.size()==2 && headlist.at(0) == "ncols")
	{
		return STRUCT_RASTER_ASC;
	}
	else if (headlist.size()==3)
	{
		return STRUCT_PROFILE;
	}

	/*else if (headlist.at(0) != "#")
	{
		return UNKONW_TYPE;
	}*/
	

	return UNKONW_TYPE;
}
