#pragma once

#include <QWidget>
#include "tmui_global.h"
#include "ui_cipascomposershapewidgetbase.h"

class CCipasComposerShape;

class TMUI_EXPORT CCipasComposerShapeWidget :	public QWidget ,private Ui::CipasComposerShapeWidgetBase
{
	Q_OBJECT
public:
	CCipasComposerShapeWidget(CCipasComposerShape* composerShape);
	~CCipasComposerShapeWidget(void);
private:
	CCipasComposerShape* mComposerShape;

	/**Blocks / unblocks the signal of all GUI elements*/
	void blockAllSignals( bool block );

private slots:
	void on_mShapeComboBox_currentIndexChanged( const QString& text );
	void on_mOutlineColorButton_clicked();
	void on_mOutlineWidthSpinBox_valueChanged( double d );
	void on_mTransparentCheckBox_stateChanged( int state );
	void on_mFillColorButton_clicked();
	void on_mRotationSpinBox_valueChanged( int val );

	/**Sets the GUI elements to the currentValues of mComposerShape*/
	void setGuiElementValues();
};

