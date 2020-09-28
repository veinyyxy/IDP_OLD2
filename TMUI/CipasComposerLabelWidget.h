#pragma once

#include <QWidget>

#include "tmui_global.h"
#include "ui_cipascomposerlabelwidgetbase.h"

class CCipasComposerLabel;

class TMUI_EXPORT CCipasComposerLabelWidget :public QWidget,private Ui::CipasComposerLabelWidgetBase
{
	Q_OBJECT
public:
	CCipasComposerLabelWidget(CCipasComposerLabel* label );
	CCipasComposerLabelWidget(CCipasComposerLabel* label , QString & lyrName);
	void setLayerName(const QString & lyrName);
	QString getLayerName()const;

public slots:
		void on_mTextEdit_textChanged();
		void on_mFontButton_clicked();
		void on_mMarginDoubleSpinBox_valueChanged( double d );
		void on_mFontColorButton_clicked();
		void on_mCenterRadioButton_clicked();
		void on_mLeftRadioButton_clicked();
		void on_mRightRadioButton_clicked();
		void on_mTopRadioButton_clicked();
		void on_mBottomRadioButton_clicked();
		void on_mMiddleRadioButton_clicked();
		void on_mLabelIdLineEdit_textChanged( const QString& text );

		void on_advanceLabeCheckBox_stateChanged(int state);

private slots:
		void setGuiElementValues();
private:
	CCipasComposerLabel* mComposerLabel;
	void blockAllSignals( bool block );

	QString mLyrName;
};

