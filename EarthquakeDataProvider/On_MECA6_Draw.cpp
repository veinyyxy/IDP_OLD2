#include <osg/Billboard>
#include <osg/ShapeDrawable>
#include <osg/Shape>
#include <osg/PositionAttitudeTransform>
#include <osg/MatrixTransform>
#include <osgUtil/UpdateVisitor>
#include <osgUtil/CullVisitor>
#include <osg/AutoTransform>
#include <osg/Material>

#include <QtCore/QStringList>
#include <QtCore/QVector>
#include <QtGui/QImage>
#include <QtCore/QCoreApplication>
#include <QtCore/QFile>
#include <QtCore/QTextStream>
#include <QtCore/QRegExp>

#include "goMark.h"
#include "EarthquakeProductionDraw.h"
#include "goSymbolPoint.h"
#include "goSymbolContainer.h"

#include "On_MECA6_Draw.h"

On_MECA6_Draw::On_MECA6_Draw(void)
{
}


On_MECA6_Draw::~On_MECA6_Draw(void)
{
}

EARTHQUAKE_DATA_TYPE On_MECA6_Draw::GetTypeOfProduct()
{
	return EARTHQUAKE_DATA_TYPE::STRUCT_MECA6;
}

void On_MECA6_Draw::LoadFile( const QString& filename )
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
	QString file1 = QCoreApplication::applicationDirPath()+tr("/Config/arrow1.3DS");
	QString file2 = QCoreApplication::applicationDirPath()+tr("/Config/arrow2.3DS");
	m_pArrowNode1 = osgDB::readNodeFile(file1.toStdString());
	m_pArrowNode2 = osgDB::readNodeFile(file2.toStdString());

	/*osg::StateSet* pSS = new osg::StateSet;
	osg::ref_ptr<osg::Material> pMatrial = new osg::Material;
	pMatrial->setDiffuse(osg::Material::FRONT_AND_BACK,osg::Vec4(1.0,0.0,0.0,1.0));
	pMatrial->setAmbient(osg::Material::FRONT_AND_BACK, osg::Vec4(1.0,0.0,0.0,1.0));
	pMatrial->setEmission(osg::Material::FRONT_AND_BACK, osg::Vec4(1.0,0.0,0.0,1.0));

	pSS->setAttribute(pMatrial);
	pSS->setMode(GL_LIGHTING, osg::StateAttribute::OFF );

	m_pArrowNode1->setStateSet(pSS);
	m_pArrowNode2->setStateSet(pSS);*/
}

bool On_MECA6_Draw::DumpAll( Layer* layer,P_E_COLORTABLE_VECTOR pColorTable )
{
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
	goSymbolContainer* pSymbolContainer1 = new goSymbolContainer;
	goSymbolContainer* pSymbolContainer2 = new goSymbolContainer;
	osg::ref_ptr<osg::Vec3Array> vec3Array = new osg::Vec3Array;
	std::vector<float> scaleArray1;
	std::vector<float> scaleArray2;

	std::vector<float> angleArray1;
	std::vector<float> angleArray2;

	for (int i = 0; i < pointArray.count();i++)
	{
		/*osg::MatrixTransform* pMatrixTransform1 = new osg::MatrixTransform;
		osg::MatrixTransform* pMatrixTransform2 = new osg::MatrixTransform;
		osg::AutoTransform* pAutoTransform1 = new osg::AutoTransform;
		osg::AutoTransform* pAutoTransform2 = new osg::AutoTransform;
		pAutoTransform1->setAutoScaleToScreen(true);
		pAutoTransform2->setAutoScaleToScreen(true);*/
		//osg::Geode* pGeode = new osg::Geode;		

		_MECHANISM tempPoint = pointArray.at(i);
		MECHANISM ms = mes.at(i);

		float fLen = 20, fAngle1, fAngle2;
		int iAngle1 = 0, iAngle2 = 0;

		iAngle2 = rand() % 360;
		iAngle1 = rand() % 360;

		fAngle1 = iAngle1 * M_PI/180;
		fAngle2 = iAngle2 * M_PI/180;

		/*pMatrixTransform1->setMatrix(m4Rot1 * m4scale);
		pMatrixTransform2->setMatrix(m4Rot2 * m4scale);

		pMatrixTransform1->addChild(m_pArrowNode1);
		pMatrixTransform2->addChild(m_pArrowNode2);

		pAutoTransform1->setPosition(dst);
		pAutoTransform2->setPosition(dst);

		pAutoTransform1->addChild(pMatrixTransform1);
		pAutoTransform2->addChild(pMatrixTransform2);

		pGroup->addChild(pAutoTransform1);
		pGroup->addChild(pAutoTransform2);*/

		vec3Array->push_back(osg::Vec3(pointArray.at(i).lon, pointArray.at(i).lat, 0));
		angleArray1.push_back(fAngle1);
		angleArray2.push_back(fAngle2);

		scaleArray1.push_back(1.5);
		scaleArray2.push_back(1.5);

		pSymbolContainer1->AddSymbol(m_pArrowNode1);
		pSymbolContainer2->AddSymbol(m_pArrowNode2);
	}

	pSymbolContainer1->SetPositionArray(vec3Array);
	pSymbolContainer1->SetAngleArray(&angleArray1);
	pSymbolContainer1->SetScaleArray(&scaleArray1);

	pSymbolContainer2->SetPositionArray(vec3Array);
	pSymbolContainer2->SetAngleArray(&angleArray2);
	pSymbolContainer2->SetScaleArray(&scaleArray2);

	layer->GetMgoGeometry()->insert(Layer::MgoGeometryPair(0, pSymbolContainer1));
	layer->GetMgoGeometry()->insert(Layer::MgoGeometryPair(1, pSymbolContainer2));

	return true;
}

void On_MECA6_Draw::LonLatToGL( Layer* layer, osg::Vec3& in, osg::Vec3& out )
{
	osg::Vec3 destVec3;
	layer->CoordTrans()->Transform(in, destVec3);
	double x = destVec3.x();
	double y = destVec3.y();
	double z = layer->LayerZ();
	out.set(x,y,z);
}

