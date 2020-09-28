#ifndef MONITORDIALOG_H
#define MONITORDIALOG_H

#include <QtGui>
#include <QMutex>
#include "ui_MonitorDialog.h"

class MonitorThread;

class MonitorDialog : public QDialog
{
	Q_OBJECT

public:
	MonitorDialog(QWidget *parent = 0, Qt::WFlags flags = 0);
	~MonitorDialog();

protected:
	void timerEvent( QTimerEvent *event );
private:
	void InitConfig();
	void CreateActions();
	void CreateTrayIcon();
private:
	Ui::MonitorDialogClass ui;

	QPointer<QSystemTrayIcon> m_pTrayIcon;
	QPointer<QMenu> m_pTrayIconMenu;

	QAction *m_pMinimizeAction;
	QAction *m_pRestoreAction;
	QAction *m_pQuitAction;

	QMutex m_lock;

	//QString m_isTimer;
	QString m_time;
	QString m_sfsPath;
	MonitorThread* mThread;
};

#endif // MONITORDIALOG_H
