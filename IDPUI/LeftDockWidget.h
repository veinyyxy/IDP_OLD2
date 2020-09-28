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
#include "ui_LeftDockWidget.h"
#include "idpui_global.h"
//#include "goConfig.h"

class MainWindow;
class FieldDialog;


 /** @class   LeftDockWidget
    @brief   ����ͼ���Եȿؼ�
    @author  �׼���
    @note   ����ͼ���Եȿؼ��Ķ���ӿ�
*/ 
class IDPUI_EXPORT  LeftDockWidget : public QDockWidget
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
	
	
	QTreeWidgetItem* m_pBasisLayerTreeItem; //����ͼ��
	QTreeWidgetItem* m_pProuductLayerTreeItem;//��Ʒͼ��

	QMenu* m_pRightMenu;
	FieldDialog* m_pFieldDialog;

	MainWindow* m_pMW;
	QString m_strLayerName;
 };

#endif // LEFTDOCKWIDGET_H
