#include "CipasComposerLegend.h"
#include "CipasComposerLegendItem.h"
#include "../XMLProvider/RadReadColor_XML.h"
#include "CipasComposerLegendLayersDialog.h"


CCipasComposerLegendLayersDialog::CCipasComposerLegendLayersDialog(CCipasComposerLegend* mLegend, QWidget* parent):QDialog(parent)
{
	setupUi(this);
	lineEditPix->setReadOnly(true);
	 /*P_SymbolLegend_Vector m_symbolLegend_v = mLegend->_SymbolLegend_Vector();
	 int count = (*m_symbolLegend_v).size();
	 for (int i = 0 ; i<count; i++)
	 {
		 P_SymbolLegend_Product cp = m_symbolLegend_v->at(i);
		 QListWidgetItem* item = new QListWidgetItem();
		 item->setText(QString::fromStdString(cp->labelText));
		 item->setIcon(QIcon(cp->pixmap));
		 listWidget->addItem(item);
	 }*/
	
	//QList<Layer*>::iterator layerIt = layers.begin();
	//for ( ; layerIt != layers.end(); ++layerIt )
	//{
	//	QListWidgetItem* item = new QListWidgetItem(QString::fromStdString(( *layerIt )->LayerName()), listMapLayers );
	//	mItemLayerMap.insert( item, *layerIt );
	//}
}


CCipasComposerLegendLayersDialog::~CCipasComposerLegendLayersDialog(void)
{
}


//Layer* CCipasComposerLegendLayersDialog::selectedLayer()
//{
//	QListWidgetItem* item = listMapLayers->currentItem();
//	if ( !item )
//	{
//		return 0;
//	}
//
//	QMap<QListWidgetItem*, Layer*>::iterator it = mItemLayerMap.find( item );
//	Layer* c = 0;
//	c = it.value();
//	return c;
//}

void CCipasComposerLegendLayersDialog::on_pushButton_clicked()
{
	QString fileName = QFileDialog::getOpenFileName(this,tr("选择图片"), "/", tr("图片文件 (*.png *.jpg)"));
	if (fileName.isEmpty())
	{
		return;
	}
	lineEditPix->setText(fileName);
}

void CCipasComposerLegendLayersDialog::on_pushButtonAdd_clicked()
{
	if (lineEditTitle->text().isEmpty())
	{
		QMessageBox::information(this,tr("提示"),tr("图例文字不能为空！"));
		return;
	}
	if (lineEditPix->text().isEmpty())
	{
		QMessageBox::information(this,tr("提示"),tr("图例图片不能为空！"));
		return;
	}
	QIcon icon;
	icon.addFile(lineEditPix->text(), QSize(), QIcon::Normal, QIcon::Off);
	QListWidgetItem *__qlistwidgetitem = new QListWidgetItem(listWidget);
	__qlistwidgetitem->setIcon(icon);
	__qlistwidgetitem->setText(lineEditTitle->text());

	listWidget->addItem(__qlistwidgetitem);
}

void CCipasComposerLegendLayersDialog::on_pushButtonDel_clicked()
{
	if (listWidget->currentRow() != -1)
	{
		QListWidgetItem *__qlistwidgetitem =listWidget->takeItem(listWidget->currentRow());
		delete __qlistwidgetitem;
		__qlistwidgetitem = 0;
	}
}
