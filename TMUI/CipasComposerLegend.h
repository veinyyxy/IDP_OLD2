#pragma once
#include "tmui_global.h"
#include "composeritem.h"
#include "CipasLegendModel.h"
#include "LegendInterface.h"
//#include "../XMLProvider/RadReadColor_XML.h"

class QPen;
class QColor;
class CCipasComposerGroupItem;
class CCipasComposerLayerItem;
class CCipasLegendStyle;

class CCipasComposerLegendItem;


class TMUI_EXPORT CCipasComposerLegend :public CComposerItem
{
	
	Q_OBJECT

public:
	/*图例方向*/
	enum LegendOrientation
	{
		Landscape=0,
		Portrait
	};
	CCipasComposerLegend(CCipasComposition *composition);
	~CCipasComposerLegend(void);

	void initLegendMode();
	void inline SetLegendInterface(goto_gis::LegendInterface* v);
	/** return correct graphics item type. Added in v1.7 */
	virtual int type() const { return ComposerLegend; }
	/** setup lengendOrientation **/
	void setLegendOrientation(CCipasComposerLegend::LegendOrientation a=Landscape){currentOrientation=a;}
	/**get current Legend Orientation**/
	CCipasComposerLegend::LegendOrientation currentLegendOrientation( ) const {return currentOrientation;}
	/** \brief Reimplementation of QCanvasItem::paint*/
	void paint( QPainter* painter, const QStyleOptionGraphicsItem* itemStyle, QWidget* pWidget );

	/**Paints the legend and calculates its size. If painter is 0, only size is calculated*/
	QSizeF paintAndDetermineSize( QPainter* painter );

	QSizeF paintAndDetermineSizeFromTemplate( QPainter* painter );
	/**Sets item box to the whole content*/
	void adjustBoxSize();
	void beginDrawSymbol(bool ok=true);
	/**Returns pointer to the legend model*/
	inline CCipasLegendModel* model() {return &mLegendModel;}
	//inline P_SymbolLegend_Vector _SymbolLegend_Vector() { return m_symbolLegend_v;}
	inline CCipasComposerLegendItem* GetCCipasComposerLegendItem() {return m_pRootCurrItem;}
	//void setMapTable( MapTable* mapTable);
	//MapTable* maptable(){return m_MapTable;}
	    /**Sets style by name
     @param styleName (untranslated) style name. Possibilities are: 'Single Box', 'Double Box', 'Line Ticks Middle', 'Line Ticks Down', 'Line Ticks Up', 'Numeric'*/
    void setStyle( const QString& styleName );

    /**Returns style name*/
    QString style() const;

	//inline void setDockView(MapViewWidget* dockView) {m_pDockMapView=dockView;}
	
	//setters and getters
	void setTitle( const QString& t ) {mTitle = t;}
	QString title() const {return mTitle;}

	QFont titleFont() const;
	void setTitleFont( const QFont& f );

	QFont groupFont() const;
	void setGroupFont( const QFont& f );

	QFont layerFont() const;
	void setLayerFont( const QFont& f );

	QFont itemFont() const;
	void setItemFont( const QFont& f );

	double boxSpace() const {return mBoxSpace;}
	void setBoxSpace( double s ) {mBoxSpace = s;}

	double layerSpace() const {return mLayerSpace;}
	void setLayerSpace( double s ) {mLayerSpace = s;}

	double symbolSpace() const {return mSymbolSpace;}
	void setSymbolSpace( double s ) {mSymbolSpace = s;}

	double iconLabelSpace() const {return mIconLabelSpace;}
	void setIconLabelSpace( double s ) {mIconLabelSpace = s;}

	double symbolWidth() const {return mSymbolWidth;}
	void setSymbolWidth( double w ) {mSymbolWidth = w;}

	double totalSymobolWidth() const {return mTotalSymbolWidth;}
	void setTotalSymbolWidth(double w) {mTotalSymbolWidth=w;}

	int symbolCounts() const {return mSymbolCounts;}
	void setSymbolCounts(int c){mSymbolCounts=c;}

	double SymbolOutlineWidth() const {return mSymbolOutlineWidth;}
	void setSymbolOutlineWidth( double w ) {mSymbolOutlineWidth= w;}

	double symbolHeight() const {return mSymbolHeight;}
	void setSymbolHeight( double h ) {mSymbolHeight = h;}

	/**Updates the model and all legend entries*/
	void updateLegend();

	 /** stores state in Dom node
       * @param elem is Dom element corresponding to 'Composer' tag
       * @param doc Dom document
       */
    bool writeXML( QDomElement& elem, QDomDocument & doc ) const;

	/** sets state from Dom document
       * @param itemElem is Dom node corresponding to item tag
       * @param doc is Dom document
       */
    bool readXML( const QDomElement& itemElem, const QDomDocument& doc );

	void setSymbolOutlineColor( const QColor& color );
	QColor SymboloutlineColor() const;

	/*是否从模板读取图例内容，默认是false，因为加载模板后，还可以从工具栏里 再添加图例*/
	void setLengendItem2Temp(bool b=false);
	bool isLegendItem2Temp();
	/*是否保存样式到模板*/
	void setLengendStyle2Template(bool b=true);
	bool isLegendStyle2Template();
	void setColumnNumbers( int c );
	int   columnNumbers();
	inline void  setDrawLayerName( bool b= true){ mIsDrawLayerName = b;}
	inline bool  isDrawLayerName () { return mIsDrawLayerName; }
	inline void setAttachLayerName(const QString& strName){m_strAttachLayerName = strName;}
	inline void setAttachLayerID(const int iID){m_iAttachLayerID = iID;}
	inline void getAttachLayerName(QString& strName){strName = m_strAttachLayerName;}
	inline int getAttachLayerID(){return m_iAttachLayerID;}
protected:
    QString mTitle;

    //different fonts for entries
    QFont mTitleFont;
    QFont mGroupFont;
    QFont mLayerFont;
    QFont mItemFont;



    /**Space between item box and contents*/
    double mBoxSpace;
    /**Vertical space between layer entries*/
    double mLayerSpace;
    /**Vertical space between symbol entries*/
    double mSymbolSpace;
    /**Horizontal space between item icon and label*/
    double mIconLabelSpace;
    /**Width of symbol icon*/
    double mSymbolWidth;
    /**Height of symbol icon*/
    double mSymbolHeight;
	/*符号总宽度*/
	/*mSymbolWidth=mTotalSymbolWidth/symbolCounts*/
	double mTotalSymbolWidth;

	int mSymbolCounts;
	/*符号外包线宽度*/
	double mSymbolOutlineWidth;

    CCipasLegendModel mLegendModel;
	public slots:
		/**Data changed*/
		void synchronizeWithModel();
  private:
    CCipasComposerLegend(); //forbidden

    /**Draws a group item and all subitems*/
    void drawGroupItem( QPainter* p, CCipasComposerGroupItem* groupItem, double& currentYCoord, double& maxXCoord );
    /**Draws a layer item and all subitems*/
   // void drawLayerItem( QPainter* p, Layer* layerItem, double& currentYCoord, double& maxXCoord );
	/**Draws a layer item and all subitems*/
	void drawLayerItem( QPainter* p, CCipasComposerLayerItem* layerItem, double& currentYCoord, double& maxXCoord );
	void drawLayerItemFromTemplate( QPainter* p, CCipasComposerLayerItem* layerItem, double& currentYCoord, double& maxXCoord );
    /**Draws child items of a layer item
       @param p painter
       @param layerItem parent model item (layer)
       @param currentYCoord in/out: current y position of legend item
       @param maxXCoord in/out: maximum x-coordinate of the whole legend
       @param layerOpacity opacity of the corresponding map layer
    */
    //void drawLayerChildItems( QPainter* p, Layer* layerItem, double& currentYCoord, double& maxXCoord, int layerOpacity = 255 );
	void drawLayerChildItems( QPainter* p, CCipasComposerLayerItem* layerItem, double& currentYCoord, double& maxXCoord, int layerOpacity = 255 );
	void drawLayerChildItemsFromTemplate( QPainter* p, CCipasComposerLayerItem* layerItem, double& currentYCoord, double& maxXCoord, int layerOpacity = 255 );
    /**Draws a symbol at the current y position and returns the new x position. Returns real symbol height, because for points,
     it is possible that it differs from mSymbolHeight*/
    //void drawSymbol( QPainter* p, QgsSymbol* s, double currentYCoord, double& currentXPosition, double& symbolHeight, int layerOpacity = 255 ) const;
    //void drawSymbolV2( QPainter* p, QgsSymbolV2* s, double currentYCoord, double& currentXPosition, double& symbolHeight, int layerOpacity = 255 ) const;
    //void drawPointSymbol( QPainter*, QgsSymbol* s, double currentYCoord, double& currentXPosition, double& symbolHeight, int opacity = 255 ) const;
    //void drawLineSymbol( QPainter*, QgsSymbol* s, double currentYCoord, double& currentXPosition, int opacity = 255 ) const;
    //void drawPolygonSymbol( QPainter* p, QgsSymbol* s, double currentYCoord, double& currentXPosition, int opacity = 255 ) const;

	void drawLayerSymbol3( QPainter* p, const goto_gis::LegendInterface* pOsgLegendInterface, double& currentYCoord, double& currentXPosition, double& symbolHeight,double& layerSymbolHeight,double& maxXCoord, int layerOpacity = 255 ) const;
	//void drawLayerSymbol3( QPainter* p, P_SymbolLegend_Vector *symbolLegendv, double& currentYCoord, double& currentXPosition, double& symbolHeight,double& layerSymbolHeight,double& maxXCoord, int layerOpacity = 255 ) const;
    /**Helper function that lists ids of layers contained in map canvas*/
    QStringList layerIdList() const;

	bool bDrawSymbol;
	/**lengend orientation**/
	CCipasComposerLegend::LegendOrientation currentOrientation;
	//P_COLORTABLE_VECTOR legendMap;
	goto_gis::LegendInterface* m_pLegendInterface;
	//CCipasLegendModel* legendModel;
	//QPen mPen;
	QColor mColor;
	/*legend style*/
	CCipasLegendStyle* mStyle;
	///*是否将图例项目内容保存到模板*/
	bool mIsLegendItem2temp;

	/*是否保存图例样式到模板*/
	bool mIsLegendStyle2Template;

	bool mAnchor;
	double templateHeightLegend;
	int mColumn;
	bool mIsDrawLayerName;

	CCipasComposerLegendItem* m_pRootCurrItem;

	QString m_strAttachLayerName;
	int m_iAttachLayerID;
};

