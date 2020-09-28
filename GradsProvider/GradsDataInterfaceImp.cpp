#include "GradsDataInterfaceImp.h"
#include "GradsProvider.h"
#include "GeneralContour.h"
#include "GeneralShaded.h"

GradsDataInterfaceImp::GradsDataInterfaceImp(GradsProvider* pGrads) : m_pGradsProvider(pGrads)
{

}

GradsDataInterfaceImp::~GradsDataInterfaceImp(void)
{

}

void GradsDataInterfaceImp::DataInput(
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
	int useNineP)
{
	m_pGradsProvider->CElement2GradsData(
		iLonNCCount, 
		iLatNCCount,  
		iTimeNCCount, 
		iLevelNCCount, 
		fLonBegin, 
		fLonEnd,
		fLonDelta,
		fLatBegin, 
		fLatEnd,
		fLatDelta,
		fMissingValue,
		fLevelBegin,
		fLevelEnd,
		fLevelDelta, 
		fLevelVector,
		strTimeBegin,
		strTimeEnd,
		iTimeDelta,
		dData, 
		nModel, 
		useNineP
		);
}

void GradsDataInterfaceImp::DataInput( const std::string& ctlFile, const std::string& dataFile )
{
	m_pGradsProvider->SetCtlFile(ctlFile, dataFile);
}

void GradsDataInterfaceImp::DataOutput()
{
	
}

void GradsDataInterfaceImp::Draw( int iType, int iDirect, const std::string& strCmd )
{
	MeteorologyProducts* pProduct = 0;
	switch(iType)
	{
	case 0:
		pProduct = new GeneralContour;
		break;
	case 1:
		pProduct = new GeneralShaded;
		break;
	default:
		break;
	}
	if(pProduct)
	{
		m_pGradsProvider->MakeProduct(pProduct, iDirect, strCmd);
	}
}
