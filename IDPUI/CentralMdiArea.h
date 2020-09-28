#ifndef CENTRALMDIAREA_H
#define CENTRALMDIAREA_H
/** @file 
* @brief   �ͻ�����ؼ�ͷ�ļ�
* @author    �׼���
* @date      2013��3��
* @version   version
  @note      2013�� 6��
*/
#include <QtGui>
#include "ui_CentralMdiArea.h"
#include "idpui_global.h"

class MainWindow;

/** @class   CentralMdiArea
    @brief   �ͻ�����ؼ�
    @author  �׼���
    @note    �ͻ�����ؼ��Ķ���ӿ�
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
	/** @brief �ؼ���Ӧ������ */ 
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
