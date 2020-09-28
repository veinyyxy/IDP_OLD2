#pragma once

#include "QT_Include.h"
#include "../CDataElement/CElement.h"

class NinePointSmoother
{
public:
	NinePointSmoother(void);
	~NinePointSmoother(void);

	void Execute(CElement *pElem, double **dArrayValue, double dMissingValue);
	bool isUse;
};