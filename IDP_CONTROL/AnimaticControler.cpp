#include <QtCore/QStringList>
#include <QtCore/QMap>
#include <QtCore/QDir>

#include <osgUtil/SmoothingVisitor>
#include <osgEarth/ImageUtils>
#include <osgEarth/MapNode>
#include <OsgEarth/ModelLayer>
#include <osgEarthUtil/AutoClipPlaneHandler>
#include <osgEarthUtil/SkyNode>
#include <osgEarthUtil/EarthManipulator>
#include <osgEarthDrivers/feature_ogr/OGRFeatureOptions>
#include <osgEarthDrivers/model_feature_geom/FeatureGeomModelOptions>

#include "Map.h"
#include "AnimaticControlerInterface.h"
#include "AnimaticDataLoader.h"
#include "AnimaticDataLoaderImp.h"
#include "AnimaticControler.h"

AnimaticControler::AnimaticControler(osgViewer::View* pVM) : m_pWidget(0), m_pRadarAnimatic(0)
{
	m_LoadFileYes = false;
	m_pAnimaticDataLoader = new AnimaticDataLoaderImp(pVM);
}


AnimaticControler::~AnimaticControler(void)
{
}

void AnimaticControler::Play()
{
	if(m_pRadarAnimatic) m_pRadarAnimatic->Play();
}

void AnimaticControler::Stop()
{
	if(m_pRadarAnimatic) m_pRadarAnimatic->Stop();
}

void AnimaticControler::Loop()
{
	if(m_pRadarAnimatic) m_pRadarAnimatic->Loop();
}

void AnimaticControler::Pause()
{
	if(m_pRadarAnimatic) m_pRadarAnimatic->Pause();
}

void AnimaticControler::LoadDataFromNameList( const QStringList& strList )
{
	m_pRadarAnimatic = m_pAnimaticDataLoader->LoadAnimaticFromFileList(strList);

	if(m_pRadarAnimatic)
	{
		m_pWidget = (QWidget*)(m_pAnimaticDataLoader->GetResult());
	}
}

bool AnimaticControler::IsInitComplete()
{
	return m_LoadFileYes;
}

void AnimaticControler::InitData(QString path)
{
	QStringList filePathList;


	QDir playDir;
	playDir.setFilter(QDir::Dirs|QDir::Files);
	playDir.setCurrent(path);
	QStringList filters;
	filters << "*.bin"<< "*.bz2";
	playDir.setNameFilters(filters);

	QFileInfoList fileInfoList = playDir.entryInfoList();
	int fileCount = fileInfoList.size();

	/*if(fileCount > 5)
	fileCount = 5;*/
	for(int iFileNum = 0; iFileNum < fileCount; iFileNum ++)
	{
		QFileInfo fileInfo = fileInfoList.at(iFileNum);

		QString filePath = fileInfo.absoluteFilePath();
		filePathList.push_back(filePath);
	}

	LoadDataFromNameList(filePathList);

	m_LoadFileYes = true;
}

void AnimaticControler::SpeedUp()
{
	if(m_pRadarAnimatic) m_pRadarAnimatic->SpeedUp();
}

void AnimaticControler::SpeedDown()
{
	if(m_pRadarAnimatic) m_pRadarAnimatic->SpeedDown();
}

void AnimaticControler::Next()
{
	if(m_pRadarAnimatic) m_pRadarAnimatic->Next();
}

void AnimaticControler::Previous()
{
	if(m_pRadarAnimatic) m_pRadarAnimatic->Previous();
}
