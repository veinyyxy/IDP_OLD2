#include "CipasComposerLegendWidget.h"
#include "CipasComposerLegend.h"
#include "CipasComposerItemWidget.h"
#include "cipascomposerlegenditemdialog.h"
#include "CipasComposerLegendLayersDialog.h"
#include "CipasComposerLegendItem.h"
#include <QFontDialog>
#include <QMessageBox>
#include <QTreeWidget>
#include <QColorDialog>



CCipasComposerLegendWidget::CCipasComposerLegendWidget(CCipasComposerLegend* legend): mLegend( legend )
{
	setupUi( this );
	// setup icons
	mAddToolButton->setIcon( QIcon(":/images/thematicMap/symbologyAdd.png"  ) );
	mEditPushButton->setIcon( QIcon(":/images/thematicMap/symbologyEdit.png" ) ) ;
	mRemoveToolButton->setIcon( QIcon(":/images/thematicMap/symbologyRemove.png" ) );
	mMoveUpToolButton->setIcon( QIcon(":/images/thematicMap/symbologyUp.png" ) ) ;
	mMoveDownToolButton->setIcon(QIcon(":/images/thematicMap/symbologyDown.png"  ) );

	//updateLegend();

	if ( legend )
	{
		mItemTreeView->setModel( legend->model());
	}

	mItemTreeView->setDragEnabled( true );
	mItemTreeView->setAcceptDrops( true );
	mItemTreeView->setDropIndicatorShown( true );
	//mItemTreeView->setDragDropMode( QAbstractItemView::InternalMove );

	setGuiElements();
	connect( mItemTreeView, SIGNAL( itemChanged() ), this, SLOT( setGuiElements() ) );
	//style
	QString s=legend->style();//从模板中获得样式
	mLegendSytleComboBox->insertItem(0,tr("GIS风格"));
	mLegendSytleComboBox->insertItem(1,tr("Grads风格（横向）"));
	mLegendSytleComboBox->insertItem(2,tr("Grads风格（纵向）"));
	if (s=="")
	{
		mLegendSytleComboBox->setCurrentIndex(0);
	}
	else
	{
		mLegendSytleComboBox->setCurrentIndex(mLegendSytleComboBox->findText(s));
	}
	//是否绘制图层名
	mCheckBoxLayerDraw->setChecked(legend->isDrawLayerName());
	//是否从模板中读取图例内容
	mLegendItem2tempCheckBox->setChecked(legend->isLegendItem2Temp());
	mLegendStyle2templateCheckBox->setChecked(legend->isLegendStyle2Template());
	//如果不从模板加载图例内容，那么加载样式 
	if(!legend->isLegendItem2Temp())
	{
		updateLegendOnlyStyle();
	}
	CCipasComposerItemWidget* itemPropertiesWidget = new CCipasComposerItemWidget( this, legend );
	toolBox->addItem( itemPropertiesWidget, tr("其他选项") );
}

CCipasComposerLegendWidget::CCipasComposerLegendWidget(): mLegend( 0 )
{
	setupUi( this );
}

CCipasComposerLegendWidget::~CCipasComposerLegendWidget(void)
{
	//delete m_MapTable;
}

QList< GroupLayerInfo > CCipasComposerLegendWidget::groupLayerSet()
{
	QList< GroupLayerInfo > groupLayerList;
	QList<QString> layerList;
	layerList=layerSet() ;
	groupLayerList.push_back( qMakePair( QString(), layerList) );
	return groupLayerList;
}

QStringList CCipasComposerLegendWidget::layerSet()
{
	QStringList mLayerSet;
	QStandardItemModel* itemModel = qobject_cast<QStandardItemModel *>( mItemTreeView->model() );
	for (int i = 0 ; i< itemModel->rowCount();i++)
	{
		mLayerSet.append(itemModel->item(i,0)->text());
	}
	return mLayerSet;
}


void CCipasComposerLegendWidget::updateLegend()
{
	if ( mLegend )
	{

		//get layer id list
		QStringList layerIdList=layerSet();
		QList< GroupLayerInfo > groupInfo =groupLayerSet();
		
		mLegend->model()->setLayerSetAndGroups( layerIdList, groupInfo );
		//mLegend->endCommand();
	}
}

void CCipasComposerLegendWidget::updateLegendOnlyStyle()
{
	if (mLegend)
	{
		//get layer id list
		QStringList layerIdList;
		QList< GroupLayerInfo > groupLayerList;
		QList<QString> layerList;
		mLegend->model()->setLayerSetAndGroups( layerIdList, groupLayerList );
	}
}

void CCipasComposerLegendWidget::setGuiElements()
{
	if ( !mLegend )
	{
		return;
	}

	blockAllSignals( true );
	mTitleLineEdit->setText( mLegend->title() );
	mSymbolWidthSpinBox->setValue( mLegend->symbolWidth() );
	mSymbolHeightSpinBox->setValue( mLegend->symbolHeight() );
	mTotalSymbolWidthSpinBox->setValue( mLegend->totalSymobolWidth());
	if (mLegendSytleComboBox->currentText()==tr("Grads风格（横向）"))
	{
		double a=mLegend->totalSymobolWidth();
		int b= mLegend->symbolCounts();
		mSymbolWidthSpinBox->setValue( mLegend->totalSymobolWidth()/mLegend->symbolCounts() );
	}
	mLayerSpaceSpinBox->setValue( mLegend->layerSpace() );
	mSymbolSpaceSpinBox->setValue( mLegend->symbolSpace() );
	mIconLabelSpaceSpinBox->setValue( mLegend->iconLabelSpace() );
	mBoxSpaceSpinBox->setValue( mLegend->boxSpace() );
	if ( mLegend->model() )
	{
		mCheckBoxAutoUpdate->setChecked( mLegend->model()->autoUpdate() );
	}

	blockAllSignals( false );
}

void CCipasComposerLegendWidget::on_mTitleLineEdit_textChanged( const QString& text )
{
	if ( mLegend )
	{
		mLegend->beginCommand( tr( "Legend title changed" ), CipasComposerMergeCommand::ComposerLegendText );
		mLegend->setTitle( text );
		mLegend->adjustBoxSize();
		mLegend->update();
		mLegend->endCommand();
	}
}

void CCipasComposerLegendWidget::on_mSymbolWidthSpinBox_valueChanged( double d )
{
	if ( mLegend )
	{
		mLegend->beginCommand( tr( "Legend symbol width" ), CipasComposerMergeCommand::LegendSymbolWidth );
		mLegend->setSymbolWidth( d );
		mLegend->adjustBoxSize();
		mLegend->update();
		mLegend->endCommand();
	}
}

void CCipasComposerLegendWidget::on_mTotalSymbolWidthSpinBox_valueChanged( double d )
{
	if (mLegend)
	{
		mLegend->setTotalSymbolWidth(d);
		mLegend->adjustBoxSize();
		mLegend->update();
	}
}

void CCipasComposerLegendWidget::on_mSymbolOutlineWidthSpinBox_valueChanged(double d )
{
	if (mLegend)
	{
		mLegend->setSymbolOutlineWidth( d );
		mLegend->adjustBoxSize();
		mLegend->update();
	}
}

void CCipasComposerLegendWidget::on_mSymbolHeightSpinBox_valueChanged( double d )
{
	if ( mLegend )
	{
		mLegend->beginCommand( tr( "Legend symbol height" ), CipasComposerMergeCommand::LegendSymbolHeight );
		mLegend->setSymbolHeight( d );
		mLegend->adjustBoxSize();
		mLegend->update();
		mLegend->endCommand();
	}
}

void CCipasComposerLegendWidget::on_mLayerSpaceSpinBox_valueChanged( double d )
{
	if ( mLegend )
	{
		mLegend->beginCommand( tr( "Legend layer space" ), CipasComposerMergeCommand::LegendLayerSpace );
		mLegend->setLayerSpace( d );
		mLegend->adjustBoxSize();
		mLegend->update();
		mLegend->endCommand();
	}
}

void CCipasComposerLegendWidget::on_mSymbolSpaceSpinBox_valueChanged( double d )
{
	if ( mLegend )
	{
		mLegend->beginCommand( tr( "Legend symbol space" ), CipasComposerMergeCommand::LegendSymbolSpace );
		mLegend->setSymbolSpace( d );
		mLegend->adjustBoxSize();
		mLegend->update();
		mLegend->endCommand();
	}
}

void CCipasComposerLegendWidget::on_mIconLabelSpaceSpinBox_valueChanged( double d )
{
	if ( mLegend )
	{
		mLegend->beginCommand( tr( "Legend icon label space" ), CipasComposerMergeCommand::LegendIconSymbolSpace );
		mLegend->setIconLabelSpace( d );
		mLegend->adjustBoxSize();
		mLegend->update();
		mLegend->endCommand();
	}
}

void CCipasComposerLegendWidget::on_mTitleFontButton_clicked()
{
	if ( mLegend )
	{
		bool ok;
#if defined(Q_WS_MAC) && QT_VERSION >= 0x040500 && defined(QT_MAC_USE_COCOA)
		// Native Mac dialog works only for Qt Carbon
		QFont newFont = QFontDialog::getFont( &ok, mLegend->titleFont(), this, QString(), QFontDialog::DontUseNativeDialog );
#else
		QFont newFont = QFontDialog::getFont( &ok, mLegend->titleFont() );
#endif
		if ( ok )
		{
			mLegend->beginCommand( tr( "Title font changed" ) );
			mLegend->setTitleFont( newFont );
			mLegend->adjustBoxSize();
			mLegend->update();
			mLegend->endCommand();
		}
	}
}

void CCipasComposerLegendWidget::on_mLayerFontButton_clicked()
{
	if ( mLegend )
	{
		bool ok;
#if defined(Q_WS_MAC) && QT_VERSION >= 0x040500 && defined(QT_MAC_USE_COCOA)
		// Native Mac dialog works only for Qt Carbon
		QFont newFont = QFontDialog::getFont( &ok, mLegend->layerFont(), this, QString(), QFontDialog::DontUseNativeDialog );
#else
		QFont newFont = QFontDialog::getFont( &ok, mLegend->layerFont() );
#endif
		if ( ok )
		{
			mLegend->beginCommand( tr( "Legend layer font changed" ) );
			mLegend->setLayerFont( newFont );
			mLegend->adjustBoxSize();
			mLegend->update();
			mLegend->endCommand();
		}
	}
}

void CCipasComposerLegendWidget::on_mItemFontButton_clicked()
{
	if ( mLegend )
	{
		bool ok;
#if defined(Q_WS_MAC) && QT_VERSION >= 0x040500 && defined(QT_MAC_USE_COCOA)
		// Native Mac dialog works only for Qt Carbon
		QFont newFont = QFontDialog::getFont( &ok, mLegend->itemFont(), this, QString(), QFontDialog::DontUseNativeDialog );
#else
		QFont newFont = QFontDialog::getFont( &ok, mLegend->itemFont() );
#endif
		if ( ok )
		{
			mLegend->beginCommand( tr( "Legend item font changed" ) );
			mLegend->setItemFont( newFont );
			mLegend->adjustBoxSize();
			mLegend->update();
			mLegend->endCommand();
		}
	}
}

void CCipasComposerLegendWidget::on_mSymbolOutlineColorButton_clicked()
{
	if ( !mLegend )
	{
		return;
	}

	QColor newBackgroundColor = QColorDialog::getColor( mLegend->brush().color(), 0 );
	if ( !newBackgroundColor.isValid() )
	{
		return; //dialog canceled
	}
	mLegend->setSymbolOutlineColor(newBackgroundColor);
	mLegend->update();
}

void CCipasComposerLegendWidget::on_mBoxSpaceSpinBox_valueChanged( double d )
{
	if ( mLegend )
	{
		mLegend->beginCommand( tr( "Legend box space" ), CipasComposerMergeCommand::LegendBoxSpace );
		mLegend->setBoxSpace( d );
		mLegend->adjustBoxSize();
		mLegend->update();
		mLegend->endCommand();
	}
}

void CCipasComposerLegendWidget::on_mMoveDownToolButton_clicked()
{
	if ( !mLegend )
	{
		return;
	}

	QStandardItemModel* itemModel = qobject_cast<QStandardItemModel *>( mItemTreeView->model() );
	if ( !itemModel )
	{
		return;
	}

	QModelIndex currentIndex = mItemTreeView->currentIndex();
	if ( !currentIndex.isValid() )
	{
		return;
	}

	//is there an older sibling?
	int row = currentIndex.row();
	QModelIndex youngerSibling = currentIndex.sibling( row + 1, 0 );

	if ( !youngerSibling.isValid() )
	{
		return;
	}

	//mLegend->beginCommand( "Moved legend item down" );
	QModelIndex parentIndex = currentIndex.parent();
	QList<QStandardItem*> itemToMove;
	QList<QStandardItem*> youngerSiblingItem;

	if ( !parentIndex.isValid() ) //move toplevel (layer) item
	{
		youngerSiblingItem = itemModel->takeRow( row + 1 );
		itemToMove = itemModel->takeRow( row );
		itemModel->insertRow( row, youngerSiblingItem );
		itemModel->insertRow( row + 1, itemToMove );
	}
	else //move child (classification) item
	{
		QStandardItem* parentItem = itemModel->itemFromIndex( parentIndex );
		youngerSiblingItem = parentItem->takeRow( row + 1 );
		itemToMove = parentItem->takeRow( row );
		parentItem->insertRow( row, youngerSiblingItem );
		parentItem->insertRow( row + 1, itemToMove );
	}

	mItemTreeView->setCurrentIndex( itemModel->indexFromItem( itemToMove.at( 0 ) ) );
	mLegend->update();
	//mLegend->endCommand();
}

void CCipasComposerLegendWidget::on_mMoveUpToolButton_clicked()
{
	if ( !mLegend )
	{
		return;
	}

	QStandardItemModel* itemModel = qobject_cast<QStandardItemModel *>( mItemTreeView->model() );
	if ( !itemModel )
	{
		return;
	}

	QModelIndex currentIndex = mItemTreeView->currentIndex();
	if ( !currentIndex.isValid() )
	{
		return;
	}

	//mLegend->beginCommand( "Moved legend item up" );
	//is there an older sibling?
	int row = currentIndex.row();
	QModelIndex olderSibling = currentIndex.sibling( row - 1, 0 );

	if ( !olderSibling.isValid() )
	{
		return;
	}

	QModelIndex parentIndex = currentIndex.parent();
	QList<QStandardItem*> itemToMove;
	QList<QStandardItem*> olderSiblingItem;

	if ( !parentIndex.isValid() ) //move toplevel item
	{
		itemToMove = itemModel->takeRow( row );
		olderSiblingItem = itemModel->takeRow( row - 1 );
		itemModel->insertRow( row - 1, itemToMove );
		itemModel->insertRow( row, olderSiblingItem );
	}
	else //move classification items
	{
		QStandardItem* parentItem = itemModel->itemFromIndex( parentIndex );
		itemToMove = parentItem->takeRow( row );
		olderSiblingItem = parentItem->takeRow( row - 1 );
		parentItem->insertRow( row - 1, itemToMove );
		parentItem->insertRow( row, olderSiblingItem );
	}

	mItemTreeView->setCurrentIndex( itemModel->indexFromItem( itemToMove.at( 0 ) ) );
	mLegend->update();
	//mLegend->endCommand();
}

void CCipasComposerLegendWidget::on_mCheckBoxAutoUpdate_stateChanged( int state )
{
	if ( !mLegend->model() )
	{
		return;
	}

	if ( state == Qt::Checked )
	{
		mLegend->model()->setAutoUpdate( true );
	}
	else
	{
		mLegend->model()->setAutoUpdate( false );
	}
}

void CCipasComposerLegendWidget::on_mCheckBoxLayerDraw_stateChanged( int state )
{
	if ( !mLegend->model() )
	{
		return;
	}
	mLegend->setDrawLayerName(state == Qt::Checked);
	mLegend->adjustBoxSize();
	mLegend->update();
}

void CCipasComposerLegendWidget::on_mLegendItem2tempCheckBox_stateChanged(int state)
{
	mLegend->setLengendItem2Temp(state);
}

void CCipasComposerLegendWidget::on_mLegendStyle2templateCheckBox_stateChanged(int state)
{
	mLegend->setLengendStyle2Template(state);
}

void CCipasComposerLegendWidget::on_mLegendSytleComboBox_currentIndexChanged( const QString& text )
{
	if (mLegendSytleComboBox->currentText()==tr("GIS风格"))
	{
		mSymbolWidthSpinBox->setEnabled(true);
		mSymbolHeightSpinBox->setEnabled(true);
		mTotalSymbolWidthSpinBox->setEnabled(false);
		mTotalSymbolHeightSpinBox->setEnabled(false);
	}
	if (mLegendSytleComboBox->currentText()==tr("Grads风格（横向）"))
	{
		mSymbolWidthSpinBox->setEnabled(false);
		mSymbolHeightSpinBox->setEnabled(true);
		mTotalSymbolWidthSpinBox->setEnabled(true);
		mTotalSymbolHeightSpinBox->setEnabled(false);
	}
	if (mLegendSytleComboBox->currentText()==tr("Grads风格（纵向）"))
	{
		mSymbolWidthSpinBox->setEnabled(true);
		mSymbolHeightSpinBox->setEnabled(true);
		mTotalSymbolWidthSpinBox->setEnabled(false);
		mTotalSymbolHeightSpinBox->setEnabled(false);
	}
	mLegend->setStyle(mLegendSytleComboBox->currentText());
	mLegend->adjustBoxSize();
	mLegend->update();
}

void CCipasComposerLegendWidget::on_mAddToolButton_clicked()
{
	if ( !mLegend )
	{
		return;
	}

	QStandardItemModel* itemModel = qobject_cast<QStandardItemModel *>( mItemTreeView->model() );
	if ( !itemModel )
	{
		return;
	}
	QStandardItem* currItem = mLegend->GetCCipasComposerLegendItem();
	CCipasComposerLegendLayersDialog addDialog( mLegend );
	QListWidget* listw = addDialog.GetListWidget();
	if ( addDialog.exec() == QDialog::Accepted )
	{
		int count = listw->count();
		//P_SymbolLegend_Vector sv = mLegend->_SymbolLegend_Vector();
		//sv->clear();
		//mLegend->model()->clear();
		int countf = currItem->rowCount();
		for(int i = 0 ; i<countf;i++)
		{
			currItem->takeRow(0);
		}
		for (int i = 0; i< count ; i++)
		{
			QListWidgetItem *__qlistwidgetitem = listw->item(i);
			//SymbolLegend* ss1 = new SymbolLegend;
			//ss1->labelText = __qlistwidgetitem->text().toStdString();
			QIcon icon = __qlistwidgetitem->icon();
			//ss1->pixmap = icon.pixmap(22,22);
			//mLegend->_SymbolLegend_Vector()->append(ss1);

			CCipasComposerLegendItem* item = new CCipasComposerLayerItem(__qlistwidgetitem->text());
			item->setIcon(icon);
			mLegend->beginCommand( "Legend item added" );
			//mLegend->model()->appendRow(item);
			currItem->appendRow(item);
			mLegend->endCommand();
		}
		//Layer* layer = addDialog.selectedLayer();
		//if ( layer )
		//{
			//CCipasComposerLayerItem* currItem = new CCipasComposerLayerItem("Legend item");
			//mLegend->beginCommand( "Legend item added" );
			//itemModel->appendRow(currItem);
			//mLegend->endCommand();
	//	}
	}
}

void CCipasComposerLegendWidget::on_mRemoveToolButton_clicked()
{
	if ( !mLegend )
	{
		return;
	}

	QStandardItemModel* itemModel = qobject_cast<QStandardItemModel *>( mItemTreeView->model() );
	if ( !itemModel )
	{
		return;
	}
	
	QModelIndex currentIndex = mItemTreeView->currentIndex();

	if ( !currentIndex.isValid() )
	{
		return;
	}

	QString t = mItemTreeView->model()->itemData(currentIndex).values()[0].toString();
	/*P_SymbolLegend_Vector sv = mLegend->_SymbolLegend_Vector();
	for (int i = 0 ; i< sv->count();i++)
	{
	if (t.toStdString() == sv->at(i)->labelText)
	{
	sv->remove(i);
	break;
	}
	}*/



	QModelIndex parentIndex = currentIndex.parent();

	mLegend->beginCommand( "Legend item removed" );
	itemModel->removeRow( currentIndex.row(), parentIndex );
	mLegend->adjustBoxSize();
	mLegend->update();
	mLegend->endCommand();
}

void CCipasComposerLegendWidget::on_mEditPushButton_clicked()
{
	if ( !mLegend )
	{
		return;
	}

	QStandardItemModel* itemModel = qobject_cast<QStandardItemModel *>( mItemTreeView->model() );//QAbstractItemModel
	//QAbstractItemModel* itemModel = qobject_cast<QAbstractItemModel *>( mItemTreeView->model() );//QAbstractItemModel
	if ( !itemModel )
	{
		return;
	}

	//get current item
	QModelIndex currentIndex = mItemTreeView->currentIndex();
	if ( !currentIndex.isValid() )
	{
		return;
	}

	QStandardItem* currentItem = itemModel->itemFromIndex( currentIndex );
	if ( !currentItem )
	{
		return;
	}

	CipasComposerLegendItemDialog itemDialog( currentItem );
	if ( itemDialog.exec() == QDialog::Accepted )
	{
		currentItem->setText( itemDialog.itemText() );
	}

	mLegend->beginCommand( tr( "Legend item edited" ) );
	mLegend->adjustBoxSize();
	mLegend->update();
	mLegend->endCommand();
}

void CCipasComposerLegendWidget::on_mItemTreeView_doubleClicked( const QModelIndex & index )
{
	//暂时调用这个方法，如果需要实现双击嵌入QLineEdit的编辑效果，需要定义一个委托类。
	on_mEditPushButton_clicked();
}

void CCipasComposerLegendWidget::on_mUpdatePushButton_clicked()
{
	if ( !mLegend )
	{
		return;
	}
	updateLegend();
	mLegend->update();
}

void CCipasComposerLegendWidget::on_mUpdateAllPushButton_clicked()
{
	updateLegend();
}

void CCipasComposerLegendWidget::blockAllSignals( bool b )
{
	mItemTreeView->blockSignals( b );
	mCheckBoxAutoUpdate->blockSignals( b );
}

QIcon CCipasComposerLegendWidget::getThemeIcon( const QString theName )
{
	QString themPath = IDPApplication::Path()+"/themes/default/";
	//QString myPreferredPath = QgsApplication::activeThemePath() + QDir::separator() + theName;
	QString myDefaultPath =themPath  + theName;
	return QIcon( myDefaultPath );
}

void CCipasComposerLegendWidget::on_mcolumnSpinbox_valueChanged( int d )
{
	if ( !mLegend )
	{
		return;
	}
	
	mLegend->beginCommand( tr( "Legend column modefy" ) );
	mLegend->adjustBoxSize();
	mLegend->setColumnNumbers(d);
	mLegend->update();
	mLegend->endCommand();
}