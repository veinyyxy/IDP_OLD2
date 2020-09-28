#pragma once
#include <string>
#include <boost/Any.hpp>
#include <map>

class FrameModel;

class Controller
{
public:
	virtual int PreExcute() = 0;
	virtual int Excute() = 0;
	virtual int PostExcute() = 0;
	virtual void GetName(std::string& strID) = 0;
	virtual void Input(const FrameModel* pFM) = 0;
	virtual FrameModel* Output() = 0;
};

