// FrameToolBoxesControler.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "FrameToolBoxesContrlerManager.h"

ControllerManager* g_pControllerManager = 0;

extern "C" __declspec(dllexport) ControllerManager* GetControllerManager()
{
	if(g_pControllerManager == 0)
	{
		g_pControllerManager = new FrameToolBoxesContrlerManager;
	}
	
	return g_pControllerManager;
}