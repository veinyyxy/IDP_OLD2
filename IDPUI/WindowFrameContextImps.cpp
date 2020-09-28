#include "WindowFrameContextImps.h"


WindowFrameContextImps::WindowFrameContextImps(void) 
	: m_pMdiWindow(0)
	, m_pViewerQT(0)
{
}


WindowFrameContextImps::~WindowFrameContextImps(void)
{
}

ViewerQT* WindowFrameContextImps::GetCurrentQGLWidget()
{
	return m_pViewerQT;
}

QMdiSubWindow* WindowFrameContextImps::GetCurrentSubWindow()
{
	return m_pMdiWindow;
}
