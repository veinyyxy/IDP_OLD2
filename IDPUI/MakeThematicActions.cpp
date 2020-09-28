#include "MakeThematicActions.h"
#include <QtGui/QMdiArea>
#include <QtGui/QMdiSubWindow>
#include "AdapterWidget.h"
#include "CipasComposer.h"
#include "IDPApplication.h"

ADD_MYACTION_CONTSTRUCTED_DECLARATION(ThematicProductAction)

extern QWidget* GetActiveWindow(QMdiArea* m_pMdiArea);

void ThematicProductAction::MyAction()
{
	QWidget* viewerWindow = GetActiveWindow(m_pMdiArea);
	
	ViewerQT* vW = dynamic_cast<ViewerQT*>(viewerWindow);
	if (vW)
	{
		CCipasComposer* pCipasComposer = new CCipasComposer(0);
		pCipasComposer->setOsgView(vW);
		pCipasComposer->setWindowModality(Qt::ApplicationModal);

		pCipasComposer->setExtent(0,-90,180,90);
		//m_pCipasComposer->setExtent(x_min,y_mim,x_max,y_max);

		pCipasComposer->statusBar()->showMessage(tr("¾ÍÐ÷"));
		QString templateString =IDPApplication::TemplatePath()+ "/ThemeTemplate/ServerGrid.cpt";
		pCipasComposer->LoadTemplatefromfile(templateString);
		pCipasComposer->open();
		pCipasComposer->showMaximized();
	}
}

void ThematicProductAction::InitMyAction()
{
	m_pMdiArea = 0;
}

ThematicProductAction::~ThematicProductAction( void )
{

}


