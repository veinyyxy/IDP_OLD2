#pragma once
#include <idp_control_global.h>

class IDP_CONTROL_EXPORT RadarCameraControler
{
public:
	RadarCameraControler(void);
	virtual ~RadarCameraControler(void);
	void ToRadarViewPort(double x, double y, double z,
		double heading_deg,
		double pitch_deg, 
		double range);

	inline void SetViewer(osgViewer::Viewer* pV){m_pViewer = pV;}
	inline osgViewer::Viewer* GetViewer(){return m_pViewer;}
private:
	osgViewer::Viewer* m_pViewer;
};

