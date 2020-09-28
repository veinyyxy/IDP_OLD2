#pragma once
#include "tmui_global.h"
#include "composeritem.h"


class TMUI_EXPORT CCipasPaperItem :public CComposerItem
{
public:
	CCipasPaperItem( CCipasComposition* c );
	CCipasPaperItem( qreal x, qreal y, qreal width, qreal height, CCipasComposition* composition );
	~CCipasPaperItem();

	/** return correct graphics item type. Added in v1.7 */
	virtual int type() const { return ComposerPaper; }
	/** \brief Reimplementation of QCanvasItem::paint*/
	void paint( QPainter* painter, const QStyleOptionGraphicsItem* itemStyle, QWidget* pWidget );

	 /** stores state in Dom element
       * @param elem is Dom element corresponding to 'Composer' tag
       * @param doc Dom document
       */
    bool writeXML( QDomElement& elem, QDomDocument & doc ) const;
	bool readXML( const QDomElement& itemElem, const QDomDocument& doc );
private:
	CCipasPaperItem();
	/**Set flags and z-value*/
	void initialize();
};

