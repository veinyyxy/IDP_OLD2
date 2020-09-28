// dllmain.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"
#include "FrameToolBoxesFactory.h"
#include "DisplayViewFactory.h"
#include "FramTooBoxesActionFactory.h"
#include "DefaultDisplayViewFactory.h"

class ToolBoxesFactory;
class DisplayViewFactory;
class ActionFactory;

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}

ToolBoxesFactory* g_pFactory = 0;
DisplayViewFactory* g_pDisplayViewFactory = 0;
ActionFactory* g_pActionFactory = 0;

extern "C" __declspec(dllexport) ToolBoxesFactory* GetToolBoxesFactory()
{
	if(g_pFactory == 0)
	{
		g_pFactory = new FrameToolBoxesFactory;
		return g_pFactory;
	}
	else
	{
		return g_pFactory;
	}
}

extern "C" __declspec(dllexport) DisplayViewFactory* GetDisplayViewFactory()
{
	if(g_pDisplayViewFactory == 0)
	{
		g_pDisplayViewFactory = new DefaultDisplayViewFactory;
		return g_pDisplayViewFactory;
	}
	else
	{
		return g_pDisplayViewFactory;
	}
}

extern "C" __declspec(dllexport) ActionFactory* GetActionFactory()
{
	if(g_pActionFactory == 0)
	{
		g_pActionFactory = new FramTooBoxesActionFactory;
		return g_pActionFactory;
	}
	else
	{
		return g_pActionFactory;
	}
}