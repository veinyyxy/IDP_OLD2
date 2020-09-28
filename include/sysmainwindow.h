/*============================================================ 
�ļ�����sysmainwindow.h
�� ���� SysMainWindow
�� �ࣺ QObject
�� �ࣺ �� 
����˵�����õ������ڹ�����
����˵����
1����
2����

----------------------------�汾����-------------------------
V 1.0 
ԭ���� ���׼���
������ڣ�2012�� 6��11��
�޸����ߣ�
������ڣ�2012�� 6��11��
============================================================*/

#ifndef  SYSMAINWINDOW_H
#define SYSMAINWINDOW_H
//QT ��
#include <QtCore/QCoreApplication>

//�Զ�����
#include "mainwindow.h"

class  SysMainWindow : public QObject
{
public:
	/************************************************************************
	 ����˵��������������wid
	 ��    ����wid ������ wid
	 ��    �ߣ��׼���
	 ������ڣ�2012��06��11��
	************************************************************************/
	static void SetMainWindow(uint wid)
	{
		qApp->setProperty("WId",QVariant(wid));
	}

	/************************************************************************
	 ����˵�����õ�������wid
	 ��    �ߣ��׼���
	 ������ڣ�2012��06��11��
	************************************************************************/
	static MainWindow* GetMainWindow()
	{
		WId wid = (WId)qApp->property("WId").toUInt();
		QWidget* widget = QWidget::find(wid);
		return dynamic_cast<MainWindow*>(widget);
	}
};
#endif // SYSMAINWINDOW_H