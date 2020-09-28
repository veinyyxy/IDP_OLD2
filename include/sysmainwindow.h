/*============================================================ 
文件名：sysmainwindow.h
类 名： SysMainWindow
父 类： QObject
子 类： 无 
功能说明：得到主窗口工具类
调用说明：
1、；
2、；

----------------------------版本更新-------------------------
V 1.0 
原作者 ：易家祥
完成日期：2012年 6月11日
修改作者：
完成日期：2012年 6月11日
============================================================*/

#ifndef  SYSMAINWINDOW_H
#define SYSMAINWINDOW_H
//QT 类
#include <QtCore/QCoreApplication>

//自定义类
#include "mainwindow.h"

class  SysMainWindow : public QObject
{
public:
	/************************************************************************
	 功能说明：设置主窗口wid
	 参    数：wid 主窗口 wid
	 作    者：易家祥
	 完成日期：2012年06月11日
	************************************************************************/
	static void SetMainWindow(uint wid)
	{
		qApp->setProperty("WId",QVariant(wid));
	}

	/************************************************************************
	 功能说明：得到主窗口wid
	 作    者：易家祥
	 完成日期：2012年06月11日
	************************************************************************/
	static MainWindow* GetMainWindow()
	{
		WId wid = (WId)qApp->property("WId").toUInt();
		QWidget* widget = QWidget::find(wid);
		return dynamic_cast<MainWindow*>(widget);
	}
};
#endif // SYSMAINWINDOW_H