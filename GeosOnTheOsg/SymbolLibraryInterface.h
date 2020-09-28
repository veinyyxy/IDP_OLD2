#pragma once
#include <string>
#include <osg/Vec4>
#include <osg/Geometry>
#include "goConfig.h"
#include "GeosOnTheOsg_Global.h"
#include "LegendInterface.h"
GOTO_NAMESPACE_START


class GEOSONTHEOSG_EXPORT SymbolLibraryInterface
{
public:
	inline LegendInterface* GetLegend(int i = 0){return m_pLegendVector.at(i);}
	inline void AddLegend(LegendInterface* pLF){m_pLegendVector.push_back(pLF);}
	virtual int InitSymbolLibrary(std::string strLibName) = 0;

protected:
	
	std::vector<LegendInterface*> m_pLegendVector;
};

GOTO_NAMESPACE_END