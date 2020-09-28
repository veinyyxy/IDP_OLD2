#pragma once
#include "QT_Include.h"
#include "Layer.h"
#include "GradsConfig.h"

using namespace std;

class GRADS_EXPORT_IMPORT MeteorologyProducts : public QObject
{
public:
	virtual int Dump(goto_gis::Layer* pLayer, int iDirect = 0) = 0;
	virtual int Draw(const std::string& strCmd = "display vA") = 0;
};