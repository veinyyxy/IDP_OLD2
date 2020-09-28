//////////////////////////////////////////////////////////////////////////
// RadarStatusData.h
// created by dqc 20080328
//////////////////////////////////////////////////////////////////////////
// 功能：1、提供本系统统一的雷达状态数据格式
//       2、提供对雷达状态数据读取和存储的格式

#pragma once

#ifndef			RADARSTATUSDATA_H
#define			RADARSTATUSDATA_H 

#pragma pack(push,1) //保证对齐 使用字节对齐

//#include "../GData.h"
#include "GData.h"

typedef struct
{
	gInt8  STATUS_DATA[9];       	// 日期
	gInt8  STATUS_TIME[7];       	// 时间

	gInt32   SEL_PATT;            		// 体扫表
	gInt32   LOCALFLG;            		// 控制方式
	gFloat ANT_PEAK_PWR;			// 天线峰值功率   
	gFloat XMTR_PEAK_PWR;			// 发射机峰值功率
	gFloat ANT_AVG_PWR;			// 天线平均功率
	gFloat XMTR_AVG_PWR;			// 发射机平均功率
	gFloat POWER_METER_RATIO_DB;		// 天线/发射机功率比
	gFloat ANT_PWR_MTR_ZERO;		// 天线功率计调零
	gFloat XMTR_PWR_MTR_ZERO;		// 发射机功率调零

	gInt32   TX_RECYCLE_CNT;		// 发射机重复循环次数
	gInt16 spare1[2];
	gFloat RNSPLN;				//窄脉冲噪声电平
	gFloat RNSPLG;
	gFloat RNLPLN;				//宽脉冲噪声电平
	gFloat RNLPLG;
	gFloat TE;				//噪声温度

	gInt16 IDU_TEST_DETECTIONS;		//干扰检测数
	gInt16 spare2;
	gFloat AGC_STEP_AMPL[6];		//AGC幅度
	gFloat AGC_STEP_PHASE[6];		//AGC相位
	gFloat AGC_IQ_AMPL_BAL;		//AGC IQ幅度平衡	
	gFloat AGC_IQ_PHASE_BAL;		//AGC IQ相位平衡
    gInt16 spare3[20];

	
	gFloat EXPECTED_REFL_AMP[8];		//反射率期望值
	gFloat MEASURED_REFL_AMP[8];		//反射率测量值
	gFloat SYSSPLN;			//窄脉冲标定常数
	gFloat SYSSPLG;
	gFloat SYSLPLN;			//宽脉冲标定常数
	gFloat SYSLPLG;


	gFloat EXPECTED_VELOCITY[4];		//速度期望值
	gFloat MEASURED_VELOCITY[4];		//速度测量值
	gFloat EXPECTED_WIDTH[4];		//谱宽期望值
	gFloat MEASURED_WIDTH[4];		//谱宽测量值
	gInt16 spare4[18];

	gFloat EXPECTED_RF8_AMP[6];
	gFloat MEASURED_RF8_AMP[6];
	gInt16 spare5[16];
	gFloat UNFILTERED_LIN_CHAN_PWR;	//滤波前功率
	gFloat FILTERED_LIN_CHAN_PWR;	//滤波后功率
}RADAR_STATUS_DATA;

class RadarStatusData :
	public GenerationData
{
public:
	RadarStatusData();
public:
	~RadarStatusData();

	//modified by zhangyulong 20080430
	//void GetWholeData(void * pRadarData); //将整个状态数据存放到pRadarData
	//读数据
	RADAR_STATUS_DATA * GetWholeData(); //返回整个状态数据
	
	int GetDate(void * pDate); //获得日期，可以用ReadData，并将其存储在void *定义的变量中
	int GetTime(void * pTime); //获得时间

	int GetSel_Patt(char * pSelPatt); //获得体扫表
	int GetLocalFlg(char * pLocalFlg); //获得控制方式
	int GetAntPeakPwr(char * pAntPeakPwr); //获得天线峰值功率 
	int GetXmtrPeakPwr(char * pXmtrPeakPwr); //获得发射机峰值功率
	int GetAntAvgPwr(char * pAntAvgPwr); //获得天线平均功率
	int GetXmtrAvgPwr(char * XmtrAvgPwr); //获得发射机平均功率
	int GetPowerMeterRatioDb(char * pPowerMeterRatioDb); //获得天线/发射机功率比
	int GetAntPwrMtrZero(char * pAntPwrMtrZero); //获得天线功率计调零
	int GetXmtrPwrMtrZero(char * pXmtrPwrMtrZero); //获得发射机功率调零

	int GetTxRecycleCnt(char * pTxRecycleCnt); //获得发射机重复循环次数
	int GetSpareOne(int * pSpare); //获得未知的空间(待用)
	int GetRnspln(char * pRnspln); //获得窄脉冲噪声电平
	int GetRnsplg(char * pRnsplg); //获得??????????
	int GetRnlpln(char * pRnlpln); //获得宽脉冲噪声电平
	int GetRnlplg(char * pRnlplg); //获得??????????
	int GetTe(char * pTe); //获得噪声温度

	int GetIduTestDetections(char * pIduTestDetections); //干扰检测数 gInt16; 1个gInt16 spare
	gInt16 GetSpareTwo(); //获得未知的空间(待用)
	int GetAgcStepAmpl(char * pAgcStepAmpl,int NumOfGet); //AGC幅度 gFloat [6]
	int GetAgcStepPhase(char * pAgcStepPhase,int NumOfGet); //AGC相位  gFloat [6]
	int GetAgcIqAmplBal(char * pAgcIqAmplBal); //AGC IQ幅度平衡 gFloat
	int GetAgcIqPhaseBal(char * pAgcIqPhaseBal); //AGC IQ相位平衡 gFloat; 20个gInt16 spare
	int GetSpareThree(short * pSpare); //获得未知的空间(待用)

	int GetExpectedReflAmp(char * pExpectedReflAmp,int NumOfGet);		//反射率期望值  gFloat [8]
	int GetMeasuredReflAmp(char * pMeasuredReflAmp,int NumOfGet);	//反射率测量值  gFloat [8]
	int GetSysspln(char * pSysspln); //窄脉冲标定常数  gFloat
	int GetSyssplg(char * pSyssplg); //?????????    gFloat
	int GetSyslpln(char * pSyslpln); //宽脉冲标定常数  gFloat
	int GetSyslplg(char * pSyslplg); //??????????  gFloat

	int GetExpectedVelocity(char *pExpectedVelocity,int NumOfGet); //速度期望值 ; gFloat [4];
	int GetMeasuredVelocity(char *pMeasuredVelocity,int NumOfGet); //速度测量值 ; gFloat [4];
	int GetExpectedWidth(char *pExpectedWidth,int NumOfGet); //谱宽期望值 ; gFloat [4];
	int GetMeasuredWidth(char *pMeasuredWidth,int NumOfGet); //谱宽测量值 ; gFloat [4]; 18个gInt16 spare
	int GetSpareFour(float * pSpare); //获得未知的空间(待用)

	int GetExpectedRf8Amp(char * pExpectedRf8Amp,int NumOfGet);   //??????????? gFloat [6];
	int GetMeasuredRf8Amp(char * pMeasuredRf8Amp,int NumOfGet);   //??????????? gFloat [6]; 16个gInt16 spare
	int GetSpareFive(short * pSpare); //获得未知的空间(待用)
	int GetUnfilteredLinChanPwr(char * pUnfilteredLinChanPwr);   //滤波前功率 gFloat;
	int GetFilteredLinChanPwr(char * pFilteredLinChanPwr);   //滤波后功率 gFloat
	

	//deleted by zhangyulong 20080430

	/*int GetSel_Patt(void * pSelPatt); //获得体扫表
	int GetLocalFlg(void * pLocalFlg); //获得控制方式
	int GetAntPeakPwr(void * pAntPeakPwr); //获得天线峰值功率 
	int GetXmtrPeakPwr(void * pXmtrPeakPwr); //获得发射机峰值功率
	int GetAntAvgPwr(void * pAntAvgPwr); //获得天线平均功率
	int GetXmtrAvgPwr(void * XmtrAvgPwr); //获得发射机平均功率
	int GetPowerMeterRatioDb(void * pPowerMeterRatioDb); //获得天线/发射机功率比
	int GetAntPwrMtrZero(void * pAntPwrMtrZero); //获得天线功率计调零
	int GetXmtrPwrMtrZero(void * pXmtrPwrMtrZero); //获得发射机功率调零

    int GetTxRecycleCnt(void * pTxRecycleCnt); //获得发射机重复循环次数
	int GetSpareOne(void * pSpare); //获得未知的空间(待用)
	int GetRnspln(void * pRnspln); //获得窄脉冲噪声电平
	int GetRnsplg(void * pRnsplg); //获得??????????
	int GetRnlpln(void * pRnlpln); //获得宽脉冲噪声电平
	int GetRnlplg(void * pRnlplg); //获得??????????
	int GetTe(void * pTe); //获得噪声温度

	int GetIduTestDetections(void * pIduTestDetections); //干扰检测数 gInt16; 1个gInt16 spare
	int GetSpareTwo(void * pSpare); //获得未知的空间(待用)
	int GetAgcStepAmpl(void * pAgcStepAmpl); //AGC幅度 gFloat [6]
	int GetAgcStepPhase(void * pAgcStepPhase); //AGC相位  gFloat [6]
	int GetAgcIqAmplBal(void * pAgcIqAmplBal); //AGC IQ幅度平衡 gFloat
	int GetAgcIqPhaseBal(void * pAgcIqPhaseBal); //AGC IQ相位平衡 gFloat; 20个gInt16 spare
	int GetSpareThree(void * pSpare); //获得未知的空间(待用)

	int GetExpectedReflAmp(void * pExpectedReflAmp);		//反射率期望值  gFloat [8]
	int GetMeasuredReflAmp(void * pMeasuredReflAmp);	//反射率测量值  gFloat [8]
	int GetSysspln(void * pSysspln); //窄脉冲标定常数  gFloat
	int GetSyssplg(void * pSyssplg); //?????????    gFloat
	int GetSyslpln(void * pSyslpln); //宽脉冲标定常数  gFloat
	int GetSyslplg(void * pSyslplg); //??????????  gFloat

	int GetExpectedVelocity(void *pExpectedVelocity); //速度期望值 ; gFloat [4];
	int GetMeasuredVelocity(void *pMeasuredVelocity); //速度测量值 ; gFloat [4];
	int GetExpectedWidth(void *pExpectedWidth); //谱宽期望值 ; gFloat [4];
	int GetMeasuredWidth(void *pMeasuredWidth); //谱宽测量值 ; gFloat [4]; 18个gInt16 spare
	int GetSpareFour(void * pSpare); //获得未知的空间(待用)

	int GetExpectedRf8Amp(void * pExpectedRf8Amp);   //??????????? gFloat [6];
	int GetMeasuredRf8Amp(void * pMeasuredRf8Amp);   //??????????? gFloat [6]; 16个gInt16 spare
	int GetSpareFive(void * pSpare); //获得未知的空间(待用)
	int GetUnfilteredLinChanPwr(void * pUnfilteredLinChanPwr);   //滤波前功率 gFloat;
	int GetFilteredLinChanPwr(void * pFilteredLinChanPwr);   //滤波后功率 gFloat
*/
	
	
	//写数据
	GHRESULT WriteWholeData ( void * DataToWrite ); //将整个状态数据存入


private:
	//added by zhangyulong for datatype transition 20080504
	void NumstrtoString(char * &pCharstrDes, int pNumstrSrc);//将int型数据转成字符串
	void NumstrtoString(char * &pCharstrDes, float pNumstrSrc);//将float型数据转成字符串
//deleted by zhangyulong for unused 20080430
//	void BiteRedundantWord (void * pData,int size); //将pData中多余的乱码字符去掉，只截取size大小的


};

#pragma pack(pop)
#endif
