#pragma once
#include <osg/MatrixTransform>
#include <QList>
#include <QString>
#include <QStringList>
#include "goConfig.h"
#include "Layer.h"

GOTO_NAMESPACE_START

class osg::MatrixTransform;
class  GEOSONTHEOSG_EXPORT Map
{
public:
	Map(osgViewer::View* pAttchViewer = 0);
	virtual ~Map(void);

public:
	typedef std::pair<std::string,Layer*> MapLayerPair;
	typedef std::map<std::string, void*> EXTEND_INTERFACES, P_EXTEND_INTERFACES;
	inline QList<QString>& BasisLayerList() {return m_basisLayerList;}
	inline void BasisLayerList(QList<QString> basisLayerList){m_basisLayerList = basisLayerList;}
	void TramsformMap();
	void TramsformMap(bool useLayerZ);
	void CreateMapData();
	inline void BackgroundColor(unsigned int bgColor){m_backgroundColor = bgColor;}
	inline unsigned int BackgroundColor(){return m_backgroundColor;}

	Layer* GetLayer(const std::string& name);
	Layer* GetLayer(int iID);
	int AddLayer(const std::string& name, Layer* mapLayer);

	int RemoveLayer(const std::string& name, bool bIsDel = true);
	int RemoveLayer(goto_gis::Layer* pLayer, bool bIsDel = true);

	void SetVisibleLayer(const std::string& name,bool visible);
	void SetVisibleAllLayer(bool visible);
	inline void SetDataProvider(DataProvider* dp) { m_pDataProvider = dp;}
	inline DataProvider* GetDataProvider() {return m_pDataProvider;}

	inline Layer* CurrentLayer(){return m_pCurrentLayer;}
	inline void CurrentLayer(Layer* l) { m_pCurrentLayer = l;}

	inline void SetCoordinateTransform(CoordinateTransform* ct) {m_pCoordTrans = ct;}
	inline CoordinateTransform* GetCoordinateTransform() {return m_pCoordTrans;}

	inline double GetMapScale() { return m_mapScale;}
	inline void SetMapScale(double scale) { m_mapScale = scale;}

	void AddFishNet( float lonstart,float latstart,float lonend,float latend,float lonCellSize,float LatCellSize ,qreal qrz );

	inline osg::Switch* GetRootSwitch() {return m_pSwitch;}
	inline osg::Node* GetRootNode(){return m_pRootNode;}
	inline std::map<std::string, Layer*> GetMapLayers() const {return m_mapLayer;}
	
	void ClearMap();
	void ClearMapAndDeleteAllLayer();
	void operator=(const Map& map);
	
	virtual void Switch2DMap();
	virtual void Switch3DMap();

	inline void SetBackgroundColor(const osg::Vec4& v4c){m_v4BackGroundColor = v4c;}
	inline const osg::Vec4& GetBackgroundColor(){return m_v4BackGroundColor;}

	inline void SetLookedRegion(float fLeft, float fBottom, float fRight, float fTop){
		m_fMapLeft = fLeft;m_fMapRight = fRight;m_fMapTop = fTop;m_fMapBottom = fBottom;}

	inline void SetMapCenter(const osg::Vec3& v3Center){m_v3MapCenter = v3Center;}
	inline void GetLookedRegion(float& fLeft, float& fBottom, float& fRight, float& fTop)
	{
		fLeft	= m_fMapLeft;
		fRight	= m_fMapRight;
		fTop	= m_fMapTop;
		fBottom	= m_fMapBottom;
	}
	inline const osg::Vec3& GetMapCenter(){return m_v3MapCenter;}

	virtual void UpdateMap(){}
	virtual osg::Vec3& GetCurrentLocation(){return m_v3CurrentLocaltion;}
	virtual void SetCurrentLocation(const osg::Vec3& v3P){m_v3CurrentLocaltion = v3P;}
	/************************************************************************/
	/* 不传任何参数为缩放到整个地图                                             */
	/************************************************************************/
	virtual void ScaleToLayer(std::string strName = ""){;}
	
	/************************************************************************/
	/*                    在三维地图中定位一个经纬度位置                        */
	/************************************************************************/
	virtual void Site(osg::Vec3 pos){;}
	void GetLayerZOrder(std::map<float, std::string>* LayerName);
	void MoveUpByZ(std::string& strName);
	void MoveDownByZ(std::string& strName);
	virtual inline std::string& GetCurrentValue(){return m_CurrentValue;}
	inline void SetCurrentValue(const std::string& currentvalue){m_CurrentValue = currentvalue;}

	inline void* SetExtendInterface(std::string& strName, void* iface)
	{
		EXTEND_INTERFACES::iterator finded = m_ExtendInterface.find(strName);
		if(finded != m_ExtendInterface.end()) return finded->second;
		m_ExtendInterface[strName] = iface;
		return iface;
	}
	inline void DelExtendInterface(std::string& strName)
	{
		EXTEND_INTERFACES::iterator finded = m_ExtendInterface.find(strName);
		if(finded != m_ExtendInterface.end())
			m_ExtendInterface.erase(strName);
	}
	inline void* GetExtendInterface(std::string& strName)
	{
		EXTEND_INTERFACES::iterator finded = m_ExtendInterface.find(strName);
		if(finded != m_ExtendInterface.end())
			return finded->second;
		else
			return 0;
	}

	virtual void Load2DMapFromConfig(std::string str2dMap){;}
	virtual void Load3DMapFromConfig(std::string str3dMap){;}
	/************************************************************************/
	/*                    在二维地图中定位一个经纬度位置                        */
	/************************************************************************/
	virtual void MapLocation(float fLon, float fLat){;}
	virtual void MapScale(float fLeft, float fBottom, float fRight, float fTop){;}

	inline virtual void SetAttachView(osgViewer::View* pV){m_pView = pV;}
	inline osgViewer::View* GetAttachView(){return m_pView;}
protected:
	osg::ref_ptr<osgViewer::View> m_pView;
	Layer* CreateVectorLayer(const std::string& name);
	void SwapLayerZ(bool bdirection, std::string& strName);

	std::map<std::string, Layer*> m_mapLayer;

	Layer* m_pCurrentLayer;
	DataProvider* m_pDataProvider;
	CoordinateTransform* m_pCoordTrans;
	double m_mapScale;
	osg::ref_ptr<osg::Group> m_pRootNode;
	unsigned int m_backgroundColor;
	QList<QString> m_basisLayerList;
	osg::Switch* m_pSwitch;
	osg::Vec4 m_v4BackGroundColor;

	float m_fMapLeft;
	float m_fMapRight;
	float m_fMapTop;
	float m_fMapBottom;

	osg::Vec3 m_v3MapCenter;
	osg::Vec3 m_v3CurrentLocaltion;
	std::string m_CurrentValue;

	EXTEND_INTERFACES m_ExtendInterface;
};

GOTO_NAMESPACE_END