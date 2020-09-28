#pragma once
#include "Map.h"
#include <osgViewer/Viewer>

class ControllerFrameContext
{
public:
	virtual osgViewer::Viewer* GetCurrentViewer() = 0;
	virtual goto_gis::Map* GetCurrentMap() = 0;
};

