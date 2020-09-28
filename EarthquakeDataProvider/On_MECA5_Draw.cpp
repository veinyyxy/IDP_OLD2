#include <osg/Billboard>
#include <osg/ShapeDrawable>
#include <osg/Shape>
#include <osg/PositionAttitudeTransform>
#include <osg/MatrixTransform>
#include <osgUtil/UpdateVisitor>
#include <osgUtil/CullVisitor>
#include <osg/AutoTransform>

#include <QtCore/QStringList>
#include <QtCore/QVector>
#include <QtGui/QImage>
#include <QtCore/QCoreApplication>
#include <QtCore/QFile>
#include <QtCore/QTextStream>
#include <QtCore/QRegExp>

#include "goMark.h"
#include "goSymbolContainer.h"
#include "On_MECA5_Draw.h"


On_MECA5_Draw::On_MECA5_Draw(void)
{
}


On_MECA5_Draw::~On_MECA5_Draw(void)
{
}

EARTHQUAKE_DATA_TYPE On_MECA5_Draw::GetTypeOfProduct()
{
	return STRUCT_MECA5;
}

void On_MECA5_Draw::LoadFile( const QString& filename )
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

bool On_MECA5_Draw::DumpAll( Layer* layer,P_E_COLORTABLE_VECTOR pColorTable /*= 0 */ )
{
#if 0
	osg::ref_ptr<osg::Vec2Array> pPositionArray = new osg::Vec2Array;
	osg::Vec3 src, dst;
	if (pointArray.isEmpty())
	{
		return false;
	}
	osg::Geode* pGeode = layer->GetLayerNode()->asGeode();
	if(!pGeode)
	{
		pGeode = new osg::Geode;
		layer->SetLayerNode(pGeode);
	}
	osg::Geometry* pGeom = new osg::Geometry;
	osg::Vec3Array* vertes = new osg::Vec3Array;
	osg::Vec4Array* colors = new osg::Vec4Array;

	for (int i = 0; i < pointArray.count();i++)
	{
		osg::Vec4 color;

		_MECHANISM tempPoint = pointArray.at(i);
		MECHANISM ms = mes.at(i);

		float fLen = 125000 / 3.0, fAngle;
		int iAngle = 0;
		src.set(tempPoint.lon, tempPoint.lat, layer->LayerZ());
		LonLatToGL(layer, src, dst);
		osg::Matrix m4Mov1, m4Rot, m4Mov2;
		osg::Vec3 v3Zero(0,0,0), v3Direction(0, fLen, layer->LayerZ());
		osg::Vec3 v3p1, v3p2;
		m4Mov2.makeTranslate(dst);
		m4Mov1.makeTranslate(v3Zero - dst);

		iAngle = rand() % 360;

		fAngle = iAngle * M_PI/180;

		m4Rot.makeRotate(fAngle, osg::Vec3(0, 0, 1));

		v3Direction = v3Direction * m4Rot;

		v3p1 = v3Zero * m4Mov2;
		v3p2 = v3Direction * m4Mov2;

		vertes->push_back(v3p1);
		vertes->push_back(v3p2);

		double dHeight = tempPoint.hight;
		if(dHeight >= 0 && dHeight <= 140)
		{
			color.set(1, 0, 0, 1);
		}
		else if(dHeight > 140 && dHeight <= 280)
		{
			color.set(233.0 / 255.0, 114 / 255.0, 224.0 / 255.0, 1);
		}
		else if(dHeight > 280 && dHeight <= 420)
		{
			color.set(122.0 / 255.0, 217 / 255.0, 224.0 / 255.0, 1);
		}
		else if(dHeight > 420 && dHeight <= 560)
		{
			color.set(233.0 / 255.0, 114 / 255.0, 224.0 / 255.0, 1);
		}
		else if(dHeight > 560 && dHeight <= 700)
		{
			color.set(0, 1, 0, 1);
		}

		colors->push_back(color);
		colors->push_back(color);
	}

	pGeom->setVertexArray(vertes);
	pGeom->addPrimitiveSet(new osg::DrawArrays(osg::DrawArrays::LINES, 0, vertes->size()));

	pGeom->setColorArray(colors);
	pGeom->setColorBinding(osg::Geometry::BIND_PER_PRIMITIVE);

	pGeode->addDrawable(pGeom);
#endif
#if 0
	osg::ref_ptr<osg::Vec2Array> pPositionArray = new osg::Vec2Array;
	osg::Vec3 src, dst;
	if (pointArray.isEmpty())
	{
		return false;
	}
	osg::Group* pGroup = layer->GetLayerNode()->asGroup();
	if(!pGroup)
	{
		pGroup = new osg::Group;
		layer->SetLayerNode(pGroup);
	}
	osg::StateSet* pStateSet = pGroup->getOrCreateStateSet();
	pStateSet->setMode(GL_LIGHTING, osg::StateAttribute::OFF );

	for (int i = 0; i < pointArray.count();i++)
	{
		osg::AutoTransform* pAutoTransform = new osg::AutoTransform;
		pAutoTransform->setAutoScaleToScreen(true);
		osg::Geode* pGeode = new osg::Geode;
		osg::Geometry* pGeom = new osg::Geometry;
		osg::Vec3Array* vertes = new osg::Vec3Array;
		osg::Vec4Array* colors = new osg::Vec4Array;

		osg::Vec4 color;

		_MECHANISM tempPoint = pointArray.at(i);
		MECHANISM ms = mes.at(i);

		float fLen = 20, fAngle;
		int iAngle = 0;
		src.set(tempPoint.lon, tempPoint.lat, layer->LayerZ());
		LonLatToGL(layer, src, dst);

		osg::Matrix m4Mov1, m4Rot, m4Mov2;
		osg::Vec3 v3Zero(0,0,0), v3Direction(0, fLen, layer->LayerZ());
		osg::Vec3 v3p1, v3p2;
		m4Mov2.makeTranslate(dst);
		m4Mov1.makeTranslate(v3Zero - dst);

		iAngle = rand() % 360;

		fAngle = iAngle * M_PI/180;

		m4Rot.makeRotate(fAngle, osg::Vec3(0, 0, 1));

		v3Direction = v3Direction * m4Rot;

		v3p1 = v3Zero/* * m4Mov2*/;
		v3p2 = v3Direction/* * m4Mov2*/;

		vertes->push_back(v3p1);
		vertes->push_back(v3p2);

		double dHeight = tempPoint.hight;
		if(dHeight >= 0 && dHeight <= 140)
		{
			color.set(1, 0, 0, 1);
		}
		else if(dHeight > 140 && dHeight <= 280)
		{
			color.set(233.0 / 255.0, 114 / 255.0, 224.0 / 255.0, 1);
		}
		else if(dHeight > 280 && dHeight <= 420)
		{
			color.set(122.0 / 255.0, 217 / 255.0, 224.0 / 255.0, 1);
		}
		else if(dHeight > 420 && dHeight <= 560)
		{
			color.set(233.0 / 255.0, 114 / 255.0, 224.0 / 255.0, 1);
		}
		else if(dHeight > 560 && dHeight <= 700)
		{
			color.set(0, 1, 0, 1);
		}

		colors->push_back(color);
		//colors->push_back(osg::Vec4(1, 1, 1, 1));

		pGeom->setVertexArray(vertes);
		pGeom->addPrimitiveSet(new osg::DrawArrays(osg::DrawArrays::LINES, 0, vertes->size()));

		pGeom->setColorArray(colors);
		pGeom->setColorBinding(osg::Geometry::BIND_OVERALL);

		pGeode->addDrawable(pGeom);
		//osg::Vec3 pos = pGeode->getBound().center();
		pAutoTransform->setPosition(dst);
		//pAutoTransform->setAutoRotateMode(osg::AutoTransform::ROTATE_TO_SCREEN);
		pAutoTransform->addChild(pGeode);
		pGroup->addChild(pAutoTransform);
	}
#endif
#if 1
	goto_gis::goSymbolContainer* pSymbolContainer = new goto_gis::goSymbolContainer;
	osg::ref_ptr<osg::Vec2Array> pPositionArray = new osg::Vec2Array;
	osg::Vec3 src, dst;
	if (pointArray.isEmpty())
	{
		return false;
	}
	osg::Group* pGroup = layer->GetLayerNode()->asGroup();
	if(!pGroup)
	{
		pGroup = new osg::Group;
		layer->SetLayerNode(pGroup);
	}
	osg::StateSet* pStateSet = pGroup->getOrCreateStateSet();
	pStateSet->setMode(GL_LIGHTING, osg::StateAttribute::OFF );
	osg::ref_ptr<osg::Vec3Array> pLatLongPosArray = new osg::Vec3Array;
	for (int i = 0; i < pointArray.count();i++)
	{
		osg::Geometry* pGeom = new osg::Geometry;
		osg::Vec3Array* vertes = new osg::Vec3Array;
		osg::Vec4Array* colors = new osg::Vec4Array;

		osg::Vec4 color;

		_MECHANISM tempPoint = pointArray.at(i);
		MECHANISM ms = mes.at(i);

		float fLen = 20, fAngle;
		int iAngle = 0;
		src.set(tempPoint.lon, tempPoint.lat, layer->LayerZ());

		osg::Matrix m4Mov1, m4Rot, m4Mov2;
		osg::Vec3 v3Zero(0,0,0), v3Direction(0, fLen, layer->LayerZ());
		osg::Vec3 v3p1, v3p2;
		m4Mov2.makeTranslate(dst);
		m4Mov1.makeTranslate(v3Zero - dst);

		iAngle = rand() % 360;

		fAngle = iAngle * M_PI/180;

		m4Rot.makeRotate(fAngle, osg::Vec3(0, 0, 1));

		v3Direction = v3Direction * m4Rot;

		v3p1 = v3Zero/* * m4Mov2*/;
		v3p2 = v3Direction/* * m4Mov2*/;

		vertes->push_back(v3p1);
		vertes->push_back(v3p2);

		double dHeight = tempPoint.hight;
		if(dHeight >= 0 && dHeight <= 140)
		{
			color.set(1, 0, 0, 1);
		}
		else if(dHeight > 140 && dHeight <= 280)
		{
			color.set(233.0 / 255.0, 114 / 255.0, 224.0 / 255.0, 1);
		}
		else if(dHeight > 280 && dHeight <= 420)
		{
			color.set(122.0 / 255.0, 217 / 255.0, 224.0 / 255.0, 1);
		}
		else if(dHeight > 420 && dHeight <= 560)
		{
			color.set(233.0 / 255.0, 114 / 255.0, 224.0 / 255.0, 1);
		}
		else if(dHeight > 560 && dHeight <= 700)
		{
			color.set(0, 1, 0, 1);
		}

		colors->push_back(color);
		//colors->push_back(osg::Vec4(1, 1, 1, 1));

		pGeom->setVertexArray(vertes);
		pGeom->addPrimitiveSet(new osg::DrawArrays(osg::DrawArrays::LINES, 0, vertes->size()));

		pGeom->setColorArray(colors);
		pGeom->setColorBinding(osg::Geometry::BIND_OVERALL);

		pSymbolContainer->AddSymbol(pGeom);

		pLatLongPosArray->push_back(src);
	}

	pSymbolContainer->SetPositionArray(pLatLongPosArray);
	layer->GetMgoGeometry()->insert(Layer::MgoGeometryPair(0, pSymbolContainer));
#endif
	return true;
}

void On_MECA5_Draw::GetColorFromTable(double iValue, osg::Vec4& color)
{

}

void On_MECA5_Draw::LonLatToGL( Layer* layer, osg::Vec3& in, osg::Vec3& out )
{
	osg::Vec3 destVec3;
	layer->CoordTrans()->Transform(in, destVec3);
	double x = destVec3.x();
	double y = destVec3.y();
	double z = layer->LayerZ();
	out.set(x,y,z);
}