#ifndef IARITHMS_LIB
	#define IARITHMS_LIB
#endif
#include "Arith_271_OHP.hpp"
#include "../Arith_262_HBR_270_QPR/Product_270_QPR.h"
#include "SingleProductBase.h"
#include  "GDataFiles.h"
#include <fstream>

#define SECONDS_PER_DAY 86400

Arith_271_OHP::Arith_271_OHP()
{

}
Arith_271_OHP::~Arith_271_OHP()
{

}

GHRESULT Arith_271_OHP::Initialize()
{
	m_pInputList = NULL;
	m_pOutputList = NULL;
	return GS_OK;
};
GHRESULT Arith_271_OHP::UnInitialize()
{
	return GS_OK;
};

GHRESULT Arith_271_OHP::LoadData(void* pdata, ReadParam * Paramter)
{
	if(pdata == NULL) return GE_INVALIDARG;
	if(!Paramter) return GE_INVALIDARG;

	m_pInputList = (GXList<GenerationData *>*)pdata;

	m_fnParam = Paramter;

	return GS_OK;
};

GHRESULT Arith_271_OHP::OutPutData(void* pdata)
{
	if(pdata == NULL) return GE_INVALIDARG;

	m_pOutputList = (GXList<GenerationData>*)pdata;

	return GS_OK;
};

extern "C" DLLEXP_GSYS IArith *Create_arith (void)//用于创建Arith_271_OHP接口
{
	return new (::std::nothrow)Arith_271_OHP;
}

GHRESULT Arith_271_OHP::Execute()
{
	ofstream logfile;
	logfile.open("./Log/Arith_271_OHP.txt",ios::app);
	gUint16 iJulDay;
	gInt32 iMilliSecond,iYear,iMonth,iDay,iHour,iMinute,iSecond,iMSecond;
	CJulMsGMT::GetLocalJulMs(iJulDay, iMilliSecond );
	CJulMsGMT::Jul2Greg(iJulDay,iYear,iMonth,iDay);
	CJulMsGMT::MilliSecond2Time(iMilliSecond,iHour,iMinute,iSecond,iMSecond);
	logfile<<endl<<iHour<<":"<<iMinute<<":"<<iSecond<<"	"<<iYear<<"-"<<iMonth<<"-"<<iDay<<endl;
	logfile<<"271_OHP：  Started: "<<endl;

	DATE_LTC	req_date;
	logfile<<"271_OHP：  Read Parameter: ScanDateTime."<<endl;
	char  *ScanDateTime  ;
	(*m_fnParam)("ScanDateTime",ScanDateTime);
	if (ScanDateTime == NULL )
	{
		return GE_INVALIDARG;
	}

	logfile<<"271_OHP：  Call Function ParseScantime() to Analyze Volume ScanDateTime."<<endl;
	//分析体扫时间
	gInt32 year , month,day,hour,min,sec;
	ParseScantime(ScanDateTime,year,month, day, hour,min, sec);

	logfile << "请求时间（时分秒表示）："<< ScanDateTime << endl;

	req_date.set(year,month, day, hour,min, sec);

	//FilterQPRData(req_date);
	gUint16 VolumeScanDate =  CJulMsGMT::Greg2Jul(year,month, day);
	gInt32  VolumeScanTime = 0;
	//VolumeScanTime = CJulMsGMT::Time2MilliSecond(hour,min, sec,0)/1000;
	VolumeScanTime = CJulMsGMT::Time2MilliSecond(hour,min, sec,0); //modify by dqc
	logfile << "请求时间（秒表示）："<< VolumeScanTime/1000 << endl;
	int cnt = (int)m_pInputList->GetCount();

	logfile<<"271_OHP：  Analyze and Judge ScanDateTime of Send-in Products."<<endl;
	Product_270_QPR *in = NULL;
	gInt32 timecha = 0;
	int cout = 0 ;
	for(int i = cnt-1; i >=0 ; i-- )
	{
		in = &(*(m_pInputList->GetAtAs<Product_270_QPR>(i)));
		timecha = (VolumeScanDate - (in->GetPDBlock()->ProductDate))*86400000 + (VolumeScanTime - (in->GetPDBlock()->ProductTime));
		if( timecha < 1800000)  //从请求时间往前半小时内的QPR产品计数
			cout++;	
	}
	if(cout ==0) // 若半小时内QPR产品计数为0，则认为不宜生成此请求时间的OHP产品。
	{
		logfile<<"271_OHP：  The ScanDateTime doesn't fill requirement and return GE_SHORTOFINDATA."<<endl;
		logfile<<"271_OHP：  Finished."<<endl;
		logfile.close();

		return GE_SHORTOFINDATA;
	}

	if(cnt == 1)
	{
		logfile<<"271_OHP：  Count of Send-in Product is 1."<<endl;
		logfile<<"271_OHP：  Create Send-out Product."<<endl;

		m_pOutputList->AddTail();

		GenerationData& out = *(m_pOutputList->GetHeadAs<GenerationData>());
		GenerationData& in = (*(m_pInputList->GetHeadAs<GenerationData>()));

		out = in;
		Product_271_OHP* pPdt = (Product_271_OHP*)(&out);
		/*	pPdt->GetGrid_Data(0);*/

		PDBlock* pdb;
		pdb = pPdt->GetPDBlock();
		pdb->ProductCode = 271;
		pdb->ProductDate = VolumeScanDate;
		pdb->ProductTime = VolumeScanTime;

		CJulMsGMT::Jul2Greg(pdb->ProductDate,iYear,iMonth,iDay);
		CJulMsGMT::MilliSecond2Time(pdb->ProductTime,iHour,iMinute,iSecond,iMSecond);
		logfile<<"271_OHP：  VCPDate："<<iYear<<"-"<<iMonth<<"-"<<iDay<<"		VCPTime："<<iHour<<":"<<iMinute<<":"<<iSecond<<endl;

		CJulMsGMT::GetLocalJulMs(pdb->GenerationDateOfProduct,pdb->GenerationTimeOfProduct);

		logfile<<"271_OHP：  Finished."<<endl;
		logfile.close();
		return GS_OK;
	}
	else if(cnt <= 0)
	{
		logfile<<"271_OHP：  Send-in Product's Count is less than or equals 0. So return GE_SHORTOFINDATA."<<endl;
		logfile<<"271_OHP：  Finished."<<endl;
		return GE_SHORTOFINDATA;
	}

	bool sum_flag = true;

	double v_sec[20];// Max count : 20
	//for(int i = 0; i < 20; i++)
	//	v_sec[i] = 0;

	gUint16 date;
	gInt32  time;

	CommonProduct* pcb = NULL;
	PDBlock*  pdb = NULL;

	gInt32	QPE_sec = 3600;

	CJulianTime jt(BTC_seconds);

	logfile<<"271_OHP：  Loop: i=0-->cnt("<<cnt<<") to Calculate array v_sec["<<cnt<<"]"<<endl;

	logfile << "计算得到的起始时间req_date.start_time（秒表示）："<< req_date.start_time << endl;
	for(int i = 0; i < cnt; ++i)
	{
		logfile<<"271_OHP：   Get m_pInputList["<<i<<"] and Calculate v_sec["<<i<<"]"<<endl;
		pcb = &(*(m_pInputList->GetAtAs<CommonProduct>(i)));
		pdb = pcb->GetPDBlockP();
		date = pdb->ProductDate;
		time = pdb->ProductTime/1000;

		//jt.set_time(date, time, LTC);
		//jt.get_time(date, time, UTC);

		if(req_date.start_date == date)
		{
			v_sec[i] = static_cast<double>(time - req_date.start_time);
		}
		else if( req_date.start_date == date - 1)
		{
			v_sec[i] = static_cast<double>(SECONDS_PER_DAY+ time - req_date.start_time);
		}
		else if(req_date.start_date == date + 1)
		{
			v_sec[i] = static_cast<double>(SECONDS_PER_DAY*-1 + time  - req_date.start_time);
		}
		logfile << "第几"<< i<< "个体扫时间date："<< date << "time （秒）="<< time<< endl;
		logfile << "v_sec ["<<i <<"]  ="<< v_sec[i] << endl;
	}
	logfile<<"271_OHP：  Loop Finished."<<endl;

	for(int j = 1; j < cnt; ++j)
	{
		int dv = (int)(v_sec[j] - v_sec[j-1]);

		if(dv > 930)
		{
			logfile<<"271_OHP：  Array v_sec["<<cnt<<"] doesn't fill requirement."<<endl;
			logfile<<"271_OHP：  Return GE_SHORTOFINDATA."<<endl;
			logfile.close();

			return GE_SHORTOFINDATA;
		}
	}


	logfile<<"271_OHP：  Calculate Array fmul["<<cnt<<"]."<<endl;

	double fmul[20];

	if(v_sec[0] < 0)
	{
		fmul[0] = (v_sec[0] + v_sec[1])/2.0;

		if(fmul[0] < 0.0) fmul[0] = 0.0;

		fmul[1] = (v_sec[1] + v_sec[2])/2.0;
		logfile << "00000000000000fmul["<< 0<< "]= "<<fmul[0]<< "fmul["<< 1<< "]= "<<fmul[1] << endl;
	}
	else
	{
		fmul[0] = /*v_sec[0] +*/ (v_sec[1] + v_sec[0])/2.0;
		logfile << "111111111fmul["<< 0<< "]= "<<fmul[0] << endl;
		if( cnt >2)
		{
			fmul[1] = (v_sec[2] - v_sec[0])/2.0;
			logfile << "111111111fmul["<< 1<< "]= "<<fmul[1]<<  endl;
		}		
	}

	if(v_sec[cnt - 1] > QPE_sec)
	{
		if(cnt >= 2)
		{
			fmul[cnt - 1] = QPE_sec - (v_sec[cnt-1] + v_sec[cnt-2])/2.0;

			if(fmul[cnt - 1] < 0.0)
				fmul[cnt-1] = 0.0;
		}
		if(cnt > 3)
			fmul[cnt-2] = QPE_sec - (v_sec[cnt-3] + v_sec[cnt-2])/2.0 - fmul[cnt-1];

		logfile << "n11111111fmul["<< cnt - 1<< "]= "<<fmul[cnt - 1]<< "fmul["<< cnt-2<< "]= "<<fmul[cnt-2] << endl;
	}
	else
	{
		if(cnt >= 2)
		{
			fmul[cnt-1] = QPE_sec - (v_sec[cnt-1] + v_sec[cnt-2])/2.0;
			logfile << "n222222222222222fmul["<< cnt - 1<< "]= "<<fmul[cnt - 1]<< endl;
		}
		if(cnt > 3)
		{
			fmul[cnt-2] = (v_sec[cnt-1] - v_sec[cnt-3])/2.0;
			logfile << "n222222222222222fmul["<< cnt - 2<< "]= "<<fmul[cnt - 2]<<  endl;

		}
	}

	for(int j = 2; j < cnt -2 ; ++j)
	{
		fmul[j] = (v_sec[j+1] - v_sec[j-1])/2.0;
		logfile << "2-nfmul["<< j<< "]= "<<fmul[j] << endl;

	}

	float sum = 0;
	for(int j = 0; j < cnt; ++j) 
	{
		fmul[j] /= 3600.0;
		sum += fmul[j];
		logfile << "fmul["<< j<< "]= "<<fmul[j] << endl;
	}
	logfile<< "fmul----sum = "<< sum << endl;
	/*if( fabs(sum -1.0) < 0.000001 )
	{
	logfile<<"271_OHP： sum of fmul[...] != 1."<<endl;
	return GE_SHORTOFINDATA;
	}*/



	m_pOutputList->AddTail();

	 int w = 480;
	 int h = 480;

	logfile<<"271_OHP：  Get the last Send-in Product in the List of Product_270_QPR."<<endl;
	Product_270_QPR& PdtQPR = (*(m_pInputList->GetTailAs<Product_270_QPR>()));//20140210

	/*  GRADARDATAHEADER*    pRadarHdr = (pstdRadar->GetHeadP());*/
	PDBlock*  pPdtQPR = PdtQPR.GetPDBlock();


	logfile<<"271_OHP：  Create Send-out Product: Product_271_OHP. "<<endl;
	Product_271_OHP* pPdt = &(*(m_pOutputList->GetTailAs<Product_271_OHP>()));

	 w= PdtQPR.GetGrid_Head()->NumberOfRows;
	 h= PdtQPR.GetGrid_Head()->NumberOfCols;
	GHRESULT ConstrutHand;
	ConstrutHand = pPdt->ConstructIt(w,h);
	if(ConstrutHand != GS_OK)
	{
		logfile<<"271_OHP：  Create New Product Failed and Return GE_FAILOFNEWPRODUMEM."<<endl;
		logfile.close();
		return GE_FAILOFNEWPRODUMEM;
	}


	logfile<<"271_OHP：  Get and Set RadarInfor and Radial_Head Information."<<endl;

	pdb = pPdt->GetPDBlock();

	pdb->ProductCode = 271;
	//pdb->ElevationNumber = -1;
	/* pdb->CinradType = pRadarHdr->RadarType;
	pdb->Height = pRadarHdr->RadarHeight;
	pdb->Latitude = pRadarHdr->RadarLat;
	pdb->Longitude = pRadarHdr->RadarLon;
	pdb->VCP = pRadarHdr->VCPMode;
	pdb->OperationalMode = pRadarHdr->ScanMode;
	pdb->VolumeScanDate = pRadarHdr->VCPDate;
	pdb->VolumeScanTime = pRadarHdr->VCPTime;
	CJulMsGMT::GetLocalJulMs(pdb->GenerationDateOfProduct,
	pdb->GenerationTimeOfProduct);
	memcpy(pdb->SiteID, pRadarHdr->SiteCode, sizeof(gInt8)*8);
	memcpy(pdb->SiteName, pRadarHdr->SiteName, sizeof(gInt8)*18);*/  //add by sunqc 20080716
	memcpy(pPdt->GetRadarInfor()->RadarType, PdtQPR.GetRadarInfor()->RadarType, sizeof(gInt8)* 4);
	pPdt->GetRadarInfor()->Altitude = PdtQPR.GetRadarInfor()->Altitude;
	pPdt->GetRadarInfor()->Latitude = PdtQPR.GetRadarInfor()->Latitude;
	pPdt->GetRadarInfor()->Longitude = PdtQPR.GetRadarInfor()->Longitude;
	memcpy(pdb->VCPMode, pPdtQPR->VCPMode, sizeof(gInt8) * 4);
	memcpy(pdb->OperationalMode, pPdtQPR->OperationalMode, sizeof(gInt8)*2);
	pdb->ProductDate = VolumeScanDate;
	pdb->ProductTime = VolumeScanTime;

	CJulMsGMT::Jul2Greg(pdb->ProductDate,iYear,iMonth,iDay);
	CJulMsGMT::MilliSecond2Time(pdb->ProductTime,iHour,iMinute,iSecond,iMSecond);
	logfile<<"271_OHP：  VCPDate："<<iYear<<"-"<<iMonth<<"-"<<iDay<<"		VCPTime："<<iHour<<":"<<iMinute<<":"<<iSecond<<endl;

	pdb->ScaleOfData = 100; 
	pdb->IndexOfTabular = -1;
	pdb->IndexOfGraphic = -1;
	CJulMsGMT::GetLocalJulMs(pdb->GenerationDateOfProduct,
		pdb->GenerationTimeOfProduct);
	memcpy(pPdt->GetRadarInfor()->SiteCode, PdtQPR.GetRadarInfor()->SiteCode, sizeof(gInt8)*8);
	//memcpy(pdb->SiteName, pPdtQPR->SiteName, sizeof(gInt8)*18);
	DILayer* dil;

	dil = pPdt->GetLayer();
	dil->HeightOfElevation = 0;
	GridHead*	ghd;
	ghd = pPdt->GetGrid_Head();

	//ghd->PacketCode = 0x0B00;
	//ghd->NumberOfRows = w;
	//ghd->NumberOfCols = h;
	ghd->ResolutionOfRow = (PdtQPR.GetGrid_Head())->ResolutionOfRow;
	ghd->ResolutionOfCol = (PdtQPR.GetGrid_Head())->ResolutionOfCol;
	logfile<<"271_OHP：  rows ="<<h<< "w="<< w << "resrow ="<<ghd->ResolutionOfRow <<"rescol ="<<ghd->ResolutionOfCol <<endl;
	//ghd->ScaleOfData = 100;
	Product_270_QPR* pQPR;

	/* pOutData = pPdt->GetGrid_Data(0);

	gInt16* pOutWritePtr = pOutData;

	memset(pOutWritePtr, -10, sizeof(gInt16)*w*h);*/
	gInt16* pData = NULL;
	gInt16* pOutData = NULL;

	logfile<<"271_OHP：  Loop: m=0-->h("<<h<<") to Initial Send-out Product Data as 0"<<endl;
	for( int m = 0; m < h ; ++m)
	{
		pOutData = pPdt->GetGrid_Data(m);
		memset(pOutData,0,sizeof(gInt16)*w);
	}
	logfile<<"271_OHP：  Loop Finished."<<endl;

	logfile<<"271_OHP：  Loop: i=0-->cnt("<<cnt<<") to Calculate Send-out Product Data"<<endl;
	for(int k = 0; k < cnt; ++k)
	{
		if( fmul[k] < 0.001 ) continue;
		pQPR = &(*(m_pInputList->GetAtAs<Product_270_QPR>(k)));
		for( int m = 0; m < h ; ++m)
		{
			pData = pQPR->GetGrid_Data(m);
			pOutData = pPdt->GetGrid_Data(m);
			for(int n = 0; n < w; ++n)
			{
				if(pData[n]> 5)
					pOutData[n] += (gInt16)((double)pData[n]* fmul[k]);
			}
		}
	}
	logfile<<"271_OHP：  Loop Finished."<<endl;
	logfile<<"271_OHP：  Finished."<<endl;
	logfile.close();

	return GS_OK;
};

GHRESULT Arith_271_OHP::FilterQPRData(DATE_LTC& date)
{
	// GData objects in m_pInputList have been sorted by date and time.
	int cnt = (int)(m_pInputList->GetCount());

	CommonProduct* pcb = NULL;
	PDBlock* pdb = NULL;

	date.set_boundary_symmetry_sub_area(7*60, 7*60);

	int	start_num[2] = {-1,-1};
	int	end_num[2] = {-1,-1};

	for(int i = 0; i < cnt; ++i)
	{
		// pcb = static_cast<CommonProduct*>(&(*(m_pInputList->GetAt(0))));
		// pcb = (CommonProduct*)(&(m_pInputList->GetAt(i)));
		pcb = *(m_pInputList->GetAtAs<CommonProduct *>(i));

		pdb = pcb->GetPDBlockP();

		if(pdb->ProductDate >= date.tar_date &&
			pdb->ProductTime/1000 > date.tar_time)
		{
			break;
		}

		//|<--L--|---->|
		if(pdb->ProductDate >= date.s_date_sub[0] &&
			pdb->ProductTime/1000 >= date.s_time_sub[0] &&
			pdb->ProductDate <= date.start_date &&
			pdb->ProductTime/1000 < date.start_time)
		{
			start_num[0] = i;
		}
		if(pdb->ProductDate <= date.s_date_sub[1] &&
			pdb->ProductTime/1000 < date.s_time_sub[1] &&
			pdb->ProductDate >= date.start_date &&
			pdb->ProductTime/1000 >= date.start_time)
		{
			start_num[1] = i;
		}
		if(pdb->ProductDate >= date.t_date_sub[0] &&
			pdb->ProductTime/1000 >= date.t_time_sub[0] &&
			pdb->ProductDate <= date.tar_date &&
			pdb->ProductTime/1000 < date.tar_time)
		{
			end_num[0] = i;
		}
		if(pdb->ProductDate <= date.t_date_sub[1] &&
			pdb->ProductTime/1000 < date.t_time_sub[1] &&
			pdb->ProductDate >= date.tar_date &&
			pdb->ProductTime/1000 >= date.tar_time)
		{
			end_num[1] = i;
		}
	}

	int st = (start_num[0] != -1)?(start_num[0]):(start_num[1]);
	int ed = (end_num[0] != -1)?(end_num[0]):(end_num[1]);

	if(st == -1 && ed != -1) st = 0;
	else if(ed == -1 && st != -1) ed = cnt - 1;

	if( st == -1 || ed == -1)
	{
		return GE_FAIL;
	}


	GXList<GenerationData *>::iterator itr = m_pInputList->GetAt(ed);

	++itr;

	fnc_RemoveListItem<GenerationData *> fnc_gdata;
	GXList<GenerationData *>::iterator itr2;
	itr2 = m_pInputList->end();
	do_each_on((*m_pInputList), itr, itr2, fnc_gdata);

	itr = m_pInputList->GetAt(st);
	itr2 = m_pInputList->begin();
	do_each_on((*m_pInputList), itr2, itr, fnc_gdata);

	return GS_OK;
};

void Arith_271_OHP::ParseScantime(char *ScanDateTime,gInt32& year,gInt32& month,gInt32& day,gInt32& hour,gInt32& min,gInt32& sec)
{
	int i=0;
	int j=0;
	char *ptemp = new (::std::nothrow)char[10];
	for (j=0;j<4;j++)
	{
		*(ptemp+j)=*(ScanDateTime+i);
		i++;
	}
	*(ptemp+j)='\0';
	/*year=stringToInt(ptemp);*/ //md by sunqc 20080827
	year=atoi(ptemp);

	for (j=0;j<2;j++)
	{
		*(ptemp+j)=*(ScanDateTime+i);
		i++;
	}
	*(ptemp+j)='\0';
	/*month=stringToInt(ptemp);*/
	month=atoi(ptemp);
	for (j=0;j<2;j++)
	{
		*(ptemp+j)=*(ScanDateTime+i);
		i++;
	}
	*(ptemp+j)='\0';
	/*day=stringToInt(ptemp);*/
	day=atoi(ptemp);

	for (j=0;j<2;j++)
	{
		*(ptemp+j)=*(ScanDateTime+i);
		i++;
	}
	*(ptemp+j)='\0';
	//hour=stringToInt(ptemp);
	hour=atoi(ptemp);

	for (j=0;j<2;j++)
	{
		*(ptemp+j)=*(ScanDateTime+i);
		i++;
	}
	*(ptemp+j)='\0';
	/*min=stringToInt(ptemp);*/
	min=atoi(ptemp);

	for (j=0;j<2;j++)
	{
		*(ptemp+j)=*(ScanDateTime+i);
		i++;
	}
	*(ptemp+j)='\0';
	/*sec=stringToInt(ptemp);*/
	sec=atoi(ptemp);

	if (ptemp != NULL)
	{
		delete[] ptemp;
		ptemp = NULL;
	}
};
/*int Arith_271_OHP::stringToInt(char *s) // md by sunqc20080827
{
int i=0;
int nlength =(int) strlen(s);
int j = nlength;
int k;
int ntemp;
int result = 0;
bool Isminus=false;

if(*s=='-') //如果是负数
{
Isminus=true;
s++;
nlength--;
}
j = nlength;
while (i<nlength)
{
ntemp = (*(s)-48);
for (k=1;k<j;k++)
{
ntemp *= 10;
}
result += ntemp;
i++;
j--;
s++;
}
if (Isminus) //如果是负数
{
result=-result;
}

return result;
};*/
