#ifndef DEFAULTTOOLBAR_H
#define DEFAULTTOOLBAR_H
/** @file 
* @brief   文件打开工具栏类头文件
* @author    易家祥
* @date      2013年3月
* @version   version
  @note      2013年 6月
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
    @brief   文件打开工具栏类头文件
    @author  易家祥
    @note   文件打开工具栏类的对外接口
*/ 
class IDPUI_EXPORT  DefaultToolBar : public QToolBar
{
	Q_OBJECT

public:
	DefaultToolBar(QWidget *parent = 0);
	~DefaultToolBar();

public:
	/** @brief 初始化“数据路径” */ 
	void SetEmitForm(QString dataPath);			
	/** @brief 发送数据路径信号” */ 
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
