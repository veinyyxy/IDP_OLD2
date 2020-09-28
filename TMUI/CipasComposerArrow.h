#pragma once

#include <Qpen>
#include <QBrush>

#include "tmui_global.h"
#include "composeritem.h"

#define M_PI       3.14159265358979323846

class TMUI_EXPORT CCipasComposerArrow :public CComposerItem
{
public:
	enum MarkerMode
	{
		DefaultMarker,
		NoMarker,
		SVGMarker
	};
	CCipasComposerArrow(CCipasComposition *composition);
	CCipasComposerArrow( const QPointF& startPoint, const QPointF& stopPoint, CCipasComposition* c );
	~CCipasComposerArrow(void);

	
    /** return correct graphics item type. Added in v1.7 */
    virtual int type() const { return ComposerArrow; }

    /** \brief Reimplementation of QCanvasItem::paint - draw on canvas */
    void paint( QPainter* painter, const QStyleOptionGraphicsItem* itemStyle, QWidget* pWidget );

    /**Modifies position of start and endpoint and calls QgsComposerItem::setSceneRect*/
    void setSceneRect( const QRectF& rectangle );

    /**Sets the width of the arrow head in mm*/
    void setArrowHeadWidth( double width );
    double arrowHeadWidth() const {return mArrowHeadWidth;}

    void setOutlineWidth( double width );
    double outlineWidth() const {return mPen.widthF();}

    void setStartMarker( const QString& svgPath );
    QString startMarker() const {return mStartMarkerFile;}
    void setEndMarker( const QString& svgPath );
    QString endMarker() const {return mEndMarkerFile;}

    QColor arrowColor() const { return mArrowColor; }
    void setArrowColor( const QColor& c ) { mArrowColor = c; }

    MarkerMode markerMode() const { return mMarkerMode;}
    void setMarkerMode( MarkerMode mode ) {mMarkerMode = mode;}

    /** stores state in Dom element
    * @param elem is Dom element corresponding to 'Composer' tag
    * @param doc document
    */
    bool writeXML( QDomElement& elem, QDomDocument & doc ) const;

    /** sets state from Dom document
    * @param itemElem is Dom node corresponding to item tag
    * @param doc is the document to read
    */
    bool readXML( const QDomElement& itemElem, const QDomDocument& doc );

  private:

    enum MarkerType
    {
      StartMarker,
      EndMarker
    };

	QPointF mStartPoint;
	QPointF mStopPoint;

    QPen mPen;
    QBrush mBrush;

    /**Width of the arrow marker in mm. May be specified by the user. The height is automatically adapted*/
    double mArrowHeadWidth;
    /**Height of the arrow marker in mm. Is calculated from arrow marker width and apsect ratio of svg*/
    double mStartArrowHeadHeight;
    double mStopArrowHeadHeight;

    /**Path to the start marker file*/
    QString mStartMarkerFile;
    /**Path to the end marker file*/
    QString mEndMarkerFile;
    /**Default marker, no marker or svg marker*/
    MarkerMode mMarkerMode;
    QColor mArrowColor;



    /**Adapts the item scene rect to contain the start point, the stop point including the arrow marker and the outline.
        Needs to be called whenever the arrow width/height, the outline with or the endpoints are changed*/
    void adaptItemSceneRect();
    /**Draws the default marker at the line end*/
    void drawHardcodedMarker( QPainter* p, MarkerType type );
    /**Draws a user-defined marker (must be an svg file)*/
    void drawSVGMarker( QPainter* p, MarkerType type, const QString& markerPath );
    /**Apply default graphics settings*/
    void initGraphicsSettings();
};

