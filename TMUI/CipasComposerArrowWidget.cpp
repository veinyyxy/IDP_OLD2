
#include <QColorDialog>
#include <QFileDialog>
#include <QFileInfo>

#include "CipasComposerArrow.h"
#include "CipasComposerItemWidget.h"
#include "CipasComposerArrowWidget.h"

CCipasComposerArrowWidget::CCipasComposerArrowWidget(CCipasComposerArrow* arrow): QWidget( 0 ), mArrow( arrow )
{
	setupUi( this );
	mRadioButtonGroup = new QButtonGroup( this );
	mRadioButtonGroup->addButton( mDefaultMarkerRadioButton );
	mRadioButtonGroup->addButton( mNoMarkerRadioButton );
	//mRadioButtonGroup->addButton( mSvgMarkerRadioButton );
	mRadioButtonGroup->setExclusive( true );

	//disable the svg related gui elements by default
	//on_mSvgMarkerRadioButton_toggled( false );

	//add widget for general composer item properties
	CCipasComposerItemWidget* itemPropertiesWidget = new CCipasComposerItemWidget( this, mArrow );
	toolBox->addItem( itemPropertiesWidget, tr("常规选项") );

	setGuiElementValues();

	if ( arrow )
	{
		connect( arrow, SIGNAL( itemChanged() ), this, SLOT( setGuiElementValues() ) );
	}
}


CCipasComposerArrowWidget::~CCipasComposerArrowWidget(void)
{
}


void CCipasComposerArrowWidget::on_mOutlineWidthSpinBox_valueChanged( double d )
{
	if ( !mArrow )
	{
		return;
	}

	mArrow->beginCommand( tr( "Arrow outline width" ), CipasComposerMergeCommand::ArrowOutlineWidth );
	mArrow->setOutlineWidth( d );
	mArrow->update();
	mArrow->endCommand();
}

void CCipasComposerArrowWidget::on_mArrowHeadWidthSpinBox_valueChanged( double d )
{
	if ( !mArrow )
	{
		return;
	}

	mArrow->beginCommand( tr( "Arrowhead width" ), CipasComposerMergeCommand::ArrowHeadWidth );
	mArrow->setArrowHeadWidth( d );
	mArrow->update();
	mArrow->endCommand();
}

void CCipasComposerArrowWidget::on_mArrowColorButton_clicked()
{
	if ( !mArrow )
	{
		return;
	}

#if QT_VERSION >= 0x040500
	QColor newColor = QColorDialog::getColor( mArrow->arrowColor(), 0, tr( "Arrow color" ), QColorDialog::ShowAlphaChannel );
#else
	QColor newColor = QColorDialog::getColor( mArrow->arrowColor() );
#endif
	if ( newColor.isValid() )
	{
		mArrow->beginCommand( tr( "Arrow color changed" ) );
		mArrow->setArrowColor( newColor );
		mArrow->update();
		mArrow->endCommand();
	}
}

void CCipasComposerArrowWidget::blockAllSignals( bool block )
{
	mArrowColorButton->blockSignals( block );
	mOutlineWidthSpinBox->blockSignals( block );
	mArrowHeadWidthSpinBox->blockSignals( block );
	mDefaultMarkerRadioButton->blockSignals( block );
	mNoMarkerRadioButton->blockSignals( block );
	//mSvgMarkerRadioButton->blockSignals( block );
	//mStartMarkerLineEdit->blockSignals( block );
	//mStartMarkerToolButton->blockSignals( block );
	//mEndMarkerLineEdit->blockSignals( block );
	//mEndMarkerToolButton->blockSignals( block );
}

void CCipasComposerArrowWidget::setGuiElementValues()
{
	if ( !mArrow )
	{
		return;
	}

	blockAllSignals( true );
	mOutlineWidthSpinBox->setValue( mArrow->outlineWidth() );
	mArrowHeadWidthSpinBox->setValue( mArrow->arrowHeadWidth() );

	CCipasComposerArrow::MarkerMode mode = mArrow->markerMode();
	if ( mode == CCipasComposerArrow::DefaultMarker )
	{
		mDefaultMarkerRadioButton->setChecked( true );
	}
	else if ( mode == CCipasComposerArrow::NoMarker )
	{
		mNoMarkerRadioButton->setChecked( true );
	}
	//else //svg marker
	//{
	//	mSvgMarkerRadioButton->setChecked( true );
	//	enableSvgInputElements( true );
	//}
	//mStartMarkerLineEdit->setText( mArrow->startMarker() );
	//mEndMarkerLineEdit->setText( mArrow->endMarker() );
	blockAllSignals( false );
}

//void CCipasComposerArrowWidget::enableSvgInputElements( bool enable )
//{
//	mStartMarkerLineEdit->setEnabled( enable );
//	mStartMarkerToolButton->setEnabled( enable );
//	mEndMarkerLineEdit->setEnabled( enable );
//	mEndMarkerToolButton->setEnabled( enable );
//}

void CCipasComposerArrowWidget::on_mDefaultMarkerRadioButton_toggled( bool toggled )
{
	if ( mArrow && toggled )
	{
		mArrow->beginCommand( tr( "Arrow marker changed" ) );
		mArrow->setMarkerMode( CCipasComposerArrow::DefaultMarker );
		mArrow->update();
		mArrow->endCommand();
	}
}

void CCipasComposerArrowWidget::on_mNoMarkerRadioButton_toggled( bool toggled )
{
	if ( mArrow && toggled )
	{
		mArrow->beginCommand( tr( "Arrow marker changed" ) );
		mArrow->setMarkerMode( CCipasComposerArrow::NoMarker );
		mArrow->update();
		mArrow->endCommand();
	}
}

//void CCipasComposerArrowWidget::on_mSvgMarkerRadioButton_toggled( bool toggled )
//{
//	enableSvgInputElements( toggled );
//	if ( mArrow && toggled )
//	{
//		//mArrow->beginCommand( tr( "Arrow marker changed" ) );
//		mArrow->setMarkerMode( CCipasComposerArrow::SVGMarker );
//		mArrow->update();
//		//mArrow->endCommand();
//	}
//}
/*
void CCipasComposerArrowWidget::on_mStartMarkerLineEdit_editingFinished( const QString & text )
{
	if ( mArrow )
	{
		//mArrow->beginCommand( tr( "Arrow start marker" ) );
		QFileInfo fi( text );
		if ( fi.exists() )
		{
			mArrow->setStartMarker( text );
		}
		else
		{
			mArrow->setStartMarker( "" );
		}
		mArrow->update();
		//mArrow->endCommand();
	}
}

void CCipasComposerArrowWidget::on_mEndMarkerLineEdit_editingFinished( const QString & text )
{
	if ( mArrow )
	{
		//mArrow->beginCommand( tr( "Arrow start marker" ) );
		QFileInfo fi( text );
		if ( fi.exists() )
		{
			mArrow->setEndMarker( text );
		}
		else
		{
			mArrow->setEndMarker( "" );
		}
		mArrow->update();
		//mArrow->endCommand();
	}
}

void CCipasComposerArrowWidget::on_mStartMarkerToolButton_clicked()
{
	QFileInfo fi( mStartMarkerLineEdit->text() );
	QString svgFileName = QFileDialog::getOpenFileName( 0, tr( "Start marker svg file" ), fi.dir().absolutePath() );
	if ( !svgFileName.isNull() )
	{
		//mArrow->beginCommand( tr( "Arrow start marker" ) );
		mStartMarkerLineEdit->setText( svgFileName );
		//mArrow->endCommand();
	}
}

void CCipasComposerArrowWidget::on_mEndMarkerToolButton_clicked()
{
	QFileInfo fi( mEndMarkerLineEdit->text() );
	QString svgFileName = QFileDialog::getOpenFileName( 0, tr( "End marker svg file" ), fi.dir().absolutePath() );
	if ( !svgFileName.isNull() )
	{
		//mArrow->beginCommand( tr( "Arrow end marker" ) );
		mEndMarkerLineEdit ->setText( svgFileName );
		//mArrow->endCommand();
	}
}

*/