#pragma once
#include <QtGui>
#include "cipaslegendstyle.h"
class CCipasGradsHorizontalLegendStyle :public CCipasLegendStyle
{
public:
	CCipasGradsHorizontalLegendStyle(const CCipasComposerLegend* legend);
	~CCipasGradsHorizontalLegendStyle(void);
	QString name() const;
	void drawLabels( QPainter* p ,QStringList laberList,QFont mItemFont,double& currentXCoord,double& currentYCoord, double& maxXCoord) const;
	//void drawColorCard( QPainter* p, const QMap<double,QColor>* pColorIndex,double rasterScaleFactor, double& currentYCoord, double& currentXPosition, double& symbolHeight,double& maxXCoord) const;
	void drawColorCard( QPainter* p, const goto_gis::LegendInterface* pColorIndex,double rasterScaleFactor, double& currentYCoord, double& currentXPosition, double& symbolHeight,double& maxXCoord) const;
	//void drawSymbolCard(QPainter* p, P_SymbolLegend_Vector *symbolLegendv ,double rasterScaleFactor,double& currentYCoord, double& currentXPosition, double& symbolHeight,double& maxXCoord) const;
	void drawLegend(QPainter* p, 
		const goto_gis::LegendInterface* pColorIndex, 
		double rasterScaleFactor, 
		double& currentXCoord, 
		double& currentYCoord, 
		double& symbolHeight,
		QStringList laberList,
		QFont mItemFont,
		double& maxXCoord) const ;
};

