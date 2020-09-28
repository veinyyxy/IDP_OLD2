#ifndef IARITHMS_LIB
	#define IARITHMS_LIB
#endif
#include "Arith_273_THP.hpp"
#include "Product_273_THP.hpp"
#include "../Arith_271_OHP/Product_271_OHP.hpp"
//#include "StandardRadarData.h"
#include "GDef.h"
#include "CJulMsGMT.h"
#include <fstream>

Arith_273_THP::Arith_273_THP()
{

}
Arith_273_THP::~Arith_273_THP()
{

}

GHRESULT Arith_273_THP::Initialize()
{
	m_pInputList = NULL;
	m_pOutputList = NULL;
	return GS_OK;
};
GHRESULT Arith_273_THP::UnInitialize()
{
	return GS_OK;
};

GHRESULT Arith_273_THP::LoadData(void* pValue, ReadParam * ReadParameter)
{
	if(pValue == NULL) return GE_INVALIDARG;
	if(!ReadParameter) 
		return GE_INVALIDARG;

	m_pInputList = (GXList<GenerationData *>*)pValue;
	m_fnParam = ReadParameter;

	return GS_OK;
};

GHRESULT Arith_273_THP::OutPutData(void* pValue)
{
	if(pValue == NULL) return GE_INVALIDARG;

	m_pOutputList = (GXList<GenerationData>*)pValue;

	return GS_OK;
};

extern "C" DLLEXP_GSYS IArith *Create_arith (void)//用于创建Arith_273_THP接口
{
	return new (::std::nothrow)Arith_273_THP;
}

GHRESULT Arith_273_THP::Execute()
{
	return GE_SHORTOFINDATA;
	ofstream logfile;
	logfile.open("./Log/Arith_273_THP.txt",ios::app);
	gUint16 iJulDay;
	gInt32 iMilliSecond,iYear,iMonth,iDay,iHour,iMinute,iSecond,iMSecond;
	CJulMsGMT::GetLocalJulMs(iJulDay, iMilliSecond );
	CJulMsGMT::Jul2Greg(iJulDay,iYear,iMonth,iDay);
	CJulMsGMT::MilliSecond2Time(iMilliSecond,iHour,iMinute,iSecond,iMSecond);
	logfile<<endl<<iHour<<":"<<iMinute<<":"<<iSecond<<"	"<<iYear<<"-"<<iMonth<<"-"<<iDay<<endl;
	logfile<<"273_THP：  Started: "<<endl;

	logfile<<"273_THP：  Get the m_pInputList's count and check if count=3."<<endl;
	// GData objects in m_pInputList have been sorted by date and time.
	int cnt = (int)m_pInputList->GetCount();

	if (cnt != 3)
	{
		logfile<<"273_THP：  The m_pInputList's Count != 3."<<endl;
		logfile<<"273_THP：  Return GE_SHORTOFINDATA."<<endl;
		logfile.close();

		return GE_SHORTOFINDATA;
	}

	 int w = 480;
	 int h = 480;
	PDBlock* pdb = NULL;
	gInt16* pOutData = NULL;

	logfile<<"273_THP：  Get the m_pInputList's Last Product."<<endl;
	m_pOutputList->AddTail();
	Product_271_OHP& PdtOHP = (*(m_pInputList->GetTailAs<Product_271_OHP>()));
	Product_273_THP* pPdt = &(*(m_pOutputList->GetTailAs<Product_273_THP>()));

	logfile<<"273_THP：  Create Send-out Product: Product_273_THP."<<endl;
	GHRESULT ConstrutHand;
	w= PdtOHP.GetGrid_Head()->NumberOfRows;
	h= PdtOHP.GetGrid_Head()->NumberOfCols;
	ConstrutHand = pPdt->ConstructIt(w,h);
	if(ConstrutHand != GS_OK)
	{
		logfile<<"273_THP：  Create New Product Failed and Return GE_FAILOFNEWPRODUMEM."<<endl;
		logfile.close();
		return GE_FAILOFNEWPRODUMEM;
	}
	pdb = pPdt->GetPDBlock();
	PDBlock* pdbt = PdtOHP.GetPDBlock();

	logfile<<"273_THP：  Get and Set RadarInfor and Radial_Head Information."<<endl;
	pdb->ProductCode = 273;
	//pdb->ElevationNumber = -1;
	memcpy(pPdt->GetRadarInfor()->RadarType, PdtOHP.GetRadarInfor()->RadarType, sizeof(gInt8) * 4);
	pPdt->GetRadarInfor()->Altitude = PdtOHP.GetRadarInfor()->Altitude;
	pPdt->GetRadarInfor()->Latitude = PdtOHP.GetRadarInfor()->Latitude;
	pPdt->GetRadarInfor()->Longitude = PdtOHP.GetRadarInfor()->Longitude;
	memcpy(pdb->VCPMode, pdbt->VCPMode, sizeof(gInt8) *4);
	memcpy(pdb->OperationalMode, pdbt->OperationalMode, sizeof(gInt8) *2);
	pdb->ProductDate = pdbt->ProductDate;
	pdb->ProductTime = pdbt->ProductTime;

	CJulMsGMT::Jul2Greg(pdb->ProductDate,iYear,iMonth,iDay);
	CJulMsGMT::MilliSecond2Time(pdb->ProductTime,iHour,iMinute,iSecond,iMSecond);
	logfile<<"273_THP：  VCPDate："<<iYear<<"-"<<iMonth<<"-"<<iDay<<"		VCPTime："<<iHour<<":"<<iMinute<<":"<<iSecond<<endl;

	pdb->ScaleOfData = 100; 
	pdb->IndexOfTabular = -1;
	pdb->IndexOfGraphic = -1;
	CJulMsGMT::GetLocalJulMs(pdb->GenerationDateOfProduct,pdb->GenerationTimeOfProduct);
	memcpy(pPdt->GetRadarInfor()->SiteCode, PdtOHP.GetRadarInfor()->SiteCode, sizeof(gInt8)*8);
	//memcpy(pdb->SiteName, pdbt->SiteName, sizeof(gInt8)*18);

	DILayer* dil;

	dil = pPdt->GetLayer();
	dil->HeightOfElevation = 0;
	GridHead*	ghd;
	ghd = pPdt->GetGrid_Head();

	//ghd->PacketCode = 0x0B00;
	/*ghd->NumberOfRows = h;
	ghd->NumberOfCols = w;*/
	ghd->ResolutionOfRow = (PdtOHP.GetGrid_Head())->ResolutionOfRow;
	ghd->ResolutionOfCol = (PdtOHP.GetGrid_Head())->ResolutionOfCol;
	//ghd->ScaleOfData = 1;

	/*pOutData = pPdt->GetGrid_Data(0);
	gInt16* pOutWritePtr = pOutData;
	memset(pOutWritePtr, 0, sizeof(gInt16)*w*h);
	int total = w * h;
	for (int i = 0; i< cnt; i++)
	{	
	CommonProduct* pcb = &(*(m_pInputList->GetAtAs<CommonProduct>(i)));
	gInt16* pData = pcb->GetGridDataP();
	pOutWritePtr = pOutData;
	for( int m = 0; m < total; ++m)
	{
	*pOutWritePtr = *pOutWritePtr + (gInt16)((*pData)/PdtOHP.GetPDBlock()->ScaleOfData);
	pOutWritePtr ++;
	pData ++;
	}
	}*/

	//////////////////////////////////////////////////////////////////////////////////  modified by sunqc 20090304
	Product_271_OHP* pOHP;

	gInt16* pData = NULL;

	logfile<<"273_THP：  Loop: m=0-->h("<<h<<") to Initial Send-out Product Data as 0."<<endl;
	for( int m = 0; m < h ; ++m)
	{
		pOutData = pPdt->GetGrid_Data(m);
		memset(pOutData,0,sizeof(gInt16)*w);
	}
	logfile<<"273_THP：  Loop Finished."<<endl;

	logfile<<"273_THP：  Loop: k=0-->cnt("<<cnt<<") to Calculate Send-out Product Data."<<endl;
	for(int k = 0; k < cnt; ++k)
	{
		pOHP = &(*m_pInputList->GetAtAs<Product_271_OHP>(k));
		for( int m = 0; m < h ; ++m)
		{
			pData = pOHP->GetGrid_Data(m);
			pOutData = pPdt->GetGrid_Data(m);
			for(int n = 0; n < w; ++n)
			{
				if(pData[n]> 0)
					pOutData[n] += pData[n];
			}
		}			
	}
	logfile<<"273_THP：  Loop Finished."<<endl;
	logfile<<"273_THP：  Finished."<<endl;
	logfile.close();

	return GS_OK;
};
