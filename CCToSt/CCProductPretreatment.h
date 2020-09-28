
//////////////////////////////////////////////////////////////////////////
// CCProductPretreatment.h
// created by sunqc 20090327
// CC�״��Ʒ���ݸ�ʽ,
//ע����SA/SB/CB�״�����ݱ����ʽ��ÿ������ĸ�ʽ����ͬ������ĸ�ʽ��������ɨ��ʽ��������ɨ��һ��ͷ�ļ�����
//////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef CCPRODUCTPRETREATMENT_H
#define CCPRODUCTPRETREATMENT_H
#include "../GDataTypes/GData.h"
#include "../GDataTypes/GDef.h"


#pragma pack(push,1)//���������Ӧ��popһ��Ҫ���ϣ���֤����
//tagVPPISCANPARAMETER���ǲ�Ľṹ
typedef struct 
{
	gUint16 usMaxV;			//���ɲ��ٶ�(����/��)
	gUint16 usMaxL;			//���ɲ����(10��)
	gUint16 usBindWidth;		//�ⳤ(��)
	gUint16 usBinNumber;		//ÿ�������
	gUint16 usRecordNumber;	        //��Ȧ������
	gUint16 usArotate;		//��Ȧת��(0.01��/��)
	gUint16 usPrf1;			//��Ȧ��һ���ظ�Ƶ��(0.1Hz)
	gUint16 usPrf2;			//��Ȧ�ڶ����ظ�Ƶ��(0.1Hz)
	gUint16 usSpulseW;		//��Ȧ����(΢��)
	gInt16  usAngle;			//����(0.01��)
	gUint8  cSweepStatus;		//1=��Ҫ��	2=��Ҫ��(����Ƶ)	3=��Ҫ��(˫��Ƶ)
	gUint8 cAmbiguousp;		//0=�������ģ��	1=�����ģ��
}VPPISCANPARAMETER;
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

typedef struct 
{
	gChar cFileType[16];    //3830���ݱ�ʶ("CINRADC")
	//973�����ʶ("973")
	gChar cCountry[30];     //������
	gChar cProvince[20];    //ʡ��
	gChar cStation[40];     //վ��
	gChar cStationNumber[10];//��վ��
	gChar cRadarType[20];   //�״��ͺ�
	gChar cLongitude[16];   //�������ھ���
	gChar cLatitude[16];    //��������γ��
	gInt32 lLongitudeValue;  //���徭��  uint 1/1000 s
	gInt32 lLatitudeValue;   //����γ�� uint 1/1000 s
	gInt32 lHeight;	       //���ߺ��θ߶�  uint 1/1000 m
	gInt16 sMaxAngle;       //�����赲�������
	gInt16 sOptAngle;       //��ѹ۲�����
	gUint8 ucSYear1;	//�۲⿪ʼʱ�����ǧ��λ(19-20)
	gUint8 ucSYear2;	//�۲⿪ʼʱ�����ʮ��λ(00-99)
	gUint8 ucSMonth;	//�۲⿪ʼʱ�����(1-12)
	gUint8 ucSDay;  	//�۲⿪ʼʱ�����(1-31)
	gUint8 ucSHour; 	//�۲⿪ʼʱ���ʱ(0-23)
	gUint8 ucSMinute;//�۲⿪ʼʱ��ķ�(0-59)
	gUint8 ucSSecond;//�۲⿪ʼʱ�����(0-59)
	gUint8 ucTimeFrom;//ʱ����Դ 0-�����ʱ��(1����δ��ʱ)
	//		   1-�����ʱ��(1�����Ѷ�ʱ)
	//		   2-GPS
	//		   3-����
	gUint8 ucEYear1;	//�۲����ʱ�����ǧ��λ(19-20)
	gUint8 ucEYear2;	//�۲����ʱ�����ʮ��λ(00-99)
	gUint8 ucEMonth;	//�۲����ʱ�����(1-12)
	gUint8 ucEDay;  	//�۲����ʱ�����(1-31)
	gUint8 ucEHour; 	//�۲����ʱ���ʱ(0-23)
	gUint8 ucEMinute;//�۲����ʱ��ķ�(0-59)
	gUint8 ucESecond;//�۲����ʱ�����(0-59)
	gUint8 ucScanMode;//ɨ�跽ʽ  1-RHI  10-PPI��ZPPI
	//  1XX=VPPI(XXΪɨ��Ȧ��)
	gUint32 ulSmilliSecond;//��΢��Ϊ��λ��ʾ�����С��λ
	gUint16 usRHIA;	     //RHI���ڵķ�λ��(0.01��Ϊ��λ) PPI��
	//VPPIʱΪFFFF
	gInt16 sRHIL;		 //RHI���ڵ��������(0.01��Ϊ��λ) PPI��
	//VPPIʱΪFFFF
	gInt16 sRHIH;		//RHI���ڵ��������(0.01��Ϊ��λ) PPI��
	//VPPIʱΪFFFF
	gUint16 usEchoType;//�ز�����  0x405a-Z  0x406a-V
	// 0x407a-W  0x408a-ZVW��Ҫ��
	gUint16 usProdCode; //��������  0x8001-PPI����
	//  0x8002-RHI����
	//  0x8003-VPPI����
	//   0x8004-��ǿ��PPI����
	//   0x8005-CAPPI����
	gUint8 ucCalibration;//��У״̬  0-��  1-�Զ�
	//  2-1�������˹�  3-1�����˹�
	gUint8 remain1[3]; //������
	VPPISCANPARAMETER  vsp[30]; //��PPI��RHI�ļ�:ֻ��1��ṹ����; ��VPPI�ļ�:��N��ṹ����
	gInt32 lAntennaG;	//��������(0.001dB)
	gInt32 lPower;  	//��ֵ����(��)
	gInt32 lWavelength;  //����(΢��)
	gUint16 usBeamH;	//��ֱ�������(��)
	gUint16 usBeamL;	//ˮƽ�������(��)
	gUint16 usPolarization;//����״̬ 0-ˮƽ 1-��ֱ 2-˫ƫ��
	// 3-Բƫ�� 4-����
	gUint16 usLogA; //������̬��Χ(0.01dB)
	gUint16 usLineA;	//���Զ�̬��Χ(0.01dB)
	gUint16 usAGCP; //AGC�ӳ���(΢��)
	gUint16 usFreqMode;//Ƶ�ʷ�ʽ	1-���ظ�Ƶ��
	//  2-˫�ظ�Ƶ��3:2  3-˫�ظ�Ƶ��4:3
	gUint16 usFreqRepeat; //�ظ�Ƶ��
	gUint16 usPPPPulse;  //PPP������
	gUint16 usFFTPoint;//FFT�������
	gUint16 usProcessType;//�źŴ���ʽ	1-PPP
	//	2-ȫ��FFT  3-����FFT
	gUint8 ucClutterT;//�Ӳ�������ֵ(��STC)
	gChar cSidelobe;	 	//��һ�԰�(dB)
	gUint8 ucVelocityT; //�ٶ�����
	gUint8 ucFilderP;//����������ʽ	0-��
	//1-IIR�˲���1	2-IIR�˲���2
	//3-IIR�˲���3	4-IIR�˲���4
	gUint8 ucNoiseT;	//����������ֵ(��ǿ������)
	gUint8 ucSQIT;	//SQI����
	gUint8 ucIntensityC;//DVIPǿ��ֵ������õ�ͨ��
	// 1-����ͨ�� 2-����ͨ��
	gUint8 ucIntensityR; //ǿ��ֵ�����Ƿ���붩��
	// 0-��(dB) 1-�Ѷ���(dBZ)
	gUint8 ucCalNoise;//����ϵ���궨ֵ
	gUint8 ucCalPower;//���书�ʱ궨ֵ
	gUint8 ucCalPulseWidth; //�����ȱ궨ֵ
	gUint8 ucCalWorkFreq; //����Ƶ�ʱ궨ֵ
	gUint8 ucCalLog;	//����б�ʱ궨ֵ
	gChar remain3[95]; //������ ԭʼͷ�ļ�Ϊchar remain3[92]��
	//��������ֽ�Ϊ92����VC�ж������ļ�ͷ��1021�ֽڣ�����ƫ�Ƶ�ַΪ0��
	//����C++ Builder�ж������ļ�ͷ��1024�ֽڣ�����ƫ�Ƶ�ַΪ1024,
	//��������ֽ�Ϊ95����VC�ж������ļ�ͷ��1024�ֽڣ�����ƫ�Ƶ�ַΪ1024

	gUint32 liDataOffset;	//����ƫ�Ƶ�ַ
}CC_DATAHEADER;
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//3.��������
//���ڷ�973��ʽ���ļ���
typedef struct tagCC_DAT
{
	gInt16 z[500];//Z����(500����) ---- �Ŵ�10�����õ�dBz����
	gInt16 v[500];//V����(500����) ---- �Ŵ�10�����õ�m/s����
	gInt16 w[500];//W����(500����) ---- �Ŵ�10�����õ�m/s����
}DATA_CC;
/////////////////////////////////////////////////////////

class CCProductPretreatment :	public GenerationData
{
public:
	CCProductPretreatment();
	~CCProductPretreatment();

	CC_DATAHEADER * GetTArchiveCCHeadP( );
	gInt16 * GetTArchiveCCRP(gInt16 LayIndex,gInt16 RadialIndex);
	gInt16 * GetTArchiveCCVP(gInt16 LayIndex,gInt16 RadialIndex);
	gInt16 * GetTArchiveCCWP(gInt16 LayIndex,gInt16 RadialIndex);
	DATA_CC * GetTArchiveCC_RVWP(gInt16 LayIndex);
	gInt16 GetLayerNubP();
private:
	inline CC_DATAHEADER * GetTArchiveCCHead ( );
	inline gInt16 * GetTArchiveCCR(gInt16 LayIndex,gInt16 RadialIndex);
	inline gInt16 * GetTArchiveCCV(gInt16 LayIndex,gInt16 RadialIndex);
	inline gInt16 * GetTArchiveCCW(gInt16 LayIndex,gInt16 RadialIndex);
	inline DATA_CC * GetTArchiveCC_RVW(gInt16 LayIndex);
	inline gInt16 GetLayerNub();
};
#pragma pack(pop)
#endif
