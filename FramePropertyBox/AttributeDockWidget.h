#ifndef ATTRIBUTEDOCKWIDGET_H
#define ATTRIBUTEDOCKWIDGET_H
/** @file 
* @brief   属性控件头文件
* @author    易家祥
* @date      2013年3月
* @version   version
  @note      2013年 6月
*/
#include <QtGui>
#include "./GeneratedFiles/ui_AttributeDockWidget.h"
#include "idpui_global.h"

#include "qtpropertybrowser\qtpropertymanager.h"
#include "qtpropertybrowser\qteditorfactory.h"
#include "qtpropertybrowser\qttreepropertybrowser.h"
#include "qtpropertybrowser\qtbuttonpropertybrowser.h"
#include "qtpropertybrowser\qtgroupboxpropertybrowser.h"

#include "Map.h"
#include "Layer.h"

using namespace goto_gis;


/** @class   AttributeDockWidget
    @brief   属性控件头文件
    @author  易家祥
    @note    属性控件的对外接口
*/ 
class AttributeDockWidget : public QDockWidget
{
	Q_OBJECT

public:
	AttributeDockWidget(QWidget *parent = 0);
	~AttributeDockWidget();
	/** @brief 显示属性 */ 
	void ShowAttributePropertyBrowser(Layer* layer,bool readonly = false);

protected:
	boost::any GetAttributeValue(int index);
	void AddPropertyBrowser(FIELD_MAP layerAttributeDescripe, bool readonly = false /*说明是?能修改 1可以修改 */);
public Q_SLOTS:
	/** @brief 显示对应槽 */ 
		virtual void setVisible(bool visible);
		void Slot_ValueChanged(QtProperty *property, int val);
		void Slot_ValueChanged(QtProperty *property, const QStringList &val);
		void Slot_ValueChanged(QtProperty *property, const QSize &val);
		void Slot_ValueChanged(QtProperty *property, const QPoint &val);
		void Slot_ValueChanged(QtProperty *property, const QFont &val);
		void Slot_ValueChanged(QtProperty *property, const QColor &val);
		void Slot_ValueChanged(QtProperty *property, const QString &val);
		void Slot_ValueChanged(QtProperty *property, double val);
		void Slot_ValueChanged(QtProperty *property, bool val);
		void UpdateProperty(QtProperty *property,QVariant& val);
private:
	Ui::AttributeDockWidget ui;
	Layer* m_pLayer;
	QtTreePropertyBrowser* m_pPropertyBrowser;
};

#endif // ATTRIBUTEDOCKWIDGET_H
