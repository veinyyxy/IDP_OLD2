#pragma once
#include "idp_control_global.h"
#include "Map.h"
#include "Layer.h"
#include <map>

class ViewManager;
class QGLWidget;
class DrawLrmImage;
class DefaultManipulator;
class ViewerQT;
class IsoSurface;

using namespace goto_gis;


class IDP_CONTROL_EXPORT RadarDataAccess : public QObject
{
	Q_OBJECT

public:
	virtual ~RadarDataAccess(void);
	QWidget*  ReadRadarData(const QString& fileName, int wdt,QWidget* parent = 0);

	inline void SetViewManager(ViewManager* pVM){m_pViewManager = pVM;}
	inline ViewManager* GetViewManger(){return m_pViewManager;}

	void Switch2DMap(ViewerQT* viewer);//二、三维切换 变换操控器
	void Switch3DMap(ViewerQT* viewer);

	QWidget* Win4Viewer(Map* srcMap); // 四联屏
	void DB2Widget(Map* srcMap);
	void PlotWidget(ViewerQT* viewer); // 剖面
	void AreaWidget(Map* srcMap); //面积统计
	void VolumeRender(Map* srcMap);
	void Fuzzy(Map* srcMap);
	void ClipPlane(Map* srcMap, int iD = 0);
	void DleClipPlane(Map* srcMap, int iD);
	void LoadRadarClipPlane(Map* p3DMap, IsoSurface* pOEB );
	void InitMachCubeData(IsoSurface* pOEB);
	void InitMachCubeData( Map* p3DMap, IsoSurface* pOEB );
	void SetRadarCenter(Map* p3DMap, IsoSurface* pOEB );
	void LoadRadarData( QString filename ,Map* map);

	void MakeFenCengZuHeFanSheLuRect(DefaultManipulator* pDrawRect);

	void MakeFenCengZuHeFanSheLu(const QStringList& strList);
	void LoadRadarDataThreadSafe(QString filename ,Map* map);

	static RadarDataAccess* Instance(ViewManager* pVM = 0,QObject* parent = 0);
private:
	RadarDataAccess(QObject* parent = 0);
	RadarDataAccess(ViewManager* pVM ,QObject* parent = 0);
	RadarDataAccess(RadarDataAccess&){}
	void operator=(RadarDataAccess&){}
	boost::any GetAttributeValue(int index ,Layer* layer);
	double Get_Length( double dx, double dy );
	void StartShowLayerIndex(Map* map , int index);
	//////////////////////////////////////////////////////////////////////////
	static RadarDataAccess* g_RadarDataAccess;
private:
	DrawLrmImage* m_DrawLrmImage;
	ViewManager* m_pViewManager;
	std::map< int, void*> m_ClipBoardMap;

};

