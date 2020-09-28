#pragma once
#include <QWidget>
#include "ui_cipascomposerarrowwidgetbase.h"

class CCipasComposerArrow;

class CCipasComposerArrowWidget :public QWidget ,private Ui::CipasComposerArrowWidgetBase
{
	Q_OBJECT
public:
	CCipasComposerArrowWidget(CCipasComposerArrow* arrow);
	~CCipasComposerArrowWidget(void);
private:
	CCipasComposerArrow* mArrow;

	void blockAllSignals( bool block );

	QButtonGroup* mRadioButtonGroup;

	/**Enables / disables the SVG line inputs*/
	//void enableSvgInputElements( bool enable );

	private slots:
		void on_mOutlineWidthSpinBox_valueChanged( double d );
		void on_mArrowHeadWidthSpinBox_valueChanged( double d );
		void on_mArrowColorButton_clicked();
		void on_mDefaultMarkerRadioButton_toggled( bool toggled );
		void on_mNoMarkerRadioButton_toggled( bool toggled );
		//void on_mSvgMarkerRadioButton_toggled( bool toggled );
		//void on_mStartMarkerLineEdit_editingFinished( const QString & text );
		//void on_mEndMarkerLineEdit_editingFinished( const QString & text );
		//void on_mStartMarkerToolButton_clicked();
		//void on_mEndMarkerToolButton_clicked();

		void setGuiElementValues();
};

