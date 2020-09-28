#pragma once

#include <QWidget>
#include "tmui_global.h"
#include "ui_ciapscomposeritemwidgetbase.h"

class CComposerItem;

class TMUI_EXPORT CCipasComposerItemWidget :public QWidget,private Ui::CipasComposerItemWidgetBase
{
	Q_OBJECT

public:
	CCipasComposerItemWidget(QWidget* parent, CComposerItem* item );
	~CCipasComposerItemWidget();

public slots:
	void on_mFrameColorButton_clicked();
	void on_mBackgroundColorButton_clicked();
	void on_mOpacitySlider_sliderReleased();
	void on_mOutlineWidthSpinBox_valueChanged( double d );
	void on_mFrameCheckBox_stateChanged( int state );
	//void on_mPositionButton_clicked();

private:
	CCipasComposerItemWidget();
	void setValuesForGuiElements();
	double m_width;
	bool m_ischeck;
	CComposerItem* mItem;
};

