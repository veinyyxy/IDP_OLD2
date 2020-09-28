//////////////////////////////////////////////////////////////////////////
// RadarStatusData.h
// created by dqc 20080328
//////////////////////////////////////////////////////////////////////////
// ���ܣ�1���ṩ��ϵͳͳһ���״�״̬���ݸ�ʽ
//       2���ṩ���״�״̬���ݶ�ȡ�ʹ洢�ĸ�ʽ

#pragma once

#ifndef			RADARSTATUSDATA_H
#define			RADARSTATUSDATA_H 

#pragma pack(push,1) //��֤���� ʹ���ֽڶ���

//#include "../GData.h"
#include "GData.h"

typedef struct
{
	gInt8  STATUS_DATA[9];       	// ����
	gInt8  STATUS_TIME[7];       	// ʱ��

	gInt32   SEL_PATT;            		// ��ɨ��
	gInt32   LOCALFLG;            		// ���Ʒ�ʽ
	gFloat ANT_PEAK_PWR;			// ���߷�ֵ����   
	gFloat XMTR_PEAK_PWR;			// �������ֵ����
	gFloat ANT_AVG_PWR;			// ����ƽ������
	gFloat XMTR_AVG_PWR;			// �����ƽ������
	gFloat POWER_METER_RATIO_DB;		// ����/��������ʱ�
	gFloat ANT_PWR_MTR_ZERO;		// ���߹��ʼƵ���
	gFloat XMTR_PWR_MTR_ZERO;		// ��������ʵ���

	gInt32   TX_RECYCLE_CNT;		// ������ظ�ѭ������
	gInt16 spare1[2];
	gFloat RNSPLN;				//խ����������ƽ
	gFloat RNSPLG;
	gFloat RNLPLN;				//������������ƽ
	gFloat RNLPLG;
	gFloat TE;				//�����¶�

	gInt16 IDU_TEST_DETECTIONS;		//���ż����
	gInt16 spare2;
	gFloat AGC_STEP_AMPL[6];		//AGC����
	gFloat AGC_STEP_PHASE[6];		//AGC��λ
	gFloat AGC_IQ_AMPL_BAL;		//AGC IQ����ƽ��	
	gFloat AGC_IQ_PHASE_BAL;		//AGC IQ��λƽ��
    gInt16 spare3[20];

	
	gFloat EXPECTED_REFL_AMP[8];		//����������ֵ
	gFloat MEASURED_REFL_AMP[8];		//�����ʲ���ֵ
	gFloat SYSSPLN;			//խ����궨����
	gFloat SYSSPLG;
	gFloat SYSLPLN;			//������궨����
	gFloat SYSLPLG;


	gFloat EXPECTED_VELOCITY[4];		//�ٶ�����ֵ
	gFloat MEASURED_VELOCITY[4];		//�ٶȲ���ֵ
	gFloat EXPECTED_WIDTH[4];		//�׿�����ֵ
	gFloat MEASURED_WIDTH[4];		//�׿����ֵ
	gInt16 spare4[18];

	gFloat EXPECTED_RF8_AMP[6];
	gFloat MEASURED_RF8_AMP[6];
	gInt16 spare5[16];
	gFloat UNFILTERED_LIN_CHAN_PWR;	//�˲�ǰ����
	gFloat FILTERED_LIN_CHAN_PWR;	//�˲�����
}RADAR_STATUS_DATA;

class RadarStatusData :
	public GenerationData
{
public:
	RadarStatusData();
public:
	~RadarStatusData();

	//modified by zhangyulong 20080430
	//void GetWholeData(void * pRadarData); //������״̬���ݴ�ŵ�pRadarData
	//������
	RADAR_STATUS_DATA * GetWholeData(); //��������״̬����
	
	int GetDate(void * pDate); //������ڣ�������ReadData��������洢��void *����ı�����
	int GetTime(void * pTime); //���ʱ��

	int GetSel_Patt(char * pSelPatt); //�����ɨ��
	int GetLocalFlg(char * pLocalFlg); //��ÿ��Ʒ�ʽ
	int GetAntPeakPwr(char * pAntPeakPwr); //������߷�ֵ���� 
	int GetXmtrPeakPwr(char * pXmtrPeakPwr); //��÷������ֵ����
	int GetAntAvgPwr(char * pAntAvgPwr); //�������ƽ������
	int GetXmtrAvgPwr(char * XmtrAvgPwr); //��÷����ƽ������
	int GetPowerMeterRatioDb(char * pPowerMeterRatioDb); //�������/��������ʱ�
	int GetAntPwrMtrZero(char * pAntPwrMtrZero); //������߹��ʼƵ���
	int GetXmtrPwrMtrZero(char * pXmtrPwrMtrZero); //��÷�������ʵ���

	int GetTxRecycleCnt(char * pTxRecycleCnt); //��÷�����ظ�ѭ������
	int GetSpareOne(int * pSpare); //���δ֪�Ŀռ�(����)
	int GetRnspln(char * pRnspln); //���խ����������ƽ
	int GetRnsplg(char * pRnsplg); //���??????????
	int GetRnlpln(char * pRnlpln); //��ÿ�����������ƽ
	int GetRnlplg(char * pRnlplg); //���??????????
	int GetTe(char * pTe); //��������¶�

	int GetIduTestDetections(char * pIduTestDetections); //���ż���� gInt16; 1��gInt16 spare
	gInt16 GetSpareTwo(); //���δ֪�Ŀռ�(����)
	int GetAgcStepAmpl(char * pAgcStepAmpl,int NumOfGet); //AGC���� gFloat [6]
	int GetAgcStepPhase(char * pAgcStepPhase,int NumOfGet); //AGC��λ  gFloat [6]
	int GetAgcIqAmplBal(char * pAgcIqAmplBal); //AGC IQ����ƽ�� gFloat
	int GetAgcIqPhaseBal(char * pAgcIqPhaseBal); //AGC IQ��λƽ�� gFloat; 20��gInt16 spare
	int GetSpareThree(short * pSpare); //���δ֪�Ŀռ�(����)

	int GetExpectedReflAmp(char * pExpectedReflAmp,int NumOfGet);		//����������ֵ  gFloat [8]
	int GetMeasuredReflAmp(char * pMeasuredReflAmp,int NumOfGet);	//�����ʲ���ֵ  gFloat [8]
	int GetSysspln(char * pSysspln); //խ����궨����  gFloat
	int GetSyssplg(char * pSyssplg); //?????????    gFloat
	int GetSyslpln(char * pSyslpln); //������궨����  gFloat
	int GetSyslplg(char * pSyslplg); //??????????  gFloat

	int GetExpectedVelocity(char *pExpectedVelocity,int NumOfGet); //�ٶ�����ֵ ; gFloat [4];
	int GetMeasuredVelocity(char *pMeasuredVelocity,int NumOfGet); //�ٶȲ���ֵ ; gFloat [4];
	int GetExpectedWidth(char *pExpectedWidth,int NumOfGet); //�׿�����ֵ ; gFloat [4];
	int GetMeasuredWidth(char *pMeasuredWidth,int NumOfGet); //�׿����ֵ ; gFloat [4]; 18��gInt16 spare
	int GetSpareFour(float * pSpare); //���δ֪�Ŀռ�(����)

	int GetExpectedRf8Amp(char * pExpectedRf8Amp,int NumOfGet);   //??????????? gFloat [6];
	int GetMeasuredRf8Amp(char * pMeasuredRf8Amp,int NumOfGet);   //??????????? gFloat [6]; 16��gInt16 spare
	int GetSpareFive(short * pSpare); //���δ֪�Ŀռ�(����)
	int GetUnfilteredLinChanPwr(char * pUnfilteredLinChanPwr);   //�˲�ǰ���� gFloat;
	int GetFilteredLinChanPwr(char * pFilteredLinChanPwr);   //�˲����� gFloat
	

	//deleted by zhangyulong 20080430

	/*int GetSel_Patt(void * pSelPatt); //�����ɨ��
	int GetLocalFlg(void * pLocalFlg); //��ÿ��Ʒ�ʽ
	int GetAntPeakPwr(void * pAntPeakPwr); //������߷�ֵ���� 
	int GetXmtrPeakPwr(void * pXmtrPeakPwr); //��÷������ֵ����
	int GetAntAvgPwr(void * pAntAvgPwr); //�������ƽ������
	int GetXmtrAvgPwr(void * XmtrAvgPwr); //��÷����ƽ������
	int GetPowerMeterRatioDb(void * pPowerMeterRatioDb); //�������/��������ʱ�
	int GetAntPwrMtrZero(void * pAntPwrMtrZero); //������߹��ʼƵ���
	int GetXmtrPwrMtrZero(void * pXmtrPwrMtrZero); //��÷�������ʵ���

    int GetTxRecycleCnt(void * pTxRecycleCnt); //��÷�����ظ�ѭ������
	int GetSpareOne(void * pSpare); //���δ֪�Ŀռ�(����)
	int GetRnspln(void * pRnspln); //���խ����������ƽ
	int GetRnsplg(void * pRnsplg); //���??????????
	int GetRnlpln(void * pRnlpln); //��ÿ�����������ƽ
	int GetRnlplg(void * pRnlplg); //���??????????
	int GetTe(void * pTe); //��������¶�

	int GetIduTestDetections(void * pIduTestDetections); //���ż���� gInt16; 1��gInt16 spare
	int GetSpareTwo(void * pSpare); //���δ֪�Ŀռ�(����)
	int GetAgcStepAmpl(void * pAgcStepAmpl); //AGC���� gFloat [6]
	int GetAgcStepPhase(void * pAgcStepPhase); //AGC��λ  gFloat [6]
	int GetAgcIqAmplBal(void * pAgcIqAmplBal); //AGC IQ����ƽ�� gFloat
	int GetAgcIqPhaseBal(void * pAgcIqPhaseBal); //AGC IQ��λƽ�� gFloat; 20��gInt16 spare
	int GetSpareThree(void * pSpare); //���δ֪�Ŀռ�(����)

	int GetExpectedReflAmp(void * pExpectedReflAmp);		//����������ֵ  gFloat [8]
	int GetMeasuredReflAmp(void * pMeasuredReflAmp);	//�����ʲ���ֵ  gFloat [8]
	int GetSysspln(void * pSysspln); //խ����궨����  gFloat
	int GetSyssplg(void * pSyssplg); //?????????    gFloat
	int GetSyslpln(void * pSyslpln); //������궨����  gFloat
	int GetSyslplg(void * pSyslplg); //??????????  gFloat

	int GetExpectedVelocity(void *pExpectedVelocity); //�ٶ�����ֵ ; gFloat [4];
	int GetMeasuredVelocity(void *pMeasuredVelocity); //�ٶȲ���ֵ ; gFloat [4];
	int GetExpectedWidth(void *pExpectedWidth); //�׿�����ֵ ; gFloat [4];
	int GetMeasuredWidth(void *pMeasuredWidth); //�׿����ֵ ; gFloat [4]; 18��gInt16 spare
	int GetSpareFour(void * pSpare); //���δ֪�Ŀռ�(����)

	int GetExpectedRf8Amp(void * pExpectedRf8Amp);   //??????????? gFloat [6];
	int GetMeasuredRf8Amp(void * pMeasuredRf8Amp);   //??????????? gFloat [6]; 16��gInt16 spare
	int GetSpareFive(void * pSpare); //���δ֪�Ŀռ�(����)
	int GetUnfilteredLinChanPwr(void * pUnfilteredLinChanPwr);   //�˲�ǰ���� gFloat;
	int GetFilteredLinChanPwr(void * pFilteredLinChanPwr);   //�˲����� gFloat
*/
	
	
	//д����
	GHRESULT WriteWholeData ( void * DataToWrite ); //������״̬���ݴ���


private:
	//added by zhangyulong for datatype transition 20080504
	void NumstrtoString(char * &pCharstrDes, int pNumstrSrc);//��int������ת���ַ���
	void NumstrtoString(char * &pCharstrDes, float pNumstrSrc);//��float������ת���ַ���
//deleted by zhangyulong for unused 20080430
//	void BiteRedundantWord (void * pData,int size); //��pData�ж���������ַ�ȥ����ֻ��ȡsize��С��


};

#pragma pack(pop)
#endif
