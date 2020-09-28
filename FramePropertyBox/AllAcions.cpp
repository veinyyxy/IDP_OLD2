#include "StdAfx.h"
#include "AllAcions.h"
#include "Map.h"
#include "MainControlFrame.h"
#include "MainFrameWindow.h"
#include "ControllerFrameContext.h"
#include "GeosOnTheOsg.h"
#include "FieldDialog.h"

goto_gis::Map* ActionGetCurrentMap()
{
	MainControlFrame* pMCF = MainControlFrame::GetMainControlFrameInstance();
	ControllerFrameContext* pCFC = pMCF->GetCurrentContext();
	goto_gis::Map* pMap = pCFC->GetCurrentMap();
	return pMap;
}

ADD_MYACTION_CONTSTRUCTED_DECLARATION(MoveUpAction)

void MoveUpAction::MyAction()
{
	goto_gis::Map* pMap = ActionGetCurrentMap();
	if(pMap)
		pMap->MoveUpByZ(std::string(""));
}

void MoveUpAction::InitMyAction()
{

}

void MoveUpAction::SetController( Controller* pController )
{

}

ADD_MYACTION_CONTSTRUCTED_DECLARATION(MoveDownAction)

	void MoveDownAction::MyAction()
{
	goto_gis::Map* pMap = ActionGetCurrentMap();
	if(pMap)
		pMap->MoveDownByZ(std::string(""));

}

void MoveDownAction::InitMyAction()
{

}

void MoveDownAction::SetController( Controller* pController )
{

}

ADD_MYACTION_CONTSTRUCTED_DECLARATION(AddBasicAction)

void AddBasicAction::MyAction()
{
	MainFrameWindow* pMFW = MainFrameWindow::GetUIMainFramInstance();
	QString fileName = QFileDialog::
		getOpenFileName(pMFW,tr("打开地图文件"), "/"
		, tr("矢量文件 (*.shp);;TIFF文件 (*.tif)"));

	if(fileName.size() == 0) return;

	goto_gis::Layer* player = new goto_gis::VectorLayer;
	goto_gis::GeosOnTheOsg* pGOTO = goto_gis::GeosOnTheOsg::Instantiate();

	goto_gis::Map* pMap = ActionGetCurrentMap();

	if(pMap != 0)
	{
		player->CoordTrans(pMap->GetCoordinateTransform());
		pGOTO->OpenDataAsLayer(fileName.toStdString(), player);
		player->CreateLayerData();

		pMap->AddLayer(player->GetLayerName(), player);
	}

}

void AddBasicAction::InitMyAction()
{

}

void AddBasicAction::SetController( Controller* pController )
{

}

ADD_MYACTION_CONTSTRUCTED_DECLARATION(ShowLayerPropertyAction)

void ShowLayerPropertyAction::MyAction()
{
	goto_gis::Map* pMap = ActionGetCurrentMap();
	if(pMap == 0) return;

	goto_gis::Layer* layer = pMap->CurrentLayer();
	if (layer)
	{
		//std::map<int, GeomField>* mapField = layer->GetFieldMap();
		MainFrameWindow* pMFW = MainFrameWindow::GetUIMainFramInstance();
		FieldDialog* m_pFieldDialog = new FieldDialog(layer, pMFW);
		m_pFieldDialog->show();
	}

}

void ShowLayerPropertyAction::InitMyAction()
{

}

void ShowLayerPropertyAction::SetController( Controller* pController )
{

}

ADD_MYACTION_CONTSTRUCTED_DECLARATION(DeleteLayerAction)

void DeleteLayerAction::MyAction()
{
	
	goto_gis::Map* pMap = ActionGetCurrentMap();
	if(pMap == 0) return;

	goto_gis::Layer* layer = pMap->CurrentLayer();
	if (layer)
	{
		pMap->RemoveLayer(pMap->CurrentLayer());
	}
}

void DeleteLayerAction::InitMyAction()
{

}

void DeleteLayerAction::SetController( Controller* pController )
{

}

ADD_MYACTION_CONTSTRUCTED_DECLARATION(ShowLayerAction)

	void ShowLayerAction::MyAction()
{
	goto_gis::Map* pMap = ActionGetCurrentMap();
	if(pMap == 0) return;

	goto_gis::Layer* layer = pMap->CurrentLayer();
	if (layer)
	{
		pMap->SetVisibleLayer(pMap->CurrentLayer()->LayerName(), true);
	}
}

void ShowLayerAction::InitMyAction()
{

}

void ShowLayerAction::SetController( Controller* pController )
{

}


ADD_MYACTION_CONTSTRUCTED_DECLARATION(HideLayerAction)

	void HideLayerAction::MyAction()
{
	goto_gis::Map* pMap = ActionGetCurrentMap();
	if(pMap == 0) return;

	goto_gis::Layer* layer = pMap->CurrentLayer();
	if (layer)
	{
		pMap->SetVisibleLayer(pMap->CurrentLayer()->LayerName(), false);
	}

}

void HideLayerAction::InitMyAction()
{

}

void HideLayerAction::SetController( Controller* pController )
{

}