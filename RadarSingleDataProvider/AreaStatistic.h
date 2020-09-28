#pragma once
#include "radarsingledataprovider_global.h"

#include <osg/Vec3>
#include <osg/Array>
#include <osg/Vec4>

#include "QT_Include.h"
#include "../GDataTypes/GDef.h"
#include "../GDataTypes/GData.h"
#include "../GDataTypes/GDataFiles.h"
#include "../GDataTypes/cDrawProduct.h"

#include "../GeosOnTheOsg/DataProvider.h"
#include "Draw3dPieChart.h"
#include "CDraw3dColumn.h"

class RADARSINGLEDATAPROVIDER_EXPORT  AreaStatistic :public QObject
{
public:
	AreaStatistic(void);
	~AreaStatistic(void);
	void AreaAdd(bool,bool,float,float,goto_gis::DataProvider*);
	void Swap(float &m_small, float &m_big);
	inline void SetPoint(osg::Vec3Array *p){pPoint = p;ispoint = true;}

private:
	osg::Vec3Array *pPoint;
	CDraw3dPieChart *m_pPieView;
	CDraw3dColumn *m_pColumn;
	bool ispoint;
};

