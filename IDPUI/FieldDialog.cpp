#include <QDebug>
#include <osg/ValueObject>
#include <QTableWidgetItem>
#include "FieldDialog.h"

FieldDialog::FieldDialog(Layer* layer ,QWidget *parent): QDialog(parent)
{
	ui.setupUi(this);
	InitDialog(layer);
	setWindowFlags(windowFlags() | Qt::WindowSystemMenuHint|Qt::WindowMinMaxButtonsHint);
}

FieldDialog::~FieldDialog()
{

}

void FieldDialog::InitDialog(Layer* layer)
{
	ui.gridLayout->setContentsMargins(1,0,0,0);
	ui.gridLayout_2->setContentsMargins(0,0,0,0);

	ui.tableWidget->setSelectionBehavior (QAbstractItemView::SelectRows); //设置选择行为，以行为单位
	ui.tableWidget->setSelectionMode (QAbstractItemView::SingleSelection); //设置选择模式，选择单行
	ui.tableWidget->horizontalHeader()->setDefaultAlignment(Qt::AlignHCenter);
	ui.tableWidget->setTextElideMode(Qt::ElideMiddle);
	ui.tableWidget->setAlternatingRowColors(true);
	ui.tableWidget->horizontalHeader()->setHighlightSections(false);
	ui.tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);

	ui.tableWidget->setColumnWidth(0,100);
	ui.tableWidget->setColumnWidth(1,100);
	ui.tableWidget->setColumnWidth(2,100);
	ui.tableWidget->clearContents();

	std::map<int, GeomField>* mapFields = layer->GetFieldMap();
	int rowCount = mapFields->size();
	int columnCount = 0;

	

	

	int ic = 0;
	int i = 0;
	int irow = 0;
	std::map<int, GeomField>::iterator fieldIter = mapFields->begin();
	while(fieldIter != mapFields->end())
	{
		GeomField geomF = fieldIter->second;//->at(i);//
		
		if (i == 0)
		{
			columnCount = geomF.length(); //10;//
			ui.tableWidget->setRowCount(rowCount/columnCount);
			ui.tableWidget->setColumnCount(columnCount);
		}

		//QString typeName = QString::fromStdString(geomF.typeName());
		QString name = QString::fromStdString(geomF.name());
		QString comment = QString::fromStdString(geomF.comment());
		
		
		//QTableWidgetItem *item0 = new QTableWidgetItem(typeName);
		QTableWidgetItem *item1 = new QTableWidgetItem(name);
		QTableWidgetItem *item2 = new QTableWidgetItem(comment);

		//item0->setTextAlignment(Qt::AlignCenter);
		item1->setTextAlignment(Qt::AlignCenter);
		item2->setTextAlignment(Qt::AlignCenter);
		bool be = false;
		for (int  k =0 ; k< columnCount ; k++)
		{
			if(ui.tableWidget->horizontalHeaderItem(k))
			{
				if (ui.tableWidget->horizontalHeaderItem(k)->text() == name)
				{
					be = true;
					break;
				}
			}
		}
		if( !be)//小于列数 直接加标题栏
		{
			ui.tableWidget->setHorizontalHeaderItem(ic,item1);
		}
		else if(i%columnCount == 0)
		{
			ic = 0;
			irow++;
		}
		ui.tableWidget->setItem(irow, ic, item2);
		ic++;
		i++;
		fieldIter++;
	}
}
