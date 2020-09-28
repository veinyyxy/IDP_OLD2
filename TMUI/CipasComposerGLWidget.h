#pragma once

#include <QWidget>
#include "tmui_global.h"
#include "ui_cipascomposermapwidgetbase.h"

class CCipasComposerGLW;

class TMUI_EXPORT CCipasComposerGLWidget :public QWidget,private Ui::CipasComposerMapWidgetBase
{
	Q_OBJECT
public:
	CCipasComposerGLWidget(CCipasComposerGLW* mGLW);
	~CCipasComposerGLWidget(void);
public slots:
	void on_mWidthLineEdit_editingFinished();
	void on_mHeightLineEdit_editingFinished();
	//void on_mPreviewModeComboBox_activated( int i );
	//void on_mScaleLineEdit_editingFinished();
	void on_mRotationSpinBox_valueChanged( int value );
	void on_mSetToMapCanvasExtentButton_clicked();
	//void on_mUpdatePreviewButton_clicked();
	//void on_mKeepLayerListCheckBox_stateChanged( int state );
	//void on_mDrawCanvasItemsCheckBox_stateChanged( int state );

	void on_mXMinLineEdit_editingFinished();
	void on_mXMaxLineEdit_editingFinished();
	void on_mYMinLineEdit_editingFinished();
	void on_mYMaxLineEdit_editingFinished();

	void on_mGridCheckBox_toggled( bool state );
	void on_mIntervalXSpinBox_editingFinished();
	void on_mIntervalYSpinBox_editingFinished();
	void on_mOffsetXSpinBox_editingFinished();
	void on_mOffsetYSpinBox_editingFinished();
	void on_mLineWidthSpinBox_valueChanged( double d );
	//地图格网(框架内)的颜色
	void on_mLineColorButton_clicked();
	void on_mGridTypeComboBox_currentIndexChanged( const QString& text );
	//void on_mCrossWidthSpinBox_valueChanged( double d );
	void on_mAnnotationFontButton_clicked();
	//地图格网标注的颜色
	void on_mAnnotationColorButton_clicked();
	void on_mDistanceToMapFrameSpinBox_valueChanged( double d );
	void on_mAnnotationPositionComboBox_currentIndexChanged( const QString& text );
	void on_mAnnotationPositionComboBox2_currentIndexChanged( const QString& text );
	void on_mDrawAnnotationCheckBox_stateChanged( int state );
	void on_mAnnotationDirectionComboBox_currentIndexChanged( const QString& text );
	void on_mCoordinatePrecisionSpinBox_valueChanged( int value );
	void on_mLabelAxesButtonGroup_buttonClicked(int id);	/*标注轴*/
	
	void on_mAnnotationStyleComboBox_currentIndexChanged(  int index );
private slots:
	/**Sets the GUI elements to the values of mPicture*/
	void setGuiElementValues();
//protected:
//	void showEvent( QShowEvent * event );

private:
	CCipasComposerGLW* mGLW;

	/**Sets the current composer map values to the GUI elements*/
	void updateGuiElements();

	/**Sets extent of composer map from line edits*/
	void updateComposerExtentFromGui();
	/**Blocks / unblocks the signals of all GUI elements*/
	void blockAllSignals( bool b );
};

