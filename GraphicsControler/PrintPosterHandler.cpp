#include "PrintPosterHandler.h"
#include "osg/Timer"

PrintPosterHandler::PrintPosterHandler(PosterPrinter* printer)
	: _printer(printer), _started(false), _bGiveMeImage(false) 
{
	preTime = 0.0000;
	nowTime = 0.0000;
	finalTime = 0;
	finalTime2 = 0;
}

PrintPosterHandler::~PrintPosterHandler(void)
{
}

bool PrintPosterHandler::handle( const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa )
{
	static int iFrame = 0;
	osgViewer::Viewer* view = dynamic_cast<osgViewer::Viewer*>( &aa );
	if ( !view ) return false;

	switch( ea.getEventType() )
	{
	case osgGA::GUIEventAdapter::FRAME:

		if ( view->getDatabasePager() )
		{
			// Wait until all paged nodes are processed
			if ( view->getDatabasePager()->getRequestsInProgress() )
				break;
		}

		if ( _printer.valid() )
		{
			_printer->frame( view->getFrameStamp(), view->getSceneData() );
			if ( _started && _printer->done() )
			{
				osg::Switch* root = dynamic_cast<osg::Switch*>( view->getSceneData() );
				if ( root )
				{
					// Assume child 0 is the loaded model and 1 is the poster camera
					// Switch them in time to prevent dual traversals of subgraph
					root->setValue( 0, true );
					root->setValue( 1, false );
				}
				_started = false;
			}
		}
		iFrame++;
		break;
	default:
		break;
	}
	

	if(iFrame == 10)
	{
		_bGiveMeImage = true;
		iFrame = 0;
	}

	if (_bGiveMeImage)
	{
		if ( _printer.valid() )
		{
			osg::Switch* root = dynamic_cast<osg::Switch*>( view->getSceneData() );
			if ( root )
			{
				// Assume child 0 is the loaded model and 1 is the poster camera
				root->setValue( 0, false );
				root->setValue( 1, true );
			}
			osg::Camera* pCamera = view->getCamera();
			osg::Matrix viewMatrix = pCamera->getViewMatrix();
			osg::Matrix projMatrix = pCamera->getProjectionMatrix();

			double fovy, aspectRatio, zNear, zFar;

			osg::Vec2 v2Size = _printer->getPosterSize();

			projMatrix.getPerspective(fovy, aspectRatio, zNear, zFar);
			aspectRatio = v2Size.x()/v2Size.y();
			projMatrix.makePerspective(fovy, aspectRatio, zNear, zFar);

			_printer->init(viewMatrix, projMatrix);
			_started = true;
			_bGiveMeImage = false;
		}
		return true;
	}
	return false;
}
