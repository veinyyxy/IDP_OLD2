#include "OsgSceneDialog.h"
#include "AdapterWidget.h"

OsgSceneDialog::OsgSceneDialog(void)
{
	ViewerQT* pViewerQt = new ViewerQT(0);

	m_pViewer = pViewerQt->getViewer();


}

OsgSceneDialog::~OsgSceneDialog(void)
{

}
