#pragma once
#include "idp_control_global.h"

using namespace goto_gis;

class IDP_CONTROL_EXPORT LayerTreeControler
{
public:
	LayerTreeControler(Map* pM, osgViewer::Viewer* pV);
	LayerTreeControler(void);
	virtual ~LayerTreeControler(void);

	inline void SetMapAndViewer(Map* pM, osgViewer::Viewer* pV){m_pMap = pM; m_pViewer = pV;}
	void AlignmentRadar(const QString& layerName);

private:
	Map* m_pMap;
	osgViewer::Viewer* m_pViewer;
};

