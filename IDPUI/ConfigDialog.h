#ifndef CONFIGDIALOG_H
#define CONFIGDIALOG_H
/** @file 
* @brief   系统配置类头文件
* @author    易家祥
* @date      2013年3月
* @version   version
  @note      2013年 6月
*/
#include <QtGui>
#include "ui_configdialog.h"

#include "idpui_global.h"
/** @class   ConfigDialog
    @brief   系统配置类头文件
    @author  易家祥
    @note   系统配置类的对外接口
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
