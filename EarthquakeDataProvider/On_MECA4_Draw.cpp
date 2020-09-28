#include <osg/Billboard>
#include <osg/ShapeDrawable>
#include <osg/Shape>
#include <osg/PositionAttitudeTransform>
#include <osg/MatrixTransform>
#include <osgUtil/UpdateVisitor>
#include <osgUtil/CullVisitor>

#include <QtCore/QStringList>
#include <QtCore/QVector>
#include <QtGui/QImage>
#include <QtCore/QCoreApplication>
#include <QtCore/QFile>
#include <QtCore/QTextStream>
#include <QtCore/QRegExp>

#include "goMark.h"
#include "On_MECA4_Draw.h"


On_MECA4_Draw::On_MECA4_Draw(void)
{
}


On_MECA4_Draw::~On_MECA4_Draw(void)
{
}

EARTHQUAKE_DATA_TYPE On_MECA4_Draw::GetTypeOfProduct()
{
	return STRUCT_MECA4;
}

void On_MECA4_Draw::LoadFile( const QString& filename )
{
	QStringList          m_slFileLineList;
	m_fileName = filename;

	QFile f(m_fileName);
	if (f.open(QFile::ReadOnly | QFile::Truncate)) 
	{
		QTextStream          in(&f);
		QRegExp rx("\\s+|\\t");  //至少一个空格和只有一个制表符

		QString head = in.readLine();
		if (head.contains("\n"))
		{
			head.replace("\n","");
		}
		QStringList headlist = head.split(rx,QString::SkipEmptyParts);

		if (headlist.count() == 0  )
		{
			return;
		}
		else if (headlist.at(0) != "#")
		{
			return ;
		}

		m_slFileLineList.clear();			//清除读取的文件

		while(!in.atEnd())
		{
			m_slFileLineList.append(in.readLine());
		}

		foreach(QString line ,m_slFileLineList)
		{
			QStringList linelist = line.split(" ");
			_MECHANISM temp1;
			MECHANISM ms;
			temp1.id= linelist.at(0);
			temp1.lon= linelist.at(1).toDouble();
			temp1.lat= linelist.at(2).toDouble();
			temp1.hight= linelist.at(3).toDouble();
			ms.NP1.str = linelist.at(4).toDouble();
			ms.NP1.dip = linelist.at(5).toDouble();
			ms.NP1.rake =linelist.at(6).toDouble();
			ms.NP2.str = linelist.at(7).toDouble();
			ms.NP2.dip = linelist.at(8).toDouble();
			ms.NP2.rake = linelist.at(9).toDouble();
			ms.magms = linelist.at(10).toDouble();
			temp1.magms= linelist.at(10).toDouble();
			//temp1.img= imgHelp.DrawMechanism(ms);

			pointArray.push_back(temp1);
			mes.push_back(ms);
		}
	}
	f.close();
}

bool On_MECA4_Draw::DumpAll( Layer* layer,P_E_COLORTABLE_VECTOR pColorTable /*= 0 */ )
{
	osg::ref_ptr<osg::Vec2Array> pPositionArray = new osg::Vec2Array;
	osg::Vec3 src, dst;
	if (pointArray.isEmpty())
	{
		return false;
	}
	osg::Geode* pGroup = layer->GetLayerNode()->asGeode();
	if(!pGroup)
	{
		layer->SetLayerNode(new osg::Geode);
	}

	for (int i = 0; i < pointArray.count();i++)
	{
		_MECHANISM tempPoint = pointArray.at(i);
		MECHANISM ms = mes.at(i);

		src.set(tempPoint.lon, tempPoint.lat, layer->LayerZ());
		//LonLatToGL(layer, src, dst);

		goto_gis::goMark* pMark = new goto_gis::goMark("H", src, layer);
		pMark->SetParentLayer(layer);
		QString fontFile = QCoreApplication::applicationDirPath()+tr("/Config/earthquake.ttf");
		pMark->SetFont(fontFile.toStdString());
		//rand() % 255 / 255.0, rand() % 255 / 255.0, rand() % 255 / 255.0, 1.0
		//pMark->SetTextColor(1.0, 0, 0, 1.0);
		pMark->SetTextDirection(M_PI / 2);
		pMark->UsedLayerSet(false);
		pMark->MarkPostion(src);

		float fS = 0;
		std::string strText;
		osg::Vec4 color;

		double dHeight = tempPoint.magms;
		if(dHeight >= 0 && dHeight <= 5)
		{
			color.set(0, 0, 1, 1);
		}
		else if(dHeight > 5 && dHeight <= 10)
		{
			color.set(1, 0, 0, 1);
		}
		else if(dHeight > 10 && dHeight <= 15)
		{
			color.set(0.5, 0.5, 0.5, 1);
		}
		else if(dHeight > 15 && dHeight <= 20)
		{
			color.set(1, 0, 0, 1);
		}
		pMark->SetTextColor(color.x(), color.y(), color.z(), color.w());
		pMark->MarkSize(10);
		layer->GetMgoGeometry()->insert(Layer::MgoGeometryPair(i,pMark));
	}
	return true;
}

void On_MECA4_Draw::GetColorFromTable(double iValue, osg::Vec4& color)
{

}

void On_MECA4_Draw::LonLatToGL( Layer* layer, osg::Vec3& in, osg::Vec3& out )
{
	osg::Vec3 destVec3;
	layer->CoordTrans()->Transform(in, destVec3);
	double x = destVec3.x();
	double y = destVec3.y();
	double z = layer->LayerZ();
	out.set(x,y,z);
}