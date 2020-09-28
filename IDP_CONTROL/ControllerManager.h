#pragma once
#include "Controller.h"
#include <vector>

class ControllerManager
{
public:
	virtual Controller* GetControllerFromName(const std::string& strName) = 0;
	virtual void GetControllerVector(std::vector<Controller*>*) = 0;
	virtual void GetControllerNameVector(std::vector<std::string>*) = 0;
};
