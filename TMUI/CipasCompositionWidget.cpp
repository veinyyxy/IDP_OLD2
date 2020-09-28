#include <QColorDialog>
#include <QWidget>
#include <QPrinter> //for screen resolution

#include "CipasComposition.h"
#include "CipasCompositionWidget.h"

CCipasCompositionWidget::CCipasCompositionWidget(QWidget* parent, CCipasComposition* c): QWidget( parent ), mComposition( c )
{
	setupUi( this );
	blockSignals( true );
	createPaperEntries();

	//unit
	mPaperUnitsComboBox->addItem( tr( "mm" ) );
	mPaperUnitsComboBox->addItem( tr( "inch" ) );

	//orientation
	//mPaperOrientationComboBox->insertItem( 0, tr( "Landscape" ) );
	//mPaperOrientationComboBox->insertItem( 1, tr( "Portrait" ) );
	mPaperOrientationComboBox->insertItem( 0, tr("横向") );
	mPaperOrientationComboBox->insertItem( 1, tr("纵向") );
	//mPaperOrientationComboBox->setCurrentIndex( 0 );

	//read with/height from composition and find suitable entries to display
	displayCompositionWidthHeight();

	if ( mComposition )
	{
		//read printout resolution from composition
		mResolutionSpinBox->setValue( mComposition->printResolution() );
		//snap grid
		if ( mComposition->snapToGridEnabled() )
		{
			mSnapToGridCheckBox->setCheckState( Qt::Checked );
		}
		else
		{
			mSnapToGridCheckBox->setCheckState( Qt::Unchecked );
		}
		mGridResolutionSpinBox->setValue( mComposition->snapGridResolution() );
		mOffsetXSpinBox->setValue( mComposition->snapGridOffsetX() );
		mOffsetYSpinBox->setValue( mComposition->snapGridOffsetY() );
		//grid pen width
		mPenWidthSpinBox->setValue( mComposition->gridPen().widthF() );
		//grid pen color
		mGridColorButton->setColor( mComposition->gridPen().color() );
		mGridStyleComboBox->insertItem( 0, tr("实线")/*tr( "Solid" )*/ );
		//mGridStyleComboBox->insertItem( 1, tr( "Dots" ) );
		mGridStyleComboBox->insertItem( 1, tr("十字线")/*tr( "Crosses" )*/ );

		CCipasComposition::GridStyle snapGridStyle = mComposition->gridStyle();
		if ( snapGridStyle == CCipasComposition::Solid )
		{
			mGridStyleComboBox->setCurrentIndex( 0 );
		}
		//else if ( snapGridStyle == CCipasComposition::Dots )
		//{
		//	mGridStyleComboBox->setCurrentIndex( 1 );
		//}//
		else
		{
			mGridStyleComboBox->setCurrentIndex( 1 );
		}
		//边框
		mpIsDrawLineRect->setChecked(mComposition->IsDrawBounding());

	}
	blockSignals( false );
}


CCipasCompositionWidget::CCipasCompositionWidget(): QWidget( 0 ), mComposition( 0 )
{
	setupUi( this );
}


CCipasCompositionWidget::~CCipasCompositionWidget(void)
{
}


void CCipasCompositionWidget::createPaperEntries()
{
	QList<CipasCompositionPaper> formats;

	formats
		// ISO formats
		<< CipasCompositionPaper( tr( "A5 (148x210 mm)" ), 148, 210 )
		<< CipasCompositionPaper( tr( "A4 (210x297 mm)" ), 210, 297 )
		<< CipasCompositionPaper( tr( "A3 (297x420 mm)" ), 297, 420 )
		<< CipasCompositionPaper( tr( "A2 (420x594 mm)" ), 420, 594 )
		<< CipasCompositionPaper( tr( "A1 (594x841 mm)" ), 594, 841 )
		<< CipasCompositionPaper( tr( "A0 (841x1189 mm)" ), 841, 1189 )
		<< CipasCompositionPaper( tr( "B5 (176 x 250 mm)" ), 176, 250 )
		<< CipasCompositionPaper( tr( "B4 (250 x 353 mm)" ), 250, 353 )
		<< CipasCompositionPaper( tr( "B3 (353 x 500 mm)" ), 353, 500 )
		<< CipasCompositionPaper( tr( "B2 (500 x 707 mm)" ), 500, 707 )
		<< CipasCompositionPaper( tr( "B1 (707 x 1000 mm)" ), 707, 1000 )
		<< CipasCompositionPaper( tr( "B0 (1000 x 1414 mm)" ), 1000, 1414 )
		// North american formats
		<< CipasCompositionPaper( tr( "Legal (8.5x14 inches)" ), 215.9, 355.6 )
		<< CipasCompositionPaper( tr( "ANSI A (Letter; 8.5x11 inches)" ), 215.9, 279.4 )
		<< CipasCompositionPaper( tr( "ANSI B (Tabloid; 11x17 inches)" ), 279.4, 431.8 )
		<< CipasCompositionPaper( tr( "ANSI C (17x22 inches)" ), 431.8, 558.8 )
		<< CipasCompositionPaper( tr( "ANSI D (22x34 inches)" ), 558.8, 863.6 )
		<< CipasCompositionPaper( tr( "ANSI E (34x44 inches)" ), 863.6, 1117.6 )
		<< CipasCompositionPaper( tr( "Arch A (9x12 inches)" ), 228.6, 304.8 )
		<< CipasCompositionPaper( tr( "Arch B (12x18 inches)" ), 304.8, 457.2 )
		<< CipasCompositionPaper( tr( "Arch C (18x24 inches)" ), 457.2, 609.6 )
		<< CipasCompositionPaper( tr( "Arch D (24x36 inches)" ), 609.6, 914.4 )
		<< CipasCompositionPaper( tr( "Arch E (36x48 inches)" ), 914.4, 1219.2 )
		<< CipasCompositionPaper( tr( "Arch E1 (30x42 inches)" ), 762, 1066.8 )
		;
	mPaperSizeComboBox->addItem( tr( "Custom" ) );

	for ( QList<CipasCompositionPaper>::const_iterator it = formats.begin(); it != formats.end(); it++ )
	{
		mPaperSizeComboBox->addItem( it->mName );
		mPaperMap.insert( it->mName, *it );
	}
	mPaperSizeComboBox->setCurrentIndex( 2 ); //A4
}

void CCipasCompositionWidget::on_mPaperSizeComboBox_currentIndexChanged( const QString& text )
{
	if ( mPaperSizeComboBox->currentText() == tr( "Custom" ) )
	{
		mPaperWidthDoubleSpinBox->setEnabled( true );
		mPaperHeightDoubleSpinBox->setEnabled( true );
		mPaperUnitsComboBox->setEnabled( true );
	}
	else
	{
		mPaperWidthDoubleSpinBox->setEnabled( false );
		mPaperHeightDoubleSpinBox->setEnabled( false );
		mPaperUnitsComboBox->setEnabled( false );
	}
	applyCurrentPaperSettings();
}

void CCipasCompositionWidget::on_mPaperOrientationComboBox_currentIndexChanged( const QString& text )
{
	if ( mPaperSizeComboBox->currentText() == tr( "Custom" ) )
	{
		adjustOrientation();
		applyWidthHeight();
	}
	else
	{
		adjustOrientation();
		applyCurrentPaperSettings();
	}
}

void CCipasCompositionWidget::on_mPaperUnitsComboBox_currentIndexChanged( const QString& text )
{
	double width = size( mPaperWidthDoubleSpinBox );
	double height = size( mPaperHeightDoubleSpinBox );

	if ( mPaperUnitsComboBox->currentIndex() == 0 )
	{
		// mm, value was inch
		width *= 25.4;
		height *= 25.4;
	}
	else
	{
		// inch, values was mm,
		width /= 25.4;
		height /= 25.4;
	}

	setSize( mPaperWidthDoubleSpinBox, width );
	setSize( mPaperHeightDoubleSpinBox, height );

	if ( mPaperSizeComboBox->currentText() == tr( "Custom" ) )
	{
		adjustOrientation();
		applyWidthHeight();
	}
	else
	{
		adjustOrientation();
		applyCurrentPaperSettings();
	}
}

void CCipasCompositionWidget::adjustOrientation()
{
	double width = size( mPaperWidthDoubleSpinBox );
	double height = size( mPaperHeightDoubleSpinBox );

	if ( width < 0 || height < 0 )
	{
		return;
	}

	if ( height > width ) //change values such that width > height
	{
		double tmp = width;
		width = height;
		height = tmp;
	}

	bool lineEditsEnabled = mPaperWidthDoubleSpinBox->isEnabled();

	mPaperWidthDoubleSpinBox->setEnabled( true );
	mPaperHeightDoubleSpinBox->setEnabled( true );
	if ( mPaperOrientationComboBox->currentText() == tr("横向") )
	{
		setSize( mPaperWidthDoubleSpinBox, width );
		setSize( mPaperHeightDoubleSpinBox, height );
	}
	else
	{
		setSize( mPaperWidthDoubleSpinBox, height );
		setSize( mPaperHeightDoubleSpinBox, width );
	}
	mPaperWidthDoubleSpinBox->setEnabled( lineEditsEnabled );
	mPaperHeightDoubleSpinBox->setEnabled( lineEditsEnabled );
}

void CCipasCompositionWidget::setSize( QDoubleSpinBox *spin, double v )
{
	if ( mPaperUnitsComboBox->currentIndex() == 0 )
	{
		// mm
		spin->setValue( v );
	}
	else
	{
		// inch (show width in inch)
		spin->setValue( v / 25.4 );
	}
}

double CCipasCompositionWidget::size( QDoubleSpinBox *spin )
{
	double size = spin->value();

	if ( mPaperUnitsComboBox->currentIndex() == 0 )
	{
		// mm
		return size;
	}
	else
	{
		// inch return in mm
		return size * 25.4;
	}
}

void CCipasCompositionWidget::applyCurrentPaperSettings()
{
	if ( mComposition )
	{
		//find entry in mPaper map to set width and height
		QMap<QString, CipasCompositionPaper>::iterator it = mPaperMap.find( mPaperSizeComboBox->currentText() );
		if ( it == mPaperMap.end() )
		{
			return;
		}

		mPaperWidthDoubleSpinBox->setEnabled( true );
		mPaperHeightDoubleSpinBox->setEnabled( true );
		setSize( mPaperWidthDoubleSpinBox, it->mWidth );
		setSize( mPaperHeightDoubleSpinBox, it->mHeight );
		mPaperWidthDoubleSpinBox->setEnabled( false );
		mPaperHeightDoubleSpinBox->setEnabled( false );

		adjustOrientation();
		applyWidthHeight();
	}
}

void CCipasCompositionWidget::applyWidthHeight()
{
	double width = size( mPaperWidthDoubleSpinBox );
	double height = size( mPaperHeightDoubleSpinBox );

	if ( width < 0 || height < 0 )
		return;

	mComposition->setPaperSize( width, height );
}

void CCipasCompositionWidget::on_mPaperWidthDoubleSpinBox_editingFinished()
{
	applyWidthHeight();
}

void CCipasCompositionWidget::on_mPaperHeightDoubleSpinBox_editingFinished()
{
	applyWidthHeight();
}

void CCipasCompositionWidget::displayCompositionWidthHeight()
{
	if ( !mComposition )
	{
		return;
	}

	double paperWidth = mComposition->paperWidth();
	setSize( mPaperWidthDoubleSpinBox, paperWidth );

	double paperHeight = mComposition->paperHeight();
	setSize( mPaperHeightDoubleSpinBox, paperHeight );

	//set orientation
	if ( paperWidth > paperHeight )
	{
		mPaperOrientationComboBox->setCurrentIndex( mPaperOrientationComboBox->findText( tr("横向") ) );
	}
	else
	{
		mPaperOrientationComboBox->setCurrentIndex( mPaperOrientationComboBox->findText(tr("纵向") ) );
	}

	//set paper name
	bool found = false;
	QMap<QString, CipasCompositionPaper>::const_iterator paper_it = mPaperMap.constBegin();
	for ( ; paper_it != mPaperMap.constEnd(); ++paper_it )
	{
		CipasCompositionPaper currentPaper = paper_it.value();

		//consider width and height values may be exchanged
		if (( doubleNear( currentPaper.mWidth, paperWidth ) && doubleNear( currentPaper.mHeight, paperHeight ) )
			|| ( doubleNear( currentPaper.mWidth, paperHeight ) && doubleNear( currentPaper.mHeight, paperWidth ) ) )
		{
			mPaperSizeComboBox->setCurrentIndex( mPaperSizeComboBox->findText( paper_it.key() ) );
			found = true;
			break;
		}
	}

	if ( !found )
	{
		//custom
		mPaperSizeComboBox->setCurrentIndex( 0 );
	}
}

void CCipasCompositionWidget::displaySnapingSettings()
{
	if ( !mComposition )
	{
		return;
	}

	if ( mComposition->snapToGridEnabled() )
	{
		mSnapToGridCheckBox->setCheckState( Qt::Checked );
	}
	else
	{
		mSnapToGridCheckBox->setCheckState( Qt::Unchecked );
	}

	mGridResolutionSpinBox->setValue( mComposition->snapGridResolution() );
	mOffsetXSpinBox->setValue( mComposition->snapGridOffsetX() );
	mOffsetYSpinBox->setValue( mComposition->snapGridOffsetY() );
}

void CCipasCompositionWidget::on_mResolutionSpinBox_valueChanged( const int value )
{
	mComposition->setPrintResolution( value );
}

void CCipasCompositionWidget::on_mPrintAsRasterCheckBox_stateChanged( int state )
{
	if ( !mComposition )
	{
		return;
	}

	if ( state == Qt::Checked )
	{
		mComposition->setPrintAsRaster( true );
	}
	else
	{
		mComposition->setPrintAsRaster( false );
	}
}

void CCipasCompositionWidget::on_mSnapToGridCheckBox_stateChanged( int state )
{
	if ( mComposition )
	{
		if ( state == Qt::Checked )
		{
			mComposition->setSnapToGridEnabled( true );
		}
		else
		{
			mComposition->setSnapToGridEnabled( false );
		}
	}
}

void CCipasCompositionWidget::on_mGridResolutionSpinBox_valueChanged( double d )
{
	if ( mComposition )
	{
		mComposition->setSnapGridResolution( d );
	}
}

void CCipasCompositionWidget::on_mOffsetXSpinBox_valueChanged( double d )
{
	if ( mComposition )
	{
		mComposition->setSnapGridOffsetX( d );
	}
}

void CCipasCompositionWidget::on_mOffsetYSpinBox_valueChanged( double d )
{
	if ( mComposition )
	{
		mComposition->setSnapGridOffsetY( d );
	}
}

void CCipasCompositionWidget::on_mGridColorButton_clicked()
{
	QColor newColor = QColorDialog::getColor( mGridColorButton->color() );
	if ( !newColor.isValid() )
	{
		return ; //dialog canceled by user
	}
	mGridColorButton->setColor( newColor );

	if ( mComposition )
	{
		QPen pen = mComposition->gridPen();
		pen.setColor( newColor );
		mComposition->setGridPen( pen );
	}
}

void CCipasCompositionWidget::on_mGridStyleComboBox_currentIndexChanged( const QString& text )
{
	if ( mComposition )
	{
		if ( mGridStyleComboBox->currentText() == tr("实线") )
		{
			mComposition->setGridStyle( CCipasComposition::Solid );
		}
		//else if ( mGridStyleComboBox->currentText() == tr( "Dots" ) )
		//{
		//	mComposition->setGridStyle( CCipasComposition::Dots );
		//}
		else if ( mGridStyleComboBox->currentText() == tr("十字线") )
		{
			mComposition->setGridStyle( CCipasComposition::Crosses );
		}
	}
}

void CCipasCompositionWidget::on_mPenWidthSpinBox_valueChanged( double d )
{
	if ( mComposition )
	{
		QPen pen = mComposition->gridPen();
		pen.setWidthF( d );
		mComposition->setGridPen( pen );
	}
}

void CCipasCompositionWidget::blockSignals( bool block )
{
	mPaperSizeComboBox->blockSignals( block );
	mPaperUnitsComboBox->blockSignals( block );
	mPaperWidthDoubleSpinBox->blockSignals( block );
	mPaperHeightDoubleSpinBox->blockSignals( block );
	mPaperOrientationComboBox->blockSignals( block );
	mResolutionSpinBox->blockSignals( block );
	//mPrintAsRasterCheckBox->blockSignals( block );
	mSnapToGridCheckBox->blockSignals( block );
	mGridResolutionSpinBox->blockSignals( block );
	mOffsetXSpinBox->blockSignals( block );
	mOffsetYSpinBox->blockSignals( block );
	mPenWidthSpinBox->blockSignals( block );
	mGridColorButton->blockSignals( block );
	mGridStyleComboBox->blockSignals( block );
}

void CCipasCompositionWidget::on_mpColorButton_clicked()
{
	QColor newColor = QColorDialog::getColor( mGridColorButton->color() );
	if ( !newColor.isValid() )
	{
		return ;
	}
	
	mpColorButton->setColor(newColor );
	QPen* pP = mComposition->BoundingPen();
	pP->setColor(newColor);
	mComposition->update();
	/*if ( mComposition )
	{
		QPen pen = mComposition->gridPen();
		pen.setColor( newColor );
		mComposition->setGridPen( pen );
	}*/
}

void CCipasCompositionWidget::on_mpLineStyle_currentIndexChanged( const QString& text )
{
	QPen* pP = mComposition->BoundingPen();
	if(text == tr("实线"))
	{
		pP->setStyle(Qt::SolidLine);
	}
	else if(text == tr("虚线"))
	{
		pP->setStyle(Qt::DashLine);
	}
	else if(text == tr("点线"))
	{
		pP->setStyle(Qt::DotLine);
	}
	else if(text == tr("虚点线"))
	{
		pP->setStyle(Qt::DashDotLine);
	}
	else if(text == tr("虚点点线"))
	{
		pP->setStyle(Qt::DashDotDotLine);
	}
	else
	{
		pP->setStyle(Qt::CustomDashLine);
	}
	mComposition->update();
}

//void CCipasCompositionWidget::on_mpConerSytle_currentIndexChanged( const QString& text )
//{
//	QPen* pP = mComposition->BoundingPen();
//	if(text == tr("平角"))
//	{
//		pP->setJoinStyle(Qt::BevelJoin);
//	}
//	else if(text == tr("锐角"))
//	{
//		pP->setJoinStyle(Qt::MiterJoin);
//	}
//	else
//	{
//		pP->setJoinStyle(Qt::RoundJoin);
//	}
//	mComposition->update();
//}

void CCipasCompositionWidget::on_mpLineNum_editingFinished()
{
	mComposition->SetLineNum(mpLineNum->value());
	mComposition->update();
}

void CCipasCompositionWidget::on_mpLineInterval_editingFinished()
{
	mComposition->SetLineInv(mpLineInterval->value());
	mComposition->update();
}

void CCipasCompositionWidget::on_mpLineWidth_editingFinished()
{
	mComposition->BoundingPen()->setWidthF(mpLineWidth->value());
	mComposition->update();
}

//void CCipasCompositionWidget::on_mpLineDashOffset_editingFinished()
//{
//	mComposition->BoundingPen()->setDashOffset(mpLineDashOffset->value());
//	mComposition->update();
//}

void CCipasCompositionWidget::on_mpIsDrawLineRect_stateChanged( int state )
{
	mComposition->SetIsDrawBounding(state);
	mComposition->update();
}

void CCipasCompositionWidget::on_mMarginXSpinBox_valueChanged( double d )
{
	mComposition->setMarginX(d);
	mComposition->update();

}

void CCipasCompositionWidget::on_mMarginYSpinBox_valueChanged( double d )
{
	mComposition->setMarginY(d);
	mComposition->update();

}