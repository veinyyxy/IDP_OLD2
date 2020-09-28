#ifndef PLOTDIALOG_H
#define PLOTDIALOG_H
/** @file 
* @brief   ����ͼ����
* @author    �׼���
* @date      2013��3��
* @version   version
  @note      2013�� 6��
*/
#include <QtGui>
#include "..\XMLProvider\RadReadColor_XML.h"
#include "ui_PlotDialog.h"
#include "idp_control_global.h"

 /** @class   PlotDialog
    @brief   ����ͼ����
    @author  �׼���
    @note   ����ͼ�Ķ���ӿ�
*/ 
class IDP_CONTROL_EXPORT PlotDialog : public QDialog
{
	Q_OBJECT

public:
	PlotDialog(QWidget *parent = 0);
	~PlotDialog();
private:
	void InitDialog();
	void InitSlots();
public:
		/** @brief �õ��������� */ 
	void GetArray(QString proname,int ds,int dh,int** buffer,int m_max,double m_maxH,int m_maxAz,int maxR,P_COLORTABLE_VECTOR pTableColor,QStringList m_AzList,QStringList m_RList,QStringList m_InfoList);
	void GetImage(QImage* pImage);
protected:
	void paintEvent( QPaintEvent *event );
protected Q_SLOTS:
		void Slots_PushButtonClicked();
private:
	Ui::PlotDialog ui;

	QString m_proname;
	QStringList AzList,RList,InfoList;
	int w,h,pmax,pmaxAz,pmaxR;
	double pmaxH;
	int **buf;
	P_COLORTABLE_VECTOR m_pTableColor;
};

#endif // PLOTDIALOG_H
