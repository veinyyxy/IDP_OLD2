#pragma once
#include <string>
#include <boost/any.hpp>

class FrameModel
{
public:
	virtual boost::any* GetInterface(const std::string& name) = 0;
	virtual void SetInterface(const std::string& name, const boost::any* pI) = 0;

	virtual boost::any* GetParameter(const std::string& name) = 0;
	virtual void SetParameter(const std::string& name, const boost::any* pI) = 0;
};

