#include <osg/BoundsChecking>
#include <osgGA/NodeTrackerManipulator>
#include <osgViewer/Viewer>
#include <cassert>
#include "DefaultManipulator.h"
#include "ManualDrawLine.h"
#include "ManualDrawRect.h"
#include "ObtainLongLatEventHandler.h"

int DefaultManipulator::_minimumDistanceFlagIndex = allocateRelativeFlag();
float DefaultManipulator::_offset = 500000;

/// Constructor.
DefaultManipulator::DefaultManipulator( int flags )
   : inherited( flags ),
     _distance( 1. ),
     _trackballSize( 0.8 ),
	 _DrawLine(new ManualDrawLine(0)),
	 _DrawRect(new ManualDrawRect(0))
{
    setMinimumDistance( 0.05, true );
    setWheelZoomFactor( 0.1 );
    if( _flags & SET_CENTER_ON_WHEEL_FORWARD_MOVEMENT )
        setAnimationTime( 0.2 );
	m_mType = _2D;
}


/// Constructor.
DefaultManipulator::DefaultManipulator( const DefaultManipulator& om, const osg::CopyOp& copyOp )
   : inherited( om, copyOp ),
     _center( om._center ),
     _rotation( om._rotation ),
     _distance( om._distance ),
     _trackballSize( om._trackballSize ),
     _wheelZoomFactor( om._wheelZoomFactor ),
     _minimumDistance( om._minimumDistance )
{
	m_mType = _2D;
	_LongLat = 0;
}


/** Set the position of the manipulator using a 4x4 matrix.*/
void DefaultManipulator::setByMatrix( const osg::Matrixd& matrix )
{
    _center = osg::Vec3d( 0., 0., -_distance ) * matrix;
    _rotation = matrix.getRotate();

    // fix current rotation
    if( getVerticalAxisFixed() )
        fixVerticalAxis( _center, _rotation, true );
}


/** Set the position of the manipulator using a 4x4 matrix.*/
void DefaultManipulator::setByInverseMatrix( const osg::Matrixd& matrix )
{
    setByMatrix( osg::Matrixd::inverse( matrix ) );
}


/** Get the position of the manipulator as 4x4 matrix.*/
osg::Matrixd DefaultManipulator::getMatrix() const
{
    return osg::Matrixd::translate( 0., 0., _distance ) *
           osg::Matrixd::rotate( _rotation ) *
           osg::Matrixd::translate( _center );
}


/** Get the position of the manipulator as a inverse matrix of the manipulator,
    typically used as a model view matrix.*/
osg::Matrixd DefaultManipulator::getInverseMatrix() const
{
    return osg::Matrixd::translate( -_center ) *
           osg::Matrixd::rotate( _rotation.inverse() ) * /*rotationUp **/
           osg::Matrixd::translate( 0.0, 0.0, -_distance );
}


// doc in parent
void DefaultManipulator::setTransformation( const osg::Vec3d& eye, const osg::Quat& rotation )
{
    _center = eye + rotation * osg::Vec3d( 0., 0., -_distance );
    _rotation = rotation;

    // fix current rotation
    if( getVerticalAxisFixed() )
        fixVerticalAxis( _center, _rotation, true );
}


// doc in parent
void DefaultManipulator::getTransformation( osg::Vec3d& eye, osg::Quat& rotation ) const
{
    eye = _center - _rotation * osg::Vec3d( 0., 0., -_distance );
    rotation = _rotation;
}


// doc in parent
void DefaultManipulator::setTransformation( const osg::Vec3d& eye, const osg::Vec3d& center, const osg::Vec3d& up )
{
    Vec3d lv( center - eye );

    Vec3d f( lv );
    f.normalize();
    Vec3d s( f^up );
    s.normalize();
    Vec3d u( s^f );
    u.normalize();

    osg::Matrixd rotation_matrix(  s[0], u[0], -f[0], 0.0f,
												s[1], u[1], -f[1], 0.0f,
												s[2], u[2], -f[2], 0.0f,
												0.0f, 0.0f,  0.0f, 1.0f );

    _center = center;
    _distance = lv.length();
    _rotation = rotation_matrix.getRotate().inverse();

    // fix current rotation
    if( getVerticalAxisFixed() )
        fixVerticalAxis( _center, _rotation, true );
}


// doc in parent
void DefaultManipulator::getTransformation( osg::Vec3d& eye, osg::Vec3d& center, osg::Vec3d& up ) const
{
    center = _center;
    eye = _center + _rotation * osg::Vec3d( 0., 0., _distance );
    up = _rotation * osg::Vec3d( 0., 1., 0. );
}


/** Sets the transformation by heading. Heading is given as an angle in radians giving a azimuth in xy plane.
    Its meaning is similar to longitude used in cartography and navigation.
    Positive number is going to the east direction.*/
void DefaultManipulator::setHeading( double azimuth ) 
{
    CoordinateFrame coordinateFrame = getCoordinateFrame( _center );
    Vec3d localUp = getUpVector( coordinateFrame );
    Vec3d localRight = getSideVector( coordinateFrame );

    Vec3d dir = Quat( getElevation(), localRight ) * Quat( azimuth, localUp ) * Vec3d( 0., -_distance, 0. );

    setTransformation( _center + dir, _center, localUp );
}


/// Returns the heading in radians. \sa setHeading
double DefaultManipulator::getHeading() const
{
    CoordinateFrame coordinateFrame = getCoordinateFrame( _center );
    Vec3d localFront = getFrontVector( coordinateFrame );
    Vec3d localRight = getSideVector( coordinateFrame );

    Vec3d center, eye, tmp;
    getTransformation( eye, center, tmp );

    Plane frontPlane( localFront, center );
    double frontDist = frontPlane.distance( eye );
    Plane rightPlane( localRight, center );
    double rightDist = rightPlane.distance( eye );

    return atan2( rightDist, -frontDist );
}


/** Sets the transformation by elevation. Elevation is given as an angle in radians from xy plane.
    Its meaning is similar to latitude used in cartography and navigation.
    Positive number is going to the north direction, negative to the south.*/
void DefaultManipulator::setElevation( double elevation )
{
    CoordinateFrame coordinateFrame = getCoordinateFrame( _center );
    Vec3d localUp = getUpVector( coordinateFrame );
    Vec3d localRight = getSideVector( coordinateFrame );

    Vec3d dir = Quat( -elevation, localRight ) * Quat( getHeading(), localUp ) * Vec3d( 0., -_distance, 0. );

    setTransformation( _center + dir, _center, localUp );
}


/// Returns the elevation in radians. \sa setElevation
double DefaultManipulator::getElevation() const
{
    CoordinateFrame coordinateFrame = getCoordinateFrame( _center );
    Vec3d localUp = getUpVector( coordinateFrame );
    localUp.normalize();

    Vec3d center, eye, tmp;
    getTransformation( eye, center, tmp );

    Plane plane( localUp, center );
    double dist = plane.distance( eye );

    return asin( -dist / (eye-center).length() );
}


// doc in parent
bool DefaultManipulator::handleMouseWheel( const GUIEventAdapter& ea, GUIActionAdapter& us )
{
    osgGA::GUIEventAdapter::ScrollingMotion sm = ea.getScrollingMotion();

    // handle centering
    if( _flags & SET_CENTER_ON_WHEEL_FORWARD_MOVEMENT )
    {

        if( ((sm == GUIEventAdapter::SCROLL_DOWN && _wheelZoomFactor > 0.)) ||
            ((sm == GUIEventAdapter::SCROLL_UP   && _wheelZoomFactor < 0.)) )
        {

            if( getAnimationTime() <= 0. )
            {
                // center by mouse intersection (no animation)
                setCenterByMousePointerIntersection( ea, us );
            }
            else
            {
                // start new animation only if there is no animation in progress
                if( !isAnimating() )
                    startAnimationByMousePointerIntersection( ea, us );

            }

        }
    }

    switch( sm )
    {
		double left, right, top, bottom, zNear, zFar;
        // mouse scroll up event
        case GUIEventAdapter::SCROLL_UP:
        {
            // perform zoom

			if(us.asView()->getCamera()->getProjectionMatrixAsOrtho(left, right, top, bottom, zNear, zFar))
			{
				left *= 1.1;
				right *= 1.1;
				top *= 1.1;
				bottom *= 1.1;
				us.asView()->getCamera()->setProjectionMatrixAsOrtho(left, right, top, bottom, zNear, zFar);
			}
			else
				zoomModel( _wheelZoomFactor, true );
            us.requestRedraw();
            us.requestContinuousUpdate( isAnimating() || _thrown );
            return true;
        }

        // mouse scroll down event
        case GUIEventAdapter::SCROLL_DOWN:
        {
            // perform zoom
			if(us.asView()->getCamera()->getProjectionMatrixAsOrtho(left, right, top, bottom, zNear, zFar))
			{
				left *= 0.9;
				right *= 0.9;
				top *= 0.9;
				bottom *= 0.9;
				us.asView()->getCamera()->setProjectionMatrixAsOrtho(left, right, top, bottom, zNear, zFar);
			}
			else
				zoomModel( -_wheelZoomFactor, true );
            us.requestRedraw();
            us.requestContinuousUpdate( false );
            return true;
        }

        // unhandled mouse scrolling motion
        default:
            return false;
   }
}


// doc in parent
bool DefaultManipulator::performMovementLeftMouseButton( const double eventTimeDelta, const double dx, const double dy )
{
    // rotate camera

	if (m_mType == _3D)
	{
		if( getVerticalAxisFixed() )
			rotateWithFixedVertical( dx, dy );
		else
			rotateTrackball( _ga_t0->getXnormalized(), _ga_t0->getYnormalized(),
			_ga_t1->getXnormalized(), _ga_t1->getYnormalized(),
			getThrowScale( eventTimeDelta ) );
	}
	else
	{
		float scale = -0.3f * _distance * getThrowScale( eventTimeDelta );
		panModel( dx*scale, dy*scale );
	}
   return true;
}


// doc in parent
bool DefaultManipulator::performMovementMiddleMouseButton( const double eventTimeDelta, const double dx, const double dy )
{
    // pan model
	float scale = -0.3f * _distance * getThrowScale( eventTimeDelta );
	panModel( dx*scale, dy*scale );
	return true;

}


// doc in parent
bool DefaultManipulator::performMovementRightMouseButton( const double eventTimeDelta, const double dx, const double dy )
{
    // zoom model
    zoomModel( dy * getThrowScale( eventTimeDelta ), true );
    return true;
}


bool DefaultManipulator::performMouseDeltaMovement( const float dx, const float dy )
{
    // rotate camera
    if( getVerticalAxisFixed() )
        rotateWithFixedVertical( dx, dy );
    else
        rotateTrackball( 0.f, 0.f, dx, dy, 1.f );

    return true;
}


void DefaultManipulator::applyAnimationStep( const double currentProgress, const double prevProgress )
{
    DefaultAnimationData *ad = dynamic_cast< DefaultAnimationData* >( _animationData.get() );
    assert( ad );

    // compute new center
    osg::Vec3d prevCenter, prevEye, prevUp;
    getTransformation( prevEye, prevCenter, prevUp );
    osg::Vec3d newCenter = osg::Vec3d(prevCenter) + (ad->_movement * (currentProgress - prevProgress));

    // fix vertical axis
    if( getVerticalAxisFixed() )
    {

        CoordinateFrame coordinateFrame = getCoordinateFrame( newCenter );
        Vec3d localUp = getUpVector( coordinateFrame );

        fixVerticalAxis( newCenter - prevEye, prevUp, prevUp, localUp, false );
   }

   // apply new transformation
   setTransformation( prevEye, newCenter, prevUp );
}


bool DefaultManipulator::startAnimationByMousePointerIntersection(
      const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& us )
{
    // get current transformation
    osg::Vec3d prevCenter, prevEye, prevUp;
    getTransformation( prevEye, prevCenter, prevUp );

    // center by mouse intersection
    if( !setCenterByMousePointerIntersection( ea, us ) )
        return false;

    DefaultAnimationData *ad = dynamic_cast< DefaultAnimationData*>( _animationData.get() );
    assert( ad );

    // setup animation data and restore original transformation
    ad->start( osg::Vec3d(_center) - prevCenter, ea.getTime() );
    setTransformation( prevEye, prevCenter, prevUp );

    return true;
}


void DefaultManipulator::DefaultAnimationData::start( const osg::Vec3d& movement, const double startTime )
{
    AnimationData::start( startTime );

    _movement = movement;
}


/** Performs trackball rotation based on two points given, for example,
    by mouse pointer on the screen.

    Scale parameter is useful, for example, when manipulator is thrown.
    It scales the amount of rotation based, for example, on the current frame time.*/
void DefaultManipulator::rotateTrackball( const float px0, const float py0,
                                        const float px1, const float py1, const float scale )
{
    osg::Vec3d axis;
    float angle;

    trackball( axis, angle, px1, py1, px0, py0 );

    Quat new_rotate;
    new_rotate.makeRotate( angle, axis );

    _rotation = _rotation * new_rotate;
}


/** Performs rotation horizontally by dx parameter and vertically by dy parameter,
    while keeping UP vector.*/
void DefaultManipulator::rotateWithFixedVertical( const float dx, const float dy )
{
    CoordinateFrame coordinateFrame = getCoordinateFrame( _center );
    Vec3d localUp = getUpVector( coordinateFrame );

    rotateYawPitch( _rotation, dx, dy, localUp );
}


/** Performs rotation horizontally by dx parameter and vertically by dy parameter,
    while keeping UP vector given by up parameter.*/
void DefaultManipulator::rotateWithFixedVertical( const float dx, const float dy, const Vec3f& up )
{
    rotateYawPitch( _rotation, dx, dy, up );
}


/** Moves camera in x,y,z directions given in camera local coordinates.*/
void DefaultManipulator::panModel( const float dx, const float dy, const float dz )
{
    Matrix rotation_matrix;
    rotation_matrix.makeRotate( _rotation );

    Vec3d dv( dx, dy, dz );

    _center += dv * rotation_matrix;
}


/** Changes the distance of camera to the focal center.
    If pushForwardIfNeeded is true and minimumDistance is reached,
    the focal center is moved forward. Otherwise, distance is limited
    to its minimum value.
    \sa DefaultManipulator::setMinimumDistance
 */
void DefaultManipulator::zoomModel( const float dy, bool pushForwardIfNeeded )
{
    // scale
    float scale = 1.0f + dy;

    // minimum distance
    float minDist = _minimumDistance;
    if( getRelativeFlag( _minimumDistanceFlagIndex ) )
        minDist *= _modelSize;

    if( _distance*scale > minDist )
    {
        // regular zoom
        _distance *= scale;
    }
    else
    {
        if( pushForwardIfNeeded )
        {
            // push the camera forward
            float scale = -_distance;
            Matrixd rotation_matrix( _rotation );
            Vec3d dv = (Vec3d( 0.0f, 0.0f, -1.0f ) * rotation_matrix) * (dy * scale);
            _center += dv;
        }
        else
        {
            // set distance on its minimum value
            _distance = minDist;
        }
    }
}


/**
 * Simulate a track-ball.  Project the points onto the virtual
 * trackball, then figure out the axis of rotation, which is the cross
 * product of P1 P2 and O P1 (O is the center of the ball, 0,0,0)
 * Note:  This is a deformed trackball-- is a trackball in the center,
 * but is deformed into a hyperbolic sheet of rotation away from the
 * center.  This particular function was chosen after trying out
 * several variations.
 *
 * It is assumed that the arguments to this routine are in the range
 * (-1.0 ... 1.0)
 */
void DefaultManipulator::trackball( osg::Vec3d& axis, float& angle, float p1x, float p1y, float p2x, float p2y )
{
    /*
        * First, figure out z-coordinates for projection of P1 and P2 to
        * deformed sphere
        */

    osg::Matrixd rotation_matrix(_rotation);

    osg::Vec3d uv = Vec3d(0.0f,1.0f,0.0f)*rotation_matrix;
    osg::Vec3d sv = Vec3d(1.0f,0.0f,0.0f)*rotation_matrix;
    osg::Vec3d lv = Vec3d(0.0f,0.0f,-1.0f)*rotation_matrix;

    osg::Vec3d p1 = sv * p1x + uv * p1y - lv * tb_project_to_sphere(_trackballSize, p1x, p1y);
    osg::Vec3d p2 = sv * p2x + uv * p2y - lv * tb_project_to_sphere(_trackballSize, p2x, p2y);

    /*
        *  Now, we want the cross product of P1 and P2
        */
    axis = p2^p1;
    axis.normalize();

    /*
        *  Figure out how much to rotate around that axis.
        */
    float t = (p2 - p1).length() / (2.0 * _trackballSize);

    /*
        * Avoid problems with out-of-control values...
        */
    if (t > 1.0) t = 1.0;
    if (t < -1.0) t = -1.0;
    angle = inRadians(asin(t));
}


/**
 * Helper trackball method that projects an x,y pair onto a sphere of radius r OR
 * a hyperbolic sheet if we are away from the center of the sphere.
 */
float DefaultManipulator::tb_project_to_sphere( float r, float x, float y )
{
    float d, t, z;

    d = sqrt(x*x + y*y);
                                 /* Inside sphere */
    if (d < r * 0.70710678118654752440)
    {
        z = sqrt(r*r - d*d);
    }                            /* On hyperbola */
    else
    {
        t = r / 1.41421356237309504880;
        z = t*t / d;
    }
    return z;
}


/** Get the FusionDistanceMode. Used by SceneView for setting up stereo convergence.*/
osgUtil::SceneView::FusionDistanceMode DefaultManipulator::getFusionDistanceMode() const
{
    return osgUtil::SceneView::USE_FUSION_DISTANCE_VALUE;
}

/** Get the FusionDistanceValue. Used by SceneView for setting up stereo convergence.*/
float DefaultManipulator::getFusionDistanceValue() const
{
    return _distance;
}


/** Set the center of the manipulator. */
void DefaultManipulator::setCenter( const Vec3d& center )
{
    _center = center;
}


/** Get the center of the manipulator. */
const Vec3d& DefaultManipulator::getCenter() const
{
    return _center;
}


/** Set the rotation of the manipulator. */
void DefaultManipulator::setRotation( const Quat& rotation )
{
    _rotation = rotation;
}


/** Get the rotation of the manipulator. */
const Quat& DefaultManipulator::getRotation() const
{
    return _rotation;
}


/** Set the distance of camera to the center. */
void DefaultManipulator::setDistance( double distance )
{
    _distance = distance;
}


/** Get the distance of the camera to the center. */
double DefaultManipulator::getDistance() const
{
    return _distance;
}


/** Set the size of the trackball. Value is relative to the model size. */
void DefaultManipulator::setTrackballSize( const double& size )
{
    /*
    * This size should really be based on the distance from the center of
    * rotation to the point on the object underneath the mouse.  That
    * point would then track the mouse as closely as possible.  This is a
    * simple example, though, so that is left as an Exercise for the
    * Programmer.
    */
    _trackballSize = size;
    clampBetweenRange( _trackballSize, 0.1, 1.0, "TrackballManipulator::setTrackballSize(float)" );
}


/** Set the mouse wheel zoom factor.
    The amount of camera movement on each mouse wheel event
    is computed as the current distance to the center multiplied by this factor.
    For example, value of 0.1 will short distance to center by 10% on each wheel up event.
    Use negative value for reverse mouse wheel direction.*/
void DefaultManipulator::setWheelZoomFactor( double wheelZoomFactor )
{
    _wheelZoomFactor = wheelZoomFactor;
}


/** Set the minimum distance of the eye point from the center
    before the center is pushed forward.*/
void DefaultManipulator::setMinimumDistance( const double& minimumDistance, bool relativeToModelSize )
{
    _minimumDistance = minimumDistance;
    setRelativeFlag( _minimumDistanceFlagIndex, relativeToModelSize );
}


/** Get the minimum distance of the eye point from the center
    before the center is pushed forward.*/
double DefaultManipulator::getMinimumDistance( bool *relativeToModelSize ) const
{
    if( relativeToModelSize )
        *relativeToModelSize = getRelativeFlag( _minimumDistanceFlagIndex );

    return _minimumDistance;
}

void DefaultManipulator::GoToNodeCenter( osg::Node* pN )
{
	osg::ref_ptr<osgGA::NodeTrackerManipulator> pNTM = new osgGA::NodeTrackerManipulator;
	pNTM->setTrackNode(pN);
	pNTM->setTrackerMode(NodeTrackerManipulator::NODE_CENTER_AND_ROTATION);
	osg::Matrixd newMatrixd = pNTM->getInverseMatrix();
	osg::Matrixd translatZ;
	translatZ.makeTranslate(0, 0, -1300000);
	//newMatrixd *= translatZ;
	this->setByInverseMatrix(newMatrixd);
}

void DefaultManipulator::GoToNodeCenter( osg::Camera* pCamera, osg::Node* pN, double dR )
{
	/*setAutoComputeHomePosition(false);
	setNode(pN);
	computeHomePosition(pCamera);
	home(0);*/
	if(!pCamera || !pN) return;
	osg::BoundingSphere boundingSphere = pN->getBound();

	if(dR >0) boundingSphere._radius = dR;
	/*double fovy, aspectRatio, zNear,  zFar;
	pCamera->getProjectionMatrixAsPerspective(fovy, aspectRatio, zNear, zFar);
	double dR = boundingSphere.radius();
	double dist = fabs(dR / tan(fovy/2));*/
	double left, right, bottom, top, zNear, zFar;
	double dist;
	if(pCamera->getProjectionMatrixAsFrustum(left,right,bottom,top,zNear,zFar))
	{
		double vertical2 = fabs(right - left)/* / zNear*/ / 2.;
		double horizontal2 = fabs(top - bottom)/* / zNear *// 2.;
		double dim = horizontal2 < vertical2 ? horizontal2 : vertical2;
		double viewAngle = atan2(dim,1.);
		dist = boundingSphere.radius() / sin(viewAngle);
		osg::Vec3d vec3Zero(0,0,0);
		osg::Vec3d eye, center, up;

		eye.set(boundingSphere.center().x(), boundingSphere.center().z(), boundingSphere.center().y());
		center = eye;
	
		setHomePosition(eye + osg::Vec3d(0.0, -dist,0.0f), center, osg::Vec3d(0.0f,0.0f,1.0f), false);
		home(0);
	}
	else
	{
#if 0
		pCamera->getProjectionMatrixAsOrtho(left, right,bottom, top, zNear, zFar);
		double vertical2 = fabs(right - left);
		double horizontal2 = fabs(top - bottom);
		//double dscale = vertical2/horizontal2;
		double dim = horizontal2 < vertical2 ? horizontal2 : vertical2;
		double dscale = dim/(boundingSphere.radius() * 2);
		pCamera->setProjectionMatrixAsOrtho(left / dscale, right / dscale, bottom / dscale, top / dscale, zNear, zFar);
		dist = fabs(zFar - zNear) / 2;
#else
		pCamera->getProjectionMatrixAsOrtho(left, right,bottom, top, zNear, zFar);
		float fR = boundingSphere.radius();
		float fX = boundingSphere.center().x();
		float fY = boundingSphere.center().z();
		pCamera->setProjectionMatrixAsOrtho(fX - fR, fX + fR, fY - fR, fY + fR, zNear, zFar);
		dist = fabs(zFar - zNear) / 2;

		osg::Vec3d eye, center, up;
		eye.set(boundingSphere.center().x(), boundingSphere.center().y(), boundingSphere.center().z());
		center = eye;
		setHomePosition(eye + osg::Vec3d(0.0, -dist-100000,0.0f), center, osg::Vec3d(0.0f,0.0f,1.0f), false);
		home(0);
#endif
	}
	
}

bool DefaultManipulator::handle( const osgGA::GUIEventAdapter& ea,osgGA::GUIActionAdapter& aa )
{
#if 1
	static bool bPress = false;
	static bool bRectPress = false;
	osg::Camera* pC = 0;
	osg::View* pOsgView = aa.asView();
	osgViewer::Viewer* pViewer = dynamic_cast<osgViewer::Viewer*>(pOsgView);
	osg::Group* pSceneData = 0;
	
	if(pViewer)
	{
		pViewer->updateTraversal();
		pC = pViewer->getCamera();
		pSceneData = dynamic_cast<osg::Group*>(pViewer->getSceneData());
	}
	else
		return false;

	ScreenToWorld(pC,osg::Vec3(ea.getX(),ea.getY(),0),_currentPosition);

	switch(ea.getEventType())
	{
	case GUIEventAdapter::KEYDOWN:
		{
			int key  = ea.getKey();
			if( ea.getKey() == 65507)
			{
				
				bRectPress = true;
			}
			else if(ea.getKey() == 82)
			{
				if(_LongLat)
					_LongLat->handle(ea, aa);
			}else if (ea.getKey() == 65513 )
			{
				bPress = true;
			}

		}
		break;
	case GUIEventAdapter::KEYUP:
		{
			bPress = false;
			bRectPress = false;
			/*if(ea.getKey() == 0xffffffee)
			{

			}*/
		}
		break;
	default:
		if(bPress)
		{
			if(pSceneData)
				pSceneData->addChild(_DrawLine->GetDrawGeode());
			//_DrawLine->setZValue(_homeEye.y() - getDistance());
			return _DrawLine->handle(ea, aa);
		}else 
		if (bRectPress)
		{
			if(pSceneData)
				pSceneData->addChild(_DrawRect->GetDrawGeode());
			//_DrawLine->setZValue(_homeEye.y() - getDistance());
			return _DrawRect->handle(ea, aa);
		}
		else
			return osgGA::StandardManipulator::handle(ea, aa);
	}

	return osgGA::StandardManipulator::handle(ea, aa);
#else
	return osgGA::StandardManipulator::handle(ea, aa);
#endif
}

void DefaultManipulator::ScreenToWorld( osg::Camera* pCamera, const osg::Vec3& v3In, osg::Vec3& v3Out )
{
	osg::Matrix VPW = pCamera->getViewMatrix() *
		pCamera->getProjectionMatrix() *
		pCamera->getViewport()->computeWindowMatrix();
	osg::Matrix inverseVPW;
	inverseVPW.invert(VPW);
	v3Out = v3In  * inverseVPW;

	v3Out.set(v3Out.x(),v3Out.z(),v3Out.y());
	//v3Out = m_pCoord->Transform(v3Out);

}

void DefaultManipulator::WorldToScreen( osg::Camera* pCamera, const osg::Vec3& v3In, osg::Vec3& v3Out )
{
	osg::Vec3 v3localIn;
	v3localIn.set(v3In.x(), v3In.z(), v3In.y());
	osg::Matrix VPW = pCamera->getViewMatrix() * 
		pCamera->getProjectionMatrix() *
		pCamera->getViewport()->computeWindowMatrix();

	v3Out = v3localIn * VPW;
	v3Out.set(v3Out.x(), v3Out.y(), 1);

}

//void DefaultManipulator::CoordTrans( CoordinateTransform* coord )
//{
//	m_pCoord->setSourceCRS(coord->getDesCRS());
//	m_pCoord->setSourceProjName(coord->getDesProjName());
//	m_pCoord->setDestCRS(coord->getSourceCRS());
//	m_pCoord->setDesProjName(coord->getSourceProjName());
//
//}

osg::Vec3d DefaultManipulator::GetCurrentPosition()
{
	return _currentPosition;
}

void DefaultManipulator::MapMZoomIn()
{
	double dist = getDistance();
	osg::Vec3d vec3Zero(0,0,0);
	osg::Vec3d eye, center, up;
	eye.set(_center.x(), _center.y()+_offset, _center.z());
	center = eye;
	setHomePosition(eye - osg::Vec3d(0.0, dist,0.0f), center, osg::Vec3d(0.0f,0.0f,1.0f), false);
	home(0);
}
void DefaultManipulator::MapMZoomOut()
{
	double dist = getDistance();
	osg::Vec3d vec3Zero(0,0,0);
	osg::Vec3d eye, center, up;
	eye.set(_center.x(), _center.y() - _offset, _center.z());
	center = eye;
	setHomePosition(eye - osg::Vec3d(0.0, dist,0.0f), center, osg::Vec3d(0.0f,0.0f,1.0f), false);
	home(0);
}

void DefaultManipulator::MapZoomIn()
{

}

void DefaultManipulator::MapZoomOut()
{

}

void DefaultManipulator::MapTop()
{
	double dist = getDistance();
	osg::Vec3d vec3Zero(0,0,0);
	osg::Vec3d eye, center, up;
	eye.set(_center.x() , _center.y(), _center.z()- _offset);
	center = eye;
	setHomePosition(eye - osg::Vec3d(0.0, dist,0.0f), center, osg::Vec3d(0.0f,0.0f,1.0f), false);
	home(0);
}

void DefaultManipulator::MapBottom()
{
	double dist = getDistance();
	osg::Vec3d vec3Zero(0,0,0);
	osg::Vec3d eye, center, up;
	eye.set(_center.x() , _center.y(), _center.z()+ _offset);
	center = eye;
	setHomePosition(eye - osg::Vec3d(0.0, dist,0.0f), center, osg::Vec3d(0.0f,0.0f,1.0f), false);
	home(0);
}

void DefaultManipulator::MapLeft()
{
	double dist = getDistance();
	osg::Vec3d vec3Zero(0,0,0);
	osg::Vec3d eye, center, up;
	eye.set(_center.x() + _offset, _center.y(), _center.z());
	center = eye;
	setHomePosition(eye - osg::Vec3d(0.0, dist,0.0f), center, osg::Vec3d(0.0f,0.0f,1.0f), false);
	home(0);
}

void DefaultManipulator::MapRight()
{
	double dist = getDistance();
	osg::Vec3d vec3Zero(0,0,0);
	osg::Vec3d eye, center, up;
	eye.set(_center.x() - _offset, _center.y(), _center.z());
	center = eye;
	setHomePosition(eye - osg::Vec3d(0.0, dist,0.0f), center, osg::Vec3d(0.0f,0.0f,1.0f), false);
	home(0);
}

void DefaultManipulator::MapALLMap()
{
	osg::Node* node = this->getNode();
	double dist = getDistance();
	osg::Vec3d vec3Zero(0,0,0);
	osg::Vec3d eye, center, up;
	center = node->getBound().center();
	eye.set(center.x(), center.y(), center.z());
	center = eye;
	setHomePosition(eye - osg::Vec3d(0.0, dist,0.0f), center, osg::Vec3d(0.0f,0.0f,1.0f), true);
	home(0);
}
