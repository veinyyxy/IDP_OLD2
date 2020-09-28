#include "CipasGISLegendStyle.h"

CCipasGISLegendStyle::CCipasGISLegendStyle(const CCipasComposerLegend* legend):CCipasLegendStyle(legend)
{
}


CCipasGISLegendStyle::~CCipasGISLegendStyle(void)
{
}

QString CCipasGISLegendStyle::name()const
{
	return QObject::tr("GIS风格");
	//return "GradsStyle(Horizontal)";
}


void CCipasGISLegendStyle::drawLabels( QPainter* p ,QStringList laberList,QFont mItemFont,double& currentXCoord,double& currentYCoord, double& maxXCoord) const
{
	int symbolTextCounts=laberList.count();

	if ( p &&symbolTextCounts>0)
	{

		double textHeightMM= mLegend->textHeightMillimeters( mItemFont, laberList.first());
		double textWidthMM=mLegend->textWidthMillimeters( mItemFont, laberList.first());
		currentYCoord+=mLegend->symbolHeight()/2;
		//下面默认色卡的高度要大于刻度的高度
		currentYCoord+=textHeightMM/2;
		//色卡和色卡标志之间的距离
		currentXCoord+=mLegend->iconLabelSpace();
		//currentYCoord+=mLegend->symbolSpace();

		p->setPen( QColor( 0, 0, 0 ) );
		//左闭右开，升序，是否放开参数
		for (int j=0;j<laberList.count();j++)
		{

			mLegend->drawText( p, currentXCoord, currentYCoord  ,laberList.at(j), mItemFont );

			textWidthMM=qMax(textWidthMM,mLegend->textWidthMillimeters( mItemFont, laberList.at(j)));
			currentYCoord +=mLegend->symbolHeight();
			currentYCoord+=mLegend->symbolSpace();
		}
		//mLegend->drawText( p, currentXCoord, currentYCoord  ,tr(" > ")+ laberList.last(), mItemFont );
		currentXCoord +=textWidthMM;
	} 
		

}

void CCipasGISLegendStyle::drawColorCard(QPainter* p, const goto_gis::LegendInterface* pLegend, double rasterScaleFactor,double& currentYCoord, double& currentXPosition, double& symbolHeight,double& maxXCoord) const
{
	COLORTABLE_VECTOR ColorIndex;

	CCipasLegendStyle::ConvertLegendInterfaceToColorTable(pLegend, &ColorIndex);

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
		mBrush.setColor(QColor(layerIter->nRed,(layerIter)->nGreen,(layerIter)->nBlue));
		p->setBrush(mBrush);
		double halfPenWidth=0.5*(mLegend->SymbolOutlineWidth());
		p->drawRect(xStartCord,yStartCord,(mLegend->symbolWidth())*rasterScaleFactor,(mLegend->symbolHeight())*rasterScaleFactor);

		//xStartCord+=(mLegend->symbolWidth()+mLegend->symbolSpace())*rasterScaleFactor;
		yStartCord+=(mLegend->symbolHeight()+mLegend->symbolSpace())*rasterScaleFactor;


		++layerIter;
		colorcardcounts++;
	}
	currentXPosition+=mLegend->symbolWidth();
	currentYCoord+=yStartCord;
	//currentXPosition += (mLegend->symbolWidth()+mLegend->symbolSpace())*colorcardcounts;
	//currentYCoord+= (mLegend->symbolHeight()+mLegend->symbolSpace())*colorcardcounts;
}
#if 0
void CCipasGISLegendStyle::drawSymbolCard(QPainter* p, P_SymbolLegend_Vector *symbolLegendv, double rasterScaleFactor,double& currentYCoord, double& currentXPosition, double& symbolHeight,double& maxXCoord) const
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
		
		//p->drawRect(xStartCord,yStartCord,(mLegend->symbolWidth())*rasterScaleFactor,(mLegend->symbolHeight())*rasterScaleFactor);
		//int w = (*layerIter)->pixmap.width();
		//int h = (*layerIter)->pixmap.height();
		//p->drawPixmap(xStartCord,yStartCord,w,h,(*layerIter)->pixmap);
		p->drawPixmap(xStartCord,yStartCord,(mLegend->symbolWidth())*rasterScaleFactor,(mLegend->symbolHeight())*rasterScaleFactor,(*layerIter)->pixmap);

		//xStartCord+=(mLegend->symbolWidth()+mLegend->symbolSpace())*rasterScaleFactor;
		yStartCord+=(mLegend->symbolHeight()+mLegend->symbolSpace())*rasterScaleFactor;


		++layerIter;
		colorcardcounts++;
	}
	currentXPosition+=mLegend->symbolWidth();
	currentYCoord+=yStartCord;
}
#endif
void CCipasGISLegendStyle::drawLegend(QPainter* p, const goto_gis::LegendInterface* pColorIndex, double rasterScaleFactor, double& currentXCoord, double& currentYCoord, double& symbolHeight, QStringList laberList, QFont mItemFont, double& maxXCoord) const
{
	drawColorCard(p,pColorIndex,rasterScaleFactor,currentYCoord,currentXCoord,symbolHeight,maxXCoord);
	drawLabels(p,laberList,mItemFont,currentXCoord,currentYCoord,maxXCoord);
}
