
#include <QColorDialog>
#include <QFontDialog>
#include <QWidget>

#include "CipasComposerItemWidget.h"
#include "CipasComposerLabel.h"
#include "CipasComposerLabelWidget.h"

CCipasComposerLabelWidget::CCipasComposerLabelWidget(CCipasComposerLabel* label): QWidget(), mComposerLabel( label )
{
	setupUi( this );

	//add widget for general composer item properties
	CCipasComposerItemWidget* itemPropertiesWidget = new CCipasComposerItemWidget( this, label );
	toolBox->addItem( itemPropertiesWidget, tr("常规选项") );
	advanceLabeCheckBox->setChecked(mComposerLabel->IsAdvanced());
	if ( mComposerLabel )
	{
		setGuiElementValues();
		connect( mComposerLabel, SIGNAL( itemChanged() ), this, SLOT( setGuiElementValues() ) );
	}
}

CCipasComposerLabelWidget::CCipasComposerLabelWidget(CCipasComposerLabel* label, QString & lyrName): QWidget(), mComposerLabel( label )
{

	setupUi( this );

	//add widget for general composer item properties
	CCipasComposerItemWidget* itemPropertiesWidget = new CCipasComposerItemWidget( this, label );
	toolBox->addItem( itemPropertiesWidget, QString::fromLocal8Bit("常规选项") );
	advanceLabeCheckBox->setChecked(mComposerLabel->IsAdvanced());
	if ( mComposerLabel )
	{
		setGuiElementValues();
		connect( mComposerLabel, SIGNAL( itemChanged() ), this, SLOT( setGuiElementValues() ) );
		mComposerLabel->setLayerName(lyrName);
	}
}

void CCipasComposerLabelWidget::setLayerName(const QString & lyrName)
{
	mLyrName=lyrName;
}

QString CCipasComposerLabelWidget::getLayerName()const
{
	return mLyrName;
}

//CCipasComposerLabelWidget::~CCipasComposerLabelWidget(void)
//{
//}


void CCipasComposerLabelWidget::on_mTextEdit_textChanged()
{
	if ( mComposerLabel )
	{
		mComposerLabel->beginCommand( tr( "Label text changed" ), CipasComposerMergeCommand::ComposerLabelSetText );
		mComposerLabel->setText( mTextEdit->toPlainText() );
		mComposerLabel->update();
		mComposerLabel->endCommand();
	}
}

void CCipasComposerLabelWidget::on_mFontButton_clicked()
{
	if ( mComposerLabel )
	{
		bool ok;
#if defined(Q_WS_MAC) && QT_VERSION >= 0x040500 && defined(QT_MAC_USE_COCOA)
		// Native Mac dialog works only for Qt Carbon
		QFont newFont = QFontDialog::getFont( &ok, mComposerLabel->font(), this, QString(), QFontDialog::DontUseNativeDialog );
#else
		QFont newFont = QFontDialog::getFont( &ok, mComposerLabel->font(), this );
#endif
		if ( ok )
		{
			mComposerLabel->beginCommand( tr( "Label font changed" ) );
			mComposerLabel->setFont( newFont );
			mComposerLabel->update();
			mComposerLabel->endCommand();
		}
	}
}

void CCipasComposerLabelWidget::on_mMarginDoubleSpinBox_valueChanged( double d )
{
	if ( mComposerLabel )
	{
		mComposerLabel->beginCommand( tr( "Label margin changed" ) );
		mComposerLabel->setMargin( d );
		mComposerLabel->update();
		mComposerLabel->endCommand();
	}
}

void CCipasComposerLabelWidget::on_mFontColorButton_clicked()
{
	if ( !mComposerLabel )
	{
		return;
	}
	QColor newColor = QColorDialog::getColor( mComposerLabel->fontColor() );
	if ( !newColor.isValid() )
	{
		return;
	}
	mComposerLabel->beginCommand( tr( "Label font changed" ) );
	mComposerLabel->setFontColor( newColor );
	mComposerLabel->endCommand();
}

void CCipasComposerLabelWidget::on_mCenterRadioButton_clicked()
{
	if ( mComposerLabel )
	{
		mComposerLabel->beginCommand( tr( "Label alignment changed" ) );
		mComposerLabel->setHAlign( Qt::AlignHCenter );
		mComposerLabel->update();
		mComposerLabel->endCommand();
	}
}

void CCipasComposerLabelWidget::on_mRightRadioButton_clicked()
{
	if ( mComposerLabel )
	{
		mComposerLabel->beginCommand( tr( "Label alignment changed" ) );
		mComposerLabel->setHAlign( Qt::AlignRight );
		mComposerLabel->update();
		mComposerLabel->endCommand();
	}
}

void CCipasComposerLabelWidget::on_mLeftRadioButton_clicked()
{
	if ( mComposerLabel )
	{
		mComposerLabel->beginCommand( tr( "Label alignment changed" ) );
		mComposerLabel->setHAlign( Qt::AlignLeft );
		mComposerLabel->update();
		mComposerLabel->endCommand();
	}
}

void CCipasComposerLabelWidget::on_mTopRadioButton_clicked()
{
	if ( mComposerLabel )
	{
		mComposerLabel->beginCommand( tr( "Label alignment changed" ) );
		mComposerLabel->setVAlign( Qt::AlignTop );
		mComposerLabel->update();
		mComposerLabel->endCommand();
	}
}

void CCipasComposerLabelWidget::on_mBottomRadioButton_clicked()
{
	if ( mComposerLabel )
	{
		mComposerLabel->beginCommand( tr( "Label alignment changed" ) );
		mComposerLabel->setVAlign( Qt::AlignBottom );
		mComposerLabel->update();
		mComposerLabel->endCommand();
	}
}

void CCipasComposerLabelWidget::on_mMiddleRadioButton_clicked()
{
	if ( mComposerLabel )
	{
		mComposerLabel->beginCommand( tr( "Label alignment changed" ) );
		mComposerLabel->setVAlign( Qt::AlignVCenter );
		mComposerLabel->update();
		mComposerLabel->endCommand();
	}
}

void CCipasComposerLabelWidget::on_mLabelIdLineEdit_textChanged( const QString& text )
{
	if ( mComposerLabel )
	{
		mComposerLabel->beginCommand( tr( "Label id changed" ), CipasComposerMergeCommand::ComposerLabelSetId );
		mComposerLabel->setId( text );
		mComposerLabel->endCommand();
	}
}

void CCipasComposerLabelWidget::setGuiElementValues()
{
	blockAllSignals( true );
	mTextEdit->setText( mComposerLabel->text() );
	mMarginDoubleSpinBox->setValue( mComposerLabel->margin() );
	mTopRadioButton->setChecked( mComposerLabel->vAlign() == Qt::AlignTop );
	mMiddleRadioButton->setChecked( mComposerLabel->vAlign() == Qt::AlignVCenter );
	mBottomRadioButton->setChecked( mComposerLabel->vAlign() == Qt::AlignBottom );
	mLeftRadioButton->setChecked( mComposerLabel->hAlign() == Qt::AlignLeft );
	mCenterRadioButton->setChecked( mComposerLabel->hAlign() == Qt::AlignHCenter );
	mRightRadioButton->setChecked( mComposerLabel->hAlign() == Qt::AlignRight );
	mLabelIdLineEdit->setText( mComposerLabel->text() );
	blockAllSignals( false );
}

void CCipasComposerLabelWidget::blockAllSignals( bool block )
{
	mTextEdit->blockSignals( block );
	mMarginDoubleSpinBox->blockSignals( block );
	mTopRadioButton->blockSignals( block );
	mMiddleRadioButton->blockSignals( block );
	mBottomRadioButton->blockSignals( block );
	mLeftRadioButton->blockSignals( block );
	mCenterRadioButton->blockSignals( block );
	mRightRadioButton->blockSignals( block );
	mLabelIdLineEdit->blockSignals( block );
}

void CCipasComposerLabelWidget::on_advanceLabeCheckBox_stateChanged(int state)
{
	mComposerLabel->setAdvaced(state);
}