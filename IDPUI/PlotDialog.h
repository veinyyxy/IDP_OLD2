#ifndef PLOTDIALOG_H
#define PLOTDIALOG_H
/** @file 
* @brief   剖面图界面
* @author    易家祥
* @date      2013年3月
* @version   version
  @note      2013年 6月
*/
#include <QtGui>
#include "..\XMLProvider\RadReadColor_XML.h"
#include "ui_PlotDialog.h"
#include "idpui_global.h"

 /** @class   PlotDialog
    @brief   剖面图界面
    @author  易家祥
    @note   剖面图的对外接口
*/ 
class IDPUI_EXPORT PlotDialog : public QDialog
{
	Q_OBJECT

public:
	PlotDialog(QWidget *parent = 0);
	~PlotDialog();
private:
	void InitDialog();
	void InitSlots();
public:
		/** @brief 得到剖面区域 */ 
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
