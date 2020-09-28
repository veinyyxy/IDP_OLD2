#pragma once
/** @file		CoordinateTransform.h
* @brief     
* @author		������
* @date			2012.10
* @version		1.0
  @note			
*/

#include "goConfig.h"

GOTO_NAMESPACE_START

/** @class  CoordinateTransform
    @brief  ����ͶӰת���ӿ���
    @author ������
    @note   
*/ 
class GEOSONTHEOSG_EXPORT CoordinateTransform
{
public:
	typedef enum __TransformType__{UNKNOW, TRANSFORM_2D, TRANSFORM_3D} TRANSFORM_TYPE;
	CoordinateTransform(void);
	virtual ~CoordinateTransform(void);

	/** 
	  * @brief			����ת������    
	  * @author			������
	  * @param [in]		osg::Vec3 double osg::Vec3Array osg::Vec3
	  * @param [out]	osg::Vec3
	  * @return			osg::Vec3
	  * @note			
	  * @date			2010.10                                
	  * @exception		null  
	  * @remarks remark null             
	  * @deprecated		null          
	  * @since			1.0           
	  * @see references         
	*/
	virtual osg::Vec3 Transform(const osg::Vec3 vertex) = 0;
	virtual osg::Vec3 Transform(double gLx, double gLy, double gLz) = 0;
	virtual int Transform(const osg::Vec3& src, osg::Vec3& dst) = 0;
	//virtual QRectF TransformBoundingBox(const QRectF rectangle, TRANSFORM_DIRECTION direction = FORWARD_DIRECTION) = 0;
	virtual std::string ImportFromESRI(const char *  pszFname ) = 0;

	/** 
	  * @brief			����Ŀ��ͶӰ    
	  * @author			������
	  * @param [in]		std::string
	  * @param [out]	void
	  * @return			void
	  * @note			
	  * @date			2010.10                                
	  * @exception		null  
	  * @remarks remark null             
	  * @deprecated		null          
	  * @since			1.0           
	  * @see references
	*/
	virtual void setDestCRS (std::string ProjString) = 0;

	/** 
	  * @brief			�õ�Ŀ��ͶӰ    
	  * @author			������
	  * @param [in]		null
	  * @param [out]	std::string
	  * @return			null
	  * @note			
	  * @date			2010.10                                
	  * @exception		null  
	  * @remarks remark null             
	  * @deprecated		null          
	  * @since			1.0           
	  * @see references         
	*/
	virtual std::string getDesCRS() = 0;

	/** 
	  * @brief			����ԴͶӰ    
	  * @author			������
	  * @param [in]		std::string
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
	virtual void setSourceCRS (std::string ProjString) = 0;

	/** 
	  * @brief			�õ�ԴͶӰ    
	  * @author			������
	  * @param [in]		null
	  * @param [out]	null
	  * @return			std::string
	  * @note			
	  * @date			2010.10                                
	  * @exception		null  
	  * @remarks remark null             
	  * @deprecated		null          
	  * @since			1.0           
	  * @see references         
	*/
	virtual std::string getSourceCRS() = 0;

	/** 
	  * @brief			�õ�Ŀ��ͶӰ    
	  * @author			������
	  * @param [in]		null
	  * @param [out]	null
	  * @return			std::string
	  * @note			
	  * @date			2010.10                                
	  * @exception		null  
	  * @remarks remark null             
	  * @deprecated		null          
	  * @since			1.0           
	  * @see references         
	*/
	virtual std::string getDesProjName() = 0;
	/** 
	  * @brief			�õ�ԴͶӰ    
	  * @author			������
	  * @param [in]		null
	  * @param [out]	null
	  * @return			std::string
	  * @note			
	  * @date			2010.10                                
	  * @exception		null  
	  * @remarks remark null             
	  * @deprecated		null          
	  * @since			1.0           
	  * @see references         
	*/
	virtual std::string getSourceProjName() = 0;

	/** 
	  * @brief			����ת������    
	  * @author			������
	  * @param [in]		std::string
	  * @param [out]	null
	  * @return			
	  * @note			������
	  * @date			2010.10                                
	  * @exception		null  
	  * @remarks remark null             
	  * @deprecated		null          
	  * @since			1.0           
	  * @see references         
	*/
	virtual void setSourceProjName(std::string ProjString) = 0;

	/** 
	  * @brief			����ԴͶӰ    
	  * @author			������
	  * @param [in]		std::string
	  * @param [out]	null
	  * @return			null
	  * @note			������
	  * @date			2010.10                                
	  * @exception		null  
	  * @remarks remark null             
	  * @deprecated		null          
	  * @since			1.0           
	  * @see references         
	*/
	virtual void setDesProjName(std::string ProjString) = 0;

	/** 
	  * @brief			δ֪    
	  * @author			
	  * @param [in]		null
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
	virtual inline bool setCenter(double lon,double lat) =0;
	virtual inline void getCenter(double &lon,double &lat)=0;

	inline CoordinateTransform::TRANSFORM_TYPE GetTransformType(){return m_TransType;}
protected:
	double m_lat,m_lon;
	TRANSFORM_TYPE m_TransType;
};

GOTO_NAMESPACE_END