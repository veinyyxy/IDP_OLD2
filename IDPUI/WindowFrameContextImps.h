#pragma once
#include "windowframecontext.h"
class WindowFrameContextImps :
	public WindowFrameContext
{
public:
	WindowFrameContextImps(void);
	~WindowFrameContextImps(void);

	virtual ViewerQT* GetCurrentQGLWidget();

	virtual QMdiSubWindow* GetCurrentSubWindow();

	inline void SetViewerQT(ViewerQT* pV){
		m_pViewerQT = pV;
	}
	inline void SetMdiSubWindow(QMdiSubWindow* pMSW){
		m_pMdiWindow = pMSW;
	}

private:
	ViewerQT* m_pViewerQT;
	QMdiSubWindow* m_pMdiWindow;
};

