#include "VolumeRadarSingleVTKProvider.h"
#include <QtCore/QCoreApplication>
#include <QtCore/QDateTime>

#define  racial 10000.000
#define  km  1000.000

RadarSingleVTKProvider::RadarSingleVTKProvider(void):pDrawProduct(0),pSingleGdata(0),gData(0)
{
	cube = vtkPolyData::New();
	pColorTable=vtkLookupTable::New();
	proj = new goto_gis::RadProjTransform;
}


RadarSingleVTKProvider::~RadarSingleVTKProvider(void)
{
	cube->Delete();
	cubeMapper->Delete();
	cubeActor->Delete();
	camera->Delete();
	renderer->Delete();
	renWin->Delete();
	iren->Delete();
	pColorTable->Delete();
}

vtkImageData * RadarSingleVTKProvider::LoadFile( const std::string& filename )
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

	pSingleGdata = (ProductBase *)gData;
	int ret = gDataFile->ReadFrom((char *)filename.c_str());
	if(ret!=0)
	{
		return NULL;
	}

	//proj = NULL;
	proAttr pAttributeHaHa;
	if (pDrawProduct != NULL)
	{
		pDrawProduct->ReadInfo(&pAttributeHaHa,0);
	}
	osg::Vec3 p = osg::Vec3(pAttributeHaHa.latitude/10000.0,pAttributeHaHa.longitude /10000.0,(float)pAttributeHaHa.altitude*0.1);

	
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

	if (!InitColorTable())
	{
		return NULL;
	}
	double maxvalue = 0.0;
	m_interpolation = new InterpolationAlgorithm;
	m_interpolation->setcDrawProduct(pDrawProduct);
	return m_interpolation->GetVolumeData(m_pColorValueIndexHash);	
}

bool RadarSingleVTKProvider::InitColorTable()
{
	QString appConfigPth = QCoreApplication::applicationDirPath().remove("/bin") + "/config/";

	COLORTABLE_HASH        m_hColorTableHash;

	xml_reader.ReadFile("E:/Cpp/VTKtoOSG/Debug/Config/basicInfo/m_single_radar_color.xml");

	m_hColorTableHash = (xml_reader.ColorTable());
	int productcode = pSingleGdata->GetPDBlock()->ProductCode;
	m_ColorTable =  m_hColorTableHash.value(productcode);

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
		double m_nParam;
		if (pAttributeHaHa.pro_code == 254 || pAttributeHaHa.pro_code == 255 || pAttributeHaHa.pro_code == 301) //301-IW-逆风区
		{
			m_nParam = pAttributeHaHa.param1;
		}
		for (int ii = 0; ii<m_ColorTable->count();ii++){
			m_ColorTable->at(ii)->fStart=(m_ColorTable->at(ii)->fStart)/100*m_nParam;
			m_ColorTable->at(ii)->fEnd=(m_ColorTable->at(ii)->fEnd)/100*m_nParam;
			m_hColorTableHash.value(productcode)->at(ii)->fStart=m_ColorTable->at(ii)->fStart;
			m_hColorTableHash.value(productcode)->at(ii)->fEnd=m_ColorTable->at(ii)->fEnd;
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
		}/*else if (254==productcode||255==productcode||301==productcode)
		{
			step = 15*m_nParam/100;
			key = (int)(fstart/step );
		}*/else if (256==productcode||257==productcode)
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
			key = GetColorIndexKey(productcode,fstart);
		}
		m_pColorValueIndexHash.insert(key,QVector4D(color->nRed/255.0 ,color->nGreen/255.0,color->nBlue/255.0,1.0));
	}

	pColorTable->SetNumberOfColors(m_ColorTable->count());
	for (int i = 0; i<m_ColorTable->count();i ++)
	{
		P_COLORNODE_PRODUCT color = m_ColorTable->at(i);	
		pColorTable->SetTableValue(i,color->nRed/255.0 ,color->nGreen/255.0,color->nBlue/255.0,1.0);

	}
	pColorTable->Build();
	return true;
}


int RadarSingleVTKProvider::GetColorIndexKey( int productcode,double fstart )
{
	double step = 0.0;
	if (264 == productcode||265 == productcode)
	{
		if (fstart<0)
		{
			return 99999;
		}
		if (fstart<3.0)
		{
			step = 3;
			return 1;		
		}
		if (fstart<10.0)
		{
			step = 1;
			return (int)(fstart/ step) +10;
		}

		step = 2;
		return (int)(fstart/ step) +20;

	}else if (266 == productcode)
	{
		if (fstart<2.01)
		{
			step = 1;
			return (int)(fstart/ step);
		}
		if (fstart<5)
		{
			step = 3;
			return 11;
		}
		if (fstart<21)
		{
			step = 5;
			return(int)(fstart/ step) +20;
		}
		if (fstart<71)
		{
			step = 10;
			return (int)(fstart/ step) +30;
		}
		step = 30;
		return (int)(fstart/ step) +40;
	}else if (267 == productcode)
	{
		if (fstart<0.1)
		{
			return 99999;
		}
		if (fstart<0.5)
		{
			step = 1;
			return 1;
		}
		if (fstart<1)
		{
			step = 3;
			return 11;
		}
		if (fstart<5)
		{
			step = 0.5;
			return (int)(fstart/ step) +20;
		}
		if (fstart<10)
		{
			step = 1;
			return (int)(fstart/ step) +30;
		}
		return 41;
	}else if (270 == productcode||300 == productcode||298 == productcode||299 == productcode)
	{
		if (fstart<0.1)
		{
			return 99999;
		}
		if (fstart<0.31)
		{
			step = 0.1;
			return (int)(fstart/ step);
		}
		if (fstart<0.5)
		{
			step = 0.2;
			return 11;
		}
		if (fstart<1.0)
		{
			step =0.5;
			return 21;
		}
		if (fstart<2.0)
		{
			step = 1;
			return 31;
		}
		if (fstart<5)
		{
			step = 3;
			return 41;
		}
		if (fstart<10)
		{
			step = 5;
			return 51;
		}
		if (fstart<20)
		{
			step = 10;
			return 61;
		}
		if (fstart<50)
		{
			step = 20;
			return 71;
		}
		step = 50;
		return (int)(fstart/ step) +80;
	}else if (271 == productcode||272 == productcode||273 == productcode||274 == productcode||297 == productcode)
	{
		if(fstart<0.1)
		{
			return 99999; 
		}
		if (fstart<1.0)
		{
			//step = 0.9;
			return 1;
		}
		if (fstart<2.0)
		{
			step = 1;
			return 6;
		}
		if (fstart<5.0)
		{
			step = 2;
			return 11;
		}
		if (fstart<20.1)
		{
			step = 5;
			return (int)(fstart/ step) +20;
		}
		if (fstart<50.1)
		{
			step = 10;
			return (int)(fstart/ step) +30;
		}
		if (fstart<80.1)
		{
			step = 30;
			return (int)(fstart/ step) +40;
		}
		if (fstart<100.1)
		{
			step = 20;
			return (int)(fstart/ step) +50;
		}
		step = 50;
		return (int)(fstart/ step) +60;
		
	}
	return 0;
}