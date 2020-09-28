#include "RadProjTransform.h"

#include <cstdlib>
#include <cstdio>
#include <cmath>
#define  km 1000.00
GOTO_NAMESPACE_START
RadProjTransform::RadProjTransform(void)
{
	EarthRadius=6371.004 ; //average value of the Earth radius
	rx = ry = 0;     // default coodinates of radar site 
	r_res = 1.000; // default resolution (km) of the length of sample volumes
	m_productAngle = 0.0;
	m_isSet = false;
	m_isCheck = false;
	m_elevation = false;
	initialise();
	initProMap();
	initGeoMap();
}


RadProjTransform::~RadProjTransform(void)
{
	if ( mSourceProjection != 0&&mSourceProjection != "RADARPROJ"&&mSourceProjection != " ")
	{

		pj_free( mSourceProjection );
	}
	if ( mDestinationProjection != 0 )
	{
		if (mDestinationProjection == "RADARPROJ"||mDestinationProjection == " ")
		{
			mDestinationProjection = NULL;
		}
		else
			pj_free( mDestinationProjection );
	}
}

//
// ���ݲο��㣨�״��γ�ȡ�ĳ��γ�Ⱥ͸õ���ο���֮��ľ��Ȳ����
// �õ�����ڲο���ķ�λ�Լ��õ���ο���֮��ĵ��Ľǻ���ֵ
//
void RadProjTransform::geo2polar( double phy, double delta_numda, double &beta, double &theta )
{
	// ������Ľ�beta
	double cos_beta = sin(phy)*sin(r_phy)+cos(phy)*cos(r_phy)*cos(delta_numda);        //r_phy��ʾ�״�վγ�ȵĻ���ֵ
	double sin_beta = sqrt(1-cos_beta*cos_beta);
	if (sin_beta == 0)
	{
		theta = 0;
		beta = asin(sin_beta);
		return;
	}
	if( sin_beta>1 ) sin_beta=1;
	else if (sin_beta<-1) sin_beta=-1;
	beta = asin(sin_beta);
	double 	sin_theta = 0;
	sin_theta = cos(phy)*sin(delta_numda)/sin_beta;
	if( sin_theta>1 ) sin_theta = 1;
	else if (sin_theta<-1) sin_theta = -1;

	theta = asin(sin_theta);
	double lat90 = asin(cos_beta*sin(r_phy));

	//�жϷ�λtheta
	if( phy<lat90)
		theta = PI-theta;
	else 
		if(delta_numda<0) theta = 2*PI+theta;

	return ;
}

//
// ����ĳ����Ľǡ���λ�Ļ���ֵ�Ͳο��㣨�״��γ�ȼ���õ�
// ��γ���Լ�����֮��ľ��Ȳ�
//
void RadProjTransform::polar2geo( double beta, double theta, double &phy, double &delta_numda )
{

	double sin_phy = cos(beta)*sin(r_phy)+sin(beta)*cos(r_phy)*cos(theta);
	double cos_phy = sqrt(1-sin_phy*sin_phy);
	phy = asin(sin_phy);

	double sin_numda=sin(theta)*sin(beta)/cos_phy;

	if( sin_numda>1 ) sin_numda=1;
	else if (sin_numda<-1) sin_numda=-1;

	delta_numda = asin(sin_numda);

	return ;
}

//
// ���ݲο��㣨�״��γ�Ⱥ�ĳ������ڲο���ķ�λ
// �Լ�ˮƽ�������õ��γ�Ⱥ;���
//
void RadProjTransform::arcS_of_gps( double az, double S,double& lon, double& lat )
{

	double beta = S/EarthRadius;           //���Ľ�
	double theta = az*PI/180.0;            //��λ��
	double phy, delta_numda;

	polar2geo(beta, theta, phy, delta_numda);

	lat = phy*180/PI;
	lon = (delta_numda+r_numda)*180/PI;

	return ;
}

//
// ���ݲο��㣨�״��γ�Ⱥ�ĳ��γ�Ⱥ;��ȼ���
// �õ�����ڲο���ķ�λ�Լ�ˮƽ�����Ӧ�ĵ��Ľ�
//
void RadProjTransform::gps_to_Azimuth( double lon, double lat, double& az, double& beta )
{
	double phy = lat*PI/180.0;
	double delta_numda = lon*PI/180.0-r_numda;

	double theta;

	geo2polar(phy,delta_numda,beta, theta);

	az = theta*180.0/PI;

	return ;
}

void RadProjTransform::gps_to_arcS( double lon, double lat, double& az, double& S )
{
	gps_to_Azimuth( lon, lat, az, S);

	S *= EarthRadius;

	return ;
}

//
// ���ݲο��㣨�״��γ�Ⱥ�ĳ��γ�Ⱥ;��ȼ���
// �õ�����ڲο���ĸ�����꣨X��Y��
//
void RadProjTransform::gps_to_arcS( double lon,double lat, int& sx, int& sy )
{

	double phy = lat*PI/180.0;
	double delta_numda = lon*PI/180.0-r_numda;

	double beta, theta;

	geo2polar(phy,delta_numda,beta, theta);

	sx=int(beta*EarthRadius*sin(theta)/r_res+rx+0.5);
	sy=int(ry-beta*EarthRadius*cos(theta)/r_res+0.5);

	return ;

}

void RadProjTransform::Initialized(double dNdz,   //�����ݶ� N/Km
									int x0, int y0,  //�״�վ�ο�����
									int res// ���ֱ��ʣ��ף�
									)
{
	r_res = res*0.001 ; //Unit:Kilometer
	rx = x0; ry = y0 ; //radar position
	//	Km = 1/(1-EarthRadius*dNdz*0.000001) ;
	Km=4.0/3.0 ; //��Ч����뾶ϵ��
	
}

void RadProjTransform::Set( double lon, /*�״�վ�ľ��ȣ������� */ double lat,/*�״�վ��γ�ȣ������� */ double h /*�״����ߵİκ��߶ȣ��ף� */ )
{
	m_central = QVector3D(lon,lat,h);
	r_numda = lon*PI/180.0;;  //unit:����
	r_phy = lat*PI/180.0;  //unit:����
	r_h = h*0.001;
	m_isSet = true;
}

//�����״�ز��������Ŀ����ĵ�������
void RadProjTransform::tilt_of_gps( 
									double e,  //���Ƕ���
									double dl,   //�״�̽����루ǧ�ף�
									double& dh, //�κ��߶ȷ���ֵ��ǧ�ף�
									double& dS  //ˮƽ����
									)
{
	if(dl < r_res)
	{
		dh=r_h;
		dS=dl;
		return ;
	}

	Km=4.0/3.0;

	double Rm=Km*EarthRadius;   //��Ч����뾶
	double alpha = e*PI/180.0;
	double beta=Km*atan(dl*cos(alpha)/(Rm+r_h+dl*sin(alpha)));  

	dS=EarthRadius*beta;  //ˮƽ����
	dh = sqrt((Rm+r_h)*(Rm+r_h)+dl*dl+2*(Rm+r_h)*dl*sin(alpha))-Rm; //�߶�

	return ;
}

void RadProjTransform::tilt_of_gps( double e,  //���Ƕ���
									double az,   //�״﷽λ����
									double dl,   //�״�̽����루ǧ�ף�
									double& lon, //���ȷ���ֵ
									double& lat, //γ�ȷ���ֵ
									double& dh, //�κ��߶ȷ���ֵ��ǧ�ף�
									double& dS  //ˮƽ����
	)
{
	tilt_of_gps(e,  dl, dh, dS );

	if(dl < r_res)
	{
		lon = r_numda*180.0/PI ;
		lat = r_phy*180.0/PI ;
		return ;
	}

	arcS_of_gps(az, dS, lon, lat);

	return ;
}

void RadProjTransform::tilt_of_gps( double e,  //���Ƕ���
									int i,     //������������
									int j,     //�ϱ���������
									double& lon, //���ȷ���ֵ
									double& lat, //γ�ȷ���ֵ
									double& dh, //�κ��߶ȷ���ֵ
									double& dS  //���ȷ���ֵ 
	)
{
	double dl = sqrt(double(((i-rx)*(i-rx)+(ry-j)*(ry-j))*r_res));
	double az = asin((i-rx)*r_res/dl) ;

	if( i >= rx  && j <= ry ) az=az*180.0/PI;
	else if( i < rx  && j <= ry ) az = 360.0+az*180.0/PI;
	else az = 180.0-az*180.0/PI;

	tilt_of_gps(e, az, dl, lon, lat, dh, dS);
}


//�������Ǻ͵��������㲨���߶�
void RadProjTransform::
	ES_of_H( double e,  //elevation angle (deg)
	double dS,  //ˮƽ����,km
	double& dh //height, km
	)
{
	if( dS <= r_res ) 
		dh = r_h;
	else
	{
		Km=4.0/3.0;
		double alpha = e*PI/180.0;
		double Rm=EarthRadius*Km ;
		double betam = dS/Rm;
		dh = (Rm+r_h)*cos(alpha)/cos(alpha+betam)-Rm; //�߶�
	}
}

//����ˮƽ����͸߶ȼ�������
void RadProjTransform::
	HS_of_E(double H, //�߶ȣ�km
	double dS, //ˮƽ���룬km
	double &ele //���Ƕ���
	)
{
	ele = 0.0;
	Km=4.0/3.0;
	if(dS<0.01) return ;

	double Rm = EarthRadius*Km;
	double beta = dS/Rm;
	double cos_beta = cos(beta);
	if(cos_beta>1.0) cos_beta=1.0;
	double sin_beta=sqrt(1.0-cos_beta*cos_beta);
	double a = cos_beta*(Rm+H);
	double b = (Rm+r_h);
	double c = sin_beta*(Rm+H);

	if(a>b)
		ele = atan((a-b)/c)*180.0/PI;

	return ;
}

//�������Ǻ͸߶ȼ���б��\ˮƽ����
void RadProjTransform::
	EH_to_LS(double e, 
	double dh,
	double& dl, 
	double& ds)
{
	if( dh < r_h+0.001) 
	{
		dl = 0;
		return ;
	}

	Km=4.0/3.0;

	double alpha = e*PI/180.0;
	double Rm = EarthRadius*Km;
	double a = r_h+Rm;
	double b = dh+Rm;
	double c = a*sin(alpha);
	a = a*cos(alpha);
	dl = sqrt(b*b-a*a) - c;

	double beta = asin(dl*cos(alpha)/b);
	ds = Rm*beta;

	return ;
}

//���ݾ�γ��ֵ�����������㷽λ������
void RadProjTransform::
	gps_to_tilt(double lon, //����ֵ
	double lat, //γ��ֵ
	double e,  //���Ƕ���
	double& Az, //�״﷽λ��������ֵ
	double& L,  //�״�̽����뷵��ֵ(ǧ��)
	double& H  //�߶ȷ���ֵ(ǧ��)
	)
{
	double beta0;
	gps_to_Azimuth(lon, lat, Az, beta0);

	Km=4.0/3.0;
	beta0 = beta0/Km;

	double alpha = e*PI/180.0;
	L=(Km*EarthRadius+r_h)*sin(beta0)/cos(alpha+beta0);

	double Rm=EarthRadius*Km ;
	H = sqrt((Rm+r_h)*(Rm+r_h)+L*L+2*(Rm+r_h)*L*sin(alpha))-Rm; //�߶�
	//	H=r_h+L*sin(alpha)+L*L*cos(alpha)*cos(alpha)/(2*Rm) ;

}

void RadProjTransform::
	gps_to_tilt(double lon, //����ֵ
	double lat, //γ��ֵ
	double e,  //���Ƕ���
	double &x, double &y //���صĸ������
	) 
{
	double Az, L, H; 
	gps_to_tilt(lon,lat,e, Az,L,H );

	double sin_az=sin(Az*PI/180);
	double cos_az=cos(Az*PI/180);

	x=L*sin_az/r_res;
	y=L*cos_az/r_res;

}

//���ݾ�γ�ȺͰκ��߶ȼ������ǡ���λ�;��룻
void RadProjTransform::
	DEM_to_tilt(double lon, //����ֵ
	double lat, //γ��ֵ
	double H,   //�߶�ֵ(ǧ��)
	double &e,  //���Ƿ��ض���
	double &Az, //�״﷽λ��������ֵ
	double &L,  //�״�̽����뷵��ֵ(ǧ��)
	double &S   //ˮƽ���뷵��ֵ(ǧ��)
	)
{
	double beta;
	gps_to_Azimuth(lon,lat,Az, beta); 
	S=beta*EarthRadius;
	if( H<=r_h) //û���ڵ�
	{
		e=0;
		L=S;
		return ;
	}

	Km=4.0/3.0;

	beta=beta/Km;
	double Rm=Km*EarthRadius;
	double cos_beta = cos(beta);
	if(cos_beta>1.0) cos_beta=1.0;
	double sin_beta=sin(beta);

	e=atan(((Rm+H)*cos_beta-(Rm+r_h))/((Rm+H)*sin_beta));

	L=(Rm+H)*sin_beta/cos(e);

	e=e*180.0/PI ;

	return ;
}

//------------------   ��Ч����������뾶���ƶ�λ���� --------------------------




osg::Vec3 RadProjTransform::Transform( const osg::Vec3 vertex)
{
	osg::Vec3 Src = vertex;//(vertex.x(), vertex.z(), vertex.y());

	if (!m_isCheck)
	{
		if(!isSet())
		{
			r_numda = 1.6;
			r_phy = 0.7;
			r_h = 200;
		}
	}
	if ( mSourceProjection == mDestinationProjection) return vertex;

	x = Src.x();
	y = Src.y();
	z = Src.z();

	if(/*strDestinationProName == "RADARPROJ" ||*/ strDestinationProName == "�״�����������ͶӰ")
	{
		return xyWorld2DC(x,y) ;
	}
	else if (/*strSourceProName == "RADARPROJ"||*/ strSourceProName == "�״�����������ͶӰ")
	{
		return DC2xyWorld(x,y,m_productAngle);
	}
	else
	{
		transformCoords( 1, &x, &y, &z);
		return osg::Vec3(x, y, z);
	}
	

	
}

osg::Vec3 RadProjTransform::Transform( GLdouble gLx, GLdouble gLy, GLdouble gLz)
{
	return Transform( osg::Vec3( (qreal)gLx, (qreal)gLy, (qreal)gLz ) );
}

int RadProjTransform::Transform( const osg::Vec3Array& src, osg::Vec3Array& dst)
{
	return 0;
}

int RadProjTransform::Transform( const osg::Vec3& src, osg::Vec3& dst )
{
	if (!m_isCheck)
	{
		if(!isSet())
		{
			r_numda = 1.7;
			r_phy = 0.6;
			r_h = 200;
		}
	}
	int projResult = 0;

	if(std::string::npos  != strDestinationProName.find("�״�����������ͶӰ")/* || std::string::npos  != strDestinationProName.find("RADARPROJ")*/)
	{
		dst = xyWorld2DC(src.x(),src.y()) ;
		projResult = 0 ;
	}
	else if (std::string::npos  != strSourceProName.find("�״�����������ͶӰ"))
	{
		dst = DC2xyWorld(src.x(),src.y(),m_productAngle);
	}
	else
	{
		double dX, dY, dZ;

		if ( mDestinationProjection == NULL || mSourceProjection == NULL)
		{
			return projResult;
		}
		dX = src.x() * DEG_TO_RAD;
		dY = src.y() * DEG_TO_RAD;
		dZ = src.z() * DEG_TO_RAD;
		projResult = pj_transform(mSourceProjection, mDestinationProjection, 1, 0, &dX, &dY, &dZ);
		dst.set(dX,dY,dZ);

		//projResult = 1;
	}
	if (!m_elevation)
	{
		double xx = dst.x();
		double yy = dst.y();
		double zz = src.z();
		dst.set(xx,yy,zz);
	}
	//dst.set(dst.x(), dst.z(), dst.y());
	return projResult;
}

//QRectF RadProjTransform::TransformBoundingBox( const QRectF rectangle, TRANSFORM_DIRECTION direction /*= FORWARD_DIRECTION*/ )
//{
//	if ( mSourceProjection == mDestinationProjection ) return rectangle;
//
//	xMAX = rectangle.right();
//	yMAX = rectangle.top();
//	xMIN = rectangle.left();
//	yMIN = rectangle.bottom();
//
//	z = 0.0;
//	transformCoords( 1, &xMAX, &yMAX, &z, direction );
//	transformCoords( 1, &xMIN, &yMIN, &z, direction );
//
//	return QRectF( xMIN, yMAX, xMAX - xMIN, yMAX - yMIN );
//}

void RadProjTransform::setDestCRS( std::string ProjString )
{
	QStringList projstringlist ;
	if (std::string::npos != ProjString.find("RADARPROJ"))
	{
		projstringlist = QString(ProjString.c_str()).split(" ");
		//ProjString = "RADARPROJ";
		float x = projstringlist[1].toDouble();
		float y = projstringlist[2].toDouble();
		float z = projstringlist[3].toDouble();
		Set(x,y,z);
		strDestinationProjection = ProjString;
		setDesProjName((projstringlist[0]).toStdString());
		mDestinationProjection = " ";
		m_lat = y;
		m_lon = x;
		return;
	}
	else
	{
		if(Pro_map.contains(ProjString))
		{
			setDesProjName(Pro_map.find(ProjString).value());
		}	
		  strDestinationProjection = ProjString;  
		 
		  QString s_lat;
		  QString s_lon;
		  int iLat = -1, iLon = -1;
		  if (0 == strDestinationProjection.size()) return;
		  QString tempDestination(strDestinationProjection.c_str());
		  QStringList templist = tempDestination.split(" ");
		  bool b_lat = false,b_lon = false;
		  for(int i = 0; i < templist.size(); i++)
		  {
			  if(templist.at(i).contains("+lat_0"))
			  {
				  s_lat = templist.at(i);
				  iLat = i;
				  b_lat = true;
			  }
			  if(templist.at(i).contains("+lon_0"))
			  {
				  s_lon = templist.at(i);
				  iLon = i;
				  b_lon = true;
			  }
		  }

		  if (b_lat)
		  {
			  QStringList latlist = s_lat.split("=");
			  m_lat = latlist[1].toDouble();
		  }else
			  m_lat = 0.0;

		  if (b_lon)
		  {
			  QStringList lonlist = s_lon.split("=");
			  m_lon =lonlist[1].toDouble();
		  }else
			  m_lon = 0.0;
		mDestinationProjection = pj_init_plus((char*)(strDestinationProjection.c_str()));
	}
	if ( mDestinationProjection == NULL )
	{
		qDebug("Set mDestinationProjection fail");
	}
}

std::string RadProjTransform::getDesCRS()
{
	return strDestinationProjection;
}

void RadProjTransform::setSourceCRS( std::string ProjString )
{
	if (std::string::npos != ProjString.find("RADARPROJ"))
	{
		QStringList projstringlist ;
		projstringlist = QString(ProjString.c_str()).split(" ");
		//ProjString = "RADARPROJ";
		float x = projstringlist[1].toDouble();
		float y = projstringlist[2].toDouble();
		float z = projstringlist[3].toDouble();
		Set(x,y,z);
		strSourceProjection = ProjString;
		mSourceProjection = " ";
		setSourceProjName((projstringlist[0]).toStdString());	
		return;
	}
	else
	{
		if (Pro_map.contains(ProjString))
		{
			setSourceProjName(Pro_map.find(ProjString).value());	

			strSourceProjection = ProjString;
			mSourceProjection = pj_init_plus((char*)(strSourceProjection.c_str()));
			if ( mSourceProjection == NULL )
			{
				qDebug("Set mSourceProjection fail");
			}
		}
	}

}

std::string RadProjTransform::getSourceCRS()
{
	return strSourceProjection;
}

std::string RadProjTransform::getDesProjName()
{
	return strDestinationProName;
}

std::string RadProjTransform::getSourceProjName()
{
	return strSourceProName;
}

void RadProjTransform::setSourceProjName( std::string ProjString )
{
	strSourceProName = ProjString;
}

void RadProjTransform::setDesProjName( std::string ProjString )
{
	strDestinationProName = ProjString;
}

void RadProjTransform::initProMap()
{
	if (!Pro_map.isEmpty()) return;

	Pro_map.insert("+proj=gstmerc +lat_0=34 +lon_0=108 +x_0=0 +y_0=0 +a=6378140 +b=6356755.288157528 +units=m +no_defs","GAUSS");
	Pro_map.insert("+proj=lcc +lat_1=30 +lat_2=62 +lat_0=0 +lon_0=110 +x_0=0 +y_0=0 +a=6378140 +b=6356755.288157528 +units=m +no_defs","LAMBERT");
	Pro_map.insert("+proj=merc +lat_0=34 +lon_0=110 +x_0=0 +y_0=0 +a=6378140 +b=6356755.288157528 +units=m +no_defs","MERCATOR");
	Pro_map.insert("+proj=eqc +lat_ts=0 +lat_0=-3 +lon_0=110 +x_0=0 +y_0=0 +a=6378140 +b=6356755.288157528 +units=m +no_defs","LONGLAT");
	Pro_map.insert("+proj=stere +lat_0=90 +lon_0=110 +x_0=0 +y_0=11000000 +a=6378140 +b=6356755.288157528 +units=m +no_defs","STEREOGRAM");	
	Pro_map.insert("+proj=stere +lat_0=-90 +lon_0=110 +x_0=0 +y_0=-20000000 +a=6378140 +b=6356755.288157528 +units=m +no_defs","STEREOGRAM1");
	Pro_map.insert("+proj=aea +lat_1=27n +lat_2=45n +lon_0=110e +x_0=0 +y_0=0 +ellps=clrk66","ALBERS");

	Pro_map.insert("+proj=robin +lon_0=110 +x_0=0 +y_0=0 +a=6378140 +b=6356755.288157528 +units=m +no_defs","ROBINSON");
	Pro_map.insert("+proj=moll +lon_0=110 +x_0=0 +y_0=0 +a=6378140 +b=6356755.288157528 +units=m +no_defs","MOLLWEIDE");
	Pro_map.insert("+proj=ortho +lat_0=34 +lon_0=110 +x_0=0 +y_0=0 +a=6378140 +b=6356755.288157528 +units=m +no_defs","ORTHOGRAPHIC");
	Pro_map.insert("+proj=longlat +ellps=WGS84 +datum=WGS84 +no_defs","WGS84");
	Pro_map.insert("+proj=longlat +a=6378140 +b=6356755.288157528 +no_defs","XIAN80");
	Pro_map.insert("RADARPROJ","RADARPROJ");
}

void RadProjTransform::initGeoMap()
{
	if(!Geo_map.isEmpty()) return;

	Geo_map.insert("+proj=longlat +ellps=WGS84 +datum=WGS84 +no_defs","WGS84");
	Geo_map.insert("+proj=longlat +a=6378140 +b=6356755.288157528 +no_defs","XIAN80");
	Geo_map.insert("+proj=cea +lon_0=0 +lat_ts=30 +x_0=0 +y_0=0 +a=6371228 +b=6371228 +units=m +no_defs","LONGLAT");

}

void RadProjTransform::initialise()
{
	mSourceProjection = NULL;
	mDestinationProjection = NULL;
}

int RadProjTransform::transformCoords( const int &numPoints, double *x, double *y, double *z)
{
	if ( mDestinationProjection == NULL )
	{
		qDebug("DestinationProjection NULL");
		return 0;
	}
	if ( mSourceProjection == NULL )
	{
		qDebug("SourceProjection NULL");
		return 0;
	}

	// use proj4 to do the transform
	// if the source/destination projection is lat/long, convert the points to radians prior to transforming
	if (pj_is_latlong( mDestinationProjection )
		|| ( pj_is_latlong( mSourceProjection )))
	{
		for (i = 0; i < numPoints; ++i )
		{
			x[i] *= DEG_TO_RAD;
			y[i] *= DEG_TO_RAD;
			z[i] *= DEG_TO_RAD;
		}
	}

	projResult = pj_transform( mSourceProjection, mDestinationProjection, numPoints, 0, x, y, z );
	
	if ( projResult != 0 )
	{
		//something bad happened....

		for ( int i = 0; i < numPoints; ++i )
		{
			points += QString( "(%1, %2)\n" ).arg( x[i] ).arg( y[i] );

			msg = QString( "%1 of %2 failed with error: %3\n" ).arg( x[i] ).arg( y[i] ).arg( QString::fromUtf8( pj_strerrno( projResult ) ) );
			qDebug("pj_transform fail");
			qDebug()<<msg;

		}


		return 0;

	}

	// if the result is lat/long, convert the results from radians back to degrees
	if (pj_is_latlong( mDestinationProjection )
		|| ( pj_is_latlong( mSourceProjection )) )
	{
		for ( int i = 0; i < numPoints; ++i )
		{
			x[i] *= RAD_TO_DEG;
			y[i] *= RAD_TO_DEG;
			z[i] *= RAD_TO_DEG;
		}
	}
	return 1;
}

osg::Vec3 RadProjTransform::xyWorld2DC( double lx,double ly )
{
	double direction,range,height ,productAngle = 0.0;
	gps_to_tilt(lx,ly,productAngle,direction,range,height);
	return  osg::Vec3(range * sin(direction*PI/180) * km,range * cos(direction*PI/180) * km,0);       
}

osg::Vec3 RadProjTransform::DC2xyWorld( double x, double y ,double angle)
{
	double temp_x =x/km;
	double temp_y = y/km;
	double direction,range;
	double lat,lon,dh,dS;   //���ȡ�γ�ȡ����θ߶ȡ�ˮƽ����
	direction = 180 *Get_Angle_Of_Direction(temp_x,temp_y) /PI;
	range = Get_Length(temp_x,temp_y);
	tilt_of_gps(angle,direction,range,lon,lat,dh,dS);
	return osg::Vec3(lon,lat,dh);
}

osg::Vec3 RadProjTransform::DC2rvWorld( double x,double y )
{
	double temp_x =x/km;
	double temp_y = y/km;
	double direction,range,productAngle = 0.0;
	double lat,lon,dh,dS;   //���ȡ�γ�ȡ����θ߶ȡ�ˮƽ����
	direction = 180 *Get_Angle_Of_Direction(temp_x,temp_y) /PI;
	range = Get_Length(temp_x,temp_y);
	tilt_of_gps(productAngle,direction,range,lon,lat,dh,dS);
	return osg::Vec3(direction,range,dh);
}

double RadProjTransform::Get_Angle_Of_Direction( double dx, double dy )
{
	//if ( dx == 0 && dy>0 )
	//{
	//	return 0;
	//}
	if (dx == 0 && dy < 0)
	{
		return PI;
	}
	if ( dx>0 && dy==0)
	{
		return PI / 2.0;
	}
	if ( dx<0 && dy==0 )
	{
		return PI * 3.0 / 2.0;
	}
	if ( dx>0 && dy<0 )
	{
		return atan2(dx,dy);
	}
	if ( dx>0 && dy >0)
	{
		return PI - atan2(dx,-dy);
	}
	if ( dx<0 && dy<0 )
	{
		return 2*PI - atan2(-dx,dy);
	}
	if ( dx<0 && dy>0 )
	{
		return PI + atan2(-dx,-dy);
	}
	return 0;
}

double RadProjTransform::Get_Length( double dx, double dy )
{
	return( sqrt(dx*dx + dy*dy) );
}

bool RadProjTransform::setCenter( double lon,double lat )
{
	if (std::string::npos != strDestinationProjection.find("RADARPROJ"))
	{
		QStringList projstringlist ;
		projstringlist = QString(strDestinationProjection.c_str()).split(" ");
		float z = projstringlist[3].toDouble();
		Set(lon,lat,z);
		m_lat = lat;
		m_lon = lon;
		QString destproj = "RADARPROJ" ;
		destproj.append(" ");
		destproj.append(QString::number(lon));
		destproj.append(" ");
		destproj.append( QString::number(lat));
		destproj.append(" ");
		destproj.append(QString::number(z));
		//strSourceProjection = destproj.toStdString();
		mSourceProjection = " ";
		setSourceProjName((projstringlist[0]).toStdString());	
		setDestCRS(destproj.toStdString());
		return true;
	}
	else
	{
		if (Pro_map.contains(strDestinationProjection))
		{
			setSourceProjName(Pro_map.find(strDestinationProjection).value());	

			QString s_lat;
			QString s_lon;
			int iLat = -1, iLon = -1;
			if (0 == strDestinationProjection.size()) return false;
			QString tempDestination(strDestinationProjection.c_str());
			QStringList templist = tempDestination.split(" ");
			for(int i = 0; i < templist.size(); i++)
			{
				if(templist.at(i).contains("+lat_0"))
				{
					s_lat = templist.at(i);
					iLat = i;
				}
				if(templist.at(i).contains("+lon_0"))
				{
					s_lon = templist.at(i);
					iLon = i;
				}
			}

			QStringList latlist = s_lat.split("=");
			m_lat = lat;
			latlist[1] = QString::number(lat)+"n";

			QStringList lonlist = s_lon.split("=");
			lonlist[1] = QString::number(lon)+"e";
			m_lon =lon;

			templist[iLat] = latlist[0] + "=" +latlist[1];
			templist[iLon] = lonlist[0] + "=" + lonlist[1];
			tempDestination = templist.join(" ");

			setDestCRS(tempDestination.toStdString());
			return true;
		}
	}
	return false;
}




GOTO_NAMESPACE_END