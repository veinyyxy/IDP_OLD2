#pragma once
#include "GData.h"
#include "cDrawProduct.h"
class ShowSystemApp 
{
public:
	ShowSystemApp(void);
	~ShowSystemApp(void);

	void GetProdctAttr(proAttr& m_attr, int proNum, GenerationData *pGata , int requestLayer,bool showflag);
};

