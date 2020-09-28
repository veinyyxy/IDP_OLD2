#ifndef CONFIGDIALOG_H
#define CONFIGDIALOG_H
/** @file 
* @brief   ϵͳ������ͷ�ļ�
* @author    �׼���
* @date      2013��3��
* @version   version
  @note      2013�� 6��
*/
#include <QtGui>
#include "ui_configdialog.h"

#include "idpui_global.h"
/** @class   ConfigDialog
    @brief   ϵͳ������ͷ�ļ�
    @author  �׼���
    @note   ϵͳ������Ķ���ӿ�
*/ 
class IDPUI_EXPORT  ConfigDialog : public QDialog
{
	Q_OBJECT

public:
	ConfigDialog(QWidget *parent = 0);
	~ConfigDialog();

private:
	void InitSkin(QString skinPath);

protected Q_SLOTS:
		void Slot_ButtonClicked();
		void Slot_ChangePage(QListWidgetItem *current, QListWidgetItem *previous);
private:
	Ui::ConfigDialog ui;
};

#endif // CONFIGDIALOG_H
