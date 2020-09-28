#ifndef CENTRALMDIAREA_H
#define CENTRALMDIAREA_H
/** @file 
* @brief   客户区域控件头文件
* @author    易家祥
* @date      2013年3月
* @version   version
  @note      2013年 6月
*/
#include <QtGui>
#include "ui_CentralMdiArea.h"
#include "idpui_global.h"

class MainWindow;

/** @class   CentralMdiArea
    @brief   客户区域控件
    @author  易家祥
    @note    客户区域控件的对外接口
*/ 
class IDPUI_EXPORT  CentralMdiArea : public QMdiArea
{
	Q_OBJECT

public:
	CentralMdiArea(QWidget *parent = 0);
	~CentralMdiArea();
	inline QWidget* CurrentWidget() {return _currentWidget;}
	inline void CurrentWidget(QWidget* cw) { _currentWidget = cw;}

	QMdiSubWindow* CreateSubWindow(QWidget* widget , Qt::WindowFlags flags = 0);
protected:
	QMdiSubWindow *addSubWindow(QWidget *widget, Qt::WindowFlags flags = 0);
public:
	/** @brief 控件对应槽链接 */ 
	void InitSlots();
public Q_SLOTS:
	void	Slot_SubWindowActivated ( QMdiSubWindow * window );
	void Slot_ActivateWindow(QWidget* currentWidget);
private:
	Ui::CentralMdiArea ui;
	QWidget* _currentWidget;
	MainWindow* m_pMW;
};

#endif // CENTRALMDIAREA_H
