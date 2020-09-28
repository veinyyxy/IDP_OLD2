#include <QMap>
#include <QFileInfo>
#include <QImageReader>
#include <QPainter>
#include "CipasComposition.h"

#include "CipasComposerMap.h"
#include "CipasComposerGLW.h"

typedef QMap<int,QImage> QIMAGEMAP;
static QIMAGEMAP QImagelist;

#if 0
bool AttachImageCamera(osgViewer::View* pViewer, osg::Node* pParentNode, osg::Node* pMakeToImageNode, osg::Image* outImage, int tex_width, int tex_height)
{
#if 0
	osg::Group* pParentGroup = dynamic_cast<osg::Group*>(pParentNode);
	osg::Group* pImageGroup = dynamic_cast<osg::Group*>(pMakeToImageNode);

	osg::Camera* camera = new osg::Camera;

	// set up the background color and clear mask.
	camera->setClearColor(osg::Vec4(0.1f,0.1f,0.3f,1.0f));
	camera->setClearMask(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	const osg::BoundingSphere& bs = pMakeToImageNode->getBound();
	if (!bs.valid())
	{
		return false;
	}

	float znear = 1.0f*bs.radius();
	float zfar  = 3.0f*bs.radius();

	// 2:1 aspect ratio as per flag geometry below.
	float proj_top   = 0.25f*znear;
	float proj_right = 0.5f*znear;

	znear *= 0.9f;
	zfar *= 1.1f;

	// set up projection.
	camera->setProjectionMatrixAsFrustum(-proj_right,proj_right,-proj_top,proj_top,znear,zfar);

	// set view
	camera->setReferenceFrame(osg::Transform::ABSOLUTE_RF);
	camera->setViewMatrixAsLookAt(bs.center()-osg::Vec3(0.0f,2.0f,0.0f)*bs.radius(),bs.center(),osg::Vec3(0.0f,0.0f,1.0f));

	// set viewport
	camera->setViewport(0,0,tex_width,tex_height);

	// set the camera to render before the main camera.
	camera->setRenderOrder(osg::Camera::PRE_RENDER);

	// tell the camera to use OpenGL frame buffer object where supported.
	camera->setRenderTargetImplementation(osg::Camera::FRAME_BUFFER_OBJECT);


	//osg::Image* image = new osg::Image;
	outImage->allocateImage(tex_width, tex_height, 1, GL_RGBA, GL_FLOAT);

	// attach the image so its copied on each frame.
	camera->attach(osg::Camera::COLOR_BUFFER, outImage);

	camera->addChild(pImageGroup);
	pParentGroup->addChild(camera);

	camera->setFinalDrawCallback(new SaveImageDrawCallback(outImage));
#else
	osg::Group* pParentGroup = dynamic_cast<osg::Group*>(pParentNode);

	osg::ref_ptr<osg::Camera> camera = new osg::Camera;
	camera->setClearColor(osg::Vec4(1, 1, 1, 1));
	camera->setClearMask( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	camera->setReferenceFrame( osg::Transform::ABSOLUTE_RF );
	camera->setRenderOrder( osg::Camera::PRE_RENDER );
	camera->setRenderTargetImplementation(osg::Camera::FRAME_BUFFER_OBJECT);
	camera->setViewport( 0, 0, 800, 600);
	camera->addChild(pMakeToImageNode);

	// Set the printer
	PosterPrinter* printer = new PosterPrinter;
	printer->setTileSize( 800, 600 );
	printer->setPosterSize( 800, 600 );
	printer->setCamera( camera.get() );

	osg::Image* posterImage = 0;
	posterImage = new osg::Image;
	posterImage->allocateImage( 800, 600, 1, GL_RGBA, GL_UNSIGNED_BYTE );
	printer->setFinalPoster( posterImage);
	printer->setOutputPosterName("000000.png");

	pViewer->addEventHandler( new PrintPosterHandler(printer) );

	pParentGroup->addChild(camera);
	return true;
#endif
}
#endif

CCipasComposerGLW::CCipasComposerGLW(CCipasComposition *composition): CComposerItem( composition ,true)
	, m_pViewerImage(0)
{
	mImage.load(IDPApplication::TempPath()+"Thematic.png");
	mPictureWidth = rect().width();
	mGridEnabled = false;
	mGridAnnotationPrecision = 0;
	/**True if coordinate values should be drawn*/
	mShowGridAnnotation = false;
	/**Annotation can be horizontal / vertical or different for axes*/
	mGridAnnotationDirection = Horizontal;
	/**Solid or crosses*/
	mGridStyle = Solid;
	/**Grid line interval in x-direction (map units)*/
	mGridIntervalX = 10.0;
	/**The length of the cross sides for mGridStyle Cross*/
	/**Grid line interval in y-direction (map units)*/
	mGridIntervalY = 10.0;
	mCrossLength = 0;
	/**Grid line offset in x-direction*/
	mGridOffsetX = 0;
	/**Grid line offset in y-direction*/
	mGridOffsetY = 0;
	/**Font for grid line annotation*/
	mGridAnnotationFont;
	/**Annotation position inside or outside of map frame*/
	mGridAnnotationPosition = OutsideMapFrame;
	/**Distance between map frame and annotation*/
	mAnnotationFrameDistance = 1;
	/*label axes switch*/
	LabelAxesTop=false;
	LabelAxesLeft=true;
	LabelAxesBottom=true;
	LabelAxesRight=false;
	mXOffset = 0;
	mYOffset = 0;
	mDrawing = false;
	mComposition = composition;
	mAnnotionStyle=AnnotationStyle::NoStyle;
	
	//mId = mComposition->composerMapItems().size();
	int maxId = -1;
	QList<const CCipasComposerGLW*> mapList = mComposition->composerGLWItems();
	QList<const CCipasComposerGLW*>::const_iterator mapIt = mapList.constBegin();
	for ( ; mapIt != mapList.constEnd(); ++mapIt )
	{
		if (( *mapIt )->id() > maxId )
		{
			maxId = ( *mapIt )->id();
		}
	}
	mId = maxId + 1;
	//mGridPen.setStyle(Qt::DashLine);
	mGridPosition = InsideMapFrame;
	mDisToFrame = 3;
	miGraduationLineLength = 2;
	isAdjustExtent=false;
	mReCalcGrid=true;
	isFirstCalcGrid=true;
}

CCipasComposerGLW::CCipasComposerGLW(qreal x, qreal y, qreal width, qreal height,CCipasComposition *composition)
	: CComposerItem( x, y, width, height,composition )
	, m_pViewerImage(0)
{
	mImage.load(IDPApplication::TempPath()+"Thematic.png");
	setSceneRect( QRectF( x, y, width, height) );
	mGLWidth = width;
	mGLHeight= height;

	mPictureWidth = rect().width();
	mComposition = composition;
	//mId = mComposition->composerMapItems().size();
	int maxId = -1;
	QList<const CCipasComposerGLW*> mapList = mComposition->composerGLWItems();
	QList<const CCipasComposerGLW*>::const_iterator mapIt = mapList.constBegin();
	for ( ; mapIt != mapList.constEnd(); ++mapIt )
	{
		if (( *mapIt )->id() > maxId )
		{
			maxId = ( *mapIt )->id();
		}
	}
	mId = maxId + 1;

	mGridEnabled = false;
	mGridAnnotationPrecision = 0;
	/**True if coordinate values should be drawn*/
	mShowGridAnnotation = false;
	/**Annotation can be horizontal / vertical or different for axes*/
	mGridAnnotationDirection = Horizontal;
	/**Solid or crosses*/
	mGridStyle = Solid;
	/**Grid line interval in x-direction (map units)*/
	mGridIntervalX = 10.0;
	/**The length of the cross sides for mGridStyle Cross*/
	/**Grid line interval in y-direction (map units)*/
	mGridIntervalY = 10.0;
	mCrossLength = 0;
	/**Grid line offset in x-direction*/
	mGridOffsetX = 0;
	/**Grid line offset in y-direction*/
	mGridOffsetY = 0;
	/**Font for grid line annotation*/
	mGridAnnotationFont;
	/**Annotation position inside or outside of map frame*/
	mGridAnnotationPosition = OutsideMapFrame;
	/**Distance between map frame and annotation*/
	mAnnotationFrameDistance = 1;
	mXOffset = 0;
	mYOffset = 0;
	mDrawing = false;
	//mGridPen.setStyle(Qt::DashLine);
	/*mExtent.setXMinimum(0);
	mExtent.setXMaximum(10);
	mExtent.setYMinimum(0);
	mExtent.setYMaximum(10);*/
	/*label axes switch*/
	LabelAxesTop=false;
	LabelAxesLeft=true;
	LabelAxesBottom=true;
	LabelAxesRight=false;
	mGridPosition = InsideMapFrame;
	mDisToFrame = 3;
	miGraduationLineLength = 2;
	isAdjustExtent=false;
}

CCipasComposerGLW::CCipasComposerGLW( ): CComposerItem( 0)
{
	mPictureHeight = rect().height();
}

CCipasComposerGLW::~CCipasComposerGLW(void)
{
	/*QGraticuleCoordinates* pGraCoord = dynamic_cast<QGraticuleCoordinates*>(mpCurrentMap->GetLayer(tr("网格地图")));
	if(pGraCoord) pGraCoord->setColor(QVector4D(1.0, 1.0, 1.0, 0));*/
}

void CCipasComposerGLW::setAdjustExtent(bool b)
{
	isAdjustExtent=b;
}

void CCipasComposerGLW::setInputExtent(double xmin /* = 0 */, double ymin /* = 0 */, double xmax /* = 0 */, double ymax /* = 0  */,ImageKeepRadio b/* =Keep */ )
{
	mLeftTop.setX(xmin);
	mLeftTop.setY(ymax);
	mRightBottom.setX(xmax);
	mRightBottom.setY(ymin);
}

void CCipasComposerGLW::setImage(QImage *img)
{
	mImage=*img;	

	if (!QImagelist.contains(mId))
	{
		QImagelist.insert(mId,mImage);
	}
}

void CCipasComposerGLW::updateImage(QImage *img)
{
	mImage=*img;	
	mImage.load(IDPApplication::TempPath()+"Thematic.png");
	if (!QImagelist.contains(mId))
	{
		QImagelist.insert(mId,mImage);
	}
	else
	{
		QImagelist.remove(mId);
		QImagelist.insert(mId,mImage);			
	}
	cache();
}

void CCipasComposerGLW::setGLExtent2( double xmin , double ymin , double xmax , double ymax ,ImageKeepRadio b)
{
	//设置新的经纬度四至范围
	mExtent.set(xmin,ymin,xmax,ymax);
	QRectF lenRect;
	qreal x, y, width, height, qrResult, camraX, camraY, cameraWidth, cameraHeight, cameraResult, qrScale;
}


void CCipasComposerGLW::setGLExtent( double xmin /* = 0 */, double ymin /* = 0 */, double xmax /* = 0 */, double ymax /* = 0 */ )
{
	mExtent.set(xmin,ymin,xmax,ymax);
	this->update();
}

void CCipasComposerGLW::setOffset( double xOffset, double yOffset )
{
	mXOffset = xOffset;
	mYOffset = yOffset;
}

void CCipasComposerGLW::setReCalcGrid(bool b)
{
	mReCalcGrid=b;
	if (b)
	{
		mReCalcGrid=false;
	}
}

void CCipasComposerGLW::moveContent( double dx, double dy )
{
	//	//更新地图范围
	emit itemChanged();
	update();
	emit extentChanged();
}
void CCipasComposerGLW::cache()
{
	updateBoundingRect();
	QGraphicsRectItem::update();
	emit itemChanged();
}

void CCipasComposerGLW::paint( QPainter* painter, const QStyleOptionGraphicsItem* itemStyle, QWidget* pWidget )
{
	/*是否根据输入的经纬度范围调整视口*/
	if (isAdjustExtent)	
	{
		paintAdjustExtent(painter,itemStyle,pWidget);
	} 
	else
	{
		paintCommon(painter,itemStyle,pWidget);
	}
}

void CCipasComposerGLW::SetGLWImg()
{
	QRectF ItemRect = rect();
	double dWidth = ItemRect.width();
	double dHeight = ItemRect.height();
	//int newDpi = ( painter->device()->logicalDpiX() + painter->device()->logicalDpiY() ) / 2;
	int newDpi=mComposition->printResolution();

	double rectPixelWidth = dWidth * newDpi / 25.4;
	double rectPixelHeight =  dHeight * newDpi / 25.4;
	QRectF boundRect;


	QVector3D screenPoint;
	//////////////////////////////////////////////////////////////////////////////////////

	QVector3D leftTop, rightBottom;	

	//m_pViewerImage->setUpdatesEnabled(true);
	//m_pViewerImage->setVisible(true);

	emit myImages(rectPixelWidth, rectPixelHeight);

	//int iF = 10000000;
	//for(int i = 0; i < iF; i++)
		//m_pViewerImage->update();
	
	mImage.load(IDPApplication::TempPath()+"Thematic.png");
	
	//if (mpCurrentMap != NULL)
	{
		

		if(leftTop.x() > 0 && rightBottom.x() < 0)
		{
			rightBottom.setX(rightBottom.x()  + 360);
		}

		//double dImageWidth = mImage.width();
		//double dImageHeight = mImage.height();

		boundRect = boundedImageRect( rectPixelWidth, rectPixelHeight );
		boundRectWidthMM = boundRect.width() / newDpi * 25.4;
		boundRectHeightMM = boundRect.height() / newDpi * 25.4;

		mExtent.setXMinimum(leftTop.x());
		mExtent.setXMaximum(mImage.width()/*boundRectWidthMM*/);
		mExtent.setYMinimum(rightBottom.y());
		mExtent.setYMaximum(mImage.height()/*boundRectHeightMM*/);

	}
}

void CCipasComposerGLW::paintCommon( QPainter* painter, const QStyleOptionGraphicsItem* itemStyle, QWidget* pWidget )
{
	static int iCount = 0;
	if ( !painter )
	{
		return;
	}

	drawBackground( painter );

	SetGLWImg();
	//////////////////////////////////////////////////////////////////////////
	painter->save();
	mImage.load(IDPApplication::TempPath()+"Thematic.png");
	painter->translate( rect().width() / 2.0, rect().height() / 2.0 );
	painter->rotate( mRotation );
	painter->translate( -boundRectWidthMM / 2.0, -boundRectHeightMM / 2.0 );
	painter->drawImage( QRectF( 0, 0, boundRectWidthMM,  boundRectHeightMM ), mImage, 
		QRectF( 0, 0, mImage.width(), mImage.height()));
	//////////////////////////////////////////////////////////////////////////
	
	//frame and selection boxes
	drawFrame( painter );
	if ( isSelected() )
	{
		drawSelectionBoxes( painter );
	}
	painter->restore();
}



void CCipasComposerGLW::paintAdjustExtent( QPainter* painter, const QStyleOptionGraphicsItem* itemStyle, QWidget* pWidget )
{
	static int iCount = 0;
	QRectF ItemRect = rect();
	double dWidth = ItemRect.width();
	double dHeight = ItemRect.height();

	if ( !painter )
	{
		return;
	}

	drawBackground( painter );

	//int newDpi = ( painter->device()->logicalDpiX() + painter->device()->logicalDpiY() ) / 2;
	int newDpi=mComposition->printResolution();

	double rectPixelWidth = dWidth * newDpi / 25.4;
	double rectPixelHeight =  dHeight * newDpi / 25.4;
	QRectF boundRect;
	mImage.load(IDPApplication::TempPath()+"Thematic.png");

		double dImageWidth = mImage.width();
		double dImageHeight = mImage.height();
		boundRect = boundedImageRect( rectPixelWidth, rectPixelHeight );
		double boundRectWidthMM = boundRect.width() / newDpi * 25.4;
		double boundRectHeightMM = boundRect.height() / newDpi * 25.4;
		
		painter->save();
		painter->translate( rect().width() / 2.0, rect().height() / 2.0 );
		painter->rotate( mRotation );
		painter->translate( -boundRectWidthMM / 2.0, -boundRectHeightMM / 2.0 );
		QImage newImage= mImage.scaled(rect().width(),rect().height(),Qt::IgnoreAspectRatio);
		painter->drawImage( QRectF( 0, 0, boundRectWidthMM,  boundRectHeightMM ) ,newImage,  QRectF( 0, 0, newImage.width(), newImage.height()));

	
	//frame and selection boxes
	drawFrame( painter );
	if ( isSelected() )
	{
		drawSelectionBoxes( painter );
	}
	painter->restore();
}

QRectF CCipasComposerGLW::boundedImageRect( double deviceWidth, double deviceHeight )
{
	return QRectF(0,0,deviceWidth,deviceHeight);
}

void CCipasComposerGLW::setSceneRect( const QRectF& rectangle )
{


	//consider to change size of the shape if the rectangle changes width and/or height
	if ( rectangle.width() != rect().width() || rectangle.height() != rect().height() )
	{
		double newGLWidth = rectangle.width();
		double newGLHeight = rectangle.height();
		//imageSizeConsideringRotation( newGLWidth, newGLHeight );
		mGLWidth = newGLWidth;
		mGLHeight = newGLHeight;
	}

	CComposerItem::setSceneRect( rectangle );
	emit extentChanged();
}

void CCipasComposerGLW::mouseMoveEvent( QGraphicsSceneMouseEvent * event )
{
	CComposerItem::mouseMoveEvent(event);
}

bool CCipasComposerGLW::writeXML( QDomElement& elem, QDomDocument & doc ) const
{
	if ( elem.isNull() )
	{
		return false;
	}

	QDomElement composerMapElem = doc.createElement( "ComposerGLW" );
	composerMapElem.setAttribute( "id", mId );
	QDomElement extentElem = doc.createElement( "Extent" );
	extentElem.setAttribute( "xmin", mExtent.xMinimum() );
	extentElem.setAttribute( "xmax", mExtent.xMaximum() );
	extentElem.setAttribute( "ymin", mExtent.yMinimum() );
	extentElem.setAttribute( "ymax", mExtent.yMaximum() );
	composerMapElem.appendChild( extentElem );


	//layer set
	QDomElement layerSetElem = doc.createElement( "LayerSet" );
	QStringList::const_iterator layerIt = mLayerSet.constBegin();
	for ( ; layerIt != mLayerSet.constEnd(); ++layerIt )
	{
		QDomElement layerElem = doc.createElement( "Layer" );
		QDomText layerIdText = doc.createTextNode( *layerIt );
		layerElem.appendChild( layerIdText );
		layerSetElem.appendChild( layerElem );
	}
	composerMapElem.appendChild( layerSetElem );	

	//grid    mGridPosition
	QDomElement gridElem = doc.createElement( "Grid" );
	gridElem.setAttribute( "show", mGridEnabled );
	gridElem.setAttribute( "gridStyle", mGridStyle );
	gridElem.setAttribute( "intervalX", mGridIntervalX );
	gridElem.setAttribute( "intervalY", mGridIntervalY );
	gridElem.setAttribute( "offsetX", mGridOffsetX );
	gridElem.setAttribute( "offsetY", mGridOffsetY );
	gridElem.setAttribute( "penWidth", mGridPen.widthF() );
	gridElem.setAttribute( "penColorRed", mGridPen.color().red() );
	gridElem.setAttribute( "penColorGreen", mGridPen.color().green() );
	gridElem.setAttribute( "penColorBlue", mGridPen.color().blue() );
	gridElem.setAttribute( "crossLength", mCrossLength );
	gridElem.setAttribute( "gridPosition", mGridPosition );


	//grid annotation
	QDomElement annotationElem = doc.createElement( "Annotation" );
	annotationElem.setAttribute( "show", mShowGridAnnotation );
	annotationElem.setAttribute( "position", mGridAnnotationPosition );
	annotationElem.setAttribute( "frameDistance", mAnnotationFrameDistance );
	annotationElem.setAttribute( "direction", mGridAnnotationDirection );
	annotationElem.setAttribute( "font", mGridAnnotationFont.toString() );
	annotationElem.setAttribute( "precision", mGridAnnotationPrecision );
	annotationElem.setAttribute( "LabelAxesTop", LabelAxesTop );
	annotationElem.setAttribute( "LabelAxesLeft", LabelAxesLeft );
	annotationElem.setAttribute( "LabelAxesBottom", LabelAxesBottom );
	annotationElem.setAttribute( "LabelAxesRight", LabelAxesRight );
	annotationElem.setAttribute( "annotionStyle", (int)mAnnotionStyle );
	gridElem.appendChild( annotationElem );
	composerMapElem.appendChild( gridElem );	

	elem.appendChild( composerMapElem );
	return _writeXML( composerMapElem, doc );
}

bool CCipasComposerGLW::readXML( const QDomElement& itemElem, const QDomDocument& doc )
{

	if ( itemElem.isNull() )
	{
		return false;
	}

	//mLayerSet
	QDomNodeList layerSetNodeList = itemElem.elementsByTagName( "LayerSet" );
	QStringList layerSet;
	if ( layerSetNodeList.size() > 0 )
	{
		QDomElement layerSetElem = layerSetNodeList.at( 0 ).toElement();
		QDomNodeList layerIdNodeList = layerSetElem.elementsByTagName( "Layer" );
		for ( int i = 0; i < layerIdNodeList.size(); ++i )
		{
			layerSet << layerIdNodeList.at( i ).toElement().text();
		}
	}
	mLayerSet = layerSet;

	//grid
	QDomNodeList gridNodeList = itemElem.elementsByTagName( "Grid" );
	if ( gridNodeList.size() > 0 )
	{
		QDomElement gridElem = gridNodeList.at( 0 ).toElement();
		mGridEnabled = ( gridElem.attribute( "show", "0" ) != "0" );
		mGridStyle = CCipasComposerGLW::GridStyle( gridElem.attribute( "gridStyle", "0" ).toInt() );
		mGridIntervalX = gridElem.attribute( "intervalX", "10" ).toDouble();
		mGridIntervalY = gridElem.attribute( "intervalY", "10" ).toDouble();
		mGridOffsetX = gridElem.attribute( "offsetX", "0" ).toDouble();
		mGridOffsetY = gridElem.attribute( "offsetY", "0" ).toDouble();
		mGridPen.setWidthF( gridElem.attribute( "penWidth", "0" ).toDouble() );
		mGridPen.setColor( QColor( gridElem.attribute( "penColorRed", "0" ).toInt(), \
			gridElem.attribute( "penColorGreen", "0" ).toInt(), \
			gridElem.attribute( "penColorBlue", "0" ).toInt() ) );
		mCrossLength = gridElem.attribute( "crossLength", "3" ).toDouble();
		int inOrOutsideFrame=gridElem.attribute( "gridPosition", "0" ).toInt();
		if (!inOrOutsideFrame)		//0 框架内
		{
			mGridPosition=InsideMapFrame;
		} 
		else						//1	框架外
		{
			mGridPosition=OutsideMapFrame;
		}

		QDomNodeList annotationNodeList = gridElem.elementsByTagName( "Annotation" );
		if ( annotationNodeList.size() > 0 )
		{
			QDomElement annotationElem = annotationNodeList.at( 0 ).toElement();
			mShowGridAnnotation = ( annotationElem.attribute( "show", "0" ) != "0" );
			mGridAnnotationPosition = CCipasComposerGLW::GridAnnotationPosition( annotationElem.attribute( "position", "0" ).toInt() );
			mAnnotationFrameDistance = annotationElem.attribute( "frameDistance", "0" ).toDouble();
			mGridAnnotationDirection = CCipasComposerGLW::GridAnnotationDirection( annotationElem.attribute( "direction", "0" ).toInt() );
			mGridAnnotationFont.fromString( annotationElem.attribute( "font", "" ) );
			mGridAnnotationPrecision = annotationElem.attribute( "precision", "3" ).toInt();
			int iLabelAxes=annotationElem.attribute( "LabelAxesTop", "0" ).toUInt();
			LabelAxesTop=(bool)iLabelAxes;
			iLabelAxes=annotationElem.attribute( "LabelAxesLeft", "1" ).toUInt();
			LabelAxesLeft=(bool)iLabelAxes;
			iLabelAxes=annotationElem.attribute( "LabelAxesBottom", "1" ).toUInt();
			LabelAxesBottom=(bool)iLabelAxes;
			iLabelAxes=annotationElem.attribute( "LabelAxesRight", "0" ).toUInt();
			LabelAxesRight=(bool)iLabelAxes;
			mAnnotionStyle = (AnnotationStyle)annotationElem.attribute( "annotionStyle", "0" ).toInt();
		}
	}

	//restore general composer item properties
	QDomNodeList composerItemList = itemElem.elementsByTagName( "ComposerItem" );
	if ( composerItemList.size() > 0 )
	{
		QDomElement composerItemElem = composerItemList.at( 0 ).toElement();
		_readXML( composerItemElem, doc );
	}


	//extent
	QDomNodeList extentNodeList = itemElem.elementsByTagName( "Extent" );
	if ( extentNodeList.size() > 0 )
	{
		QDomElement extentElem = extentNodeList.at( 0 ).toElement();
		double xmin, xmax, ymin, ymax;
		xmin = extentElem.attribute( "xmin" ).toDouble();
		xmax = extentElem.attribute( "xmax" ).toDouble();
		ymin = extentElem.attribute( "ymin" ).toDouble();
		ymax = extentElem.attribute( "ymax" ).toDouble();
		/*
		如果为真 根据输入经纬度范围调整地图截图的视口范围
		否则 地图范围取模板*/
		if (isAdjustExtent)
		{
			setGLExtent(mLeftTop.x(),mRightBottom.y(),mRightBottom.x(),mLeftTop.y());
		}
		else
		{
			setGLExtent(xmin,ymin,xmax,ymax);
		}
	}
	this->update();
	emit itemChanged();
	return true;

}


void CCipasComposerGLW::drawCoordinateAnnotations( QPainter* p, const QList< QPair< double, QLineF > >& hLines, 
	const QList< QPair< double, QLineF > >& vLines )
{
	if(mGridAnnotationPosition == InsideMapFrame) return;

	if ( !p )
	{
		return;
	}


	QString currentAnnotationString;
	QList< QPair< double, QLineF > >::const_iterator it = hLines.constBegin();
	for ( ; it != hLines.constEnd(); ++it )
	{
		if (it->first>10e-4)
		{
			float f=it->first;
			if (f>88&&f<91) f=90;
			currentAnnotationString = QString::number( f, 'f', mGridAnnotationPrecision );
			currentAnnotationString = formateAnnotionString(currentAnnotationString);
			currentAnnotationString += QString("N");
		} 
		else if (abs(it->first)<=10e-4)
		{
			currentAnnotationString= QString("EQ");
		}
		else 
		{
			float f=it->first;
			if (f<-88&&f>-91) f=-90;
			currentAnnotationString = QString::number( -1*f, 'f', mGridAnnotationPrecision );
			currentAnnotationString = formateAnnotionString(currentAnnotationString);
			currentAnnotationString += QString("S");
		}
		drawCoordinateAnnotation( p, it->second.p1(), currentAnnotationString ,Qt::Vertical);
		/*if(mGridAnnotationPosition == InsideMapFrame)
		drawCoordinateAnnotation( p, it->second.p2(), currentAnnotationString );*/
	}

	it = vLines.constBegin();
	for ( ; it != vLines.constEnd(); ++it )
	{
		float f=it->first;
		if (it->first>-10e-4&&it->first<180)
		{

			currentAnnotationString = QString::number( f, 'f', mGridAnnotationPrecision );
			if (abs(f)>10e-4)
			{
				currentAnnotationString = formateAnnotionString(currentAnnotationString);
				currentAnnotationString += QString("E");
			}
		} 
		else if (abs(it->first-180)<=10e-4)
		{
			currentAnnotationString= QString("180");
			currentAnnotationString = formateAnnotionString(currentAnnotationString);
		}
		else
		{
			if (f>=357)
			{
				f=360;
			}
			f=360-f;
			currentAnnotationString = QString::number( f, 'f', mGridAnnotationPrecision );
			if (abs(f)>10e-4)
			{
				currentAnnotationString = formateAnnotionString(currentAnnotationString);
				currentAnnotationString += QString("W");
			}
		}
		drawCoordinateAnnotation( p, it->second.p1(), currentAnnotationString,Qt::Horizontal );
		/*if(mGridAnnotationPosition == InsideMapFrame)
		drawCoordinateAnnotation( p, it->second.p2(), currentAnnotationString );*/
	}
}

void CCipasComposerGLW::drawCoordinateAnnotation( QPainter* p, const QPointF& pos, QString annotationString,Qt::Orientation orient )
{
	//根据点的位置判断属于哪个边
	Border frameBorder = borderForLineCoord( pos ,orient);
	double textWidth = textWidthMillimeters( mGridAnnotationFont, annotationString );
	//relevant for annotations is the height of digits
	double textHeight = fontHeightCharacterMM( mGridAnnotationFont, QChar( '0' ) );
	double xpos = pos.x();
	double ypos = pos.y();
	int rotation = 0;

	if ( frameBorder == Left )
	{
		if ( false/*mGridAnnotationPosition == InsideMapFrame*/ )
		{
			if ( mGridAnnotationDirection == Vertical || mGridAnnotationDirection == BoundaryDirection )
			{
				xpos += textHeight + mAnnotationFrameDistance + mDisToFrame;
				ypos += textWidth / 2.0;
				rotation = 270;
			}
			else
			{
				xpos += mAnnotationFrameDistance + mDisToFrame;
				ypos += textHeight / 2.0;
			}
		}
		else //Outside map frame
		{
			if (!LabelAxesLeft) return;
			if ( mGridAnnotationDirection == Vertical || mGridAnnotationDirection == BoundaryDirection )
			{
				xpos -= mAnnotationFrameDistance/* + mDisToFrame*/;
				ypos += textWidth / 2.0;
				rotation = 270;
			}
			else
			{
				xpos -= textWidth + mAnnotationFrameDistance;
				ypos += textHeight / 2.0;
			}
		}

	}
	else if ( frameBorder == Right )
	{
		if ( false/*mGridAnnotationPosition == InsideMapFrame*/ )
		{
			if ( mGridAnnotationDirection == Vertical || mGridAnnotationDirection == BoundaryDirection )
			{
				xpos -= mAnnotationFrameDistance + mDisToFrame;
				ypos += textWidth / 2.0;
				rotation = 270;
			}
			else //Horizontal
			{
				xpos -= textWidth + mAnnotationFrameDistance + mDisToFrame;
				ypos += textHeight / 2.0;
			}
		}
		else //OutsideMapFrame
		{
			if (!LabelAxesRight) return;
			if ( mGridAnnotationDirection == Vertical || mGridAnnotationDirection == BoundaryDirection )
			{
				xpos += textHeight + mAnnotationFrameDistance + miGraduationLineLength/* + mDisToFrame*/;
				ypos += textWidth / 2.0;
				rotation = 270;
			}
			else //Horizontal
			{
				xpos += mAnnotationFrameDistance + miGraduationLineLength/* + mDisToFrame*/;
				ypos += textHeight / 2.0;
			}
		}
	}
	else if ( frameBorder == Bottom )
	{
		if ( false/*mGridAnnotationPosition == InsideMapFrame*/ )
		{
			if ( mGridAnnotationDirection == Horizontal || mGridAnnotationDirection == BoundaryDirection )
			{
				ypos -= mAnnotationFrameDistance + miGraduationLineLength;
				xpos -= textWidth / 2.0;
			}
			else //Vertical
			{
				xpos += textHeight / 2.0;
				ypos -= mAnnotationFrameDistance + miGraduationLineLength;
				rotation = 270;
			}
		}
		else //OutsideMapFrame
		{
			if (!LabelAxesBottom) return;
			if ( mGridAnnotationDirection == Horizontal || mGridAnnotationDirection == BoundaryDirection )
			{
				ypos += mAnnotationFrameDistance + textHeight + miGraduationLineLength;
				xpos -= textWidth / 2.0;
			}
			else //Vertical
			{
				xpos += textHeight / 2.0;
				ypos += textWidth + mAnnotationFrameDistance + miGraduationLineLength;
				rotation = 270;
			}
		}
	}
	else //Top
	{
		if ( false/*mGridAnnotationPosition == InsideMapFrame*/ )
		{
			if ( mGridAnnotationDirection == Horizontal || mGridAnnotationDirection == BoundaryDirection )
			{
				xpos -= textWidth / 2.0;
				ypos += textHeight + (mAnnotationFrameDistance/* + miGraduationLineLength*/);
			}
			else //Vertical
			{
				xpos += textHeight / 2.0;
				ypos += textWidth + (mAnnotationFrameDistance/* + miGraduationLineLength*/);
				rotation = 270;
			}
		}
		else //OutsideMapFrame
		{
			if (!LabelAxesTop) return;
			if ( mGridAnnotationDirection == Horizontal || mGridAnnotationDirection == BoundaryDirection )
			{
				xpos -= textWidth / 2.0;
				ypos -= (mAnnotationFrameDistance/* - miGraduationLineLength*/);
			}
			else //Vertical
			{
				xpos += textHeight / 2.0;
				ypos -= (mAnnotationFrameDistance/* - miGraduationLineLength*/);
				rotation = 270;
			}
		}
	}

	drawAnnotation( p, QPointF( xpos, ypos ), rotation, annotationString );
}

void CCipasComposerGLW::drawAnnotation( QPainter* p, const QPointF& pos, int rotation, const QString& annotationText )
{
	p->save();
	p->translate( pos );
	p->rotate( rotation );
	p->setPen( mAnntationColor );
	drawText( p, 0, 0, annotationText, mGridAnnotationFont );
	p->restore();
}



QPolygonF CCipasComposerGLW::transformedMapPolygon() const
{
	double dx = mXOffset;
	double dy = mYOffset;
	//qWarning("offset");
	//qWarning(QString::number(dx).toLocal8Bit().data());
	//qWarning(QString::number(dy).toLocal8Bit().data());
	transformShift( dx, dy );
	//qWarning("transformed:");
	//qWarning(QString::number(dx).toLocal8Bit().data());
	//qWarning(QString::number(dy).toLocal8Bit().data());
	QPolygonF poly;
	mapPolygon( poly );
	poly.translate( -dx, -dy );
	return poly;
}

void CCipasComposerGLW::transformShift( double& xShift, double& yShift ) const
{
	double mmToMapUnits = 1.0 / mapUnitsToMM();
	double dxScaled = xShift * mmToMapUnits;
	double dyScaled = - yShift * mmToMapUnits;

	rotate( mRotation, dxScaled, dyScaled );

	xShift = dxScaled;
	yShift = dyScaled;
}

double CCipasComposerGLW::mapUnitsToMM() const
{
	return 1;
}

void CCipasComposerGLW::mapPolygon( QPolygonF& poly ) const
{
	poly.clear();
	if ( mRotation == 0 )
	{
		poly << QPointF( mExtent.xMinimum(), mExtent.yMaximum() );
		poly << QPointF( mExtent.xMaximum(), mExtent.yMaximum() );
		poly << QPointF( mExtent.xMaximum(), mExtent.yMinimum() );
		poly << QPointF( mExtent.xMinimum(), mExtent.yMinimum() );
		return;
	}

	//there is rotation
	CCipasPoint rotationPoint(( mExtent.xMaximum() + mExtent.xMinimum() ) / 2.0, ( mExtent.yMaximum() + mExtent.yMinimum() ) / 2.0 );
	double dx, dy; //x-, y- shift from rotation point to corner point

	//top left point
	dx = rotationPoint.x() - mExtent.xMinimum();
	dy = rotationPoint.y() - mExtent.yMaximum();
	rotate( mRotation, dx, dy );
	poly << QPointF( rotationPoint.x() + dx, rotationPoint.y() + dy );

	//top right point
	dx = rotationPoint.x() - mExtent.xMaximum();
	dy = rotationPoint.y() - mExtent.yMaximum();
	rotate( mRotation, dx, dy );
	poly << QPointF( rotationPoint.x() + dx, rotationPoint.y() + dy );

	//bottom right point
	dx = rotationPoint.x() - mExtent.xMaximum();
	dy = rotationPoint.y() - mExtent.yMinimum();
	rotate( mRotation, dx, dy );
	poly << QPointF( rotationPoint.x() + dx, rotationPoint.y() + dy );

	//bottom left point
	dx = rotationPoint.x() - mExtent.xMinimum();
	dy = rotationPoint.y() - mExtent.yMinimum();
	rotate( mRotation, dx, dy );
	poly << QPointF( rotationPoint.x() + dx, rotationPoint.y() + dy );
}

CCipasComposerGLW::Border CCipasComposerGLW::borderForLineCoord( const QPointF& p ,Qt::Orientation orient) const
{
	/*仅根据点是无法判断是否属于哪个边的，例如 底部起始经度*/
	if ( p.x() <= pen().widthF() && orient==Qt::Vertical)
	{
		return Left;
	}
	else if ( p.x() >= ( rect().width() - pen().widthF() )&& orient==Qt::Vertical )
	{
		return Right;
	}
	else if ( p.y() <= pen().widthF() )
	{
		return Top;
	}
	else
	{
		return Bottom;
	}
}

QPointF CCipasComposerGLW::mapToItemCoords( const QPointF& mapCoords ) const
{
	QPolygonF mapPoly = transformedMapPolygon();
	if ( mapPoly.size() < 1 )
	{
		return QPointF( 0, 0 );
	}

	CCipasRectangle tExtent = transformedExtent();
	CCipasPoint rotationPoint(( tExtent.xMaximum() + tExtent.xMinimum() ) / 2.0, ( tExtent.yMaximum() + tExtent.yMinimum() ) / 2.0 );
	double dx = mapCoords.x() - rotationPoint.x();
	double dy = mapCoords.y() - rotationPoint.y();
	rotate( -mRotation, dx, dy );
	CCipasPoint backRotatedCoords( rotationPoint.x() + dx, rotationPoint.y() + dy );

	CCipasRectangle unrotatedExtent = transformedExtent();
	double xItem = rect().width() * ( backRotatedCoords.x() - unrotatedExtent.xMinimum() ) / unrotatedExtent.width();
	double yItem = rect().height() * ( 1 - ( backRotatedCoords.y() - unrotatedExtent.yMinimum() ) / unrotatedExtent.height() );
	return QPointF( xItem, yItem );
}


CCipasRectangle CCipasComposerGLW::transformedExtent() const
{
	double dx = mXOffset;
	double dy = mYOffset;
	transformShift( dx, dy );
	return CCipasRectangle( mExtent.xMinimum() - dx, mExtent.yMinimum() - dy, mExtent.xMaximum() - dx, mExtent.yMaximum() - dy );
}

void CCipasComposerGLW::setGridPenWidth( double w )
{
	mGridPen.setWidthF( w );
}

void CCipasComposerGLW::updateComposition()
{
	mComposition->update();
}

void CCipasComposerGLW::updateBoundingRect()
{
	QRectF rectangle = rect();
	double extension = maxExtension();
	rectangle.setLeft( rectangle.left() - extension );
	rectangle.setRight( rectangle.right() + extension );
	rectangle.setTop( rectangle.top() - extension );
	rectangle.setBottom( rectangle.bottom() + extension );
	if ( rectangle != mCurrentRectangle )
	{
		prepareGeometryChange();
		mCurrentRectangle = rectangle;
	}
}

double CCipasComposerGLW::maxExtension() const
{
	if ( !mGridEnabled || !mShowGridAnnotation || mGridAnnotationPosition != OutsideMapFrame )
	{
		return 0;
	}

	QList< QPair< double, QLineF > > xLines;
	QList< QPair< double, QLineF > > yLines;



	double maxExtension = 0;
	double currentExtension = 0;
	QString currentAnnotationString;

	QList< QPair< double, QLineF > >::const_iterator it = xLines.constBegin();
	for ( ; it != xLines.constEnd(); ++it )
	{
		currentAnnotationString = QString::number( it->first, 'f', mGridAnnotationPrecision );
		currentExtension = qMax( textWidthMillimeters( mGridAnnotationFont, currentAnnotationString ), fontAscentMillimeters( mGridAnnotationFont ) );
		maxExtension = qMax( maxExtension, currentExtension );
	}

	it = yLines.constBegin();
	for ( ; it != yLines.constEnd(); ++it )
	{
		currentAnnotationString = QString::number( it->first, 'f', mGridAnnotationPrecision );
		currentExtension = qMax( textWidthMillimeters( mGridAnnotationFont, currentAnnotationString ), fontAscentMillimeters( mGridAnnotationFont ) );
		maxExtension = qMax( maxExtension, currentExtension );
	}

	return maxExtension + mAnnotationFrameDistance;
}

void CCipasComposerGLW::zoomContent( int delta, double x, double y )
{
	//cache();
	//update();
	emit itemChanged();
	//emit extentChanged();
	update();
	emit extentChanged();
}

void CCipasComposerGLW::draw( QPainter *painter, const CCipasRectangle& extent, const QSizeF& size, double dpi )
{
	if ( !painter )
	{
		return;
	}
}


void CCipasComposerGLW::setInOrOutSideFrame(int i)
{
	if (i==0)
	{
		mGridPosition=InsideMapFrame;
	}
	else if (i==1)
	{
		mGridPosition=OutsideMapFrame;
	}
}

int CCipasComposerGLW::getInOrOutSideFrame()
{
	if (mGridPosition==InsideMapFrame)
	{
		return 0;
	}
	else
		return 1;
}

QString CCipasComposerGLW::formateAnnotionString(QString inputs)
{
	if (mAnnotionStyle == AnnotationStyle::Degree)
	{
		return inputs+QString("°");
	}
	else if (mAnnotionStyle == AnnotationStyle::DegreeMinuteSecond)
	{
		return inputs;		//未处理
	}
	else 
		return inputs;

}

void CCipasComposerGLW::updateFuckImage()
{
	this->update();
}

unsigned long long CCipasComposerGLW::GetMapScale()
{
	return m_pViewerImage->getMapScale(0);
}

