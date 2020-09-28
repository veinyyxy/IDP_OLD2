#pragma once
#include <osgGA/GUIActionAdapter>
#include <osgGA/GUIEventHandler>
#include <osgViewer/Viewer>
#include "PosterPrinter.h"

class PrintPosterHandler : public osgGA::GUIEventHandler
{
public:
	PrintPosterHandler( PosterPrinter* printer = 0 );
	~PrintPosterHandler();
	bool handle( const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa );
	inline void setPosterPrinter(PosterPrinter* pPP){_printer = pPP;}
	inline void setGiveMeImage(bool bG){_bGiveMeImage = bG;}
protected:
	osg::ref_ptr<PosterPrinter> _printer;
	bool _started;
	bool _bGiveMeImage;
	double preTime;
	double nowTime;
	double finalTime;
	double finalTime2;
};
