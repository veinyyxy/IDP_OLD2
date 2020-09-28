/*============================================================ 
文件名：statustoolbar.h
类 名： StatusToolBar
父 类： QToolBar
子 类： 无 
功能说明：状态工具栏
调用说明：
1、；
2、；

----------------------------版本更新-------------------------
V 1.0 
原作者 ：易家祥
完成日期：2012年 6月11日
修改作者：
完成日期：2012年 6月11日
============================================================*/
#ifndef STATUSTOOLBAR_H
#define STATUSTOOLBAR_H
#include "idpui_global.h"
#include <QtCore/QPointer>
#include <QToolBar>
class QComboBox;
class QSpinBox;
class AnimaticControler;
class MyActionInterface;

class IDPUI_EXPORT StatusToolBar : public QToolBar
{
	Q_OBJECT

public:

	enum OptionModel//操作模式
	{
		NoneModel = 0,
		SearchWidgetModel = 1,
		RadProductWidgetModel = 2,
		LoadModel = 3,
		AttentionModel = 4
	};

	explicit StatusToolBar(QWidget *parent);
	~StatusToolBar();

	//inline QPointer<QSpinBox> GetSpinBox() { return m_pSpinBox;}

	OptionModel getOptionModel(){return m_optionModel;}
protected:
	void initStatusToolBar();//初始化状态工具栏
	void initVisible();
	void closeALLMapView();
public Q_SLOTS:
		void slot_statusToolBarEvent(QAction* action);
		void slot_currentIndexChanged(int index);
		void slot_RefreshCoordInfo(QString mess);
		void slot_changLayer(int index);
private:
	QPointer<QAction> m_pZoomInAction;		//放大
	QPointer<QAction> m_pZoomOutAction;		//缩小
	QPointer<QAction> m_pRoamAction;			//漫游
	//QPointer<QComboBox> m_pTypeBox;

	QPointer<MyActionInterface> m_pPlayAction;				//播放
	QPointer<MyActionInterface> m_pPauseAction;			//暂停
	QPointer<MyActionInterface> m_pStopAction;			//停止
	QPointer<MyActionInterface> m_pPre1Action;			//上一争
	QPointer<MyActionInterface> m_pPreAction;				//向前
	QPointer<MyActionInterface> m_pNextAction;			//向后
	QPointer<MyActionInterface> m_pNext1Action;			//下一争
	QPointer<MyActionInterface> m_pRefreshAction;		//刷新

	QPointer<QAction> m_pWin4Action;			//4窗口
	QPointer<QAction> m_pMeAction;				//尺子

	QPointer<QAction> m_pAttentionAction;		//关注
	QPointer<QAction> m_pShareAction;			//共享
	QPointer<QAction> m_pAreaAction;			//面积
	QPointer<QAction> m_pMaxAction;				//最大化

	QPointer<QSpinBox> m_pSpinBox;
	AnimaticControler* m_AnimaitcControler;
	//QPointer<SearchWidget> m_pSearchWidget;
private:
	OptionModel m_optionModel;
};

#endif // STATUSTOOLBAR_H
