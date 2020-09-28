#ifndef DBZDIALOG_H
#define DBZDIALOG_H
/** @file 
* @brief   地层计算类头文件
* @author    易家祥
* @date      2013年3月
* @version   version
  @note      2013年 6月
*/
#include <QDialog>
#include "ui_DBZDialog.h"
#include "idpui_global.h"
/** @class   DB2Dialog
    @brief   地层计算类头文件
    @author  易家祥
    @note   地层计算类的对外接口
*/

class IsoUserData : public QObjectUserData
{
public:
	IsoUserData();
	void ComputerIsoSurface(int iValue);
	void DeleteIsoSuerface();
private:
	bool m_computed;
	int m_iValue;
};

class IDPUI_EXPORT  DBZDialog : public QDialog
{
	Q_OBJECT

public:
	DBZDialog(int mix , int max ,QWidget *parent = 0);
	~DBZDialog();
private:
	void InitDialog();
	void InitSlots();

protected Q_SLOTS:
		void Slots_PushButtonClicked();
private:
	Ui::DBZDialog ui;
	static int gRowCount;
	QIntValidator* m_intValidator;
};

#endif // DB2DIALOG_H
