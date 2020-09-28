#include <osgEarth/SpatialReference>

#include "Map.h"
#include "AdapterWidget.h"
#include "AnimaticControlerInterface.h"
#include "SysMainWindow.h"
#include "AnimaticControler.h"
#include "statustoolbar.h"
#include "ViewManager.h"
#include "AnimaticAction.h"

//RadarAnimatic ControlAnimatic::s_RadarAnimatic;
//

AnimaticControler* GetAnimaticControler(QAction* pAction)
{
	AnimaticControler* m_pAnimaticControler = 0;
	QVariant pVar =  pAction->parentWidget()->property("m_AnimaticControler");
	if(pVar.isValid())
	{
		AnimaticControler* m_pAnimaticControler = (AnimaticControler*)(pVar.toULongLong());
		return m_pAnimaticControler;
	}
	else
		return m_pAnimaticControler;
}

RadarAnimatic* ControlAnimatic::GetControlAnimaticor()
{
	RadarAnimatic* pRadarAnimatic = 0;
	/*MapViewWidget* pMvw = MapViewWidget::GetCurrentViewWidget();

	if(!pMvw) return 0;
	if(pMvw->GetGraphDisplayManager()->GetViewType() == NONE_TYPE)
		return 0;
	MapTreeWidgetAPP* pMtwa = pMvw->GetTreeWidgetApp();
	pRadarAnimatic = pMtwa->GetRadarAnimatic();*/
	return pRadarAnimatic;
}

ControlAnimatic::ControlAnimatic()// : m_FrameNumber(0)
{

}

bool ControlAnimatic::GetOptionModel()
{
	MainWindow* pMainWinow = SysMainWindow::GetMainWindow();
	/*StatusToolBar* pStatusToolBar = pMainWinow->getStatusToolBar();


	if(pStatusToolBar->getOptionModel() != StatusToolBar::NoneModel && pStatusToolBar->getOptionModel() != StatusToolBar::AttentionModel)
	return true;
	else*/
		return false;
}

ADD_MYACTION_CONTSTRUCTED_DECLARATION(PlayAction)
ADD_MYACTION_CONTSTRUCTED_DECLARATION(PauseAction)
ADD_MYACTION_CONTSTRUCTED_DECLARATION(StopAction)
ADD_MYACTION_CONTSTRUCTED_DECLARATION(speeddownAction)
ADD_MYACTION_CONTSTRUCTED_DECLARATION(speedupAction)
ADD_MYACTION_CONTSTRUCTED_DECLARATION(PreAction)
ADD_MYACTION_CONTSTRUCTED_DECLARATION(NextAction)
ADD_MYACTION_CONTSTRUCTED_DECLARATION(LoopAction)

//------------------PlayAction------------------------
PlayAction::~PlayAction(void)
{

}

void PlayAction::MyAction()
{
	int iLayerCount = 0;

	m_pAnimaticControler = GetAnimaticControler(this);
	if(!m_pAnimaticControler) return;
	if(!m_pAnimaticControler->IsInitComplete())
	{
		QString dir = this->parentWidget()->property("dir").toString();

		m_pAnimaticControler->InitData(dir);
		m_pAnimaticControler->Play();
	}
	else
		m_pAnimaticControler->Play();
}

void PlayAction::InitMyAction()
{
	m_pAnimaticControler = 0;
}
//--------------------PauseAction----------------------
PauseAction::~PauseAction(void)
{

}

void PauseAction::MyAction()
{
	m_pAnimaticControler = GetAnimaticControler(this);
	if(m_pAnimaticControler)
	{
		m_pAnimaticControler->Pause();
	}
}

void PauseAction::InitMyAction()
{
	m_pAnimaticControler = 0;
}
//--------------------StopAction-----------------------
StopAction::~StopAction(void)
{

}

void StopAction::MyAction()
{
	m_pAnimaticControler = GetAnimaticControler(this);
	if(m_pAnimaticControler)
	{
		m_pAnimaticControler->Stop();
	}
}

void StopAction::InitMyAction()
{
	m_pAnimaticControler = 0;
}
//-----------------speeddownAction--------------------------
speeddownAction::~speeddownAction(void)
{

}

void speeddownAction::MyAction()
{
	m_pAnimaticControler = GetAnimaticControler(this);
	if(m_pAnimaticControler)
	{
		m_pAnimaticControler->SpeedDown();
	}
}

void speeddownAction::InitMyAction()
{
	m_pAnimaticControler = 0;
}
//----------------------speedupAction---------------------
speedupAction::~speedupAction(void)
{

}

void speedupAction::MyAction()
{
	m_pAnimaticControler = GetAnimaticControler(this);
	if(m_pAnimaticControler)
	{
		m_pAnimaticControler->SpeedUp();
	}
}

void speedupAction::InitMyAction()
{
	m_pAnimaticControler = 0;
}
//----------------------PreAction---------------------
PreAction::~PreAction(void)
{

}

void PreAction::MyAction()
{
	m_pAnimaticControler = GetAnimaticControler(this);
	if(m_pAnimaticControler)
	{
		m_pAnimaticControler->Previous();
	}
}

void PreAction::InitMyAction()
{
	m_pAnimaticControler = 0;
}
//---------------------NextAction----------------------
NextAction::~NextAction(void)
{

}

void NextAction::MyAction()
{
	m_pAnimaticControler = GetAnimaticControler(this);
	if(m_pAnimaticControler)
	{
		m_pAnimaticControler->Next();
	}
}

void NextAction::InitMyAction()
{
	m_pAnimaticControler = 0;
}
//----------------------LoopAction---------------------
LoopAction::~LoopAction(void)
{

}

void LoopAction::MyAction()
{
	m_pAnimaticControler = GetAnimaticControler(this);
	if(m_pAnimaticControler)
	{
		m_pAnimaticControler->Loop();
	}
}

void LoopAction::InitMyAction()
{
	m_pAnimaticControler = 0;
}