#pragma once
/** @file		goGeometry.h
* @brief		
* @author		������
* @date			2013.4
* @version		1.0
  @note			
*/
#include "goConfig.h"
#include "CoordinateTransform.h"

GOTO_NAMESPACE_START
class Layer;

/** @class  goGeometry
    @brief  ����ͼ�������ӿ���
    @author ������
    @note   
*/

class OSGGeometryMapGEOSGeometry : public osg::Referenced
{
public:
	OSGGeometryMapGEOSGeometry(){m_pGeometry = 0;}
	const geom::Geometry* m_pGeometry;
};

class GEOSONTHEOSG_EXPORT goGeometry
{
public:
	typedef std::map<int, boost::any> ATTRIBUTE_MAP, *P_ATTRIBUTE_MAP;
	typedef std::pair<int, boost::any>  ATTRIBUTE_MAPPair, *P_ATTRIBUTE_MAPPair;
	
		
	
	goGeometry(void);
	goGeometry(geom::Geometry* geometry, CoordinateTransform* pCT, Layer* pL);

	/*inline goGeometry(geom::CoordinateSequence* pCoordinateSequence, CoordinateTransform* pCT, Layer* pL) 
	: m_pCoordinateSequence(pCoordinateSequence), m_pCorrdinateTransform(pCT), m_pParentLayer(pL){m_pOsgGeometry = new osg::Geometry;};*/
	virtual ~goGeometry(void);

	/** 
	  * @brief			����osgͼ���GEOS�ĵ���������
	  * @author			������
	  * @param [in]		osg::Node
	  * @param [out]	null
	  * @return			null
	  * @note			
	  * @date			2013.4                                
	  * @exception		null  
	  * @remarks remark null             
	  * @deprecated		null          
	  * @since			1.0           
	  * @see references         
	*/
	virtual void CreateOsgFromGeos(osg::Node* pNode) = 0;

	/** 
	  * @brief			��������ͶӰ�任
	  * @author			������
	  * @param [in]		CoordinateTransform*
	  * @param [out]	null
	  * @return			null
	  * @note			
	  * @date			2013.4                                
	  * @exception		null  
	  * @remarks remark null             
	  * @deprecated		null          
	  * @since			1.0           
	  * @see references         
	*/
	virtual void SetCoordinateTransform(CoordinateTransform* pCT) = 0;

	/** 
	  * @brief			�õ���������
	  * @author			������
	  * @param [in]		null
	  * @param [out]	null
	  * @return			geom::GeometryTypeId
	  * @note			
	  * @date			2013.4                                
	  * @exception		null  
	  * @remarks remark null             
	  * @deprecated		null          
	  * @since			1.0           
	  * @see references         
	*/
	virtual geom::GeometryTypeId GetGeometryType() = 0;

	/** 
	  * @brief			�任����
	  * @author			������
	  * @param [in]		null
	  * @param [out]	null
	  * @return			null
	  * @note			
	  * @date			2013.4                                
	  * @exception		null  
	  * @remarks remark null             
	  * @deprecated		null          
	  * @since			1.0           
	  * @see references         
	*/
	virtual void TransformGeometry() = 0;

	/** 
	  * @brief			����GEOS������
	  * @author			������
	  * @param [in]		geom::Geometry*
	  * @param [out]	null
	  * @return			null
	  * @note			
	  * @date			2013.4                                
	  * @exception		null  
	  * @remarks remark null             
	  * @deprecated		null          
	  * @since			1.0           
	  * @see references         
	*/
	virtual void CreateGeometry(geom::Geometry* pGeosGeometry){m_pGeosGeomtry.reset(pGeosGeometry);}

	/** 
	  * @brief			����GEOS������
	  * @author			������
	  * @param [in]		geom::Geometry*
	  * @param [out]	null
	  * @return			null
	  * @note			
	  * @date			2013.4                                
	  * @exception		null  
	  * @remarks remark null             
	  * @deprecated		null          
	  * @since			1.0           
	  * @see references         
	*/
	inline void SetGeosGeomtry(geom::Geometry* pG){return m_pGeosGeomtry.reset(pG);};

	/** 
	  * @brief			���OSG������
	  * @author			������
	  * @param [in]		null
	  * @param [out]	null
	  * @return			osg::Geometry*
	  * @note			
	  * @date			2013.4                                
	  * @exception		null  
	  * @remarks remark null             
	  * @deprecated		null          
	  * @since			1.0           
	  * @see references         
	*/
	inline osg::Geometry* GetOsgGeometry(){return m_pOsgGeometry;}

	/** 
	  * @brief			���ü���ID 
	  * @author			������
	  * @param [in]		int
	  * @param [out]	null
	  * @return			null
	  * @note			
	  * @date			2013.4                                
	  * @exception		null  
	  * @remarks remark null             
	  * @deprecated		null          
	  * @since			1.0           
	  * @see references         
	*/
	inline void SetGeometryID(int iID){m_iGeometryID = iID;}

	/** 
	  * @brief			�õ�����ID
	  * @author			������
	  * @param [in]		null
	  * @param [out]	null
	  * @return			null
	  * @note			
	  * @date			2013.4                                
	  * @exception		null  
	  * @remarks remark null             
	  * @deprecated		null          
	  * @since			1.0           
	  * @see references         
	*/
	inline int GetGeometryID(){return m_iGeometryID;}

	/** 
	  * @brief			���ü����������ĸ���
	  * @author			������
	  * @param [in]		Layer*
	  * @param [out]	null
	  * @return			null
	  * @note			
	  * @date			2013.4                                
	  * @exception		null  
	  * @remarks remark null             
	  * @deprecated		null          
	  * @since			1.0           
	  * @see references         
	*/
	inline void SetParentLayer(Layer* pL){m_pParentLayer = pL;}

	/** 
	  * @brief			�õ�������ز�
	  * @author			������
	  * @param [in]		null
	  * @param [out]	null
	  * @return			Layer*
	  * @note			
	  * @date			2013.4                                
	  * @exception		null  
	  * @remarks remark null             
	  * @deprecated		null          
	  * @since			1.0           
	  * @see references         
	*/
	inline Layer* GetParentLayer(){return m_pParentLayer;}

	/** 
	  * @brief			ͨ��GEOM�������г�ʼ��
	  * @author			������
	  * @param [in]		geom::CoordinateSequence*
	  * @param [out]	null
	  * @return			null
	  * @note			
	  * @date			2013.4                                
	  * @exception		null  
	  * @remarks remark null             
	  * @deprecated		null          
	  * @since			1.0           
	  * @see references         
	*/
	inline void SetCoordinateSequence(geom::CoordinateSequence* pCoordinateSequence){m_pCoordinateSequence = pCoordinateSequence;}

	/** 
	  * @brief			�õ�GEOM������
	  * @author			������
	  * @param [in]		null
	  * @param [out]	null
	  * @return			null
	  * @note			
	  * @date			2013.4                                
	  * @exception		null  
	  * @remarks remark null             
	  * @deprecated		null          
	  * @since			1.0           
	  * @see references         
	*/
	inline geom::Geometry* GetGeomGeometry(){return m_pGeosGeomtry.get();}

	/** 
	  * @brief			�õ��Ƿ�μ�ͶӰ�任
	  * @author			������
	  * @param [in]		null
	  * @param [out]	null
	  * @return			null
	  * @note			
	  * @date			2013.4                                
	  * @exception		null  
	  * @remarks remark null             
	  * @deprecated		null          
	  * @since			1.0           
	  * @see references         
	*/
	inline bool GetBTransform() {return m_bTransform;}

	/** 
	  * @brief			�����Ƿ�μ�ͶӰ�任    
	  * @author			������
	  * @param [in]		bool
	  * @param [out]	null
	  * @return			null
	  * @note			
	  * @date			2013.4                                
	  * @exception		null  
	  * @remarks remark null             
	  * @deprecated		null          
	  * @since			1.0           
	  * @see references         
	*/
	virtual void operator=(const goGeometry& other){};
	inline void SetBTransform(bool bTransform) { m_bTransform = bTransform;}

	inline osg::Vec4Array* GetColorArray(){return m_pColors;}
	inline void UserLayerColor(bool bU){m_bUserLayerColor = bU;}
	inline void UserLayerZValue(bool bU){m_bUserLayerZ = bU;}

	inline std::string GeometryName() {return m_strName;}
	inline void GeometryName(std::string strName) {m_strName = strName;}

	virtual void UpdateGeometry();

	inline void AddAttribute(boost::any& anyAttribute){
		int iID = m_GeometryAttribute.size();m_GeometryAttribute[iID] = anyAttribute;
	}
	inline void GetAttribute(goGeometry::ATTRIBUTE_MAP* pAttribtueMap){*pAttribtueMap = m_GeometryAttribute;}

	virtual void TransormPoint(osg::Vec3& v3In, osg::Vec3& v3Out);

protected:
	void UpdateColor();
	int m_iGeometryID;
	CoordinateTransform* m_pCorrdinateTransform;
	osg::ref_ptr<osg::Geometry> m_pOsgGeometry;
	osg::ref_ptr<osg::Vec4Array> m_pColors;
	std::auto_ptr<geom::Geometry> m_pGeosGeomtry;
	Layer* m_pParentLayer;

	ATTRIBUTE_MAP m_GeometryAttribute;
	geom::CoordinateSequence* m_pCoordinateSequence;
	bool m_bTransform;
	bool m_bUserLayerColor;
	bool m_bUserLayerZ;
	std::string m_strName;
};

GOTO_NAMESPACE_END