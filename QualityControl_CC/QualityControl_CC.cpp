#ifndef IARITHMS_LIB
	#define IARITHMS_LIB
#endif


#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif

#include <math.h>
#include<vector>
#include<algorithm>

#include "../GDataTypes/GDef.h"
#include "QualityControl_CC.h"

QualityControl_CC::QualityControl_CC()
{
	;/*Initialize();*/
}

QualityControl_CC::~QualityControl_CC()
{
	;
}

GHRESULT QualityControl_CC :: Initialize()
{
	m_pGenerateProductList = NULL;
	m_pParameter			= NULL;
	m_pOutputProductList	= NULL;

	return GS_OK;
}
GHRESULT QualityControl_CC :: UnInitialize()
{
	return GS_OK;
}

GHRESULT QualityControl_CC::LoadData (void *pValue,ReadParam * ReadParameter)
{
	if (pValue == NULL )
	{
		return GE_INVALIDARG;
	}
	//if ( ReadParameter == NULL )
	//{
	//	return GE_INVALIDARG;
	//}
	m_pGenerateProductList = (GXList< GenerationData > *)pValue;//取得输入数据列表
	m_pParameter = ReadParameter;	//取得参数函数指针

	return GS_OK;
}

GHRESULT QualityControl_CC::Execute()
{
	//------------------------------------------------------------------------------------------
	gInt16 iAzimuthNumber=0,iReflectGates[3],iDopplerGates[3];

	StandardRadarData * pInData = &(*(m_pGenerateProductList->GetHeadAs<StandardRadarData>()));
	GRADARDATAHEADER * pInHead = pInData->GetHeadP();
	m_pOutputProductList->AddTail();
	StandardRadarData * pOutData = &(*(m_pOutputProductList->GetTailAs<StandardRadarData>()));

	GRADARDATAHEADER * pOutHead = new GRADARDATAHEADER;

	pOutHead->ElevationCount =pInHead->ElevationCount;
	pOutHead->RadarHeight = pInHead->RadarHeight;
	pOutHead->RadarLat = pInHead->RadarLat;
	pOutHead->RadarLon = pInHead->RadarLon;
	memcpy(pOutHead->ScanMode, pInHead->ScanMode, 2*sizeof(gInt8));
	memcpy(pOutHead->SiteCode, pInHead->SiteCode, 8*sizeof(gInt8));
	memcpy(pOutHead->RadarType, pInHead->RadarType, 4*sizeof(gInt8));
	memcpy(pOutHead->OperationMode, pInHead->OperationMode, 2*sizeof(gInt8));
	pOutHead->VCPTime = pInHead->VCPTime;
	pOutHead->VCPDate = pInHead->VCPDate;
	memcpy(pOutHead->VCPMode, pInHead->VCPMode, 4*sizeof(gInt8));
	pOutHead->SectorCode = pInHead->SectorCode;
	pOutHead->SizeOfSector = pInHead->SizeOfSector;
	pOutHead->ReflectivityGateSize = pInHead->ReflectivityGateSize;
	pOutHead->DopplerGateSize = pInHead->DopplerGateSize;
	pOutHead->ConstantOfSystemCorrect = pInHead->ConstantOfSystemCorrect;

	GHRESULT gResult;
	if ((gResult = pOutData->WriteHead(pOutHead))!=GS_OK)
	{
		delete pOutHead;
		pOutHead = NULL;
		return gResult;
	}
	delete pOutHead;
	pOutHead = NULL;

	gInt16 ** ref[3];
	gInt16 ** vel[3];
	gInt16 ** spw[3];
	iAzimuthNumber = pInData->GetElevationHeadP(0)->AzimuthNumber;
	for(int k=0;k<3;k++)
	{
		iReflectGates[k] = pInData->GetElevationHeadP(k)->ReflectivityGates;
		iDopplerGates[k] = pInData->GetElevationHeadP(k)->DopplerGates;

		ref[k] = new(::std::nothrow) gInt16*[iAzimuthNumber];
		vel[k] = new(::std::nothrow) gInt16*[iAzimuthNumber];
		spw[k] = new(::std::nothrow) gInt16*[iAzimuthNumber];
		memset(ref[k],0,iAzimuthNumber);
		memset(vel[k],0,iAzimuthNumber);
		memset(spw[k],0,iAzimuthNumber);
		for (int i=0;i<iAzimuthNumber;i++)
		{
			ref[k][i]=new(::std::nothrow) gInt16[iReflectGates[0]];
			vel[k][i]=new(::std::nothrow) gInt16[iDopplerGates[0]];
			spw[k][i]=new(::std::nothrow) gInt16[iDopplerGates[0]];
		}
		for (int i=0;i<iAzimuthNumber;i++)
		{
			for (int j=0;j<iReflectGates[k];j++)
			{
				ref[k][i][j]=pInData->GetRadarDataR(k,i)[j];
			}
			for (int j=0;j<iDopplerGates[k];j++)
			{
				vel[k][i][j]=pInData->GetRadarDataV(k,i)[j];
				spw[k][i][j]=pInData->GetRadarDataW(k,i)[j];
			}
		}
	}
	for(int k=0;k<2;k++)
		ap_CC(pInData,k,ref[k],ref[k+1],vel[k],spw[k]);
	float NOISE_FILTER_THRESHOLD =0.4; //可调参数，0.35~0.45

	gResult=writedata(pInData,pOutData,ref,vel,spw);

	gInt16 **REF = NULL;
	gInt16 **VEL = NULL;
	gInt16 ** SPW = NULL;
	REF = new gInt16*[pOutData->GetElevationHeadP(0)->AzimuthNumber];
	VEL = new gInt16*[pOutData->GetElevationHeadP(0)->AzimuthNumber];
	SPW = new gInt16*[pOutData->GetElevationHeadP(0)->AzimuthNumber];
	for(int i=0; i< pOutData->GetElevationHeadP(0)->AzimuthNumber; i++)
	{
		REF[i] = new gInt16[pOutData->GetElevationHeadP(0)->ReflectivityGates];
		VEL[i] = new gInt16[pOutData->GetElevationHeadP(0)->DopplerGates];
		SPW[i] = new gInt16[pOutData->GetElevationHeadP(0)->DopplerGates];
		memset(REF[i],0,sizeof(gInt16)*pOutData->GetElevationHeadP(0)->ReflectivityGates);
		memset(VEL[i],0,sizeof(gInt16)*pOutData->GetElevationHeadP(0)->DopplerGates);
		memset(SPW[i],0,sizeof(gInt16)*pOutData->GetElevationHeadP(0)->DopplerGates);
	}
	for( int k =0; k< pInData->GetHeadP()->ElevationCount;k++)
	{
		for(int i=0; i< pOutData->GetElevationHeadP(0)->AzimuthNumber; i++)
		{
			for(int j =0;j<pOutData->GetElevationHeadP(k)->ReflectivityGates;j++)
				REF[i][j] = pOutData->GetRadarDataR(k,i)[j];

			for(int j =0;j<pOutData->GetElevationHeadP(k)->DopplerGates;j++)
			{
				VEL[i][j] = pOutData->GetRadarDataV(k,i)[j];
				SPW[i][j] = pOutData->GetRadarDataW(k,i)[j];
			}
		}
		sun_beam_remove(REF,VEL, SPW,iAzimuthNumber, pOutData->GetElevationHeadP(k)->ReflectivityGates); 
		noise_remove(REF,VEL, SPW, iAzimuthNumber,pOutData->GetElevationHeadP(k)->ReflectivityGates, 0, pOutData->GetElevationHeadP(k)->ReflectivityGates, NOISE_FILTER_THRESHOLD,pInData->GetHeadP()->ReflectivityGateSize);
		for(int i=0; i< pOutData->GetElevationHeadP(0)->AzimuthNumber; i++)
		{
			for(int j =0;j<pOutData->GetElevationHeadP(k)->ReflectivityGates;j++)
				pOutData->GetRadarDataR(k,i)[j] = REF[i][j];

			for(int j =0;j<pOutData->GetElevationHeadP(k)->DopplerGates;j++)
			{
				gInt16 DopperGates = pOutData->GetElevationHeadP(k)->DopplerGates;
				pOutData->GetRadarDataV(k,i)[j] = VEL[i][j];
				pOutData->GetRadarDataW(k,i)[j] = SPW[i][j] ;
			}
		}
	}

	for(int i =0; i <pOutData->GetElevationHeadP(0)->AzimuthNumber;i++ )
	{
		delete []REF[i];
		delete []VEL[i];
		delete []SPW[i];
	}
	delete []REF;
	delete []VEL;
	delete []SPW;
	//noise_filter(pInData);


	for (int k=0;k<3;k++)
	{
		for (int i=0;i<iAzimuthNumber;i++)
		{
			delete [] ref[k][i];
			delete [] vel[k][i];
			delete [] spw[k][i];
		}
		delete [] ref[k];
		delete [] vel[k];
		delete [] spw[k];
	}

	delete [] ref;
	delete [] vel;
	delete [] spw;

	return GS_OK;
}
GHRESULT QualityControl_CC::ap_CC(StandardRadarData* pInData,gInt16 iElevIndex,gInt16 ** ref,gInt16 ** ref2,gInt16 ** vel, gInt16 ** spw)
{
	gInt16 clutter,clearair,seaclutter;
	gFloat dazim,thresh_ap;
	gInt32 i2,ir,iv,jj,mgateref,mazimref,numthreshref;
	gInt32 m0,n0,mm,nn;
	mem_para mem_para_ap,mem_para_cl,mem_para_sea;
	gInt16 nptve,mgateve,mazimve,iminrgve,numthreshve,iminrgref;

	mem_para_ap.mdve[0]=-23;
	mem_para_ap.mdve[1]=0;
	mem_para_ap.mdve[2]=23;

	//速度谱宽的区域平均值
	mem_para_ap.mdsw[0]=-1000;
	mem_para_ap.mdsw[1]=0;
	mem_para_ap.mdsw[2]=32;
	//方差
	mem_para_ap.sdve[0]=-1000;
	mem_para_ap.sdve[1]=0;
	mem_para_ap.sdve[2]=10;
	//回波强度的纹理
	mem_para_ap.tdbz[0]=0;
	mem_para_ap.tdbz[1]=45;
	mem_para_ap.tdbz[2]=10000;
	//沿径向的库间变化程度!
	mem_para_ap.spin[0]=0;
	mem_para_ap.spin[1]=50;
	mem_para_ap.spin[2]=100;
	//沿径向方向的变号!
	mem_para_ap.sign[0]=-60;
	mem_para_ap.sign[1]=0;
	mem_para_ap.sign[2]=60;
	// 垂直变化
	mem_para_ap.gdz[0]=-9000;
	mem_para_ap.gdz[1]=-150;
	mem_para_ap.gdz[2]=0.0;
	//****for clear air echo *******************
	mem_para_cl.tve[0]=0;
	mem_para_cl.tve[1]=100;
	mem_para_cl.tve[2]=150;

	mem_para_cl.mdsw[0]=00;
	mem_para_cl.mdsw[1]=30;
	mem_para_cl.mdsw[2]=100;

	mem_para_cl.tdbz[0]=0;
	mem_para_cl.tdbz[1]=2000;
	mem_para_cl.tdbz[2]=6000;

	mem_para_cl.mdz[0]=-125;
	mem_para_cl.mdz[1]=-5;
	mem_para_cl.mdz[2]=-300;

	mem_para_cl.spin[0]=0;
	mem_para_cl.spin[1]=10;
	mem_para_cl.spin[2]=1000;

	mem_para_cl.sign[0]=0;
	mem_para_cl.sign[1]=70;
	mem_para_cl.sign[2]=100;
	// for Sea Clutter

	mem_para_sea.mdsw[0]=-1000;
	mem_para_sea.mdsw[1]=0;
	mem_para_sea.mdsw[2]=60;

	mem_para_sea.sdve[0]=-1000;
	mem_para_sea.sdve[1]=0;
	mem_para_sea.sdve[2]=7;

	mem_para_sea.tdbz[0]=0;
	mem_para_sea.tdbz[1]=5500;
	mem_para_sea.tdbz[2]=10000;

	mem_para_sea.spin[0]=40;
	mem_para_sea.spin[1]=60;
	mem_para_sea.spin[2]=1000;

	mem_para_sea.gdz[0]=-9000;
	mem_para_sea.gdz[1]=-80;
	mem_para_sea.gdz[2]=0;
	gFloat a1 = 55;

	mgateref=5;
	mazimref=5;
	mgateve=9;
	mazimve=5;

	numthreshref=(mgateref*mazimref)/3.0;
	numthreshve=(mgateve*mazimve)/3.0;
	iminrgref=1+mgateref;
	//	nrangref=230;
	iminrgve=1+mgateve;
	//	nrangve=ng-mgateve;

	gInt32 dbzthresh=460,spval,ddbz,ddmax,mbsh_s;

	gInt16 id,jd;

	gFloat r21,ifact;

	dazim=1.0;

	gInt16 iAziCount = pInData->GetElevationHeadP(iElevIndex)->AzimuthNumber;
	gInt16 iRefGates = pInData->GetElevationHeadP(iElevIndex)->ReflectivityGates;
	gInt16 iDopGates = pInData->GetElevationHeadP(iElevIndex)->DopplerGates;

	gInt16 ** mdsw= new(::std::nothrow) gInt16*[iDopGates];
	gInt16 ** mdve= new(::std::nothrow) gInt16*[iDopGates];
	gInt16 ** sdevv= new(::std::nothrow) gInt16*[iDopGates];
	gFloat ** tve= new(::std::nothrow) gFloat*[iDopGates];

	gInt16 ** mdz1= new(::std::nothrow) gInt16*[iRefGates];
	gInt16 ** mdz2= new(::std::nothrow) gInt16*[iRefGates];
	gInt16 ** spin= new(::std::nothrow) gInt16*[iRefGates];
	gInt16 ** sign= new(::std::nothrow) gInt16*[iRefGates];
	gFloat ** tdbz= new(::std::nothrow) gFloat*[iRefGates];

	gInt16 ** mbsh_num_ve= new(::std::nothrow) gInt16*[iRefGates];
	gInt16 ** gdzz= new(::std::nothrow) gInt16*[iRefGates];
	gInt16 ** mbsh_out= new(::std::nothrow) gInt16*[NGATE];

	for (int i=0;i<iDopGates;i++)
	{
		mdsw[i]=new(::std::nothrow) gInt16[iAziCount];//!
		mdve[i]=new(::std::nothrow) gInt16[iAziCount];//!
		sdevv[i]=new(::std::nothrow) gInt16[iAziCount];//!
		tve[i]=new(::std::nothrow) gFloat[iAziCount];//!
		memset(mdsw[i],0,iAziCount);
		memset(mdve[i],0,iAziCount);
		memset(sdevv[i],0,iAziCount);
		memset(tve[i],0,iAziCount);
	}

	for (int i=0;i<iRefGates;i++)
	{
		mdz1[i]=new(::std::nothrow) gInt16[iAziCount];//!
		mdz2[i]=new(::std::nothrow) gInt16[iAziCount];//!
		spin[i]=new(::std::nothrow) gInt16[iAziCount];//!
		sign[i]=new(::std::nothrow) gInt16[iAziCount];//!
		tdbz[i]=new(::std::nothrow) gFloat[iAziCount];//!
		mbsh_num_ve[i]=new(::std::nothrow) gInt16[iAziCount];//!
		gdzz[i]=new(::std::nothrow) gInt16[iAziCount];
		memset(mdz1[i],0,iAziCount);
		memset(mdz2[i],0,iAziCount);
		memset(spin[i],0,iAziCount);
		memset(sign[i],0,iAziCount);
		memset(tdbz[i],0,iAziCount);
		memset(mbsh_num_ve[i],0,iAziCount);
		memset(gdzz[i],0,iAziCount);
	}


	for (int i=0;i<NGATE;i++)
	{
		mbsh_out[i]=new(::std::nothrow) gInt16[NAZIM];
		memset(mbsh_out[i],0,NAZIM);
	}


	//! calculate of feature fields
	//! median of spw,vel and ref   mdsw

	//TurnTO360(ref,pInData,0, R);
	//TurnTO360(ref2,pInData,1, R);
	//TurnTO360(vel,pInData,0, V);
	//TurnTO360(spw,pInData,0, W);
///中值滤波，按同一径向上前后各四个点，共九个点，排序后取中间值
	median(spw,iDopGates,iAziCount,mdsw);

	median(vel,iDopGates,iAziCount,mdve);

	median(ref,iRefGates,iAziCount,mdz1);

	median(ref2,iRefGates,iAziCount,mdz2);
//利用每个点周围25个点，求每个点的均方差，存sdevv里面
	sdt_dev_ve(vel,mdve,iAziCount,iDopGates,mgateve,mazimve,numthreshve,sdevv);

	dbzpara(ref,iAziCount,iRefGates,mgateref,mazimref,dbzthresh,numthreshref,tdbz,sign,spin);

	ttve(vel,tve,iAziCount,iminrgve,iRefGates,mgateve,mazimve,numthreshve);


	r21=cos(M_PI/360.0)/cos(1.5*M_PI/180.0);//含义？？？？？？？？？？？？？？

	//do j=1,iAziCount

	for(int j=0;j<iAziCount;j++)
	{
		for(int i=2;i<iRefGates;i++)
		{
			i2=int(i*r21+0.5);
			gdzz[i][j]=RADAR_DATA_NOECHO;
			if(ref[j][i]>RADAR_DATA_NOECHO)
			{
				if(ref2[j][i2]>RADAR_DATA_NOECHO)
				{
					ddmax=-1000 ;
					for( m0=-4;m0<=4;m0++)
					{
						for( n0=-2;n0<=2;n0++)
						{
							mm=i2+m0;
							nn=n0+j;
							if(mm>=1 && mm<iRefGates && nn >= 1 && nn <= iAziCount)
							{
								ddbz=mdz2[mm][nn]-mdz1[i][j];
								if(ddbz > ddmax)
									ddmax=ddbz;
							}
						}
					}
					ddbz=ddmax;
				}
				else
				{
					ddbz=10-ref[j][i];
				}
				if (i < 160)
				{
					ifact=1;
				}
				else
				{
					ifact=(300.0-i/4.0)/(300.0-40.0);
				}
				ddbz=ddbz*ifact;
				gdzz[i][j]=ddbz;
			}
		}
	}

	/////////////////////////////////////
	for(int j=0;j<iAziCount;j++)// j=1,iAziCount
	{
		for(int i=2;i<iRefGates;i++)//do i=2,a3
		{

			iv = i;
			mbsh_num_ve[i][j]=0;
			mbsh_out[i][j]=0.0;
			if(ref[j][i]>-50)
			{
				if(iv< iDopGates)
				{
					if(sdevv[iv][j]!= RADAR_DATA_NOECHO)
					{
						mbsh_s=mfun(mem_para_ap.sdve,sdevv[iv][j]);
						mbsh_out[i][j]=mbsh_out[i][j]+mbsh_s;
						mbsh_num_ve[i][j]=mbsh_num_ve[i][j]+1;
					}
					if(mdve[iv][j] != RADAR_DATA_NOECHO)
					{
						mbsh_s=mfun(mem_para_ap.mdve,mdve[iv][j]);
						mbsh_out[i][j]=mbsh_out[i][j]+mbsh_s;
						mbsh_num_ve[i][j]=mbsh_num_ve[i][j]+1;
					}
					if(mdsw[iv][j]!=RADAR_DATA_NOECHO)
					{
						mbsh_s=mfun(mem_para_ap.mdsw,mdsw[iv][j]);
						mbsh_out[i][j]=mbsh_out[i][j]+mbsh_s;
						mbsh_num_ve[i][j]=mbsh_num_ve[i][j]+1;
					}
				}

				if(gdzz[i][j]!=RADAR_DATA_NOECHO)
				{
					mbsh_s=mfun(mem_para_ap.gdz,gdzz[i][j]);
					mbsh_out[i][j]=mbsh_out[i][j]+mbsh_s;
					mbsh_num_ve[i][j]=mbsh_num_ve[i][j]+1;
				}
				if(tdbz[i][j]!=RADAR_DATA_NOECHO)
				{
					mbsh_s=mfun_r(mem_para_ap.tdbz,tdbz[i][j]);
					mbsh_out[i][j]=mbsh_out[i][j]+mbsh_s;
					mbsh_num_ve[i][j]=mbsh_num_ve[i][j]+1;
				}
				if(spin[i][j]!=RADAR_DATA_NOECHO)
				{
					mbsh_s=mfun(mem_para_ap.spin,spin[i][j]);
					mbsh_out[i][j]=mbsh_out[i][j]+mbsh_s;
					mbsh_num_ve[i][j]=mbsh_num_ve[i][j]+1;
				}

				if(mbsh_num_ve[i][j] >=1)
				{
					mbsh_out[i][j]=mbsh_out[i][j]/mbsh_num_ve[i][j];
				}
				else
				{
					mbsh_out[i][j]=0;
				}
			}
		}
	}

	for(int j=0;j<iAziCount;j++)//do j=1,iAziCount
	{
		for(int i=1;i<iRefGates;i++) //do i=2,a3
		{
			iv = i*4;
			thresh_ap = 50;
			if(i < 230)
			{
				if(abs(vel[j][iv])> 20.0 && abs(vel[j][iv])<200.0)
					thresh_ap=10;
				if(mbsh_out[i][j]>thresh_ap)
				{
					for(jd=-1;jd<=1;jd++)
					{
						jj=j+jd;
						for(id=-1;id<1;id++)
						{
							if(jj>=iAziCount)
								jj=0;
							if(jj<0)
								jj=iAziCount-1;
							if (ref[jj][i+id]>RADAR_DATA_NOECHO)
							{
								ref[jj][i+id]=RADAR_DATA_NOECHO;
							}
						}
					}
					if(iv<=iDopGates-5)
					{   //do jd=-1,1
						for(jd=-1;jd<=1;jd++)
							jj=j+jd;
						for(id=-4;id<=4;id++)
						{
							if(jj>=iAziCount)
								jj=0;
							if(jj<0)
								jj=iAziCount-1;
							if (vel[jj][iv+id]>RADAR_DATA_NOECHO)
							{
								vel[jj][iv+id]=RADAR_DATA_NOECHO;
							}
							if (spw[jj][iv+id]>RADAR_DATA_NOECHO)
							{
								spw[jj][iv+id]=RADAR_DATA_NOECHO;
							}
						}
					}
				}
			}
		}
	}

////////////////////////////////////////////////////

	//!    clear air echo distinguish
	for(int j=0;j<iAziCount;j++)//		do j=1,iAziCount
	{
		for(int i=1;i<iRefGates;i++)//do i=2,iRefGates
		{
			ir=(i+2)/4;
			mbsh_num_ve[i][j]=0;
			mbsh_out[i][j]=0.0;
			if(ref[j][ir]<10.0)
			{
				if(tve[i][j] != RADAR_DATA_NOECHO)
				{
					mbsh_s=mfun_r(mem_para_cl.tve,tve[i][j]);
					mbsh_out[i][j]=mbsh_out[i][j]+mbsh_s;
					mbsh_num_ve[i][j]=mbsh_num_ve[i][j]+1;
				}
				if(spw[j][i] != RADAR_DATA_NOECHO)
				{
					mbsh_s=mfun(mem_para_cl.mdsw,spw[j][i]);
					mbsh_out[i][j]=mbsh_out[i][j]+mbsh_s;
					mbsh_num_ve[i][j]=mbsh_num_ve[i][j]+1;
				}
				if(tdbz[ir][j]!=RADAR_DATA_NOECHO)
				{
					mbsh_s=mfun_r(mem_para_cl.tdbz,tdbz[ir][j]);
					mbsh_out[i][j]=mbsh_out[i][j]+mbsh_s;
					mbsh_num_ve[i][j]=mbsh_num_ve[i][j]+1;
				}
				if(ref[j][ir]!=RADAR_DATA_NOECHO)
				{
					mbsh_s=mfun(mem_para_cl.mdz,ref[j][ir]);
					mbsh_out[i][j]=mbsh_out[i][j]+mbsh_s;
					mbsh_num_ve[i][j]=mbsh_num_ve[i][j]+1;
				}
				if(spin[ir][j]!=RADAR_DATA_NOECHO)
				{
					mbsh_s=mfun(mem_para_cl.spin,spin[ir][j]);
					mbsh_out[i][j]=mbsh_out[i][j]+mbsh_s;
					mbsh_num_ve[i][j]=mbsh_num_ve[i][j]+1;
				}
				if(sign[ir][j]!=RADAR_DATA_NOECHO)
				{
					mbsh_s=mfun(mem_para_cl.sign,sign[ir][j]);
					mbsh_out[i][j]=mbsh_out[i][j]+mbsh_s;
					mbsh_num_ve[i][j]=mbsh_num_ve[i][j]+1;
				}
				if(mbsh_num_ve[i][j]>1)
					mbsh_out[i][j]=mbsh_out[i][j]/mbsh_num_ve[i][j];
			}
		}
	}
	//!set the Clear Air echo to spval

	for(int j=0;j<iAziCount;j++)
	{
		for(int i=8;i<=iDopGates-8;i++)
		{
			ir=(i+2)/4+1;
			thresh_ap=0.2;
			if(vel[j][i]>RADAR_DATA_NOECHO)
				thresh_ap=0.1;
			if(mbsh_out[i][j]>thresh_ap)
			{
				for(jd=-1;jd<=1;jd++)
				{
					jj=j+jd;
					for(id=-1;id<=1;id++)
					{
						if(jj>=iAziCount)
							jj=0;
						if(jj<0)
							jj=iAziCount-1;

						if (ref[jj][ir+id]>RADAR_DATA_NOECHO)
						{
							ref[jj][ir+id]=RADAR_DATA_NOECHO;
						}
					}
				}
				for(jd=-1;jd<=1;jd++)
				{
					jj=jd+j;
					for(id=-4;id<=4;id++)
					{
						if(jj>=iAziCount)
							jj=0;
						if(jj<0)
							jj=iAziCount-1;
						if (vel[jj][i+id]>RADAR_DATA_NOECHO)
						{
							vel[jj][i+id]=RADAR_DATA_NOECHO;
						}
						if (spw[jj][i+id]>RADAR_DATA_NOECHO)
						{
							spw[jj][i+id]=RADAR_DATA_NOECHO;
						}
					}
				}
			}
		}
	}

////////////////////////////////////////////////////////////////////////////
	///////*!sea clutter distinguish*/
	for(int j=0;j<iAziCount;j++)
	{
		for(int i=1;i<iRefGates;i++)
		{
			ir=(i+2)/4;
			mbsh_num_ve[i][j]=0;
			mbsh_out[i][j]=0.0;
			if(spw[j][i]!=RADAR_DATA_NOECHO)
			{
				mbsh_s=mfun(mem_para_sea.mdsw,spw[j][i]);
				mbsh_out[i][j]=mbsh_out[i][j]+mbsh_s;
				mbsh_num_ve[i][j]=mbsh_num_ve[i][j]+1;
			}
			if(tdbz[ir][j]!=RADAR_DATA_NOECHO)
			{
				mbsh_s=mfun_r(mem_para_sea.tdbz,tdbz[ir][j]);
				mbsh_out[i][j]=mbsh_out[i][j]+mbsh_s;
				mbsh_num_ve[i][j]=mbsh_num_ve[i][j]+1;
			}
			if(spin[ir][j]!=RADAR_DATA_NOECHO)
			{
				mbsh_s=mfun(mem_para_sea.spin,spin[ir][j]);
				mbsh_out[i][j]=mbsh_out[i][j]+mbsh_s;
				mbsh_num_ve[i][j]=mbsh_num_ve[i][j]+1;
			}
			if(gdzz[ir][j]!=RADAR_DATA_NOECHO)
			{
				mbsh_s=mfun(mem_para_sea.gdz,gdzz[ir][j]);
				mbsh_out[i][j]=mbsh_out[i][j]+3*mbsh_s;
				mbsh_num_ve[i][j]=mbsh_num_ve[i][j]+3;
			}
			if(mbsh_num_ve[i][j]>1)
			{
				mbsh_out[i][j]=mbsh_out[i][j]/mbsh_num_ve[i][j];
			}
		}
	}
	//!set the Clear Air echo to spval
	for(int j=0;j<iAziCount;j++)
	{
		for(int i=8;i<iDopGates-8;i++)
		{
			ir=(i+2)/4+1;
			thresh_ap=0.1;
			if(vel[j][i]>RADAR_DATA_NOECHO)
				thresh_ap=0.1;
			if(mbsh_out[i][j]>thresh_ap)
			{
				for(jd=-1;jd<=1;jd++)
				{
					jj=j+jd;
					for(id=-1;id<=1;id++)
					{
						if(jj>=iAziCount)
							jj=0;
						if(jj<0)
							jj=iAziCount-1;
						if (ref[jj][ir+id]>RADAR_DATA_NOECHO)
						{
							ref[jj][ir+id]=RADAR_DATA_NOECHO;
						}
					}
				}
				for(jd=-1;jd<=1;jd++)
				{
					jj=jd+j;
					for(id=-4;id<=4;id++)
					{
						if(jj>=iAziCount)
							jj=0;
						if(jj<0)
							jj=iAziCount-1;
						if (vel[jj][i+id]>RADAR_DATA_NOECHO)
						{
							vel[jj][i+id]=RADAR_DATA_NOECHO;
						}
						if (spw[jj][i+id]>RADAR_DATA_NOECHO)
						{
							spw[jj][i+id]=RADAR_DATA_NOECHO;
						}
					}
				}
			}
		}
	}

	for (int i=0;i<iDopGates;i++)
	{
		delete [] mdsw[i];
		delete [] mdve[i];
		delete [] sdevv[i];
		delete [] tve[i];
	}
	for (int i=0;i<iRefGates;i++)
	{
		delete [] mdz1[i];
		delete [] mdz2[i];
		delete [] spin[i];
		delete [] sign[i];
		delete [] tdbz[i];
		delete [] mbsh_num_ve[i];
		delete [] gdzz[i];
	}

	for (int i=0;i<NGATE;i++)
	{
		delete [] mbsh_out[i];
	}

	delete [] mdsw;
	delete [] mdve;
	delete [] mdz1;
	delete [] mdz2;
	delete [] sdevv;
	delete [] gdzz;
	delete [] spin;
	delete [] sign;
	delete [] mbsh_num_ve;
	delete [] mbsh_out;
	delete [] tve;
	delete [] tdbz;

	mdsw = NULL;
	mdve = NULL;
	mdz1 = NULL;
	mdz2 = NULL;
	sdevv = NULL;
	gdzz = NULL;
	spin = NULL;
	sign = NULL;
	mbsh_num_ve = NULL;
	mbsh_out = NULL;
	tve = NULL;
	tdbz = NULL;
	return GS_OK;
}
GHRESULT QualityControl_CC ::median(gInt16** pData,gInt16 nr,gInt16 np,gInt16** med_fld)
{
	gInt16 l_half,ic;
	gInt16* a = new(std::nothrow) gInt16[9];

	/*l_half=int((npt-1)/2);*/

	for (gInt16 j=0;j<np;j++)
	{
		for (gInt16 i=4;i<nr;i++)
		{
			ic=0;
			for(gInt16 m=i-4;m<i+4;m++)
			{
				a[ic]=pData[j][m];
				ic++;
			}

			for (gInt16 jj=1;jj<9;jj++)
			{
				for (gInt16 ii=jj-1;ii>=0;ii--)
				{
					if(a[ii]<= a[jj])
					{
						a[ii+1]=a[jj];
						break;
					}
					else
					{
						a[ii+1]=a[ii];
					}
				}
			}
			med_fld[i][j]=a[4];
		}
	}

	delete []a;
	a=NULL;

	return GS_OK;
}

gInt32 QualityControl_CC ::mfun(gInt32 para[],gInt16 x)
{
	gInt32 y;
	if(x<=para[0]|| x>=para[2])
	{
		y=0;
	}
	else if(x>para[0] && x<=para[1])
	{
		y=100*(x-para[0])/(para[1]-para[0]);
	}
	else
	{
		y=100*(para[2]-x)/(para[2]-para[1]);
	}

	return y;
}


gInt32 QualityControl_CC ::mfun_r(gInt32 para[],gFloat x)
{
	gInt32 y;
	if(x<=para[0] || x >= para[2])
	{
		y=0;
	}
	else if(x>para[0]&& x<=para[1])
	{
		y=100*(x-para[0])/(para[1]-para[0]);
	}
	else
	{
		y=100*(para[2]-x)/(para[2]-para[1]);
	}

	return y;
}

GHRESULT QualityControl_CC ::ttve(gInt16** pData,gFloat** tve,gInt16 np,gInt16 iminrg,gInt16 nrang,gInt16 mgate,gInt16 mazim,gInt16 numthresh)
{
	gInt16 mgate_hf,mazim_hf;
	gInt16 i,j,ma,mr,num;
	gFloat nsign,ttdbz,sspin,diff;

	mgate_hf=(mgate-1)/2;
	mazim_hf=(mazim-1)/2;
	if(iminrg-mgate_hf<1 || iminrg+mgate_hf>np)
	{
		return GS_FALSE;
	}

	for (j=0;j<np;j++)
	{
		for (i=iminrg;i<nrang;i++)
		{
			nsign=0;
			ttdbz=0;
			sspin=0;
			num=0;
			if (pData[j][i]>RADAR_DATA_NOECHO)
			{
				for (ma=j-mazim_hf;ma<=j+mazim_hf;ma++)
				{
					gInt16 m=ma;
					if (ma<0)
					{
						m=np+ma;
					}
					if (ma>=np)
					{
						m=ma-np;
					}
					for(mr=i-mgate_hf;mr<=i+mgate_hf;mr++)
					{
						if (pData[m][mr+1]>RADAR_DATA_NOECHO && pData[m][mr]>RADAR_DATA_NOECHO)
						{
							num++;
							diff=pData[m][mr+1]-pData[m][mr];
							ttdbz=ttdbz+(diff*diff);
						}
					}
				}
				if (num>numthresh)
				{
					tve[i][j]=ttdbz/num;
				}
			}
		}
	}

	return GS_OK;
}
GHRESULT QualityControl_CC::dbzpara(gInt16** pData,gInt16 np,gInt16 nrang,gInt32 mgate,gInt32 mazim,gInt32 dbzthresh,gInt32 numthresh,gFloat** tdbz,gInt16** sign,gInt16** spin)
{
	gInt16 mgate_hf,mazim_hf;
	gInt16 sspin;
	gFloat ttdbz,diff;
	gInt16 i,j,m,ma,mr,num;
	gFloat nsign;

	mgate_hf=(mgate-1)/2;
	mazim_hf=(mazim-1)/2;
	if(5-mgate_hf<1||5+mgate_hf>np)
	{
		return GS_FALSE;
	}

	for (j=0;j<np;j++)
	{
		for (i=5;i<nrang;i++)
		{
			nsign=0;
			ttdbz=0;
			sspin=0;
			num=0;
			tdbz[i][j]=RADAR_DATA_NOECHO;
			sign[i][j]=RADAR_DATA_NOECHO;
			spin[i][j]=RADAR_DATA_NOECHO;
			if (pData[j][i]>RADAR_DATA_NOECHO)
			{
				for (ma=j-mazim_hf;ma<=j+mazim_hf;ma++)
				{
					m=ma;
					if (ma<0)
					{
						m=np+ma;
					}
					if (ma>=np)
					{
						m=ma-np;
					}
					for(mr=i-mgate_hf;mr<=i+mgate_hf;mr++)
					{
						if (pData[m][mr+1]>RADAR_DATA_NOECHO && pData[m][mr]>RADAR_DATA_NOECHO)
						{
							num++;
							diff=pData[m][mr+1]-pData[m][mr];
							ttdbz+=gFloat(diff*diff);
							if (fabs(diff)>dbzthresh)
							{
								sspin++;
							}
							if (diff>0)
							{
								nsign++;
							}
							if (diff<0)
							{
								nsign--;
							}
						}
					}
				}
				if (num>numthresh)
				{
					sign[i][j]=100*nsign/num;
					tdbz[i][j]=ttdbz/num/100;
					spin[i][j]=100*sspin/num;
				}
			}
		}
	}

	return GS_OK;

}
GHRESULT QualityControl_CC ::sdt_dev_ve(gInt16** pVData,gInt16** fld_mdv,gInt16 np,gInt16 nrang,gInt16 mgate,gInt16 mazim,gInt16 numthresh,gInt16** sdevv)
{
	gInt16 mgate_hf,mazim_hf;
	gInt16 i,j,m,ma,mr;
	gFloat ssv,avg;
	gInt16 numv;
	//!c-------------------------------------------------------
	mgate_hf=(mgate-1)/2;
	mazim_hf=(mazim-1)/2;
	if(5-mgate_hf<1||5+mgate_hf>np)
	{
		return GS_FALSE;
	}

	for (j=0;j<np;j++)
	{
		for (i=5;i<nrang;i++)
		{
			sdevv[i][j]=RADAR_DATA_NOECHO;
		}
	}

	for (j=0;j<np;j++)
	{
		for (i=5;i<nrang;i++)
		{
			ssv=0;
			avg=0;
			numv=0;
			if (fld_mdv[i][j]>RADAR_DATA_NOECHO)
			{
				for (ma=j-mazim_hf;ma<j+mazim_hf;ma++)
				{
					m=ma;
					if(ma<0)
						m=np+ma;
					if(ma>=np)
						m=ma-np;
					for (mr=i-mgate_hf;mr<i+mgate_hf;mr++)
					{
						if(pVData[m][mr]>RADAR_DATA_NOECHO)
						{
							numv=numv+1;
							ssv=ssv+gFloat(pVData[m][mr]-fld_mdv[i][j])*gFloat(pVData[m][mr]-fld_mdv[i][j]);
						}
					}
				}
				if(numv>numthresh)
				{
					sdevv[i][j]=gInt16(sqrt(ssv/(numv-1)));
				}
			}
		}
	}

	return GS_OK;
}



GHRESULT QualityControl_CC ::OutPutData(void *pValue)
{
	if (pValue == NULL)
	{
		return GE_INVALIDARG;
	}
	m_pOutputProductList = (GXList<GenerationData>*)pValue;
	return GS_OK;
}
GHRESULT  QualityControl_CC::writedata(StandardRadarData* pInData,StandardRadarData* pOutData,gInt16 ** ref[],gInt16**  vel[], gInt16** spw[])
{

	//GHRESULT gResult;
	//gInt16 iElevationCount = pInData->GetHeadP()->ElevationCount;
	//GELEVATIONDATAHEAD * pOutEleHead = new GELEVATIONDATAHEAD;
	//if (pOutEleHead==NULL)
	//{
	//	return GS_FALSE;
	//}
	//for(gInt16 iEleIndex = 0;iEleIndex < iElevationCount; iEleIndex++ )
	//{
	//	GELEVATIONDATAHEAD * pInEleHead=pInData->GetElevationHeadP(iEleIndex);

	//	pOutEleHead->SNOfElevationData =pInEleHead->SNOfElevationData;
	//	pOutEleHead->UnambiguousRange = pInEleHead->UnambiguousRange;
	//	pOutEleHead->NyquistVelocity = pInEleHead->NyquistVelocity;
	//	pOutEleHead->ReflectivityResolution = pInEleHead->ReflectivityResolution;
	//	pOutEleHead->VelocityResolution = pInEleHead->VelocityResolution;
	//	pOutEleHead->SpectrumWidthOfVelocityResolution = pInEleHead->SpectrumWidthOfVelocityResolution;
	//	pOutEleHead->FirstGateRangeOfRef = pInEleHead->FirstGateRangeOfRef;
	//	pOutEleHead->ReflectivityGates = pInEleHead->ReflectivityGates;
	//	pOutEleHead->FirstGateRangeOfDoppler = pInEleHead->FirstGateRangeOfDoppler;
	//	pOutEleHead->DopplerGates = pInEleHead->DopplerGates;
	//	pOutEleHead->Elevation = pInEleHead ->Elevation;
	//	pOutEleHead->AzimuthNumber = pInEleHead->AzimuthNumber;

	//	if ((gResult = pOutData->WriteElevationHead(iEleIndex, pOutEleHead))!=GS_OK)
	//	{
	//		delete pOutEleHead;
	//		pOutEleHead = NULL;
	//		return gResult;
	//	}
	//	gInt16 iAziCount = pInEleHead->AzimuthNumber;
	//	gInt16 iRefGates = pInEleHead->ReflectivityGates;
	//	gInt16 iDopGates = pInEleHead->DopplerGates;
	//	for(gInt16 iAziIndex=0;iAziIndex<iAziCount;iAziIndex++)
	//	{
	//		RVWDATAHEAD * pInRVWHead = pInData->GetRadarDataP(iEleIndex,iAziIndex);
	//		RVWDATAHEAD * pOutRVWHead = pOutData->GetRadarDataP(iEleIndex,iAziIndex);
	//		//写该RVW块数据头
	//		pOutRVWHead->SNOfRadialData = pInRVWHead->SNOfRadialData;
	//		pOutRVWHead->Elevation = pInRVWHead->Elevation;          //单位0.01度
	//		pOutRVWHead->Azimuth = pInRVWHead->Azimuth;  //单位0.01度
	//		pOutRVWHead->Date = pInRVWHead->Date;
	//		pOutRVWHead->Time = pInRVWHead->Time;

	//		gInt16 * pOutRData = pOutData->GetRadarDataR(iEleIndex,iAziIndex);
	//		gInt16 * pInRData = pInData->GetRadarDataR(iEleIndex,iAziIndex);
	//		gInt16 * pOutVData = pOutData->GetRadarDataV(iEleIndex,iAziIndex);
	//		gInt16 * pInVData = pInData->GetRadarDataV(iEleIndex,iAziIndex);
	//		gInt16 * pOutWData = pOutData->GetRadarDataW(iEleIndex,iAziIndex);
	//		gInt16 * pInWData = pInData->GetRadarDataW(iEleIndex,iAziIndex);

	//		if (iEleIndex<3)
	//		{
	//			for(gInt16 i=0;i<iRefGates;i++)
	//			{
	//				pOutRData[i]=ref[iEleIndex][iAziIndex][i];
	//			}
	//			for(gInt16 i=0;i<iDopGates;i++)
	//			{
	//				pOutVData[i]=vel[iEleIndex][iAziIndex][i];
	//				pOutWData[i]=spw[iEleIndex][iAziIndex][i];
	//			}
	//		}
	//		else
	//		{
	//		for(gInt16 i=0;i<iRefGates;i++)
	//		{
	//			pOutRData[i]=pInRData[i];
	//		}
	//		for(gInt16 i=0;i<iDopGates;i++)
	//		{
	//			pOutVData[i]=pInVData[i];
	//			pOutWData[i]=pInWData[i];
	//		}
	//		}
	//	}
	//}
	//delete pOutEleHead;
	//pOutEleHead = NULL;
	GRADARDATAHEADER * pOutHead = new GRADARDATAHEADER;
	GRADARDATAHEADER * pInHead = pInData->GetHeadP();

	pOutHead->ElevationCount =pInHead->ElevationCount;
	//memcpy(pOutHead->IndexOfElevationData, pInHead->IndexOfElevationData, 20*sizeof(gInt32));
	pOutHead->RadarHeight = pInHead->RadarHeight;
	pOutHead->RadarLat = pInHead->RadarLat;
	pOutHead->RadarLon = pInHead->RadarLon;
	memcpy(pOutHead->ScanMode, pInHead->ScanMode, 2*sizeof(gInt8));
	memcpy(pOutHead->SiteCode, pInHead->SiteCode, 8*sizeof(gInt8));
	memcpy(pOutHead->RadarType, pInHead->RadarType, 4*sizeof(gInt8));
	memcpy(pOutHead->OperationMode, pInHead->OperationMode, 2*sizeof(gInt8));
	pOutHead->VCPTime = pInHead->VCPTime;
	pOutHead->VCPDate = pInHead->VCPDate;
	memcpy(pOutHead->VCPMode, pInHead->VCPMode, 4*sizeof(gInt8));
	pOutHead->SectorCode = pInHead->SectorCode;
	pOutHead->SizeOfSector = pInHead->SizeOfSector;
	pOutHead->ReflectivityGateSize = pInHead->ReflectivityGateSize;
	pOutHead->DopplerGateSize = pInHead->DopplerGateSize;
	pOutHead->ConstantOfSystemCorrect = pInHead->ConstantOfSystemCorrect;

	//GHRESULT gResult;
	//if ((gResult = pOutData->WriteHead(pOutHead))!=GS_OK)
	//{
	//	delete pOutHead;
	//	pOutHead = NULL;
	//	return gResult;
	//}	
	//delete pOutHead;
	//pOutHead = NULL;
	GHRESULT gResult;
	gInt16 iElevationCount = pInData->GetHeadP()->ElevationCount;
	GELEVATIONDATAHEAD * pOutEleHead = new GELEVATIONDATAHEAD[iElevationCount];
	if (pOutEleHead==NULL)
	{
		return GS_FALSE;
	}
	for(gInt16 iEleIndex = 0;iEleIndex < iElevationCount; iEleIndex++ )
	{
		GELEVATIONDATAHEAD * pInEleHead=pInData->GetElevationHeadP(iEleIndex);

		pOutEleHead[iEleIndex].SNOfElevationData =iEleIndex+1;
		pOutEleHead[iEleIndex].UnambiguousRange = pInEleHead->UnambiguousRange;
		pOutEleHead[iEleIndex].NyquistVelocity = pInEleHead->NyquistVelocity;
		pOutEleHead[iEleIndex].ReflectivityResolution = pInEleHead->ReflectivityResolution;
		pOutEleHead[iEleIndex].VelocityResolution = pInEleHead->VelocityResolution;
		pOutEleHead[iEleIndex].SpectrumWidthOfVelocityResolution = pInEleHead->SpectrumWidthOfVelocityResolution;
		pOutEleHead[iEleIndex].FirstGateRangeOfRef = pInEleHead->FirstGateRangeOfRef;
		pOutEleHead[iEleIndex].ReflectivityGates = pInEleHead->ReflectivityGates;
		pOutEleHead[iEleIndex].FirstGateRangeOfDoppler = pInEleHead->FirstGateRangeOfDoppler;
		pOutEleHead[iEleIndex].DopplerGates = pInEleHead->DopplerGates;
		pOutEleHead[iEleIndex].Elevation = pInEleHead ->Elevation;
		pOutEleHead[iEleIndex].AzimuthNumber = pInEleHead->AzimuthNumber;
	}
	if ((gResult = pOutData-> WriteHeadAndElevationHead( pOutHead, pOutEleHead,iElevationCount))!=GS_OK)
	{
		delete pOutHead;
		delete []pOutEleHead;
		pOutEleHead = NULL;
		pOutHead = NULL;
		return gResult;
	}

	for(gInt16 iEleIndex = 0;iEleIndex < iElevationCount; iEleIndex++ )
	{
		gInt16 iAziCount = pOutEleHead[iEleIndex].AzimuthNumber;
		gInt16 iRefGates = pOutEleHead[iEleIndex].ReflectivityGates;
		gInt16 iDopGates = pOutEleHead[iEleIndex].DopplerGates;
		for(gInt16 iAziIndex=0;iAziIndex<iAziCount;iAziIndex++)
		{
			RVWDATAHEAD * pInRVWHead = pInData->GetRadarDataP(iEleIndex,iAziIndex);
			RVWDATAHEAD * pOutRVWHead = pOutData->GetRadarDataP(iEleIndex,iAziIndex);
			//写该RVW块数据头
			pOutRVWHead->SNOfRadialData = iAziIndex+1;
			pOutRVWHead->Elevation = pInRVWHead->Elevation;          //单位0.01度 
			pOutRVWHead->Azimuth =  (gInt16)iAziIndex*100;  //单位0.01度 
			pOutRVWHead->Date = pInRVWHead->Date;
			pOutRVWHead->Time = pInRVWHead->Time;

			gInt16 * pOutRData = pOutData->GetRadarDataR(iEleIndex,iAziIndex);
			gInt16 * pInRData = pInData->GetRadarDataR(iEleIndex,iAziIndex);
			gInt16 * pOutVData = pOutData->GetRadarDataV(iEleIndex,iAziIndex);
			gInt16 * pInVData = pInData->GetRadarDataV(iEleIndex,iAziIndex);
			gInt16 * pOutWData = pOutData->GetRadarDataW(iEleIndex,iAziIndex);
			gInt16 * pInWData = pInData->GetRadarDataW(iEleIndex,iAziIndex);

			if (iEleIndex<3)
			{
				for(gInt16 i=0;i<iRefGates;i++)
				{
					pOutRData[i]=ref[iEleIndex][iAziIndex][i];
				}
				for(gInt16 i=0;i<iDopGates;i++)
				{
					pOutVData[i]=vel[iEleIndex][iAziIndex][i];
					pOutWData[i]=spw[iEleIndex][iAziIndex][i];
				}
			}
			else
			{
				for(gInt16 i=0;i<iRefGates;i++)
				{
					pOutRData[i]=pInRData[i];
				}
				for(gInt16 i=0;i<iDopGates;i++)
				{
					pOutVData[i]=pInVData[i];
					pOutWData[i]=pInWData[i];
				}
			}
		}
	}
	delete pOutHead;
	delete []pOutEleHead;
	pOutEleHead = NULL;
	pOutHead = NULL;


	return GS_OK;
}

GHRESULT QualityControl_CC ::noise_filter(StandardRadarData* pData)
{

	gInt16**  data_tmp = NULL;
	gInt16*  ref = NULL;
	gInt16*  vel = NULL;
	gInt16*  spw = NULL;
	gInt16 ia;
	gFloat nt,nf,thresh,ratio;
	gInt16 dazim,dgate,h_dazim,h_dgate;
	dazim = 5;
	dgate = 10;
	nt = dgate*dazim;
	thresh = 0.4;//
	h_dazim = int(dazim/2)+1;
	h_dgate = int(dgate/2)+1;

	gInt16 iEleCount = pData->GetHeadP()->ElevationCount;
	gInt16 iAziCount = 0;
	gInt16 iRefGates = 0;
	gInt16 iDopGates = 0;

	data_tmp = new(::std::nothrow) gInt16* [NAZIM];
	for (gInt16 i=0;i<NAZIM;i++)
	{
		data_tmp[i]=new(::std::nothrow) gInt16[NGATE];
	}
	for(gInt16 iEleIndex=0;iEleIndex<iEleCount;iEleIndex++)
	{
		iAziCount = pData->GetElevationHeadP(iEleIndex)->AzimuthNumber;
		iRefGates = pData->GetElevationHeadP(iEleIndex)->ReflectivityGates;
		for (gInt16 i=0;i<iAziCount;i++)
		{
			for (gInt16 j=0;j<iRefGates;j++)
			{
				data_tmp[i][j] = 99;
			}
		}
		for (gInt16 i=0;i<iAziCount;i++)
		{
			for (gInt16 j=0;j<iRefGates-dgate;j++)
			{
				nf=0;
				for (gInt16 j1=j;j1<j+dgate;j1++)
				{
					for (gInt16 i1=i;i1<i+dazim;i1++)
					{
						ia=i1;
						if (i1>=iAziCount)
						{
							ia = i1-iAziCount;
						}
						ref = pData->GetRadarDataR(iEleIndex,ia);
						if (ref[j1]>RADAR_DATA_NOECHO)
						{
							nf++;
						}
					}
				}
				ratio=nf/nt;
				if(ratio < thresh)
				{
					ia=i+h_dazim;
					if(ia>=iAziCount)
						ia=ia-iAziCount;
					data_tmp[ia][j+h_dgate]=RADAR_DATA_NOECHO;
				}
			}
		}

		for (gInt16 i=0;i<iAziCount;i++)
		{
			ref = pData->GetRadarDataR(iEleIndex,i);
			for (gInt16 j=0;j<iRefGates-dgate;j++)
			{
				if (data_tmp[i][j] == RADAR_DATA_NOECHO)
				{
					ref[j] = RADAR_DATA_NOECHO;//miss_data;
				}
			}
		}


		//!vel noise filter
		//!strct_in_tmp%field=99
		iDopGates = pData->GetElevationHeadP(iEleIndex)->DopplerGates;
		for (gInt16 i=0;i<iAziCount;i++)
		{
			for (gInt16 j=0;j<iDopGates;j++)
			{
				data_tmp[i][j] = 99;
			}
		}

		for (gInt16 i=0;i<iAziCount;i++)
		{
			for (gInt16 j=0;j<iDopGates-dgate;j++)
			{
				nf=0;
				for (gInt16 j1=j;j1<j+dgate;j1++)
				{
					for (gInt16 i1=i;i1<i+dazim;i1++)
					{
						ia=i1;
						if (i1>=iAziCount)
						{
							ia=i1-iAziCount;
						}
						vel = pData->GetRadarDataV(iEleIndex,ia);
						if (vel[j1]> RADAR_DATA_NOECHO)
						{
							nf++;
						}
					}
				}
				ratio=(nf)/nt;
				if(ratio<thresh)
				{
					ia=i+h_dazim;
					if(ia>=iAziCount)
						ia=ia-iAziCount;
					data_tmp[ia][j+h_dgate]=RADAR_DATA_NOECHO;
				}
			}
		}

		for (gInt16 i=0;i<iAziCount;i++)
		{
			vel = pData->GetRadarDataV(iEleIndex,i);
			for (gInt16 j=0;j<iDopGates-dgate;j++)
			{
				if (data_tmp[i][j] == RADAR_DATA_NOECHO)
				{
					vel[j] = RADAR_DATA_NOECHO;//miss_data
				}
			}
		}

		//!sw noise filter
		//!strct_in_tmp%field=99
		for (gInt16 i=0;i<iAziCount;i++)
		{
			for (gInt16 j=0;j<iDopGates;j++)
			{
				data_tmp[i][j] = 99;
			}
		}

		for (gInt16 i=0;i<iAziCount;i++)
		{
			for (gInt16 j=0;j<iDopGates-dgate;j++)
			{
				nf=0;
				for (gInt16 j1=j;j1<j+dgate;j1++)
				{
					for (gInt16 i1=i;i1<i+dazim;i1++)
					{
						ia=i1;
						if (i1>=iAziCount)
						{
							ia=i1-iAziCount;
						}
						spw = pData->GetRadarDataW(iEleIndex,ia);
						if (spw[j1] > RADAR_DATA_NOECHO)
						{
							nf++;
						}
					}
				}
				ratio=(nf)/nt;
				if(ratio<thresh)
				{
					ia=i+h_dazim;
					if(ia>=iAziCount)
						ia=ia-iAziCount;
					data_tmp[ia][j+h_dgate]=RADAR_DATA_NOECHO;
				}
			}
		}

		for (gInt16 i=0;i<iAziCount;i++)
		{
			spw = pData->GetRadarDataW(iEleIndex,i);
			for (gInt16 j=0;j<iDopGates-dgate;j++)
			{
				if (data_tmp[i][j] == RADAR_DATA_NOECHO)
				{
					spw[j] = RADAR_DATA_NOECHO;//miss_data;
				}
			}
		}
	}

	for (gInt16 i=0;i<NAZIM;i++)
	{
		delete []data_tmp[i];
		data_tmp[i]=NULL;
	}
	delete [] data_tmp;
	data_tmp = NULL;


	return GS_OK;
}

void QualityControl_CC::sun_beam_remove(short **bref,short ** v, short** w,	int nrays,	int ngates )
{
	int jp,jn,num;
	int i,j,k;

	short **echo_num = new short * [nrays];
	for(  i = 0; i < nrays; i++)
		echo_num[i] = new short [ngates];

	for(  j = 0; j < nrays; j++)
		for(  k = 0; k < ngates; k++)
		{
			echo_num[j][k] =  0;
			if( bref[j][k] < -1600)
				continue;

			jn = j - 4;      jp = j + 4;
			if( jn < 0 )     jn = jn + nrays;
			if( jp >= nrays ) jp = jp - nrays;

			if( bref[jp][k] > 0 || bref[jn][k] > 0 )
				continue;

			num = 1;  
			for( i = 1; i < 4; i++)
			{
				jn = j - i;
				if( jn < 0 )  jn = jn + nrays;
				if( bref[jn][k] >  0 )
					num++;
				else break;
			}

			for( i = 1; i < 4; i++)
			{
				jp = j + i;
				if( jp >= nrays  ) jp = jp - nrays;
				if( bref[jp][k] >  0 )
					num++;
				else break;
			}
			echo_num[j][k] =  num;
		} //end of (k)

		num = 0;
		for( j = 0; j < nrays; j++)
			for( k = 0; k < ngates-1; k++)
			{
				if( echo_num[j][k] > 0 && echo_num[j][k+1] > 0 &&
					abs(echo_num[j][k] - echo_num[j][k+1] )<= 1 )   
				{
					if( num > 5 )
					{
						bref[j][k] = RADAR_DATA_NOECHO;
						v[i][k] = RADAR_DATA_NOECHO;
						w[i][k] = RADAR_DATA_NOECHO;
					}
					else if( num == 5 )
					{
						for(int i = 1; i <= 5; i++)
						{
							bref[j][k - i] = RADAR_DATA_NOECHO;
							v[j][k - i] = RADAR_DATA_NOECHO;
							w[j][k - i] = RADAR_DATA_NOECHO;
						}
							
					}
					num++;
				}
				else num = 0;
			}//end of (k)

			for( i = 0; i < nrays; i++) 
				delete [] echo_num[i];

			delete [] echo_num;
}
void QualityControl_CC::noise_remove(short **bref, short ** v, short** w,int nrays,   int ngates, int start, int end,  short threshold ,int gatelen)
{
	float NOISE_FILTER_THRESHOLD = (float)0.0; //  可调参数，0.35~0.45
	int REF_SCALE   = 100;    //bref的放大倍数
	// gatelen 距离库的长度(米)

	short fthd = threshold * REF_SCALE;
	int j,k,m,jp1,jp2,jn1,jn2,idigit;

	float Stotal, Sref;

	float S0 = (float)(gatelen*0.001*gatelen*0.001);

	float *SGate = new float[ngates];
	SGate[0] = S0;
	for(k=1;k<ngates;++k)
		SGate[k] = k*k*S0-SGate[k-1];

	// store of average value of 5x5 box
	short  **tmpbuf = new short *[nrays];
	for(j=0;j<nrays;++j)
	{
		tmpbuf[j] = new short [ngates];
		memcpy(tmpbuf[j], bref[j], ngates*2);
	}

	//Loop through the 3-D array by radial/gate position.
	//accumulate total points and total effective points in
	//5 x 5 domain.

	threshold *= REF_SCALE;

	if(end > ngates) end = ngates;
	for( j = 0; j < nrays; j++)
		for( k = start; k < end; k++)
		{
			if( tmpbuf[j][k] < -1600) continue;

			Sref = 0;    Stotal = 0;

			jp1 = j + 1;      jp2 = j + 2;
			jn1 = j - 1;      jn2 = j - 2;
			if( jp1 >= nrays ) jp1 = jp1 - nrays;
			if( jp2 >= nrays ) jp2 = jp2 - nrays;
			if( jn1 < 0 )     jn1 = jn1 + nrays;
			if( jn2 < 0 )     jn2 = jn2 + nrays;

			for( m = max(k-2,0); m <= min(k+2,ngates-1); m++ )
			{
				idigit = 0; 
				if(tmpbuf[jp1][m] > threshold )  ++idigit;
				if(tmpbuf[jp2][m] > threshold )  ++idigit;
				if(tmpbuf[j][m]   > threshold )  ++idigit;
				if(tmpbuf[jn1][m] > threshold )  ++idigit;
				if(tmpbuf[jn2][m] > threshold )  ++idigit;
				Stotal += SGate[m]*5;
				Sref += idigit * SGate[m];
			}

			//remove isolated dot, line echo,abnormal dot
			if(Sref/Stotal < NOISE_FILTER_THRESHOLD )
			{
				bref[j][k] = RADAR_DATA_NOECHO;
				v[j][k] = RADAR_DATA_NOECHO;
				w[j][k] = RADAR_DATA_NOECHO;
			
			}
		} //end of numcols loop

		delete []SGate;
		for(j=0;j<nrays;++j) delete [] tmpbuf[j];
		delete [] tmpbuf;

		return;

}
extern "C" DLLEXP_GSYS IArith *Create_arith (void)//用于创建接口
{
	return new QualityControl_CC;
}
