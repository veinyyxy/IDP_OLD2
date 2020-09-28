#pragma once
#include "ControllerManager.h"

class FrameToolBoxesContrlerManager :
	public ControllerManager
{
public:
	FrameToolBoxesContrlerManager(void);
	~FrameToolBoxesContrlerManager(void);

	virtual Controller* GetControllerFromName( const std::string& strName );

	virtual void GetControllerVector( std::vector<Controller*>* pOutVector);

	virtual void GetControllerNameVector( std::vector<std::string>* pOutVector);
private:
	Controller* m_pOpenDisplayWindowController;
	Controller* m_pDockLayerTreeController;
};

