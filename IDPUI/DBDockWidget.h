#ifndef DBDOCKWIDGET_H
#define DBDOCKWIDGET_H

#include <QtGui>
#include "ui_DBDockWidget.h"
#include "idpui_global.h"
#include "IDPdb.h"

class SqliteManager;

class IDPUI_EXPORT DBDockWidget : public QDockWidget
{
	Q_OBJECT

public:
	DBDockWidget(QWidget *parent = 0);
	~DBDockWidget();
	public Q_SLOTS:
		void Slot_AreaCurrentIndexChanged_S(int index);
		void Slot_CmbCurrentIndexChanged_S(int index);
		void Slot_CmbDayCurrentIndexChanged_S(int index);
		void Slot_CmbHourCurrentIndexChanged_S(int index);
		void Slot_DateTimeChanged_S( const QDateTime & datetime );
		void Slot_Chbclicked_S();
		void Slot_ItemDoubleClicked_S(QTableWidgetItem* item);
		//////////////////////////////////////////////////////////////////////////
		void Slot_CmbCurrentIndexChanged_M(int index);
		void Slot_CmbDayCurrentIndexChanged_M(int index);
		void Slot_CmbHourCurrentIndexChanged_M(int index);
		void Slot_DateTimeChanged_M( const QDateTime & datetime );
		void Slot_Chbclicked_M();
		void Slot_ItemDoubleClicked_M(QTableWidgetItem* item);
private:
	void InitWidget();
	void InitWidgetDB();
	void InitDBManager();
	void InitSlost();
	void AddTableWidgetItem_S(QString path,QString date , QString time);
	void AddTableWidgetItem_M(QString path,QString date , QString time);
private:
	Ui::DBDockWidget ui;
	SqliteManager* m_pSqliteManager;
};

#endif // DBDOCKWIDGET_H
