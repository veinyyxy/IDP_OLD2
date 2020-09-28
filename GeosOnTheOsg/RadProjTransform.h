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
	//���ݾ�γ��ֵ�����㷽λ�͵��Ľ�
	inline void geo2polar(double phy, double delta_numda, double &beta, double &theta);
	inline void polar2geo(double beta, double theta, double &phy, double &delta_numda);

	inline void arcS_of_gps(double az, double S,double& lon, double& lat);

	inline void gps_to_Azimuth(double lon, double lat, double& az, double& beta);
	inline void gps_to_arcS(double lon, double lat, double& Az, double& S);
	inline void gps_to_arcS(double lon,double lat, int& sx, int& sy);

	inline void Initialized(double dNdz, int x0, int y0, int res);
public:
	inline void Set(double lon, //�״�վ�ľ��ȣ�������
		double lat,//�״�վ��γ�ȣ�������
		double h //�״����ߵİκ��߶ȣ��ף�
		);

	//�����״�ز��������Ŀ����ĵ�������
	inline void tilt_of_gps(double e, double dl, double& dh, double& dS);
	inline void tilt_of_gps(double e,  double az, double dl,double& lon, double& lat,double& dh,double& dS );

	inline void tilt_of_gps(double e, int i, int j, double& lon, double& lat,double& dh, double& dS) ;

	//���ݾ�γ��ֵ�����������㷽λ������
	inline void gps_to_tilt(double lon, double lat,double e, double& Az, double& L,double& H );
	inline void gps_to_tilt(double lon,double lat,double e, double& x, double& y);

	inline void DEM_to_tilt(double lon, double lat, double h, \
		double& ele, double& az ,double& L,double& S) ;

	inline void ES_of_H(double e, double dS, double& dh);
	inline void HS_of_E(double H, double dS, double& ele);

	inline void EH_to_LS(double e, double dh, double& dl, double& ds);

	//���ݼ��㲨���߶�
	inline void RayPath_of_SH(double e, int Bins, short *S, short *H );


	double       Get_Angle_Of_Direction(double dx, double dy);
	double	     Get_Length(double dx, double dy);
	inline osg::Vec3 xyWorld2DC	(double lx,double ly);                 //��γ��ת��Ϊ��Ļ���꣬xΪ���ȣ�yΪγ��
	inline osg::Vec3 DC2xyWorld(double x, double y,double angle);                   //��Ļ����ת��Ϊ��γ�ȣ�xΪ�����꣬yΪ������
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
//--------------------  ��������ϵ��λ���� ---------------------------
