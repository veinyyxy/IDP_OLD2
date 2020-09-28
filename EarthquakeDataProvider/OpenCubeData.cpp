#include <osg/Billboard>
#include <osg/ShapeDrawable>
#include <osg/Shape>
#include <osg/PositionAttitudeTransform>
#include <osg/MatrixTransform>
#include <osgUtil/UpdateVisitor>
#include <osgUtil/CullVisitor>
#include <osg/AutoTransform>
#include <osgUtil/SmoothingVisitor>
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
#include "EarthquakeCubeData.h"
#include "../GISExtend/MarchingCubes.h"
#include "OpenCubeData.h"

OpenCubeData::OpenCubeData(void)
{
	fJingDu = 115;
	fWeiDu = 39;
	fStepJingDu = 0.06;
	fStepWeiDu = 0.06;
}


OpenCubeData::~OpenCubeData(void)
{
}

EARTHQUAKE_DATA_TYPE OpenCubeData::GetTypeOfProduct()
{
	return STRUCT_CUBE_DATA;
}

void OpenCubeData::LoadFile( const QString& filename )
{
	QStringList          m_slFileLineList;
	int iDataCount = 0, iVectCount = 0;
	QFile f(filename);
	if (f.open(QFile::ReadOnly | QFile::Truncate)) 
	{
		QTextStream          in(&f);
		QRegExp rx("\\s+|\\t");  //至少一个空格和只有一个制表符
		
		int iRowCount = 0;
		while(!in.atEnd())
		{
			QString strLineText;

			if(iRowCount == 0 || iRowCount == 2 || iRowCount == 3)
			{
				strLineText = in.readLine();
				iRowCount++;
				continue;
			}
			strLineText = in.readLine();
			if(iRowCount == 1)
			{
				QStringList strDataList = strLineText.split(' ');

				if(strDataList.size() < 3) return;

				iX = strDataList[2].toInt();
				iY = strDataList[1].toInt();
				iZ = strDataList[0].toInt();
				
				VecData = new QVector<float>[iZ];
				
				++iRowCount;
				continue;
			}
			
			float fvalue = strLineText.toFloat();
			
			if(iDataCount == iX*iY)
			{
				iVectCount++;
				iDataCount = 0;
			}
			if(iVectCount >= iZ) return;

			(VecData + iVectCount)->push_back(fvalue);
			iDataCount++;

			++iRowCount;
		}
	}
}

bool OpenCubeData::DumpAll( Layer* layer,P_E_COLORTABLE_VECTOR pColorTable )
{
	float zHeight = 0, fXDirect = fJingDu, fYDirect = fWeiDu;
	QVector<float>* pData = 0;
	VectTemp = new QVector<v3Point>[iZ];
	QVector<v3Point>* pTempData, *pTempDataUp;
	for(int ih = 0; ih < iZ; ih++)
	{
		fXDirect = fJingDu, fYDirect = fWeiDu;

		pData = VecData + ih;
		pTempData = VectTemp + ih;
		for(int ir = 0; ir < iX; ir++)
		{
			fYDirect = fWeiDu;
			for(int il = 0; il < iY; il++)
			{
				v3Point v3p;
				v3p.x = fXDirect;
				v3p.y = fYDirect;
				v3p.z = zHeight;
				v3p.value = pData->at(ir*iY + il) * 100;
				pTempData->push_back(v3p);
				fYDirect+=fStepJingDu;
			}
			fXDirect+=fStepJingDu;;
		}
		zHeight+=50000;
	}

	for(int ih = 1; ih < iZ; ih++)
	{
		pTempData = VectTemp + ih;
		pTempDataUp = VectTemp + ih - 1;

		for(int ix = 1; ix < iX; ix++)
		{
			for(int iy = 0; iy < iY -1; iy++)
			{
				v3Point temp1, temp2, temp3, temp4, temp5, temp6, temp7, temp8;
				temp1 = pTempData->at(ix*iY + iy);
				temp2 = pTempData->at(ix*iY + iy+1);
				temp3 = pTempData->at((ix-1)*iY + iy + 1);
				temp4 = pTempData->at((ix-1)*iY + iy);
				
				temp5 = pTempDataUp->at(ix*iY + iy);
				temp6 = pTempDataUp->at(ix*iY + iy+1);
				temp7 = pTempDataUp->at((ix-1)*iY + iy + 1);
				temp8 = pTempDataUp->at((ix-1)*iY + iy);
				
				CubeCoreData.push_back(temp1);
				CubeCoreData.push_back(temp2);
				CubeCoreData.push_back(temp3);
				CubeCoreData.push_back(temp4);
				CubeCoreData.push_back(temp5);
				CubeCoreData.push_back(temp6);
				CubeCoreData.push_back(temp7);
				CubeCoreData.push_back(temp8);
			}
		}
		
	}

	for(int i = 0; i < CubeCoreData.size(); i++)
	{
		osg::Vec3 v3Temp;
		v3Temp.set(CubeCoreData.at(i).x, CubeCoreData.at(i).y, CubeCoreData.at(i).z);
		layer->CoordTrans()->Transform(v3Temp, v3Temp);

		CubeCoreData.at(i).x = v3Temp.x();
		CubeCoreData.at(i).y = v3Temp.y();
		CubeCoreData.at(i).z = v3Temp.z();
	}

	osg::Geode* pGeode = layer->GetLayerNode()->asGeode();
	if(!pGeode)
	{
		pGeode = new osg::Geode;
		layer->SetLayerNode(pGeode);

		pGeode->addDrawable(GetIsosurface(800));
		pGeode->addDrawable(GetIsosurface(860));
		pGeode->addDrawable(GetIsosurface(700));
		pGeode->addDrawable(GetIsosurface(650));
		pGeode->addDrawable(GetIsosurface(500));
		pGeode->addDrawable(GetIsosurface(400));
		/*for(float fi = 5; fi < 8.1; fi += 0.1)
		{
			pGeode->addDrawable(GetIsosurface(fi));
		}*/
	}

	return false;
}

void OpenCubeData::LonLatToGL( Layer* layer, osg::Vec3& in, osg::Vec3& out )
{
	osg::Vec3 destVec3;
	layer->CoordTrans()->Transform(in, destVec3);
	double x = destVec3.x();
	double y = destVec3.y();
	double z = layer->LayerZ();
	out.set(x,y,z);
}

osg::Geometry* OpenCubeData::GetIsosurface(float fV )
{
	EarthquakeCubeData CubeData;
	/////////////////////////////////////创建图形/////////////////////////////////////
	osg::Geometry*pGeom = new osg::Geometry;
	osg::Vec3Array* pVertes = new osg::Vec3Array;
	osg::Vec4Array* pColors = new osg::Vec4Array;

	CubeData.SetCubeData(&CubeCoreData);
	CubeData.SetVertexColor(pVertes, pColors);
	MarchingCubes marchingCubes(&CubeData);

	
	marchingCubes.MarchingCubesMain(fV);

	pGeom->setVertexArray(pVertes);
	pGeom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::TRIANGLES, 0, pVertes->size()));
	pGeom->setColorArray(pColors);
	pGeom->setColorBinding(osg::Geometry::BIND_PER_VERTEX);

	osgUtil::SmoothingVisitor::smooth(*pGeom, osg::PI);

	return pGeom;
}

