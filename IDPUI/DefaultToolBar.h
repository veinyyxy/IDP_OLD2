#ifndef DEFAULTTOOLBAR_H
#define DEFAULTTOOLBAR_H
/** @file 
* @brief   �ļ��򿪹�������ͷ�ļ�
* @author    �׼���
* @date      2013��3��
* @version   version
  @note      2013�� 6��
*/
#include <QtGui>
#include "ui_DefaultToolBar.h"
#include "idpui_global.h"

#include "RadarDataAccess.h"

class ConfigDialog;
class ViewerQT;
class QRecentFilesMenu;

class MainWindow;
/** @class   DefaultToolBar
    @brief   �ļ��򿪹�������ͷ�ļ�
    @author  �׼���
    @note   �ļ��򿪹�������Ķ���ӿ�
*/ 
class IDPUI_EXPORT  DefaultToolBar : public QToolBar
{
	Q_OBJECT

public:
	DefaultToolBar(QWidget *parent = 0);
	~DefaultToolBar();

public:
	/** @brief ��ʼ��������·���� */ 
	void SetEmitForm(QString dataPath);			
	/** @brief ��������·���źš� */ 
	void EmitOpenSigal(QString fromType) {Slots_ActionTriggered(ui.actionOpen); }
	
private:
	void InitSlots();

public Q_SLOTS:
		void Slots_ActionTriggered( QAction* action );
		void LoadData(const QString & fileName);

private:
	osg::Vec3d WorldToScreen(ViewerQT *view,osg::Vec3 worldpoint);
	void Transform_Point(double out[4],const double m[16],const double in[4]);

private:
	Ui::DefaultToolBar ui;
	QActionGroup m_DefaultActionGroup;

	//QPointer<ConfigDialog> m_pConfigDialog;
	QString m_EmitFormDataPath;

	RadarDataAccess* m_pRaderDataAccess;
	
	MainWindow* m_pMW;
};

#endif // DEFAULTTOOLBAR_H
