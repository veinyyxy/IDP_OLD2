#pragma once

#include <list>
#include <map>
#include <hash_map>
#include <boost/shared_ptr.hpp>
#include <osg/LineWidth>
#include <osg/LineStipple>
#include <osg/Point>
#include <osgText/Text>
#include <osg/Material>
#include "goConfig.h"
#include "Field.h"
#include "goGeometry.h"
#include "CoordinateTransform.h"
#include "SymbolLibraryInterface.h"
#include "DataProvider.h"

GOTO_NAMESPACE_START
class DataProvider;
class Map;

class GEOSONTHEOSG_EXPORT Layer
{
public:
	typedef std::map<int, std::auto_ptr<goGeometry>> MgoGeometry, *P_MgoGeometry;
	typedef std::pair<int, std::auto_ptr<goGeometry>> MgoGeometryPair, *P_MgoGeometryPair;
	typedef std::map<int, boost::any> LAYER_ATTRIBUTE_MAP, *P_LAYER_ATTRIBUTE_MAP;
	typedef std::pair<int, boost::any>  LAYER_ATTRIBUTE_MAPPair, *P_LAYER_ATTRIBUTE_MAPPair;
	Layer(DataProvider* data , CoordinateTransform* ct, int iNodeType = 0);
	virtual ~Layer(void);
	inline std::string LayerName(){return m_layerName;}
	inline void LayerType(std::string layerType){m_layerType = layerType;}
	inline std::string LayerType()
	{
		return m_layerType;
	}
	inline void SetChildLayer(Layer* pL){m_pChildLayer = pL;}
	inline void SetParentLayer(Layer* pL){m_pParentLayer = pL;}

public:
	P_FIELD_MAP GetFieldMap();
	P_FIELD_MAP GetLayerAttributeMap();
	void LayerZ(float z);
	float LayerZ();

	void InsertLayerAttribute(boost::any _val,
		std::string name = std::string(),
		std::string typeName = std::string(),
		std::string comment = std::string(),
		int type = 0,
		int len = 0,
		int prec = 0
		);

	double MinimumScale();
	void MinimumScale(double mix);
	void MaximumScale(double max);
	double MaximumScale();

	virtual void TransformLayer() = 0;
	virtual void CreateLayerData() = 0;
	virtual int CreateLayer(const std::string& filename) = 0;

	inline MgoGeometry* GetMgoGeometry() {return m_pgoGeometry.get();}
	inline osg::Node* GetLayerNode();
	inline void SetLayerNode(osg::Node* pN){
		m_pNode = pN;
	}

	osg::Vec4Array* GetColorArray();

	inline Map* GetParentMap(){return m_pParentMap;}
	inline void SetParentMap(Map* pM){m_pParentMap = pM;}
	inline P_LAYER_ATTRIBUTE_MAP GetLayerAttribteTable(){return &m_pLayerAttribteTable;}
	inline DataProvider* GetDataProvider(){return m_dataProvider.get();}
	inline void SetDataProvider(DataProvider* pD){m_dataProvider.reset(pD);}
	inline void UseLayerZ(bool bU){m_bUseLayerZ = bU;}
	inline bool UseLayerZ(){return m_bUseLayerZ;}
	void CoordTrans(CoordinateTransform* ct);
	CoordinateTransform* CoordTrans();

	inline void GeometrySize(float size) {m_fSize = size;}
	inline float GeometrySize() {return m_fSize;}

	inline void FontSize(float fontSize) {m_fontSize = fontSize;}
	inline float FontSize() {return m_fontSize;}

	inline void TextColor(unsigned int textColor){m_textColor = textColor;}
	inline unsigned int TextColor(){return m_textColor;}

	inline void ShowFiedName(std::string name){m_fiedName = name;}
	inline std::string ShowFiedName(){return m_fiedName;}

	bool GetLayerAtrributeFromName(const std::string& name, boost::any& pValue);
	inline bool Visible() {return m_bVisible;}
	inline void Visible(bool bVisible){m_bVisible = bVisible;}

	inline void SetLayerName(const std::string& layerName){m_pNode->setName(layerName);m_layerName = layerName;}
	inline std::string GetLayerName(){return m_pNode->getName();}

	inline void SetLineWidth(float dW){if(m_LineWidth) m_LineWidth->setWidth(dW);}
	inline void SetLineStripple(int iFactor, short shPattern){if(m_LineStripple) m_LineStripple->setFactor(iFactor);m_LineStripple->setPattern(shPattern);}
	inline void SetPointSize(float dS){if(m_PointSize) m_PointSize->setSize(dS);}

	inline void SetSymbolLibrary(SymbolLibraryInterface* pSLI){m_pSymbolLibrary = pSLI;}
	inline SymbolLibraryInterface* GetSymbolLibrary(){return m_pSymbolLibrary;}
	inline void SetLegendID(int iID){m_iUsedLegendID = iID;}
	inline int GetLegendID(){return m_iUsedLegendID;}
	virtual void operator=(const Layer& other){};
	inline void SetTrans(bool trans){b_trans = trans;}//标示图层是否参加投影变换
	inline bool Gettrans(){return b_trans;}

	inline void SetLayerColor(const osg::Vec4& v4Color){m_v4LayerColor.set(v4Color.r(), v4Color.g(), v4Color.b(), m_uAlphaValue/100.0);}
	inline const osg::Vec4& GetLayerColor(){return m_v4LayerColor;}

	virtual void UpdateLayer();

	inline void SetAlphaValue(unsigned uAlpha){m_uAlphaValue = uAlpha;}
	inline unsigned GetAlphaValue(){return m_uAlphaValue;}
protected:

	inline virtual void SyncLayerAttributeTable();
	osg::ref_ptr<osg::LineWidth> m_LineWidth;
	osg::ref_ptr<osg::LineStipple> m_LineStripple;
	osg::ref_ptr<osg::Point> m_PointSize;
	osg::ref_ptr<osg::Material> m_LayerMaterial;
	osg::ref_ptr<osg::Node> m_pNode;
	Map* m_pParentMap;
	FIELD_MAP m_PendingFieldMap;
	FIELD_MAP m_LayerAttributeDescripe;
	LAYER_ATTRIBUTE_MAP m_pLayerAttribteTable;
	osg::Vec4 m_v4LayerColor;
	float m_layerZ;
	float m_fSize;
	double m_minScale;
	double m_maxScale;
	CoordinateTransform* m_coordTrans;
	boost::shared_ptr<DataProvider> m_dataProvider;
	//是否参加投影变换（针对雷达单站数据）
	bool b_trans;
	bool m_bUseLayerZ;
	unsigned int m_textColor;
	std::auto_ptr<MgoGeometry> m_pgoGeometry;
	float m_fontSize;
	std::string m_fiedName;
	bool m_bVisible;
	std::string m_layerType;
	std::string m_layerName;

	SymbolLibraryInterface* m_pSymbolLibrary;
	int m_iUsedLegendID;

	//图层链表
	Layer* m_pParentLayer;
	Layer* m_pChildLayer;

	unsigned m_uAlphaValue;
};

GOTO_NAMESPACE_END