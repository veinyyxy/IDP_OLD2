// FramePropertyBox.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"

#include "DockWidgetFactoryImps.h"
#include "ActionFactory.h"
#include "DockActionFactory.h"

DockWidgetFactory* g_pDockWidgetFacotry = 0;
ActionFactory* g_pActionFactory = 0;

extern "C" __declspec(dllexport) DockWidgetFactory* GetDockWidgetFactory()
{
	if(g_pDockWidgetFacotry == 0)
		return g_pDockWidgetFacotry = new DockWidgetFactoryImps;
	else
		return g_pDockWidgetFacotry;
}

extern "C" __declspec(dllexport) ActionFactory* GetActionFactory()
{
	if(g_pActionFactory == 0)
	{
		g_pActionFactory = new DockActionFactory;
		return g_pActionFactory;
	}
	else
	{
		return g_pActionFactory;
	}
}