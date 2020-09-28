#pragma once
#include "Map.h"
#include "MainWindow.h"
#include "MyActionInterface.h"

class RadarDataAccess;
class ViewerQT;

class FourUnitScreenAction :
	public MyActionInterface
{
public:
	ADD_MYACTION_CONTSTRUCTED_DEFINITION(FourUnitScreenAction);
	~FourUnitScreenAction(void);

	void MyAction();
	void InitMyAction();
	RadarDataAccess* m_pRadarDataAccess;
	goto_gis::Map* m_pMap;
	MainWindow* m_pMainWindow;
};

class ContourPlanesAction :
	public MyActionInterface
{
public:
	ADD_MYACTION_CONTSTRUCTED_DEFINITION(ContourPlanesAction);
	~ContourPlanesAction(void);

	void MyAction();
	void InitMyAction();
	RadarDataAccess* m_pRadarDataAccess;
};

class SectionPlansAction :
	public MyActionInterface
{
public:
	ADD_MYACTION_CONTSTRUCTED_DEFINITION(SectionPlansAction);
	~SectionPlansAction(void);

	void MyAction();
	void InitMyAction();
	RadarDataAccess* m_pRadarDataAccess;
	ViewerQT* m_pViewer;
};

class AreaStatisticsAction :
	public MyActionInterface
{
public:
	ADD_MYACTION_CONTSTRUCTED_DEFINITION(AreaStatisticsAction);
	~AreaStatisticsAction(void);

	void MyAction();
	void InitMyAction();
	RadarDataAccess* m_pRadarDataAccess;
	ViewerQT* m_pViewer;
	goto_gis::Map* m_pMap;
};

class VolumeRenderAction :
	public MyActionInterface
{
public:
	ADD_MYACTION_CONTSTRUCTED_DEFINITION(VolumeRenderAction);
	~VolumeRenderAction(void);

	void MyAction();
	void InitMyAction();
	RadarDataAccess* m_pRadarDataAccess;
	ViewerQT* m_pViewer;
	goto_gis::Map* m_pMap;
};