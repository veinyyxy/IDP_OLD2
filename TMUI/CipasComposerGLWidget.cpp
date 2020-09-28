
#include <QDoubleValidator>
#include <QFileDialog>
#include <QFileInfo>
#include <QImageReader>
#include <QMessageBox>
#include <QPainter>
#include <QProgressDialog>
#include <QSettings>
#include <QColorDialog>
#include <QFontDialog>
#include "CipasComposerItemWidget.h"
#include "CipasComposition.h"

#include "CipasComposerMap.h"
#include "CipasComposerGLW.h"
#include "CipasComposerGLWidget.h"

CCipasComposerGLWidget::CCipasComposerGLWidget(CCipasComposerGLW* GLW)
	: QWidget(),mGLW(GLW)
{
	setupUi( this );

	//add widget for general composer item properties
	CCipasComposerItemWidget* itemPropertiesWidget = new CCipasComposerItemWidget( this, GLW );
	
	toolBox->addItem( itemPropertiesWidget, QString::fromLocal8Bit("常规选项"));

	mWidthLineEdit->setValidator( new QDoubleValidator( mWidthLineEdit ) );
	mHeightLineEdit->setValidator( new QDoubleValidator( mHeightLineEdit ) );
	//mScaleLineEdit->setValidator( new QDoubleValidator( mScaleLineEdit ) );

	mXMinLineEdit->setValidator( new QDoubleValidator( mXMinLineEdit ) );
	mXMaxLineEdit->setValidator( new QDoubleValidator( mXMaxLineEdit ) );
	mYMinLineEdit->setValidator( new QDoubleValidator( mYMinLineEdit ) );
	mYMaxLineEdit->setValidator( new QDoubleValidator( mYMaxLineEdit ) );

	blockAllSignals( true );
	//格网
	mAnnotationPositionComboBox2->setCurrentIndex(GLW->getInOrOutSideFrame());
	mIntervalXSpinBox->setMinimum(1);
	mIntervalXSpinBox->setMaximum(100);
	mIntervalYSpinBox->setMinimum(1);
	mIntervalYSpinBox->setMaximum(100);
	//标注
	mAnnotationPositionComboBox->setCurrentIndex(0);
	mAnnotationPositionComboBox->setEnabled(false);
	mAnnotationStyleComboBox->setCurrentIndex(GLW->annotionStyle());
	mAnnotationDirectionComboBox->setCurrentIndex(0);	//尚待完善


	if ( GLW )
	{
		connect( GLW, SIGNAL( itemChanged() ), this, SLOT( setGuiElementValues() ) );
	}

	updateGuiElements();
	blockAllSignals( false );
}


CCipasComposerGLWidget::~CCipasComposerGLWidget(void)
{
}




void CCipasComposerGLWidget::on_mWidthLineEdit_editingFinished()
{
	if ( mGLW )
	{
		//QRectF pictureRect = mGLW->rect();

		//bool conversionOk;
		//double newWidth = mWidthLineEdit->text().toDouble( &conversionOk );
		//if ( conversionOk )
		//{
		//	//mGLW->beginCommand( tr( "Picture width changed" ) );
		//	QRectF newSceneRect( mGLW->transform().dx(), mGLW->transform().dy(), newWidth, pictureRect.height() );
		//	mGLW->setSceneRect( newSceneRect );
		//	//mGLW->endCommand();
		//}
		bool conversionSuccess = true;
		double newWidth = mWidthLineEdit->text().toDouble( &conversionSuccess );
		if ( !conversionSuccess )
		{
			return;
		}
		QRectF composerMapRect = mGLW->rect();
		QTransform composerMapTransform = mGLW->transform();

		QRectF newRect( composerMapTransform.dx(), composerMapTransform.dy(), newWidth, composerMapRect.height() );

		mGLW->beginCommand( tr( "Change item width" ) );
		mGLW->setSceneRect( newRect );
		mGLW->endCommand();
	}
}

void CCipasComposerGLWidget::on_mHeightLineEdit_editingFinished()
{
	if ( mGLW )
	{
		//QRectF pictureRect = mGLW->rect();

		//bool conversionOk;
		//double newHeight = mHeightLineEdit->text().toDouble( &conversionOk );
		//if ( conversionOk )
		//{
		//	//mGLW->beginCommand( tr( "Picture height changed" ) );
		//	QRectF newSceneRect( mGLW->transform().dx(), mGLW->transform().dy(), pictureRect.width(), newHeight );
		//	mGLW->setSceneRect( newSceneRect );
		//	//mGLW->endCommand();
		//}
		bool conversionSuccess = true;
		double newHeight = mHeightLineEdit->text().toDouble( &conversionSuccess );
		if ( !conversionSuccess )
		{
			return;
		}
		QRectF composerMapRect = mGLW->rect();
		QTransform composerMapTransform = mGLW->transform();

		QRectF newRect( composerMapTransform.dx(), composerMapTransform.dy(), composerMapRect.width(), newHeight );
		mGLW->beginCommand( tr( "Change item height" ) );
		mGLW->setSceneRect( newRect );
		mGLW->endCommand();
	}
}

void CCipasComposerGLWidget::on_mRotationSpinBox_valueChanged( int d)
{
	if ( mGLW )
	{
		mGLW->beginCommand( tr( "Picture rotation changed" ), CipasComposerMergeCommand::ComposerPictureRotation );
		mGLW->setRotation( d );
		mGLW->update();
		mGLW->endCommand();
	}

}



//void CCipasComposerGLWidget::showEvent( QShowEvent * event )
//{
//	refreshMapComboBox();
//	QWidget::showEvent( event );
//}


void CCipasComposerGLWidget::setGuiElementValues()
{
#if 0
	//set initial gui values
	if ( mGLW )
	{
		mWidthLineEdit->blockSignals( true );
		mHeightLineEdit->blockSignals( true );
		mRotationSpinBox->blockSignals( true );
		mPictureLineEdit->blockSignals( true );
		mComposerMapComboBox->blockSignals( true );
		mRotationFromComposerMapCheckBox->blockSignals( true );

		mPictureLineEdit->setText( /*mGLW->pictureFile() */"ss");
		QRectF pictureRect = mGLW->rect();
		mWidthLineEdit->setText( QString::number( pictureRect.width() ) );
		mHeightLineEdit->setText( QString::number( pictureRect.height() ) );
		mRotationSpinBox->setValue( mGLW->rotation() );

		refreshMapComboBox();

		if (false )/* mGLW->useRotationMap()*/
		{
			mRotationFromComposerMapCheckBox->setCheckState( Qt::Checked );
			mRotationSpinBox->setEnabled( false );
			mComposerMapComboBox->setEnabled( true );
			QString mapText ="aa";
			int itemId = 1;
			if ( itemId >= 0 )
			{
				mComposerMapComboBox->setCurrentIndex( itemId );
			}
		}
		else
		{
			mRotationFromComposerMapCheckBox->setCheckState( Qt::Unchecked );
			mRotationSpinBox->setEnabled( true );
			mComposerMapComboBox->setEnabled( false );
		}


		mRotationFromComposerMapCheckBox->blockSignals( false );
		mWidthLineEdit->blockSignals( false );
		mHeightLineEdit->blockSignals( false );
		mRotationSpinBox->blockSignals( false );
		mPictureLineEdit->blockSignals( false );
		mComposerMapComboBox->blockSignals( false );
	}
#endif
	mHeightLineEdit->blockSignals( true );
	mWidthLineEdit->blockSignals( true );
//	mScaleLineEdit->blockSignals( true );
	//mPreviewModeComboBox->blockSignals( true );

	updateGuiElements();

	mHeightLineEdit->blockSignals( false );
	mWidthLineEdit->blockSignals( false );
//	mScaleLineEdit->blockSignals( false );
	//mPreviewModeComboBox->blockSignals( false );
}


//////////////////////////////////////////////////////////////////////////
///
//////////////////////////////////////////////////////////////////////////


/*
void CCipasComposerGLWidget::on_mPreviewModeComboBox_activated( int i )
{
	if ( !mGLW )
	{
		return;
	}

	//if ( mGLW->isDrawing() )
	//{
	//	return;
	//}

	//QString comboText = mPreviewModeComboBox->currentText();
	//if ( comboText == QString::fromLocal8Bit("缓冲区") )
	//{
	//	mGLW->setPreviewMode( QgsComposerMap::Cache );
	//	mUpdatePreviewButton->setEnabled( true );
	//}
	//else if ( comboText == QString::fromLocal8Bit("绘制") )
	//{
	//	mGLW->setPreviewMode( QgsComposerMap::Render );
	//	mUpdatePreviewButton->setEnabled( true );
	//}
	//else if ( comboText == QString::fromLocal8Bit("框选区"))
	//{
	//	mGLW->setPreviewMode( QgsComposerMap::Rectangle );
	//	mUpdatePreviewButton->setEnabled( false );
	//}
	mUpdatePreviewButton->setEnabled( true );
	mGLW->cache();
	mGLW->update();
}

void CCipasComposerGLWidget::on_mScaleLineEdit_editingFinished()
{
	if ( !mGLW )
	{
		return;
	}

	bool conversionSuccess;
	double scaleDenominator = mScaleLineEdit->text().toDouble( &conversionSuccess );

	if ( !conversionSuccess )
	{
		return;
	}

	//mGLW->beginCommand( tr( "Map scale changed" ) );
	//mGLW->setNewScale( scaleDenominator );
	//mGLW->endCommand();
}
*/


void CCipasComposerGLWidget::on_mSetToMapCanvasExtentButton_clicked()
{
	int j=0;
	//if ( mGLW )
	//{
	//	const QgsMapRenderer* renderer = mGLW->mapRenderer();
	//	if ( renderer )
	//	{
	//		QgsRectangle newExtent = renderer->extent();

	//		//Make sure the width/height ratio is the same as in current composer map extent.
	//		//This is to keep the map item frame and the page layout fixed
	//		QgsRectangle currentMapExtent = mGLW->extent();
	//		double currentWidthHeightRatio = currentMapExtent.width() / currentMapExtent.height();
	//		double newWidthHeightRatio = newExtent.width() / newExtent.height();

	//		if ( currentWidthHeightRatio < newWidthHeightRatio )
	//		{
	//			//enlarge height of new extent
	//			double newHeight = newExtent.width() / currentWidthHeightRatio;
	//			newExtent.setYMinimum( newExtent.yMaximum() - newHeight );
	//		}
	//		else if ( currentWidthHeightRatio > newWidthHeightRatio )
	//		{
	//			//enlarge width of new extent
	//			double newWidth = currentWidthHeightRatio * newExtent.height();
	//			newExtent.setXMaximum( newExtent.xMinimum() + newWidth );
	//		}

	//		//fill text into line edits
	//		mXMinLineEdit->setText( QString::number( newExtent.xMinimum() ) );
	//		mXMaxLineEdit->setText( QString::number( newExtent.xMaximum() ) );
	//		mYMinLineEdit->setText( QString::number( newExtent.yMinimum() ) );
	//		mYMaxLineEdit->setText( QString::number( newExtent.yMaximum() ) );

	//		mGLW->beginCommand( tr( "Map extent changed" ) );
	//		mGLW->setNewExtent( newExtent );
	//		mGLW->endCommand();
	//	}
	//}
}

void CCipasComposerGLWidget::on_mXMinLineEdit_editingFinished()
{
	updateComposerExtentFromGui();
}

void CCipasComposerGLWidget::on_mXMaxLineEdit_editingFinished()
{
	updateComposerExtentFromGui();
}

void CCipasComposerGLWidget::on_mYMinLineEdit_editingFinished()
{
	updateComposerExtentFromGui();
}

void CCipasComposerGLWidget::on_mYMaxLineEdit_editingFinished()
{
	updateComposerExtentFromGui();
}



void CCipasComposerGLWidget::updateGuiElements()
{
	if ( mGLW )
	{
		blockAllSignals( true );

		//width, height, scale
		QRectF composerMapRect = mGLW->rect();
		int ww=mGLW->rect().width();
		int hh=mGLW->rect().height();
		mWidthLineEdit->setText( QString::number( composerMapRect.width() ) );
		mHeightLineEdit->setText( QString::number( composerMapRect.height() ) );
		//mScaleLineEdit->setText( QString::number( mGLW->scale(), 'f', 0 ) );

		//composer map extent
		CCipasRectangle composerMapExtent = mGLW->extent();
		mXMinLineEdit->setText( QString::number( composerMapExtent.xMinimum(), 'f', 3 ) );
		mXMaxLineEdit->setText( QString::number( composerMapExtent.xMaximum(), 'f', 3 ) );
		mYMinLineEdit->setText( QString::number( composerMapExtent.yMinimum(), 'f', 3 ) );
		mYMaxLineEdit->setText( QString::number( composerMapExtent.yMaximum(), 'f', 3 ) );

		//
		mLabelAxesTopChb->setChecked(mGLW->LabelAxesTop);
		mLabelAxesLeftChb->setChecked(mGLW->LabelAxesLeft);
		mLabelAxesBottomChb->setChecked(mGLW->LabelAxesBottom);
		mLabelAxesRightChb->setChecked(mGLW->LabelAxesRight);
		//grid
		if ( mGLW->gridEnabled() )
		{
			mGridCheckBox->setChecked( true );
		}
		else
		{
			mGridCheckBox->setChecked( false );
		}
		mIntervalXSpinBox->setValue( mGLW->gridIntervalX() );
		mIntervalYSpinBox->setValue( mGLW->gridIntervalY() );
		mOffsetXSpinBox->setValue( mGLW->gridOffsetX() );
		mOffsetYSpinBox->setValue( mGLW->gridOffsetY() );

		mDistanceToMapFrameSpinBox->setValue(mGLW->annotationFrameDistance());

		//CCipasComposerGLW::GridStyle gridStyle = mGLW->gridStyle();
		//if ( gridStyle == CCipasComposerGLW::Solid )
		//{
		//	/*mGridTypeComboBox->setCurrentIndex( mGridTypeComboBox->findText( QString::fromLocal8Bit("虚线") ) );*/
		//	mGridTypeComboBox->setCurrentIndex( 0 );
		//}
		//else
		//{
		//	/*mGridTypeComboBox->setCurrentIndex( mGridTypeComboBox->findText( QString::fromLocal8Bit("实线") ) );*/
		//	mGridTypeComboBox->setCurrentIndex( 1);
		//}

		mGridTypeComboBox->setCurrentIndex(mGLW->gridStyle());
		//mCrossWidthSpinBox->setValue( mGLW->crossLength() );

		//CCipasComposerGLW::GridAnnotationPosition annotationPos = mGLW->gridAnnotationPosition();
		//if ( annotationPos == CCipasComposerGLW::InsideMapFrame )
		//{
		//	mAnnotationPositionComboBox->setCurrentIndex( mAnnotationPositionComboBox->findText( QString::fromLocal8Bit("框架外") ) );
		//}
		//else
		//{
		//	mAnnotationPositionComboBox->setCurrentIndex( mAnnotationPositionComboBox->findText( QString::fromLocal8Bit("框架内") ) );
		//}
		mAnnotationPositionComboBox->setCurrentIndex(mGLW->gridAnnotationPosition());
		mDistanceToMapFrameSpinBox->setValue( mGLW->annotationFrameDistance() );

		if ( mGLW->showGridAnnotation() )
		{
			mDrawAnnotationCheckBox->setCheckState( Qt::Checked );
		}
		else
		{
			mDrawAnnotationCheckBox->setCheckState( Qt::Unchecked );
		}

		CCipasComposerGLW::GridAnnotationDirection dir = mGLW->gridAnnotationDirection();
		if ( dir == CCipasComposerGLW::Horizontal )
		{
			mAnnotationDirectionComboBox->setCurrentIndex( mAnnotationDirectionComboBox->findText( QString::fromLocal8Bit("水平")) );
		}
		else if ( dir == CCipasComposerGLW::Vertical )
		{
			mAnnotationDirectionComboBox->setCurrentIndex( mAnnotationDirectionComboBox->findText( QString::fromLocal8Bit("垂直") ) );
		}
		else if ( dir == CCipasComposerGLW::HorizontalAndVertical )
		{
			mAnnotationDirectionComboBox->setCurrentIndex( mAnnotationDirectionComboBox->findText( QString::fromLocal8Bit("水平和垂直") ) );
		}
		else //BoundaryDirection
		{
			mAnnotationDirectionComboBox->setCurrentIndex( mAnnotationDirectionComboBox->findText( QString::fromLocal8Bit("边界方向") ) );
		}

		mCoordinatePrecisionSpinBox->setValue( mGLW->gridAnnotationPrecision() );

		QPen gridPen = mGLW->gridPen();
		mLineWidthSpinBox->setValue( gridPen.widthF() );
		mLineColorButton->setColor( gridPen.color() );

		blockAllSignals( false );
	}
}

void CCipasComposerGLWidget::on_mLabelAxesButtonGroup_buttonClicked(int id)
{
	/*
	mLabelAxesButtonGroup->addButton(mLabelAxesTopChb,0);
	mLabelAxesButtonGroup->addButton(mLabelAxesLeftChb,1);
	mLabelAxesButtonGroup->addButton(mLabelAxesBottomChb,2);
	mLabelAxesButtonGroup->addButton(mLabelAxesRightChb,3);
	*/
	/*int currentButtonId=id;*/
	QList<QAbstractButton *> chbList=	mLabelAxesButtonGroup->buttons ();
	for (int i=0;i<chbList.count();i++)
	{
		QAbstractButton *chb=chbList.at(i);
		bool b= chb->isChecked();
		if (i==0)
		{
			mGLW->LabelAxesTop=b;
		}
		else if (i==1)
		{
			mGLW->LabelAxesLeft=b;
		}
		else if (i==2)
		{
			mGLW->LabelAxesBottom=b;
		}
		else
		{
			mGLW->LabelAxesRight=b;
		}
		mGLW->setReCalcGrid(true);
	}

	mGLW->updateComposition();
}

void CCipasComposerGLWidget::updateComposerExtentFromGui()
{
	if ( !mGLW )
	{
		return;
	}

	double xmin, ymin, xmax, ymax;
	bool conversionSuccess;

	xmin = mXMinLineEdit->text().toDouble( &conversionSuccess );
	if ( !conversionSuccess ) {return;}
	xmax = mXMaxLineEdit->text().toDouble( &conversionSuccess );
	if ( !conversionSuccess ) {return;}
	ymin = mYMinLineEdit->text().toDouble( &conversionSuccess );
	if ( !conversionSuccess ) {return;}
	ymax = mYMaxLineEdit->text().toDouble( &conversionSuccess );
	if ( !conversionSuccess ) {return;}

	mGLW->setGLExtent(xmin,ymin,xmax,ymax);
	//CCipasRectangle newExtent( xmin, ymin, xmax, ymax );
	//mGLW->beginCommand( tr( "Map extent changed" ) );
	//mGLW->setNewExtent( newExtent );
	//mGLW->endCommand();
}

void CCipasComposerGLWidget::blockAllSignals( bool b )
{
	mWidthLineEdit->blockSignals( b );
	mHeightLineEdit->blockSignals( b );
	//mScaleLineEdit->blockSignals( b );
	mXMinLineEdit->blockSignals( b );
	mXMaxLineEdit->blockSignals( b );
	mYMinLineEdit->blockSignals( b );
	mYMaxLineEdit->blockSignals( b );
	mIntervalXSpinBox->blockSignals( b );
	mIntervalYSpinBox->blockSignals( b );
	mOffsetXSpinBox->blockSignals( b );
	mOffsetYSpinBox->blockSignals( b );
	mGridTypeComboBox->blockSignals( b );
	//mCrossWidthSpinBox->blockSignals( b );
	//mPreviewModeComboBox->blockSignals( b );
	//mKeepLayerListCheckBox->blockSignals( b );
	mSetToMapCanvasExtentButton->blockSignals( b );
	//mUpdatePreviewButton->blockSignals( b );
	mLineWidthSpinBox->blockSignals( b );
	mLineColorButton->blockSignals( b );
	mDrawAnnotationCheckBox->blockSignals( b );
	mAnnotationFontButton->blockSignals( b );
	mAnnotationPositionComboBox->blockSignals( b );
	mDistanceToMapFrameSpinBox->blockSignals( b );
	mAnnotationDirectionComboBox->blockSignals( b );
	mCoordinatePrecisionSpinBox->blockSignals( b );
	//mDrawCanvasItemsCheckBox->blockSignals( b );
}

void CCipasComposerGLWidget::on_mGridCheckBox_toggled( bool state )
{
	if ( !mGLW )
	{
		return;
	}

	mGLW->beginCommand( tr( "Grid checkbox toggled" ) );
	mGLW->setGridEnabled( state );
	//double  ix=mIntervalXSpinBox->value();
	//double  iy=mIntervalYSpinBox->value();
	//mGLW->setGridIntervalX(ix);
	//mGLW->setGridIntervalY(iy);
	mGLW->setReCalcGrid(true);
	/*mGLW->updateBoundingRect();*/
	mGLW->updateComposition();
	mGLW->endCommand();
}

void CCipasComposerGLWidget::on_mIntervalXSpinBox_editingFinished()
{
	if ( !mGLW )
	{
		return;
	}

	mGLW->beginCommand( tr( "Grid interval changed" ) );
	mGLW->setGridIntervalX( mIntervalXSpinBox->value() );
	//mGLW->updateBoundingRect();
	mGLW->setReCalcGrid(true);
	mGLW->updateComposition();
	mGLW->endCommand();
}

void CCipasComposerGLWidget::on_mIntervalYSpinBox_editingFinished()
{
	if ( !mGLW )
	{
		return;
	}
	mGLW->beginCommand( tr( "Grid interval changed" ) );
	mGLW->setGridIntervalY( mIntervalYSpinBox->value() );
	//mGLW->updateBoundingRect();
	mGLW->setReCalcGrid(true);
	mGLW->updateComposition();
	mGLW->endCommand();
}

void CCipasComposerGLWidget::on_mOffsetXSpinBox_editingFinished()
{
	if ( !mGLW )
	{
		return;
	}
	mGLW->beginCommand( tr( "Grid offset changed" ) );
	mGLW->setGridOffsetX( mOffsetXSpinBox->value() );
	//mGLW->updateBoundingRect();
	mGLW->updateComposition();
	mGLW->endCommand();
}

void CCipasComposerGLWidget::on_mOffsetYSpinBox_editingFinished()
{
	if ( !mGLW )
	{
		return;
	}
	mGLW->beginCommand( tr( "Grid offset changed" ) );
	mGLW->setGridOffsetY( mOffsetYSpinBox->value() );
	//mGLW->updateBoundingRect();
	mGLW->updateComposition();
	mGLW->endCommand();
}

void CCipasComposerGLWidget::on_mLineWidthSpinBox_valueChanged( double d )
{
	if ( !mGLW )
	{
		return;
	}
	mGLW->beginCommand( tr( "Grid pen changed" ) );
	mGLW->setGridPenWidth( d );
	mGLW->setReCalcGrid(true);
	mGLW->updateComposition();
	mGLW->endCommand();
}

void CCipasComposerGLWidget::on_mLineColorButton_clicked()
{
	if ( !mGLW )
	{
		return;
	}
	QColor newColor = QColorDialog::getColor( mLineColorButton->color() );
	if ( newColor.isValid() )
	{
		mGLW->beginCommand( tr( "Grid pen changed" ) );
		mLineColorButton->setColor( newColor );
		mGLW->setReCalcGrid(true);
		mGLW->endCommand();
	}
	mGLW->updateComposition();
}

void CCipasComposerGLWidget::on_mGridTypeComboBox_currentIndexChanged( const QString& text )
{
	if ( !mGLW )
	{
		return;
	}

	if ( text == QString::fromLocal8Bit("虚线") )
	{
		mGLW->setGridStyle( CCipasComposerGLW::Cross );
		mGLW->setReCalcGrid(true);
	}
	else
	{
		mGLW->setGridStyle( CCipasComposerGLW::Solid );
		mGLW->setReCalcGrid(true);
	}
	mGLW->beginCommand( tr( "Grid type changed" ) );
	mGLW->updateComposition();
	mGLW->endCommand();
}
//
//void CCipasComposerGLWidget::on_mCrossWidthSpinBox_valueChanged( double d )
//{
//	if ( !mGLW )
//	{
//		return;
//	}
//
//	//mGLW->beginCommand( tr( "Grid cross width changed" ) );
//	mGLW->setCrossLength( d );
//	mGLW->update();
//	//mGLW->endCommand();
//}

void CCipasComposerGLWidget::on_mAnnotationFontButton_clicked()
{
	if ( !mGLW )
	{
		return;
	}

	bool ok;
#if defined(Q_WS_MAC) && QT_VERSION >= 0x040500 && defined(QT_MAC_USE_COCOA)
	// Native Mac dialog works only for Qt Carbon
	QFont newFont = QFontDialog::getFont( &ok, mGLW->gridAnnotationFont(), this, QString(), QFontDialog::DontUseNativeDialog );
#else
	QFont newFont = QFontDialog::getFont( &ok, mGLW->gridAnnotationFont(), this );
#endif
	if ( ok )
	{
		mGLW->beginCommand( tr( "Annotation font changed" ) );
		mGLW->setGridAnnotationFont( newFont );
		//mGLW->updateBoundingRect();
		mGLW->updateComposition();
		mGLW->endCommand();
	}
}

void CCipasComposerGLWidget::on_mAnnotationColorButton_clicked()
{
	if ( !mGLW )
	{
		return;
	}
	QColor newColor = QColorDialog::getColor( mAnnotationColorButton->color() );
	if ( newColor.isValid() )
	{
		mAnnotationColorButton->setColor(newColor);
		mGLW->setGridAnnotationColor( newColor );
	}
	mGLW->updateComposition();
}

void CCipasComposerGLWidget::on_mDistanceToMapFrameSpinBox_valueChanged( double d )
{
	if ( !mGLW )
	{
		return;
	}
	mGLW->beginCommand( tr( "Annotation distance changed" ), CipasComposerMergeCommand::ComposerMapAnnotationDistance );
	mGLW->setAnnotationFrameDistance( d );
	//mGLW->updateBoundingRect();
	mGLW->updateComposition();
	mGLW->endCommand();
}

void CCipasComposerGLWidget::on_mAnnotationPositionComboBox_currentIndexChanged( const QString& text )
{
	if ( !mGLW )
	{
		return;
	}

	mGLW->beginCommand( tr( "Annotation position changed" ) );
	if ( text == QString::fromLocal8Bit("框架内") )
	{
		mGLW->setGridAnnotationPosition( CCipasComposerGLW::InsideMapFrame );
	}
	else
	{
		mGLW->setGridAnnotationPosition( CCipasComposerGLW::OutsideMapFrame );
	}
	//mGLW->updateBoundingRect();
	mGLW->updateComposition();
	mGLW->endCommand();
}

void CCipasComposerGLWidget::on_mDrawAnnotationCheckBox_stateChanged( int state )
{
	if ( !mGLW )
	{
		return;
	}

	mGLW->beginCommand( tr( "Annotation toggled" ) );
	if ( state == Qt::Checked )
	{
		mGLW->setShowGridAnnotation( true );
	}
	else
	{
		mGLW->setShowGridAnnotation( false );
	}
	//mGLW->updateBoundingRect();
	mGLW->updateComposition();
	mGLW->endCommand();
}

void CCipasComposerGLWidget::on_mAnnotationDirectionComboBox_currentIndexChanged( const QString& text )
{
	if ( !mGLW )
	{
		return;
	}

	mGLW->beginCommand( tr( "Changed annotation direction" ) );
	if ( text == QString::fromLocal8Bit("水平"))
	{
		mGLW->setGridAnnotationDirection( CCipasComposerGLW::Horizontal );
	}
	else if ( text == QString::fromLocal8Bit("垂直") )
	{
		mGLW->setGridAnnotationDirection( CCipasComposerGLW::Vertical );
	}
	else if ( text == QString::fromLocal8Bit("水平和垂直") )
	{
		mGLW->setGridAnnotationDirection( CCipasComposerGLW::HorizontalAndVertical );
	}
	else //BoundaryDirection
	{
		mGLW->setGridAnnotationDirection( CCipasComposerGLW::BoundaryDirection );
	}
	//mGLW->updateBoundingRect();
	mGLW->updateComposition();
	mGLW->endCommand();
}

void CCipasComposerGLWidget::on_mCoordinatePrecisionSpinBox_valueChanged( int value )
{
	if ( !mGLW )
	{
		return;
	}
	mGLW->beginCommand( tr( "Changed annotation precision" ) );
	mGLW->setGridAnnotationPrecision( value );
	//mGLW->updateBoundingRect();
	mGLW->updateComposition();
	mGLW->endCommand();
}

void CCipasComposerGLWidget::on_mAnnotationPositionComboBox2_currentIndexChanged( const QString& text )
{
	if ( !mGLW )
	{
		return;
	}

	if ( text == QString::fromLocal8Bit("框架内") )
	{
		mGLW->setGridPosition( CCipasComposerGLW::InsideMapFrame );
		mGLW->setReCalcGrid(true);
	}
	else
	{
		mGLW->setGridPosition( CCipasComposerGLW::OutsideMapFrame );
		mGLW->setReCalcGrid(true);
	}
	mGLW->updateComposition();
}

void CCipasComposerGLWidget::on_mAnnotationStyleComboBox_currentIndexChanged(  int index )
{
	if ( !mGLW )
	{
		return;
	}
	mGLW->setAnnotationStyle((CCipasComposerGLW::AnnotationStyle)index);
	mGLW->updateComposition();

}