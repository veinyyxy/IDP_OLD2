/*============================================================ 
�ļ�����statustoolbar.h
�� ���� StatusToolBar
�� �ࣺ QToolBar
�� �ࣺ �� 
����˵����״̬������
����˵����
1����
2����

----------------------------�汾����-------------------------
V 1.0 
ԭ���� ���׼���
������ڣ�2012�� 6��11��
�޸����ߣ�
������ڣ�2012�� 6��11��
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

	enum OptionModel//����ģʽ
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
	void initStatusToolBar();//��ʼ��״̬������
	void initVisible();
	void closeALLMapView();
public Q_SLOTS:
		void slot_statusToolBarEvent(QAction* action);
		void slot_currentIndexChanged(int index);
		void slot_RefreshCoordInfo(QString mess);
		void slot_changLayer(int index);
private:
	QPointer<QAction> m_pZoomInAction;		//�Ŵ�
	QPointer<QAction> m_pZoomOutAction;		//��С
	QPointer<QAction> m_pRoamAction;			//����
	//QPointer<QComboBox> m_pTypeBox;

	QPointer<MyActionInterface> m_pPlayAction;				//����
	QPointer<MyActionInterface> m_pPauseAction;			//��ͣ
	QPointer<MyActionInterface> m_pStopAction;			//ֹͣ
	QPointer<MyActionInterface> m_pPre1Action;			//��һ��
	QPointer<MyActionInterface> m_pPreAction;				//��ǰ
	QPointer<MyActionInterface> m_pNextAction;			//���
	QPointer<MyActionInterface> m_pNext1Action;			//��һ��
	QPointer<MyActionInterface> m_pRefreshAction;		//ˢ��

	QPointer<QAction> m_pWin4Action;			//4����
	QPointer<QAction> m_pMeAction;				//����

	QPointer<QAction> m_pAttentionAction;		//��ע
	QPointer<QAction> m_pShareAction;			//����
	QPointer<QAction> m_pAreaAction;			//���
	QPointer<QAction> m_pMaxAction;				//���

	QPointer<QSpinBox> m_pSpinBox;
	AnimaticControler* m_AnimaitcControler;
	//QPointer<SearchWidget> m_pSearchWidget;
private:
	OptionModel m_optionModel;
};

#endif // STATUSTOOLBAR_H
