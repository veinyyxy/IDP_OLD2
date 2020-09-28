#pragma once
/** @file		DataProvider.h
* @brief		
* @author		杨晓宇
* @date			2012.10
* @version		1.0
  @note			
*/
#include "..\XMLProvider\RadReadColor_XML.h"
#include "goConfig.h"
#include "goGeometry.h"
#include "Layer.h"
#include <QtGui/QPixmap>
#include "../CDataElement/CElement.h"

GOTO_NAMESPACE_START

using namespace std;
typedef enum _DATATYPE
{
	RADAR_SINGLE,//雷达单站数据
	RADAR_MOSAIC,//雷达组网数据
	NO_TYPE

} DataType;


/** @class  DataProvider
    @brief  图形数据提供器接口类
    @author 杨晓宇
    @note   
*/ 
class GEOSONTHEOSG_EXPORT DataProvider
{
public:
	typedef std::pair<int, void*> INDEX_INTERFACE_PAIR, *P_INDEX_INTERFACE_PAIR;
	typedef std::map<int, void*> INDEX_INTERFACE_MAP, *P_INDEX_INTERFACE_MAP;
	DataProvider();
	virtual ~DataProvider();
public:
	/** 
	  * @brief			获得数据提供器相关联的“层”    
	  * @author			杨晓宇
	  * @param [in]		null
	  * @param [out]	null
	  * @return			Layer*
	  * @note			
	  * @date			2010.10                                
	  * @exception		null  
	  * @remarks remark null             
	  * @deprecated		null          
	  * @since			1.0           
	  * @see references         
	*/
	inline Layer* ParentLayer() {return m_pParentLayer;};

	/** 
	  * @brief			设置数据提供器相关联的“层”     
	  * @author			杨晓宇
	  * @param [in]		Layer*
	  * @param [out]	null
	  * @return			null
	  * @note			
	  * @date			2010.10                                
	  * @exception		null  
	  * @remarks remark null             
	  * @deprecated		null          
	  * @since			1.0           
	  * @see references         
	*/
	inline void ParentLayer(Layer* pl) {m_pParentLayer = pl;};

	/** 
	  * @brief			获得数据的类型    
	  * @author			杨晓宇
	  * @param [in]		null
	  * @param [out]	null
	  * @return			DataType
	  * @note			
	  * @date			2010.10                                
	  * @exception		null  
	  * @remarks remark null             
	  * @deprecated		null          
	  * @since			1.0           
	  * @see references         
	*/
	inline DataType GetDataType(){return m_edataType;};

	/** 
	  * @brief			加载文件    
	  * @author			杨晓宇
	  * @param [in]		std::string
	  * @param [out]	null
	  * @return			bool
	  * @note			
	  * @date			2010.10
	  * @exception		null
	  * @remarks remark null
	  * @deprecated		null
	  * @since			1.0 
	  * @see references         
	*/
	virtual bool LoadFile(const std::string& filename) = 0;
//	virtual inline bool LoadFile(const std::list<std::string>&filenameList ){return true;};
	/** 
	  * @brief			未知    
	  * @author			杨晓宇
	  * @param [in]		std::string
	  * @param [out]	null
	  * @return			bool
	  * @note			
	  * @date			2010.10
	  * @exception		null
	  * @remarks remark null
	  * @deprecated		null
	  * @since			1.0 
	  * @see references         
	*/
	virtual inline bool LoadRefFile(const std::string&filename) {return false;};
	
	/** 
	  * @brief			添加一个数据提供器扩展接口    
	  * @author			杨晓宇
	  * @param [in]		void*
	  * @param [out]	null
	  * @return			null
	  * @note			
	  * @date			2010.10                                
	  * @exception		null  
	  * @remarks remark null             
	  * @deprecated		null          
	  * @since			1.0           
	  * @see references         
	*/
	void InsertExtendInterface(void* pIF);
	
	/** 
	  * @brief			    
	  * @author			得到一个数据提供器扩展接口
	  * @param [in]		null
	  * @param [out]	null
	  * @return			void*
	  * @note			
	  * @date			2010.10
	  * @exception		null
	  * @remarks remark null
	  * @deprecated		null
	  * @since			1.0
	  * @see references
	*/
	void* GetExtendInterface(int iI);
	virtual P_COLORTABLE_VECTOR GetColorTable() {return m_ColorTable;}
	virtual P_SymbolLegend_Vector GetSymbolLegend() {return m_symbolLegend;}
	virtual void AreaAdd(bool tArea,bool tPie,float tMin,float tMax,osg::Vec3Array* p)= 0;
	virtual bool MineData(const std::string& strIns = "");
	virtual QString GetDataUnit(){return "";};//获得数据计量单位
	virtual float GetPositionValue(double lon,double lat,double height){return 0.0;};//获取鼠标位置的数据数值
	inline int GetProductCode(){return m_productcode;};//获取鼠标位置的数据数值
	inline std::string GetFileName(){return m_fileName.toStdString();}//获得数据名称
protected:
	Layer* m_pParentLayer;
	DataType m_edataType;//数据类型（雷达单站数据、雷达组网数据、GIS底图数据、等值线色斑图数据、卫星影像数据、云图数据等等）
	INDEX_INTERFACE_MAP m_ExtendInterface;
	P_COLORTABLE_VECTOR     m_ColorTable;
	P_SymbolLegend_Vector m_symbolLegend;
	int m_productcode;//产品编号（雷达、micaps、girb、hdf等数据类型）
	QString     m_fileName;
};

GOTO_NAMESPACE_END