#pragma once

#include <QWidget>

#include "tmui_global.h"
#include "ui_cipascompositionwidgetbase.h"

class CCipasComposition;

/** \ingroup MapComposer
 * Struct to hold map composer paper properties.
 */
struct CipasCompositionPaper
{
  CipasCompositionPaper( QString name, double width, double height ) {mName = name; mWidth = width; mHeight = height;}
  QString mName;
  double mWidth;
  double mHeight;
};

class TMUI_EXPORT CCipasCompositionWidget :
	public QWidget,private Ui::CipasCompositionWidgetBase
{
	Q_OBJECT
public:
	CCipasCompositionWidget(QWidget* parent, CCipasComposition* c);
	~CCipasCompositionWidget(void);
	inline bool doubleNear( double a, double b, double epsilon = 4 * DBL_EPSILON )
	{
		const double diff = a - b;
		return diff > -epsilon && diff <= epsilon;
	}
public slots:
	void on_mPaperSizeComboBox_currentIndexChanged( const QString& text );
	void on_mPaperUnitsComboBox_currentIndexChanged( const QString& text );
	void on_mPaperOrientationComboBox_currentIndexChanged( const QString& text );
	void on_mPaperWidthDoubleSpinBox_editingFinished();
	void on_mPaperHeightDoubleSpinBox_editingFinished();
	void on_mResolutionSpinBox_valueChanged( const int value );
	void on_mPrintAsRasterCheckBox_stateChanged( int state );

	void on_mSnapToGridCheckBox_stateChanged( int state );
	void on_mGridResolutionSpinBox_valueChanged( double d );
	void on_mOffsetXSpinBox_valueChanged( double d );
	void on_mOffsetYSpinBox_valueChanged( double d );
	void on_mGridColorButton_clicked();
	void on_mGridStyleComboBox_currentIndexChanged( const QString& text );
	void on_mPenWidthSpinBox_valueChanged( double d );
	/**Sets GUI elements to width/height from composition*/
	void displayCompositionWidthHeight();
	////////////////////////////////////////////////////////////////////////////////////////////
	void on_mpColorButton_clicked();
	void on_mpLineStyle_currentIndexChanged(const QString& text);
	//void on_mpConerSytle_currentIndexChanged(const QString& text);
	void on_mpLineNum_editingFinished();
	void on_mpLineInterval_editingFinished();
	void on_mpLineWidth_editingFinished();
	//void on_mpLineDashOffset_editingFinished();
	void on_mpIsDrawLineRect_stateChanged(int state);
	void on_mMarginXSpinBox_valueChanged( double d );
	void on_mMarginYSpinBox_valueChanged( double d );

private:
	CCipasComposition* mComposition;
	QMap<QString, CipasCompositionPaper> mPaperMap;

	CCipasCompositionWidget(); //default constructor is forbidden
	/**Sets width/height to chosen paper format and updates paper item*/
	void applyCurrentPaperSettings();
	/**Applies the current width and height values*/
	void applyWidthHeight();
	/**Makes sure width/height values for custom paper matches the current orientation*/
	void adjustOrientation();
	/**Sets GUI elements to snaping distances of composition*/
	void displaySnapingSettings();

	void createPaperEntries();
	void insertPaperEntries();

	double size( QDoubleSpinBox *spin );
	void setSize( QDoubleSpinBox *spin, double v );
	/**Blocks / unblocks the signals of all items*/
	void blockSignals( bool block );
};

