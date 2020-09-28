#ifndef RADARTOOLBAR_H
#define RADARTOOLBAR_H
/** @file 
* @brief   雷达工具栏
* @author    易家祥
* @date      2013年3月
* @version   version
  @note      2013年 6月
*/
#include <QtGui>
#include "ui_RadarToolBar.h"
#include "idpui_global.h"

#include "Layer.h"
#include "Map.h"

using namespace goto_gis;
class AreaDialog;
class PlotDialog;
class IDPComposer;
class CCipasComposer;
class DBZDialog;
class MainWindow;
class RadarDataAccess;
class AnimaticWidow;

 /** @class   RadarToolBar
    @brief   雷达工具栏
    @author  易家祥
    @note  雷达工具栏的对外接口
*/ 
class IDPUI_EXPORT  RadarToolBar : public QToolBar
{
	Q_OBJECT

public:
	RadarToolBar(QWidget *parent = 0);
	~RadarToolBar();

	void SetEnabledList(QStringList actionList,bool endabled);
	void DrawPlotGraph(QImage* pImage, osg::Vec3& p1, osg::Vec3& p2, osg::Vec3& p3, osg::Vec3& p4);
	void DrawPlotGraph(osg::Vec3& p1, osg::Vec3& p2, osg::Vec3& p3, osg::Vec3& p4, osg::Node* pClipNode);
	void LoadData(QString filename);

	void ActionTrigger(QAction* action);

private:
	void InitSlots();
	boost::any GetAttributeValue(int index ,goto_gis::Layer* layer);
public Q_SLOTS:
		void Slots_ActionTriggered( QAction* action );
		void Slot_ActivateWindow(QWidget* widget);
private:
	Ui::RadarToolBar ui;
	DBZDialog* m_pDB2Dialog;
	QActionGroup m_DefaultActionGroup;
	AreaDialog* m_pAreaDialog;
	PlotDialog* m_pPlotDialog;
	double Get_Length( double dx, double dy );
	
	MainWindow* m_pMW;
	RadarDataAccess* m_pRadarDataAccess;
};

#endif // RADARTOOLBAR_H
