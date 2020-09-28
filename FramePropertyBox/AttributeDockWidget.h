#ifndef ATTRIBUTEDOCKWIDGET_H
#define ATTRIBUTEDOCKWIDGET_H
/** @file 
* @brief   ���Կؼ�ͷ�ļ�
* @author    �׼���
* @date      2013��3��
* @version   version
  @note      2013�� 6��
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
    @brief   ���Կؼ�ͷ�ļ�
    @author  �׼���
    @note    ���Կؼ��Ķ���ӿ�
*/ 
class AttributeDockWidget : public QDockWidget
{
	Q_OBJECT

public:
	AttributeDockWidget(QWidget *parent = 0);
	~AttributeDockWidget();
	/** @brief ��ʾ���� */ 
	void ShowAttributePropertyBrowser(Layer* layer,bool readonly = false);

protected:
	boost::any GetAttributeValue(int index);
	void AddPropertyBrowser(FIELD_MAP layerAttributeDescripe, bool readonly = false /*˵����?���޸� 1�����޸� */);
public Q_SLOTS:
	/** @brief ��ʾ��Ӧ�� */ 
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
