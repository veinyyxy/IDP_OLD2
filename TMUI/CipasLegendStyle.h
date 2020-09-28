#pragma once
#include <QtCore/QVector>
#include <QtCore/QPair>
#include "tmui_global.h"
#include "CipasComposerLegend.h"

//class CCipasComposerLegend;
class QPainter;

class  TMUI_EXPORT CCipasLegendStyle
{
public:
	typedef struct ColorNode_Product
	{
		double fStart;
		double fEnd;
		int   nRed;
		int   nGreen;
		int   nBlue;
	}COLORNODE_PRODUCT, *P_COLORNODE_PRODUCT;

	typedef QVector<COLORNODE_PRODUCT>  COLORTABLE_VECTOR, *P_COLORTABLE_VECTOR;

	CCipasLegendStyle(const CCipasComposerLegend* legend);
	virtual ~CCipasLegendStyle();

	virtual void drawLabels( QPainter* p ,QStringList laberList,QFont mItemFont,double& currentXCoord,double& currentYCoord, double& maxXCoord) const = 0; //to do by every subclass
	//virtual void drawColorCard( QPainter* p, const QMap<double,QColor>* pColorIndex, double rasterScaleFactor, double& currentYCoord, double& currentXPosition, double& symbolHeight,double& maxXCoord) const = 0;
	//virtual void drawColorCard( QPainter* p, const P_COLORTABLE_VECTOR* pColorIndex, double rasterScaleFactor, double& currentYCoord, double& currentXPosition, double& symbolHeight,double& maxXCoord) const = 0;
	//virtual void drawSymbolCard(QPainter* p,P_SymbolLegend_Vector *symbolLegendv ,double rasterScaleFactor,double& currentYCoord, double& currentXPosition, double& symbolHeight,double& maxXCoord) const = 0;
	virtual void drawColorCard( QPainter* p, const goto_gis::LegendInterface* pColorIndex, double rasterScaleFactor, double& currentYCoord, double& currentXPosition, double& symbolHeight,double& maxXCoord) const = 0;
	//稍后增加别的参数
	virtual QString name() const = 0; //return name of the style

	virtual void drawLegend(QPainter* p, 
		const goto_gis::LegendInterface* pColorIndex, 
		double rasterScaleFactor, 
		double& currentXCoord, 
		double& currentYCoord, 
		double& symbolHeight,
		QStringList laberList,
		QFont mItemFont,
		double& maxXCoord)const = 0;

	void ConvertLegendInterfaceToColorTable(const goto_gis::LegendInterface* pColorIndex, P_COLORTABLE_VECTOR oldColorTable) const;
	void GetValueListFromLegendInterface(const goto_gis::LegendInterface* pColorIndex, QVector< QPair<double, double> >& valueList);

  protected:
	const CCipasComposerLegend* mLegend;
};

