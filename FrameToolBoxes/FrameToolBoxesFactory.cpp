#include "StdAfx.h"
#include "FrameToolBoxesFactory.h"
#include "StandardTools.h"
#include "MapToolsBox.h"
#include "WindowControlTools.h"

FrameToolBoxesFactory::FrameToolBoxesFactory(void)
{
	m_pToolBoxInterface = new StandardTools;
	m_pMapToosBox = new MapToolsBox;
	m_pWindowTools = new WindowControlTools;
}


FrameToolBoxesFactory::~FrameToolBoxesFactory(void)
{
}

ToolBoxInterface* FrameToolBoxesFactory::GetToolBoxesInterface(const QString& name)
{
	if(name == QString::fromLocal8Bit("DefaultTooBox"))
	{
		return m_pToolBoxInterface;
	}
	else if(name == QString::fromLocal8Bit("MapToolBox"))
	{
		return m_pMapToosBox;
	}
	else if(name == QString::fromLocal8Bit("WindowToolBox"))
	{
		return m_pWindowTools;
	}
	else
		return 0;
}

int FrameToolBoxesFactory::GetToolBoxCount()
{
	return 3;
}
