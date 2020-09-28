#ifndef IARITHMS_LIB
	#define IARITHMS_LIB
#endif
#include "Arith_262_HBR_270_QPR.h"
#include <malloc.h>
#include <memory.h>
#include <wchar.h>
#include "CJulMsGMT.h"

#include <stdio.h>
#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif
#include <math.h>
#include <fstream>

#define STD_GATELEN 1000
//#ifndef max
//#define max(a, b)  (((a) > (b)) ? (a) : (b)) 
//#endif
//#ifndef min
//#define min(a,b) (((a) < (b))?(a):(b))
//#endif

#define NEW_ARRAY_2D(type, pointer, width, height)\
	type** pointer =  new(::std::nothrow) type*[height]; \
{for(int x = 0; x < height; ++x) {pointer[x] =  new(::std::nothrow) type[width];memset(pointer[x], 0x00, sizeof(type)*width);}}\

#define DELETE_ARRAY_2D(pointer, width, height) \
{for(int x = 0; x < height; ++x) delete [] pointer[x];delete pointer;pointer = NULL;}\

Arith_262_HBR_270_QPR::Arith_262_HBR_270_QPR()
{

}
Arith_262_HBR_270_QPR::~Arith_262_HBR_270_QPR()
{

}

GHRESULT Arith_262_HBR_270_QPR::Initialize()
{
	m_pInputList = NULL;
	m_pOutputList = NULL;

	return GS_OK;
};
GHRESULT Arith_262_HBR_270_QPR::UnInitialize()
{
	return GS_OK;
};

GHRESULT Arith_262_HBR_270_QPR::LoadData(void* pdata, ReadParam * ReadParameter)
{
	if(pdata == NULL) 
		return GE_INVALIDARG;
	m_pInputList = (GXList<GenerationData >*)pdata;
	if(ReadParameter == NULL) 
		return GE_INVALIDARG;
	m_Parameter = ReadParameter;
	return GS_OK;
};

GHRESULT Arith_262_HBR_270_QPR::OutPutData(void* pdata)
{
	if(pdata == NULL) 
		return GE_INVALIDARG;
	m_pOutputList = (GXList<GenerationData>*)pdata;
	return GS_OK;
};

extern "C" DLLEXP_GSYS IArith *Create_arith (void)//用于创建Arith_262_HBR_270_QPR接口
{
	return  new(::std::nothrow) Arith_262_HBR_270_QPR;
}



GHRESULT Arith_262_HBR_270_QPR::Execute()
{
	ofstream logfile;
	logfile.open("./Log/Arith_262_HBR_270_QPR.txt",ios::app);
	gUint16 iJulDay;
	gInt32 iMilliSecond,iYear,iMonth,iDay,iHour,iMinute,iSecond,iMSecond;
	CJulMsGMT::GetLocalJulMs(iJulDay, iMilliSecond );
	CJulMsGMT::Jul2Greg(iJulDay,iYear,iMonth,iDay);
	CJulMsGMT::MilliSecond2Time(iMilliSecond,iHour,iMinute,iSecond,iMSecond);
	logfile<<endl<<iHour<<":"<<iMinute<<":"<<iSecond<<"	"<<iYear<<"-"<<iMonth<<"-"<<iDay<<endl;
	logfile<<"262_HBR_270_QPR：  Started: "<<endl;

	StandardRadarData* pstdRData = &(*(m_pInputList->GetHeadAs<StandardRadarData >()));

	logfile<<"262_HBR_270_QPR：  GetHeadP() and GetElevationHeadP() from StandardRadarData. "<<endl;

	GRADARDATAHEADER*    pRadarDataHdr = (pstdRData->GetHeadP());
	GELEVATIONDATAHEAD*  pElevDataHdr = (pstdRData->GetElevationHeadP(0));
	RVWDATAHEAD*         pRVWDataHdr = NULL;

	gInt16 nTotalElevCnt = pRadarDataHdr->ElevationCount;

	logfile<<"262_HBR_270_QPR：  Judge if ElevationCount>=4. "<<endl;

	if(nTotalElevCnt < 4) 
	{
		logfile<<"262_HBR_270_QPR：  ElevationCount<4 and Return GE_WRONGOFINPUTDATA. "<<endl;
		return GE_WRONGOFINPUTDATA;
	}
	gInt32 maxRefGateNum = 0, maxVelGateNum = 0;

	logfile<<"262_HBR_270_QPR：  Find MaxRefGateNum and MaxVelGateNum in all Elevations. "<<endl;

	for(gInt16 i = 0; i< pstdRData->GetHeadP()->ElevationCount; i++)
	{
		gInt16 TempNumOfGates1 = pstdRData->GetElevationHeadP(i)->ReflectivityGates;
		gInt16 TempNumOfGates2 = pstdRData->GetElevationHeadP(i)->DopplerGates;
		if(maxRefGateNum < TempNumOfGates1)
		{		
			maxRefGateNum = TempNumOfGates1;			
		}
		if(maxVelGateNum < TempNumOfGates2)
		{		
			maxVelGateNum = TempNumOfGates2;
		}
	}
	//maxRefGateNum = pRadarDataHdr->maxRefgatenum;
	//maxVelGateNum = pRadarDataHdr->maxVelgatenum;

	// Construct input parameters.
	gInt32   nAzimuthNum, nRefGateLen;
	gInt32*  pRefGateCnt = NULL;
	//gInt16*** parBasicRefRateData =  new(::std::nothrow) gInt16**[nTotalElevCnt];
	gUint16** ppTiltAzimuthAngle =  new(::std::nothrow) gUint16*[nTotalElevCnt];
	gInt16*  pTiltElev =  new(::std::nothrow) gInt16[nTotalElevCnt];

	nAzimuthNum =  pElevDataHdr->AzimuthNumber;
	nRefGateLen =  (gInt32)pRadarDataHdr->ReflectivityGateSize;

	pRefGateCnt =  new(::std::nothrow) gInt32[nTotalElevCnt];
	logfile<<"262_HBR_270_QPR：  Loop: i=0-->ElevationCount("<<nTotalElevCnt<<")"<<endl;
	logfile<<"262_HBR_270_QPR：  Get Elevation Head and RadarData Information."<<endl;

	for(int i = 0; i < nTotalElevCnt; ++i)
	{
		(ppTiltAzimuthAngle[i]) =  new(::std::nothrow) gUint16[nAzimuthNum];

		//parBasicRefRateData[i] =  new(::std::nothrow) gInt16*[nAzimuthNum];
		pElevDataHdr = (pstdRData->GetElevationHeadP(i));
		pRefGateCnt[i] = (gInt32)(pElevDataHdr->ReflectivityGates);
		pTiltElev[i] = (gInt16)(pElevDataHdr->Elevation);
		for(int j = 0; j < nAzimuthNum; ++j)
		{
			pRVWDataHdr = pstdRData->GetRadarDataP(i, j);
			(ppTiltAzimuthAngle[i])[j] = pRVWDataHdr->Azimuth;
		}
	}

	QPRDATA** pOutputQPR = NULL;
	gUint32 OutSizeQPR = 0;
	gUint32 OutRowQPR = 0;
	gUint32 OutColQPR = 0;

	void** pOutputHBR = NULL;
	gUint32 OutSizeHBR = 0;
	gUint32 nRayCount  = 0;
	gUint32 OutGateCount = 0;

	logfile<<"262_HBR_270_QPR：  Loop Finished."<<endl;
	logfile<<"262_HBR_270_QPR：  Function: DoQRate( )."<<endl;

	gInt32 ret = DoQRate(nAzimuthNum,
		nRefGateLen,
		pRefGateCnt,
		/*    parBasicRefRateData,*/
		pstdRData,
		ppTiltAzimuthAngle,
		pTiltElev,
		(&(pOutputQPR)), &OutSizeQPR, &OutRowQPR, &OutColQPR,&(pOutputHBR), &OutSizeHBR, &OutGateCount);

	logfile<<"262_HBR_270_QPR：  Finished DoQRate() and Get the return value: ret."<<endl;

	if(ret <= 0) 
	{
		if(pRefGateCnt != NULL)
		{
			delete [] pRefGateCnt;
			pRefGateCnt = NULL;
		}

		if(ppTiltAzimuthAngle != NULL)
		{
			for(gInt16 i = 0; i< nTotalElevCnt; i++)
			{
				delete [] ppTiltAzimuthAngle[i];
			}
			delete[] ppTiltAzimuthAngle;
			ppTiltAzimuthAngle = NULL;
		}

		if(pTiltElev != NULL)
		{
			delete [] pTiltElev;
			pTiltElev = NULL;
		}

		DELETE_ARRAY_2D(pOutputHBR, OutGateCount, nAzimuthNum);
		DELETE_ARRAY_2D(pOutputQPR, (gInt32)OutColQPR, (gInt32)OutRowQPR);

		logfile<<"262_HBR_270_QPR：  ret<=0 and Return GE_FAIL."<<endl;
		logfile.close();

		return GE_FAIL;
	}
	else 
	{
		if(ret&0x1)
		{
			//logfile<<"262_HBR_270_QPR：  ret&0x1 and call Function: SaveHBRToProductData()."<<endl;

			//// Do HBR data saving.
			//SaveHBRToProductData(pstdRData,
			//	pRadarDataHdr,
			//	pOutputHBR,
			//	OutSizeHBR,
			//	nAzimuthNum,
			//	OutGateCount);
			//logfile<<"262_HBR_270_QPR：  DELETE_ARRAY_2D(pOutputHBR, OutGateCount, nAzimuthNum)."<<endl;
			//DELETE_ARRAY_2D(pOutputHBR, OutGateCount, nAzimuthNum);
		}
		if(ret&0x2)
		{
			logfile<<"262_HBR_270_QPR：  ret&0x2 and call Function: SaveQPRToProductData()."<<endl;
			// Do QPR data saving.
			SaveQPRToProductData(pstdRData,
				pRadarDataHdr,
				(QPRDATA**)(pOutputQPR),
				OutSizeQPR,
				OutRowQPR,
				OutColQPR);
			logfile<<"262_HBR_270_QPR：  DELETE_ARRAY_2D(pOutputQPR, OutColQPR, OutRowQPR)."<<endl;
			DELETE_ARRAY_2D(pOutputQPR, (gInt32)OutColQPR, (gInt32)OutRowQPR);
		}
	}

	if(pRefGateCnt != NULL)
	{
		delete [] pRefGateCnt;
		pRefGateCnt = NULL;
	}

	if(ppTiltAzimuthAngle != NULL)
	{
		for(gInt16 i = 0; i< nTotalElevCnt; i++)
		{
			delete [] ppTiltAzimuthAngle[i];
		}
		delete[] ppTiltAzimuthAngle;
		ppTiltAzimuthAngle = NULL;
	}

	if(pTiltElev != NULL)
	{
		delete [] pTiltElev;
		pTiltElev = NULL;
	}

	logfile<<"262_HBR_270_QPR：  Finished."<<endl;
	logfile.close();

	return GS_OK;
}

//GHRESULT Arith_262_HBR_270_QPR::CalcGridRes(GRID_DATA**	pGridData,
//                                            gInt32	w,
//                                            gInt32	h)
//{
//    // 算法中库长等于分辨率（x/y），GRID_DATA中的dist_x，dist_y即为栅格分辨率。
//    // 如果要更精确的计算分辨率，在此添加处理，并修改ConvertRadialToGrid函数，
//    // 在GRID_DATA中填写栅格同雷达站的物理距离。
//    return GS_OK;
//};




////////////////////////////////////////////////////////////////
// Algorithm Begin
////////////////////////////////////////////////////////////////

// gInt32 Arith_262_HBR_270_QPR::DoQRate( gInt32 nVCPElevTiltCnt,	// number of VCP tilts
/*  gInt32 Arith_262_HBR_270_QPR::DoQRate( gInt32 nRay,			//number of radials 
gInt32 iRefGateLen,		// length of ref gate in meters 
gInt32 *nRefGateCnt,		//array of refl gates number of VCP tilts 
gInt16  ***parData,		// reflecvitiy data arrays
gUint16 **ppAzTiltAngle,	// azimuth angles of VCP tilts
gInt16 *pTiltElev,		// elevation angles of VCP tilts
QPRDATA*** pOutputQPR,
gUint32* OutputQPRSize,
gUint32* OutputQPRRow,
gUint32* OutputQPRCol,
void*** pOutputHBR,
gUint32* OutputHBRSize,
gUint32* OutputHBRGateCnt
)*/
gInt32 Arith_262_HBR_270_QPR::DoQRate( gInt32 nRay,			//number of radials 
									  gInt32 iRefGateSize,		// length of ref gate in meters 
									  gInt32 *nRefGateCnt,		//array of refl gates number of VCP tilts 
									  /*gInt16  ***parData,*/		// reflecvitiy data arrays
									  StandardRadarData* pstdRData,
									  gUint16 **ppAzTiltAngle,	// azimuth angles of VCP tilts
									  gInt16 *pTiltElev,		// elevation angles of VCP tilts
									  QPRDATA*** pOutputQPR,
									  gUint32* OutputQPRSize,
									  gUint32* OutputQPRRow,
									  gUint32* OutputQPRCol,
									  void*** pOutputHBR,
									  gUint32* OutputHBRSize,
									  gUint32* OutputHBRGateCnt
									  )
{
	gInt32 bret = 0;
	gInt32 i,j,k;
	//m_bins = 0;

	// reflect gate number of that resized to QRate 
	gInt32 qr_gateCnt, qr_gateLen;
	qr_gateLen = STD_GATELEN;

	//SA/SB/SC/CB/CD为240,CC为250;
	gInt32 Rmax = nRefGateCnt[0] * iRefGateSize/1000 ;
	if( Rmax > 240 ) Rmax = 240;
	qr_gateCnt = Rmax*1000/qr_gateLen;

	*OutputHBRGateCnt = qr_gateCnt;

	// alloc hybrid refl buffer to generate Qrate
	RADIALREF *qr_dbz =  new(::std::nothrow) RADIALREF[nRay];
	RADIALREF **href =  new(::std::nothrow) RADIALREF *[4];

	gInt16* parData = NULL;
	for(k=0;k<4;k++)
		href[k] =  new(::std::nothrow) RADIALREF [nRay];

	// construct refl of the 4 lowest tilts 

	//if(m_bins == 1)
	//{
		for(k=0; k< 4; ++k)
			for(j=0; j< nRay; ++j)
			{
				href[k][j].DeltaAngel = pTiltElev[k];
				href[k][j].AzimuthAngle = ppAzTiltAngle[k][j];
				parData = pstdRData->GetRadarDataR(k,j);

				for(i=0;i<qr_gateCnt;++i)
				{
					gInt16 begingate = i * STD_GATELEN/iRefGateSize;
					/*  if( parData[k][j][i] < -160 )*/
					gInt16 bins = gInt16(STD_GATELEN/iRefGateSize);

					gInt32 sum_bins = 0;
					for(int n = 0; n < bins; n++)
					{
						if( parData[begingate + n]/10 < -160 || j >=pstdRData->GetElevationHeadP(k)->AzimuthNumber\
							|| i>= pstdRData->GetElevationHeadP(k)->ReflectivityGates)

							sum_bins += -320;
						else
							sum_bins += parData[begingate + n]/10;
					}
					href[k][j].ref[i] = sum_bins/bins;
				}
			}
	//}
	// QC to QRate here 
	DoIsolatedCheck(href, nRay, qr_gateCnt);
	DoOutlierCheck(href, nRay, qr_gateCnt);

	//  build hybrid here	
	// save product file as hybrid scan reflectivity 
	// qr_dbz 既是混合扫描的处理的结果?
	BuildHybridScan(href, qr_dbz, nRay, qr_gateCnt, qr_gateLen ) ;

	NEW_ARRAY_2D(gInt16, ptemp, qr_gateCnt, nRay);

	*pOutputHBR = (void**)ptemp;
	*OutputHBRSize = sizeof(gInt16)*nRay*qr_gateCnt;

	for(int y = 0; y < nRay; ++y)
	{
		for(int x = 0; x < qr_gateCnt; ++x)
		{
			ptemp[y][x] = qr_dbz[y].ref[x];
		}
	}

	bret |= 0x1;

	if( SaveQPR(qr_dbz, 
		nRay, 
		qr_gateCnt, 
		qr_gateLen, 
		pOutputQPR,
		OutputQPRSize,
		OutputQPRRow,
		OutputQPRCol))
	{
		bret |= 0x2;
	}


	for(i=0; i<4;i++)	
		delete [] href[i] ;  
	delete [] href;  
	href = NULL;
	delete [] qr_dbz;
	qr_dbz = NULL;
	return bret;
};

void Arith_262_HBR_270_QPR::DoIsolatedCheck(RADIALREF **href, 
											gInt32 nRay,
											gInt32 qr_gates )
{
	gInt16 gate_zmin = 0;
	gInt32 tilt,az,bins;
	gInt32 azLeft = 0, azRight = 0;//, count = 0;

	// gUint8**  mark =  new(::std::nothrow) gUint8*[nRay];

	//for( az=0;az<nRay;az++)
	//    mark[az] =  new(::std::nothrow) gUint8 [qr_gateCnt] ;

	for(tilt=0;tilt<4;tilt++)
	{
		//for( az=0;az<nRay;az++)
		//memset(mark[az],0x00,qr_gateCnt);

		for(az=0;az<nRay;az++)
		{            
			if( az == 0 ) 
				azLeft = nRay - 1;
			else 
				// 前一个Az角
				azLeft = az-1;

			if( az == nRay - 1) 
				azRight = 0;
			else 
				// 下一个Az角
				azRight = az+1;

			for( bins = 1; bins < qr_gates - 1; bins++)
			{
				// tilt 面az 角的第bins库的基本反射率
				if( href[tilt][az].ref[bins] > gate_zmin - 1 ) 
				{	
					//判断八邻库中大于gate_zmin-1的个数是否少于3
					if( IsIsolated( href, tilt, azLeft, az, 
						azRight, bins, gate_zmin) < 3) 
					{
						/*
						mark[az][bins]=1;
						count++;
						*/
						// if(mark[az][bins] == 1 )
						href[tilt][az].ref[bins] = -320;
					}
				}
			}
		}

	}

};

gInt32 Arith_262_HBR_270_QPR::IsIsolated( RADIALREF **href,
										 gInt32 tilt, 
										 gInt32 azLeft,
										 gInt32 az, 
										 gInt32 azRight,
										 gInt32 bin,
										 gInt16 gate_zmin)
{
	gInt32 i,neighbor_count = 0;

	//判断八邻库中大于gate_zmin-1的个数是否少于3
	for( i = -1;i <= 1;i++)
	{
		if( href[tilt][azLeft].ref[bin + i]> gate_zmin-1 ) 
		{
			neighbor_count++;
		}
		if( href[tilt][azRight].ref[bin + i]> gate_zmin - 1 ) 
		{
			neighbor_count++;
		}

		// 跳过对az角bin库的检测
		if( (i != 0) && href[tilt][az].ref[bin + i]> gate_zmin-1) 
		{
			neighbor_count++;
		}
	}

	return neighbor_count;
};


void Arith_262_HBR_270_QPR::DoOutlierCheck( RADIALREF **href, 
										   gInt32 nRay,
										   gInt32 qr_gates )
{
	gInt16 gate_zmax = 70*10; //70dBZ, scale: 10
	gInt32 tilt,az,bins;
	gInt32 azLeft = 0, azRight = 0, count=0;

	for(tilt = 0;tilt < 4; tilt++)
	{
		count = 0 ;
		//for( az=0;az<nRay;az++)
		//    memset(Mark[az],0x00,qr_gateCnt);

		for(az = 0;az < nRay; ++az)
		{
			if( az==0) azLeft=nRay-1;
			else azLeft = az - 1;

			if(  az== nRay-1) azRight=0;
			else azRight=az+1;

			for( bins = 1;bins < qr_gates - 1; ++bins)
			{
				if( href[tilt][az].ref[bins]>=gate_zmax) 
				{	
					count++;
					//判断八邻库中小于gate_zmax的个数是否为8，
					// 可能过于严厉
					if( IsSpuriousNoise(href, tilt, azLeft, az, \
						azRight, bins,gate_zmax) == 8) 
					{
						// Mark[az][bins]=1;
						RemoveNoise(href, nRay,tilt, az,bins);
					}
					else 
						// Mark[az][bins]=2;
						href[tilt][az].ref[bins]=-320;
				}
			}
		}

	}

}

gInt32  Arith_262_HBR_270_QPR::IsSpuriousNoise(RADIALREF **href,
											   gInt32 tilt, 
											   gInt32 azLeft,
											   gInt32 az, 
											   gInt32 azRight,
											   gInt32 bin,
											   gInt16 gate_zmax)
{
	gInt32 i,neighbor_count = 0;

	//判断八邻库中小于gate_zmax的个数
	for( i=-1;i<=1;i++)
	{
		if( href[tilt][azLeft].ref[bin + i] < gate_zmax) 
		{
			neighbor_count++;
		}
		if( href[tilt][azRight].ref[bin + i] < gate_zmax) 
		{
			neighbor_count++;
		}
		if( (i != 0) && href[tilt][az].ref[bin + i] < gate_zmax) 
		{
			neighbor_count++;
		}
	}

	return neighbor_count;
}

void Arith_262_HBR_270_QPR::RemoveNoise(RADIALREF **href,
										gInt32 nRay,
										gInt32 tilt,
										gInt32 az,
										gInt32 bin)
{
	gInt32 i, azLeft, azRight;
	gDouble refl=0.0, dBZ_avg;

	if( az == 0) azLeft = nRay - 1;
	else azLeft = az - 1;

	if( az == nRay-1) azRight=0;
	else azRight = az + 1;

	refl = 0.0;

	for( i= -1;i <= 1; ++i)
	{
		//Volume->data[tilt][azLeft].Z[bin+i] 
		refl += pow(10,(href[tilt][azLeft].ref[bin+i])/20.0);

		//Volume->data[tilt][az].Z[bin+i]
		if(i!= 0) refl += pow(10,href[tilt][az].ref[bin+i]*0.01);


		//Volume->data[tilt][azRight].Z[bin+i] 
		refl += pow(10,href[tilt][azRight].ref[bin+i]*0.01);
	}

	// Average reflectivity.
	refl = refl/8;

	// dBZ average value.
	dBZ_avg = 10.0*log10(refl);

	if( dBZ_avg >= -16 )
		href[tilt][az].ref[bin] = (gInt16)(dBZ_avg*10+0.5);
	else
		href[tilt][az].ref[bin] = -320;

	return ;
};

void Arith_262_HBR_270_QPR::BuildHybridScan( RADIALREF **href,
											RADIALREF *qr_dbz,
											gInt32 nRay,
											gInt32 qr_gates,
											gInt32 qr_gatelen)
{
	gInt32 n,i,j,k;
	gInt32 rgate[5];

	n = ComputeLowestElevLayerArea(href,nRay) ;

	//  不同方位、不同仰角考虑不同范围段,并订正到水平面上 
	//构造混合平面参数
	rgate[0]=qr_gates;
	rgate[1]=40000/qr_gatelen;
	rgate[2]=30000/qr_gatelen;
	rgate[3]=15000/qr_gatelen;
	rgate[4]=0 ; // simplify model

	if(n==2)
	{
		for(j=0;j<nRay;++j)
		{
			qr_dbz[j].AzimuthAngle = href[1][j].AzimuthAngle;
			qr_dbz[j].DeltaAngel = href[1][j].DeltaAngel;

			for(i=rgate[0]; i>=rgate[2]; i--)
				qr_dbz[j].ref[i]=href[1][j].ref[i];
		}
	}
	else 
	{  
		for(j = 0;j< nRay; j++)
		{
			qr_dbz[j].AzimuthAngle = href[0][j].AzimuthAngle;

			qr_dbz[j].DeltaAngel = href[0][j].DeltaAngel;

			for( i = rgate[0]; i>(rgate[2]+rgate[0])/2; i--)
			{
				if(href[0][j].ref[i] > href[1][j].ref[i]+140) 
					qr_dbz[j].ref[i] = href[1][j].ref[i];
				else
					qr_dbz[j].ref[i]=max(href[0][j].ref[i], href[1][j].ref[i]);
			}

			for( i = (rgate[2]+rgate[0])/2; i >= rgate[2]; i--)
			{
				if(href[0][j].ref[i]>href[1][j].ref[i]+70) 
					qr_dbz[j].ref[i]=href[1][j].ref[i];
				else
					qr_dbz[j].ref[i]=max(href[0][j].ref[i], href[1][j].ref[i]);
			}
		}
	}

	for( k=2;k<4;k++)
	{
		for(j=0;j < nRay;j++)
			for( i = rgate[k]-1; i>=rgate[k+1]; i--)
				if(href[1][j].ref[i]>100)
					qr_dbz[j].ref[i] = href[k][j].ref[i];
				else 
					qr_dbz[j].ref[i] =  href[1][j].ref[i];
	}

	DoMedianPolarBuf(qr_dbz, nRay, qr_gates) ;

	for(j=0;j<nRay;j++)
		for( i=0; i<qr_gates; i++)
		{
			if( qr_dbz[j].ref[i] < -100 )
				qr_dbz[j].ref[i]=RADAR_DATA_NOECHO;
		}
		return ;
};

gInt32  Arith_262_HBR_270_QPR::ComputeLowestElevLayerArea( RADIALREF **href, 
														  gInt32 nRay)
{
	gInt32 minAPR=51;
	gInt32 minArea=600;
	gInt32 minZAV=10;
	gInt32 RInner=40;
	gInt32 ROuter=150;
	gInt32 Zmin=-50;

	gInt32 nCnt=0,APR=0, rVal=0;
	gDouble ALE=0.0, ASL=0.0, ASVn=0.0,ZAVlow=0.0; 
	gInt32 az,bins;

	for(az=0;az<nRay;az++)
		for(bins=RInner;bins<ROuter;bins++)
		{
			if( href[0][az].ref[bins]>=Zmin)
			{
				ASVn=2*M_PI*bins/360.0;
				ALE += ASVn;
				ZAVlow += ASVn*href[0][az].ref[bins]*0.1;
				if( href[1][az].ref[bins]<Zmin) ASL += ASVn;
				nCnt++;
			}
		}

		if(ALE>minArea)
		{
			gInt32 AVZlow = (gInt32)(ZAVlow/ALE)+1;
			if( ZAVlow>minZAV )
			{
				APR = (gInt32)(ASL/ALE*100)+1 ;
				if( APR > minAPR ) rVal=2;
				else rVal=3;
			}
			else
				rVal=3;
		}
		else rVal=3 ;

		return rVal;
}

void Arith_262_HBR_270_QPR::DoMedianPolarBuf( RADIALREF *qr_dbz, 
											 gInt32 RadialNums, 
											 gInt32 bins)
{
	gInt32 i,j;
	gInt16 **tmpbuf =  new(::std::nothrow) gInt16 * [RadialNums + 2] ;

	for(i=0;i<RadialNums+2;i++) tmpbuf[i] =  new(::std::nothrow) gInt16 [bins+2] ;

	for(j=0;j<RadialNums;j++)
		for(i=0;i<bins;i++)
			tmpbuf[j+1][i+1] = qr_dbz[j].ref[i];

	for(j=0;j<RadialNums;j++)
	{
		tmpbuf[j+1][0] = qr_dbz[j].ref[0];
		tmpbuf[j+1][bins+1] = qr_dbz[j].ref[bins-1];
	}

	for(i=0;i<=bins;i++)
	{
		tmpbuf[0][i] = qr_dbz[RadialNums-1].ref[i];
		tmpbuf[RadialNums+1][i] = qr_dbz[0].ref[i];
	}

	//4 corner
	tmpbuf[0][0] = qr_dbz[0].ref[0]; //left;
	tmpbuf[0][bins+1] = qr_dbz[0].ref[bins-1]; //right;
	tmpbuf[RadialNums+1][0] = qr_dbz[RadialNums-1].ref[0]; //left;
	tmpbuf[RadialNums+1][bins+1] = qr_dbz[RadialNums-1].ref[bins-1]; //right;

	gInt16 *Windowbuf =  new(::std::nothrow) gInt16[16] ; //十字

	for(j=1;j<=RadialNums;j++)
		for(i=1;i<=bins;i++)
		{
			Windowbuf[0]=tmpbuf[j-1][i-1];
			Windowbuf[1]=tmpbuf[j-1][i];
			Windowbuf[2]=tmpbuf[j-1][i+1];
			Windowbuf[3]=tmpbuf[j][i-1];
			Windowbuf[4]=tmpbuf[j][i];
			Windowbuf[5]=tmpbuf[j][i+1];
			Windowbuf[6]=tmpbuf[j+1][i-1];
			Windowbuf[7]=tmpbuf[j+1][i];
			Windowbuf[8]=tmpbuf[j+1][i+1];

			qr_dbz[j-1].ref[i-1] = DoMedianWindow(Windowbuf,9);  
		}
		delete [] Windowbuf;
		Windowbuf = NULL;
		for( i=0;i<RadialNums+2;i++) delete [] tmpbuf[i];
		delete [] tmpbuf;
		tmpbuf = NULL;
}

gInt16 Arith_262_HBR_270_QPR::DoMedianWindow( gInt16*Windowbuf, 
											 gInt32  nsize)
{
	gInt32 s,t;
	gInt16 a = 0;

	t=0;
	for(s = 0;s < nsize; s++)
	{
		if(Windowbuf[s]>100) t++;
	}

	// Center pointer of the windows.
	if(t > nsize/2 && Windowbuf[nsize/2] > 100)
		return Windowbuf[nsize/2];

	for( s = 1; s < nsize; s++)
	{
		a = Windowbuf[s];
		t = s-1;
		while((t >= 0)&& (Windowbuf[t]>a) )
		{
			Windowbuf[t+1]=Windowbuf[t];
			t--;
		}
		Windowbuf[t+1]=a;
	}

	if( nsize&1)
		a = Windowbuf[nsize/2];
	else
		a =(short)((Windowbuf[nsize/2-1]+Windowbuf[nsize/2])/2);

	return a;
}
bool Arith_262_HBR_270_QPR::SaveHBR(RADIALREF *qr_dbz, 
									gInt32 nRay, 
									gInt32 qr_gates, 
									gInt32 qr_gatelen,
									void**  pOutput,
									gUint32* OutputSize)
{
	return true;
};

bool Arith_262_HBR_270_QPR::SaveQPR(RADIALREF *qr_dbz, 
									gInt32 nRay, 
									gInt32 qr_gateCnt, 
									gInt32 qr_gatelen,
									QPRDATA***  pOutput,
									gUint32* OutputSize,
									gUint32* OutputRow,
									gUint32* OutputCol)
{
	// raster 
	gInt16 InitValue = RADAR_DATA_NOECHO;
	gInt32 rw,rh;
	gInt32 i,j;

	rw = rh = qr_gateCnt*2;

	NEW_ARRAY_2D(GRID_DATA, Hydrobuf, rw, rh);

	for( j=0; j<rh;j++)
		for( i=0; i<rw; i++)
			Hydrobuf[j][i].data = InitValue;

	ConvertRadialToGrid(Hydrobuf, 
		rw, rh, 
		qr_dbz, nRay,
		qr_gatelen,
		0, qr_gateCnt-1,              
		rw/2, rh/2, // 原点位置
		1.0);

	/*CalcGridRes(Hydrobuf, rw, rh);*/

	if(*pOutput != NULL) delete [] pOutput;

	*pOutput =  new(::std::nothrow) QPRDATA*[rh];
	for(int i = 0; i < rh; ++i)
	{
		(*pOutput)[i]=  new(::std::nothrow) QPRDATA[rw];
	}

	NEW_ARRAY_2D(gInt16, pBuf, rw, rh);

	QPRDATA** pQPRData = (QPRDATA**)(*pOutput);

	for(gInt32 m = 0; m < rh; ++m)
	{
		for(gInt32 n = 0; n < rw; ++n)
		{
			pQPRData[m][n].packet_code = 0x0B00;
			pQPRData[m][n].row_num = m;
			pQPRData[m][n].col_num = n;
			pQPRData[m][n].QPR = Hydrobuf[m][n].data;
			pQPRData[m][n].row_res = Hydrobuf[m][n].dist_y;
			pQPRData[m][n].col_res = Hydrobuf[m][n].dist_x;

			pBuf[m][n] = Hydrobuf[m][n].data;
		}
	}

	DELETE_ARRAY_2D(Hydrobuf, rw, rh);

	Smooth(pBuf, rw, rh, InitValue);

	for(gInt32 m = 0; m < rh; ++m)
	{
		for(gInt32 n = 0; n < rw; ++n)
		{
			pQPRData[m][n].QPR = pBuf[m][n];
		}
	}

	DELETE_ARRAY_2D(pBuf, rw, rh);

	// save product file as QPR here
	*pOutput = pQPRData;
	*OutputSize = sizeof(QPRDATA) * rw * rh;
	*OutputRow = rh;
	*OutputCol = rw;

	return true;
}


// void ConvertRadialToGrid(gInt16**Gridbuf,
GHRESULT Arith_262_HBR_270_QPR::ConvertRadialToGrid(GRID_DATA** GridBuf,
													gInt32 w, 
													gInt32 h,
													RADIALREF *qr_dbz,
													gInt32 nRay,
													gInt32 gateSize,
													gInt32 R1, gInt32 R2,
													gInt32 rx, gInt32 ry,
													double beam_width )
{
	gInt32 j, rk,ri, ix,iy;
	gDouble da,fsin,fcos,fs,fc,fx,fy;
	gDouble paz = 2.0*M_PI/nRay;
	gDouble azSin = 0.0;
	gDouble azCos = 0.0;

	//----------------------------------
	//	外部传入参数ZR_A, ZR_b替换;
	gFloat ZR_A = 300;
	gFloat ZR_b = ( gFloat )1.4;
	//--------------------------------------
#if defined (WIN32)|| defined(WIN64)
#pragma warning(push)
#pragma warning(disable:4996)
#endif
	ZR_A = 300;
	ZR_b = 1.4;
#if defined (WIN32)|| defined(WIN64)
#pragma warning(pop)
#endif

	gInt16 *dBZTable =  new(::std::nothrow) gInt16[470];//edit dBZTable l by ply.20080625

	for (int i=0;i<470;i++)
	{
		dBZTable[i]=RADAR_DATA_NOECHO;
	}
	dBZTable[469]=(gInt16)(pow(10,((539)*0.01-log10(ZR_A))/ZR_b)*100);

	for(j=0; j < nRay;j++)
	{
		azSin = sin(j*paz);
		azCos = cos(j*paz);
		da = j*paz-beam_width/2*M_PI/180.0;

		fsin=sin(da); // Left radial Azimuth Angle Sin value.
		fcos=cos(da); // Left radial Azimuth Angle Cos value

		da += beam_width*M_PI/180.0;
		fs=sin(da); // Right radial Azimuth Angle Sin value.
		fc=cos(da); // Right radial Azimuth Angle Cos value.

		for( rk = 0; rk < R2;rk += 20)
		{
			fx = rk*fs + rx;
			fy = ry - rk*fc;

			for( ri = rk;ri < R2; ri++)
			{
				if( ri >= R1)
				{
					ix = (gInt32)(fx+0.5);
					iy = (gInt32)(fy+0.5);
					if( ix>=0 && ix<w && iy>=0 && iy<h )
					{
						// Set grid dist_x and dist_y.
						//GridBuf[iy][ix].data = dBZTable[qr_dbz[j].ref[ri]+32768];
						int temdbz=qr_dbz[j].ref[ri];
						GridBuf[iy][ix].data=temdbz<70?RADAR_DATA_NOECHO:(temdbz>=539?dBZTable[539-70]:(dBZTable[temdbz-70]!=RADAR_DATA_NOECHO?dBZTable[temdbz-70]:dBZTable[temdbz-70]=(gInt16)(pow(10,((temdbz)*0.01-log10(ZR_A))/ZR_b)*100)));//by ply.
						GridBuf[iy][ix].dist_x = gateSize;
						GridBuf[iy][ix].dist_y = gateSize;
					}
				}
				fx += fsin ;
				fy -= fcos ;
			} //end of for i
		} //end of for k
	}

	delete [] dBZTable ;
	dBZTable = NULL;

	return GS_OK;
};
void Arith_262_HBR_270_QPR::Smooth(gInt16**Ingest, 
								   gInt32 w, 
								   gInt32 h, 
								   gInt16 InitValue)
{
	gFloat fpArray[3][3], fcoef=1.0; //
	//            │1  2  1│
	//  fpArray = │2  4  2│,  fcoef = 1/16
	//            │1  2  1│ 

	fcoef = (gFloat)(1.0/16);
	fpArray[1][1] = 4.0;
	fpArray[2][1] = fpArray[1][2] = fpArray[1][0] = fpArray[0][1] = 2.0;  
	fpArray[2][2] = fpArray[2][0] = fpArray[0][2] = fpArray[0][0] = 1.0; 

	gInt32 i,j;

	NEW_ARRAY_2D(gFloat, ftmp, w, h);

	for( j=0;j<h;++j)
		for( gInt32 i=0;i<w;++i)
			ftmp[j][i] = InitValue;

	for( j=1;j<h-1;j++)
	{
		for( i=1;i<w-1;i++)
		{
			gFloat  fsum=0;
			for(gInt32 jj=-1;jj<=1;jj++)
				for(gInt32 ii=-1;ii<=1;ii++)
					fsum += fpArray[jj+1][ii+1]*Ingest[j+jj][i+ii] ;
			ftmp[j][i] = fsum*fcoef;
		}
	}
	for( j=1;j<h-1;j++)
	{
		for( i=1;i<w-1;i++)
		{
			Ingest[j][i] = (gInt16)ftmp[j][i];
		}
	}

	DELETE_ARRAY_2D(ftmp, w, h);
	/*
	for(j=0;j<h;++j)
	delete [] ftmp[j];
	delete [] ftmp;
	*/

	return ;
}

GHRESULT Arith_262_HBR_270_QPR::SaveQPRToProductData(StandardRadarData*	pstdRadar,
													 GRADARDATAHEADER*	pRadarHdr,
													 QPRDATA**		pInput,
													 gUint32		inputSize,
													 gUint32		row,
													 gUint32		col)
{
	m_pOutputList->AddTail();

	Product_270_QPR* pPdt = &(*m_pOutputList->GetTailAs<Product_270_QPR>());

	GHRESULT ConstrutHand;
	ConstrutHand = pPdt->ConstructIt(row, col);
	if(ConstrutHand != GS_OK)
		return (ConstrutHand ==( (ConstrutHand == GE_NOENOUGHMEMORY)?GE_NOENOUGHMEMORY:GE_INVALIDARG));
	PDBlock* pdb = pPdt->GetPDBlock();
	RadarInfor * pRadarInfor = pPdt->GetRadarInfor();

	QPRDATA** pQPR = (QPRDATA**)pInput;

	pdb->ProductCode = 270;
	//pdb->ElevationNumber = -1;
	memcpy(pRadarInfor->RadarType, pRadarHdr->RadarType, sizeof(gInt8) * 4);
	pRadarInfor->Altitude = pRadarHdr->RadarHeight;
	pRadarInfor->Latitude = pRadarHdr->RadarLat;
	pRadarInfor->Longitude = pRadarHdr->RadarLon;
	memcpy(pdb->VCPMode, pRadarHdr->VCPMode, sizeof(gInt8)* 4);
	memcpy(pdb->OperationalMode, pRadarHdr->OperationMode, sizeof(gInt8) * 2);
	pdb->ProductDate = pRadarHdr->VCPDate;
	pdb->ProductTime = pRadarHdr->VCPTime;
	CJulMsGMT::GetLocalJulMs(pdb->GenerationDateOfProduct,
		pdb->GenerationTimeOfProduct);
	memcpy(pRadarInfor->SiteCode, pRadarHdr->SiteCode, sizeof(gInt8)*8);
	//memcpy(pdb->SiteName, pRadarHdr->SiteName, sizeof(gInt8)*18);
	pdb->ScaleOfData = 100; 
	pdb->IndexOfTabular = -1;
	pdb->IndexOfGraphic = -1;

	pPdt->GetLayer()->HeightOfElevation = 0;

	GridHead*	ghd;
	GridRowData*	grd;

	ghd = pPdt->GetGrid_Head();

	//ghd->PacketCode = 0x0B00;
	ghd->NumberOfRows = row;
	ghd->NumberOfCols = col;
	ghd->ResolutionOfRow = STD_GATELEN;
	ghd->ResolutionOfCol = STD_GATELEN;
	//ghd->ScaleOfData = 100;

	gInt16* pData = NULL;
	for(gUint32 i = 0; i < row; ++i)
	{
		grd = pPdt->GetGrid_Row(i);
		grd->SNOfRow = i;

		pData = pPdt->GetGrid_Data(i);
		for(gUint32 j = 0; j < col; ++j)
		{
			pData[j] = pInput[i][j].QPR;
		}
	}

	return GS_OK;
};

GHRESULT Arith_262_HBR_270_QPR::SaveHBRToProductData(StandardRadarData* pstdRadarData,
													 GRADARDATAHEADER* pRadarHdr,
													 void** pInput,
													 gUint32 inputSize,
													 gUint32 nRayCount,
													 gUint32 gateCnt)
{
	m_pOutputList->AddTail();

	Product_262_HBR* pPdt = &(*(m_pOutputList->GetTailAs<Product_262_HBR>()));

	GHRESULT ConstrutHand;
	ConstrutHand = pPdt->ConstructIt(nRayCount, gateCnt);
	if(ConstrutHand != GS_OK)
		return (ConstrutHand ==( (ConstrutHand == GE_NOENOUGHMEMORY)?GE_NOENOUGHMEMORY:GE_INVALIDARG));
	PDBlock* pdb = pPdt->GetPDBlock();
	RadarInfor * pRadarInfor = pPdt->GetRadarInfor();

	pdb->ProductCode = 262;
	//pdb->ElevationNumber = -1;
	memcpy(pRadarInfor->RadarType, pRadarHdr->RadarType, sizeof(gInt8) * 4);
	pRadarInfor->Altitude = pRadarHdr->RadarHeight;
	pRadarInfor->Latitude = pRadarHdr->RadarLat;
	pRadarInfor->Longitude = pRadarHdr->RadarLon;
	memcpy(pdb->VCPMode, pRadarHdr->VCPMode, sizeof(gInt8)* 4);
	memcpy(pdb->OperationalMode, pRadarHdr->OperationMode, sizeof(gInt8) * 2);
	pdb->ProductDate = pRadarHdr->VCPDate;
	pdb->ProductTime = pRadarHdr->VCPTime;
	CJulMsGMT::GetLocalJulMs(pdb->GenerationDateOfProduct,
		pdb->GenerationTimeOfProduct);
	memcpy(pRadarInfor->SiteCode, pRadarHdr->SiteCode, sizeof(gInt8)*8);
	//memcpy(pdb->SiteName, pRadarHdr->SiteName, sizeof(gInt8)*18);
	pdb->ScaleOfData = 10; 
	pdb->IndexOfTabular = -1;
	pdb->IndexOfGraphic = -1;

	pPdt->GetLayer()->HeightOfElevation = 0;

	pPdt->GetRadial_Head()->NumberOfGates = gateCnt ;

	pPdt->GetRadial_Head()->DistanceOfFirstGate = pstdRadarData->GetElevationHeadP(0)->FirstGateRangeOfRef;//add by sunqc 20080815

	//pPdt->GetRadial_Head()->ScaleOfData = 10;

	gInt16** pData = (gInt16**)pInput;

	GELEVATIONDATAHEAD* pElvHdr = pstdRadarData->GetElevationHeadP(0);

	pPdt->GetRadial_Head()->LengthOfGate = STD_GATELEN;
	//pPdt->GetRadial_Head()->ScaleOfData = 10;

	gInt32 total_az_count = pElvHdr->AzimuthNumber;
	// gInt32 gate_count = pElvHdr->ReflectivityGates;

	for(int i = 0; i < total_az_count; ++i)
	{
		pPdt->GetRadial_RadialData(i)->AzimuthAngle = pstdRadarData->GetRadarDataP(0,i)->Azimuth;

		if(i == pElvHdr->AzimuthNumber - 1)
		{
			pPdt->GetRadial_RadialData(i)->DeltaAngle = \
				(pstdRadarData->GetRadarDataP(0, 0)->Azimuth + 36000 - pstdRadarData->GetRadarDataP(0, i)->Azimuth)%36000;
		}
		else
		{
			pPdt->GetRadial_RadialData(i)->DeltaAngle = \
				(pstdRadarData->GetRadarDataP(0, i+1)->Azimuth + 36000 - pstdRadarData->GetRadarDataP(0, i)->Azimuth)%36000;
		}

		memcpy(pPdt->GetRadial_Data(i), (pData[i]), gateCnt*2);
	}

	return GS_OK;
};


////////////////////////////////////////////////////////////////
// Algorithm End
////////////////////////////////////////////////////////////////
