#ifndef DB2DIALOG_H
#define DB2DIALOG_H
/** @file 
* @brief   �ز������ͷ�ļ�
* @author    �׼���
* @date      2013��3��
* @version   version
  @note      2013�� 6��
*/
#include <QDialog>
#include "ui_DB2Dialog.h"
#include "idp_control_global.h"
#include "Map.h"
/** @class   DB2Dialog
    @brief   �ز������ͷ�ļ�
    @author  �׼���
    @note   �ز������Ķ���ӿ�
*/
using namespace goto_gis;

class IsoUserData : public QObjectUserData
{
public:
	IsoUserData(Map* srcMap);
	void ComputerIsoSurface(int iValue);
	void DeleteIsoSuerface();
private:
	bool m_computed;
	int m_iValue;
	Map* m_pMap;
};

class IDP_CONTROL_EXPORT  DB2Dialog : public QDialog
{
	Q_OBJECT

public:
	DB2Dialog(Map* srcMap , int mix , int max ,QWidget *parent = 0);
	~DB2Dialog();
private:
	void InitDialog();
	void InitSlots();

protected Q_SLOTS:
		void Slots_PushButtonClicked();
private:
	Ui::DB2Dialog ui;
	static int gRowCount;
	QIntValidator* m_intValidator;
	Map* m_pMap;
};

#endif // DB2DIALOG_H
