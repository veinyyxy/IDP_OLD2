#pragma once
#include "idp_control_global.h"


using namespace goto_gis;
class QGLWidget;

class IDP_CONTROL_EXPORT EarthquakeFileDisplay : public QObjectUserData
{
public:
	EarthquakeFileDisplay(ViewerQT* pvewer);
	virtual ~EarthquakeFileDisplay(void);

	QWidget* CreateView();
	Layer* ReadEarthquakeFileToLayer(const QString& fileName, goto_gis::CoordinateTransform* ct);
	Layer* ReadTIFFFileToLayer(const QString& fileName, goto_gis::CoordinateTransform* ct);
	void LoadBasicGeogInfo(const QString& configName);
	void OpenEarthquakeFile(const QString& mapConfig, const QString& fileName);
	void OpenTIFFFile(const QString& fileName,const QString& mapConfig = QString());
	void OpenEarthquakeFile(const QString& fileName);
	inline goto_gis::Map * Getmap(){return m_pMap;};
	//inline GISMapManager * GetMapManager(){return &m_MapManager;};
private:
	goto_gis::Map* m_pMap;
	//GISMapManager m_MapManager;
	ViewerQT* m_pViewer;
};

