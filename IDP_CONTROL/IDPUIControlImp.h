#pragma once
#include "idp_control_global.h"

class IDP_CONTROL_EXPORT IDPUIControlImp
{
public:
	IDPUIControlImp(void);
	virtual ~IDPUIControlImp(void);
	int InitIDPUIControler();
	inline RadarDataAccess* GetRaderDataAccess(){return m_RaderDataAccess;}
	inline AnimaticControler* GetAnimaticControler(){return &m_AnimaticControler;}

private:
	ViewManager m_ViewManager;
	LayerTreeControler m_LayerTreeControler;
	RadarDataAccess* m_RaderDataAccess;
	AnimaticControler m_AnimaticControler;
	QGLWidget* m_pGLWidget;
	osgViewer::Viewer* m_pOsgViewer;
};

