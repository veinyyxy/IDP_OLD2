#pragma once

#include <QtGui/QMainWindow>
#include <QDomDocument>
#include <QDomElement>
#include <QtCore/QPointer>
#include <QtCore/QScopedPointer>
#include <QSharedPointer>
#include <QPrinter>
#include <osgViewer/View>
#include "tmui_global.h"
#include "ui_cipascomposerbase.h"
#include "../XMLProvider/RadReadColor_XML.h"
#include "AdapterWidget.h"

class QGridLayout;
class QDomNode;
class QDomDocument;
class QMoveEvent;
class QResizeEvent;
class QFile;
class QSizeGrip;
class QUndoView;
class QGraphicsView;


class CCipasComposition;
class CCipasPaperItem;
class CComposerItem;
class CCipasComposerView;
class CCipasComposerLabel;
class CCipasComposerPicture;
class CCipasComposerMap;
class CCipasComposerArrow;
class CCipasComposerShape;
class CCipasComposerScaleBar;
class CCipasComposerLegend;

class CCipasComposerGLW;

class TMUI_EXPORT CCipasComposer : public QMainWindow,public Ui::CipasComposerBase
{
	Q_OBJECT
public:
	typedef std::vector<osgViewer::View*> OSGVIEW_VECTOR, *P_OSGVIEW_VECTOR;
	CCipasComposer(QWidget* parent = 0);
	~CCipasComposer(void);
	//void inline Set_P_COLORTABLE_VECTOR(P_COLORTABLE_VECTOR l) { legendMap = l;}
	//void inline SetSymbolLegend_Vector(P_SymbolLegend_Vector v) {m_symbolLegend_v =  v;}

	void setupTheme();
	void setuoUi2();
	QIcon getThemeIcon( const QString theName );
	void setIconSizes( int size );
	
	void open();

	void zoomFull();

	CCipasComposerView *view( void );

	void showCompositionOptions( QWidget *w );

	void restoreWindowState();

	void saveWindowState();

	QAction* windowAction() {return mWindowAction;}

	const QString& title() const {return mTitle;}
	void setTitle( const QString& title );

	void LoadTemplatefromfile(const QString& filename);

	void ExportToImage(QString& saveFileName,int dpi);

	void setExtent(double xmin,double ymin,double xmax,double ymax);
	
	inline void setOnlineGraphic(bool b) { isOnlineGraphic = b; }
	inline bool Onlinegraphic() { return isOnlineGraphic; }

	void setLoadFromTemplateIsClicked(bool b=false){mbLoadFromTemplate_clicked=b;}

	bool loadFromTemplateIsClicked() {return mbLoadFromTemplate_clicked;}

	void setOsgView(ViewerQT* pView);

	void UpdateView(ViewerQT* pView);
protected:
	//! Move event
	virtual void moveEvent( QMoveEvent * );

	//! Resize event
	virtual void resizeEvent( QResizeEvent * );

	//! Close event
	virtual void closeEvent(QCloseEvent *event);

signals:
	//! Is emitted every time the view zoom has changed
	void zoomLevelChanged();

	void composerAdded( CCipasComposerView* v );
	//!Composer deletes the old composerview when loading a template
	void composerWillBeRemoved( CCipasComposerView* v );

	void updateImage(QImage *img);

	void eMapFull();
	void eMapZoonIn();
	void eMapZoonOut();
	void eMapMove(QPoint beginPoint , QPoint endPoint);
public slots:
	//模板库的slots
	//保存对模板库的改变
	void on_mSavePushButton_clicked();
	//载入选定的模板
	void on_mOpenPushButton_clicked();
	//更新模板的描述信息
	void on_mCipasTemplateTree_itemClicked(QTreeWidgetItem *item, int column);
	//双击打开选中item
	void on_mCipasTemplateTree_itemDoubleClicked ( QTreeWidgetItem * item, int column );
	//模板管理：保存
	void on_saveT_clicked();
	//模板管理：另存
	void on_saveAsT_clicked();
	//新建空白模板
	void on_newBlankT_clicked();
	//模板管理：删除分类/项
	void on_mRemoveToolButton_clicked();
	//模板管理：编辑项目名称
	void on_mEditToolButton_clicked();
	//增加新项
	void on_mAddToolButton_clicked();

	void on_mCompositionNameComboBox_activated(const QString& text);

	//! Move selected items one position up
	void on_mActionRaiseItems_triggered();

	//!Move selected items one position down
	void on_mActionLowerItems_triggered();

	//!Move selected items to top
	void on_mActionMoveItemsToTop_triggered();

	//!Move selected items to bottom
	void on_mActionMoveItemsToBottom_triggered();

	//!Align selected composer items left
	void on_mActionAlignLeft_triggered();

	//!Align selected composere items horizontally centered
	void on_mActionAlignHCenter_triggered();

	//!Align selected composer items right
	void on_mActionAlignRight_triggered();

	//!Align selected composer items to top
	void on_mActionAlignTop_triggered();

	//!Align selected composer items vertically centered
	void on_mActionAlignVCenter_triggered();

	//!Align selected composer items to bottom
	void on_mActionAlignBottom_triggered();

	void on_mActionLoadFromTemplate_triggered();

	void on_mActionSaveAsTemplate_triggered();

	//! Zoom to full extent of the paper
	void on_mActionZoomAll_triggered();

	//! Zoom in
	void on_mActionZoomIn_triggered();

	//! Zoom out
	void on_mActionZoomOut_triggered();

	//! Refresh view
	void on_mActionRefreshView_triggered();

	//导出图片
	void on_mActionExportAsImage_triggered();

	//复制到剪切板
	void on_mActionExport2Clipboard_triggered();

	//! Print as PDF
	void on_mActionExportAsPDF_triggered();
	void on_mActionExportAsEPS_triggered();
	//svg
	void on_mActionExportAsSVG_triggered();

	//! Print the composition
	void on_mActionPrint_triggered();

	//! Page Setup for composition
//	void on_mActionPageSetup_triggered();
	//! Add new legend
	void on_mActionAddNewLegend_triggered();
	//! Add new label
	void on_mActionAddNewLabel_triggered();
	//! Add new picture
	void on_mActionAddImage_triggered();
	//! Add new map
	void on_mActionAddNewMap_triggered();

	//! Group selected items
	void on_mActionGroupItems_triggered();

	//! Ungroup selected item group
	void on_mActionUngroupItems_triggered();

	void on_mActionMoveItemContent_triggered();

	//! Map Zoom to full extent of the paper
	void on_mActionMapZoomAll_triggered();

	//! Map Zoom in
	void on_mActionMapZoomIn_triggered();

	//! Map Zoom out
	void on_mActionMapZoomOut_triggered();

	//! Add ellipse shape item
	void on_mActionAddBasicShape_triggered();

	void on_mActionAddGLW_triggered();

	void on_mActionAddArrow_triggered();
	//! Add new scalebar
	void on_mActionAddNewScalebar_triggered();
	void addComposerGLW(CCipasComposerGLW* glw);
	/**Adds a composer label to the item/widget map and creates a configuration widget for it*/
	void addComposerLabel( CCipasComposerLabel* label );
	/**Add a composer map to the item/widget map and creates a configuration widget for it*/
	void addComposerMap( CCipasComposerMap* map );
	/**Adds a composer picture to the item/widget map and creates a configuration widget*/
	void addComposerLegend( CCipasComposerLegend* legend );
	void addComposerPicture( CCipasComposerPicture* picture );
	/**Adds a composer shape to the item/widget map and creates a configuration widget*/
	void addComposerScaleBar( CCipasComposerScaleBar* scalebar );
	void addComposerArrow( CCipasComposerArrow* arrow );
	void addComposerShape( CCipasComposerShape* shape );
	void setSelectionTool();
	//! Select item
	//!Undo last composer change
	void on_mActionUndo_triggered();

	//!Redo last composer change
	void on_mActionRedo_triggered();

	/**Removes item from the item/widget map and deletes the configuration widget. Does not delete the item itself*/
	void deleteItem( CComposerItem* item );
	void on_mActionSelectMoveItem_triggered();
	/**Shows the configuration widget for a composer item*/
	void showItemOptions( CComposerItem* i );
	//! Raise, unminimize and activate this window
	void activate();
private:
	void refreshmCompositionNameComboBox();

	//布局窗口自适应 主要是工作区和属性设置区的比例
	void selfAdaptLayout();

	void connectSlots();
	    //! Print to a printer object
    void print( QPrinter &printer );

	//! Writes state under DOM element
	void writeXML( QDomNode& parentNode, QDomDocument& doc );
	//! Sets state from Dom document
	void readXML( const QDomDocument& doc );
	void readXML( const QDomElement& composerElem, const QDomDocument& doc, bool fromTemplate = false );

	//! Changes elements that are not suitable for this project
	void cleanupAfterTemplateRead();

	//! Removes all the item from the graphics scene and deletes them
	void deleteItems();

	/**Composer title*/
	/**Composer title*/
	QString mTitle;

	QString mCurrentTemplateName;
	//! Pointer to composer view
	CCipasComposerView *mView;

	//! Current composition
	CCipasComposition *mComposition;


	////! Pointer to QGIS application
	//QgisApp *mQgis;

	//! The composer was opened first time (-> set defaults)
	bool mFirstTime;

	//! Layout
	QGridLayout *mCompositionOptionsLayout;

	//! Layout
	QGridLayout *mItemOptionsLayout;

	//! Size grip
	QSizeGrip *mSizeGrip;

	//! To know which item to show if selection changes
	QMap<CComposerItem*, QWidget*> mItemWidgetMap;

	//! Window menu action to select this window
	QAction *mWindowAction;

	//! Page & Printer Setup
	QPrinter mPrinter;

	QUndoView* mUndoView;
	

	

	QMainWindow *m_mainwind;

	//QPointer<MapTabWidget> m_pMapTabWidget;	//添加Tab
//	MapTabWidget m_pMapTabWidget;
	//是否点击加载模板按钮
	bool mbLoadFromTemplate_clicked;
	
	//经纬度四至范围
	double mxmin;
	double mymin;
	double mxmax;
	double mymax;
	//是否从外部传出经纬度四至范围
	bool isExtentInput;
	bool isOnlineGraphic;
	//P_COLORTABLE_VECTOR legendMap;
	//P_SymbolLegend_Vector m_symbolLegend_v;

	ViewerQT* m_osgViewers;
	ViewerQT* m_osgOldViewers;
};

