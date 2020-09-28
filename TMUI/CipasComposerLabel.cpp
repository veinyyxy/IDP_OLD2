#include <QDate>
#include <QPainter>

#include "CipasComposition.h"
#include "CipasComposerLabel.h"

CCipasComposerLabel::CCipasComposerLabel( CCipasComposition *composition )
	: CComposerItem( composition ,true)
	, mMargin( 1.0 )
	, mFontColor( QColor( 0, 0, 0 ) )
	, mHAlignment( Qt::AlignLeft )
	, mVAlignment( Qt::AlignTop )
	, mIsAdvanced(false)
{
	//default font size is 10 point
	mFont.setPointSizeF( 20 );

	mComposition = composition;
	//mId = mComposition->composerMapItems().size();
// 	int maxId = -1;
// 	QList<const CCipasComposerLabel*> mapList = mComposition->composerLabelItems();
// 	QList<const CCipasComposerLabel*>::const_iterator mapIt = mapList.constBegin();
// 	for ( ; mapIt != mapList.constEnd(); ++mapIt )
// 	{
// 		if (( *mapIt )->id() > maxId )
// 		{
// 			maxId = ( *mapIt )->id();
// 		}
// 	}
// 	mId = maxId + 1;
}


CCipasComposerLabel::~CCipasComposerLabel(void)
{
}


void CCipasComposerLabel::paint( QPainter* painter, const QStyleOptionGraphicsItem* itemStyle, QWidget* pWidget )
{
	if ( !painter )
	{
		return;
	}

	drawBackground( painter );
	painter->setPen( QPen( QColor( mFontColor ) ) ); //draw all text black
	painter->setFont( mFont );

	QFontMetricsF fontSize( mFont );

	//support multiline labels
	double penWidth = pen().widthF();
	QRectF painterRect( penWidth + mMargin, penWidth + mMargin, rect().width() - 2 * penWidth - 2 * mMargin,
		rect().height() - 2 * penWidth - 2 * mMargin );


	drawText( painter, painterRect, displayText(), mFont, mHAlignment, mVAlignment );

	drawFrame( painter );
	if ( isSelected() )
	{
		drawSelectionBoxes( painter );
	}
}

void CCipasComposerLabel::setText( const QString& text )
{
	mText = text;
}

QString CCipasComposerLabel::displayText() const
{
	QString disText = mText;
	//replaceDateText( displayText );
	if (IsAdvanced())
	{
		replaceText(disText);
	}
	if (IsAdvanced()&&disText.length()>2)
	{
		int lastPositon=disText.lastIndexOf("-");
		if (lastPositon!=-1)
		{
			QString resultsText=disText.left(lastPositon);
			return resultsText;
		}
		else
			return disText;
	}
	else
	{
		return disText;
	}
}

void CCipasComposerLabel::replaceText( QString& text )const
{
	int currentDatePos = text.indexOf( "[$LAYERNAME$]" );
	if ( currentDatePos != -1 )
	{
		text.replace( "[$LAYERNAME$]", mLayerName );
	}
}

/*
void CCipasComposerLabel::replaceDateText( QString& text ) const
{
	int currentDatePos = text.indexOf( "$CURRENT_DATE" );
	if ( currentDatePos != -1 )
	{
		//check if there is a bracket just after $CURRENT_DATE
		QString formatText;
		int openingBracketPos = text.indexOf( "(", currentDatePos );
		int closingBracketPos = text.indexOf( ")", openingBracketPos + 1 );
		if ( openingBracketPos != -1 && closingBracketPos != -1 && ( closingBracketPos - openingBracketPos ) > 1 )
		{
			formatText = text.mid( openingBracketPos + 1, closingBracketPos - openingBracketPos - 1 );
			text.replace( currentDatePos, closingBracketPos - currentDatePos + 1, QDate::currentDate().toString( formatText ) );
		}
		else //no bracket
		{
			text.replace( "$CURRENT_DATE", QDate::currentDate().toString() );
		}
	}
}

*/
void CCipasComposerLabel::setFont( const QFont& f )
{
	mFont = f;
}

void CCipasComposerLabel::adjustSizeToText()
{
	double textWidth = textWidthMillimeters( mFont, displayText() );
	double fontAscent = fontAscentMillimeters( mFont );

	setSceneRect( QRectF( transform().dx(), transform().dy(), textWidth + 2 * mMargin + 2 * pen().widthF() + 1, \
		fontAscent + 2 * mMargin + 2 * pen().widthF() + 1 ) );
}

QFont CCipasComposerLabel::font() const
{
	return mFont;
}

bool CCipasComposerLabel::writeXML( QDomElement& elem, QDomDocument & doc ) const
{
	QString alignment;

	if ( elem.isNull() )
	{
		return false;
	}

	QDomElement composerLabelElem = doc.createElement( "ComposerLabel" );

	composerLabelElem.setAttribute( "labelText", mText );
	composerLabelElem.setAttribute( "margin", QString::number( mMargin ) );

	composerLabelElem.setAttribute( "halign", mHAlignment );
	composerLabelElem.setAttribute( "valign", mVAlignment );
	composerLabelElem.setAttribute( "id", mId );
	composerLabelElem.setAttribute( "isAdvanced", mIsAdvanced );


	//font
	QDomElement labelFontElem = doc.createElement( "LabelFont" );
	labelFontElem.setAttribute( "description", mFont.toString() );
	composerLabelElem.appendChild( labelFontElem );

	//font color
	QDomElement fontColorElem = doc.createElement( "FontColor" );
	fontColorElem.setAttribute( "red", mFontColor.red() );
	fontColorElem.setAttribute( "green", mFontColor.green() );
	fontColorElem.setAttribute( "blue", mFontColor.blue() );
	composerLabelElem.appendChild( fontColorElem );

	elem.appendChild( composerLabelElem );
	return _writeXML( composerLabelElem, doc );
}

bool CCipasComposerLabel::readXML( const QDomElement& itemElem, const QDomDocument& doc )
{
	QString alignment;

	if ( itemElem.isNull() )
	{
		return false;
	}

	//restore label specific properties

	//text
	mText = itemElem.attribute( "labelText" );

	int isli2t=itemElem.attribute( "isAdvanced", "0" ).toUInt();
	mIsAdvanced=(bool) isli2t;

	//margin
	mMargin = itemElem.attribute( "margin" ).toDouble();

	//Horizontal alignment
	mHAlignment = ( Qt::AlignmentFlag )( itemElem.attribute( "halign" ).toInt() );

	//Vertical alignment
	mVAlignment = ( Qt::AlignmentFlag )( itemElem.attribute( "valign" ).toInt() );

	//id	 
	mId = itemElem.attribute( "id", "" );
	
	//font
	QDomNodeList labelFontList = itemElem.elementsByTagName( "LabelFont" );
	if ( labelFontList.size() > 0 )
	{
		QDomElement labelFontElem = labelFontList.at( 0 ).toElement();
		mFont.fromString( labelFontElem.attribute( "description" ) );
	}

	//font color
	QDomNodeList fontColorList = itemElem.elementsByTagName( "FontColor" );
	if ( fontColorList.size() > 0 )
	{
		QDomElement fontColorElem = fontColorList.at( 0 ).toElement();
		int red = fontColorElem.attribute( "red", "0" ).toInt();
		int green = fontColorElem.attribute( "green", "0" ).toInt();
		int blue = fontColorElem.attribute( "blue", "0" ).toInt();
		mFontColor = QColor( red, green, blue );
	}
	else
	{
		mFontColor = QColor( 0, 0, 0 );
	}

	//restore general composer item properties
	QDomNodeList composerItemList = itemElem.elementsByTagName( "ComposerItem" );
	if ( composerItemList.size() > 0 )
	{
		QDomElement composerItemElem = composerItemList.at( 0 ).toElement();
		_readXML( composerItemElem, doc );
	}
	emit itemChanged();
	return true;
}

void CCipasComposerLabel::setAdvaced(bool b/* =false */)
{
	mIsAdvanced=b;
}


void CCipasComposerLabel::setLayerName(const QString & lyrName)
{
	mLayerName=lyrName;
}

QString CCipasComposerLabel::getLayerName()const
{
	return mLayerName;
}
