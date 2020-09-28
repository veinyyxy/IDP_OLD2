#pragma once

#include <osgGA/StandardManipulator>
#include "GraphicsControler_global.h"


using namespace osg;
using namespace osgGA;

class ManualDrawLine;
class ManualDrawRect;
class ObtainLongLatEventHandler;



class GRAPHICSCONTROLER_EXPORT DefaultManipulator :public osgGA::StandardManipulator
{
public:
	enum MType
	{
		_2D,
		_3D
	};
	typedef StandardManipulator inherited;

public:
	void GoToNodeCenter(osg::Camera* pCamera, osg::Node* pN, double dR = -1);
	void GoToNodeCenter(osg::Node* pN);
	DefaultManipulator( int flags = DEFAULT_SETTINGS );
	DefaultManipulator( const DefaultManipulator& om,
		const osg::CopyOp& copyOp = osg::CopyOp::SHALLOW_COPY );

	inline void DefaultMType(MType mType) {m_mType = mType;}
	inline MType DefaultMType() {return m_mType;}

	virtual void setByMatrix( const osg::Matrixd& matrix );
	virtual void setByInverseMatrix( const osg::Matrixd& matrix );
	virtual osg::Matrixd getMatrix() const;
	virtual osg::Matrixd getInverseMatrix() const;

	virtual void setTransformation( const osg::Vec3d& eye, const osg::Quat& rotation );
	virtual void setTransformation( const osg::Vec3d& eye, const osg::Vec3d& center, const osg::Vec3d& up );
	virtual void getTransformation( osg::Vec3d& eye, osg::Quat& rotation ) const;
	virtual void getTransformation( osg::Vec3d& eye, osg::Vec3d& center, osg::Vec3d& up ) const;

	void setHeading( double azimuth );
	double getHeading() const;
	void setElevation( double elevation );
	double getElevation() const;

	virtual void setCenter( const osg::Vec3d& center );
	const osg::Vec3d& getCenter() const;
	virtual void setRotation( const osg::Quat& rotation );
	const osg::Quat& getRotation() const;
	virtual void setDistance( double distance );
	double getDistance() const;

	virtual void setTrackballSize( const double& size );
	inline double getTrackballSize() const;
	virtual void setWheelZoomFactor( double wheelZoomFactor );
	inline double getWheelZoomFactor() const;

	virtual void setMinimumDistance( const double& minimumDistance, bool relativeToModelSize = NULL );
	double getMinimumDistance( bool *relativeToModelSize = NULL ) const;

	virtual osgUtil::SceneView::FusionDistanceMode getFusionDistanceMode() const;
	virtual float getFusionDistanceValue() const;
	bool handle(const osgGA::GUIEventAdapter& ea,osgGA::GUIActionAdapter& aa);

	inline ManualDrawLine *GetManualDrawLine() {return _DrawLine;};
	inline ManualDrawRect *GetManualDrawRect() {return _DrawRect;};
	/** @brief 设置投影变换 */
	//void CoordTrans(CoordinateTransform* coord);
	/** @brief 屏幕坐标与世界坐标互转 */ 
	void ScreenToWorld( osg::Camera* pCamera, const osg::Vec3& v3In, osg::Vec3& v3Out);
	void WorldToScreen( osg::Camera* pCamera, const osg::Vec3& v3In, osg::Vec3& v3Out);
	osg::Vec3d GetCurrentPosition();

	void MapALLMap();
	//按比例
	void MapMZoomIn();
	void MapMZoomOut();

	void MapZoomIn();
	void MapZoomOut();

	void MapTop();
	void MapBottom();
	void MapLeft();
	void MapRight();
protected:

	virtual bool handleMouseWheel( const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& us );

	virtual bool performMovementLeftMouseButton( const double eventTimeDelta, const double dx, const double dy );
	virtual bool performMovementMiddleMouseButton( const double eventTimeDelta, const double dx, const double dy );
	virtual bool performMovementRightMouseButton( const double eventTimeDelta, const double dx, const double dy );
	virtual bool performMouseDeltaMovement( const float dx, const float dy );
	virtual void applyAnimationStep( const double currentProgress, const double prevProgress );

	virtual void rotateTrackball( const float px0, const float py0,
		const float px1, const float py1, const float scale );
	virtual void rotateWithFixedVertical( const float dx, const float dy );
	virtual void rotateWithFixedVertical( const float dx, const float dy, const osg::Vec3f& up );
	virtual void panModel( const float dx, const float dy, const float dz = 0.f );
	virtual void zoomModel( const float dy, bool pushForwardIfNeeded = true );
	void trackball( osg::Vec3d& axis, float& angle, float p1x, float p1y, float p2x, float p2y );
	float tb_project_to_sphere( float r, float x, float y );
	virtual bool startAnimationByMousePointerIntersection( const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& us );

	osg::Vec3d _center;
	osg::Vec3 _currentPosition; //屏幕坐标
	osg::Quat  _rotation;
	double     _distance;

	double _trackballSize;
	double _wheelZoomFactor;

	double _minimumDistance;
	static int _minimumDistanceFlagIndex;
	
	MType m_mType;

	class DefaultAnimationData : public AnimationData {
	public:
		osg::Vec3d _movement;
		void start( const osg::Vec3d& movement, const double startTime );
	};
	virtual void allocAnimationData() { _animationData = new DefaultAnimationData(); }
	osg::Geometry* pickGeometry( osgViewer::View* view, const osgGA::GUIEventAdapter& ea );
	ManualDrawLine* _DrawLine;
	ManualDrawRect* _DrawRect;
	ObtainLongLatEventHandler* _LongLat;
	static float _offset;
};

