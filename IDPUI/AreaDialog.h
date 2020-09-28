#ifndef AREADIALOG_H
#define AREADIALOG_H
/** @file 
* @brief   ���ͳ����ͷ�ļ�
* @author    �׼���
* @date      2013��3��
* @version   version
  @note      2013�� 6��
*/
#include <QtGui>
#include "ui_AreaDialog.h"
#include "idpui_global.h"
#include "Map.h"
using namespace goto_gis;
/** @class   AreaDialog
    @brief   ���ͳ��������ļ�
    @author  �׼���
    @note    ���ͳ�������Ķ���ӿ�
*/ 


class IDPUI_EXPORT  AreaDialog : public QDialog
{
	Q_OBJECT

public:
	AreaDialog(Map* srcMap , QWidget *parent = 0);
	~AreaDialog();
public:
	/** @brief ��ʼ���Ի��� */ 
	void InitDialog();
	/** @brief ��ʼ���Ի���ؼ���Ӧ������ */ 
	void InitSlots();
	void DrawArea();
public Q_SLOTS:
	/** @brief �Ի���ؼ���Ӧ����Ӧ */ 
	void Slots_PushButtonClicked();
	void Slots_RadioButtonClicked();
private:
	Ui::AreaDialog ui;
	Map* m_pMap;
	bool m_bPie;
	float m_vMin;
	float m_vMax;
	osg::Vec3Array* m_pVec3Array;
};

#endif // AREADIALOG_H
