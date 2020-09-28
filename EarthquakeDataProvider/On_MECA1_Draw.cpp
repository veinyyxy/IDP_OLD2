#include "On_MECA1_Draw.h"
#include <osg/Billboard>
#include <osg/ShapeDrawable>
#include <osg/Shape>
#include <osg/PositionAttitudeTransform>
#include <osg/MatrixTransform>
#include <osgUtil/UpdateVisitor>
#include <osgUtil/CullVisitor>
#include <osg/AutoTransform>
#include <QtCore/QCoreApplication>

#include "EarthquakeImageLegend.h"
#include "../XMLProvider/RadReadColor_XML.h"
#include "goSymbolGeode.h"
#include "Layer.h"
#include "gogeometry.h"
#include "CoordinateTransform.h"
#include "goSymbolPoint.h"
#include <QStringList>
#include <QTextStream>
#include <QtCore/QFile>
#include <QGLWidget>
#include "helper.h"

class TransformNodeCallback : public osg::NodeCallback
{
public:
	virtual void operator()(osg::Node* node, osg::NodeVisitor* nv)
	{
		static osg::Vec3 localEye(0, 0, 0);
		osgUtil::CullVisitor* pCullVisitor = dynamic_cast<osgUtil::CullVisitor*>(nv);
		osgUtil::UpdateVisitor* pUpdapteVistor = dynamic_cast<osgUtil::UpdateVisitor*>(nv);

		if(pCullVisitor)
		{
			localEye = pCullVisitor->getEyeLocal();
			traverse(node, nv);
			return;
		}

		if(pUpdapteVistor && localEye.z() != 0)
		{
			float fDis = localEye.z() - m_ObjectPos.z();
			osg::Matrix matix;
			matix.setTrans(osg::Vec3(0, 0, fDis));
			osg::MatrixTransform* pMatrixTrans = dynamic_cast<osg::MatrixTransform*>(node);
			pMatrixTrans->setMatrix(matix);
			traverse(node,nv);
		}
	}
	osg::Vec3 m_ObjectPos;

};

On_MECA1_Draw::On_MECA1_Draw(void)
{
}


On_MECA1_Draw::~On_MECA1_Draw(void)
{
}

EARTHQUAKE_DATA_TYPE On_MECA1_Draw::GetTypeOfProduct()
{
	return EARTHQUAKE_DATA_TYPE::STRUCT_MECA1;
}

void On_MECA1_Draw::LoadFile( const QString& filename )
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
			temp1.img= imgHelp.DrawMechanism(ms);

			pointArray.push_back(temp1);
			mes.push_back(ms);
		}
	}
	f.close();

}

bool On_MECA1_Draw::DumpAll( Layer* layer,P_E_COLORTABLE_VECTOR pColorTable )
{
	//goto_gis::goSymbolPoint* pSymbolPoint = new goto_gis::goSymbolPoint;
	osg::ref_ptr<osg::Vec2Array> pPositionArray = new osg::Vec2Array;
	if (pointArray.isEmpty())
	{
		return false;
	}
	osg::Geode* pGeode = layer->GetLayerNode()->asGeode();
	if(!pGeode)
		layer->SetLayerNode(new osg::Geode);
	/*osg::Group* pGroup = layer->GetLayerNode()->asGroup();
	if(!pGroup) return false;*/
	
	/*QString strPath = QCoreApplication::applicationDirPath()+tr("/Config/earth_black.jpg");
	osg::Image* pImg = osgDB::readImageFile(strPath.toStdString());
	osg::StateSet* pSet = pGroup->getOrCreateStateSet();
	osg::Texture2D* tex2d = new osg::Texture2D;
	tex2d->setImage(pImg);
	pSet->setAttributeAndModes(tex2d);
	osg::AutoTransform* pMatrixTransform = new osg::AutoTransform;
	pMatrixTransform->setAutoScaleToScreen(true);*/

	for (int i = 0; i < pointArray.count();i++)
	{
		_MECHANISM tempPoint = pointArray.at(i);
#if 0
		osg::Geometry* geom = new osg::Geometry;

		pPositionArray->push_back(osg::Vec2(tempPoint.lon,tempPoint.lat));

		osg::Vec3 corner,width,height;

		corner = osg::Vec3(0,0,0);
		double footstep = 100000;
		width = osg::Vec3(footstep,0,0);
		height = osg::Vec3(0,footstep,0);

		osg::Vec4Array* colorArray = new osg::Vec4Array(4);
		colorArray->push_back(osg::Vec4(1.0, 1.0, 1.0, 1.0));
		colorArray->push_back(osg::Vec4(1.0, 1.0, 1.0, 1.0));
		colorArray->push_back(osg::Vec4(1.0, 1.0, 1.0, 1.0));
		colorArray->push_back(osg::Vec4(1.0, 1.0, 1.0, 1.0));
		//geom->setColorArray(colorArray);
		//geom->setColorBinding(osg::Geometry::BIND_PER_VERTEX);

		osg::Vec3Array* coords = new osg::Vec3Array(4);
		(*coords)[0] = corner;
		(*coords)[1] = corner+width;
		(*coords)[2] = corner+width+height;
		(*coords)[3] = corner+height;


		geom->setVertexArray(coords);

		/*osg::Vec3Array* norms = new osg::Vec3Array(1);
		(*norms)[0] = width^height;
		(*norms)[0].normalize();

		geom->setNormalArray(norms);
		geom->setNormalBinding(osg::Geometry::BIND_OVERALL);*/

		osg::Vec2Array* tcoords = new osg::Vec2Array(4);
		(*tcoords)[0].set(0.0f,0.0f);
		(*tcoords)[1].set(1.0f,0.0f);
		(*tcoords)[2].set(1.0f,1.0f);
		(*tcoords)[3].set(0.0f,1.0f);
		geom->setTexCoordArray(0,tcoords);

		geom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::QUADS,0,4));

		tempPoint.img.save("maincall.png");
		//QImage img; 
		//img = QGLWidget::convertToGLFormat(tempPoint.img); 

		//unsigned char* data = img.bits(); 
		osg::Image* s = osgDB::readImageFile("maincall.png");/*new osg::Image()*/; 
		//s->setImage(img.width(), img.height(), 1, 4, GL_RGBA, GL_UNSIGNED_BYTE, data, osg::Image::NO_DELETE, 1); 

		if (s)
		{
			osg::StateSet* stateset = new osg::StateSet;
			osg::Texture2D* texture = new osg::Texture2D;
			texture->setImage(s);
			stateset->setTextureAttributeAndModes(0,texture,osg::StateAttribute::ON);
			geom->setStateSet(stateset);
		}
#else
		

		////osg::PositionAttitudeTransform* posAttriTransform = new osg::PositionAttitudeTransform;
		////osg::Projection* HUDNode = new osg::Projection;
		////osg::Billboard *pBillboard = new goto_gis::goSymbolGeode;
		//osg::Geode* pGeode = new osg::Geode;

		//osg::Sphere* pSphere = new osg::Sphere;
		//osg::Vec3 Vec3p(tempPoint.lon, tempPoint.lat, 0), vec3Center;

		//LonLatToGL(layer, Vec3p, vec3Center);
		//pSphere->setCenter(vec3Center);
		////pSphere->setCenter(osg::Vec3(0, 0, 0));
		//pSphere->setRadius(tempPoint.magms * 10);
		//osg::ShapeDrawable* pShape = new osg::ShapeDrawable(pSphere);
		//
		////pBillboard->addDrawable(pShape, vec3Center);
		////pGroup->addChild(pBillboard);
		//
		//pGeode->addDrawable(pShape);
		//pMatrixTransform->addChild(pGeode);
		//TransformNodeCallback* pNodeCallback = new TransformNodeCallback;
		//pNodeCallback->m_ObjectPos = osg::Vec3(0, 0, layer->LayerZ());
		////pMatrixTransform->setCullCallback(pNodeCallback);
		////pMatrixTransform->setUpdateCallback(pNodeCallback);
		
#endif
		//posAttriTransform->addChild(pGeode);
		//posAttriTransform->setScale(osg::Vec3(2.0, 2.0, 2.0));
		//pGroup->addChild(posAttriTransform);
		goto_gis::goSymbolPoint* pSymbolPoint = new goto_gis::goSymbolPoint;
		pSymbolPoint->SetPosition(osg::Vec3(tempPoint.lon, tempPoint.lat, 0));
		pSymbolPoint->SetSize(tempPoint.magms * 5);
		tempPoint.img.save("maincall.png");
		osg::Image* s = osgDB::readImageFile("maincall.png");
		pSymbolPoint->SetTexture(s);
		//pSymbolPoint->SetAngle(M_PI/180 * 90);
		Layer::MgoGeometry* pMG1 = layer->GetMgoGeometry();
		pMG1->insert(Layer::MgoGeometryPair(i,pSymbolPoint));
	}
	//读取图例图片，并初始化图例
	/*EarthquakeImageLegend* pImageLegend = new EarthquakeImageLegend;
	SymbolLibraryInterface* pSLI = layer->GetSymbolLibrary();
	if(pSLI)
	{
	pSLI->SetLegend(pImageLegend);
	}*/
	/*SymbolLegend_Vector* pSymbolLegend = new SymbolLegend_Vector;
	SymbolLegend* ss1 = new SymbolLegend;
	ss1->labelText="0-111";
	ss1->pixmap = QPixmap("D:/workspace/IDP/IDPUI/images/logo.png");
	m_symbolLegend->append(ss1);*/
	return true;
}

void On_MECA1_Draw::LonLatToGL( Layer* layer, osg::Vec3& in, osg::Vec3& out )
{
	osg::Vec3 destVec3;
	layer->CoordTrans()->Transform(in, destVec3);
	double x = destVec3.x();
	double y = destVec3.y();
	double z = layer->LayerZ();
	out.set(x,y,z);
}

