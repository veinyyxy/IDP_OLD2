#ifndef DATADOCKWIDGET_H
#define DATADOCKWIDGET_H

#include <QtGui>
#include "ui_DataDockWidget.h"
#include "idpui_global.h"
#include "IDPdb.h"

class TsCallArithms;

class IDPUI_EXPORT DataDockWidget : public QDockWidget
{
	Q_OBJECT

public:
	DataDockWidget(QWidget *parent = 0);
	~DataDockWidget();
private:	
	void InitDepArithmsMap();
	void InitSlots();
	void InitConfig();
public Q_SLOTS:
	virtual void setVisible(bool visible);
	void Slot_PushButtonClicked();
	void	Slot_DoubleClicked ( const QModelIndex & index );
private:
	Ui::DataDockWidget ui;
	QFileSystemModel* m_pFileSystemModel;
	QString m_defaultReadPath;
	QString m_defaultSavePath;
	QStringList m_defaultFilters;

	//////////////////////////////////////////////////////////////////////////
	QMap<QString, QString> m_DepArithmsMapStr;
	TsCallArithms* m_pCallArithms;
};

#endif // DATADOCKWIDGET_H
