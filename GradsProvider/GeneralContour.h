#pragma once

#include "meteorologyproducts.h"

#include "gxX.h"
#include "gauser.h"
#include <QFile>
#include <QList>
#include <QCoreApplication>

class GeneralContour : public MeteorologyProducts
{
public:
	GeneralContour(void);
	~GeneralContour(void);
	int Dump(goto_gis::Layer* pLayer, int iDirect = 0);
	int Draw(const std::string& strCmd = "display vA");
protected:
	void InitCommand();
	void ScaleDataLine();
	void ScaleDataText();

	char m_Command[512];			//Ω≈±æ√¸¡Ó
	char m_Temp[1024];
};