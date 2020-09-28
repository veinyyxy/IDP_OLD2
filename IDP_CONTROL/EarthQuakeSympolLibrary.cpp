#include <QtCore/QFileInfo>
#include <QtCore/QCoreApplication>

#include <osg/Node>
#include "LegendInterface.h"
#include "EarthQuakeSympolLibrary.h"


EarthQuakeSympolLibrary::EarthQuakeSympolLibrary(void)
{
	AddLegend(0);
}


EarthQuakeSympolLibrary::~EarthQuakeSympolLibrary(void)
{
}

int EarthQuakeSympolLibrary::GetSymbolColor( double dvalue, osg::Vec4* v4Color )
{
	return 0;
}

int EarthQuakeSympolLibrary::GetSymbolSize( double dvalue, double* size )
{
	return 0;
}

int EarthQuakeSympolLibrary::GetSymbolStyle( double dvalue, osg::Geometry** pGeometry )
{
	return 0;
}

int EarthQuakeSympolLibrary::GetSymbolStyle( double dvalue, int* FontID )
{
	if(m_iLibType != 1) return 0;
	return 0;
}

int EarthQuakeSympolLibrary::GetSymbolStyle( double dvalue, osg::Node** pNode )
{
	if(m_iLibType != 0) return 0;
	
	if(abs(dvalue - 0) < 0.00001)
	{
		*pNode = m_NodeLibList[dvalue];
	}
	else if(abs(dvalue - 1) < 0.0001)
	{
		*pNode = m_NodeLibList[dvalue];
	}
	else
	{
		*pNode = 0;
		return 0;
	}
	return 1;
}

int EarthQuakeSympolLibrary::InitSymbolLibrary( std::string strLibName )
{
	QFileInfo temDir(strLibName.data());
	QString fileSuf = temDir.suffix();

	if(fileSuf == "3ds")
	{
		m_iLibType = 0;

		QString file1 = QCoreApplication::applicationDirPath()+QString::fromLocal8Bit("/Config/arrow1.3DS");
		QString file2 = QCoreApplication::applicationDirPath()+QString::fromLocal8Bit("/Config/arrow2.3DS");
		osg::Node* pArrowNode1 = osgDB::readNodeFile(file1.toStdString());
		osg::Node* pArrowNode2 = osgDB::readNodeFile(file2.toStdString());

		m_NodeLibList.insert(std::pair<double, osg::Node*>(0, pArrowNode1));
		m_NodeLibList.insert(std::pair<double, osg::Node*>(1, pArrowNode2));
	}
	else if(fileSuf == "ttf")
	{
		m_iLibType = 1;


	}
	else
		return 0;
	return 1;
}
