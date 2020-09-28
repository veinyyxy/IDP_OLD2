#include "StdAfx.h"
#include "Layer.h"
#include "Map.h"
#include "ControllerFrameContext.h"
#include "MainControlFrame.h"
#include "TreeViewEventImps.h"


TreeViewEventImps::TreeViewEventImps(void)
{
}


TreeViewEventImps::~TreeViewEventImps(void)
{
}

void TreeViewEventImps::ItemClick(const QModelIndex& index)
{
	try
	{
		//////////////////////////////////////////////////////////////////////////
		//const QAbstractItemModel* pStandardItemModel = index.model();
		
		QStandardItem *currentItem = m_StandardItemModel.itemFromIndex(index);

		QString m_strLayerName = currentItem->text();
		if (currentItem == NULL)
		{
			return;
		}
		
		goto_gis::Map* pMap = GetCurentMap();

		if (pMap == NULL)
		{
			return;
		}
		pMap->CurrentLayer(pMap->GetLayer( currentItem->text().toStdString()));

		//currentItem->setCheckState(0,Qt::Unchecked);
		//QString layerName = currentItem->text();
		//pMap->SetVisibleLayer(layerName.toStdString(),item->checkState(0));
	}
	catch (...)
	{

	}
}

void TreeViewEventImps::ShowContextMenu( const QPoint& point )
{

}

void TreeViewEventImps::ItemDoubleClicked(const QModelIndex& index)
{
	/*try
	{
	QStandardItem *currentItem = m_StandardItemModel.itemFromIndex(index);
	goto_gis::Map* pMap = GetCurentMap();
	QString winType = "";
	QTreeWidgetItem * m_pProuductLayerTreeItem = 0;
	if(m_pProuductLayerTreeItem == item->parent())
	{
	QString layerName = 0;
	layerName = item->text(0);
	if (layerName.isEmpty() || layerName.isNull())
	{
	return;
	}
	goto_gis::Layer* pLayer =0;
	pLayer = pMap->GetLayer(layerName.toStdString());
	if (pLayer == NULL)
	{
	return;
	}
	boost::any AttriValue;
	if(!(pLayer->GetLayerAtrributeFromName(QString::fromLocal8Bit("中心：").toStdString(), AttriValue))) return;
	osg::Vec3 v3Central = boost::any_cast<osg::Vec3>(AttriValue);

	pMap->Site(v3Central);
	}
	}
	catch(...)
	{

	}*/
}

goto_gis::Map* TreeViewEventImps::GetCurentMap()
{
	MainControlFrame* pMCF = MainControlFrame::GetMainControlFrameInstance();
	ControllerFrameContext* pCFC = pMCF->GetCurrentContext();
	goto_gis::Map* pMap = pCFC->GetCurrentMap();
	return pMap;
}

QAbstractItemModel* TreeViewEventImps::CreateModel()
{
	m_StandardItemModel.clear();
	m_StandardItemModel.setColumnCount(2);

	m_StandardItemModel.setHeaderData(0, Qt::Horizontal
		, QString::fromLocal8Bit("可见"));

	m_StandardItemModel.setHeaderData(1, Qt::Horizontal
		, QString::fromLocal8Bit("图层名"));

	goto_gis::Map* pMap = GetCurentMap();
	std::map<std::string, goto_gis::Layer*> allLayers = pMap->GetMapLayers();

	std::map<std::string, goto_gis::Layer*>::iterator ib = allLayers.begin();
	int i = 0;
	for(ib; ib != allLayers.end(); ib++, i++)
	{
		QString strLayerName = QString::fromStdString(ib->first);
		QStandardItem* pSI = m_StandardItemModel.invisibleRootItem();
		if(pSI)
		{
			QStandardItem* pChildItem = new QStandardItem(strLayerName);
			//pChildItem->setCheckable(true);
			QIcon icon1(":/images/MapLayer/show.png");
			QIcon icon2(":/images/MapLayer/hide.png");
			QStandardItem* pCheckWidget = 0;
			if(ib->second->Visible())
			{
				pCheckWidget = new QStandardItem(icon1,"");
			}
			else
			{
				pCheckWidget = new QStandardItem(icon2,"");
			}
			//pCheckWidget->setCheckable(true);
			pSI->setChild(i, 0, pCheckWidget);
			pSI->setChild(i, 1, pChildItem);
		}
		//ib->second;
	}

	return &m_StandardItemModel;
}
