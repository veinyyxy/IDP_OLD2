#pragma once

#include <QWidget>

#include "tmui_global.h"
#include "ui_cipascomposerscalebarwidgetbase.h"

class CCipasComposerScaleBar;

class TMUI_EXPORT CCipasComposerScaleBarWidget :public QWidget ,private Ui::CipasComposerScaleBarWidgetBase
{
	Q_OBJECT
public:
	CCipasComposerScaleBarWidget(CCipasComposerScaleBar* scaleBar);
	~CCipasComposerScaleBarWidget(void);

public slots:
	void on_mMapComboBox_activated( const QString& text );
	void on_mHeightSpinBox_valueChanged( int i );
	void on_mLineWidthSpinBox_valueChanged( double d );
	void on_mSegmentSizeSpinBox_valueChanged( double d );
	void on_mSegmentsLeftSpinBox_valueChanged( int i );
	void on_mNumberOfSegmentsSpinBox_valueChanged( int i );
	void on_mUnitLabelLineEdit_textChanged( const QString& text );
	void on_mMapUnitsPerBarUnitSpinBox_valueChanged( double d );
	void on_mColorPushButton_clicked();
	void on_mFontButton_clicked();
	void on_mStyleComboBox_currentIndexChanged( const QString& text );
	void on_mLabelBarSpaceSpinBox_valueChanged( double d );
	void on_mBoxSizeSpinBox_valueChanged( double d );


protected:
	void showEvent( QShowEvent * event );
private:
	CCipasComposerScaleBar* mComposerScaleBar;

	void refreshMapComboBox();
	void setGuiElements();
	/**Enables/disables the signals of the input gui elements*/
	void blockMemberSignals( bool enable );

};

