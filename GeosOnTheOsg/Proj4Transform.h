#pragma once

#include "goConfig.h"
extern "C"
{
#include <proj_api.h>
}
#include "CoordinateTransform.h"

GOTO_NAMESPACE_START

class  GEOSONTHEOSG_EXPORT Proj4Transform : public CoordinateTransform
{
public:
	Proj4Transform(void);
	virtual ~Proj4Transform(void);
	virtual osg::Vec3 Transform(const osg::Vec3 vertex);
	virtual osg::Vec3 Transform(double gLx, double gLy, double gLz);
	virtual int Transform(const osg::Vec3& src, osg::Vec3& dst);
	virtual void setDestCRS (std::string ProjString);
	virtual void setSourceCRS (std::string ProjString);
	virtual std::string getDesCRS();
	virtual std::string getSourceCRS();
	std::string ImportFromESRI(const char *  pszFname );

 	virtual std::string getDesProjName();
 	virtual std::string getSourceProjName();
	virtual void setSourceProjName(std::string ProjString);
	virtual void setDesProjName(std::string ProjString);
	virtual bool setCenter(double lon,double lat);
	virtual void getCenter(double &lon,double &lat){lon = m_lon;lat = m_lat;};
private:
	projPJ mSourceProjection,mDestinationProjection;
	std::string strSourceProjection, strDestinationProjection;
	std::string strSourceProName,strDestinationProName;
	int i,projResult;
	std::string msg, points;
	double x,y,z;
	double xMAX,yMAX,xMIN,yMIN;
	void initialise();
	int transformCoords(const int &numPoint, double *x, double *y, double *z);
};

GOTO_NAMESPACE_END

