#include <QDoubleValidator>
#include <QFileDialog>
#include <QFileInfo>
#include <QImageReader>
#include <QMessageBox>
#include <QPainter>
#include <QProgressDialog>
#include <QSettings>
#include <QSvgRenderer>


#include "CipasComposerItemWidget.h"
#include "CipasComposition.h"
#include "CipasComposerPicture.h"
#include "CipasComposerMap.h"
#include "CipasComposerPictureWidget.h"

CCipasComposerPictureWidget::CCipasComposerPictureWidget(CCipasComposerPicture* picture)
	: QWidget(), mPicture( picture )
{
	setupUi( this );
	//add widget for general composer item properties
	CCipasComposerItemWidget* itemPropertiesWidget = new CCipasComposerItemWidget( this, picture );
	toolBox->addItem( itemPropertiesWidget, tr("常规选项") );

	mWidthLineEdit->setValidator( new QDoubleValidator( this ) );
	mHeightLineEdit->setValidator( new QDoubleValidator( this ) );
	setGuiElementValues();

//	mPreviewListWidget->setIconSize( QSize( 30, 30 ) );

	//add preview icons
//	addStandardDirectoriesToPreview();
	connect( mPicture, SIGNAL( itemChanged() ), this, SLOT( setGuiElementValues() ) );
	connect( mPicture, SIGNAL( rotationChanged( double ) ), this, SLOT( setGuiElementValues() ) );
}


CCipasComposerPictureWidget::~CCipasComposerPictureWidget(void)
{
}


void CCipasComposerPictureWidget::on_mPictureBrowseButton_clicked()
{
	QString openDir;
	QString lineEditText = mPictureLineEdit->text();
	if ( !lineEditText.isEmpty() )
	{
		QFileInfo openDirFileInfo( lineEditText );
		openDir = openDirFileInfo.path();
	}


	//show file dialog
	QString filePath = QFileDialog::getOpenFileName( 0,  tr("选择图片或者SVG文件"), openDir );
	if ( filePath.isEmpty() )
	{
		return;
	}

	//check if file exists
	QFileInfo fileInfo( filePath );
	if ( !fileInfo.exists() || !fileInfo.isReadable() )
	{
		QMessageBox::critical( 0,  tr("无效文件"),  tr("文件不存在或者读取错误！") );
		return;
	}

	mPictureLineEdit->blockSignals( true );
	mPictureLineEdit->setText( filePath );
	mPictureLineEdit->blockSignals( false );

	//pass file path to QgsComposerPicture
	if ( mPicture )
	{
		mPicture->beginCommand( tr( "Picture changed" ) );
		mPicture->setPictureFile( filePath );
		mPicture->update();
		mPicture->endCommand();
	}
}

void CCipasComposerPictureWidget::on_mPictureLineEdit_editingFinished()
{
	if ( mPicture )
	{
		QString filePath = mPictureLineEdit->text();
		if (filePath.isEmpty())
		{
			return;
		}
		//check if file exists
		QFileInfo fileInfo( filePath );

		if ( !fileInfo.exists() || !fileInfo.isReadable() )
		{
			//QMessageBox::critical( 0, tr("无效文件"), tr("文件不存在或者读取错误！") );
			return;
		}

		mPicture->beginCommand( tr( "Picture changed" ) );
		mPicture->setPictureFile( filePath );
		mPicture->update();
		mPicture->endCommand();
	}
}

void CCipasComposerPictureWidget::on_mWidthLineEdit_editingFinished()
{
	if ( mPicture )
	{
		QRectF pictureRect = mPicture->rect();

		bool conversionOk;
		double newWidth = mWidthLineEdit->text().toDouble( &conversionOk );
		if ( conversionOk )
		{
			mPicture->beginCommand( tr( "Picture width changed" ) );
			QRectF newSceneRect( mPicture->transform().dx(), mPicture->transform().dy(), newWidth, pictureRect.height() );
			mPicture->setSceneRect( newSceneRect );
			mPicture->endCommand();
		}
	}
}

void CCipasComposerPictureWidget::on_mHeightLineEdit_editingFinished()
{
	if ( mPicture )
	{
		QRectF pictureRect = mPicture->rect();

		bool conversionOk;
		double newHeight = mHeightLineEdit->text().toDouble( &conversionOk );
		if ( conversionOk )
		{
			mPicture->beginCommand( tr( "Picture height changed" ) );
			QRectF newSceneRect( mPicture->transform().dx(), mPicture->transform().dy(), pictureRect.width(), newHeight );
			mPicture->setSceneRect( newSceneRect );
			mPicture->endCommand();
		}
	}
}

void CCipasComposerPictureWidget::on_mRotationSpinBox_valueChanged( double d )
{
	if ( mPicture )
	{
		mPicture->beginCommand( tr( "Picture rotation changed" ), CipasComposerMergeCommand::ComposerPictureRotation );
		mPicture->setRotation( d );
		mPicture->update();
		mPicture->endCommand();
	}
}
void CCipasComposerPictureWidget::showEvent( QShowEvent * event )
{
	//refreshMapComboBox();
	QWidget::showEvent( event );
}

/*
void CCipasComposerPictureWidget::on_mPreviewListWidget_currentItemChanged( QListWidgetItem* current, QListWidgetItem* previous )
{
	if ( !mPicture || !current )
	{
		return;
	}

	QString absoluteFilePath = current->data( Qt::UserRole ).toString();
	//mPicture->beginCommand( tr( "Picture changed" ) );
	mPicture->setPictureFile( absoluteFilePath );
	mPictureLineEdit->setText( absoluteFilePath );
	mPicture->update();
	//mPicture->endCommand();
}

void CCipasComposerPictureWidget::on_mAddDirectoryButton_clicked()
{
	//let user select a directory
	QString directory = QFileDialog::getExistingDirectory( 0, tr( "Select new preview directory" ) );
	if ( directory.isNull() )
	{
		return; //dialog canceled by user
	}

	//add entry to mSearchDirectoriesComboBox
	mSearchDirectoriesComboBox->addItem( directory );

	//and add icons to the preview
	addDirectoryToPreview( directory );

	//update the image directory list in the settings
	QSettings s;
	QStringList userDirList = s.value( "/Composer/PictureWidgetDirectories" ).toStringList();
	if ( !userDirList.contains( directory ) )
	{
		userDirList.append( directory );
	}
	s.setValue( "/Composer/PictureWidgetDirectories", userDirList );
}

void CCipasComposerPictureWidget::on_mRemoveDirectoryButton_clicked()
{
	QString directoryToRemove = mSearchDirectoriesComboBox->currentText();
	if ( directoryToRemove.isEmpty() )
	{
		return;
	}
	mSearchDirectoriesComboBox->removeItem( mSearchDirectoriesComboBox->currentIndex() );

	//remove entries from back to front (to have the indices of existing items constant)
	for ( int i = ( mPreviewListWidget->count() - 1 ); i >= 0; --i )
	{
		QListWidgetItem* currentItem = mPreviewListWidget->item( i );
		if ( currentItem && currentItem->data( Qt::UserRole ).toString().startsWith( directoryToRemove ) )
		{
			delete( mPreviewListWidget->takeItem( i ) );
		}
	}

	//update the image directory list in the settings
	QSettings s;
	QStringList userDirList = s.value( "/Composer/PictureWidgetDirectories" ).toStringList();
	userDirList.removeOne( directoryToRemove );
	s.setValue( "/Composer/PictureWidgetDirectories", userDirList );
}


void CCipasComposerPictureWidget::on_mRotationFromComposerMapCheckBox_stateChanged( int state )
{
	if ( !mPicture )
	{
		return;
	}

	//mPicture->beginCommand( tr( "Rotation synchronisation toggled" ) );
	if ( state == Qt::Unchecked )
	{
		mPicture->setRotationMap( -1 );
		mRotationSpinBox->setEnabled( true );
		mComposerMapComboBox->setEnabled( false );
	}
	else
	{
		int currentItemIndex = mComposerMapComboBox->currentIndex();
		if ( currentItemIndex == -1 )
		{
			return;
		}
		int composerId = mComposerMapComboBox->itemData( currentItemIndex, Qt::UserRole ).toInt();

		mPicture->setRotationMap( composerId );
		mRotationSpinBox->setEnabled( false );
		mComposerMapComboBox->setEnabled( true );
	}
	//mPicture->endCommand();
}


void CCipasComposerPictureWidget::on_mComposerMapComboBox_activated( const QString & text )
{
	if ( !mPicture || text.isEmpty() || !mPicture->useRotationMap() )
	{
		return;
	}

	//get composition
	const CCipasComposition* composition = mPicture->composition();
	if ( !composition )
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

	const CCipasComposerMap* composerMap = composition->getComposerMapById( id );
	if ( !composerMap )
	{
		return;
	}
	//mPicture->beginCommand( tr( "Rotation map changed" ) );
	mPicture->setRotationMap( id );
	mPicture->update();
	//mPicture->endCommand();
}

void CCipasComposerPictureWidget::refreshMapComboBox()
{
	mComposerMapComboBox->blockSignals( true );
	//save the current entry in case it is still present after refresh
	QString saveCurrentComboText = mComposerMapComboBox->currentText();

	mComposerMapComboBox->clear();

	if ( mPicture )
	{
		//insert available maps into mMapComboBox
		const CCipasComposition* composition = mPicture->composition();
		if ( composition )
		{
			QList<const CCipasComposerMap*> availableMaps = composition->composerMapItems();
			QList<const CCipasComposerMap*>::const_iterator mapItemIt = availableMaps.constBegin();
			for ( ; mapItemIt != availableMaps.constEnd(); ++mapItemIt )
			{
				mComposerMapComboBox->addItem( tr( "Map %1" ).arg(( *mapItemIt )->id() ), ( *mapItemIt )->id() );
			}
		}
	}

	if ( !saveCurrentComboText.isEmpty() )
	{
		if ( mComposerMapComboBox->findText( saveCurrentComboText ) == -1 )
		{
			//the former entry is no longer present. Inform the scalebar about the changed composer map
			on_mComposerMapComboBox_activated( mComposerMapComboBox->currentText() );
		}
		else
		{
			//the former entry is still present. Make it the current entry again
			mComposerMapComboBox->setCurrentIndex( mComposerMapComboBox->findText( saveCurrentComboText ) );
		}
	}
	mComposerMapComboBox->blockSignals( false );
}
*/
void CCipasComposerPictureWidget::setGuiElementValues()
{
	//set initial gui values
	if ( mPicture )
	{
		mWidthLineEdit->blockSignals( true );
		mHeightLineEdit->blockSignals( true );
		mRotationSpinBox->blockSignals( true );
		mPictureLineEdit->blockSignals( true );
		//mComposerMapComboBox->blockSignals( true );
		//mRotationFromComposerMapCheckBox->blockSignals( true );

		mPictureLineEdit->setText( mPicture->pictureFile() );
		QRectF pictureRect = mPicture->rect();
		mWidthLineEdit->setText( QString::number( pictureRect.width() ) );
		mHeightLineEdit->setText( QString::number( pictureRect.height() ) );
		mRotationSpinBox->setValue( mPicture->rotation() );

		//refreshMapComboBox();

		//if (false )/* mPicture->useRotationMap()*/
		//{
		//	//mRotationFromComposerMapCheckBox->setCheckState( Qt::Checked );
		//	mRotationSpinBox->setEnabled( false );
		//	//mComposerMapComboBox->setEnabled( true );
		//	QString mapText = tr( "Map %1" ).arg( mPicture->rotationMap() );
		//	int itemId = mComposerMapComboBox->findText( mapText );
		//	if ( itemId >= 0 )
		//	{
		//		mComposerMapComboBox->setCurrentIndex( itemId );
		//	}
		//}
		//else
		//{
		//	mRotationFromComposerMapCheckBox->setCheckState( Qt::Unchecked );
		//	mRotationSpinBox->setEnabled( true );
		//	mComposerMapComboBox->setEnabled( false );
		//}


		//mRotationFromComposerMapCheckBox->blockSignals( false );
		mWidthLineEdit->blockSignals( false );
		mHeightLineEdit->blockSignals( false );
		mRotationSpinBox->blockSignals( false );
		mPictureLineEdit->blockSignals( false );
		//mComposerMapComboBox->blockSignals( false );
	}
}
/*
int CCipasComposerPictureWidget::addDirectoryToPreview( const QString& path )
{
	//go through all files of a directory
	QDir directory( path );
	if ( !directory.exists() || !directory.isReadable() )
	{
		return 1; //error
	}

	QFileInfoList fileList = directory.entryInfoList( QDir::Files );
	QFileInfoList::const_iterator fileIt = fileList.constBegin();

	QProgressDialog progress( "Adding Icons...", "Abort", 0, fileList.size() - 1, this );
	//cancel button does not seem to work properly with modal dialog
	//progress.setWindowModality(Qt::WindowModal);

	int counter = 0;
	for ( ; fileIt != fileList.constEnd(); ++fileIt )
	{

		progress.setLabelText( tr( "Creating icon for file %1" ).arg( fileIt->fileName() ) );
		progress.setValue( counter );
		QCoreApplication::processEvents();
		if ( progress.wasCanceled() )
		{
			break;
		}
		QString filePath = fileIt->absoluteFilePath();

		//test if file is svg or pixel format
		bool fileIsPixel = false;
		bool fileIsSvg = testSvgFile( filePath );
		if ( !fileIsSvg )
		{
			fileIsPixel = testImageFile( filePath );
		}

		//exclude files that are not svg or image
		if ( !fileIsSvg && !fileIsPixel )
		{
			++counter; continue;
		}

		QListWidgetItem * listItem = new QListWidgetItem( mPreviewListWidget );
		listItem->setFlags( Qt::ItemIsSelectable | Qt::ItemIsEnabled );

		if ( fileIsSvg )
		{
			QIcon icon( filePath );
			listItem->setIcon( icon );
		}
		else if ( fileIsPixel ) //for pixel formats: create icon from scaled pixmap
		{
			QPixmap iconPixmap( filePath );
			if ( iconPixmap.isNull() )
			{
				++counter; continue; //unknown file format or other problem
			}
			//set pixmap hardcoded to 30/30, same as icon size for mPreviewListWidget
			QPixmap scaledPixmap( iconPixmap.scaled( QSize( 30, 30 ), Qt::KeepAspectRatio ) );
			QIcon icon( scaledPixmap );
			listItem->setIcon( icon );
		}
		else
		{
			++counter; continue;
		}

		listItem->setText( "" );
		//store the absolute icon file path as user data
		listItem->setData( Qt::UserRole, fileIt->absoluteFilePath() );
		++counter;
	}

	return 0;
}

void CCipasComposerPictureWidget::addStandardDirectoriesToPreview()
{
	
	//list all directories in $prefix/share/qgis/svg
	QString svgPaths ="C:/Program Files/Quantum GIS Wroclaw/apps/qgis/svg/";
	for ( int i = 0; i < svgPaths.size(); i++ )
	{
		QDir svgDirectory( svgPaths );
		if ( !svgDirectory.exists() || !svgDirectory.isReadable() )
		{
			continue;
		}

		//add directory itself
		mSearchDirectoriesComboBox->addItem( svgDirectory.absolutePath() );
		addDirectoryToPreview( svgDirectory.absolutePath() );

		//and also subdirectories
		QFileInfoList directoryList = svgDirectory.entryInfoList( QDir::Dirs | QDir::NoDotAndDotDot );
		QFileInfoList::const_iterator dirIt = directoryList.constBegin();
		for ( ; dirIt != directoryList.constEnd(); ++dirIt )
		{
			if ( addDirectoryToPreview( dirIt->absoluteFilePath() ) == 0 )
			{
				mSearchDirectoriesComboBox->addItem( dirIt->absoluteFilePath() );
			}
		}
	}

	//include additional user-defined directories for images
	QSettings s;
	QStringList userDirList = s.value( "/Composer/PictureWidgetDirectories" ).toStringList();
	QStringList::const_iterator userDirIt = userDirList.constBegin();
	for ( ; userDirIt != userDirList.constEnd(); ++userDirIt )
	{
		addDirectoryToPreview( *userDirIt );
		mSearchDirectoriesComboBox->addItem( *userDirIt );
	}
	
}

bool CCipasComposerPictureWidget::testSvgFile( const QString& filename ) const
{
	//QSvgRenderer crashes with some (non-svg) xml documents.
	//So at least we try to sort out the ones with different suffixes
	if ( !filename.endsWith( ".svg" ) )
	{
		return false;
	}

	QSvgRenderer svgRenderer( filename );
	return svgRenderer.isValid();
}
*/
bool CCipasComposerPictureWidget::testImageFile( const QString& filename ) const
{
	QString formatName = QString( QImageReader::imageFormat( filename ) );
	return !formatName.isEmpty(); //file is in a supported pixel format
}
