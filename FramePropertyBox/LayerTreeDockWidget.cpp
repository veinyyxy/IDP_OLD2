#include "StdAfx.h"
#include "LeftDockWidget.h"
#include "LayerTreeDockWidget.h"
#include "FrameModel.h"
#include "Controller.h"
#include "TreeViewEventInterface.h"
#include "MainControlFrame.h"
#include "ControllerFrameContext.h"

goto_gis::Map* GetCurentMap()
{
	MainControlFrame* pMCF = MainControlFrame::GetMainControlFrameInstance();
	ControllerFrameContext* pCFC = pMCF->GetCurrentContext();
	goto_gis::Map* pMap = pCFC->GetCurrentMap();
	return pMap;
}

LayerTreeDockWidget::LayerTreeDockWidget(void)
{
	m_pLeftDockWidget = new LeftDockWidget(0);
	m_pLeftDockWidget->SetUpdateInterface(this);
}


LayerTreeDockWidget::~LayerTreeDockWidget(void)
{
}

QDockWidget* LayerTreeDockWidget::GetDockWidget()
{
	return m_pLeftDockWidget;
}

Qt::DockWidgetArea LayerTreeDockWidget::GetDockArea()
{
	return Qt::LeftDockWidgetArea;
}

void LayerTreeDockWidget::SetController( Controller* pFM, const QDomNode& discribNode )
{
	QDomNodeList nodeList = discribNode.childNodes();
	for(int i = 0; i < nodeList.size(); i++)
	{
		QDomNode childNode = nodeList.at(i);
		QString strNodeName = childNode.nodeName();
		QDomElement domElement;
		FrameModel* pFrameMoel = pFM->Output();
		if(strNodeName == "Model")
		{
			domElement = childNode.toElement();
			boost::any* pAnyType = pFrameMoel->GetParameter("type");
			std::string strType = boost::any_cast<std::string>(*pAnyType);

			QString qstrType;
			qstrType = qstrType.fromStdString(strType);
			if(domElement.attribute("type") == qstrType)
			{
				QString strModelName = domElement.attribute("name");
				boost::any* pAny = pFrameMoel->GetInterface(strModelName.toStdString());

				m_TreeViewEventInterface = boost::any_cast<TreeViewEventInterface*>(*pAny);

				m_pLeftDockWidget->SetModel(m_TreeViewEventInterface);
			}
		}
	}


}

void LayerTreeDockWidget::AddContextMenuAction( const QString& strName, MyActionInterface* pAction)
{
	m_pLeftDockWidget->AddContextMenuAction(pAction);
	m_ActionsMap.insert(strName, pAction);
}

void LayerTreeDockWidget::AddToolsBarAction( const QString& strName, MyActionInterface* pAction )
{
	m_pLeftDockWidget->AddToolsBarAction(pAction);
	m_ActionsMap.insert(strName, pAction);
}

void LayerTreeDockWidget::DelContextMenuAction( const QString& strName )
{
	m_ActionsMap.remove(strName);
}

MyActionInterface* LayerTreeDockWidget::GetContextMenuAction( const QString& strName )
{
	return m_ActionsMap.value(strName);
}

void LayerTreeDockWidget::DelToolsBarAction( const QString& strName )
{
	m_ActionsMap.remove(strName);
}

MyActionInterface* LayerTreeDockWidget::GetToolsBarAction( const QString& strName )
{
	return m_ActionsMap.value(strName);
}

void LayerTreeDockWidget::UpdateSlave()
{
	Update();
}

void LayerTreeDockWidget::Update()
{
	QAbstractItemModel* pStandardItemModel = m_TreeViewEventInterface->CreateModel();
	m_pLeftDockWidget->SetModel(pStandardItemModel);
}
