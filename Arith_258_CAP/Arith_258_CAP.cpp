// CAPPI.cpp: implementation of the CCAPPI class.
//create by yangys 20080417
//////////////////////////////////////////////////////////////////////
#ifndef IARITHMS_LIB
	#define IARITHMS_LIB
#endif

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "Arith_258_CAP.h"
#include "CJulMsGMT.h"
#include <malloc.h>
#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif
#include <math.h>
#include <fstream>
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Arith_258_CAP::Arith_258_CAP()
{
}

Arith_258_CAP::~Arith_258_CAP()
{
}
GHRESULT Arith_258_CAP::Initialize()
{
	n_C = NULL;
	m_cap = NULL;
	Escale = 0.0;
	ReadP = NULL;
	m_pGenerateProductList = NULL;
	m_pOutputProductList = NULL;
	return GS_OK;
}
GHRESULT Arith_258_CAP::UnInitialize()
{
	if(n_C!=NULL)
	{
		delete [] n_C;
		n_C = NULL;
	}
	if (m_cap!=NULL)
	{
		delete [] m_cap;
		m_cap = NULL;
	}
	return GS_OK;
}
GHRESULT Arith_258_CAP::LoadData(void *pValue,ReadParam * ReadParameter)
{
	if(ReadParameter == NULL)
		return GE_INVALIDARG;
	ReadP=ReadParameter;
	if ( pValue == NULL )
	{
		return GE_INVALIDARG;
	}

	m_pGenerateProductList = (GXList< GenerationData *> *)pValue;//取得输入数据列表
	//m_pParameter = ReadParameter;	//取得参数函数指针
	return GS_OK;
}
GHRESULT Arith_258_CAP::Execute()
{
	ofstream logfile;
	logfile.open("./Log/Arith_258_CAP.txt",ios::app);

	gUint16 iJulDay;
	gInt32 iMilliSecond,iYear,iMonth,iDay,iHour,iMinute,iSecond,iMSecond;
	CJulMsGMT::GetLocalJulMs(iJulDay, iMilliSecond );
	CJulMsGMT::Jul2Greg(iJulDay,iYear,iMonth,iDay);
	CJulMsGMT::MilliSecond2Time(iMilliSecond,iHour,iMinute,iSecond,iMSecond);

	logfile<<endl<<iHour<<":"<<iMinute<<":"<<iSecond<<"	"<<iYear<<"-"<<iMonth<<"-"<<iDay<<endl;

	logfile<<"258_CAP：  Started: "<<endl;
	logfile<<"258_CAP：  Get Parameters: Height_Choice,Height_Above_Radar[10]. "<<endl;

	StandardRadarData* pRadar = &(*(m_pGenerateProductList->GetHeadAs<StandardRadarData>()));
	int Height_Choice = 10;
	//////////////////////////////////////////////////////////////////////////传层数完毕
	int Height_Above_Radar[10];
	Height_Above_Radar[0] = 1000;
	Height_Above_Radar[1] = 2000;
	Height_Above_Radar[2] = 3000;
	Height_Above_Radar[3] = 5000;
	Height_Above_Radar[4] = 10000;
	Height_Above_Radar[5] = 10500;
	Height_Above_Radar[6] = 13000;
	Height_Above_Radar[7] = 14500;
	Height_Above_Radar[8] = 16500;
	Height_Above_Radar[9] = 17000;
	//////////////////////////////////////////////////////////////////////////参数传递完毕
	logfile<<"258_CAP：  Get and Set head Information from StandardRadarData. "<<endl;

	m_pOutputProductList->AddTail();
	Product_258_CAP* ppdt = &(*(m_pOutputProductList->GetTailAs<Product_258_CAP>()));
	GHRESULT ConstrutHand;
	ConstrutHand = ppdt->ConstructIt(gInt16(Height_Choice),pRadar->GetElevationHeadP(0)->AzimuthNumber,pRadar->GetElevationHeadP(0)->ReflectivityGates);
	if(ConstrutHand != GS_OK)
	{
		logfile<<"258_CAP：  Create New Product Failed and Return GE_FAILOFNEWPRODUMEM."<<endl;
		logfile.close();
		return GE_FAILOFNEWPRODUMEM;
	}

	logfile<<"258_CAP：  Loop: i=0-->"<<Height_Choice<<endl;
	for (int i = 0; i < Height_Choice; i++)
	{
		logfile<<"258_CAP：   i="<<i<<":"<<endl;
		logfile<<"258_CAP：    CAPPIMaker(Height_Above_Radar["<<i<<"],"<<i<<",pRadar,ppdt)"<<endl;

		CAPPIMaker( Height_Above_Radar[i],i,pRadar, ppdt);
		logfile<<"258_CAP：    CAPPIMaker() Finished."<<endl;

		DILayer *p_product = ppdt->GetMyDILayer(i);
		p_product->HeightOfElevation = Height_Above_Radar[i];//把高度写到产品中
		p_product->NumberOfBlocks = 1;
		p_product->SNOfLayer = i;
		PDBlock* pdb = ppdt->GetMyPDBlock();
		GRADARDATAHEADER* prdh = pRadar->GetHeadP();
		//pdb->NumberOfLayers=0;
		memcpy(ppdt->GetRadarInfor(), prdh->RadarType, sizeof(gInt8) * 4);
		ppdt->GetRadarInfor()->Altitude= prdh->RadarHeight;
		ppdt->GetRadarInfor()->Latitude= prdh->RadarLat;
		ppdt->GetRadarInfor()->Longitude= prdh->RadarLon;
		memcpy(pdb->VCPMode, prdh->VCPMode, sizeof(gInt8)*4);
		memcpy(pdb->OperationalMode, prdh->OperationMode, sizeof(gInt8) *2);
		pdb->ProductDate= prdh->VCPDate;
		pdb->ProductTime= prdh->VCPTime;
		CJulMsGMT::Jul2Greg(pdb->ProductDate,iYear,iMonth,iDay);
		CJulMsGMT::MilliSecond2Time(pdb->ProductTime,iHour,iMinute,iSecond,iMSecond);
		logfile<<"258_CAP：    VCPDate："<<iYear<<"-"<<iMonth<<"-"<<iDay<<"		VCPTime："<<iHour<<":"<<iMinute<<":"<<iSecond<<endl;

		pdb->ScaleOfData = 100; 
		pdb->IndexOfTabular = -1;
		pdb->IndexOfGraphic = -1;

		CJulMsGMT::GetLocalJulMs(pdb->GenerationDateOfProduct,
			pdb->GenerationTimeOfProduct);

		memcpy(ppdt->GetRadarInfor()->SiteCode, prdh->SiteCode,sizeof(gChar)*8);

		logfile<<"258_CAP：    Set Radial_Head("<<i<<") and Radial Angle Information."<<endl;

		ppdt->GetMyRadial_Head(i)->DistanceOfFirstGate = pRadar->GetElevationHeadP(0)->FirstGateRangeOfRef;
		ppdt->GetMyRadial_Head(i)->LengthOfGate = pRadar->GetHeadP()->ReflectivityGateSize;

		for( int j = 0 ; j < pRadar->GetElevationHeadP(0)->AzimuthNumber ; j++ )
		{
			ppdt->GetMyRadial_RadialData(i,j)->AzimuthAngle = pRadar->GetRadarDataP(0,j)->Azimuth ;
			ppdt->GetMyRadial_RadialData(i,j)->DeltaAngle=100;
		}
	}

	logfile<<"258_CAP：  Loop Finished."<<endl;
	logfile<<"258_CAP：  Finished."<<endl;
	logfile.close();

	return GS_OK;
}
GHRESULT Arith_258_CAP::OutPutData(void * pValue)
{
	if ( pValue == NULL )
	{
		return GE_INVALIDARG;
	}
	m_pOutputProductList = (GXList< GenerationData > *)pValue;
	return GS_OK;
}

int Arith_258_CAP::CAPPIMaker( int Height_Above_Radar,int NumofHeight, StandardRadarData * pRadar, Product_258_CAP* pRadialData )
{	
	GRADARDATAHEADER* prdh = pRadar->GetHeadP();
	int ppi_l= prdh->ElevationCount;
	int cap_gates = pRadar->GetElevationHeadP(0)->ReflectivityGates;
	int cap_nrays = pRadar->GetElevationHeadP(0)->AzimuthNumber;
	const float H_CAP=(float)(Height_Above_Radar*0.001);
	int Rscale = int(1000.0/pRadar->GetHeadP()->ReflectivityGateSize);

	if( H_CAP <0.1)
	{
		return 0;
	} //低于100米
	if ( H_CAP > 20)
	{
		return 0;
	}
	// initalize CAPPI buffer
	for(int i = 0;i < cap_nrays; i ++)
	{
		for(int j = 0; j < cap_gates; j++)
		{
			pRadialData->GetMyRadial_Data(NumofHeight,i)[j]=RADAR_DATA_NOECHO;
		}
	}
	if( !Initialization( ppi_l,pRadar))
	{
		return 0;
	}

	//求等高面与各圆锥面的交点距离
	CalculateCrossRange(H_CAP, ppi_l, pRadar) ;

	//计算插值表
	CreateCAPPITab(H_CAP,ppi_l, pRadar) ;
	//计算CAPPI
	int StartBin = (int)(n_C[0]*Rscale);
	int EndBin = (int)(n_C[ppi_l+1]*Rscale);

	if( EndBin >=cap_gates) 
	{
		EndBin=cap_gates-1 ;
	}
	short tZ = RADAR_DATA_NOECHO,bZ = RADAR_DATA_NOECHO;
	for(int j = 0; j < cap_nrays; j++)
	{
		for(int i = StartBin; i <= EndBin; i++)
		{
			short *ptemp = pRadar->GetRadarDataR(m_cap[i].te,j);
			if(!m_cap[i].n) 
			{
				continue;
			}
			if (j < pRadar->GetElevationHeadP(m_cap[i].te)->AzimuthNumber && \
				m_cap[i].tR < pRadar->GetElevationHeadP(m_cap[i].te)->ReflectivityGates - 1)
			{
				if ( ptemp[m_cap[i].tR]>-500&& ptemp[m_cap[i].tR+1]>-500 )
				{
					tZ = (ptemp[m_cap[i].tR]*m_cap[i].th1 + ptemp[m_cap[i].tR+1]*m_cap[i].th2+50)/100;
				}
				else
				{
					tZ = max(ptemp[m_cap[i].tR], ptemp[m_cap[i].tR+1]);
					if ( tZ<-1600 )
					{
						tZ=-1600;	
					}
				}
			}
			short *ptemp1 = pRadar->GetRadarDataR(m_cap[i].be,j);
			if (j < pRadar->GetElevationHeadP(m_cap[i].be)->AzimuthNumber && \
				m_cap[i].bR < pRadar->GetElevationHeadP(m_cap[i].be)->ReflectivityGates - 1)
			{
				if ( ptemp1[m_cap[i].bR]>-500 && ptemp1[m_cap[i].bR+1]>-500 )
				{
					bZ = (ptemp1[m_cap[i].bR]*m_cap[i].th1 + ptemp1[m_cap[i].bR+1]*m_cap[i].th2+50)/100;
				}
				else
				{
					bZ = max(ptemp1[m_cap[i].bR],ptemp1[m_cap[i].bR+1]);
					if ( bZ<-1600 )
					{
						bZ=-1600;
					}
				}
			}
			if(m_cap[i].n==1) 
			{
				pRadialData->GetMyRadial_Data(NumofHeight,j)[i]=bZ;
			}
			else if( m_cap[i].n==2)
			{
				pRadialData->GetMyRadial_Data(NumofHeight,j)[i]=(bZ*m_cap[i].bf+tZ*m_cap[i].tf+50)/100;
			}
			else if( m_cap[i].n==3)
			{
				pRadialData->GetMyRadial_Data(NumofHeight,j)[i]=tZ ;
			}

			if( pRadialData->GetMyRadial_Data(NumofHeight,j)[i]<-700 ) 
			{
				pRadialData->GetMyRadial_Data(NumofHeight,j)[i] = RADAR_DATA_NOECHO;
			}	
		}
	}
	delete [] n_C;
	n_C = NULL;
	delete [] m_cap;
	m_cap = NULL;
	return 0;
}

bool Arith_258_CAP::
Initialization(int ppi_l,StandardRadarData * pRadar)
{
	if(  ppi_l < 3 ) 
	{
		return false;
	}
	int cap_gates = pRadar->GetElevationHeadP(0)->ReflectivityGates;
	Escale = float(M_PI/180.0 * 0.01);
	n_C = new (::std::nothrow)float[ppi_l+2];
	m_cap  = new(::std::nothrow) CAPSTRUCT[cap_gates];

	return true;
}

void Arith_258_CAP::
CreateCAPPITab(float Height_Above_Radar, int ppi_l, StandardRadarData * pRadar) 
{
	int i = 0,j = 0,k = 0; 
	int start_bin = 0, end_bin = 0;
	float cosa = 0.0f, cosb = 0.0f;
	float tH = 0.0f,bH = 0.0f,tR = 0.0f,bR = 0.0f;
	short cap_gates= pRadar->GetElevationHeadP(0)->ReflectivityGates;
	int Rscale = int(1000.0/pRadar->GetHeadP()->ReflectivityGateSize);
	memset(m_cap,0,sizeof(CAPSTRUCT)*cap_gates) ;

	//有外向里计算
	// R(1,0)~R1之间取第一层对应的数据值
	cosa = cos(pRadar->GetElevationHeadP(ppi_l-1)->Elevation*Escale);
	start_bin = (int)(n_C[0]*Rscale);
	end_bin = (int)(n_C[1]*Rscale);

	for( i=start_bin;i<end_bin;i++)
	{
		m_cap[i].r=(gUint8)i;
		m_cap[i].n=1;
		m_cap[i].be = (gUint8)ppi_l-1;

		bR = i/cosa;
		m_cap[i].bR = (gUint16)bR;
		m_cap[i].bh2 = (gUint8)(100*(bR-m_cap[i].bR)+0.5) ;
		m_cap[i].bh1 = 100-m_cap[i].bh2 ;
		m_cap[i].bf = 100;
	}

	cosa = cos( pRadar->GetElevationHeadP(0)->Elevation*Escale );
	start_bin = (int)(n_C[ppi_l]*Rscale);
	end_bin = (int)(n_C[ppi_l+1]*Rscale);

	if( end_bin >=cap_gates) 
	{
		end_bin=cap_gates-1 ;
	}
	for( i=start_bin;i<=end_bin;i++)
	{
		m_cap[i].r=(gUint8)i;
		m_cap[i].n=3;
		m_cap[i].te = 0;

		tR = i/cosa ;
		m_cap[i].tR = (gUint16)tR;
		m_cap[i].th2 = (gUint8)(100*(tR-m_cap[i].tR)+0.5) ;
		m_cap[i].th1 = 100-m_cap[i].th2 ;
		m_cap[i].tf = 100;
	}

	//计算垂直及斜距插值系数
	k = ppi_l-1; 
	j = 1 ;
	cosa = cos(pRadar->GetElevationHeadP(k)->Elevation*Escale);
	start_bin = (int)(n_C[j]*Rscale);
	i=start_bin;
	while(j<ppi_l)
	{
		cosb = cos(pRadar->GetElevationHeadP(k-1)->Elevation*Escale);
		end_bin = (int)(n_C[j+1]*Rscale+0.5);

		if( end_bin >=cap_gates)
		{
			end_bin=cap_gates-1 ;
		}
		while( i < end_bin)
		{
			m_cap[i].r=(gUint8)i;
			m_cap[i].te=(gUint8)k;
			m_cap[i].be = (gUint8)k-1;

			tR= float((i+0.5)/cosa);
			m_cap[i].tR = (gInt16)tR;
			m_cap[i].th2 = (gUint8)(100*(tR-m_cap[i].tR)+0.5) ;
			m_cap[i].th1 = 100-m_cap[i].th2 ;
			tH =float( tR*(sqrt(1-cosa*cosa))+tR*tR*cosa*cosa/17000.0) ;

			bR= float((i+0.5)/cosb) ;
			m_cap[i].bR = (gInt16)bR;
			m_cap[i].bh2 = (gUint8)(100*(bR-m_cap[i].bR)+0.5) ;
			m_cap[i].bh1 = 100-m_cap[i].bh2 ;

			bH = float(bR*(sqrt(1-cosb*cosb))+bR*bR*cosb*cosb/17000.0);
			if( tH < Height_Above_Radar) 
			{
				m_cap[i].n=1;	
				m_cap[i].tf = 100;
			}
			else if( bH > Height_Above_Radar)
			{
				m_cap[i].n=3;	
				m_cap[i].bf = 100;
			}
			else
			{
				m_cap[i].n=2;
				m_cap[i].tf = (unsigned char)((Height_Above_Radar-bH)/(tH-bH)*100+0.5);
				m_cap[i].bf = (unsigned char)((tH-Height_Above_Radar)/(tH-bH)*100+0.5);
			}
			i++ ;
		};
		j++;
		k--;
		cosa = cosb ;
	};

	return ;
}

void Arith_258_CAP::
CalculateCrossRange( float Height_Above_Radar,int ppi_l,StandardRadarData * pRadar)
{
	// if the value of CAPPI height is less than the radar's height value,
	// this calculation will not be done.

	//求等高面与各圆锥面的交点斜距和水平距离
	float fval = 0.0f,A = 0.0f,B = 0.0f, a= 0.0f;

	//~~~~~~~~~~~~~~~~~~~~~~~~
	// Added for CINRAD/CC
	int Ref_Gate_len = pRadar->GetHeadP()->ReflectivityGateSize;
	int Ref_Gate = pRadar->GetElevationHeadP(0)->ReflectivityGates;
	int max_bin = Ref_Gate_len * Ref_Gate/1000;

	if( max_bin > 256 ) 
	{
		max_bin = 256;
	}
	max_bin = max_bin*1000/Ref_Gate_len;

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	for(int i=0;i<ppi_l;i++)
	{
		a = pRadar->GetElevationHeadP(i)->Elevation*Escale ;
		float cos_temp = cos(a);
		fval = float(17000.0/(cos_temp*cos_temp));
		A = float(fval*sin(a)/2.0);
		B = fval*Height_Above_Radar;
		n_C[ppi_l-i] = (float)((sqrt(A*A+B)-A)*cos_temp);
		if( n_C[ppi_l-i]>max_bin )
		{
			n_C[ppi_l-i]= (float)max_bin;
		}
	}

	//求出对应的各水平距离的中点
	n_C[0] = n_C[1]-(n_C[2]-n_C[1])/2;
	n_C[ppi_l+1] = n_C[ppi_l]+(n_C[ppi_l]-n_C[ppi_l-1])/2;
	if( n_C[ppi_l+1]>max_bin ) 
	{
		n_C[ppi_l+1] = (float)max_bin;
	}

	return ;
}
extern "C" DLLEXP_GSYS IArith *Create_arith (void)//用于创建Arith_258_CAP接口
{
	return new (::std::nothrow)Arith_258_CAP;
}
