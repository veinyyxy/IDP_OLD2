#pragma once
#include "MeteorologyProducts.h"

class GeneralShadedLineBound  : public MeteorologyProducts
{
public:
	GeneralShadedLineBound(void);
	virtual ~GeneralShadedLineBound(void);

	virtual int Dump( goto_gis::Layer* pLayer );

	virtual int Draw();
private:
	void InitCommand();
	char m_Command[65535];			// Ω≈±æ√¸¡Ó
	char m_Temp[65535];
};

