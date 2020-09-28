#include "Proj4Transform.h"
#include <QtCore/QStringList>

GOTO_NAMESPACE_START

Proj4Transform::Proj4Transform(void) : mSourceProjection(0), mDestinationProjection(0)
{
	m_TransType = TRANSFORM_2D;
}

Proj4Transform::~Proj4Transform(void)
{
	// free the proj objects
	if ( mSourceProjection != 0 )
	{
		pj_free( mSourceProjection );
	}
	if ( mDestinationProjection != 0 )
	{
		pj_free( mDestinationProjection );
	}
}

void Proj4Transform::setSourceCRS (std::string ProjString)
{
	strSourceProjection = ProjString;

	setSourceProjName(std::string("WGS84"));	
	projPJ pj = pj_init_plus(ProjString.c_str());
	mSourceProjection = pj;
	if ( mSourceProjection == NULL )
	{
		//qDebug("Set mSourceProjection fail");
	}
}

void Proj4Transform::setDestCRS (std::string ProjString)
{
	strDestinationProjection = ProjString;

	/*if(Pro_map.find(ProjString))
	{
	setDesProjName(Pro_map.find(ProjString).value());
	}*/

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


	mDestinationProjection = pj_init_plus(ProjString.c_str());
	if ( mDestinationProjection == NULL )
	{
		//qDebug("Set mDestinationProjection fail");
	}
}

osg::Vec3 Proj4Transform::Transform(const osg::Vec3 vertex)
{

	if ( mSourceProjection == mDestinationProjection) return vertex;

	x = vertex.x();
	z = vertex.y();
	y = vertex.z();

	transformCoords( 1, &x, &y, &z );

	return osg::Vec3(x, y, z);

}

osg::Vec3 Proj4Transform::Transform(double gLx, double gLy, double gLz)
{
	return Transform( osg::Vec3( (float)gLx, (float)gLy, (float)gLz ) );
}

//QRectF Proj4Transform::TransformBoundingBox(const QRectF rectangle, TRANSFORM_DIRECTION direction)
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
//
//}

int Proj4Transform::transformCoords( const int& numPoints, double *x, double *y, double *z)
{
	if ( mDestinationProjection == NULL )
	{
		//qDebug("DestinationProjection NULL");
		return 0;
	}
	if ( mSourceProjection == NULL )
	{
		//qDebug("SourceProjection NULL");
		return 0;
	}

	// use proj4 to do the transform
	// if the source/destination projection is lat/long, convert the points to radians prior to transforming
	if (pj_is_latlong( mSourceProjection ) )
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
		std::ostringstream ostrStream;
		//something bad happened....
		for ( int i = 0; i < numPoints; ++i )
		{
			ostrStream.clear();
			ostrStream<<"("<<x[i]<<", "<<y[i]<<")\n";
			points = ostrStream.str();
			
			ostrStream.clear();
			ostrStream<<x[i]<<"of "<<y[i]<<" failed with error: "<<pj_strerrno( projResult )<<'\n';
			msg = ostrStream.str();
			//qDebug("pj_transform fail");
			//qDebug()<<msg;
		}
		return 0;
	}
	// if the result is lat/long, convert the results from radians back to degrees
	if (pj_is_latlong( mDestinationProjection)
		|| pj_is_latlong( mSourceProjection ))
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

int Proj4Transform::Transform( const osg::Vec3& src, osg::Vec3& dst )
{
	int projResult = 0;
	double dX, dY, dZ;

	if ( mDestinationProjection == NULL || mSourceProjection == NULL)
	{
		return projResult;
	}
	dX = src.x() * DEG_TO_RAD;
	dY = src.y() * DEG_TO_RAD;
	dZ = src.z() * DEG_TO_RAD;
	projResult = pj_transform(mSourceProjection, mDestinationProjection, 1, 0, &dX, &dY, &dZ);
	dst.set(dX, src.z(), dY);

	return projResult;
}

std::string Proj4Transform::ImportFromESRI(const char *  pszFname )
{
#if 0
	OGRSpatialReference projection;

	char **papszPrj = CSLLoad(pszFname); 

	OGRErr eErr = projection.importFromESRI(papszPrj); 

	CSLDestroy( papszPrj );
	char *destproj =NULL;
	projection.exportToProj4(&destproj);
	std::string temp ;	
	temp = destproj;
	return temp.trimmed();
#else
	std::string str;
	return str;
#endif
}

std::string Proj4Transform::getDesCRS()
{
	return strDestinationProjection;
}

std::string Proj4Transform::getSourceCRS()
{
	return strSourceProjection;
}

 std::string Proj4Transform::getDesProjName()
 {
 	return strDestinationProName;
 }
 
 std::string Proj4Transform::getSourceProjName()
 {
 	return strSourceProName;
 }

 void Proj4Transform::setSourceProjName(std::string ProjString)
 {
	 strSourceProName = ProjString;
 }

 void Proj4Transform::setDesProjName(std::string ProjString)
 {
	 strDestinationProName = ProjString;
 }

 bool Proj4Transform::setCenter( double lon,double lat  )
 {
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
	 m_lon =lon;
	 latlist[1] = QString::number(lat)+"n";

	 QStringList lonlist = s_lon.split("=");
	 lonlist[1] = QString::number(lon)+"e";
	 

	 templist[iLat] = latlist[0] + "=" +latlist[1];
	 templist[iLon] = lonlist[0] + "=" + lonlist[1];
	 tempDestination = templist.join(" ");

	 setDestCRS(tempDestination.toStdString());
	 return true;

 }


GOTO_NAMESPACE_END