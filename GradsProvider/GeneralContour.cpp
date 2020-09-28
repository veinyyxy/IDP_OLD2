#include "GeneralContour.h"
#include <QMessageBox>
#include <QDialog>
#include <QLabel>
#include <QGridLayout>
#include <QApplication>
#include <QPushButton>
#include <QStyle>
#include <qmath.h>

#include <geos/geom/CoordinateArraySequence.h>
#include <geos/geom/CoordinateArraySequenceFactory.h>
#include <geos/geom/LineString.h>
#include "goLineString.h"
#include "goMark.h"

using namespace std;

extern QVector<lineData*> ContourData;						// 等值线线段数组
extern QVector<LineTextValue*> ContourTextValueArray;		// 数字位置数组
extern QVector<double>  vector_ContorValue;   // 等值线属性值
//extern QVector<WindBarbData*> WindBarbDataArray;			// 风标位置数组
//extern QVector<float>  vetor1_contourLine;					// 旧的等值线线段数组
//extern QVector<XFILLPOINT*> ShadeData;			// 填图多边形数组

const qreal MAXVALUE = qPow(10,22);
const qreal MINVALUE = -qPow(10,22);

GeneralContour::GeneralContour(void)
{
	
}

GeneralContour::~GeneralContour(void)
{
	
}

#include <QDebug>

int GeneralContour::Dump(goto_gis::Layer* pLayer, int iDirect)
{
	if(!pLayer) return 0;

	goto_gis::goGeometry* pGoGeomtry = 0;
	//geos::geom::Geometry* pGeosGeome= 0;
	const geos::geom::GeometryFactory * pGeometryFactory = geos::geom::GeometryFactory::getDefaultInstance();
	const geos::geom::CoordinateSequenceFactory* pCoordinateSequenceFactory = geos::geom::CoordinateArraySequenceFactory::instance();
	//线条
	for(int i = 0; i <  ContourData.count(); i++)
	{
		lineData * pLineInfo = ContourData.at(i);

		int iPointCount = pLineInfo->arrayData.count();

		int iPPC = pLineInfo->n;
		geos::geom::CoordinateSequence* pCoordArray = pCoordinateSequenceFactory->create((size_t)0, 3);
		
		pGoGeomtry = new goto_gis::goLineString;
		pGoGeomtry->UserLayerColor(false);

		for(int j = 0;j < iPointCount;)
		{
			pCoordArray->add(geos::geom::Coordinate(pLineInfo->arrayData[j],pLineInfo->arrayData[j+1]));
			pGoGeomtry->GetColorArray()->push_back(osg::Vec4(pLineInfo->ColorData[0],pLineInfo->ColorData[1],pLineInfo->ColorData[2],1));
			j+=3;
		}
		geos::geom::LineString* pLineString = pGeometryFactory->createLineString(pCoordArray);
		const geos::geom::CoordinateSequence* pCS = pLineString->getCoordinates();
		
		pGoGeomtry->SetGeosGeomtry(pLineString);
		pGoGeomtry->SetCoordinateSequence((geos::geom::CoordinateSequence*)pCS);
		pLayer->GetMgoGeometry()->insert(goto_gis::Layer::MgoGeometryPair(i, pGoGeomtry));
	}
	//文字
	size_t uTextCount = ContourTextValueArray.size();
	for(size_t i = 0; i < uTextCount; i++)
	{
		LineTextValue* pText = ContourTextValueArray.at(i);
		goto_gis::goMark* pTextMark = new goto_gis::goMark(pText->nTextValue, osg::Vec3(pText->x, pText->y, pLayer->LayerZ()), pLayer);
		pTextMark->SetTextColor(1, 1, 1, 1.0);
		pTextMark->SetTextDirection(M_PI / 2);
		pTextMark->MarkSize(16);
		pTextMark->SetAlignment( osgText::Text::CENTER_TOP );

		pTextMark->SetText(pText->nTextValue);
		pTextMark->MarkPostion(osg::Vec3(pText->x, pText->y, pLayer->LayerZ()));

		pLayer->GetMgoGeometry()->insert(goto_gis::Layer::MgoGeometryPair(ContourData.count() + i, pTextMark));
	}
	/******************clear line data***********/
	QVector<lineData*>::iterator iteratorLine;
	for (iteratorLine = ContourData.begin(); iteratorLine != ContourData.end();++iteratorLine)
	{
		(**iteratorLine).arrayData.clear();
		delete *iteratorLine;
	}
	ContourData.clear();

	return 1;
}

void GeneralContour::InitCommand()
{
	gcmn.sig = 0;
	gacmd("set csmooth on",&gcmn,0);
	gcmn.sig = 0;
	gacmd("set cterp off",&gcmn,0);
	gcmn.sig = 0;
	gacmd("set grid off",&gcmn,0);
	gcmn.sig = 0;
	gacmd("set mpdraw off",&gcmn,0);
	gcmn.sig = 0;
	gacmd("set frame off",&gcmn,0);
	gcmn.sig = 0;
	gacmd("set clab on",&gcmn,0);
	gcmn.sig = 0;
	gacmd("set grads off",&gcmn,0);
	gcmn.sig = 0;
	gacmd("set xlab off",&gcmn,0);
	gcmn.sig = 0;
	gacmd("set ylab off",&gcmn,0);
	gcmn.sig = 0;
	gacmd("set arrlab off",&gcmn,0);
	gcmn.sig = 0;
	gacmd("set lon 0 360",&gcmn,0);   // added by Liu Xinsheng 2011-05-02 motify 2012 3 1
	gcmn.sig = 0;
	gacmd("set lat -89 89",&gcmn,0);     // added by WangJL 2011-05-02
	gcmn.sig = 0;
	gacmd("set cstyle 1",&gcmn,0);
}

int GeneralContour::Draw(const std::string& strCmd)
{
	InitCommand();
	gcmn.sig = 0;
	gacmd("set gxout contour",&gcmn,0);		// 设置为等值线

	gcmn.sig = 0;
	if(gacmd((char*)(strCmd.c_str()),&gcmn,0))
	{
		if(gcmn.str_errorinfo != NULL)
			return 0;
		else
			return 1;
		/*CCIPASMessageBox::CIPASCritical(NULL, CCIPASMessageBox::CIPAS_OK,tr("错误"),gcmn.str_errorinfo);
		else
		CCIPASMessageBox::CIPASCritical(NULL, CCIPASMessageBox::CIPAS_OK,tr("错误"),tr("发生未知绘图错误！"));*/
	}
	else
	{
		/******************************变换X11坐标到经纬坐标*****************************/
		ScaleDataLine();
		ScaleDataText();
		return 0;
	}

		

	
}

void GeneralContour::ScaleDataLine()
{
	unsigned int nCount = ContourData.size();
	unsigned int i;
	double x,y,lon,lat;

	for(i = 0 ;i< nCount; i++)
	{
		lineData *lineData1 = ContourData.at(i);
		for(int j = 0 ; j< lineData1->arrayData.count();j = j+3)
		{
			x = lineData1->arrayData[j];
			y = lineData1->arrayData[j+1];
			gxxy2w (x, y, &lon, &lat);
			lineData1->arrayData[j] = lon;
			lineData1->arrayData[j+1] = lat;
		}
	}
}
void GeneralContour::ScaleDataText()
{
	unsigned int nCount = ContourTextValueArray.count();
	unsigned int i;
	double lon,lat;
	LineTextValue *ptextpoint = NULL;

	for(i = 0 ;i< nCount; i ++)
	{
		ptextpoint = ContourTextValueArray.at(i);

		gxxy2w (ptextpoint->x, ptextpoint->y, &lon, &lat);
		ptextpoint->x = lon;
		ptextpoint->y = lat;
	}
}