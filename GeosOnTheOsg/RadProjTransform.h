#pragma once



extern "C"
{
#include <proj_api.h>
}

#include "QT_Include.h"
#include "CoordinateTransform.h"
#include "cpl_string.h"
#include <ogr_spatialref.h>



#define PI 3.1415927


typedef std::string PROJCRS;
typedef std::string PROJNAME;

GOTO_NAMESPACE_START
class GEOSONTHEOSG_EXPORT RadProjTransform :public CoordinateTransform
{
public:
	RadProjTransform(void);
	~RadProjTransform(void);
	virtual osg::Vec3 Transform(const osg::Vec3 vertex);
	virtual osg::Vec3 Transform(GLdouble gLx, GLdouble gLy, GLdouble gLz) ;
	virtual int Transform(const osg::Vec3Array& src, osg::Vec3Array& dst);
	virtual int Transform(const osg::Vec3& src, osg::Vec3& dst);
	//virtual QRectF TransformBoundingBox(const QRectF rectangle);
	virtual std::string ImportFromESRI(const char *  pszFname ) {return NULL;};
	virtual void setDestCRS (std::string ProjString) ;
	virtual std::string getDesCRS();
	virtual void setSourceCRS (std::string ProjString) ;
	virtual std::string getSourceCRS() ;

	virtual std::string getDesProjName() ;
	virtual std::string getSourceProjName() ;
	virtual void setSourceProjName(std::string ProjString);
	virtual void setDesProjName(std::string ProjString);
	virtual void initProMap();
	virtual void initGeoMap() ;
	virtual bool setCenter(double lon,double lat);
	virtual void getCenter(double &lon,double &lat){lon = m_lon;lat = m_lat;};

public:
	//根据经纬度值来计算方位和地心角
	inline void geo2polar(double phy, double delta_numda, double &beta, double &theta);
	inline void polar2geo(double beta, double theta, double &phy, double &delta_numda);

	inline void arcS_of_gps(double az, double S,double& lon, double& lat);

	inline void gps_to_Azimuth(double lon, double lat, double& az, double& beta);
	inline void gps_to_arcS(double lon, double lat, double& Az, double& S);
	inline void gps_to_arcS(double lon,double lat, int& sx, int& sy);

	inline void Initialized(double dNdz, int x0, int y0, int res);
public:
	inline void Set(double lon, //雷达站的经度（度数）
		double lat,//雷达站的纬度（度数）
		double h //雷达天线的拔海高度（米）
		);

	//根据雷达回波坐标计算目标物的地理坐标
	inline void tilt_of_gps(double e, double dl, double& dh, double& dS);
	inline void tilt_of_gps(double e,  double az, double dl,double& lon, double& lat,double& dh,double& dS );

	inline void tilt_of_gps(double e, int i, int j, double& lon, double& lat,double& dh, double& dS) ;

	//根据经纬度值和仰角来计算方位、距离
	inline void gps_to_tilt(double lon, double lat,double e, double& Az, double& L,double& H );
	inline void gps_to_tilt(double lon,double lat,double e, double& x, double& y);

	inline void DEM_to_tilt(double lon, double lat, double h, \
		double& ele, double& az ,double& L,double& S) ;

	inline void ES_of_H(double e, double dS, double& dh);
	inline void HS_of_E(double H, double dS, double& ele);

	inline void EH_to_LS(double e, double dh, double& dl, double& ds);

	//根据计算波束高度
	inline void RayPath_of_SH(double e, int Bins, short *S, short *H );


	double       Get_Angle_Of_Direction(double dx, double dy);
	double	     Get_Length(double dx, double dy);
	inline osg::Vec3 xyWorld2DC	(double lx,double ly);                 //经纬度转化为屏幕坐标，x为经度，y为纬度
	inline osg::Vec3 DC2xyWorld(double x, double y,double angle);                   //屏幕坐标转化为经纬度，x为横坐标，y为纵坐标
	inline osg::Vec3 DC2rvWorld(double x,double y);
	//inline int  	     yWorld2DC	(double ly,double lx);
	bool isSet(){m_isCheck = true;return m_isSet;};
	void Elevation(bool b){m_elevation = b;};
	inline void setProductAngle(double angle){m_productAngle = angle;};
public:
	double r_numda, r_phy, r_h;
	double EarthRadius, r_res ;
	int rx, ry; 
	bool m_isSet, m_isCheck,m_elevation;
	float m_productAngle;
private:
	double Km;
	projPJ mSourceProjection,mDestinationProjection;
	std::string strSourceProjection, strDestinationProjection;
	std::string strSourceProName,strDestinationProName;
	int i,projResult;
	QString msg,points;
	qreal x,y,z;
	qreal xMAX,yMAX,xMIN,yMIN;
	void initialise();
	int transformCoords( const int &numPoints, double *x, double *y, double *z);
public:
	QVector3D m_central;
private:	
	QMap<PROJNAME,PROJCRS> Pro_map;
	QMap<PROJNAME,PROJCRS> Geo_map;

};

GOTO_NAMESPACE_END
//--------------------  地球坐标系定位计算 ---------------------------
