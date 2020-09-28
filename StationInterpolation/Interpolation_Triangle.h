#pragma once
#include "stationinterpolation_global.h"
#include "QT_Include.h"

#include "dem.h"
#include "tin.h"
#include "goPolygon.h"
#include "../GradsProvider/gxX.h"

//using namespace NCCGIS;
using namespace goto_gis;
class STATIONINTERPOLATION_EXPORT InterpolationTriangle
{
public:
	InterpolationTriangle(void);
	~InterpolationTriangle(void);
	bool Execute(XFILLPOINT *);
	bool Execute(goPolygon *);
	void ClearArray();
	int m_PolyNum;
	goPolygon *m_pPolygonTin;
	float m_z;
	QVector4D m_color;
private:
	bool GetContourFromFillPoint(XFILLPOINT*, CBlockContour*);
	bool GetContourFromGISPoly(goPolygon*, CBlockContour*);
	bool GetGISPolyFromTIN(CBlockTIN*);
	
	CTIN m_tin;
};