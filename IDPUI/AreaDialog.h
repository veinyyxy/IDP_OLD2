#ifndef AREADIALOG_H
#define AREADIALOG_H
/** @file 
* @brief   面积统计类头文件
* @author    易家祥
* @date      2013年3月
* @version   version
  @note      2013年 6月
*/
#include <QtGui>
#include "ui_AreaDialog.h"
#include "idpui_global.h"
#include "Map.h"
using namespace goto_gis;
/** @class   AreaDialog
    @brief   面积统计类界面文件
    @author  易家祥
    @note    面积统计类界面的对外接口
*/ 


class IDPUI_EXPORT  AreaDialog : public QDialog
{
	Q_OBJECT

public:
	AreaDialog(Map* srcMap , QWidget *parent = 0);
	~AreaDialog();
public:
	/** @brief 初始化对话框 */ 
	void InitDialog();
	/** @brief 初始化对话框控件对应槽链接 */ 
	void InitSlots();
	void DrawArea();
public Q_SLOTS:
	/** @brief 对话框控件对应槽响应 */ 
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
