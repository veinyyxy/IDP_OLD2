#pragma once
/** @file		GeoTo3DTransform.h
* @brief		
* @author		������
* @date			2012.10
* @version		1.0
  @note			
*/

#include "goConfig.h"
#include "CoordinateTransform.h"
GOTO_NAMESPACE_START

/** @class  DataProvider
    @brief  ͼ�������ṩ���ӿ���
    @author ������
    @note   ����ͶӰ��ʵ���࣬��Ա���ܲμ��ӿ��ࡣ�������Ǹ��ǽӿ��࣬��ؼ�������
*/ 
class  GEOSONTHEOSG_EXPORT GeoTo3DTransform :	public CoordinateTransform
{
public:
	GeoTo3DTransform(void);
	virtual ~GeoTo3DTransform(void);
	virtual osg::Vec3 Transform(const osg::Vec3 vertex);
	virtual osg::Vec3 Transform(GLdouble gLx, GLdouble gLy, GLdouble gLz);
	virtual int Transform(const osg::Vec3Array& src, osg::Vec3Array& dst);
	virtual int Transform(const osg::Vec3& src, osg::Vec3& dst);
	//virtual QRectF TransformBoundingBox(const QRectF rectangle, TRANSFORM_DIRECTION direction = FORWARD_DIRECTION);
	virtual std::string ImportFromESRI(const char *  pszFname );
	virtual void setDestCRS (std::string ProjString);
	virtual std::string getDesCRS();
	virtual void setSourceCRS (std::string ProjString);
	virtual std::string getSourceCRS();

	virtual std::string getDesProjName();
	virtual std::string getSourceProjName();
	virtual void setSourceProjName(std::string ProjString);
	virtual void setDesProjName(std::string ProjString);
	virtual void initProMap();
	virtual void initGeoMap();
private:
	void GeoTo3D(float fIX, float fIY, float fIZ, float* fOX, float* fOY, float* fOZ);
	float m_fEarthR;
};
GOTO_NAMESPACE_END
