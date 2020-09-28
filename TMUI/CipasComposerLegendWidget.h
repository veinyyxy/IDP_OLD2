#pragma once

#include <QWidget>

#include "tmui_global.h"
#include "ui_cipascomposerlegendwidgetbase.h"

class CCipasComposerLegend;


typedef QPair< QString, QList<QString> > GroupLayerInfo;

class TMUI_EXPORT CCipasComposerLegendWidget :public QWidget,private Ui::CipasComposerLegendWidgetBase
{
	Q_OBJECT

public:
	CCipasComposerLegendWidget(CCipasComposerLegend* legend);
	~CCipasComposerLegendWidget(void);
	QList< GroupLayerInfo > groupLayerSet();
	QStringList layerSet();
	void updateLegend();

	void updateLegendOnlyStyle();

	public slots:

		void on_mTitleLineEdit_textChanged( const QString& text );
		void on_mSymbolWidthSpinBox_valueChanged( double d );
		void on_mTotalSymbolWidthSpinBox_valueChanged( double d );
		void on_mSymbolOutlineWidthSpinBox_valueChanged(double d );
		void on_mSymbolHeightSpinBox_valueChanged( double d );
		void on_mLayerSpaceSpinBox_valueChanged( double d );
		void on_mSymbolSpaceSpinBox_valueChanged( double d );
		void on_mIconLabelSpaceSpinBox_valueChanged( double d );
		void on_mTitleFontButton_clicked();
		//void on_mGroupFontButton_clicked();
		void on_mLayerFontButton_clicked();
		void on_mItemFontButton_clicked();
		void on_mSymbolOutlineColorButton_clicked();
		void on_mBoxSpaceSpinBox_valueChanged( double d );
		void on_mCheckBoxAutoUpdate_stateChanged( int state );
		void on_mCheckBoxLayerDraw_stateChanged( int state );
		void on_mcolumnSpinbox_valueChanged( int d );
		
		//orientation
		//void on_mLegendOrientationComboBox_currentIndexChanged( const QString& text );
		//style
		void on_mLegendSytleComboBox_currentIndexChanged( const QString& text );

		//ͼ�����ݱ��浽ģ�壬����ģ���ȡͼ���������ݶ�����������ʽ
		void on_mLegendItem2tempCheckBox_stateChanged(int state);
		//ͼ����ʽ��ģ�壬��ģ���ȡͼ������ ͼ��������ʽ�������ݻ����
		void on_mLegendStyle2templateCheckBox_stateChanged(int state);

		//item manipulation
		void on_mMoveDownToolButton_clicked();
		void on_mMoveUpToolButton_clicked();
		void on_mRemoveToolButton_clicked();
		void on_mAddToolButton_clicked();
		void on_mEditPushButton_clicked();
		void on_mUpdatePushButton_clicked();
		void on_mUpdateAllPushButton_clicked();
		//void on_mAddGroupButton_clicked();
		void on_mItemTreeView_doubleClicked ( const QModelIndex & index ) ;
		private slots:
			/**Sets GUI according to state of mLegend*/
			void setGuiElements();


private:
	CCipasComposerLegendWidget();
	void blockAllSignals( bool b );
	QIcon getThemeIcon( const QString theName );
	CCipasComposerLegend* mLegend;
};

