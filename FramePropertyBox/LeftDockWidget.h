#ifndef LEFTDOCKWIDGET_H
#define LEFTDOCKWIDGET_H
/** @file 
* @brief   ����ͼ���Եȿؼ�
* @author    �׼���
* @date      2013��3��
* @version   version
  @note      2013�� 6��
*/
#include <QtGui>
#include "./GeneratedFiles/ui_LeftDockWidget.h"
#include "AdapterWidget.h"
#include "Map.h"
#include "MyActionInterface.h"
#include "TreeViewEventInterface.h"
//#include "goConfig.h"

class MainWindow;
class FieldDialog;
class DockWidgetInterface;

 /** @class   LeftDockWidget
    @brief   ����ͼ���Եȿؼ�
    @author  �׼���
    @note   ����ͼ���Եȿؼ��Ķ���ӿ�
*/ 
class  LeftDockWidget : public QDockWidget
{
	Q_OBJECT

public:
	LeftDockWidget(QWidget *parent = 0);
	~LeftDockWidget();

public:
		/** @brief �õ�����ͼ��ָ�� */ 
	inline QTreeWidgetItem* GetBasisLayerTreeItem(){return m_pBasisLayerTreeItem;} //����ͼ��
		/** @brief �õ���Ʒͼ��ָ�� */ 
	inline QTreeWidgetItem* GetProuductLayerTreeItem(){return m_pProuductLayerTreeItem;}//��Ʒͼ��
	/** @brief ���õ�ͼָ�� */ 
	//inline void SetMap(Map* map) {m_pMap = map;}
	/** @brief �õ���ָ�� */ 
	//inline QTreeWidget* GetGISTreeWidget(){return ui.treeWidget;}
	void ClearTreeWidgetItem();
	void AddTreeWidgetItem(QWidget* viewerWidget);
	inline ViewerQT* GetCurentViewerQT();
	inline goto_gis::Map* GetCurentMap();
	void AddContextMenuAction(MyActionInterface* pAction);
	void AddToolsBarAction(MyActionInterface* pAction);
	inline void SetModel(const TreeViewEventInterface* pTEI){
		m_pTreeViewEventInterface = const_cast<TreeViewEventInterface*>(pTEI);
	}
	void SetModel( QAbstractItemModel * model );
	void SetUpdateInterface(DockWidgetInterface* pDwi){m_pDockWidgetInterface = pDwi;}
private:
	void InitSlots();
	void InitLeftDockWidget();

public Q_SLOTS:
	virtual void setVisible(bool visible);
	void Slot_ItemClick(const QModelIndex& index);
	void Slot_ShowContextMenu(const QPoint& point);
	void Slot_itemDoubleClicked(const QModelIndex& index);
	void Slot_TreeRightMenuAction();
	void Slot_MoveUp();
	void Slot_MoveDown();
	void Slot_AddBaseMap();
	void Slot_ActionTriggered_dock(QAction* action);

private:

	Ui::LeftDockWidget ui;
	
	
	QTreeWidgetItem* m_pBasisLayerTreeItem; //����ͼ��
	QTreeWidgetItem* m_pProuductLayerTreeItem;//��Ʒͼ��

	QMenu* m_pRightMenu;
	FieldDialog* m_pFieldDialog;

	//MainWindow* m_pMW;
	QString m_strLayerName;
	TreeViewEventInterface* m_pTreeViewEventInterface;
	QActionGroup* m_pActionGroup;
	DockWidgetInterface* m_pDockWidgetInterface;
 };

#endif // LEFTDOCKWIDGET_H
