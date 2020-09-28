#include "CipasGradsVerticalLegendStyle.h"


CCipasGradsVerticalLegendStyle::CCipasGradsVerticalLegendStyle(const CCipasComposerLegend* legend):CCipasLegendStyle(legend)
{
}


CCipasGradsVerticalLegendStyle::~CCipasGradsVerticalLegendStyle(void)
{
}

QString CCipasGradsVerticalLegendStyle::name()const
{
	return QObject::tr("Grads风格（纵向）");
	//return "GradsStyle(Vertical)";
}
void CCipasGradsVerticalLegendStyle::drawLegend(QPainter* p, const goto_gis::LegendInterface* pColorIndex, double rasterScaleFactor, double& currentXCoord, double& currentYCoord, double& symbolHeight, QStringList laberList, QFont mItemFont, double& maxXCoord) const
{

}
void CCipasGradsVerticalLegendStyle::drawLabels( QPainter* p ,QStringList laberList,QFont mItemFont,double& currentXCoord,double& currentYCoord, double& maxXCoord) const
{
	int symbolTextCounts=laberList.count();
	//double itemHeight= qMax( mLegend->symbolHeight(), mLegend->fontAscentMillimeters( mItemFont ) );
	//double realItemHeight = itemHeight;
	currentYCoord+=mLegend->symbolHeight();
	if ( p&&symbolTextCounts>1 )
	{

		double textHeightMM= mLegend->textHeightMillimeters( mItemFont, laberList.at(1));
		double textWidthMM=mLegend->textWidthMillimeters( mItemFont, laberList.at(1));
		//下面默认色卡的高度要大于刻度的高度
		currentYCoord-=textHeightMM/2;
		currentYCoord-=( mLegend->symbolHeight() - mLegend->fontAscentMillimeters( mItemFont ) ) / 2;
		//色卡和色卡标志之间的距离
		currentXCoord+=mLegend->iconLabelSpace();
		currentYCoord+=mLegend->symbolSpace();
		p->setPen( QColor( 0, 0, 0 ) );
		for (int j=0;j<laberList.count();j++)
		{
			textWidthMM=qMax(textWidthMM,mLegend->textWidthMillimeters( mItemFont, laberList.at(j)));
			mLegend->drawText( p, currentXCoord, currentYCoord + mLegend->fontAscentMillimeters( mItemFont ) + ( mLegend->symbolHeight() - mLegend->fontAscentMillimeters( mItemFont ) ) / 2, laberList.at(j), mItemFont );
			currentYCoord +=mLegend->symbolHeight();
			currentYCoord+=mLegend->symbolSpace();
		}
		currentXCoord+=textWidthMM;
		currentYCoord +=mLegend->symbolHeight();
		currentYCoord+=mLegend->symbolSpace();
		//
	} 
	//currentXCoord +=mBoxSpace;	//外置
}

void CCipasGradsVerticalLegendStyle::drawColorCard(QPainter* p, const goto_gis::LegendInterface* pLegend,double rasterScaleFactor, double& currentYCoord, double& currentXPosition, double& symbolHeight,double& maxXCoord) const
{
	COLORTABLE_VECTOR ColorIndex;

	ConvertLegendInterfaceToColorTable(pLegend, &ColorIndex);

	int allcounts=ColorIndex.size();
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
		if (colorcardcounts==0 )//顶部绘制三角形
		{
			QPolygonF triangle;
			triangle << QPointF( xStartCord+0.5* (mLegend->symbolWidth())*rasterScaleFactor, yStartCord);
			triangle << QPointF( xStartCord+(mLegend->symbolWidth())*rasterScaleFactor, yStartCord +(mLegend->symbolHeight())*rasterScaleFactor);
			triangle << QPointF( xStartCord, yStartCord+ (mLegend->symbolHeight())*rasterScaleFactor );
			p->drawPolygon( triangle );
		} 
		else if (colorcardcounts==(allcounts-1))//底部绘制三角形
		{
			QPolygonF triangle;
			triangle << QPointF( xStartCord, yStartCord);
			triangle << QPointF( xStartCord+(mLegend->symbolWidth())*rasterScaleFactor, yStartCord );
			triangle << QPointF( xStartCord+0.5*(mLegend->symbolWidth())*rasterScaleFactor, yStartCord+ (mLegend->symbolHeight())*rasterScaleFactor );
			p->drawPolygon( triangle );
		}
		else
		{
			p->drawRect(xStartCord,yStartCord,((mLegend->symbolWidth())-0)*rasterScaleFactor,((mLegend->symbolHeight())-0)*rasterScaleFactor);
		}

		yStartCord+=(mLegend->symbolHeight()+mLegend->symbolSpace())*rasterScaleFactor;

		++layerIter;
		colorcardcounts++;
	}
	currentXPosition+=mLegend->symbolWidth();
	currentYCoord+=yStartCord;
}

#if 0
void CCipasGradsVerticalLegendStyle::drawSymbolCard( QPainter* p, P_SymbolLegend_Vector *symbolLegendv ,double rasterScaleFactor,double& currentYCoord, double& currentXPosition, double& symbolHeight,double& maxXCoord ) const
{
	int allcounts=(*symbolLegendv)->size();
	int colorcardcounts=0;
	double xStartCord=0;
	double yStartCord=0;
	QVector<P_SymbolLegend_Product>::iterator layerIter = (*symbolLegendv)->begin();
	while(layerIter != (*symbolLegendv)->end())
	{
		//QBrush mBrush;
		//QPen mPen;
		//mPen.setColor(mLegend->SymboloutlineColor());
		//mPen.setWidthF(mLegend->SymbolOutlineWidth());
		//mPen.setStyle(Qt::SolidLine);
		//p->setPen(mPen);
		//mBrush.setStyle(Qt::SolidPattern);
		//mBrush.setColor(QColor((*layerIter)->nRed,(*layerIter)->nGreen,(*layerIter)->nBlue));
		//p->setBrush(mBrush);
		double halfPenWidth=0.5*(mLegend->SymbolOutlineWidth());
		//if (colorcardcounts==0 )//顶部绘制三角形
		//{
		//	QPolygonF triangle;
		//	triangle << QPointF( xStartCord+0.5* (mLegend->symbolWidth())*rasterScaleFactor, yStartCord);
		//	triangle << QPointF( xStartCord+(mLegend->symbolWidth())*rasterScaleFactor, yStartCord +(mLegend->symbolHeight())*rasterScaleFactor);
		//	triangle << QPointF( xStartCord, yStartCord+ (mLegend->symbolHeight())*rasterScaleFactor );
		//	p->drawPolygon( triangle );
		//} 
		//else if (colorcardcounts==(allcounts-1))//底部绘制三角形
		//{
		//	QPolygonF triangle;
		//	triangle << QPointF( xStartCord, yStartCord);
		//	triangle << QPointF( xStartCord+(mLegend->symbolWidth())*rasterScaleFactor, yStartCord );
		//	triangle << QPointF( xStartCord+0.5*(mLegend->symbolWidth())*rasterScaleFactor, yStartCord+ (mLegend->symbolHeight())*rasterScaleFactor );
		//	p->drawPolygon( triangle );
		//}
		//else
		//{

		//int w = (*layerIter)->pixmap.width();
		//int h = (*layerIter)->pixmap.height();
		//p->drawPixmap(xStartCord,yStartCord,w,h,(*layerIter)->pixmap);

		p->drawPixmap(xStartCord,yStartCord,((mLegend->symbolWidth())-0)*rasterScaleFactor,((mLegend->symbolHeight())-0)*rasterScaleFactor,(*layerIter)->pixmap);
		//p->drawRect(xStartCord,yStartCord,((mLegend->symbolWidth())-0)*rasterScaleFactor,((mLegend->symbolHeight())-0)*rasterScaleFactor);
		//}

		yStartCord+=(mLegend->symbolHeight()+mLegend->symbolSpace())*rasterScaleFactor;

		++layerIter;
		colorcardcounts++;
	}
	currentXPosition+=mLegend->symbolWidth();
	currentYCoord+=yStartCord;
	//
}
#endif