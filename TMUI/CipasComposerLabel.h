#pragma once

#include <QFont>

#include "tmui_global.h"
#include "ComposerItem.h"

class TMUI_EXPORT CCipasComposerLabel :public CComposerItem
{
public:
	CCipasComposerLabel(CCipasComposition *composition);
	~CCipasComposerLabel(void);

	    /** return correct graphics item type. Added in v1.7 */
    virtual int type() const { return ComposerLabel; }

    /** \brief Reimplementation of QCanvasItem::paint*/
    void paint( QPainter* painter, const QStyleOptionGraphicsItem* itemStyle, QWidget* pWidget );

    /**resizes the widget such that the text fits to the item. Keeps top left point*/
    void adjustSizeToText();

    QString text() {return mText;}
    void setText( const QString& text );

    /**Returns the text as it appears on screen (with replaced data field)
      @note this function was added in version 1.2*/
    QString displayText() const;
	void replaceText( QString& text ) const;
    QFont font() const;
    void setFont( const QFont& f );
    Qt::AlignmentFlag vAlign() const { return mVAlignment; }
    Qt::AlignmentFlag hAlign() const { return mHAlignment; }
    void setHAlign( Qt::AlignmentFlag a ) {mHAlignment = a;}
    void setVAlign( Qt::AlignmentFlag a ) {mVAlignment = a;}
    double margin() {return mMargin;}
    void setMargin( double m ) {mMargin = m;}

    /**Sets text color
        @note: this function was added in version 1.4*/
    void setFontColor( const QColor& c ) {mFontColor = c;}
    /**Get font color
        @note: this function was added in version 1.4*/
    QColor fontColor() const {return mFontColor;}

	  /** stores state in Dom element
       * @param elem is Dom element corresponding to 'Composer' tag
       * @param doc document
       */
    bool writeXML( QDomElement& elem, QDomDocument & doc ) const;

	 /** sets state from Dom document
       * @param itemElem is Dom element corresponding to 'ComposerLabel' tag
       * @param doc document
       */
    bool readXML( const QDomElement& itemElem, const QDomDocument& doc );


	    /**Get label identification number
      @note this method was added in version 1.7*/
    QString id() const { return mId; }

    /**Set label identification number
      @note this method was added in version 1.7*/
    void setId( const QString& id ) { mId = id; }

	//设置是否使用高级标签
	void setAdvaced(bool b=false);
	bool IsAdvanced() const{return mIsAdvanced;}

	void setLayerName(const QString & lyrName);
	QString getLayerName()const;

  private:
    // Text
    QString mText;

    // Font
    QFont mFont;

    // Border between text and fram (in mm)
    double mMargin;

    // Font color
    QColor mFontColor;

    // Horizontal Alignment
    Qt::AlignmentFlag mHAlignment;

    // Vertical Alignment
    Qt::AlignmentFlag mVAlignment;

    // Label id (unique within the same composition)
    QString mId;

	//启用高级标签
	bool mIsAdvanced;

	QString mLayerName;
};

