
//////////////////////////////////////////////////////////////////////////
// CCProductPretreatment.h
// created by sunqc 20090327
// CC�״��Ʒ���ݸ�ʽ,
//ע����SA/SB/CB�״�����ݱ����ʽ��ÿ������ĸ�ʽ����ͬ������ĸ�ʽ��������ɨ��ʽ��������ɨ��һ��ͷ�ļ�����
//////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef CCPRODUCTPRETREATMENT2_H
#define CCPRODUCTPRETREATMENT2_H
#include "GData.h"
#include "GDef.h"


#pragma pack(push,1)//���������Ӧ��popһ��Ҫ���ϣ���֤����

typedef struct tagLayerParam
{
	// ����۲�Ҫ��
	unsigned char DataType;       //1=��ǿ��, 2=��Ҫ�� ��PRF, 3=��Ҫ�� ˫PRF
                                  //4=˫��ƫ��, 5=˫��ƫ�������, 6=˫��������ͬ����),7=˫�������������ߣ�
	
    //�������ٶ�ģ��״̬
	unsigned char Ambiguousp ;	  //0 = �����ٶ�ģ��״̬,1 = ������ٶ�ģ��, 2 = ˫T���ٶ�ģ��
								  //3 = ��ʽ���ٶ�ģ��, 4 = ˫T + ������ٶ�ģ��, 5 = ��ʽ + ������ٶ�ģ��
								  //6 = ˫PPI���ٶ�ģ��, 9 = ������ʽ,
	unsigned short Arotate;       //  ��������ת��,������λΪ0.01��/�룬
	                              // ��ɨ�跽ʽΪRHI��PPIʱ��ֻ�ڵ�һ��Ԫ������д������Ԫ��Ϊ0��				      (2-3)
	unsigned short PRF1 ;         // �����һ�������ظ�Ƶ��,������λ:1/10Hz (4-5)
	unsigned short PRF2	;         //  ����ڶ��������ظ�Ƶ��,������λ:1/10Hz (6-7)
	unsigned short SpulseW;       //  �����������,������λΪ΢��		  (8-9)
	unsigned short MaxV ;         //  ��������ɲ��ٶ�,������λΪ����/��	(10-11)
	unsigned short MaxL	;         //  ��������ɲ����,��10��Ϊ������λ	(12-13)
	unsigned short ZBinWidth ;    //  ����ǿ�����ݵĿⳤ,��1/10��Ϊ������λ(14-15)
	unsigned short VBinWidth ;    //  �����ٶ����ݵĿⳤ,��1/10��Ϊ������λ(16-17)
	unsigned short WBinWidth ;    //  �����׿����ݵĿⳤ,��1/10��Ϊ������λ(18-19)
	unsigned short ZBinNumber ;   //  ����ɨ��ǿ�Ⱦ���Ŀ���				(20-21)
	unsigned short VBinNumber ;   //  ����ɨ���ٶȾ���Ŀ���				(22-23)
	unsigned short WBinNumber ;	  //  ����ɨ���׿���Ŀ���				(24-25)
	unsigned short RecordNumber ; //  ����ɨ�辶�����						(26-27)
	short SwpAngles ;             //  ���������,������λΪ1/100��.
								  //  ��ɨ�跽ʽ:RHI��������飬PPIʱ��һ��Ԫ��Ϊ��PPIʱ�����ǣ�������λΪ1/100�ȣ�����Ԫ����д-32768��		(28-29)
	char DataForm ;               //  ���㾶���е��������з�ʽ:	
					  // 11��Ҫ�����У� CorZ
					  // 12��Ҫ�����У� UnZ
					  // 13��Ҫ�����У� V
					  // 14��Ҫ�����У� W
					  // 21��Ҫ�����У�CorZ+ UnZ
					  // 22��Ҫ�����У�CorZ+ V+ W
					  // 23��Ҫ�����У�UnZ+ V+ W
					  // 24��Ҫ�����У�CorZ+UnZ+ V+ W
					  // 4x˫ƫ��Ҫ������ģʽ
					  // 6x˫ƫ������հ�Ҫ������ģʽ
					  // 8x˫������Ҫ������ģʽ
	unsigned long int Dbegin ;    //  �������ݼ�¼��ʼλ��(�ֽ���)
}LAYERPARAM ;

typedef struct tagBinParam
{
	short Code;   	//			   ǿ�ȱ�ⳤ�ṹ����			
	short Begin;  	//			   ��ʼ��ľ��룬��10��Ϊ������λ
	short End;     	//			   ������ľ��룬��10��Ϊ������λ	(4-5)
	short BinLength; //			   �ⳤ����1/10��Ϊ������λ		(6-7)
}BINPARAM ;

typedef struct tagFileVolumn //ԭʼ�����ļ���ʶ��12�ֽڣ�
{
	char   FileID[4];		    // 'RD' Ϊ�״�ԭʼ����,'GD'Ϊ�״���������
	float  VersionNo;		    // ��ʾ���ݸ�ʽ�İ汾�� 2.0
	long int HeaderLength;  // ��ʾ�ļ�ͷ�ĳ���
} FILEVOLUMN;

typedef  struct  RadDataHeader  //ԭʼ�����ļ�ͷ��2048�ֽڣ�
{
         //վַ����������ݽṹ,��168���ֽ�
	char Country[30];         	//  ���������ı���ʽ����	
	char Province[20];       	//  ʡ�����ı���ʽ����	
	char Station[40];          	//  վ�����ı���ʽ����
	char StationNumber[10];    	//	��վ�ţ��ı���ʽ����
	char RadarType[20];         //	�״��ͺţ��ı���ʽ����
	char Longitude[16];       	//	�������ھ���: E115��32'12"
	char Latitude[16];          //  ��������γ�ȣ�N35��30'15"	
	long int LongitudeValue;    //  ���߾��ȣ���1/1000��Ϊ������λ,����Ϊ��������Ϊ��
	long int LatitudeValue ;    //  ����γ�ȣ���1/1000��Ϊ������λ,��γΪ������γΪ��
	long int Height;        	//	���ߺ��θ߶��Ժ���Ϊ������λ
	short MaxAngle;             //  ��վ���ܵ�������ڵ����ǣ���1/100��Ϊ������λ��						(176-177)
	short OptiAngle;            //  ��վ����ѹ۲����ǣ�����ز�ǿ��<10dBZ����1/100��Ϊ������λ	(178-179)

  		 //���ܲ�������36���ֽڣ�
	long int AntennaG ;         // ����������0.001dBΪ������λ	
	unsigned short VerBeamW ;   // ��ֱ���������1/100��Ϊ������λ
	unsigned short HorBeamW ;   // ˮƽ���������1/100��Ϊ������λ
	unsigned char Polarizations; // ƫ��״��(0=ˮƽ,1=��ֱ,2=˫��ƫ��,3=Բƫ��,4=����)
	unsigned short SideLobe;    // ��һ�԰꣬��0.01dBΪ������λ
	long int Power;             // �״������ֵ���ʣ�����Ϊ������λ
	long int WaveLength;        // ��������΢��Ϊ������λ
	unsigned short LogA;        // �������ջ���̬��Χ����0.01dBΪ������λ
	unsigned short LineA;    	// ���Խ��ջ���̬��Χ����0.01dBΪ������λ
	unsigned short AGCP;        // AGC�ӳ�������΢��Ϊ������λ
	unsigned short LogMinPower; // �������ջ���С�ɲ⹦�ʣ�������λΪ0.01dBm
	unsigned short LineMinPower; // ���Խ��ջ���С�ɲ⹦�ʣ�������λΪ0.01dBm
	unsigned char ClutterT;     // �Ӳ�������ֵ��������λΪ0.01dB
	unsigned char VelocityP;    // �ٶȴ���ʽ(0=���ٶȴ���,1=PPP,2=FFT)
	unsigned char FilterP;      // �����Ӳ�������ʽ:  0=�޵����Ӳ�����
								//	  1=�����Ӳ�ͼ�۳���
								//	  2=�����Ӳ�ͼ+�˲�������
								//	  3=�˲�������
								//	  4=�׷�������
								//	  5=��������
	unsigned char NoiseT;       // ����������ֵ��0-255��
	unsigned char SQIT;         // SQI��ֵ����0.01Ϊ������λ 
	unsigned char IntensityC ;  // RVPǿ��ֵ������õ�ͨ��(1=����ͨ��,2=����ͨ��)
	unsigned char IntensityR ;  // ǿ�ȹ����Ƿ�����˾��붩��(0=��,1=�ѽ����˾��붩��)

		//�۲��������1282���ֽڣ� 
	unsigned char SType;        // ɨ�跽ʽ(1=RHI,10=PPI,1XX=VOL��XXΪɨ�����)
	unsigned short SYear;       // �۲��¼��ʼʱ����꣨2001��
	unsigned char SMonth;		// �۲��¼��ʼʱ����£�1-12��
	unsigned char SDay;			// �۲��¼��ʼʱ����գ�1-31��
	unsigned char SHour;		// �۲��¼��ʼʱ���ʱ��00-23��
	unsigned char SMinute;		// �۲��¼��ʼʱ��ķ֣�00-59��
	unsigned char SSecond;		// �۲��¼��ʼʱ����루00-59��
	unsigned char TimeP; 		// ʱ����Դ
								// 0=�����ʱ�ӣ���һ����δ���ж�ʱ
								// 1=�����ʱ�ӣ�һ�����ѽ��ж�ʱ
								// 2=GPS
								// 3=����
	unsigned long int SMillisecond; // ���С��λ��������λ΢�룩
	unsigned char Calibration; 		// ��У״̬
						// 0=�ޱ�У
						// 1=�Զ���У
						// 2=һ�������˹���У
						// 3=һ�����˹���У
						// �����벻��
	unsigned char IntensityI; 	// ǿ�Ȼ��ִ�����32-128��
	unsigned char  VPSamples;	// �ٶȴ���������31-255��
	unsigned short ZStartBin; 	// ǿ����Ч���ݿ�ʼ����
	unsigned short VStartBin;  	// �ٶ���Ч���ݿ�ʼ����
	unsigned short WStartBin;  	// �׿���Ч���ݿ�ʼ����
	LAYERPARAM LayerParam[32];      // ��������ݽṹ(��Ȧɨ��״̬����)(238-1357)
	unsigned short RHIA;		// RHIΪ��λ�ǣ�������λΪ1/100�ȣ�PPI������ɨ��ʱΪ65535
	short RHIL;			// RHIΪ������ǣ�������λΪ1/100�ȣ�������ɨ��ʱΪ-32768
	short RHIH;			// RHIΪ������ǣ�������λΪ1/100�ȣ�������ɨ��ʱΪ-32768
	unsigned short EYear;		// �۲��¼����ʱ����꣨2000 -   ��
	unsigned char EMonth;		// �۲��¼����ʱ����£�1-12��
	unsigned char EDay;		// �۲��¼����ʱ����գ�1-31��
	unsigned char EHour;		// �۲��¼����ʱ���ʱ��00-23��
	unsigned char EMinute;		// �۲��¼����ʱ��ķ֣�00-59��
	unsigned char ESecond;		// �۲��¼����ʱ����루00-59��
	unsigned char ETenth;		// �۲��¼����ʱ���1/100�루00-99��
	unsigned short ZBinByte;      	// ԭʼǿ�������пⳤ�ޱ仯��0,�б仯��ռ���ֽ���
    BINPARAM BinRange1[5];          // 5��8�ֽ�(ǿ�ȿⳤ�ޱ仯Ϊ���ֽ�)
	unsigned short VBinByte;        // ԭʼ�ٶ������пⳤ�ޱ仯��0,�б仯��ռ���ֽ���
	BINPARAM BinRange2[5];			// 5��8�ֽڣ��ٶȿⳤ�ޱ仯Ϊ���ֽڣ�(1416-1455)
	unsigned short WBinByte;        // ԭʼ�׿������пⳤ�ޱ仯��0,�б仯��ռ���ֽ���
	BINPARAM BinRange3[5];			// 5��8�ֽڣ��׿�ⳤ�ޱ仯Ϊ���ֽڣ�(1458-1497)

		//������Ϣ��������562���ֽڣ�
	char StationID[4] ;             // ̨վ����
	char Reserved[558] ;            // �����ֽ�560��

}RADDATAHEADER ;

typedef struct tagRadialHeader   
{
	//���ݼ�¼��Ľṹͷ��
	         short int Elev	;	// ���ǣ�������λ1/100��
	unsigned short int Azimuth;		// ��λ��������λ1/100��
	     unsigned char hour;		// ʱ
	     unsigned char min;		// ��
	     unsigned char sec;		// ��
	unsigned  long int millionsec;		// ���С����������λΪ΢�룩
}RADIALHEADER;

typedef struct tagRadialDatas   
{
	//���ݼ�¼��Ľṹͷ��
	// Reflectivity or Doppler data
	RADIALHEADER RadialHeader;
	unsigned char data[1024];
}RADIALDATAS ;

class CCProductPretreatment2 :	public GenerationData
{
public:
	CCProductPretreatment2();
	~CCProductPretreatment2();

	FILEVOLUMN * GetTArchiveCCFileVolumnP( );
	RADDATAHEADER * GetTArchiveCCRadDataHeaderP();
	RADIALHEADER * GetTArchiveCCRVWP(gInt16 LayIndex,gInt16 RadialIndex);
	unsigned char * GetTArchiveCCRP(gInt16 LayIndex,gInt16 RadialIndex);
	unsigned char * GetTArchiveCCVP(gInt16 LayIndex,gInt16 RadialIndex);
	unsigned char * GetTArchiveCCWP(gInt16 LayIndex,gInt16 RadialIndex);
	gInt16 GetLayerNubP();
private:
	inline FILEVOLUMN * GetTArchiveCCFileVolumn ( );
	inline RADDATAHEADER * GetTArchiveCCRadDataHeader();
	inline RADIALHEADER * GetTArchiveCCRVW(gInt16 LayIndex,gInt16 RadialIndex);
	inline unsigned char * GetTArchiveCCR(gInt16 LayIndex,gInt16 RadialIndex);
	inline unsigned char * GetTArchiveCCV(gInt16 LayIndex,gInt16 RadialIndex);
	inline unsigned char * GetTArchiveCCW(gInt16 LayIndex,gInt16 RadialIndex);
	inline gInt16 GetLayerNub();
};
#pragma pack(pop)
#endif
