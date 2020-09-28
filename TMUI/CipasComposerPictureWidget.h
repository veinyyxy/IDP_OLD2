#pragma once

#include <QWidget>

#include "tmui_global.h"
#include "ui_cipascomposerpicturewidgetbase.h"

class CCipasComposerPicture;


class TMUI_EXPORT CCipasComposerPictureWidget :public QWidget,private Ui::CipasComposerPictureWidgetBase
{
	Q_OBJECT
public:
	CCipasComposerPictureWidget( CCipasComposerPicture* picture);
	~CCipasComposerPictureWidget(void);

public slots:
	void on_mPictureBrowseButton_clicked();
	void on_mPictureLineEdit_editingFinished();
	void on_mRotationSpinBox_valueChanged( double d );
	void on_mWidthLineEdit_editingFinished();
	void on_mHeightLineEdit_editingFinished();
	//void on_mPreviewListWidget_currentItemChanged( QListWidgetItem* current, QListWidgetItem* previous );
	//void on_mAddDirectoryButton_clicked();
	//void on_mRemoveDirectoryButton_clicked();
	//void on_mRotationFromComposerMapCheckBox_stateChanged( int state );
	//void on_mComposerMapComboBox_activated( const QString & text );

private slots:
	/**Sets the GUI elements to the values of mPicture*/
	void setGuiElementValues();

protected:
	void showEvent( QShowEvent * event );

private:
	CCipasComposerPicture* mPicture;
	/**Add the icons of a directory to the preview. Returns 0 in case of success*/
	//int addDirectoryToPreview( const QString& path );
	/**Add the icons of the standard directories to the preview*/
	//void addStandardDirectoriesToPreview();
	/**Tests if a file is valid svg*/
	//bool testSvgFile( const QString& filename ) const;
	/**Tests if a file is a valid pixel format*/
	bool testImageFile( const QString& filename ) const;
	/**Updates the map combo box with the current composer map ids*/
	//void refreshMapComboBox();
};

