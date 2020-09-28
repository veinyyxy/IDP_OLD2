#pragma once

class AnimaticFrame;
class Map;
class ViewManager;
class RadarDataAccess;
class LoadFileCallback;

namespace osgEarth
{
	class SpatialReference;
}
class QWidget;

class AnimaticDataLoaderImp : public AnimaticDataLoader
{
public:
	AnimaticDataLoaderImp(osgViewer::View* pVM);
	virtual ~AnimaticDataLoaderImp(void);

	virtual AnimaticControlerInterface* LoadAnimaticFromFileList( const QStringList& fineList );
	virtual AnimaticControlerInterface* LoadAnimaticFromFile( const QString& fileName );
	virtual void* GetResult();
	inline void SetLoadCallback(LoadFileCallback* pLFC){m_pLoadFileCallback = pLFC;}
	//osg::Group* LoadRadarData( QString filename, osgEarth::SpatialReference* pSRF, goto_gis::Map* map);
private:
	osg::ref_ptr<osgViewer::View> m_pPlayWindow;
	AnimaticControlerInterface* m_RadarAnimatic;
	QWidget* m_pWidget;
	RadarDataAccess* m_pRaderDataAccess;
	LoadFileCallback* m_pLoadFileCallback;
};

