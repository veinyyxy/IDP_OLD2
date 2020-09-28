#pragma once

#include <QPen>
#include <QBrush>

#include "tmui_global.h"
#include "composeritem.h"

class TMUI_EXPORT CCipasComposerShape :public CComposerItem
{
	Q_OBJECT
public:
	enum Shape
	{
		Ellipse,
		Rectangle,
		Triangle
	};
	CCipasComposerShape(CCipasComposition *composition);
	CCipasComposerShape(qreal x, qreal y, qreal width, qreal height,CCipasComposition *composition);
	~CCipasComposerShape(void);
	/** return correct graphics item type. Added in v1.7 */
	virtual int type() const { return ComposerShape; }

	/** \brief Reimplementation of QCanvasItem::paint - draw on canvas */
	void paint( QPainter* painter, const QStyleOptionGraphicsItem* itemStyle, QWidget* pWidget );


	//setters and getters
	void setLineWidth( double width );
	double lineWidth() const;
	void setOutlineColor( const QColor& color );
	QColor outlineColor() const;
	void setFillColor( const QColor& color );
	QColor fillColor() const;
	CCipasComposerShape::Shape shapeType() const {return mShape;}
	void setShapeType( CCipasComposerShape::Shape s ) {mShape = s;}
	bool transparentFill() const;
	void setTransparentFill( bool transparent );

	/**Sets this items bound in scene coordinates such that 1 item size units
     corresponds to 1 scene size unit. Also, the shape is scaled*/
    void setSceneRect( const QRectF& rectangle );

	 /** stores state in Dom element
     * @param elem is Dom element corresponding to 'Composer' tag
     * @param doc write template file
     */
    bool writeXML( QDomElement& elem, QDomDocument & doc ) const;

	 /** sets state from Dom document
     * @param itemElem is Dom node corresponding to item tag
     * @param doc is Dom document
     */
    bool readXML( const QDomElement& itemElem, const QDomDocument& doc );
	public slots:
		/**Sets item rotation and resizes item bounds such that the shape always has the same size*/
		virtual void setRotation( double r );


private:
	/**Ellipse, rectangle or triangle*/
	Shape mShape;
	/**Shape outline*/
	QPen mPen;
	/**Shape fill*/
	QBrush mBrush;
	double mShapeWidth;
	double mShapeHeight;

	/**Apply default graphics settings*/
	void initGraphicsSettings();

	/**Returns a point on the line from startPoint to directionPoint that is a certain distance away from the starting point*/
	QPointF pointOnLineWithDistance( const QPointF& startPoint, const QPointF& directionPoint, double distance ) const;

};

