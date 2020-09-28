#pragma once

#include <QStandardItem>

#include "tmui_global.h"

class QDomDocument;
class QDomElement;

class TMUI_EXPORT CCipasComposerLegendItem :public QStandardItem
{
public:
	CCipasComposerLegendItem();
	CCipasComposerLegendItem(const QString& text );
	CCipasComposerLegendItem(const QIcon& icon, const QString& text );
	virtual	 ~CCipasComposerLegendItem(void);

	virtual void writeXML( QDomElement& elem, QDomDocument& doc ) const = 0;
    /**Read item content from xml
      @param itemElem item to read from
      @param xServerAvailable Read item icons if true (QIcon needs x-server)*/
    virtual void readXML( const QDomElement& itemElem, bool xServerAvailable = true ) = 0;
	
	//virtual Layer::ELayerType itemType() const = 0;
	virtual QStandardItem* clone() const = 0;
	void setColor(const QColor c){mColor=c;}
	inline void setPixmap(const QPixmap p){mPixmap = p;}
	QColor getColor()  {return mColor;}
protected:
	void writeXMLChildren( QDomElement& elem, QDomDocument& doc ) const;
	QColor mColor;
	QPixmap mPixmap;
};

// ÆÕÍ¨Í¼²ã
class TMUI_EXPORT CCipasComposerLayerItem: public CCipasComposerLegendItem
{
public:
	CCipasComposerLayerItem();
	CCipasComposerLayerItem( const QString& text );
	virtual ~CCipasComposerLayerItem();
	virtual QStandardItem* clone() const;

	virtual void writeXML( QDomElement& elem, QDomDocument& doc ) const;
	virtual void readXML( const QDomElement& itemElem, bool xServerAvailable = true );

	//Layer::ELayerType itemType() const { return Layer::ELayerType::UnKnown_Layer; }

	void setLayerID( const QString& id ) { mLayerID = id; }
	QString layerID() const { return mLayerID; }

private:
	QString mLayerID;
};
//Õ¤¸ñÍ¼²ã £¨±£Áô£©

class TMUI_EXPORT CCipasComposerRasterSymbolItem: public CCipasComposerLegendItem
{
public:
	CCipasComposerRasterSymbolItem();
	CCipasComposerRasterSymbolItem( const QString& text );
	CCipasComposerRasterSymbolItem( const QIcon& icon, const QString& text );
	virtual ~CCipasComposerRasterSymbolItem();

	virtual QStandardItem* clone() const;

	virtual void writeXML( QDomElement& elem, QDomDocument& doc ) const;
	virtual void readXML( const QDomElement& itemElem, bool xServerAvailable = true );

	void setLayerID( const QString& id ) { mLayerID = id; }
	QString layerID() const { return mLayerID; }
	//Layer::ELayerType itemType() const { return Layer::ELayerType::Group_Layer; }

private:
	QString mLayerID;
};

//×éºÏÍ¼²ã
class TMUI_EXPORT CCipasComposerGroupItem: public CCipasComposerLegendItem
{
public:
	CCipasComposerGroupItem();
	CCipasComposerGroupItem( const QString& text );
	virtual ~CCipasComposerGroupItem();
	virtual QStandardItem* clone() const;

	virtual void writeXML( QDomElement& elem, QDomDocument& doc ) const;
	virtual void readXML( const QDomElement& itemElem, bool xServerAvailable = true );

	//Layer::ELayerType itemType() const { return Layer::ELayerType::Group_Layer; }
};
//////////////////////////////////////////////////////////////////////////
//É«°ßÍ¼Í¼²ã
/////////////////////////////////////////////////////////////////////////
class TMUI_EXPORT CCipasComposerShadedLayerItem: public CCipasComposerLegendItem
{
public:
	CCipasComposerShadedLayerItem();
	CCipasComposerShadedLayerItem( const QString& text );
	virtual ~CCipasComposerShadedLayerItem();
	virtual QStandardItem* clone() const;

	virtual void writeXML( QDomElement& elem, QDomDocument& doc ) const;
	virtual void readXML( const QDomElement& itemElem, bool xServerAvailable = true );

	//Layer::ELayerType itemType() const { return Layer::ELayerType::Shade_Layer; }

	void setLayerID( const QString& id ) { mLayerID = id; }
	QString layerID() const { return mLayerID; }

private:
	QString mLayerID;
};

//Õ¾µãÍ¼²ã
class TMUI_EXPORT CCipasComposerStationLayerItem: public CCipasComposerLegendItem
{
public:
	CCipasComposerStationLayerItem();
	CCipasComposerStationLayerItem( const QString& text );
	virtual ~CCipasComposerStationLayerItem();
	virtual QStandardItem* clone() const;

	virtual void writeXML( QDomElement& elem, QDomDocument& doc ) const;
	virtual void readXML( const QDomElement& itemElem, bool xServerAvailable = true );

	//Layer::ELayerType itemType() const { return Layer::ELayerType::Station_Layer; }

	void setLayerID( const QString& id ) { mLayerID = id; }
	QString layerID() const { return mLayerID; }

private:
	QString mLayerID;
};

// ÎÂ¶ÈÍ¼²ã
class TMUI_EXPORT CCipasComposerTemperatureLayerItem: public CCipasComposerLegendItem
{
public:
	CCipasComposerTemperatureLayerItem();
	CCipasComposerTemperatureLayerItem( const QString& text );
	virtual ~CCipasComposerTemperatureLayerItem();
	virtual QStandardItem* clone() const;

	virtual void writeXML( QDomElement& elem, QDomDocument& doc ) const;
	virtual void readXML( const QDomElement& itemElem, bool xServerAvailable = true );

	//Layer::ELayerType itemType() const { return Layer::ELayerType::Temperature_Layer; }

	void setLayerID( const QString& id ) { mLayerID = id; }
	QString layerID() const { return mLayerID; }

private:
	QString mLayerID;
};

// ½µË®Í¼²ã
class TMUI_EXPORT CCipasComposerRainLayerItem: public CCipasComposerLegendItem
{
public:
	CCipasComposerRainLayerItem();
	CCipasComposerRainLayerItem( const QString& text );
	virtual ~CCipasComposerRainLayerItem();
	virtual QStandardItem* clone() const;

	virtual void writeXML( QDomElement& elem, QDomDocument& doc ) const;
	virtual void readXML( const QDomElement& itemElem, bool xServerAvailable = true );

	//Layer::ELayerType itemType() const { return Layer::ELayerType::Rain_Layer; }

	void setLayerID( const QString& id ) { mLayerID = id; }
	QString layerID() const { return mLayerID; }

private:
	QString mLayerID;
};

