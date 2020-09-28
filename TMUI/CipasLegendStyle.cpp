#include <QPainter>
#include "CipasComposerLegend.h"
#include "CipasLegendStyle.h"

CCipasLegendStyle::CCipasLegendStyle(const CCipasComposerLegend* legend):mLegend(legend)
{

}



CCipasLegendStyle::~CCipasLegendStyle()
{
}

void CCipasLegendStyle::ConvertLegendInterfaceToColorTable(const goto_gis::LegendInterface* pLegend, P_COLORTABLE_VECTOR oldColorTable) const
{
	///////////////////////////////////Ìî³äÑÕÉ«±í///////////////////////////////
	COLORNODE_PRODUCT oldColorTableItem;

	//COLORTABLE_VECTOR ColorIndex;
	std::pair<double, double> pairValue;
	osg::Vec4 v4Color;
	goto_gis::LegendInterface::VALUE_SCOPE valueList;

	goto_gis::LegendInterface* pLocalLegend = const_cast<goto_gis::LegendInterface*>(pLegend);
	if(pLocalLegend)
	{
		if(pLocalLegend->GetValueScopeList(&valueList) == -1)
		{
			return;
		}
		goto_gis::LegendInterface::VALUE_SCOPE::iterator vsbe = valueList.begin();
		for(; valueList.end() != vsbe; vsbe++)
		{
			pairValue = *vsbe;
			double dValue = (pairValue.first + pairValue.second) / 2.0;
			if(pLocalLegend->GetLegendColor(dValue, &v4Color) == -1)
				return;

			oldColorTableItem.fStart = pairValue.first;
			oldColorTableItem.fEnd = pairValue.second;
			oldColorTableItem.nRed = v4Color.r() * 255.0;
			oldColorTableItem.nGreen = v4Color.g() * 255.0;
			oldColorTableItem.nBlue = v4Color.b() * 255.0;

			oldColorTable->push_back(oldColorTableItem);
		}
	}
	//////////////////////////////////////////////////////////////////////////
}

void CCipasLegendStyle::GetValueListFromLegendInterface( const goto_gis::LegendInterface* pLegend, QVector< QPair<double, double> >& valueList )
{
	if(pLegend)
	{
		std::pair<double, double> pairValue;
		osg::Vec4 v4Color;
		goto_gis::LegendInterface::VALUE_SCOPE myvalueList;

		goto_gis::LegendInterface* pLocalLegend = const_cast<goto_gis::LegendInterface*>(pLegend);
		if(pLocalLegend)
		{
			if(pLocalLegend->GetValueScopeList(&myvalueList) == -1)
			{
				return;
			}
			goto_gis::LegendInterface::VALUE_SCOPE::iterator vsbe = myvalueList.begin();
			for(; myvalueList.end() != vsbe; vsbe++)
			{
				pairValue = *vsbe;
				double dS = pairValue.first;
				double dE = pairValue.second;
				valueList.push_back(QPair<double, double>(dS, dE));
			}
		}
	}
}
