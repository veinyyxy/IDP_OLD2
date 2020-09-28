#include <QPainter>
#include <QPen>
#include <QColor>
#include <QBrush>
#include "CipasLegendStyle.h"
#include "CipasComposerLegendItem.h"
#include "CipasGradsHorizontalLegendStyle.h"
#include "CipasGradsVerticalLegendStyle.h"
#include "CipasGISLegendStyle.h"
#include "CipasComposerLegend.h"

CCipasComposerLegend::CCipasComposerLegend(CCipasComposition *composition)
	: CComposerItem( composition )
	, mTitle( tr("图例") )
	, mBoxSpace( 2 )
	, mLayerSpace( 2 )
	, mSymbolSpace( 0 )
	, mIconLabelSpace( 0 )
	, currentOrientation(Landscape)
	, mStyle(0)
	, mColumn(1)
	,mLegendModel()
	,mIsLegendItem2temp(false)
	,m_pLegendInterface(0)
	,m_iAttachLayerID(-1)
	,m_strAttachLayerName("")
{
	mTitleFont.setPointSizeF( 16.0 );
	mGroupFont.setPointSizeF( 14.0 );
	mLayerFont.setPointSizeF( 12.0 );
	mItemFont.setPointSizeF( 12.0 );
	mColor=QColor(255,255,255,0);
	mSymbolWidth = 10;
	mSymbolHeight = 5;
	mSymbolCounts=10;
	mTotalSymbolWidth=220;
	mSymbolOutlineWidth=0;
	templateHeightLegend=0;
	//mPen.setWidthF(mSymbolOutlineWidth);
	bDrawSymbol=false;
	adjustBoxSize();
	//connect( &mLegendModel, SIGNAL( layersChanged() ), this, SLOT( synchronizeWithModel() ) );
	mAnchor=false;
	mIsDrawLayerName = true ; //默认显示图层名
	
	m_pRootCurrItem= new CCipasComposerLayerItem();
	mLegendModel.clear();
	mLegendModel.insertRow(0,m_pRootCurrItem);

	//mStyle=new CCipasGradsHorizontalLegendStyle(this);
}

CCipasComposerLegend::~CCipasComposerLegend(void)
{
	delete mStyle;
}

//void CCipasComposerLegend::setMapTable(MapTable* mapTable)
//{
//	// 传递图层
//	m_MapTable=mapTable;
//	//legendModel->setMapTable(mapTable);
//}

void CCipasComposerLegend::paint( QPainter* painter, const QStyleOptionGraphicsItem* itemStyle, QWidget* pWidget )
{

	if (m_pLegendInterface)
	{
		if (mIsLegendItem2temp)
		{
			paintAndDetermineSizeFromTemplate(painter);
		}
		else
		{
			paintAndDetermineSize( painter );
		}
	}
}



QSizeF CCipasComposerLegend::paintAndDetermineSize( QPainter* painter )
{
	QSizeF size;
	double maxXCoord = 0;

	//go through model...
	QStandardItem* rootItem = mLegendModel.invisibleRootItem();

	//QHash<QString, QString>  lyrHash;
	//if (bDrawSymbol&&painter)
	//{
	//	lyrHash=m_MapTable->GetAllLayerType(Layer::ELayerType::UnKnown_Layer);
	//}
	if ( painter )
	{
		painter->save();
		drawBackground( painter );
		painter->setPen( QPen( QColor( 0, 0, 0 ) ) );
	}

	int numLayerItems = rootItem->rowCount();//mLegendModel.rowCount();//
	QStandardItem* currentLayerItem = 0;

	double currentYCoordinate = mBoxSpace;

	//font metrics
	//绘制标题
	//Y坐标 增加标题字体高度
	currentYCoordinate += fontAscentMillimeters( mTitleFont );
	if ( painter )
	{
		painter->setPen( QColor( 0, 0, 0 ) );
		drawText( painter, mBoxSpace, currentYCoordinate, mTitle, mTitleFont );
	}

	maxXCoord = 2 * mBoxSpace + textWidthMillimeters( mTitleFont, mTitle );

	for (int k=0;k<numLayerItems;k++)
	{
		currentLayerItem = rootItem->child( k );
		CCipasComposerLegendItem* currentLegendItem = dynamic_cast<CCipasComposerLegendItem*>( currentLayerItem );
		if ( currentLegendItem )
		{
			//Layer::ELayerType type = currentLegendItem->itemType();
			//if ( type == Layer::ELayerType::Group_Layer )
			//{
			//	drawGroupItem( painter, dynamic_cast<CCipasComposerGroupItem*>( currentLegendItem ), currentYCoordinate, maxXCoord );
		//	}
			//else /*if ( type == Layer::ELayerType::UnKnown_Layer )*/
			{
				drawLayerItem( painter, dynamic_cast<CCipasComposerLayerItem*>( currentLegendItem ), currentYCoordinate, maxXCoord );
			}
		}
	}

	//最后 Y坐标增加 间隔 mBoxSpace
	currentYCoordinate += mBoxSpace;
	//设置外框的宽和高
	size.setHeight( currentYCoordinate );
	size.setWidth( maxXCoord );
	setSceneRect( QRectF( transform().dx(), transform().dy(), maxXCoord, currentYCoordinate ) );
	if (painter)
	{
		/*图例左下角锚点调整*/
		if (!mAnchor&&mStyle->name()==("GIS风格")&&mIsLegendStyle2Template&&(templateHeightLegend>0.001))
		{
			setSceneRect( QRectF( transform().dx(), transform().dy()-currentYCoordinate+templateHeightLegend, maxXCoord, currentYCoordinate ) );
			mAnchor=true;
		}
	}
	if ( painter )
	{
		painter->restore();

		//draw frame and selection boxes if necessary
		drawFrame( painter );
		if ( isSelected() )
		{
			drawSelectionBoxes( painter );
		}
	}

	return size;
}

QSizeF CCipasComposerLegend::paintAndDetermineSizeFromTemplate( QPainter* painter )
{
	QSizeF size;
	double maxXCoord = 0;

	//go through model...
	QStandardItem* rootItem = mLegendModel.invisibleRootItem();
	if ( painter )
	{
		painter->save();
		drawBackground( painter );
		painter->setPen( QPen( QColor( 0, 0, 0 ) ) );
	}

	int numLayerItems = rootItem->rowCount();
	QStandardItem* currentLayerItem = 0;

	double currentYCoordinate = mBoxSpace;

	//font metrics
	//绘制标题
	//Y坐标 增加标题字体高度
	currentYCoordinate += fontAscentMillimeters( mTitleFont );
	if ( painter )
	{
		painter->setPen( QColor( 0, 0, 0 ) );
		drawText( painter, mBoxSpace, currentYCoordinate, mTitle, mTitleFont );	//mTitle 来自模板
	}

	maxXCoord = 2 * mBoxSpace + textWidthMillimeters( mTitleFont, mTitle );

	QStringList mLayerSet;

	for (int k=0;k<numLayerItems;k++)
	{
		currentLayerItem = rootItem->child( k );
		CCipasComposerLegendItem* currentLegendItem = dynamic_cast<CCipasComposerLegendItem*>( currentLayerItem );
		if ( currentLegendItem )
		{
			//Layer::ELayerType type = currentLegendItem->itemType();
			//if ( type == Layer::ELayerType::Group_Layer )
			//{
			//	drawGroupItem( painter, dynamic_cast<CCipasComposerGroupItem*>( currentLegendItem ), currentYCoordinate, maxXCoord );
			//}
			//else /*if ( type == Layer::ELayerType::UnKnown_Layer )*/
			{
				drawLayerItemFromTemplate( painter, dynamic_cast<CCipasComposerLayerItem*>( currentLegendItem ), currentYCoordinate, maxXCoord );
			}
		}
	}

	//最后 Y坐标增加 间隔 mBoxSpace
	currentYCoordinate += mBoxSpace;
	//设置外框的宽和高
	size.setHeight( currentYCoordinate );
	size.setWidth( maxXCoord );
	double rh=rect().height();
	double rw=rect().width();
	double lh=currentYCoordinate;
	double lw=maxXCoord;
	//adjust box if width or height is to small
	if ( painter && currentYCoordinate > rect().height() )
	{
		setSceneRect( QRectF( transform().dx(), transform().dy(), rect().width(), currentYCoordinate ) );
	}
	if ( painter && maxXCoord > rect().width() )
	{
		setSceneRect( QRectF( transform().dx(), transform().dy(), maxXCoord, rect().height() ) );
	}

	if ( painter )
	{
		painter->restore();

		//draw frame and selection boxes if necessary
		drawFrame( painter );
		if ( isSelected() )
		{
			drawSelectionBoxes( painter );
		}
	}

	return size;
}


void CCipasComposerLegend::drawGroupItem( QPainter* p, CCipasComposerGroupItem* groupItem, double& currentYCoord, double& maxXCoord )
{
	if ( !p || !groupItem )
	{
		return;
	}

	currentYCoord += mLayerSpace;
	currentYCoord += fontAscentMillimeters( mGroupFont );

	p->setPen( QColor( 0, 0, 0 ) );
	drawText( p, mBoxSpace, currentYCoord, groupItem->text(), mGroupFont );
	maxXCoord = qMax( maxXCoord, 2 * mBoxSpace + textWidthMillimeters( mGroupFont, groupItem->text() ) );

	//children can be other group items or layer items
	int numChildItems = groupItem->rowCount();
	QStandardItem* currentChildItem = 0;
}

void CCipasComposerLegend::drawLayerItem( QPainter* p, CCipasComposerLayerItem* layerItem, double& currentYCoord, double& maxXCoord )
{
	if ( !layerItem )
	{
		return;
	}

	//Layer* pLayer=m_MapTable->GetLayer(layerItem->layerID());
	double layerItemWidth=1.1;
	int opacity = 255;
	//Let the user omit the layer title item by having an empty layer title string
	if (mIsDrawLayerName)
	{
		
			//Y 坐标增加图层间隔
			currentYCoord += mLayerSpace;


			//draw layer Item
			if ( p  )
			{
				p->setPen( QColor( 0, 0, 0 ) );
				//如果是样式到模板，不绘制图层名 hardcode
				//调整：
				//mIsDrawLayerName
				//是否绘制图层名 

				//Y 坐标增加图层字体高度
				currentYCoord += fontAscentMillimeters( mLayerFont );
				drawText( p, mBoxSpace, currentYCoord, layerItem->text(), mLayerFont );


				//更新最大X 坐标
				maxXCoord = qMax( maxXCoord, 2 * mBoxSpace + textWidthMillimeters( mLayerFont, "" ) );
				layerItemWidth=maxXCoord;
		}
		else //layer title omited
		{
			//symbol space will be added before the item later
			currentYCoord += ( mLayerSpace - mSymbolSpace );
		}
	}
	//and child items
	drawLayerChildItems(p,layerItem,currentYCoord, maxXCoord, opacity );
	maxXCoord=qMax(layerItemWidth,maxXCoord);

}

void CCipasComposerLegend::drawLayerItemFromTemplate( QPainter* p, CCipasComposerLayerItem* layerItem, double& currentYCoord, double& maxXCoord )
{
	if ( !layerItem )
	{
		return;
	}
	double layerItemWidth=1.1;
	int opacity = 255;
	//Let the user omit the layer title item by having an empty layer title string
	if (mIsDrawLayerName)
	{
		if (! layerItem->text().isEmpty())
		{
			//Y 坐标增加图层间隔
			currentYCoord += mLayerSpace;
			//Y 坐标增加图层字体高度
			currentYCoord += fontAscentMillimeters( mLayerFont );

			//draw layer Item
			if ( p  )
			{
				p->setPen( QColor( 0, 0, 0 ) );
				QString tt=layerItem->text();
				drawText( p, mBoxSpace, currentYCoord, layerItem->text(), mLayerFont );
			}
			//更新最大X 坐标
			maxXCoord = qMax( maxXCoord, 2 * mBoxSpace + textWidthMillimeters( mLayerFont, layerItem->text() ) );
			layerItemWidth=maxXCoord;
		}
		else //layer title omited
		{
			//symbol space will be added before the item later
			currentYCoord += ( mLayerSpace - mSymbolSpace );
		}
	}
	

	//and child items
	drawLayerChildItemsFromTemplate(p,layerItem,currentYCoord, maxXCoord, opacity );

	//////////////////////////////////////////////////////////////////////////

	maxXCoord=qMax(layerItemWidth,maxXCoord);

}


void CCipasComposerLegend::beginDrawSymbol(bool ok)
{
	bDrawSymbol=ok;
}
void CCipasComposerLegend::adjustBoxSize()
{
	QSizeF size = paintAndDetermineSize( 0 );
	if ( size.isValid() )
	{
		setSceneRect( QRectF( transform().dx(), transform().dy(), size.width(), size.height() ) );
	}
}

void CCipasComposerLegend::drawLayerChildItems( QPainter* p, CCipasComposerLayerItem* layerItem, double& currentYCoord, double& maxXCoord, int layerOpacity )
{
	if ( !layerItem )
	{
		return;
	}
	//QMap<double,QColor> legendMap;
	//P_COLORTABLE_VECTOR legendMap = new COLORTABLE_VECTOR;
	QStringList symbolTextList;
	int numChildren =layerItem->rowCount(); //legendMap->size();//
	mSymbolCounts=numChildren;					//记录符号个数
	if (numChildren<1)
	{
		return;
	}
	QStandardItem* currentItem;
	for (int j=0;j<numChildren;j++)
	{
		currentItem = layerItem->child( j, 0 );
		CCipasComposerLegendItem* currentLegendItem = dynamic_cast<CCipasComposerLegendItem*>( currentItem );
		QColor c = currentLegendItem->getColor();
		//COLORNODE_PRODUCT* cp = new COLORNODE_PRODUCT;
		//cp->nRed=c.red();
		//cp->nGreen = c.green();
		//cp->nBlue = c.blue();
		//cp->fStart = 1;
		//cp->fEnd = 1;
		//(*legendMap).insert(j,cp);
		QString tts=currentLegendItem->text();
		symbolTextList.append(currentLegendItem->text());
	}
	int symbolTextCounts=0;
	symbolTextCounts=symbolTextList.count();
	double itemHeight=0;
	itemHeight=mSymbolHeight;
	itemHeight = qMax( mSymbolHeight, fontAscentMillimeters( mItemFont ) );
	double childItemsYcoord=currentYCoord + ( itemHeight - mSymbolHeight ) / 2;
	//real symbol height. Can be different from standard height in case of point symbols
	double realSymbolHeight=2;
	double realItemHeight = itemHeight; //will be adjusted if realSymbolHeight turns out to be larger

	//currentYCoord += mSymbolSpace;
	double currentXCoord = mBoxSpace;
	//currentXCoord=0;
	//Y 坐标 增加2 用来间隔图层名和色卡
	//这个参数还没有增加到界面上
	currentYCoord+=2;


	double allSymbolsHeight=0.0;
	double symbolHeightTop=currentYCoord;
	//double labelHeightTop=currentYCoord;
	drawLayerSymbol3( p, m_pLegendInterface, symbolHeightTop , currentXCoord, realSymbolHeight,allSymbolsHeight,maxXCoord, layerOpacity );
	//realItemHeight = qMax( realSymbolHeight, itemHeight );


	if ( p )
	{
		//maxXCoord-=mBoxSpace;
		mStyle->drawLabels(p,symbolTextList,mItemFont,currentXCoord,currentYCoord,maxXCoord);
		currentXCoord +=mBoxSpace;
	}
	if (mStyle->name()!=tr("Grads风格（横向）"))
	{
		currentYCoord=qMin(symbolHeightTop+allSymbolsHeight,currentYCoord);
	}
	//currentXCoord += mBoxSpace;
	maxXCoord = qMax( maxXCoord, currentXCoord );

}


void CCipasComposerLegend::drawLayerChildItemsFromTemplate( QPainter* p, CCipasComposerLayerItem* layerItem, double& currentYCoord, double& maxXCoord, int layerOpacity )
{
	if ( !layerItem )
	{
		return;
	}
	//QMap<double,QColor> legendMap;
	//P_COLORTABLE_VECTOR legendMap = new COLORTABLE_VECTOR;
	QStringList symbolTextList;
	int numChildren = layerItem->rowCount();
	mSymbolCounts=numChildren;				//记录符号个数
	if (numChildren<1)
	{
		return;
	}
	QStandardItem* currentItem;
	for (int j=0;j<numChildren;j++)
	{
		currentItem = layerItem->child( j, 0 );
		CCipasComposerLegendItem* currentLegendItem = dynamic_cast<CCipasComposerLegendItem*>( currentItem );
		QColor c = currentLegendItem->getColor();
		/*COLORNODE_PRODUCT* cp = new COLORNODE_PRODUCT;
		cp->nRed=c.red();
		cp->nGreen = c.green();
		cp->nBlue = c.blue();
		cp->fStart = 1;
		cp->fEnd = 1;
		(*legendMap).insert(j,cp);*/
		symbolTextList.append(currentLegendItem->text());
	}
	int symbolTextCounts=0;
	symbolTextCounts=symbolTextList.count();
	double itemHeight=0;
	itemHeight=mSymbolHeight;
	itemHeight = qMax( mSymbolHeight, fontAscentMillimeters( mItemFont ) );
	double childItemsYcoord=currentYCoord + ( itemHeight - mSymbolHeight ) / 2;
	for ( int i = 0; i < 1; ++i )
	{
		//real symbol height. Can be different from standard height in case of point symbols
		double realSymbolHeight=2;
		double realItemHeight = itemHeight; //will be adjusted if realSymbolHeight turns out to be larger

		//currentYCoord += mSymbolSpace;
		double currentXCoord = mBoxSpace;
		//currentXCoord=0;
		//Y 坐标 增加2 用来间隔图层名和色卡
		//这个参数还没有增加到界面上
		currentYCoord+=2;
		double allSymbolsHeight=0.0;
		drawLayerSymbol3( p, m_pLegendInterface, currentYCoord , currentXCoord, realSymbolHeight,allSymbolsHeight,maxXCoord, layerOpacity );
		//realItemHeight = qMax( realSymbolHeight, itemHeight );
		double symbolHeightTop=currentYCoord;
		if ( p&&symbolTextCounts>1 )
		{
			maxXCoord-=mBoxSpace;
			mStyle->drawLabels(p,symbolTextList,mItemFont,currentXCoord,currentYCoord,maxXCoord);
			currentXCoord +=mBoxSpace;
		}
		if (mStyle->name()!=tr("Grads风格（横向）"))
		{
			currentYCoord=qMin(symbolHeightTop+allSymbolsHeight,currentYCoord);
		}
		maxXCoord = qMax( maxXCoord, currentXCoord );
	}
}


void CCipasComposerLegend::setStyle( const QString& styleName )
{
	delete mStyle;
	mStyle = 0;

	if (styleName==tr("Grads风格（横向）"))
	{
		mStyle=new CCipasGradsHorizontalLegendStyle(this);
	}
	else if (styleName==tr("Grads风格（纵向）"))
	{
		mStyle= new CCipasGradsVerticalLegendStyle(this);
	}
	else if (styleName==tr("GIS风格"))
	{
		mStyle = new CCipasGISLegendStyle(this);
	}
}

QString CCipasComposerLegend::style() const
{
	if ( mStyle )
	{
		return mStyle->name();
	}
	else
	{
		return "";
	}
}


QStringList CCipasComposerLegend::layerIdList() const
{
	//take layer list from map renderer (to have legend order)
	if ( mComposition )
	{
		//QgsMapRenderer* r = mComposition->mapRenderer();
		//if ( r )
		//{
		//	return r->layerSet();
		//}
	}
	return QStringList();
}

void CCipasComposerLegend::synchronizeWithModel()
{
	adjustBoxSize();
	update();
}

void CCipasComposerLegend::setTitleFont( const QFont& f )
{
	mTitleFont = f;
	adjustBoxSize();
	update();
}

void CCipasComposerLegend::setGroupFont( const QFont& f )
{
	mGroupFont = f;
	adjustBoxSize();
	update();
}

void CCipasComposerLegend::setLayerFont( const QFont& f )
{
	mLayerFont = f;
	adjustBoxSize();
	update();
}

void CCipasComposerLegend::setItemFont( const QFont& f )
{
	mItemFont = f;
	adjustBoxSize();
	update();
}

QFont CCipasComposerLegend::titleFont() const
{
	return mTitleFont;
}

QFont CCipasComposerLegend::groupFont() const
{
	return mGroupFont;
}

QFont CCipasComposerLegend::layerFont() const
{
	return mLayerFont;
}

QFont CCipasComposerLegend::itemFont() const
{
	return mItemFont;
}

void CCipasComposerLegend::updateLegend()
{
	mLegendModel.setLayerSet( layerIdList() );
	adjustBoxSize();
	update();
}

bool CCipasComposerLegend::writeXML( QDomElement& elem, QDomDocument & doc ) const
{
	if ( elem.isNull() )
	{
		return false;
	}

	QDomElement composerLegendElem = doc.createElement( "ComposerLegend" );

	//write general properties
	composerLegendElem.setAttribute( "title", mTitle);
	composerLegendElem.setAttribute( "titleFont", mTitleFont.toString() );
	composerLegendElem.setAttribute( "groupFont", mGroupFont.toString() );
	composerLegendElem.setAttribute( "layerFont", mLayerFont.toString() );
	composerLegendElem.setAttribute( "itemFont", mItemFont.toString() );
	composerLegendElem.setAttribute( "boxSpace", QString::number( mBoxSpace ) );
	composerLegendElem.setAttribute( "layerSpace", QString::number( mLayerSpace ) );
	composerLegendElem.setAttribute( "symbolSpace", QString::number( mSymbolSpace ) );
	composerLegendElem.setAttribute( "iconLabelSpace", QString::number( mIconLabelSpace ) );
	composerLegendElem.setAttribute( "symbolOutlineWidth", QString::number( mSymbolOutlineWidth ) );
	QString colors=QString::number(mColor.red())+":"+QString::number(mColor.green())+":"+QString::number(mColor.blue());
	composerLegendElem.setAttribute("symbolOutlinecolors",colors);
	composerLegendElem.setAttribute( "symbolWidth", mSymbolWidth );
	composerLegendElem.setAttribute( "symbolHeight", mSymbolHeight );
	composerLegendElem.setAttribute( "totalSmbolHeight", mTotalSymbolWidth );
	composerLegendElem.setAttribute( "style", style() );
	composerLegendElem.setAttribute( "isItem2template", mIsLegendItem2temp );
	composerLegendElem.setAttribute( "isLegendStyle2Template", mIsLegendStyle2Template );
	composerLegendElem.setAttribute( "isDrawLayerName", mIsDrawLayerName );

	//write model properties
	mLegendModel.writeXML( composerLegendElem, doc );

	elem.appendChild( composerLegendElem );
	return _writeXML( composerLegendElem, doc );
}

bool CCipasComposerLegend::readXML( const QDomElement& itemElem, const QDomDocument& doc )
{
	if ( itemElem.isNull() )
	{
		return false;
	}

	//read general properties
	mTitle = itemElem.attribute( "title" );
	//title font
	QString titleFontString = itemElem.attribute( "titleFont" );
	if ( !titleFontString.isEmpty() )
	{
		mTitleFont.fromString( titleFontString );
	}
	//group font
	QString groupFontString = itemElem.attribute( "groupFont" );
	if ( !groupFontString.isEmpty() )
	{
		mGroupFont.fromString( groupFontString );
	}

	//layer font
	QString layerFontString = itemElem.attribute( "layerFont" );
	if ( !layerFontString.isEmpty() )
	{
		mLayerFont.fromString( layerFontString );
	}
	//item font
	QString itemFontString = itemElem.attribute( "itemFont" );
	if ( !itemFontString.isEmpty() )
	{
		mItemFont.fromString( itemFontString );
	}

	//spaces
	mBoxSpace = itemElem.attribute( "boxSpace", "2.0" ).toDouble();
	mLayerSpace = itemElem.attribute( "layerSpace", "3.0" ).toDouble();
	mSymbolSpace = itemElem.attribute( "symbolSpace", "2.0" ).toDouble();
	mIconLabelSpace = itemElem.attribute( "iconLabelSpace", "2.0" ).toDouble();
	mSymbolWidth = itemElem.attribute( "symbolWidth", "7.0" ).toDouble();
	mSymbolHeight = itemElem.attribute( "symbolHeight", "14.0" ).toDouble();
	mTotalSymbolWidth=itemElem.attribute( "totalSmbolHeight", "220.0" ).toDouble();
	mSymbolOutlineWidth=itemElem.attribute( "symbolOutlineWidth", "0.0" ).toDouble();
	QString inColors=itemElem.attribute("symbolOutlinecolors","255:255:255");
	QStringList inColorLs=inColors.split(":");
	QColor inColor(inColorLs.at(0).toUInt(),inColorLs.at(1).toUInt(),inColorLs.at(2).toUInt());
	mColor=inColor;
	//设置图例默认风格
	QString defaultStyle=itemElem.attribute("style",tr("Grads风格（横向）"));
	setStyle(defaultStyle);
	//是否从模板读取图例内容
	int isli2t=itemElem.attribute( "isItem2template", "0" ).toUInt();
	mIsLegendItem2temp=(bool) isli2t;
	//是否从模板读取图例内容
	isli2t=itemElem.attribute( "isLegendStyle2Template", "1" ).toUInt();
	mIsLegendStyle2Template=(bool) isli2t;
	//是否绘制图层名
	mIsDrawLayerName = (bool)itemElem.attribute( "isDrawLayerName", "1" ).toUInt();
	//read model properties
	QDomNodeList modelNodeList = itemElem.elementsByTagName( "Model" );
	mSymbolCounts=modelNodeList.count();
	int sc=modelNodeList.size();
	if ( modelNodeList.size() > 0 )
	{
		QDomElement modelElem = modelNodeList.at( 0 ).toElement();
		mLegendModel.readXML( modelElem, doc );
	}

	//restore general composer item properties
	QDomNodeList composerItemList = itemElem.elementsByTagName( "ComposerItem" );
	if ( composerItemList.size() > 0 )
	{
		QDomElement composerItemElem = composerItemList.at( 0 ).toElement();
		_readXML( composerItemElem, doc );
		templateHeightLegend=templateHeight;
	}

	emit itemChanged();
	return true;
}

void CCipasComposerLegend::setSymbolOutlineColor( const QColor& color )
{
	//mPen.setColor( color );
	////wuenping  hardcode 外包线可以设为背景色并且宽度为0，
	//mPen.setStyle(Qt::PenStyle::SolidLine);

	mColor=color;
}

QColor CCipasComposerLegend::SymboloutlineColor() const
{
	//return mPen.color();
	return mColor;
}

void CCipasComposerLegend::setLengendItem2Temp(bool b)
{
	mIsLegendItem2temp=b;
}

bool CCipasComposerLegend::isLegendItem2Temp()
{
	return mIsLegendItem2temp;
}

void CCipasComposerLegend::setLengendStyle2Template(bool b/* =true */)
{
	mIsLegendStyle2Template=b;
}

bool CCipasComposerLegend::isLegendStyle2Template()
{
	return mIsLegendStyle2Template;
}

void CCipasComposerLegend::setColumnNumbers( int c )
{
	mColumn = c ;
}

int CCipasComposerLegend::columnNumbers()
{
	return mColumn;
}

void CCipasComposerLegend::drawLayerSymbol3( QPainter* p,  const goto_gis::LegendInterface* pOsgLegendInterface, double& currentYCoord, double& currentXPosition, double& symbolHeight,double& layerSymbolHeight,double& maxXCoord, int layerOpacity /*= 255 */ ) const
{
	if ( !p )
	{
		return;
	}

	double rasterScaleFactor = 1.0;
	if ( p )
	{
		QPaintDevice* paintDevice = p->device();
		if ( !paintDevice )
		{
			return;
		}
		rasterScaleFactor = ( paintDevice->logicalDpiX() + paintDevice->logicalDpiY() ) / 2.0 / 25.4;
	}
	p->save();
	p->translate( currentXPosition, currentYCoord );
	p->scale( 1.0 / rasterScaleFactor, 1.0 / rasterScaleFactor );
	//////////////////////////////////////////////////////////////////////////
	//drawColorCard 的参数currentYCoord 在此处有不同的含义：
	//返回绘制之后符号的高度而不是坐标
	//////////////////////////////////////////////////////////////////////////
	if(mStyle&&pOsgLegendInterface)
	{
		mStyle->drawColorCard(p,pOsgLegendInterface,rasterScaleFactor,layerSymbolHeight,currentXPosition,symbolHeight,maxXCoord);

	}
	p->restore();
	maxXCoord=currentXPosition;
	//maxXCoord+=mBoxSpace;
	symbolHeight = mSymbolHeight;
	layerSymbolHeight /=rasterScaleFactor;
	//currentYCoord+=layerSymbolHeight;
}

#if 0
void CCipasComposerLegend::drawLayerSymbol3( QPainter* p, P_SymbolLegend_Vector *symbolLegendv, double& currentYCoord, double& currentXPosition, double& symbolHeight,double& layerSymbolHeight,double& maxXCoord, int layerOpacity /*= 255 */ ) const
{
	if ( !p )
	{
		return;
	}

	double rasterScaleFactor = 1.0;
	if ( p )
	{
		QPaintDevice* paintDevice = p->device();
		if ( !paintDevice )
		{
			return;
		}
		rasterScaleFactor = ( paintDevice->logicalDpiX() + paintDevice->logicalDpiY() ) / 2.0 / 25.4;
	}
	p->save();
	p->translate( currentXPosition, currentYCoord );
	p->scale( 1.0 / rasterScaleFactor, 1.0 / rasterScaleFactor );
	//////////////////////////////////////////////////////////////////////////
	//drawColorCard 的参数currentYCoord 在此处有不同的含义：
	//返回绘制之后符号的高度而不是坐标
	//////////////////////////////////////////////////////////////////////////
	if(mStyle&&(*symbolLegendv)->size()>0)
	{
		mStyle->drawSymbolCard(p,symbolLegendv,rasterScaleFactor,layerSymbolHeight,currentXPosition,symbolHeight,maxXCoord);

	}
	p->restore();
	maxXCoord=currentXPosition;
	//maxXCoord+=mBoxSpace;
	symbolHeight = mSymbolHeight;
	layerSymbolHeight /=rasterScaleFactor;
	//currentYCoord+=layerSymbolHeight;
}
#endif

void CCipasComposerLegend::SetLegendInterface(goto_gis::LegendInterface* v)
{
	if (v)
	{
		m_pLegendInterface =  v;
	}
}

void CCipasComposerLegend::initLegendMode()
{
	QVector< QPair<double, double> > valueList;
	QPair<double, double> pairValue;
	osg::Vec4 v4Color;

	if(m_pLegendInterface)
	{
		mLegendModel.clear();
		CCipasComposerLegendItem* currItem = new CCipasComposerLayerItem();
		mLegendModel.insertRow(currItem->rowCount(),currItem);

		mStyle->GetValueListFromLegendInterface(m_pLegendInterface, valueList);

		for(int i = 0; valueList.count() > i; i++)
		{
			pairValue = valueList.at(i);

			CCipasComposerLegendItem* item = new CCipasComposerLayerItem();
			item->setText(QString::number(pairValue.first)+"~"+QString::number(pairValue.second));

			double dValue = (pairValue.first + pairValue.second) / 2.0;
			if(m_pLegendInterface->GetLegendColor(dValue, &v4Color) == -1)
				return;

			item->setColor(QColor::fromRgb(v4Color.r() * 255.0, v4Color.g() * 255.0, v4Color.b() * 255.0));
			currItem->insertRow(i,item);
		}
	}
}
