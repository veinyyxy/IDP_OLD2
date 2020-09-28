#pragma once
#include "ControllerFrameContext.h"

class ControllerFrameContextImps :
	public ControllerFrameContext
{
public:
	ControllerFrameContextImps(void);
	~ControllerFrameContextImps(void);

	virtual osgViewer::Viewer* GetCurrentViewer();

	virtual goto_gis::Map* GetCurrentMap();

	inline void SetCurrentViewer(osgViewer::Viewer* pV){m_pViewer = pV;}
	inline void SetCurrentMap(goto_gis::Map* pMap){m_pMap = pMap;}
private:

	osgViewer::Viewer* m_pViewer;
	goto_gis::Map* m_pMap;
};

