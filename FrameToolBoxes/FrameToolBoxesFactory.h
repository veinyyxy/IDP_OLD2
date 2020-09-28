#pragma once
#include "ToolBoxesFactory.h"

class FrameToolBoxesFactory :
	public ToolBoxesFactory
{
public:
	FrameToolBoxesFactory(void);
	~FrameToolBoxesFactory(void);

	virtual ToolBoxInterface* GetToolBoxesInterface(const QString& name);

	virtual int GetToolBoxCount();
private:
	ToolBoxInterface* m_pToolBoxInterface;
	ToolBoxInterface* m_pMapToosBox;
	ToolBoxInterface* m_pWindowTools;
};

