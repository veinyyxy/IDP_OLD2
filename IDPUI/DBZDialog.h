#ifndef DBZDIALOG_H
#define DBZDIALOG_H
/** @file 
* @brief   �ز������ͷ�ļ�
* @author    �׼���
* @date      2013��3��
* @version   version
  @note      2013�� 6��
*/
#include <QDialog>
#include "ui_DBZDialog.h"
#include "idpui_global.h"
/** @class   DB2Dialog
    @brief   �ز������ͷ�ļ�
    @author  �׼���
    @note   �ز������Ķ���ӿ�
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
