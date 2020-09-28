#pragma once
#include "idp_control_global.h"
#include "drawgeometrydata.h"
#include "GeosOnTheOsg.h"
#include "Layer.h"

class IDP_CONTROL_EXPORT IDPDrawGeometryData :
	public DrawGeometryData
{
public:
	IDPDrawGeometryData(void);
	virtual ~IDPDrawGeometryData(void);
	inline void SetDrawLayer(goto_gis::Layer* pLayer){m_pLayer = pLayer;}
	void SetResultHandle(RectResult* pHandler){m_RectResult = pHandler;}
protected:
	RectResult* m_RectResult;
	goto_gis::Layer* m_pLayer;
};

