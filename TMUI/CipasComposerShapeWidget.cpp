
#include <QColorDialog>
#include <QPen>


#include "CipasComposerShape.h"
#include "CipasComposerItemWidget.h"
#include "CipasComposerShapeWidget.h"

CCipasComposerShapeWidget::CCipasComposerShapeWidget(CCipasComposerShape* composerShape): QWidget( 0 ), mComposerShape( composerShape )
{
	 setupUi( this );
	 //add widget for general composer item properties
	 CCipasComposerItemWidget* itemPropertiesWidget = new CCipasComposerItemWidget( this, composerShape );
	 toolBox->addItem( itemPropertiesWidget, tr("常规选项") );

	 blockAllSignals( true );

	 //shape types
	 mShapeComboBox->addItem( tr("椭圆") );
	 mShapeComboBox->addItem( tr("矩形"));
	 mShapeComboBox->addItem( tr("三角形") );

	 setGuiElementValues();

	 blockAllSignals( false );

	 connect( mShapeComboBox, SIGNAL( currentIndexChanged( int ) ), this, SLOT( setGuiElementValues() ) );
}


CCipasComposerShapeWidget::~CCipasComposerShapeWidget(void)
{
}

void CCipasComposerShapeWidget::blockAllSignals( bool block )
{
	mShapeComboBox->blockSignals( block );
	mOutlineColorButton->blockSignals( block );
	mOutlineWidthSpinBox->blockSignals( block );
	mFillColorButton->blockSignals( block );
	mRotationSpinBox->blockSignals( block );
	mTransparentCheckBox->blockSignals( block );
}

void CCipasComposerShapeWidget::setGuiElementValues()
{
	if ( !mComposerShape )
	{
		return;
	}

	blockAllSignals( true );
	//double r=mComposerShape->lineWidth();
	mOutlineWidthSpinBox->setValue( mComposerShape->lineWidth() );
	mOutlineWidthSpinBox->setValue( mComposerShape->lineWidth() );
	mRotationSpinBox->setValue( mComposerShape->rotation() );
	if ( mComposerShape->shapeType() == CCipasComposerShape::Ellipse )
	{
		mShapeComboBox->setCurrentIndex( mShapeComboBox->findText(  tr("椭圆") ) );
	}
	else if ( mComposerShape->shapeType() == CCipasComposerShape::Rectangle )
	{
		mShapeComboBox->setCurrentIndex( mShapeComboBox->findText( tr("矩形") ) );
	}
	else if ( mComposerShape->shapeType() == CCipasComposerShape::Triangle )
	{
		mShapeComboBox->setCurrentIndex( mShapeComboBox->findText( tr("三角形") ) );
	}

	if ( mComposerShape->transparentFill() )
	{
		mTransparentCheckBox->setCheckState( Qt::Checked );
		mFillColorButton->setEnabled( false );
	}
	else
	{
		mTransparentCheckBox->setCheckState( Qt::Unchecked );
		mFillColorButton->setEnabled( true );
	}
	blockAllSignals( false );
}

void CCipasComposerShapeWidget::on_mRotationSpinBox_valueChanged( int val )
{
	if ( mComposerShape )
	{
		mComposerShape->beginCommand( tr( "Shape rotation changed" ), CipasComposerMergeCommand::ShapeRotation );
		mComposerShape->setRotation( val );
		mComposerShape->update();
		mComposerShape->endCommand();
	}
}

void CCipasComposerShapeWidget::on_mShapeComboBox_currentIndexChanged( const QString& text )
{
	if ( !mComposerShape )
	{
		return;
	}

	mComposerShape->beginCommand( tr( "Shape type changed" ) );
	if ( text == tr("椭圆") )
	{
		mComposerShape->setShapeType( CCipasComposerShape::Ellipse );
		mShapeComboBox->setCurrentIndex(0);
	}
	else if ( text == tr("矩形") )
	{
		mComposerShape->setShapeType( CCipasComposerShape::Rectangle );
		mShapeComboBox->setCurrentIndex(1);
	}
	else if ( text == tr("三角形") )
	{
		mComposerShape->setShapeType( CCipasComposerShape::Triangle );
		mShapeComboBox->setCurrentIndex(2);
	}
	mComposerShape->update();	
	mComposerShape->endCommand();
}

void CCipasComposerShapeWidget::on_mOutlineColorButton_clicked()
{
	if ( !mComposerShape )
	{
		return;
	}
	QColor existingColor = mComposerShape->outlineColor();
#if QT_VERSION >= 0x040500
	QColor newColor = QColorDialog::getColor( existingColor, 0, tr( "Select outline color" ), QColorDialog::ShowAlphaChannel );
#else
	QColor newColor = QColorDialog::getColor( existingColor );
#endif
	if ( newColor.isValid() )
	{
		mComposerShape->beginCommand( tr( "Shape outline color" ) );
		mComposerShape->setOutlineColor( newColor );
		mComposerShape->update();
		mComposerShape->endCommand();
	}
}

void CCipasComposerShapeWidget::on_mOutlineWidthSpinBox_valueChanged( double d )
{
	if ( !mComposerShape )
	{
		return;
	}
	mComposerShape->beginCommand( tr( "Shape outline width" ), CipasComposerMergeCommand::ShapeOutlineWidth );
	mComposerShape->setLineWidth( d );
	mComposerShape->update();
	mComposerShape->endCommand();
}

void CCipasComposerShapeWidget::on_mTransparentCheckBox_stateChanged( int state )
{
	if ( !mComposerShape )
	{
		return;
	}

	mComposerShape->beginCommand( tr( "Shape transparency toggled" ) );
	if ( state == Qt::Checked )
	{
		mComposerShape->setTransparentFill( true );
		mFillColorButton->setEnabled( false );
	}
	else
	{
		mComposerShape->setTransparentFill( false );
		mFillColorButton->setEnabled( true );
	}
	mComposerShape->update();
	mComposerShape->endCommand();
}


void CCipasComposerShapeWidget::on_mFillColorButton_clicked()
{
	if ( !mComposerShape )
	{
		return;
	}
	QColor existingColor = mComposerShape->fillColor();
#if QT_VERSION >= 0x040500
	QColor newColor = QColorDialog::getColor( existingColor, 0, tr( "Select fill color" ), QColorDialog::ShowAlphaChannel );
#else
	QColor newColor = QColorDialog::getColor( existingColor );
#endif
	if ( newColor.isValid() )
	{
		mComposerShape->beginCommand( tr( "Shape fill color" ) );
		mComposerShape->setFillColor( newColor );
		mComposerShape->update();
		mComposerShape->endCommand();
	}
}
