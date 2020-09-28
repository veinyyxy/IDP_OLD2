#include "DrawLine.h"
#include "DrawCircle.h"
#include "DrawPolygonalLine.h"
#include "DrawGeometryData.h"
#include "DrawGeometryManipulator.h"
#include "DrawRect.h"
#include "MoveSymbol.h"
#include "DeleteSymbol.h"
#include "AdapterWidget.h"
#include "MainWindow.h"
#include "sysmainwindow.h"
#include <QtGui/QMdiSubWindow>
#include "ManualDrawLayer.h"
#include "DrawWindSymbol.h"

#include "DrawGeometryActions.h"
#include "DrawSymbol.h"

//osgGA::CameraManipulator* g_pBackCameraMainpulator = 0;
//osg::Node* g_pCanvasNode = 0;

ViewerQT* GetViwer()
{
	static osgGA::CameraManipulator* localCM = 0;
	MainWindow* mw = SysMainWindow::GetMainWindow();
	ViewerQT* viewerWindow = dynamic_cast<ViewerQT*>(mw->FromWidgetToQGLW());
	return viewerWindow;
}

osgGA::CameraManipulator* GetCurrentCameraMainpulator()
{
	static osgGA::CameraManipulator* localCM = 0;
	MainWindow* mw = SysMainWindow::GetMainWindow();
	ViewerQT* viewerWindow = dynamic_cast<ViewerQT*>(mw->FromWidgetToQGLW());
	if(viewerWindow)
	{
		return viewerWindow->getViewer()->getCameraManipulator();
	}
	else
		return 0; 
}

osgGA::CameraManipulator* GetViewerCameraMainpulator()
{
	QVariant Pointer =  GetViwer()->property("g_pBackCameraMainpulator");

	osgGA::CameraManipulator* pBackCameraMainpulator = 0;
	if(Pointer.isValid())
		pBackCameraMainpulator = (osgGA::CameraManipulator*)(Pointer.toULongLong());

	return pBackCameraMainpulator;
}

void SetViewerCameraMainpulator(osgGA::CameraManipulator* pCM)
{
	GetViwer()->setProperty("g_pBackCameraMainpulator", QVariant((qlonglong)pCM));
}

DrawGeometryManipulator* SwitchCameraMainpulator()
{
	//////////////////////////////////////////////////////////////////////////
	osgGA::CameraManipulator* g_pBackCameraMainpulator = GetViewerCameraMainpulator();
	
	if(g_pBackCameraMainpulator)
	{
		DrawGeometryManipulator* pDGM = dynamic_cast<DrawGeometryManipulator*>(g_pBackCameraMainpulator);
		if(pDGM) return 0;
	}
	DrawGeometryManipulator* m_pDrawMainpulator = new DrawGeometryManipulator(0);

	m_pDrawMainpulator->setByMatrix(GetViwer()->getViewer()->getCameraManipulator()->getMatrix());
	osg::Vec4 backColor = GetViwer()->getViewer()->getCamera()->getClearColor();
	osg::Node* pNode = GetViwer()->getViewer()->getCameraManipulator()->getNode();
	osg::BoundingSphere bound = pNode->getBound();
	
	
	//osg::Node* pCanvasNode = m_pDrawMainpulator->CreateCanvas(bound.radius(), bound.center(), backColor/*osg::Vec4(1, 1, 1, 1)*/);
	
	//m_pDrawMainpulator->SetCanvasZ(-2);
	goto_gis::ManualDrawLayer* pLayer = new goto_gis::ManualDrawLayer(0, 0, 1);
	pLayer->LayerZ(20);
	//osg::Group* pLayerGroup = dynamic_cast<osg::Group*>(pLayer->GetLayerNode());
	/*if(pLayerGroup)
	pLayerGroup->addChild(pCanvasNode);
	else
	{
	delete pLayer;
	return 0;
	}*/
	goto_gis::Map* pMap = (goto_gis::Map*)GetViwer()->property("Map").toLongLong();//Map2D();
	goto_gis::Layer* poldLayer = pMap->GetLayer("·ûºÅ²ã");
	if(!poldLayer)
		pMap->AddLayer("·ûºÅ²ã", pLayer);

	GetViwer()->getViewer()->setCameraManipulator(m_pDrawMainpulator);

	//m_pDrawMainpulator->AddCanvasNode(pMap->GetRootNode());
	//m_pDrawMainpulator->AddCanvasNode(pMap->GetRootSwitch());
	//m_pDrawMainpulator->AddCanvasNode(pLayerGroup);
	//m_pDrawMainpulator->AddCanvasNode(pCanvasNode);
	return m_pDrawMainpulator;
	/////////////////////////////////////////////////////////////////////////
}

void DeleteCanvas()
{

}
ADD_MYACTION_CONTSTRUCTED_DECLARATION(HandAction)
ADD_MYACTION_CONTSTRUCTED_DECLARATION(DrawLineAction)
ADD_MYACTION_CONTSTRUCTED_DECLARATION(DrawCircleAction)
ADD_MYACTION_CONTSTRUCTED_DECLARATION(DrawRectAction)
ADD_MYACTION_CONTSTRUCTED_DECLARATION(DrawPolygonalLineAction)
ADD_MYACTION_CONTSTRUCTED_DECLARATION(DrawSymbol1Action)

ADD_MYACTION_CONTSTRUCTED_DECLARATION(DrawColdFrontAction)
ADD_MYACTION_CONTSTRUCTED_DECLARATION(DrawWarmFrontAction)
ADD_MYACTION_CONTSTRUCTED_DECLARATION(DrawOcclusionFrontAction)
ADD_MYACTION_CONTSTRUCTED_DECLARATION(DrawWindAction)
ADD_MYACTION_CONTSTRUCTED_DECLARATION(DeleteSymbolAction)
ADD_MYACTION_CONTSTRUCTED_DECLARATION(MoveSymbolAction)

void DrawPolygonalLineAction::MyAction()
{
	m_DrawPolyLine = new DrawPolygonalLine;
	DrawGeometryManipulator* pDGM = SwitchCameraMainpulator();

	if(!pDGM)
	{
		osgGA::CameraManipulator* g_pBackCameraMainpulator = GetViewerCameraMainpulator();
		DrawGeometryManipulator* pDGM = dynamic_cast<DrawGeometryManipulator*>(g_pBackCameraMainpulator);
		if(pDGM)
			pDGM->SetDrawGeometryData(m_DrawPolyLine);
	}
	else
	{
		SetViewerCameraMainpulator(pDGM);
		pDGM->SetDrawGeometryData(m_DrawPolyLine);
	}
	
	m_DrawPolyLine->SetColor(osg::Vec4(0, 1, 0, 1));
	goto_gis::Map* pMap = (goto_gis::Map*)GetViwer()->property("Map").toLongLong();;
	m_DrawPolyLine->SetDrawLayer(pMap->GetLayer("·ûºÅ²ã"));
}

void DrawPolygonalLineAction::InitMyAction()
{
	
}

void DrawRectAction::MyAction()
{
	m_DrawRect = new DrawRect;
	DrawGeometryManipulator* pDGM = SwitchCameraMainpulator();
	if(!pDGM)
	{
		osgGA::CameraManipulator* g_pBackCameraMainpulator = GetViewerCameraMainpulator();
		DrawGeometryManipulator* pDGM = dynamic_cast<DrawGeometryManipulator*>(g_pBackCameraMainpulator);
		if(pDGM)
			pDGM->SetDrawGeometryData(m_DrawRect);
	}
	else
	{
		SetViewerCameraMainpulator(pDGM);
		pDGM->SetDrawGeometryData(m_DrawRect);
	}
	m_DrawRect->SetColor(osg::Vec4(1, 1, 1, 1));
	goto_gis::Map* pMap = (goto_gis::Map*)GetViwer()->property("Map").toLongLong();//Map2D();
	m_DrawRect->SetDrawLayer(pMap->GetLayer("·ûºÅ²ã"));
}

void DrawRectAction::InitMyAction()
{
	
}

void DrawCircleAction::MyAction()
{
	m_DrawCircle = new DrawCircle;
	DrawGeometryManipulator* pDGM = SwitchCameraMainpulator();
	if(!pDGM)
	{
		osgGA::CameraManipulator* g_pBackCameraMainpulator = GetViewerCameraMainpulator();
		DrawGeometryManipulator* pDGM = dynamic_cast<DrawGeometryManipulator*>(g_pBackCameraMainpulator);
		if(pDGM)
			pDGM->SetDrawGeometryData(m_DrawCircle);
	}
	else
	{
		SetViewerCameraMainpulator(pDGM);
		pDGM->SetDrawGeometryData(m_DrawCircle);
	}
	m_DrawCircle->SetColor(osg::Vec4(0, 1, 0, 1));
	goto_gis::Map* pMap = (goto_gis::Map*)GetViwer()->property("Map").toLongLong();//Map2D();
	m_DrawCircle->SetDrawLayer(pMap->GetLayer("·ûºÅ²ã"));
}

void DrawCircleAction::InitMyAction()
{
	
}

void DrawLineAction::MyAction()
{
	m_DrawLine = new DrawLine;
	DrawGeometryManipulator* pDGM = SwitchCameraMainpulator();
	if(!pDGM)
	{
		osgGA::CameraManipulator* g_pBackCameraMainpulator = GetViewerCameraMainpulator();
		DrawGeometryManipulator* pDGM = dynamic_cast<DrawGeometryManipulator*>(g_pBackCameraMainpulator);
		if(pDGM)
			pDGM->SetDrawGeometryData(m_DrawLine);
	}
	else
	{
		SetViewerCameraMainpulator(pDGM);
		pDGM->SetDrawGeometryData(m_DrawLine);
	}
	m_DrawLine->SetColor(osg::Vec4(0, 1, 0, 1));
	goto_gis::Map* pMap = (goto_gis::Map*)GetViwer()->property("Map").toLongLong();//Map2D();
	m_DrawLine->SetDrawLayer(pMap->GetLayer("·ûºÅ²ã"));
}

void DrawLineAction::InitMyAction()
{
	
}

void HandAction::MyAction()
{
	DefaultManipulator* pDM = new DefaultManipulator;
	//pDM->setAutoComputeHomePosition(false);
	SetViewerCameraMainpulator(0);
	GetViwer()->getViewer()->setCameraManipulator(pDM);
	//pDM->setByMatrix(g_pBackCameraMainpulator->getMatrix());
}

void HandAction::InitMyAction()
{

}

void DrawSymbol1Action::MyAction()
{
	QImage image("C:\\Windows\\winsxs\\x86_microsoft-windows-g..ets-slideshowgadget_31bf3856ad364e35_6.1.7600.16385_none_253e8c58002c48e1\\logo.png");
	m_DrawSymbol = new DrawSymbol;
	m_DrawSymbol->SetPixmap(image);
	DrawGeometryManipulator* pDGM = SwitchCameraMainpulator();
	if(!pDGM)
	{
		osgGA::CameraManipulator* g_pBackCameraMainpulator = GetViewerCameraMainpulator();
		DrawGeometryManipulator* pDGM = dynamic_cast<DrawGeometryManipulator*>(g_pBackCameraMainpulator);
		if(pDGM)
			pDGM->SetDrawGeometryData(m_DrawSymbol);
	}
	else
	{
		SetViewerCameraMainpulator(pDGM);
		pDGM->SetDrawGeometryData(m_DrawSymbol);
	}
	m_DrawSymbol->SetColor(osg::Vec4(0, 1, 0, 1));
	goto_gis::Map* pMap = (goto_gis::Map*)GetViwer()->property("Map").toLongLong();//Map2D();
	m_DrawSymbol->SetDrawLayer(pMap->GetLayer("·ûºÅ²ã"));
}

void DrawSymbol1Action::InitMyAction()
{

}

void DrawColdFrontAction::InitMyAction()
{
	m_ColdFront.SetLineAlgorithmWidth(5000);
}

void DrawColdFrontAction::MyAction()
{
	m_DrawPolyLine = new DrawPolygonalLine;
	m_DrawPolyLine->SetAlgorithm(&m_ColdFront);
	DrawGeometryManipulator* pDGM = SwitchCameraMainpulator();

	if(!pDGM)
	{
		osgGA::CameraManipulator* g_pBackCameraMainpulator = GetViewerCameraMainpulator();
		DrawGeometryManipulator* pDGM = dynamic_cast<DrawGeometryManipulator*>(g_pBackCameraMainpulator);
		if(pDGM)
			pDGM->SetDrawGeometryData(m_DrawPolyLine);
	}
	else
	{
		SetViewerCameraMainpulator(pDGM);
		pDGM->SetDrawGeometryData(m_DrawPolyLine);
	}

	m_DrawPolyLine->SetColor(osg::Vec4(0, 1, 0, 1));
	goto_gis::Map* pMap = (goto_gis::Map*)GetViwer()->property("Map").toLongLong();;
	m_DrawPolyLine->SetDrawLayer(pMap->GetLayer("·ûºÅ²ã"));
}

void DrawWarmFrontAction::InitMyAction()
{
	m_WarmFront.SetLineAlgorithmWidth(5000);
}

void DrawWarmFrontAction::MyAction()
{
	m_DrawPolyLine = new DrawPolygonalLine;
	m_DrawPolyLine->SetAlgorithm(&m_WarmFront);
	DrawGeometryManipulator* pDGM = SwitchCameraMainpulator();

	if(!pDGM)
	{
		osgGA::CameraManipulator* g_pBackCameraMainpulator = GetViewerCameraMainpulator();
		DrawGeometryManipulator* pDGM = dynamic_cast<DrawGeometryManipulator*>(g_pBackCameraMainpulator);
		if(pDGM)
			pDGM->SetDrawGeometryData(m_DrawPolyLine);
	}
	else
	{
		SetViewerCameraMainpulator(pDGM);
		pDGM->SetDrawGeometryData(m_DrawPolyLine);
	}

	m_DrawPolyLine->SetColor(osg::Vec4(0, 1, 0, 1));
	goto_gis::Map* pMap = (goto_gis::Map*)GetViwer()->property("Map").toLongLong();;
	m_DrawPolyLine->SetDrawLayer(pMap->GetLayer("·ûºÅ²ã"));
}

void DrawOcclusionFrontAction::InitMyAction()
{
	m_OcclusionFront.SetLineAlgorithmWidth(5000);
}

void DrawOcclusionFrontAction::MyAction()
{
	m_DrawPolyLine = new DrawPolygonalLine;
	m_DrawPolyLine->SetAlgorithm(&m_OcclusionFront);
	DrawGeometryManipulator* pDGM = SwitchCameraMainpulator();

	if(!pDGM)
	{
		osgGA::CameraManipulator* g_pBackCameraMainpulator = GetViewerCameraMainpulator();
		DrawGeometryManipulator* pDGM = dynamic_cast<DrawGeometryManipulator*>(g_pBackCameraMainpulator);
		if(pDGM)
			pDGM->SetDrawGeometryData(m_DrawPolyLine);
	}
	else
	{
		SetViewerCameraMainpulator(pDGM);
		pDGM->SetDrawGeometryData(m_DrawPolyLine);
	}

	m_DrawPolyLine->SetColor(osg::Vec4(0, 1, 0, 1));
	goto_gis::Map* pMap = (goto_gis::Map*)GetViwer()->property("Map").toLongLong();;
	m_DrawPolyLine->SetDrawLayer(pMap->GetLayer("·ûºÅ²ã"));
}

void DrawWindAction::InitMyAction()
{
	m_iWindSpeed = 5;
	m_DrawWind = new DrawWindSymbol(m_iWindSpeed, 4.0);
}

void DrawWindAction::MyAction()
{
	m_DrawWind->SetWindSpeed(m_iWindSpeed);

	DrawGeometryManipulator* pDGM = SwitchCameraMainpulator();

	if(!pDGM)
	{
		osgGA::CameraManipulator* g_pBackCameraMainpulator = GetViewerCameraMainpulator();
		DrawGeometryManipulator* pDGM = dynamic_cast<DrawGeometryManipulator*>(g_pBackCameraMainpulator);
		if(pDGM)
			pDGM->SetDrawGeometryData(m_DrawWind);
	}
	else
	{
		SetViewerCameraMainpulator(pDGM);
		pDGM->SetDrawGeometryData(m_DrawWind);
	}

	m_DrawWind->SetColor(osg::Vec4(0, 1, 0, 1));
	goto_gis::Map* pMap = (goto_gis::Map*)GetViwer()->property("Map").toLongLong();;
	m_DrawWind->SetDrawLayer(pMap->GetLayer("·ûºÅ²ã"));
}

void DeleteSymbolAction::InitMyAction()
{

}

void DeleteSymbolAction::MyAction()
{
	osg::ref_ptr<DeleteSymbol> pDeleteSymbol = new DeleteSymbol;
	pDeleteSymbol->DeleteSymbolByName("Symbol_Image");

	DrawGeometryManipulator* pDGM = SwitchCameraMainpulator();

	if(!pDGM)
	{
		osgGA::CameraManipulator* g_pBackCameraMainpulator = GetViewerCameraMainpulator();
		DrawGeometryManipulator* pDGM = dynamic_cast<DrawGeometryManipulator*>(g_pBackCameraMainpulator);
		if(pDGM)
			pDGM->SetDrawGeometryData(pDeleteSymbol);
	}
	else
	{
		SetViewerCameraMainpulator(pDGM);
		pDGM->SetDrawGeometryData(pDeleteSymbol);
	}

	goto_gis::Map* pMap = (goto_gis::Map*)GetViwer()->property("Map").toLongLong();;
	pDeleteSymbol->SetDrawLayer(pMap->GetLayer("·ûºÅ²ã"));
}

void MoveSymbolAction::InitMyAction()
{

}

void MoveSymbolAction::MyAction()
{
	osg::ref_ptr<MoveSymbol> pMoveSymbol = new MoveSymbol;
	pMoveSymbol->MoveSymbolByName("Symbol_Image");

	DrawGeometryManipulator* pDGM = SwitchCameraMainpulator();

	if(!pDGM)
	{
		osgGA::CameraManipulator* g_pBackCameraMainpulator = GetViewerCameraMainpulator();
		DrawGeometryManipulator* pDGM = dynamic_cast<DrawGeometryManipulator*>(g_pBackCameraMainpulator);
		if(pDGM)
			pDGM->SetDrawGeometryData(pMoveSymbol);
	}
	else
	{
		SetViewerCameraMainpulator(pDGM);
		pDGM->SetDrawGeometryData(pMoveSymbol);
	}

	goto_gis::Map* pMap = (goto_gis::Map*)GetViwer()->property("Map").toLongLong();;
	pMoveSymbol->SetDrawLayer(pMap->GetLayer("·ûºÅ²ã"));
}