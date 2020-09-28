#ifndef LEFTDOCKWIDGET_H
#define LEFTDOCKWIDGET_H
/** @file 
* @brief   左侧地图属性等控件
* @author    易家祥
* @date      2013年3月
* @version   version
  @note      2013年 6月
*/
#include <QtGui>
#include "ui_LeftDockWidget.h"
#include "idpui_global.h"
//#include "goConfig.h"

class MainWindow;
class FieldDialog;


 /** @class   LeftDockWidget
    @brief   左侧地图属性等控件
    @author  易家祥
    @note   左侧地图属性等控件的对外接口
*/ 
class IDPUI_EXPORT  LeftDockWidget : public QDockWidget
{
	Q_OBJECT

public:
	LeftDockWidget(QWidget *parent = 0);
	~LeftDockWidget();

public:
		/** @brief 得到基础图层指针 */ 
	inline QTreeWidgetItem* GetBasisLayerTreeItem(){return m_pBasisLayerTreeItem;} //基础图层
		/** @brief 得到产品图层指针 */ 
	inline QTreeWidgetItem* GetProuductLayerTreeItem(){return m_pProuductLayerTreeItem;}//产品图层
	/** @brief 设置地图指针 */ 
	//inline void SetMap(Map* map) {m_pMap = map;}
	/** @brief 得到树指针 */ 
	//inline QTreeWidget* GetGISTreeWidget(){return ui.treeWidget;}
	void ClearTreeWidgetItem();
	void AddTreeWidgetItem(QWidget* viewerWidget);
private:
	void InitSlots();
	void InitLeftDockWidget();

public Q_SLOTS:
	virtual void setVisible(bool visible);
	void Slot_ItemClick( QTreeWidgetItem * item, int column);
	void Slot_ShowContextMenu(const QPoint& point);
	void Slot_itemDoubleClicked(QTreeWidgetItem * item, int column);
	void Slot_TreeRightMenuAction();
	void Slot_MoveUp();
	void Slot_MoveDown();
	void Slot_AddBaseMap();

private:

	Ui::LeftDockWidget ui;
	
	
	QTreeWidgetItem* m_pBasisLayerTreeItem; //基础图层
	QTreeWidgetItem* m_pProuductLayerTreeItem;//产品图层

	QMenu* m_pRightMenu;
	FieldDialog* m_pFieldDialog;

	MainWindow* m_pMW;
	QString m_strLayerName;
 };

#endif // LEFTDOCKWIDGET_H
