#pragma once

#include <string>
#include<osg\Node>

class DataInterface
{
public:
	virtual void DataInput(
		size_t iLonNCCount, 
		size_t iLatNCCount,  
		size_t iTimeNCCount, 
		size_t iLevelNCCount, 
		float fLonBegin, 
		float fLonEnd,
		float fLonDelta,
		float fLatBegin, 
		float fLatEnd,
		float fLatDelta,
		float fMissingValue,
		float fLevelBegin,
		float fLevelEnd,
		float fLevelDelta, 
		std::vector<float>& fLevelVector, 
		std::string& strTimeBegin,
		std::string& strTimeEnd,
		int iTimeDelta,
		double* dData, 
		int nModel, 
		int useNineP) = 0;
	virtual void DataOutput() = 0;
	virtual virtual void DataInput(const std::string& ctlFile, const std::string& dataFile) = 0;
	virtual void Draw(int iType, int iDirect, const std::string& strCmd = "display Va") = 0;
};

