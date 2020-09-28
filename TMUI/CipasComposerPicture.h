#pragma once

#include <QFile>
#include <QImage>
#include <QSvgRenderer>

#include "tmui_global.h"
#include "composeritem.h"

class CCipasComposerMap;

class TMUI_EXPORT CCipasComposerPicture :	public CComposerItem
{
	Q_OBJECT
public:
	CCipasComposerPicture(CCipasComposition *composition);
	~CCipasComposerPicture(void);
    /** return correct graphics item type. Added in v1.7 */
    virtual int type() const { return ComposerPicture; }

    /**Reimplementation of QCanvasItem::paint*/
    void paint( QPainter* painter, const QStyleOptionGraphicsItem* itemStyle, QWidget* pWidget );

    /**Sets the source file of the image (may be svg or a raster format)*/
    void setPictureFile( const QString& path );
    QString pictureFile() const;

    /**Sets this items bound in scene coordinates such that 1 item size units
       corresponds to 1 scene size unit and resizes the svg symbol / image*/
    void setSceneRect( const QRectF& rectangle );

   /** stores state in Dom element
       * @param elem is Dom element corresponding to 'Composer' tag
       * @param doc is Dom document
       */
    bool writeXML( QDomElement& elem, QDomDocument & doc ) const;

    ///** sets state from Dom document
    //  * @param itemElem is Dom node corresponding to item tag
    //  * @param doc is Dom document
    //  */
    bool readXML( const QDomElement& itemElem, const QDomDocument& doc );
	/*这个怎么用？？？吴恩平*/
    /**Sets the map object for rotation (by id). A value of -1 disables the map rotation*/
    void setRotationMap( int composerMapId );
    /**Returns the id of the rotation map*/
    int rotationMap() const;
    ///**True if the rotation is taken from a map item*/
    bool useRotationMap() const {return mRotationMap;}

  public slots:
    /**Sets the rotation and adapts the item rect*/
    virtual void setRotation( double r );

  private:

    enum Mode //SVG or raster graphic format
    {
      SVG,
      RASTER,
      Unknown
    };

    //default constructor is forbidden
    CCipasComposerPicture();
    /**Calculates bounding rect for svg file (mSourcefile) such that aspect ratio is correct*/
    QRectF boundedSVGRect( double deviceWidth, double deviceHeight );
    /**Calculates bounding rect for image such that aspect ratio is correct*/
    QRectF boundedImageRect( double deviceWidth, double deviceHeight );


    QImage mImage;
    QSvgRenderer mSVG;
    QFile mSourceFile;
    Mode mMode;

    QSize mDefaultSvgSize;
    ///**Map that sets the rotation (or 0 if this picture uses map independent rotation)*/
    const CCipasComposerMap* mRotationMap;
    /**Width of the picture (in mm)*/
    double mPictureWidth;
    /**Height of the picture (in mm)*/
    double mPictureHeight;

	QString m_filepath;
};

