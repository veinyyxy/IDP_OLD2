#include <QColorDialog>
#include <QFontDialog>
#include <QWidget>


#include "CipasComposerScaleBar.h"
#include "CipasComposerItemWidget.h"
#include "CipasComposition.h"
#include "CipasComposerScaleBarWidget.h"

CCipasComposerScaleBarWidget::CCipasComposerScaleBarWidget(CCipasComposerScaleBar* scaleBar): QWidget()
	, mComposerScaleBar( scaleBar )
{
	setupUi( this );

	//add widget for general composer item properties
	CCipasComposerItemWidget* itemPropertiesWidget = new CCipasComposerItemWidget( this, scaleBar );
	toolBox->addItem( itemPropertiesWidget, tr("常规选项") );

	blockMemberSignals( true );
	mStyleComboBox->insertItem( 0, tr( "Single Box" ) );		//已经添加
	mStyleComboBox->insertItem( 1, tr( "Double Box" ) );		//下面几个类尚未添加
	mStyleComboBox->insertItem( 2, tr( "Line Ticks Middle" ) );
	mStyleComboBox->insertItem( 3, tr( "Line Ticks Down" ) );
	mStyleComboBox->insertItem( 4, tr( "Line Ticks Up" ) );
	mStyleComboBox->insertItem( 5, tr( "Numeric" ) );
	//scaleBar->setUnitLabeling( tr("度") );
	//赋缺省值需要更新框的范围
	setGuiElements(); //set the GUI elements to the state of scaleBar
	blockMemberSignals( false );
}


CCipasComposerScaleBarWidget::~CCipasComposerScaleBarWidget(void)
{
}



void CCipasComposerScaleBarWidget::refreshMapComboBox()
{
	//save the current entry in case it is still present after refresh
	QString saveCurrentComboText = mMapComboBox->currentText();

	mMapComboBox->clear();

	if ( mComposerScaleBar )
	{
		//insert available maps into mMapComboBox
		const CCipasComposition* scaleBarComposition = mComposerScaleBar->composition();
		if ( scaleBarComposition )
		{
			
		}
	}

	if ( mMapComboBox->findText( saveCurrentComboText ) == -1 )
	{
		//the former entry is no longer present. Inform the scalebar about the changed composer map
		on_mMapComboBox_activated( mMapComboBox->currentText() );
	}
	else
	{
		//the former entry is still present. Make it the current entry again
		mMapComboBox->setCurrentIndex( mMapComboBox->findText( saveCurrentComboText ) );
	}
}

void CCipasComposerScaleBarWidget::showEvent( QShowEvent * event )
{
	refreshMapComboBox();
	QWidget::showEvent( event );
}

void CCipasComposerScaleBarWidget::on_mMapComboBox_activated( const QString& text )
{
	if ( !mComposerScaleBar || text.isEmpty() )
	{
		return;
	}

	const CCipasComposition* comp = mComposerScaleBar->composition();
	if ( !comp )
	{
		return;
	}

	//extract id
	int id;
	bool conversionOk;
	QStringList textSplit = text.split( " " );
	if ( textSplit.size() < 1 )
	{
		return;
	}

	QString idString = textSplit.at( textSplit.size() - 1 );
	id = idString.toInt( &conversionOk );

	if ( !conversionOk )
	{
		return;
	}
}

void CCipasComposerScaleBarWidget::setGuiElements()
{
	if ( !mComposerScaleBar )
	{
		return;
	}

	mNumberOfSegmentsSpinBox->setValue( mComposerScaleBar->numSegments() );
	mSegmentsLeftSpinBox->setValue( mComposerScaleBar->numSegmentsLeft() );
	mSegmentSizeSpinBox->setValue( mComposerScaleBar->numUnitsPerSegment() );
	mLineWidthSpinBox->setValue( mComposerScaleBar->pen().widthF() );
	mHeightSpinBox->setValue( mComposerScaleBar->height() );
	mMapUnitsPerBarUnitSpinBox->setValue( mComposerScaleBar->numMapUnitsPerScaleBarUnit() );
	mLabelBarSpaceSpinBox->setValue( mComposerScaleBar->labelBarSpace() );
	mBoxSizeSpinBox->setValue( mComposerScaleBar->boxContentSpace() );
	mUnitLabelLineEdit->setText( mComposerScaleBar->unitLabeling() );

	QString style = mComposerScaleBar->style();
	mStyleComboBox->setCurrentIndex( mStyleComboBox->findText( tr( style.toLocal8Bit().data() ) ) );
}

void CCipasComposerScaleBarWidget::on_mLineWidthSpinBox_valueChanged( double d )
{
	if ( !mComposerScaleBar )
	{
		return;
	}

	mComposerScaleBar->beginCommand( tr( "Scalebar line width" ), CipasComposerMergeCommand::ScaleBarLineWidth );
	QPen newPen( QColor( 0, 0, 0 ) );
	newPen.setWidthF( d );
	mComposerScaleBar->setPen( newPen );
	mComposerScaleBar->update();
	mComposerScaleBar->endCommand();
}

void CCipasComposerScaleBarWidget::on_mSegmentSizeSpinBox_valueChanged( double d )
{
	if ( !mComposerScaleBar )
	{
		return;
	}

	mComposerScaleBar->beginCommand( tr( "Scalebar segment size" ), CipasComposerMergeCommand::ScaleBarSegmentSize );
	mComposerScaleBar->setNumUnitsPerSegment( d );
	mComposerScaleBar->update();
	mComposerScaleBar->endCommand();
}

void CCipasComposerScaleBarWidget::on_mSegmentsLeftSpinBox_valueChanged( int i )
{
	if ( !mComposerScaleBar )
	{
		return;
	}

	mComposerScaleBar->beginCommand( tr( "Scalebar segments left" ), CipasComposerMergeCommand::ScaleBarSegmentsLeft );
	mComposerScaleBar->setNumSegmentsLeft( i );
	mComposerScaleBar->update();
	mComposerScaleBar->endCommand();
}

void CCipasComposerScaleBarWidget::on_mNumberOfSegmentsSpinBox_valueChanged( int i )
{
	if ( !mComposerScaleBar )
	{
		return;
	}

	mComposerScaleBar->beginCommand( tr( "Scalebar n segments" ), CipasComposerMergeCommand::ScaleBarNSegments );
	mComposerScaleBar->setNumSegments( i );
	mComposerScaleBar->update();
	mComposerScaleBar->endCommand();
}

void CCipasComposerScaleBarWidget::on_mHeightSpinBox_valueChanged( int i )
{
	if ( !mComposerScaleBar )
	{
		return;
	}
	mComposerScaleBar->beginCommand( tr( "Scalebar height changed" ), CipasComposerMergeCommand::ScaleBarHeight );
	mComposerScaleBar->setHeight( i );
	mComposerScaleBar->update();
	mComposerScaleBar->endCommand();
}

void CCipasComposerScaleBarWidget::on_mFontButton_clicked()
{
	if ( !mComposerScaleBar )
	{
		return;
	}

	bool dialogAccepted;
	QFont oldFont = mComposerScaleBar->font();
#if defined(Q_WS_MAC) && QT_VERSION >= 0x040500 && defined(QT_MAC_USE_COCOA)
	// Native Mac dialog works only for Qt Carbon
	QFont newFont = QFontDialog::getFont( &dialogAccepted, oldFont, 0, QString(), QFontDialog::DontUseNativeDialog );
#else
	QFont newFont = QFontDialog::getFont( &dialogAccepted, oldFont, 0 );
#endif
	if ( dialogAccepted )
	{
		mComposerScaleBar->beginCommand( tr( "Scalebar font changed" ) );
		mComposerScaleBar->setFont( newFont );
		mComposerScaleBar->endCommand();
	}
	mComposerScaleBar->update();
}

void CCipasComposerScaleBarWidget::on_mColorPushButton_clicked()
{
	if ( !mComposerScaleBar )
	{
		return;
	}

	QColor oldColor = mComposerScaleBar->brush().color();
	QColor newColor = QColorDialog::getColor( oldColor, 0 );

	if ( !newColor.isValid() ) //user canceled the dialog
	{
		return;
	}

	mComposerScaleBar->beginCommand( tr( "Scalebar color changed" ) );
	QBrush newBrush( newColor );
	mComposerScaleBar->setBrush( newBrush );
	mComposerScaleBar->update();
	mComposerScaleBar->endCommand();
}

void CCipasComposerScaleBarWidget::on_mUnitLabelLineEdit_textChanged( const QString& text )
{
	if ( !mComposerScaleBar )
	{
		return;
	}

	mComposerScaleBar->beginCommand( tr( "Scalebar unit text" ), CipasComposerMergeCommand::ScaleBarUnitText );
	mComposerScaleBar->setUnitLabeling( text );
	mComposerScaleBar->update();
	mComposerScaleBar->endCommand();
}

void CCipasComposerScaleBarWidget::on_mMapUnitsPerBarUnitSpinBox_valueChanged( double d )
{
	if ( !mComposerScaleBar )
	{
		return;
	}

	mComposerScaleBar->beginCommand( tr( "Scalebar map units per segment" ), CipasComposerMergeCommand::ScaleBarMapUnitsSegment );
	mComposerScaleBar->setNumMapUnitsPerScaleBarUnit( d );
	mComposerScaleBar->update();
	mComposerScaleBar->endCommand();
}

void CCipasComposerScaleBarWidget::on_mStyleComboBox_currentIndexChanged( const QString& text )
{
	if ( !mComposerScaleBar )
	{
		return;
	}

	mComposerScaleBar->beginCommand( tr( "Scalebar style changed" ) );
	QString untranslatedStyleName;
	if ( text == tr( "Single Box" ) )
	{
		untranslatedStyleName = "Single Box";
	}
	else if ( text == tr( "Double Box" ) )
	{
		untranslatedStyleName = "Double Box";
	}
	else if ( text == tr( "Line Ticks Middle" ) )
	{
		untranslatedStyleName = "Line Ticks Middle";
	}
	else if ( text == tr( "Line Ticks Middle" ) )
	{
		untranslatedStyleName = "Line Ticks Middle";
	}
	else if ( text == tr( "Line Ticks Down" ) )
	{
		untranslatedStyleName = "Line Ticks Down";
	}
	else if ( text == tr( "Line Ticks Up" ) )
	{
		untranslatedStyleName = "Line Ticks Up";
	}
	else if ( text == tr( "Numeric" ) )
	{
		untranslatedStyleName = "Numeric";
	}
	mComposerScaleBar->setStyle( untranslatedStyleName );
	mComposerScaleBar->update();
	mComposerScaleBar->endCommand();
}

void CCipasComposerScaleBarWidget::on_mLabelBarSpaceSpinBox_valueChanged( double d )
{
	if ( !mComposerScaleBar )
	{
		return;
	}

	mComposerScaleBar->beginCommand( tr( "Scalebar label bar space" ), CipasComposerMergeCommand::ScaleBarLabelBarSize );
	mComposerScaleBar->setLabelBarSpace( d );
	mComposerScaleBar->update();
	mComposerScaleBar->endCommand();
}

void CCipasComposerScaleBarWidget::on_mBoxSizeSpinBox_valueChanged( double d )
{
	if ( !mComposerScaleBar )
	{
		return;
	}

	mComposerScaleBar->beginCommand( tr( "Scalebar box content space" ), CipasComposerMergeCommand::ScaleBarBoxContentSpace );
	mComposerScaleBar->setBoxContentSpace( d );
	mComposerScaleBar->update();
	mComposerScaleBar->endCommand();
}

void CCipasComposerScaleBarWidget::blockMemberSignals( bool block )
{
	mSegmentSizeSpinBox->blockSignals( block );
	mNumberOfSegmentsSpinBox->blockSignals( block );
	mSegmentsLeftSpinBox->blockSignals( block );
	mStyleComboBox->blockSignals( block );
	mUnitLabelLineEdit->blockSignals( block );
	mMapUnitsPerBarUnitSpinBox->blockSignals( block );
	mMapComboBox->blockSignals( block );
	mHeightSpinBox->blockSignals( block );
	mLineWidthSpinBox->blockSignals( block );
	mLabelBarSpaceSpinBox->blockSignals( block );
	mBoxSizeSpinBox->blockSignals( block );
}