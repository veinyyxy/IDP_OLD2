#pragma once
#include "datainterface.h"
#include <QtCore/QList>

class CElement;
class GradsProvider;

class GradsDataInterfaceImp :
	public DataInterface
{
public:
	GradsDataInterfaceImp(GradsProvider* pGrads);
	virtual ~GradsDataInterfaceImp(void);

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
		int useNineP);
	virtual void DataInput(const std::string& ctlFile, const std::string& dataFile);
	virtual void DataOutput();

	virtual void Draw(int iType, int iDirect, const std::string& strCmd = "display Va");
private:
	GradsProvider* m_pGradsProvider;
};

