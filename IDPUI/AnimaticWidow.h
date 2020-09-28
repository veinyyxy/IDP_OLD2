#ifndef ANIMATICWIDOW_H
#define ANIMATICWIDOW_H

#include <osgViewer/Viewer>
#include <QtGui>
#include <osg/Node>
#include "ui_AnimaticWidow.h"
#include "AnimaticControler.h"

class ViewerQT;
class MyActionInterface;

class AnimaticWidow : public QMainWindow
{
	Q_OBJECT

public:
	AnimaticWidow(osg::Node* node , QWidget *parent = 0);
	~AnimaticWidow();

	public Q_SLOTS:
		void Slot_AnimaticToolBarEvent(QAction* action);
		void Slot_ButtonClicked();
		void AddTableWidgetItem(QString dir);
		void Slot_ComboBoxIndex(int iIndex);

protected:
	virtual void closeEvent( QCloseEvent *event );
private:
	QPointer<MyActionInterface> m_pPlayAction;				//播放
	QPointer<MyActionInterface> m_pPauseAction;			//暂停
	QPointer<MyActionInterface> m_pStopAction;			//停止
	QPointer<MyActionInterface> m_pPre1Action;			//上一争
	QPointer<MyActionInterface> m_pPreAction;				//向前
	QPointer<MyActionInterface> m_pNextAction;			//向后
	QPointer<MyActionInterface> m_pNext1Action;			//下一争
	QPointer<MyActionInterface> m_pRefreshAction;		//刷新
	QToolBar* m_pAnimaticToolBar;
	ViewerQT* m_pViewerQT;
	Ui::AnimaticWidow ui;
	AnimaticControler m_AnimaticControler;
};

#endif // ANIMATICWIDOW_H
