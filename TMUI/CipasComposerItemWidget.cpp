
#include <QColorDialog>
#include <QPen>

#include "ComposerItem.h"
#include "CipasComposerItemWidget.h"

CCipasComposerItemWidget::CCipasComposerItemWidget(QWidget* parent, CComposerItem* item):QWidget( parent ), mItem( item )
{
	setupUi( this );
	setValuesForGuiElements();
}

CCipasComposerItemWidget::CCipasComposerItemWidget(): QWidget( 0 ), mItem( 0 )
{

}

CCipasComposerItemWidget::~CCipasComposerItemWidget(void)
{
}

//slots
void CCipasComposerItemWidget::on_mFrameColorButton_clicked()
{
	if ( !mItem )
	{
		return;
	}

	QColor newFrameColor = QColorDialog::getColor( mItem->pen().color(), 0 );
	if ( !newFrameColor.isValid() )
	{
		return; //dialog canceled
	}

	//mItem->beginCommand( tr( "Frame color changed" ) );
	QPen thePen;
	thePen.setColor( newFrameColor );
	thePen.setWidthF( mOutlineWidthSpinBox->value() );

	mItem->setPen( thePen );
	mItem->update();
	//mItem->endCommand();
}

void CCipasComposerItemWidget::on_mBackgroundColorButton_clicked()
{
	if ( !mItem )
	{
		return;
	}

	QColor newBackgroundColor = QColorDialog::getColor( mItem->brush().color(), 0 );
	if ( !newBackgroundColor.isValid() )
	{
		return; //dialog canceled
	}

	//mItem->beginCommand( tr( "Background color changed" ) );
	newBackgroundColor.setAlpha( mOpacitySlider->value() );
	mItem->setBrush( QBrush( QColor( newBackgroundColor ), Qt::SolidPattern ) );
	//if the item is a composer map, we need to regenerate the map image
	//because it usually is cached
	//QgsComposerMap* cm = dynamic_cast<QgsComposerMap *>( mItem );
	//if ( cm )
	//{
	//	cm->cache();
	//}
	mItem->update();
	//mItem->endCommand();
}

void CCipasComposerItemWidget::on_mOpacitySlider_sliderReleased()
{
	if ( !mItem )
	{
		return;
	}
	int value = mOpacitySlider->value();

	//mItem->beginCommand( tr( "Item opacity changed" ) );
	QBrush itemBrush = mItem->brush();
	QColor brushColor = itemBrush.color();
	brushColor.setAlpha( value );
	mItem->setBrush( QBrush( brushColor ) );
	mItem->update();
	//mItem->endCommand();
}

void CCipasComposerItemWidget::on_mOutlineWidthSpinBox_valueChanged( double d )
{
	if ( !mItem )
	{
		return;
	}

	//mItem->beginCommand( tr( "Item outline width" ), CipasComposerMergeCommand::ItemOutlineWidth );
	QPen itemPen = mItem->pen();
	itemPen.setWidthF( d );
	mItem->setPen( itemPen );
	m_width = d;

	if (m_width==0)
	{
		mItem->setFrame( false );
	}
	else if (m_ischeck)
	{
		mItem->setFrame( true );
	}
	mItem->update();
	//mItem->endCommand();
}

void CCipasComposerItemWidget::on_mFrameCheckBox_stateChanged( int state )
{
	if ( !mItem )
	{
		return;
	}

	//mItem->beginCommand( tr( "Item frame toggled" ) );
	if ( state == Qt::Checked )
	{
		if (m_width==0)
		{
			mItem->setFrame( false );
		}
		else
		{
			mItem->setFrame( true );
		}
		m_ischeck = true;
	}
	else
	{
		mItem->setFrame( false );
		m_ischeck = false;
	}
	
	mItem->update();
	//mItem->endCommand();
}

void CCipasComposerItemWidget::setValuesForGuiElements()
{
	if ( !mItem )
	{
		return;
	}

	mOpacitySlider->blockSignals( true );
	mOutlineWidthSpinBox->blockSignals( true );
	mFrameCheckBox->blockSignals( true );

	mOpacitySlider->setValue( mItem->brush().color().alpha() );
	mOutlineWidthSpinBox->setValue( mItem->pen().widthF() );
	if ( mItem->frame() )
	{
		mFrameCheckBox->setCheckState( Qt::Checked );
	}
	else
	{
		mFrameCheckBox->setCheckState( Qt::Unchecked );
	}

	mOpacitySlider->blockSignals( false );
	mOutlineWidthSpinBox->blockSignals( false );
	mFrameCheckBox->blockSignals( false );

}

//void CCipasComposerItemWidget::on_mPositionButton_clicked()
//{
//	if ( !mItem )
//	{
//		return;
//	}
//
//	//mItem->beginCommand( tr( "Item position changed" ) );
//	//QgsItemPositionDialog d( mItem, 0 );
//	//if ( d.exec() == QDialog::Accepted )
//	//{
//	//	mItem->endCommand();
//	//}
//	//else
//	//{
//	//	mItem->cancelCommand();
//	//}
//}
