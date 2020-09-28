//////////////////////////////////////////////////////////////////////////
//made modify by yangys 080610
//////////////////////////////////////////////////////////////////////////
#ifndef IARITHMS_LIB
	#define IARITHMS_LIB
#endif
#include "Arith_277_SCI.h"
#include "CJulMsGMT.h"
#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif
#include <math.h>
#include <fstream>
//#define M_PI       3.14159265358979323846

Arith_277_SCI::Arith_277_SCI(void)
{}
Arith_277_SCI::~Arith_277_SCI(void)
{}
GHRESULT Arith_277_SCI::Initialize()
{
	m_pGenerateProductList = NULL;
	m_pOutputProductList = NULL;

	THRESHOLD_REF_VALUES = NULL;
	CSpos = NULL;
	NumofSeg = NULL;
	//MEMOFSEG = NULL;
	NumofCpn = NULL;
	MEMOFCPN = NULL;

	THRESHOLD_REF_NUMBER = 0 ;
	Threshold_Dropout_Count = 0;
	Threshold_Dropout_DifRef = 0;
	Threshold_Segment_Length = 0.0;
	Threshold_AZ_SEP = 0;
	ThresholdOfSegmentNumber = 0;
	Threshold_OVERLAP = 0.0;
	Threshold_AreaCC = 0.0;
	NumofCell = 0;
	daz = 0.0;
	SVL_km = 0.0;
	gFloat Threshold_DIS_Delete =0.0;
	gInt16 Threshold_DDE_Delete = 0;
	gFloat Threshold_DIS_Merge = 0.0;
	gFloat Threshold_HEI_Merge = 0.0;
	gInt16 Threshold_ELE_Merge = 0;
	gInt32 search_Radii_Num = 0;
	Temp_Length =0;
	return GS_OK;
}
GHRESULT Arith_277_SCI::UnInitialize()
{
	if(THRESHOLD_REF_VALUES!=NULL){delete []THRESHOLD_REF_VALUES;	THRESHOLD_REF_VALUES=NULL;}
	if(CSpos!=NULL){delete []CSpos;	CSpos=NULL;}
	if(NumofSeg!=NULL){delete []NumofSeg;	NumofSeg=NULL;}
	//if(MEMOFSEG!=NULL){delete []MEMOFSEG;	MEMOFSEG=NULL;}
	if(NumofCpn!=NULL){delete []NumofCpn;	NumofCpn=NULL;}
	if(MEMOFCPN!=NULL){delete []MEMOFCPN;	MEMOFCPN=NULL;}


	return GS_OK;
}
GHRESULT Arith_277_SCI::LoadData(void *pValue,ReadParam * ReadParameter)
{
	if ( ReadParameter == NULL )
	{
		return GE_INVALIDARG;
	}
	ReadP=ReadParameter;
	if ( pValue == NULL )
	{
		return GE_INVALIDARG;
	}

	m_pGenerateProductList = (GXList< GenerationData *> *)pValue;//取得输入数据列表
	//m_pParameter = ReadParameter;	//取得参数函数指针
	return GS_OK;
}
GHRESULT Arith_277_SCI::Execute()
{
	ofstream logfile;
	logfile.open("./Log/Arith_277_SCI.txt",ios::app);
	gUint16 iJulDay;
	gInt32 iMilliSecond,iYear,iMonth,iDay,iHour,iMinute,iSecond,iMSecond;
	CJulMsGMT::GetLocalJulMs(iJulDay, iMilliSecond );
	CJulMsGMT::Jul2Greg(iJulDay,iYear,iMonth,iDay);
	CJulMsGMT::MilliSecond2Time(iMilliSecond,iHour,iMinute,iSecond,iMSecond);
	logfile<<endl<<iHour<<":"<<iMinute<<":"<<iSecond<<"	"<<iYear<<"-"<<iMonth<<"-"<<iDay<<endl;
	logfile<<"277_SCI：  Started: "<<endl;

	StandardRadarData* pRadar = &(*(m_pGenerateProductList->GetHeadAs<StandardRadarData >()));
	m_pOutputProductList->AddTail();
	tagXML_Index index;//2013
	index.Threshold_Dropout_Count = 2;//淘汰点数阈值
	index.Threshold_Dropout_DifRef = 5;   //5dBZ 淘汰反射率差阈值
	index.Threshold_Segment_Length = (gFloat)1.9;  //Km;风暴段长度阈值
	index.THRESHOLD_REF_NUMBER=15; //default

	logfile<<"277_SCI：  Allocate new dynamic memory space for members in the arithmetic. "<<endl;

	THRESHOLD_REF_VALUES =  new(::std::nothrow) gInt32[MAX_THRESHOLD_REF];

	CSpos =  new(::std::nothrow) gInt16[MAX_THRESHOLD_REF+1];
	NumofSeg =  new(::std::nothrow) gInt16[MAX_TILTS_NUM];
	//MEMOFSEG =  new(::std::nothrow) gInt16[MAX_TILTS_NUM];//maximum=3072
	NumofCpn =  new(::std::nothrow) gInt32[MAX_TILTS_NUM];
	MEMOFCPN =  new(::std::nothrow) gInt16[MAX_TILTS_NUM];

	if(THRESHOLD_REF_VALUES == NULL || CSpos == NULL||NumofSeg == NULL||
		/*	MEMOFSEG == NULL||*/NumofCpn == NULL||
		MEMOFCPN == NULL/*||CCnum == NULL||
						CCpos == NULL*/)
	{
		logfile<<"277_SCI：  Allocate failed and return GE_FAILOFNEWTEMPMEM. "<<endl;
		logfile.close();
		return GE_FAILOFNEWTEMPMEM;
	}

	logfile<<"277_SCI：  Initial the parameters. "<<endl;

	index.THRESHOLD_REF_VALUES[0] = 60;
	index.THRESHOLD_REF_VALUES[1] = 55;
	index.THRESHOLD_REF_VALUES[2] = 50;
	index.THRESHOLD_REF_VALUES[3] = 45;
	index.THRESHOLD_REF_VALUES[4] = 40;
	index.THRESHOLD_REF_VALUES[5] = 35;
	index.THRESHOLD_REF_VALUES[6] = 30;
	index.THRESHOLD_REF_VALUES[7] = 28;
	index.THRESHOLD_REF_VALUES[8] = 26;
	index.THRESHOLD_REF_VALUES[9] = 24;
	index.THRESHOLD_REF_VALUES[10] = 22;
	index.THRESHOLD_REF_VALUES[11] = 20;
	index.THRESHOLD_REF_VALUES[12] = 18;
	index.THRESHOLD_REF_VALUES[13] = 16;
	index.THRESHOLD_REF_VALUES[14] = 14;

	for (int k = index.THRESHOLD_REF_NUMBER; k < MAX_THRESHOLD_REF; k++)
	{
		index.THRESHOLD_REF_VALUES[k] = 32768;
	}
	index.Threshold_Radii[0] = 5.0;
	index.Threshold_Radii[1] = 7.5;
	index.Threshold_Radii[2] = 10;
	index.search_Radii_Num = 3;
	index.Threshold_AZ_SEP = 15; //(单位：0.1度)
	index.Threshold_OVERLAP = (gFloat)1.95;  //(单位：KM)
	index.ThresholdOfSegmentNumber = 2;  //
	index.Threshold_AreaCC = 10; // (单位：Km*km）
	index.Threshold_DIS_Delete = 5;//(单位：KM)
	index.Threshold_DDE_Delete = 4*100;//0.01KM
	index.Threshold_DIS_Merge = (gFloat)10;//(单位：KM)
	index.Threshold_HEI_Merge = (gFloat)4;//KM
	index.Threshold_ELE_Merge = 300;//0.01DEGRE
	//////////////////////////////////////////////////////////////////////////参数传递
	logfile<<"277_SCI：  Send in and read the parameters. "<<endl;
	//////////////////////////////////////////////////////////////////////////
	logfile<<"277_SCI：  Add and Create Send-out Product: Product_277_SCI."<<endl;
	Product_277_SCI* Product = &(*(m_pOutputProductList->GetTailAs<Product_277_SCI>()));
	////////////////////////////////////////////////////////////////////////////////////////////
	logfile<<"277_SCI：  Call Function: StormCell_ID(pRadar,index,Product)."<<endl;
	GHRESULT ret = StormCell_ID(pRadar,index,Product);
	logfile<<"277_SCI：  Function StormCell_ID() Finished."<<endl;

	logfile<<"277_SCI：  Release the Dynamic Memory."<<endl;
	if(THRESHOLD_REF_VALUES != NULL)
	{
		delete [] THRESHOLD_REF_VALUES;
		THRESHOLD_REF_VALUES = NULL;
	}
	if(CSpos != NULL)
	{
		delete [] CSpos;
		CSpos = NULL;
	}
	if(NumofSeg != NULL)
	{
		delete [] NumofSeg;
		NumofSeg = NULL;
	}
	/*if(MEMOFSEG != NULL)
	{
	delete [] MEMOFSEG;
	MEMOFSEG = NULL;
	}*/
	if(	NumofCpn != NULL)
	{
		delete [] NumofCpn;
		NumofCpn = NULL;
	}
	if(MEMOFCPN != NULL)
	{
		delete [] MEMOFCPN;
		MEMOFCPN = NULL;
	}
	if(ret!=GS_OK)
	{
		logfile<<"277_SCI：  Function StormCell_ID() Return Value != GS_OK."<<endl;
		logfile<<"277_SCI：  Finished and Return GE_FAIL."<<endl;
		logfile.close();
		m_pOutputProductList->clear();
		return GE_FAIL;
	}

	logfile<<"277_SCI：  Finished."<<endl;
	logfile.close();

	return GS_OK;
}
GHRESULT Arith_277_SCI::OutPutData(void * pValue)
{
	if ( pValue == NULL )
	{
		return GE_INVALIDARG;
	}
	m_pOutputProductList = (GXList< GenerationData > *)pValue;
	return GS_OK;
}
//////////////////////////////////////////////////////////////////////////主函数
GHRESULT Arith_277_SCI::StormCell_ID(StandardRadarData *pGData, tagXML_Index index,Product_277_SCI *Product)
{
	GRADARDATAHEADER* pRHead = pGData->GetHeadP();
	gInt32 NumOfTilt = pRHead->ElevationCount;
	//////////////////////////////////////////////////////////////////////////
	Threshold_Dropout_Count = index.Threshold_Dropout_Count;
	Threshold_Dropout_DifRef = index.Threshold_Dropout_DifRef;
	Threshold_Segment_Length = index.Threshold_Segment_Length;
	THRESHOLD_REF_NUMBER = index.THRESHOLD_REF_NUMBER;
	for (int k = 0; k < MAX_THRESHOLD_REF; k++)
	{
		THRESHOLD_REF_VALUES[k] = index.THRESHOLD_REF_VALUES[k];
	}
	for (int k = 0; k < 3; k++)
	{
		Threshold_Radii[k] = index.Threshold_Radii[k];
	}
	search_Radii_Num = index.search_Radii_Num;
	Threshold_AZ_SEP = index.Threshold_AZ_SEP;
	Threshold_OVERLAP = index.Threshold_OVERLAP;
	ThresholdOfSegmentNumber = index.ThresholdOfSegmentNumber;
	Threshold_AreaCC = index.Threshold_AreaCC;
	Threshold_DIS_Delete = index.Threshold_DIS_Delete;
	Threshold_DDE_Delete = index.Threshold_DDE_Delete;
	Threshold_DIS_Merge = index.Threshold_DIS_Merge;
	Threshold_HEI_Merge = index.Threshold_HEI_Merge;
	Threshold_ELE_Merge = index.Threshold_ELE_Merge;

	//////////////////////////////////////////////////////////////////////////
	if (!Init_ref_threshold())
	{
		return GE_FAIL;
	}
	vector<vector<STORM_CELLCOMPONENT> >  m_VolCCG;

	SVL_km = gFloat(pGData->GetHeadP()->ReflectivityGateSize/1000);
	gInt16 maxAziNum = pGData->GetElevationHeadP(0)->AzimuthNumber;
	for (gInt16 i = 1; i< pGData->GetHeadP()->ElevationCount; i++)
	{
		if (maxAziNum < pGData->GetElevationHeadP(i)->AzimuthNumber)
		{
			maxAziNum = pGData->GetElevationHeadP(i)->AzimuthNumber;
		}
	}
	daz = (gFloat)( 2.0* M_PI /maxAziNum);
	for (int i = 0; i < NumOfTilt; i++)
	{
		NumofSeg[i] = NumofCpn[i] = 0;
	}
	for (int k = 0; k < NumOfTilt; k++)
	{
		vector<STORM_CELLSEGMENT> m_VolCS;
		vector<STORM_CELLCOMPONENT> m_VolCC;

		NumofSeg[k] = CellSegments(pGData,k,m_VolCS);
		NumofCpn[k] = Cell2DComponents(pGData,k,m_VolCS,m_VolCC);
		/*m_VolCSG.push_back(m_VolCS);*/
		m_VolCCG.push_back(m_VolCC);
	}

	/*for (int k = 0; k < NumOfTilt; k++)
	{
	delete [] m_VolCS[k];
	}
	delete []m_VolCS;
	m_VolCS = NULL;*/
	//----------------------------------------------attributes for 3D Cells----//
	/*NumofCell = 0;*/
	//STORM_CELLATTRIBUTES *m_Cell =  new(::std::nothrow) STORM_CELLATTRIBUTES[MAX_CELLS];
	//memset(m_Cell,0,sizeof(STORM_CELLATTRIBUTES)*MAX_CELLS);
	vector<STORM_CELLATTRIBUTES> m_Cell;
	StormID_correlated(pGData,m_VolCCG,m_Cell);
	m_VolCCG.clear();
	gInt32 NumOfStorm = StormID_merger(pGData,/*m_VolCCG,*/m_Cell);

	if( NumOfStorm == 0)
		return GE_FAIL;
	GHRESULT ConstrutHand = NULL ;
	ConstrutHand = Product->ConstructIt(1,sizeof(TAGSSHEAD),NumOfStorm+1,sizeof(STORM_CELLATTRIBUTES));
	if(ConstrutHand != GS_OK)
		return (ConstrutHand ==( (ConstrutHand == GE_NOENOUGHMEMORY)?GE_NOENOUGHMEMORY:GE_INVALIDARG));
	PDBlock* pdb = Product->GetMyPDBlock();

	GRADARDATAHEADER* prdh = pGData->GetHeadP();
	pdb->ProductDependent[1] = (pGData->GetHeadP()->ReflectivityGateSize * pGData->GetElevationHeadP(0)->ReflectivityGates + pGData->GetElevationHeadP(0)->FirstGateRangeOfRef)/1000;//最大半径，单位KM

	//pdb->ElevationNumber=0;

	memcpy(Product->GetRadarInfor()->RadarType, prdh->RadarType, sizeof(gInt8) * 4);

	Product->GetRadarInfor()->Altitude= prdh->RadarHeight;

	Product->GetRadarInfor()->Latitude= prdh->RadarLat;

	Product->GetRadarInfor()->Longitude= prdh->RadarLon;

	memcpy(pdb->VCPMode, prdh->VCPMode, sizeof(gInt8)* 4);

	memcpy(pdb->OperationalMode, prdh->OperationMode, sizeof(gInt8)* 2);

	pdb->ProductDate= prdh->VCPDate;

	pdb->ProductTime= prdh->VCPTime;
	pdb->ScaleOfData = 1;
	pdb->IndexOfTabular = -1;
	pdb->IndexOfGraphic = -1;

	CJulMsGMT::GetLocalJulMs(pdb->GenerationDateOfProduct,
		pdb->GenerationTimeOfProduct);

	memcpy(Product->GetRadarInfor()->SiteCode, prdh->SiteCode,sizeof(gChar)*8);
	//pdb->ProductDependent[3] = gInt16(Temp_Length);



	Product->GetBlockAsStruct_Head(1)->LengthOfDataType = sizeof(STORM_CELLATTRIBUTES);
	Product->GetBlockAsStruct_Head(1)->NumbersOfData = NumOfStorm;
	//Product->GetBlockAsStruct_Head(1)->ScaleOfData = 1;
	TAGSSHEAD phead;
	phead.NumberOfStormCells = NumOfStorm;
	phead.VolumeScanStartTime = pGData->GetVCPTime();
	/*phead.VolumeScanEndTime;*/
	phead.LowThresholdOfReflectivity[0] = index.THRESHOLD_REF_VALUES[0];
	phead.LowThresholdOfReflectivity[1] = index.THRESHOLD_REF_VALUES[1];
	phead.LowThresholdOfReflectivity[2] = index.THRESHOLD_REF_VALUES[2];
	phead.LowThresholdOfReflectivity[3] = index.THRESHOLD_REF_VALUES[3];
	phead.LowThresholdOfReflectivity[4] = index.THRESHOLD_REF_VALUES[4];
	phead.LowThresholdOfReflectivity[5] = index.THRESHOLD_REF_VALUES[5];
	phead.LowThresholdOfReflectivity[6] = index.THRESHOLD_REF_VALUES[6];
	phead.LowThresholdOfReflectivity[7] = index.THRESHOLD_REF_VALUES[7];
	phead.LowThresholdOfReflectivity[8] = index.THRESHOLD_REF_VALUES[8];
	phead.LowThresholdOfReflectivity[9] = index.THRESHOLD_REF_VALUES[9];
	phead.LowThresholdOfReflectivity[10] = index.THRESHOLD_REF_VALUES[10];
	phead.LowThresholdOfReflectivity[11] = index.THRESHOLD_REF_VALUES[11];
	phead.LowThresholdOfReflectivity[12] = index.THRESHOLD_REF_VALUES[12];
	phead.LowThresholdOfReflectivity[13] = index.THRESHOLD_REF_VALUES[13];
	phead.LowThresholdOfReflectivity[14] = index.THRESHOLD_REF_VALUES[14];
	phead.ThresholdOfDropoutDifferenceReflectivity = index.Threshold_Dropout_DifRef;
	phead.ThresholdOfDropoutCount = index.Threshold_Dropout_Count;
	phead.ThresholdOfSegmentLength = index.Threshold_Segment_Length;
	phead.ThresholdOfSeparatedAzimuth = index.Threshold_AZ_SEP;
	phead.ThresholdOfOverlap = index.Threshold_OVERLAP;
	phead.ThresholdOfComponentArea = index.Threshold_AreaCC;
	phead.ThresholdOfSearchRadius[0] = index.Threshold_Radii[0];
	phead.ThresholdOfSearchRadius[1] = index.Threshold_Radii[1];
	phead.ThresholdOfSearchRadius[2] = index.Threshold_Radii[2];
	phead.ThresholdOfElevationMerge = index.Threshold_ELE_Merge;
	phead.ThresholdOfHorizontalMerge = index.Threshold_DIS_Merge;
	phead.ThresholdOfSegmentNumber = index.ThresholdOfSegmentNumber;
	phead.ThresholdOfHeightMerge = index.Threshold_HEI_Merge;
	phead.ThresholdOfHorizontalDelete = index.Threshold_DIS_Delete;
	phead.ThresholdOfDepthDelete = index.Threshold_DDE_Delete;
	phead.scanrang = pGData->GetElevationHeadP(0)->ReflectivityGates*cos(pGData->GetElevationHeadP(0)->Elevation*M_PI/18000)*pGData->GetHeadP()->ReflectivityGateSize/1000;

	((TAGSSHEAD *)(Product->GetBlockAsStruct_Data(0)))[0] = phead;
	Temp_Length = 0;
	for (int i = 0; i < NumOfStorm ; i++)
	{
		if (Temp_Length < m_Cell[i+1].sci.r)
		{
			Temp_Length = m_Cell[i+1].sci.r;
		}
		((STORM_CELLATTRIBUTES *)(Product->GetBlockAsStruct_Data(1)))[i] = m_Cell[i+1];
	}
	pdb->ProductDependent[3] = gInt16(Temp_Length);
	/*for (int k = 0; k < NumOfTilt; k++)
	{
	delete [] m_VolCC[k];
	}
	delete [] m_VolCC;
	m_VolCC = NULL;
	delete [] m_Cell;
	m_Cell = NULL;*/
	return GS_OK;
}
bool Arith_277_SCI::Init_ref_threshold()
{
	//初始化
	//MEMOFSEG[0] = MEMOFSEG[1] = 1024*10;
	//MEMOFSEG[2] = MEMOFSEG[3] = 1024*8;
	//MEMOFSEG[4] = MEMOFSEG[5] = 1024*7;
	//MEMOFSEG[6] = MEMOFSEG[7] = 1024*5;
	//MEMOFSEG[8] = MEMOFSEG[9] = 1024*4;
	//MEMOFSEG[10] = MEMOFSEG[11] = 1024*10;
	for ( int k = 12; k < MAX_TILTS_NUM ; k++)
	{
		MEMOFCPN[k] = 2048;  //modified by sunqc ,原此处错写成EMOFSEG
	}
	for (int k = 10; k < MAX_TILTS_NUM ; k++)
	{
		MEMOFCPN[k] = 0;
	}
	for (int k = 9; k >= 4; k--)
	{
		MEMOFCPN[k] = 600;
	}
	for (int k = 3; k >= 0; k--)
	{
		MEMOFCPN[k] = 600;
	}

	return true;
}
void Arith_277_SCI::ComMassWeiLenSegment(gInt32 SVbeg,gInt32 SVend,gFloat *DBZE,gFloat *RS,gFloat &MWL)
{
	gInt32 k=0;
	gFloat *ZE;
	ZE =  new(::std::nothrow) gFloat[SVend];
	memset(ZE,0,SVend*sizeof(gFloat));

	gFloat *PIN;
	PIN =  new(::std::nothrow) gFloat[SVend];
	memset(PIN,0,SVend*sizeof(gFloat));

	gFloat *MSV;
	MSV =  new(::std::nothrow) gFloat[SVend];
	memset(MSV,0,SVend*sizeof(gFloat));

	for(k=SVbeg; k<SVend; k++)
	{
		if(DBZE[k]>MRM)
			DBZE[k] = MRM;

		ZE[k] = (gFloat)(pow(10.0f,gFloat(DBZE[k]/10.0)));

		PIN[k] = gFloat(pow(gFloat(ZE[k]/MMF),gFloat(1/PIE))*0.001);

		MSV[k] = MWF * PIN[k];

		MWL = MWL + MSV[k]*RS[k]*0.001f;
	}
	delete []ZE;
	ZE = NULL;
	delete []PIN;
	PIN = NULL;
	delete []MSV;
	MSV = NULL;
}

///计算风暴段的质量权重长度平方kg/km
void Arith_277_SCI::ComMassWeiLenSquSegment(gInt32 SVbeg,gInt32 SVend,gFloat *DBZE,gFloat *RS,gFloat &MWLS)
{
	gInt32 k=0;
	gFloat *ZE;
	ZE =  new(::std::nothrow) gFloat[SVend];
	memset(ZE,0,SVend*sizeof(gFloat));

	gFloat *PIN;
	PIN =  new(::std::nothrow) gFloat[SVend];
	memset(PIN,0,SVend*sizeof(gFloat));

	gFloat *MSV;
	MSV =  new(::std::nothrow) gFloat[SVend];
	memset(MSV,0,SVend*sizeof(gFloat));

	for(k=SVbeg; k<SVend; k++)
	{
		if(DBZE[k]>MRM)
			DBZE[k] = MRM;

		ZE[k] = (gFloat)(pow(gFloat(10.0),gFloat(DBZE[k]/10.0)));

		PIN[k] = pow(gFloat(ZE[k]/MMF),gFloat(1/PIE))*0.001f;

		MSV[k] = MWF * PIN[k];

		MWLS = MWLS + MSV[k]*RS[k]*RS[k]*0.001f;
	}
	delete []ZE;
	ZE = NULL;
	delete []PIN;
	PIN = NULL;
	delete []MSV;
	MSV = NULL;
}
gInt32 Arith_277_SCI::CellSegments(StandardRadarData *pGData, gInt32 tilt, vector<STORM_CELLSEGMENT> & m_VolCS)
{
	gInt32 cell_flag;
	gInt32 RA = 3;
	gInt32 INT_half_RA = RA/2;
	gInt32 diff_dbz,num = 0,NumD;//漏失点个数;
	gInt16 **Temp = NULL;
	gInt32 nrays = pGData->GetElevationHeadP(tilt)->AzimuthNumber;
	gInt32 Gates = pGData->GetElevationHeadP(tilt)->ReflectivityGates;
	gInt16* CSnum =  new(::std::nothrow) gInt16[MAX_THRESHOLD_REF+1];
	Temp =  new(::std::nothrow) gInt16* [nrays];
	if (CSnum == NULL || Temp == NULL)
	{
		return GE_NOENOUGHMEMORY;
	}

	for (int i =0; i < nrays; i++)
	{
		Temp[i] = pGData->GetRadarDataR(tilt,i);
	}
	for (int k = 0; k <MAX_THRESHOLD_REF; k++)
	{
		CSnum[k] = CSpos[k] = 0;
	}
	STORM_CELLSEGMENT m_CS;
	memset(&m_CS,0,sizeof(STORM_CELLSEGMENT));
	for (int k = 0; k < THRESHOLD_REF_NUMBER; k++)
	{
		diff_dbz = THRESHOLD_REF_VALUES[k] - Threshold_Dropout_DifRef;
		for (int i = 0; i < nrays; i++)
		{
			NumD = 0;
			cell_flag = PCS_NONE;
			for (int j = INT_half_RA; j < Gates - INT_half_RA; j++)
			{

				if (Temp[i][j]/100 >= THRESHOLD_REF_VALUES[k])
				{
					if (cell_flag == PCS_NONE)
					{
						cell_flag = PCS_BEGIN;
						m_CS.RF_dBZ = THRESHOLD_REF_VALUES[k];
						m_CS.az=i*10;
						m_CS.SVbeg = j;
						m_CS.RSbeg = (gFloat)((j-0.5)*SVL_km);
						m_CS.ITR = k;
						m_CS.ICP = 0;
						cell_flag=PCS_CONTINUE;
					}
					m_CS.SVend = j;
					m_CS.RSend = (gInt16)((j+0.5)*SVL_km);
					NumD = 0;
				}
				else if (Temp[i][j]/100 >= diff_dbz && cell_flag == PCS_CONTINUE)
				{
					NumD++;
					if (NumD > Threshold_Dropout_Count)
					{
						cell_flag = PCS_END;
					}
				}
				else if (cell_flag == PCS_CONTINUE)
				{
					cell_flag = PCS_END;
				}
				if (cell_flag == PCS_END)
				{
					m_CS.len = m_CS.RSend - m_CS.RSbeg;
					if (m_CS.len >= Threshold_Segment_Length)
					{
						num++;
						CSnum[k]++;
						m_VolCS.push_back(m_CS);
					}
					cell_flag = PCS_NONE;
				}
			}
		}
	}
	if (!num)
	{
		delete [] CSnum;
		CSnum = NULL;
		delete [] Temp;
		Temp = NULL;
		return 0;
	}
	for (int k = 0; k < THRESHOLD_REF_NUMBER; k++)
	{
		CSpos[k+1] = CSpos[k] + CSnum[k];
	}
	gFloat Sum_dbz = 0;//计算MAXZ
	for (int i = 0; i < num; i++)
	{
		gInt32 azi_temp = m_VolCS[i].az/10;
		m_VolCS[i].maxZ = 0;
		for (int k = m_VolCS[i].SVbeg; k < m_VolCS[i].SVend; k++)
		{
			Sum_dbz = 0;
			for (int j = k - INT_half_RA; j <= k + INT_half_RA; j++)
			{
				if(Temp[azi_temp][j] != RADAR_DATA_NOECHO)
				Sum_dbz += Temp[azi_temp][j]/10;
			}
			Sum_dbz /= RA;
			if (m_VolCS[i].maxZ < Sum_dbz)
			{
				m_VolCS[i].maxZ = Sum_dbz;
			}
		}
		//////////////////////////////////////////////////////////////////////////计算WML与WMLS
		m_VolCS[i].MWL = m_VolCS[i].MWLS = 0;
		///每个风暴段内对应位置处的反射率
		gFloat *dataz;
		dataz =  new(::std::nothrow) gFloat[Gates];
		memset(dataz,0,Gates*sizeof(gFloat));

		///每个风暴段内对应位置处的斜距
		gFloat *RS;
		RS =  new(::std::nothrow) gFloat[Gates];
		memset(RS,0,Gates*sizeof(gFloat));
		for (int k = m_VolCS[i].SVbeg; k <= m_VolCS[i].SVend; k++)
		{
			dataz[k] = Temp[m_VolCS[i].az/10][k]/100.0f;
			RS[k] = /*m_CS[i].SVbeg*/k * SVL_km;
		}
		ComMassWeiLenSegment(m_VolCS[i].SVbeg,m_VolCS[i].SVend,dataz,RS,m_VolCS[i].MWL);
		ComMassWeiLenSquSegment(m_VolCS[i].SVbeg,m_VolCS[i].SVend,dataz,RS,m_VolCS[i].MWLS);
		delete [] dataz;
		dataz = NULL;
		delete [] RS;
		RS= NULL;
	}
	delete [] CSnum;
	CSnum = NULL;
	delete [] Temp;
	Temp = NULL;
	return num;
}
gInt32 Arith_277_SCI::Cell2DComponents(StandardRadarData *pGData, gInt32 tilt,vector<STORM_CELLSEGMENT> m_VolCS, vector<STORM_CELLCOMPONENT> &m_VolCC)
{
	if (!NumofSeg[tilt])
	{
		return 0;
	}
	bool Unassigned = true;
	gInt16 NumOfIPC,AZsep = 0;//方位角差
	gInt16 LabelC[1024];
	gFloat R0 = 0.0f,R1 = 0.0f,R2 = 0.0f,Area_Component = 0.0f;
	gInt32 Num_C = 0,Num_SC = 0;
	gInt32 comp_num = 0,discard_num = 0;
	gDouble dpx = 0.0,dpy = 0.0,dms = 0.0;
	gFloat fc1 = SVL_km * daz;
	gInt32 Elevation = pGData->GetElevationHeadP(tilt)->Elevation;
	gDouble cos_tilt = cos(Elevation*0.01*M_PI/180.0);
	gInt16* CCnum =  new(::std::nothrow) gInt16[MAX_THRESHOLD_REF+1];
	gInt16* CCpos =  new(::std::nothrow) gInt16[MAX_THRESHOLD_REF+1];
	if (CCnum == NULL || CCpos == NULL)
	{
		return GE_NOENOUGHMEMORY;
	}

	for ( int k = 0; k < MAX_THRESHOLD_REF+1; k++)
	{
		CCnum[k] = CCpos[k] = 0;
	}

	for (int itr = 0; itr < THRESHOLD_REF_NUMBER; itr++)
	{

		NumOfIPC = 0;
		for (int j = CSpos[itr];j < CSpos[itr+1]; j++)
		{
			if (!m_VolCS[j].ICP)
			{
				Unassigned = true;
				for (int i = CSpos[itr]; i < j; i++)
				{
					if (m_VolCS[i].ICP)
					{
						R1 = (m_VolCS[i].RSbeg > m_VolCS[j].RSbeg) ? (m_VolCS[i].RSbeg):(m_VolCS[j].RSbeg);
						R2 = (m_VolCS[i].RSend < m_VolCS[j].RSend) ? (m_VolCS[i].RSend):(m_VolCS[j].RSend);
						R0 = R2 - R1;
						AZsep = abs(m_VolCS[j].az-m_VolCS[i].az);
						if (AZsep > 1800)
						{
							AZsep = 3600 - AZsep;
						}
						if (AZsep <= Threshold_AZ_SEP && R0 >= Threshold_OVERLAP)
						{
							m_VolCS[j].ICP = m_VolCS[i].ICP;
							Unassigned = false;
							break;
						}
					}
				}
				if (Unassigned)
				{
					++NumOfIPC;
					m_VolCS[j].ICP = NumOfIPC;
				}
			}
		}
		Num_C = 0;
		/*for (int k = 0; k < 1024; k++)
		{
		LabelC[k] = 0;
		}*/
		memset(LabelC,0,sizeof(gInt16)*1024);
		// compute component area;
		for (int k = 1; k <= NumOfIPC; k++)
		{
			Num_SC = 0;
			Area_Component = 0;
			for (int j = CSpos[itr]; j < CSpos[itr+1]; j++)
			{
				if (m_VolCS[j].ICP == k)
				{
					Num_SC++;
					Area_Component += (m_VolCS[j].RSend + m_VolCS[j].RSbeg) * (m_VolCS[j].RSend - m_VolCS[j].RSbeg);
				}
			}
			Area_Component = (gFloat)(0.5*daz*Area_Component);
			if (Num_SC >= ThresholdOfSegmentNumber && Area_Component >= Threshold_AreaCC)
			{
				++Num_C;
				LabelC[k] = Num_C;
			}
		}
		for (int j = CCpos[itr]; j < CCpos[itr+1]; j++)
		{
			m_VolCS[j].ICP = LabelC[m_VolCS[j].ICP];
		}
		STORM_CELLCOMPONENT m_CC;
		memset(&m_CC,0,sizeof(STORM_CELLCOMPONENT));
		for (int k = 1; k <=Num_C; k++)
		{
			m_CC.ICP = k;
			m_CC.ITR = itr;
			m_CC.RCbeg = 9999;
			m_CC.RCend = -9999;
			m_CC.AZbeg = 3601;
			m_CC.AZend = -3601;
			dms = dpx = dpy = 0;
			for ( int j = CSpos[itr]; j < CSpos[itr+1]; j++)
			{
				if ( m_VolCS[j].ICP == k)
				{
					//compute beginning ending range
					if( m_CC.RCbeg > m_VolCS[j].RSbeg )
					{
						m_CC.RCbeg = m_VolCS[j].RSbeg;
					}
					if (m_CC.RCend < m_VolCS[j].RSend)
					{
						m_CC.RCend = m_VolCS[j].RSend;
					}
					//compute beginning ending Azimmuth
					if( m_CC.AZbeg > m_VolCS[j].az )
					{
						m_CC.AZbeg = m_VolCS[j].az;
					}
					if (m_CC.AZend < m_VolCS[j].az)
					{
						m_CC.AZend = m_VolCS[j].az;
					}
					//compute maximum refl. factor
					if (m_CC.maxZ < m_VolCS[j].maxZ)
					{
						m_CC.maxZ = m_VolCS[j].maxZ;

					}
					//cumulator
					dms += m_VolCS[j].MWL;
					dpx += m_VolCS[j].MWLS*sin(m_VolCS[j].az*0.1*M_PI/180.0);
					dpy += m_VolCS[j].MWLS*cos(m_VolCS[j].az*0.1*M_PI/180.0);

				}
			}
			m_CC.Cell_id = 0;
			m_CC.SC_id = 0;
			m_CC.nexted = 0;
			//compute MASS of Component
			m_CC.MASS = (gFloat)(dms * fc1);
			//compute X-position & Y-position of Component
			m_CC.px = (gFloat)(dpx*cos_tilt/dms);
			m_CC.py = (gFloat)(dpy*cos_tilt/dms);
			//compute Range, Azimuth and Height of Component
			m_CC.RC = (gFloat)(sqrt(m_CC.px*m_CC.px + m_CC.py*m_CC.py));
			m_CC.az = (gFloat)(asin(m_CC.px/m_CC.RC)/(M_PI/180.0));
			if (m_CC.py < 0)
			{
				m_CC.az = 180 - m_CC.az;
			}
			else if (m_CC.px < 0)
			{
				m_CC.az = 360+m_CC.az;
			}
			m_CC.hei = (gFloat)(m_CC.RC*(sin(Elevation*0.01*M_PI/180.0)+0.5*m_CC.RC/(IR * RE)));
			if (itr == 0)
			{
				comp_num++;
				CCnum[itr]++;
			}
			else
			{
				Unassigned = true;
				for ( int j = CCpos[0]; j < CCpos[itr]; j++)
				{
					if (m_VolCC[j].az>=m_CC.AZbeg && m_VolCC[j].az<=m_CC.AZend &&
						m_VolCC[j].RC >=m_CC.RCbeg && m_VolCC[j].RC<=m_CC.RCend)
					{
						Unassigned = false;
					}
					else if (m_CC.AZbeg > m_CC.AZend)
					{//component crosses zero degree
						if ((m_VolCC[j].az>=m_CC.AZbeg || m_VolCC[j].az<=m_CC.AZend) &&
							(m_VolCC[j].RC >=m_CC.RCbeg && m_VolCC[j].RC<=m_CC.RCend))
						{
							Unassigned = false;
						}
					}
					if (!Unassigned)
					{
						break;
					}
				}
				if (Unassigned)
				{
					comp_num++;
					CCnum[itr]++;
				}
				else
				{
					discard_num++;
				}
				if ( comp_num >= 511)
				{
					delete [] CCnum;
					CCnum = NULL;
					delete [] CCpos;
					CCpos = NULL;
					return 0;
				}
			}
			m_VolCC.push_back(m_CC);
		}

		//remember the position of current threshold
		CCpos[itr+1] = CCnum[itr] + CCpos[itr];
	}

	Unassigned = true;
	STORM_CELLCOMPONENT *m_CCtemp =  new(::std::nothrow) STORM_CELLCOMPONENT;
	memset(m_CCtemp,0,sizeof(STORM_CELLCOMPONENT));
	while (Unassigned)
	{
		Unassigned = false;

		for ( int k = 0; k < comp_num - 1; k++)
		{

			if (m_VolCC[k].MASS<m_VolCC[k+1].MASS)
			{
				memcpy(m_CCtemp, &m_VolCC[k],sizeof(STORM_CELLCOMPONENT) );

				memcpy(&m_VolCC[k], &m_VolCC[k+1],sizeof(STORM_CELLCOMPONENT) );

				memcpy(&m_VolCC[k+1],m_CCtemp,sizeof(STORM_CELLCOMPONENT) );

				Unassigned=true;
			}
		}

	}
	delete m_CCtemp;
	m_CCtemp = NULL;
	delete [] CCnum;
	CCnum = NULL;
	delete [] CCpos;
	CCpos = NULL;
	return comp_num;
}
gInt32 Arith_277_SCI::StormID_correlated(StandardRadarData *pGData, vector<vector<STORM_CELLCOMPONENT> >  m_VolCCG, vector<STORM_CELLATTRIBUTES> &m_Cell)
{
	STORM_CELLATTRIBUTES m_CellS;
	memset(&m_CellS,0,sizeof(STORM_CELLATTRIBUTES));
	m_Cell.push_back(m_CellS);
	for (int i = 0; i < MAX_TILTS_NUM; i++)
	{
		/*m_Cell->NEC[i] = 0;*/
		(m_Cell[0]).NEL[i] = 0;
	}
	NumofCell = 0;
	gFloat DD_C = 0.0f;
	gInt32 tilt = pGData->GetHeadP()->ElevationCount;
	gInt32 id = 0,nextelev = 0;
	for (int elev = 0; elev < tilt - 1; elev ++ )
	{
		if (!NumofCpn[elev])
		{
			continue;
		}
		nextelev = elev + 1;
		for (int isr = 0; isr < search_Radii_Num; isr ++)
		{
			for (int j = 0; j < NumofCpn[elev]; j++)
			{
				//do until component(tilt) is correlated with a component(next tilt)
				if ((m_VolCCG[elev])[j].nexted)
				{
					continue;
				}
				for (int i = 0; i < NumofCpn[nextelev]; i++)
				{
					if ((m_VolCCG[nextelev])[i].Cell_id)
					{
						continue;
					}
					DD_C = ((m_VolCCG[elev])[j].px - (m_VolCCG[nextelev])[i].px) * ((m_VolCCG[elev])[j].px - (m_VolCCG[nextelev])[i].px) +
						((m_VolCCG[elev])[j].py - (m_VolCCG[nextelev])[i].py) * ((m_VolCCG[elev])[j].py - (m_VolCCG[nextelev])[i].py);
					if (DD_C <= Threshold_Radii[isr] * Threshold_Radii[isr])
					{
						// if m_VolCC[elev][j] is not correlated into a Storm Cell
						if (!(m_VolCCG[elev])[j].Cell_id)
						{
							//identify  new(::std::nothrow) storm cell
							++NumofCell;
							m_CellS.sci.id = (m_VolCCG[elev])[j].Cell_id = NumofCell;
							//compute Number of Components (Storm Cell)
							//Number of Components based on 1
							(m_VolCCG[elev])[j].SC_id = 1;
							(m_VolCCG[nextelev])[i].SC_id = 2;
							m_CellS.sci.NSC=2;
							m_CellS.NEL[1] = elev;
							m_CellS.NEL[2] = nextelev;
							m_CellS.NEC[1] = (m_VolCCG[elev])[j];
							m_CellS.NEC[2] = (m_VolCCG[nextelev])[i];
							m_Cell.push_back(m_CellS);
						}
						else
						{
							//compute Number of Component(storm  cell)
							id = (m_VolCCG[elev])[j].Cell_id;
							m_Cell[id].sci.NSC++;
							(m_VolCCG[nextelev])[i].SC_id = m_Cell[id].sci.NSC;
							m_Cell[id].NEL[m_Cell[id].sci.NSC] = nextelev;
							m_Cell[id].NEC[m_Cell[id].sci.NSC] = (m_VolCCG[nextelev])[i];
						}
						//correlate m_VolCC[nextelev][i] with m_VolCC[elev][j]
						(m_VolCCG[nextelev])[i].Cell_id = (m_VolCCG[elev])[j].Cell_id;
						(m_VolCCG[elev])[j].nexted = nextelev;
						break;

					}
				}//end of nextelev
			}//end of NumofCpn[elev]
		}//end of search_Radii_Num
	}// end do for all tilt
	return NumofCell;

}

gInt32 Arith_277_SCI::StormID_merger(StandardRadarData *pGData,/*vector<vector<STORM_CELLCOMPONENT>>  m_VolCCG,*/ vector<STORM_CELLATTRIBUTES> &m_Cell)
{
	gInt32 Decrement = 0;
	gFloat DD_SC = 0.0f,HD = 0.0f;///DD_SC风暴体的质心距离差,高度差
	gInt16 ED = 0;//仰角差

	for (int n = 1; n <= NumofCell; n++)
	{
		CellLocation(pGData,/*m_VolCCG,*/&m_Cell[n]);
	}
	char *deal_flag =  new(::std::nothrow) char[NumofCell+1];
	memset(deal_flag,0,sizeof(char)*(NumofCell+1));
	//merger
	for (int n = 1; n <= NumofCell; n++)
	{
		if (deal_flag[n])
		{
			continue;
		}
		for (int m = 1; m <= NumofCell; m++)
		{
			//remaining the CELLS which have not been checked for merger
			if (n == m || deal_flag[m])
			{
				continue;
			}
			//compute CENTRIOD DISTANCE DIFFERENCE
			DD_SC = (m_Cell[m].sci.vx-m_Cell[n].sci.vx) * (m_Cell[m].sci.vx-m_Cell[n].sci.vx) +
				(m_Cell[m].sci.vy-m_Cell[n].sci.vy) * (m_Cell[m].sci.vy-m_Cell[n].sci.vy) ;
			//compute ELEVATION DIFFERENCE
			ED = (m_Cell[m].NEL[1] > m_Cell[n].NEL[m_Cell[n].sci.NSC])?pGData->GetElevationHeadP(m_Cell[m].NEL[1])->Elevation - pGData->GetElevationHeadP(m_Cell[n].NEL[m_Cell[n].sci.NSC])->Elevation \
				: pGData->GetElevationHeadP(m_Cell[n].NEL[m_Cell[n].sci.NSC])->Elevation - pGData->GetElevationHeadP(m_Cell[m].NEL[1])->Elevation;
			//height difference
			HD = (m_Cell[m].sci.base > m_Cell[n].sci.top)? m_Cell[m].sci.base - m_Cell[n].sci.top:m_Cell[n].sci.base - m_Cell[m].sci.top;
			if (DD_SC <= Threshold_DIS_Merge * Threshold_DIS_Merge && HD > 0 && ED > 0 && (ED <= Threshold_ELE_Merge || HD <= Threshold_HEI_Merge))
			{
				//merge the STORM CELL by moving the components from one STORM CELL to the other STORM CELL
				//and deleting the other STORM CELL
				if (m_Cell[m].NEL[1] > m_Cell[n].NEL[m_Cell[n].sci.NSC])
				{
					for (int k = 1; k <= m_Cell[m].sci.NSC; k++)
					{
						if(m_Cell[n].sci.NSC+k<MAX_TILTS_NUM)
						{
							m_Cell[n].NEL[m_Cell[n].sci.NSC+k] = m_Cell[m].NEL[k];
							m_Cell[n].NEC[m_Cell[n].sci.NSC+k] = m_Cell[m].NEC[k];
						}
					}
					m_Cell[n].sci.NSC += m_Cell[m].sci.NSC;
					deal_flag[m] = -1 ;   // decrement_flag
					deal_flag[n] = 1 ;    // merged_flag
				}
				else
				{
					for (int k = 1; k <= m_Cell[n].sci.NSC; k++)
					{
						if(m_Cell[m].sci.NSC+k<MAX_TILTS_NUM)
						{
							m_Cell[m].NEL[m_Cell[m].sci.NSC+k] = m_Cell[n].NEL[k];
							m_Cell[m].NEC[m_Cell[m].sci.NSC+k] = m_Cell[n].NEC[k];
						}
					}
					m_Cell[m].sci.NSC += m_Cell[n].sci.NSC;
					deal_flag[m] = 1 ;    // merged_flag
					deal_flag[n] = -1 ;   // decrement_flag
				}
				Decrement ++;
				DD_SC = 0;
			}//if
		}//for  m

	}//for n
	gInt32 num = 0;
	for (int n = 1; n <= NumofCell; n++)
	{
		/*	m_CellS = m_Cell[n];*/
		if (deal_flag[n] == -1)
		{
			//decrement
			deal_flag[n] = 0;
			m_Cell[n].sci.id = 0;
			continue;
		}
		else if (deal_flag[n] == 1)
		{
			CellLocation(pGData,/*m_VolCCG,*/&m_Cell[n]);
		}
		else
		{
			deal_flag[n] = 1;
		}
		++num;
		m_Cell[n].sci.id = num;
		//compute Range, Azimuth of Storm Cell
		m_Cell[n].sci.r = (gFloat)sqrt(m_Cell[n].sci.vx*m_Cell[n].sci.vx + m_Cell[n].sci.vy*m_Cell[n].sci.vy);
		m_Cell[n].sci.az = (gFloat)(asin(m_Cell[n].sci.vx/m_Cell[n].sci.r)/M_PI*180.0);
		if (m_Cell[n].sci.vy < 0)
		{
			m_Cell[n].sci.az = 180 - m_Cell[n].sci.az;
		}
		else if (m_Cell[n].sci.vx < 0)
		{
			m_Cell[n].sci.az = 360 + m_Cell[n].sci.az;
		}

		Cell_VIL(pGData,/*m_VolCCG,*/&m_Cell[n]);
		//deleteing
		//for (int n = 1; n <= NumofCell; n++)
		//{
		if (m_Cell[n].sci.VIL < 150)// 0.1kg/m*m
		{
			deal_flag[n] = 0;
		}
		if (!deal_flag[n])
		{
			continue;
		}
		for (int m = 1; m <= NumofCell; m++)
		{
			if ( n == m || !deal_flag[m])
			{
				continue;
			}
			//compute CENTRIOD DISTANCE DIFFERENCE
			DD_SC = (m_Cell[m].sci.vx-m_Cell[n].sci.vx) * (m_Cell[m].sci.vx-m_Cell[n].sci.vx) +
				(m_Cell[m].sci.vy-m_Cell[n].sci.vy) * (m_Cell[m].sci.vy-m_Cell[n].sci.vy);
			if (DD_SC <= Threshold_DIS_Delete*Threshold_DIS_Delete)
			{
				if (m_Cell[m].sci.VIL < m_Cell[n].sci.VIL)
				{
					deal_flag[m] = 0;
				}
				else
				{
					deal_flag[n] = 0;
					Decrement ++;
				}
			}
			if (!deal_flag[n])
			{
				break;
			}
			if (!deal_flag[m])
			{
				continue;
			}
			ED = abs(m_Cell[m].sci.depth - m_Cell[n].sci.depth);
			if (DD_SC <= Threshold_DIS_Delete*Threshold_DIS_Delete*4 &&  ED > Threshold_DDE_Delete)
			{
				if (m_Cell[m].sci.depth<m_Cell[n].sci.depth)
				{
					deal_flag[m] = 0;
				}
				else
				{
					deal_flag[n] = 0;
				}
				Decrement++;
			}
			if (!deal_flag[n])
			{
				break;
			}
		}
		//}
	}
	num = 0;
	for (int n = 1; n <= NumofCell; n++)
	{
		if (!deal_flag[n])
		{
			m_Cell[n].sci.id = 0;
		}
		else
		{
			++num;
			m_Cell[n].sci.id = num;
			if (num != n)
			{
				memcpy(&m_Cell[num], &m_Cell[n],sizeof(STORM_CELLATTRIBUTES));
			}
			m_Cell[num].sci.VIL = (m_Cell[num].sci.VIL + 5)/10;
		}
	}
	NumofCell = num;
	delete [] deal_flag;
	deal_flag = NULL;
	Decrement = 1;
	while (Decrement)
	{
		Decrement = 0;
		for ( int n =1; n < NumofCell; n++)
		{
			if (m_Cell[n].sci.VIL<m_Cell[n+1].sci.VIL || (m_Cell[n].sci.VIL == m_Cell[n+1].sci.VIL && m_Cell[n].sci.maxZ < m_Cell[n+1].sci.maxZ))
			{
				memcpy(&m_Cell[0], &m_Cell[n],sizeof(STORM_CELLATTRIBUTES));
				memcpy(&m_Cell[n], &m_Cell[n+1],sizeof(STORM_CELLATTRIBUTES));
				memcpy(&m_Cell[n+1], &m_Cell[0],sizeof(STORM_CELLATTRIBUTES));
				Decrement++;
			}
		}
	}
	//re_number the cell's id,based on 1
	for (int n = 1; n <= NumofCell; n++)
	{
		m_Cell[n].sci.id = n;
	}
	return NumofCell;
}
void Arith_277_SCI::CellLocation(StandardRadarData *pGData, /*vector<vector<STORM_CELLCOMPONENT>>  m_VolCCG,*/ STORM_CELLATTRIBUTES* pCell)
{
	//compute MASS (Storm Cell)
	//compute X-position & Y-position of Storm Cell
	//compute Base & Top of Storm Cell
	gInt32 elev = 0,nextelev = 0,lastelev = 0;
	gDouble DCH = 0.0,vxs,vys,vmass,vhs;
	vxs = vys = vmass = vhs = 0;
	gInt32 tile_num = pGData->GetHeadP()->ElevationCount;
	for (int i = 1; i <= pCell->sci.NSC; i++)
	{
		elev = pCell->NEL[i];
		if (i == 1)
		{
			DCH = (elev == 0)? (/*m_VolCC[1][pCell->NEC[2]].hei*/pCell->NEC[2].hei + /*m_VolCC[0][pCell->NEC[1]].hei*/pCell->NEC[1].hei)/2: fabs(/*m_VolCC[elev+1][pCell->NEC[2]].hei*/pCell->NEC[2].hei - /*m_VolCC[elev][pCell->NEC[1]].hei*/pCell->NEC[1].hei);
			pCell->sci.base =/* m_VolCC[elev][pCell->NEC[1]].hei*/pCell->NEC[1].hei;
		}
		else if ( i == pCell->sci.NSC)
		{
			DCH = fabs(/*m_VolCC[elev][pCell->NEC[i]].hei*/pCell->NEC[i].hei - /*m_VolCC[elev-1][pCell->NEC[i-1]].hei*/pCell->NEC[i - 1].hei);
			pCell->sci.top = (elev == tile_num - 1)? UNKNOWN_STORM_TOP + 1: /*m_VolCC[elev][pCell->NEC[i]].hei*/pCell->NEC[i].hei;
		}
		else
		{
			nextelev = pCell->NEL[i + 1];
			lastelev = pCell->NEL[i - 1];
			DCH = fabs((/*m_VolCC[nextelev][pCell->NEC[i+1]].hei*/pCell->NEC[i+1].hei - /*m_VolCC[lastelev][pCell->NEC[i-1]].hei*/pCell->NEC[i - 1].hei)/2);
		}
		DCH *= /*m_VolCC[elev][pCell->NEC[i]].MASS*/pCell->NEC[i].MASS;
		vmass += DCH;
		vxs += DCH * pCell->NEC[i].px;
		vys += DCH * pCell->NEC[i].py;
		vhs += DCH * pCell->NEC[i].hei;
	}
	pCell->sci.MASS = (gFloat)vmass;
	pCell->sci.vx = (gFloat)(vxs/vmass);
	pCell->sci.vy = (gFloat)(vys/vmass);
	pCell->sci.h = (gFloat)(vhs/vmass);
	return;
}
void Arith_277_SCI::Cell_VIL(StandardRadarData *pGData,/* vector<vector<STORM_CELLCOMPONENT>>  m_VolCCG,*/ STORM_CELLATTRIBUTES *pCell)
{
	//compute VIL(storm cell)
	gInt32 elev_num = pGData->GetHeadP()->ElevationCount;
	gDouble azi_temp = 0.0;
	gDouble tan_azi_temp[MAX_TILTS_NUM],cos2_el0 = 0.0;
	gDouble VIL = 0.0,DB = 0.0,LW = 0.0;

	pCell->sci.maxZ = 0;
	azi_temp = (pGData->GetElevationHeadP(0)->Elevation + pGData->GetElevationHeadP(1)->Elevation) * 0.01 * M_PI/180.0 * 0.5;
	tan_azi_temp[0] = tan(azi_temp);
	cos2_el0 = 2.0 * IR * RE * cos(azi_temp) * cos(azi_temp);
	for (int elev = 1; elev < elev_num; elev ++)
	{
		if ( elev == elev_num - 1)
		{
			azi_temp = (pGData->GetElevationHeadP(elev)->Elevation * 0.01 +0.5)*M_PI/180.0;
			tan_azi_temp[elev] = tan(azi_temp);
			azi_temp = pGData->GetElevationHeadP(elev - 1)->Elevation * 0.01 * M_PI/180.0;
			tan_azi_temp[elev] -= tan(azi_temp);
			tan_azi_temp[elev] *= 0.5;
		}
		else
		{
			azi_temp = pGData->GetElevationHeadP(elev + 1)->Elevation * 0.01 * M_PI/180.0;
			tan_azi_temp[elev] = tan(azi_temp);
			azi_temp = pGData->GetElevationHeadP(elev - 1)->Elevation * 0.01 * M_PI/180.0;
			tan_azi_temp[elev] -= tan(azi_temp);
			tan_azi_temp[elev] *= 0.5;
		}
	}
	for (int i = 1; i < pCell->sci.NSC; i++)
	{
		gInt32 elev = pCell->NEL[i];
		azi_temp = pCell->NEC[i].RC;
		if (pCell->NEC[i].maxZ > pCell->sci.maxZ)
		{
			pCell->sci.maxZ = pCell->NEC[i].maxZ ;
			pCell->sci.H_maxZ = pCell->NEC[i].hei ;
		}
		DB = (elev == 0)? azi_temp * (tan_azi_temp[0] + azi_temp/cos2_el0): azi_temp * tan_azi_temp[elev];
		LW = (pCell->NEC[i].maxZ > 56)? 5.6: pCell->NEC[i].maxZ * 0.1;

		LW = pow(10,LW);
		VIL += 0.00344 * pow(LW, 4.0/7.0) * DB;

	}
	pCell->sci.VIL = (gInt16)(VIL * 10 + 0.5);
	//compute depth
	if (pCell->sci.top < UNKNOWN_STORM_TOP)
	{
		pCell->sci.depth = (gInt16)((pCell->sci.top - pCell->sci.base) * 100);
	}
	return;
}
extern "C" DLLEXP_GSYS IArith *Create_arith (void)//用于创建Arith_277_SCI接口
{
	return  new(::std::nothrow) Arith_277_SCI;
}
