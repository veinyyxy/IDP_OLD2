#include "CipasGradsHorizontalLegendStyle.h"



CCipasGradsHorizontalLegendStyle::CCipasGradsHorizontalLegendStyle(const CCipasComposerLegend* legend):CCipasLegendStyle(legend)
{
}


CCipasGradsHorizontalLegendStyle::~CCipasGradsHorizontalLegendStyle(void)
{
}

void CCipasGradsHorizontalLegendStyle::drawLegend(QPainter* p, const goto_gis::LegendInterface* pColorIndex, double rasterScaleFactor, double& currentXCoord, double& currentYCoord, double& symbolHeight, QStringList laberList, QFont mItemFont, double& maxXCoord) const
{

}
QString CCipasGradsHorizontalLegendStyle::name()const
{
	return QObject::tr("Grads风格（横向）");
	//return "GradsStyle(Horizontal)";
}

void CCipasGradsHorizontalLegendStyle::drawLabels( QPainter* p ,QStringList laberList,QFont mItemFont,double& currentXCoord,double& currentYCoord, double& maxXCoord) const
{
	int symbolTextCounts=laberList.count();

	if ( p&&symbolTextCounts>1 )
	{
		currentXCoord=0;
		currentXCoord+=mLegend->boxSpace();
		//更新Y 坐标，因为绘制色卡不会更改Y坐标
		currentYCoord+=mLegend->symbolHeight();
		//Y坐标 增加 色卡和色卡的标值之间的距离
		currentYCoord+=mLegend->iconLabelSpace();
		//Y坐标 增加 色卡标值的字体高度
		currentYCoord+=mLegend->fontAscentMillimeters( mItemFont );
		//double perCardLen=maxXCoord/symbolTextCounts;
		double perCardLen=mLegend->totalSymobolWidth()/symbolTextCounts;
		currentXCoord+=perCardLen;
		//currentXCoord +=mLegend->symbolSpace();
		//下面默认色卡段要比文字宽
		//标注是从1开始的，色卡的第一个标值不用
		//currentXCoord-=(mLegend->textWidthMillimeters( mItemFont, laberList.at(1)))/2;
		
		double textSpace=perCardLen-(mLegend->textWidthMillimeters( mItemFont, laberList.at(0)))/2;

		p->setPen( QColor( 0, 0, 0 ) );
		for (int j=0;j<laberList.count();j++)
		{
			double textwidth=mLegend->textWidthMillimeters( mItemFont,laberList.at(j).trimmed());
			double currentTextStartX=currentXCoord-(textwidth)/2;
			mLegend->drawText( p, currentTextStartX, currentYCoord  , laberList.at(j), mItemFont );
			currentXCoord +=perCardLen;
			currentXCoord +=mLegend->symbolSpace();
		}
	} 
	//currentXCoord +=mBoxSpace;	//外置

}

/*根据总长度来计算单个符号宽度*/

void CCipasGradsHorizontalLegendStyle::drawColorCard(QPainter* p, const goto_gis::LegendInterface* pLegend, double rasterScaleFactor,double& currentYCoord, double& currentXPosition, double& symbolHeight,double& maxXCoord) const
{
	COLORTABLE_VECTOR ColorIndex;

	CCipasLegendStyle::ConvertLegendInterfaceToColorTable(pLegend, &ColorIndex);

	int allcounts=ColorIndex.size();
	double itemWidth=mLegend->totalSymobolWidth()/allcounts;
	int colorcardcounts=0;
	double xStartCord=0;
	double yStartCord=0;
	QVector<COLORNODE_PRODUCT>::iterator layerIter = ColorIndex.begin();
	while(layerIter != ColorIndex.end())
	{
		QBrush mBrush;
		QPen mPen;
		mPen.setColor(mLegend->SymboloutlineColor());
		mPen.setWidthF(mLegend->SymbolOutlineWidth());
		mPen.setStyle(Qt::SolidLine);
		p->setPen(mPen);
		mBrush.setStyle(Qt::SolidPattern);
		mBrush.setColor(QColor((layerIter)->nRed,(layerIter)->nGreen,(layerIter)->nBlue));
		p->setBrush(mBrush);
		double halfPenWidth=0.5*(mLegend->SymbolOutlineWidth());
		if (colorcardcounts==0 )//左边绘制三角形
		{
			QPolygonF triangle;
			triangle << QPointF( xStartCord, 0+0.5*(mLegend->symbolHeight())*rasterScaleFactor);
			triangle << QPointF( xStartCord+itemWidth*rasterScaleFactor, 0 );
			triangle << QPointF( xStartCord+itemWidth*rasterScaleFactor, 0+ (mLegend->symbolHeight())*rasterScaleFactor );
			p->drawPolygon( triangle );
			xStartCord+=(mLegend->symbolSpace())*rasterScaleFactor;
		} 
		else if (colorcardcounts==(allcounts-1))//右边绘制三角形
		{
			QPolygonF triangle;
			triangle << QPointF( xStartCord, 0);
			triangle << QPointF( xStartCord+itemWidth*rasterScaleFactor, 0+0.5* (mLegend->symbolHeight())*rasterScaleFactor );
			triangle << QPointF( xStartCord, 0+ (mLegend->symbolHeight())*rasterScaleFactor );
			p->drawPolygon( triangle );
		}
		else
		{
			p->drawRect(xStartCord,0,itemWidth*rasterScaleFactor,(mLegend->symbolHeight())*rasterScaleFactor);
		}

		xStartCord+=(itemWidth+mLegend->symbolSpace())*rasterScaleFactor;


		++layerIter;
		colorcardcounts++;
	}
	if (ColorIndex.size()>0)
	{
		currentXPosition += (itemWidth+mLegend->symbolSpace())*colorcardcounts;
	}
}

#if 0
void CCipasGradsHorizontalLegendStyle::drawSymbolCard( QPainter* p, P_SymbolLegend_Vector *symbolLegendv ,double rasterScaleFactor,double& currentYCoord, double& currentXPosition, double& symbolHeight,double& maxXCoord ) const
{
	int allcounts = (*symbolLegendv)->size();
	double itemWidth=mLegend->totalSymobolWidth()/allcounts;
	int colorcardcounts=0;
	double xStartCord=0;
	double yStartCord=0;
	QVector<P_SymbolLegend_Product>::iterator layerIter = (*symbolLegendv)->begin();
	while(layerIter != (*symbolLegendv)->end())
	{
		double halfPenWidth=0.5*(mLegend->SymbolOutlineWidth());
		//if (colorcardcounts==0 )//左边绘制三角形
		//{
		//	QPolygonF triangle;
		//	triangle << QPointF( xStartCord, 0+0.5*(mLegend->symbolHeight())*rasterScaleFactor);
		//	triangle << QPointF( xStartCord+itemWidth*rasterScaleFactor, 0 );
		//	triangle << QPointF( xStartCord+itemWidth*rasterScaleFactor, 0+ (mLegend->symbolHeight())*rasterScaleFactor );
		//	p->drawPolygon( triangle );
		//	xStartCord+=(mLegend->symbolSpace())*rasterScaleFactor;
		//} 
		//else if (colorcardcounts==(allcounts-1))//右边绘制三角形
		//{
		//	QPolygonF triangle;
		//	triangle << QPointF( xStartCord, 0);
		//	triangle << QPointF( xStartCord+itemWidth*rasterScaleFactor, 0+0.5* (mLegend->symbolHeight())*rasterScaleFactor );
		//	triangle << QPointF( xStartCord, 0+ (mLegend->symbolHeight())*rasterScaleFactor );
		//	p->drawPolygon( triangle );
		//}
		//else
		//{
		
		//int w = (*layerIter)->pixmap.width();
		//int h = (*layerIter)->pixmap.height();
		//p->drawPixmap(xStartCord,0,w,h,(*layerIter)->pixmap);

		p->drawPixmap(xStartCord,0,itemWidth*rasterScaleFactor,(mLegend->symbolHeight())*rasterScaleFactor,(*layerIter)->pixmap);
		//p->drawRect(xStartCord,0,itemWidth*rasterScaleFactor,(mLegend->symbolHeight())*rasterScaleFactor);
		//}

		xStartCord+=(itemWidth+mLegend->symbolSpace())*rasterScaleFactor;

		++layerIter;
		colorcardcounts++;
	}
	if ((*symbolLegendv)->size()>0)
	{
		currentXPosition += (itemWidth+mLegend->symbolSpace())*colorcardcounts;
	}
}
#endif