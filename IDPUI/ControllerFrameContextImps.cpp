#include "ControllerFrameContextImps.h"


ControllerFrameContextImps::ControllerFrameContextImps(void) 
	: m_pViewer(0)
	, m_pMap(0)
{
}


ControllerFrameContextImps::~ControllerFrameContextImps(void)
{
}

osgViewer::Viewer* ControllerFrameContextImps::GetCurrentViewer()
{
	return m_pViewer;
}

goto_gis::Map* ControllerFrameContextImps::GetCurrentMap()
{
	return m_pMap;
}
