//////////////////////////////////////////////////////////////////////////
// RadarStatusData.cpp
// created by dqc 20080328
//////////////////////////////////////////////////////////////////////////
// 功能：实现类RadarStatusData


#include "RadarStatusData.h"
//added by zhangyulong 20080504
#include <stdio.h>
#include <string.h>

RadarStatusData::RadarStatusData()
{
}

RadarStatusData::~RadarStatusData()
{
}

//added by zhangyulong 20080504
void RadarStatusData::NumstrtoString(char * &pCharstrDes, int pNumstrSrc)//实现int向字符串的转换
{
	int nlength=0;
	int temp = pNumstrSrc;
	while (temp)
	{
		temp /= 10;
		nlength++;
	}
	for ( int i=1;i<=nlength;i++ ) 
	{ 
		temp = pNumstrSrc%10;
		pNumstrSrc /= 10; 
		*(pCharstrDes+nlength-i) =(char) (temp+'0');
	} 
	*(pCharstrDes+nlength) = '\0';
}

//added by zhangyulong 20080504
void RadarStatusData::NumstrtoString(char *&pCharstrDes, float pNumstrSrc)//实现float向字符串的转换
{
	 sprintf(pCharstrDes, "%.4f", pNumstrSrc);

}

//Modified by zhangyulong 20080430
RADAR_STATUS_DATA  * RadarStatusData::GetWholeData()//返回整个状态数据
{
	return (RADAR_STATUS_DATA *)GetPointer(0);
}

int RadarStatusData::GetDate(void * pDate) //获得日期 gInt8 [9]
{
	pDate=GetWholeData()->STATUS_DATA;
	return 0;	
}

int RadarStatusData::GetTime(void * pTime) //获得时间 gInt8 [7]
{
	pTime=GetWholeData()->STATUS_TIME;
	return 0;	
}

int RadarStatusData::GetSel_Patt(char * pSelPatt) //获得体扫表 gInt32
{
	int temp;
	temp=GetWholeData()->SEL_PATT;
	NumstrtoString(pSelPatt,temp);
	return 0;
}

int RadarStatusData::GetLocalFlg(char * pLocalFlg) //获得控制方式 gInt32
{
	int temp;
	temp=GetWholeData()->LOCALFLG;
	NumstrtoString(pLocalFlg,temp);
	return 0;
}

int RadarStatusData::GetAntPeakPwr(char * pAntPeakPwr) //获得天线峰值功率 gFloat 
{
	float temp;
	temp=GetWholeData()->ANT_PEAK_PWR;
	NumstrtoString(pAntPeakPwr,temp);
	return 0;
}

int RadarStatusData::GetXmtrPeakPwr(char * pXmtrPeakPwr) //获得发射机峰值功率 gFloat
{
	float temp;
	temp= GetWholeData()->XMTR_PEAK_PWR;	
	NumstrtoString(pXmtrPeakPwr,temp);
	return 0;
}

int RadarStatusData::GetAntAvgPwr(char * pAntAvgPwr) //获得天线平均功率 gFloat
{
	float temp;
	temp=GetWholeData()->ANT_AVG_PWR;
	NumstrtoString(pAntAvgPwr,temp);
	return 0;
}

int RadarStatusData::GetXmtrAvgPwr(char * pXmtrAvgPwr) //获得发射机平均功率 gFloat
{
	float temp;
	temp=GetWholeData()->XMTR_AVG_PWR;
	NumstrtoString(pXmtrAvgPwr,temp);
	return 0;
}

int RadarStatusData::GetPowerMeterRatioDb(char * pPowerMeterRatioDb) //获得天线/发射机功率比 gFloat
{
	float temp;
	temp=GetWholeData()->POWER_METER_RATIO_DB;	
	NumstrtoString(pPowerMeterRatioDb,temp);
	return 0;
}

int RadarStatusData::GetAntPwrMtrZero(char * pAntPwrMtrZero) //获得天线功率计调零 gFloat
{
	float temp;
	temp=GetWholeData()->ANT_PWR_MTR_ZERO;	
	NumstrtoString(pAntPwrMtrZero,temp);
	return 0;
}

int RadarStatusData::GetXmtrPwrMtrZero(char * pXmtrPwrMtrZero) //获得发射机功率调零 gFloat
{
	float temp;
	temp=GetWholeData()->XMTR_PWR_MTR_ZERO;	
	NumstrtoString(pXmtrPwrMtrZero,temp);
	return 0;
}

int RadarStatusData::GetTxRecycleCnt(char * pTxRecycleCnt) //获得发射机重复循环次数 gInt32 ;2个gInt16 Spare
{
	int temp;
	temp=GetWholeData()->TX_RECYCLE_CNT;
	NumstrtoString(pTxRecycleCnt,temp);
	return 0;
}

int RadarStatusData::GetSpareOne(int * pSpare) //获得未知的空间 2个gInt16 Spare；暂时没有修改
{
	pSpare=NULL;
	for (int i=0;i < 2; i++)
	{
		* pSpare++ = GetWholeData()->spare1[i];
	}
	return 0;	
}

int RadarStatusData::GetRnspln(char * pRnspln) //获得窄脉冲噪声电平 gFloat
{
	float temp;
	temp=GetWholeData()->RNSPLN;	
	NumstrtoString(pRnspln,temp);
	return 0;
}

int RadarStatusData::GetRnsplg(char* pRnsplg) //获得?????????? gFloat
{
	float temp;
	temp=GetWholeData()->RNSPLG;		
	NumstrtoString(pRnsplg,temp);
	return 0;
}

int RadarStatusData::GetRnlpln(char * pRnlpln) //获得宽脉冲噪声电平 gFloat
{
	float temp;
	temp=GetWholeData()->RNLPLN;
	NumstrtoString(pRnlpln,temp);
	return 	0;
}

int RadarStatusData::GetRnlplg(char * pRnlplg) //获得??????????   gFloat
{
	float temp;
	temp=GetWholeData()->RNLPLG;	
	NumstrtoString(pRnlplg,temp);
	return 0;
}

int RadarStatusData::GetTe(char * pTe) //获得噪声温度 gFloat
{
	float temp;
	temp=GetWholeData()->TE;	
	NumstrtoString(pTe,temp);
	return 0;
}

int RadarStatusData::GetIduTestDetections(char * pIduTestDetections) //干扰检测数 gInt16; 1个gInt16 spare
{
	int temp;
	temp=GetWholeData()->IDU_TEST_DETECTIONS;
	NumstrtoString(pIduTestDetections,temp);
	return 0;
}

gInt16 RadarStatusData::GetSpareTwo() //获得未知的空间 1个gInt16 Spare;暂时没有修改
{
	return GetWholeData()->spare2;	
}

int RadarStatusData::GetAgcStepAmpl(char * pAgcStepAmpl,int NumOfGet) //AGC幅度 gFloat [6],NumOfGet表示第几次取数据
{	
	float  t_AgcStepAmpl;
	t_AgcStepAmpl = GetWholeData()->AGC_STEP_AMPL[NumOfGet];
	NumstrtoString(pAgcStepAmpl,t_AgcStepAmpl);
	return  0;
}

int RadarStatusData::GetAgcStepPhase(char * pAgcStepPhase,int NumOfGet) //AGC相位  gFloat [6],NumOfGet表示第几次取数据
{
	float t_AgcStepPhase;
	t_AgcStepPhase = GetWholeData()->AGC_STEP_PHASE[NumOfGet];
	NumstrtoString(pAgcStepPhase,t_AgcStepPhase);
	return 0;	
}

int RadarStatusData::GetAgcIqAmplBal(char * pAgcIqAmplBal) //AGC IQ幅度平衡 gFloat
{
	float temp;
	temp=GetWholeData()->AGC_IQ_AMPL_BAL;
	NumstrtoString(pAgcIqAmplBal,temp);
	return 0;
}

int RadarStatusData::GetAgcIqPhaseBal(char * pAgcIqPhaseBal) //AGC IQ相位平衡 gFloat ; 20个gInt16 spare
{
	float temp;
	temp=GetWholeData()->AGC_IQ_PHASE_BAL;
	NumstrtoString(pAgcIqPhaseBal,temp);
	return 0;
}

int RadarStatusData::GetSpareThree(short * pSpare) //获得未知的空间 20个gInt16 Spare;暂时没有修改
{
	pSpare=NULL;
	for (int i=0;i < 20; i++)
	{
		* pSpare++ = GetWholeData()->spare3[i];
	}
	return 0;	
}

int RadarStatusData::GetExpectedReflAmp(char * pExpectedReflAmp,int NumOfGet)	//反射率期望值  gFloat [8],NumOfGet表示第几次取数据
{
	float t_ExpectedReflAmp;
	t_ExpectedReflAmp = GetWholeData()->EXPECTED_REFL_AMP[NumOfGet];
	NumstrtoString(pExpectedReflAmp,t_ExpectedReflAmp);
	return 0;
}

int RadarStatusData::GetMeasuredReflAmp(char * pMeasuredReflAmp,int NumOfGet)	//反射率测量值  gFloat [8],NumOfGet表示第几次取数据
{
	float t_MeasuredReflAmp;
	t_MeasuredReflAmp = GetWholeData()->MEASURED_REFL_AMP[NumOfGet];
	NumstrtoString(pMeasuredReflAmp,t_MeasuredReflAmp);
	return 0;
}

int RadarStatusData::GetSysspln(char * pSysspln) //窄脉冲标定常数  gFloat
{
	float temp;
	temp=GetWholeData()->SYSSPLN;
	NumstrtoString(pSysspln,temp);
	return 0;
}

int RadarStatusData::GetSyssplg(char * pSyssplg) //?????????    gFloat
{
	float temp;
	temp=GetWholeData()->SYSSPLG;
	NumstrtoString(pSyssplg,temp);
	return 0;
}

int RadarStatusData::GetSyslpln(char * pSyslpln) //宽脉冲标定常数  gFloat
{
	float temp;
	temp= GetWholeData()->SYSLPLN;
	NumstrtoString(pSyslpln,temp);
	return 0;
}

int RadarStatusData::GetSyslplg(char * pSyslplg) //??????????  gFloat
{
	float temp;
	temp= GetWholeData()->SYSLPLG;
	NumstrtoString(pSyslplg,temp);
	return 0;
}

int RadarStatusData::GetExpectedVelocity(char * pExpectedVelocity,int NumOfGet) //速度期望值 ; gFloat [4];NumOfGet表示第几次取数据
{
	float t_ExpectedVelocity;
	t_ExpectedVelocity= GetWholeData()->EXPECTED_VELOCITY[NumOfGet];
	NumstrtoString(pExpectedVelocity,t_ExpectedVelocity);
	return 0;
}

int RadarStatusData::GetMeasuredVelocity(char *pMeasuredVelocity,int NumOfGet) //速度测量值 ; gFloat [4];NumOfGet表示第几次取数据
{
	float t_MeasuredVelocity;
	t_MeasuredVelocity=GetWholeData()->MEASURED_VELOCITY[NumOfGet];
	NumstrtoString(pMeasuredVelocity,t_MeasuredVelocity);
	return 0;
}

int RadarStatusData::GetExpectedWidth(char *pExpectedWidth,int NumOfGet) //谱宽期望值 ; gFloat [4];NumOfGet表示第几次取数据
{
	float t_ExpectedWidth;
	t_ExpectedWidth = GetWholeData()->EXPECTED_WIDTH[NumOfGet];
	NumstrtoString(pExpectedWidth,t_ExpectedWidth);
	return 0;
}

int RadarStatusData::GetMeasuredWidth(char *pMeasuredWidth,int NumOfGet) //谱宽测量值 ; gFloat [4]; 18个gInt16;NumOfGet表示第几次取数据
{
	float t_MeasuredWidth;
	t_MeasuredWidth = GetWholeData()->MEASURED_WIDTH[NumOfGet];
	NumstrtoString(pMeasuredWidth,t_MeasuredWidth);
	return 0;
}

int RadarStatusData::GetSpareFour(float * pSpare) //获得未知的空间 18个gInt16 Spare;暂时没有修改
{
	pSpare=NULL;
	for (int i=0;i < 18; i++)
	{
		* pSpare++ =GetWholeData()->spare4[i];
	}
	return 0;	
}

int RadarStatusData::GetExpectedRf8Amp(char * pExpectedRf8Amp,int NumOfGet)   //??????????? gFloat [6];NumOfGet表示第几次取数据
{
	float t_ExpectedRf8Amp;
	t_ExpectedRf8Amp = GetWholeData()->EXPECTED_RF8_AMP[NumOfGet];
	NumstrtoString(pExpectedRf8Amp,t_ExpectedRf8Amp);
	return 0;
}

int RadarStatusData::GetMeasuredRf8Amp(char * pMeasuredRf8Amp,int NumOfGet)   //??????????? gFloat [6]; 16个gInt16 spare;NumOfGet表示第几次取数据
{
	float t_MeasuredRf8Amp;
	t_MeasuredRf8Amp = GetWholeData()->MEASURED_RF8_AMP[NumOfGet];
	NumstrtoString(pMeasuredRf8Amp,t_MeasuredRf8Amp);
	return 0;
}

int RadarStatusData::GetSpareFive(short * pSpare) //获得未知的空间 16个gInt16 Spare;暂时没有修改
{
	pSpare=NULL;
	for (int i=0;i < 16; i++)
	{
		* pSpare++ = GetWholeData()->spare5[i];
	}
	return 0;	
}

int RadarStatusData::GetUnfilteredLinChanPwr(char * pUnfilteredLinChanPwr)   //滤波前功率 gFloat;
{
	float temp;
	temp= GetWholeData()->UNFILTERED_LIN_CHAN_PWR;
	NumstrtoString(pUnfilteredLinChanPwr,temp);
	return 0;
}

int RadarStatusData::GetFilteredLinChanPwr(char * pFilteredLinChanPwr)   //滤波后功率 gFloat
{
	float temp;
	temp=GetWholeData()->FILTERED_LIN_CHAN_PWR;
	NumstrtoString(pFilteredLinChanPwr,temp);
	return 0;
}

//delete by zhangyulong 20080430
/*void RadarStatusData::BiteRedundantWord (void * pData,int size)
{
	char * p = (char *)pData;
	*(p+size) = '\0';
	pData = p;

}

void RadarStatusData::GetWholeData(void * pRadarData)
{
	ReadData ( 0, READ_TYPE_UNKNOWN, pRadarData,sizeof(RADAR_STATUS_DATA) );	
	BiteRedundantWord(pRadarData,sizeof(RADAR_STATUS_DATA));

}

int RadarStatusData::GetDate(void * pDate) //获得日期 gInt8 [9]
{
	ReadData ( 0, READ_TYPE_UNKNOWN, pDate, 9*sizeof(gInt8) );	
	BiteRedundantWord(pDate,9*sizeof(gInt8));
	return 0;	
}

int RadarStatusData::GetTime(void * pTime) //获得时间 gInt8 [7]
{
	ReadData ( 9*sizeof(gInt8), READ_TYPE_UNKNOWN, pTime, 7*sizeof(gInt8) );
	BiteRedundantWord(pTime,7*sizeof(gInt8));
	return 0;	
}

int RadarStatusData::GetSel_Patt(void * pSelPatt) //获得体扫表 gInt32
{
	ReadData ( 16*sizeof(gInt8), READ_TYPE_UNKNOWN, pSelPatt, sizeof(gInt32) );	
	BiteRedundantWord(pSelPatt,sizeof(gInt32));
	return 0;	
}


int RadarStatusData::GetLocalFlg(void * pLocalFlg) //获得控制方式 gInt32
{
	ReadData ( 16*sizeof(gInt8)+sizeof(gInt32), READ_TYPE_UNKNOWN, pLocalFlg, sizeof(gInt32) );	
	BiteRedundantWord(pLocalFlg,sizeof(gInt32));
	return 0;	
}

int RadarStatusData::GetAntPeakPwr(void * pAntPeakPwr) //获得天线峰值功率 gFloat 
{
	ReadData ( 16*sizeof(gInt8)+2*sizeof(gInt32), READ_TYPE_UNKNOWN, pAntPeakPwr, sizeof(gFloat) );	
	BiteRedundantWord(pAntPeakPwr,sizeof(gFloat));
	return 0;	

}

int RadarStatusData::GetXmtrPeakPwr(void * pXmtrPeakPwr) //获得发射机峰值功率 gFloat
{
	ReadData ( 16*sizeof(gInt8)+2*sizeof(gInt32)+sizeof(gFloat), READ_TYPE_UNKNOWN, pXmtrPeakPwr, sizeof(gFloat) );	
	BiteRedundantWord(pXmtrPeakPwr,sizeof(gFloat));
	return 0;	
}

int RadarStatusData::GetAntAvgPwr(void * pAntAvgPwr) //获得天线平均功率 gFloat
{
	ReadData ( 16*sizeof(gInt8)+2*sizeof(gInt32)+2*sizeof(gFloat), READ_TYPE_UNKNOWN, pAntAvgPwr, sizeof(gFloat) );	
	BiteRedundantWord(pAntAvgPwr,sizeof(gFloat));
	return 0;
}

int RadarStatusData::GetXmtrAvgPwr(void * pXmtrAvgPwr) //获得发射机平均功率 gFloat
{
	ReadData ( 16*sizeof(gInt8)+2*sizeof(gInt32)+3*sizeof(gFloat), READ_TYPE_UNKNOWN, pXmtrAvgPwr, sizeof(gFloat) );
	BiteRedundantWord(pXmtrAvgPwr,sizeof(gFloat));
	return 0;
}

int RadarStatusData::GetPowerMeterRatioDb(void * pPowerMeterRatioDb) //获得天线/发射机功率比 gFloat
{
	ReadData ( 16*sizeof(gInt8)+2*sizeof(gInt32)+4*sizeof(gFloat), READ_TYPE_UNKNOWN, pPowerMeterRatioDb, sizeof(gFloat) );	
	BiteRedundantWord(pPowerMeterRatioDb,sizeof(gFloat));
	return 0;	
}

int RadarStatusData::GetAntPwrMtrZero(void * pAntPwrMtrZero) //获得天线功率计调零 gFloat
{
	ReadData ( 16*sizeof(gInt8)+2*sizeof(gInt32)+5*sizeof(gFloat), READ_TYPE_UNKNOWN, pAntPwrMtrZero, sizeof(gFloat) );	
	BiteRedundantWord(pAntPwrMtrZero,sizeof(gFloat));
	return 0;	
}

int RadarStatusData::GetXmtrPwrMtrZero(void * pXmtrPwrMtrZero) //获得发射机功率调零 gFloat
{
	ReadData ( 16*sizeof(gInt8)+2*sizeof(gInt32)+6*sizeof(gFloat), READ_TYPE_UNKNOWN, pXmtrPwrMtrZero, sizeof(gFloat) );
	BiteRedundantWord(pXmtrPwrMtrZero,sizeof(gFloat));
	return 0;	
}

int RadarStatusData::GetTxRecycleCnt(void * pTxRecycleCnt) //获得发射机重复循环次数 gInt32 ;2个gInt16 Spare
{
	ReadData ( 16*sizeof(gInt8)+2*sizeof(gInt32)+7*sizeof(gFloat), READ_TYPE_UNKNOWN, pTxRecycleCnt, sizeof(gInt32) );	
	BiteRedundantWord(pTxRecycleCnt,sizeof(gInt32));
	return 0;	
}

int RadarStatusData::GetSpareOne(void * pSpare) //获得未知的空间 2个gInt16 Spare
{
	ReadData ( 16*sizeof(gInt8)+3*sizeof(gInt32)+7*sizeof(gFloat), READ_TYPE_UNKNOWN, pSpare,2*sizeof(gInt16) );	
	BiteRedundantWord(pSpare,2*sizeof(gInt16));
	return 0;	
}

int RadarStatusData::GetRnspln(void * pRnspln) //获得窄脉冲噪声电平 gFloat
{
	ReadData ( 16*sizeof(gInt8)+2*sizeof(gInt16)+3*sizeof(gInt32)+7*sizeof(gFloat), READ_TYPE_UNKNOWN, pRnspln, sizeof(gFloat) );	
	BiteRedundantWord(pRnspln,sizeof(gFloat));
	return 0;	
}

int RadarStatusData::GetRnsplg(void * pRnsplg) //获得?????????? gFloat
{
	ReadData ( 16*sizeof(gInt8)+2*sizeof(gInt16)+3*sizeof(gInt32)+8*sizeof(gFloat), READ_TYPE_UNKNOWN, pRnsplg, sizeof(gFloat) );	
	BiteRedundantWord(pRnsplg,sizeof(gFloat));
	return 0;		
}

int RadarStatusData::GetRnlpln(void * pRnlpln) //获得宽脉冲噪声电平 gFloat
{
	ReadData ( 16*sizeof(gInt8)+2*sizeof(gInt16)+3*sizeof(gInt32)+9*sizeof(gFloat), READ_TYPE_UNKNOWN, pRnlpln, sizeof(gFloat) );
	BiteRedundantWord(pRnlpln,sizeof(gFloat));
	return 0;	
}

int RadarStatusData::GetRnlplg(void * pRnlplg) //获得??????????   gFloat
{
	ReadData ( 16*sizeof(gInt8)+2*sizeof(gInt16)+3*sizeof(gInt32)+10*sizeof(gFloat), READ_TYPE_UNKNOWN, pRnlplg, sizeof(gFloat) );
	BiteRedundantWord(pRnlplg,sizeof(gFloat));
	return 0;	
}

int RadarStatusData::GetTe(void * pTe) //获得噪声温度 gFloat
{
	ReadData ( 16*sizeof(gInt8)+2*sizeof(gInt16)+3*sizeof(gInt32)+11*sizeof(gFloat), READ_TYPE_UNKNOWN, pTe, sizeof(gFloat) );	
	BiteRedundantWord(pTe,sizeof(gFloat));
	return 0;	
}

int RadarStatusData::GetIduTestDetections(void * pIduTestDetections) //干扰检测数 gInt16; 1个gInt16 spare
{
	ReadData ( 16*sizeof(gInt8)+2*sizeof(gInt16)+3*sizeof(gInt32)+12*sizeof(gFloat), READ_TYPE_UNKNOWN, pIduTestDetections, sizeof(gInt16) );	
	BiteRedundantWord(pIduTestDetections,sizeof(gInt16));
	return 0;
}

int RadarStatusData::GetSpareTwo(void * pSpare) //获得未知的空间 1个gInt16 Spare
{
	ReadData ( 16*sizeof(gInt8)+3*sizeof(gInt16)+3*sizeof(gInt32)+12*sizeof(gFloat), READ_TYPE_UNKNOWN, pSpare,sizeof(gInt16) );	
	BiteRedundantWord(pSpare,sizeof(gInt16));
	return 0;	
}

int RadarStatusData::GetAgcStepAmpl(void * pAgcStepAmpl) //AGC幅度 gFloat [6]
{
	ReadData ( 16*sizeof(gInt8)+4*sizeof(gInt16)+3*sizeof(gInt32)+12*sizeof(gFloat), READ_TYPE_UNKNOWN, pAgcStepAmpl, 6*sizeof(gFloat) );
	BiteRedundantWord(pAgcStepAmpl,6*sizeof(gFloat));
	return 0;
}

int RadarStatusData::GetAgcStepPhase(void * pAgcStepPhase) //AGC相位  gFloat [6]
{
	ReadData ( 16*sizeof(gInt8)+4*sizeof(gInt16)+3*sizeof(gInt32)+18*sizeof(gFloat), READ_TYPE_UNKNOWN, pAgcStepPhase, 6*sizeof(gFloat) );	
	BiteRedundantWord(pAgcStepPhase,6*sizeof(gFloat));
	return 0;	
}

int RadarStatusData::GetAgcIqAmplBal(void * pAgcIqAmplBal) //AGC IQ幅度平衡 gFloat
{
	ReadData ( 16*sizeof(gInt8)+4*sizeof(gInt16)+3*sizeof(gInt32)+24*sizeof(gFloat), READ_TYPE_UNKNOWN, pAgcIqAmplBal, sizeof(gFloat) );
	BiteRedundantWord(pAgcIqAmplBal,sizeof(gFloat));
	return 0;
}

int RadarStatusData::GetAgcIqPhaseBal(void * pAgcIqPhaseBal) //AGC IQ相位平衡 gFloat ; 20个gInt16 spare
{
	ReadData ( 16*sizeof(gInt8)+4*sizeof(gInt16)+3*sizeof(gInt32)+25*sizeof(gFloat), READ_TYPE_UNKNOWN, pAgcIqPhaseBal, sizeof(gFloat));	
	BiteRedundantWord(pAgcIqPhaseBal,sizeof(gFloat));
	return 0;
}
int RadarStatusData::GetSpareThree(void * pSpare) //获得未知的空间 20个gInt16 Spare
{
	ReadData ( 16*sizeof(gInt8)+4*sizeof(gInt16)+3*sizeof(gInt32)+26*sizeof(gFloat), READ_TYPE_UNKNOWN, pSpare,20*sizeof(gInt16) );	
	BiteRedundantWord(pSpare,20*sizeof(gInt16));
	return 0;	
}

int RadarStatusData::GetExpectedReflAmp(void * pExpectedReflAmp)	//反射率期望值  gFloat [8]
{
	ReadData ( 16*sizeof(gInt8)+24*sizeof(gInt16)+3*sizeof(gInt32)+26*sizeof(gFloat), READ_TYPE_UNKNOWN, pExpectedReflAmp, 8*sizeof(gFloat));	
	BiteRedundantWord(pExpectedReflAmp,8*sizeof(gFloat));
	return 0;
}

int RadarStatusData::GetMeasuredReflAmp(void * pMeasuredReflAmp)	//反射率测量值  gFloat [8]
{
	ReadData ( 16*sizeof(gInt8)+24*sizeof(gInt16)+3*sizeof(gInt32)+34*sizeof(gFloat), READ_TYPE_UNKNOWN, pMeasuredReflAmp, 8*sizeof(gFloat));	
	BiteRedundantWord(pMeasuredReflAmp,8*sizeof(gFloat));
	return 0;
}

int RadarStatusData::GetSysspln(void * pSysspln) //窄脉冲标定常数  gFloat
{
	ReadData ( 16*sizeof(gInt8)+24*sizeof(gInt16)+3*sizeof(gInt32)+42*sizeof(gFloat), READ_TYPE_UNKNOWN, pSysspln, sizeof(gFloat));	
	BiteRedundantWord(pSysspln,sizeof(gFloat));
	return 0;
}

int RadarStatusData::GetSyssplg(void * pSyssplg) //?????????    gFloat
{
	ReadData ( 16*sizeof(gInt8)+24*sizeof(gInt16)+3*sizeof(gInt32)+43*sizeof(gFloat), READ_TYPE_UNKNOWN, pSyssplg, sizeof(gFloat));	
	BiteRedundantWord(pSyssplg,sizeof(gFloat));
	return 0;
}

int RadarStatusData::GetSyslpln(void * pSyslpln) //宽脉冲标定常数  gFloat
{
	ReadData ( 16*sizeof(gInt8)+24*sizeof(gInt16)+3*sizeof(gInt32)+44*sizeof(gFloat), READ_TYPE_UNKNOWN, pSyslpln, sizeof(gFloat));	
	BiteRedundantWord(pSyslpln,sizeof(gFloat));
	return 0;
}

int RadarStatusData::GetSyslplg(void * pSyslplg) //??????????  gFloat
{
	ReadData ( 16*sizeof(gInt8)+24*sizeof(gInt16)+3*sizeof(gInt32)+45*sizeof(gFloat), READ_TYPE_UNKNOWN, pSyslplg, sizeof(gFloat));	
	BiteRedundantWord(pSyslplg,sizeof(gFloat));
	return 0;
}

int RadarStatusData::GetExpectedVelocity(void *pExpectedVelocity) //速度期望值 ; gFloat [4];
{
	ReadData ( 16*sizeof(gInt8)+24*sizeof(gInt16)+3*sizeof(gInt32)+46*sizeof(gFloat), READ_TYPE_UNKNOWN, pExpectedVelocity, 4*sizeof(gFloat));	
	BiteRedundantWord(pExpectedVelocity,4*sizeof(gFloat));
	return 0;
}

int RadarStatusData::GetMeasuredVelocity(void *pMeasuredVelocity) //速度测量值 ; gFloat [4];
{
	ReadData ( 16*sizeof(gInt8)+24*sizeof(gInt16)+3*sizeof(gInt32)+50*sizeof(gFloat), READ_TYPE_UNKNOWN, pMeasuredVelocity, 4*sizeof(gFloat));	
	BiteRedundantWord(pMeasuredVelocity,4*sizeof(gFloat));
	return 0;
}

int RadarStatusData::GetExpectedWidth(void *pExpectedWidth) //谱宽期望值 ; gFloat [4];
{
	ReadData ( 16*sizeof(gInt8)+24*sizeof(gInt16)+3*sizeof(gInt32)+54*sizeof(gFloat), READ_TYPE_UNKNOWN, pExpectedWidth, 4*sizeof(gFloat));	
	BiteRedundantWord(pExpectedWidth,4*sizeof(gFloat));
	return 0;
}

int RadarStatusData::GetMeasuredWidth(void *pMeasuredWidth) //谱宽测量值 ; gFloat [4]; 18个gInt16
{
	ReadData ( 16*sizeof(gInt8)+24*sizeof(gInt16)+3*sizeof(gInt32)+58*sizeof(gFloat), READ_TYPE_UNKNOWN, pMeasuredWidth, 4*sizeof(gFloat));	
	BiteRedundantWord(pMeasuredWidth,4*sizeof(gFloat));
	return 0;
}

int RadarStatusData::GetSpareFour(void * pSpare) //获得未知的空间 18个gInt16 Spare
{
	ReadData (16*sizeof(gInt8)+24*sizeof(gInt16)+3*sizeof(gInt32)+62*sizeof(gFloat), READ_TYPE_UNKNOWN, pSpare,18*sizeof(gInt16) );	
	BiteRedundantWord(pSpare,18*sizeof(gInt16));
	return 0;	
}

int RadarStatusData::GetExpectedRf8Amp(void * pExpectedRf8Amp)   //??????????? gFloat [6];
{
	ReadData ( 16*sizeof(gInt8)+42*sizeof(gInt16)+3*sizeof(gInt32)+62*sizeof(gFloat), READ_TYPE_UNKNOWN, pExpectedRf8Amp, 6*sizeof(gFloat));	
	BiteRedundantWord(pExpectedRf8Amp,6*sizeof(gFloat));
	return 0;
}

int RadarStatusData::GetMeasuredRf8Amp(void * pMeasuredRf8Amp)   //??????????? gFloat [6]; 16个gInt16 spare
{
	ReadData ( 16*sizeof(gInt8)+42*sizeof(gInt16)+3*sizeof(gInt32)+68*sizeof(gFloat), READ_TYPE_UNKNOWN, pMeasuredRf8Amp, 6*sizeof(gFloat));	
	BiteRedundantWord(pMeasuredRf8Amp,6*sizeof(gFloat));
	return 0;

}

int RadarStatusData::GetSpareFive(void * pSpare) //获得未知的空间 16个gInt16 Spare
{
	ReadData ( 16*sizeof(gInt8)+42*sizeof(gInt16)+3*sizeof(gInt32)+74*sizeof(gFloat), READ_TYPE_UNKNOWN, pSpare,16*sizeof(gInt16) );	
	BiteRedundantWord(pSpare,16*sizeof(gInt16));
	return 0;	
}

int RadarStatusData::GetUnfilteredLinChanPwr(void * pUnfilteredLinChanPwr)   //滤波前功率 gFloat;
{
	ReadData ( 16*sizeof(gInt8)+58*sizeof(gInt16)+3*sizeof(gInt32)+74*sizeof(gFloat), READ_TYPE_UNKNOWN, pUnfilteredLinChanPwr, sizeof(gFloat));	
	BiteRedundantWord(pUnfilteredLinChanPwr,sizeof(gFloat));
	return 0;
}

int RadarStatusData::GetFilteredLinChanPwr(void * pFilteredLinChanPwr)   //滤波后功率 gFloat
{
	ReadData ( 16*sizeof(gInt8)+58*sizeof(gInt16)+3*sizeof(gInt32)+75*sizeof(gFloat), READ_TYPE_UNKNOWN, pFilteredLinChanPwr, sizeof(gFloat));	
	BiteRedundantWord(pFilteredLinChanPwr,sizeof(gFloat));
	return 0;
}
*/


GHRESULT RadarStatusData::WriteWholeData ( void * DataToWrite )
{
	return WriteData(0,WRITE_TYPE_UNKNOWN,DataToWrite,sizeof(RADAR_STATUS_DATA));
}


