//////////////////////////////////////////////////////////////////////////
// SAProductPretreatment.h
// created by zy 20080401
// SA�״��Ʒ���ݸ�ʽ
//////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef SAPRODUCTWSR_H
#define SAPRODUCTWSR_H

#define PRDBYTES 2432
#define RS_TILT_START 0
#define RS_TILT_KEEP 1
#define RS_TILT_STOP 2
#define RS_VOL_START 3
#define RS_VOL_STOP  4

#pragma pack(push,1)//���������Ӧ��popһ��Ҫ���ϣ���֤����

#include "GData.h"
#include "StandardRadarDataX.h"

typedef struct DATAINFO_TYPE
{	
	gUint16 AzimuthAngle;                   //36~37��λ�ǣ����뷽ʽ��[��ֵ/8.]*[180./4096.]=�ȣ�
	gInt16  AzimuthStatus;                 //0~1
	gInt16  DopplerGates;                  //4~5  �����յľ������
	gInt16  ElevationAngle;                //42~43 (value>>3)*0.043945(degree)  ���� �����뷽ʽ��[��ֵ/8.]*[180./4096.]=�ȣ�
	gUint16 MessageGenerationDate;          //18~19���������ռ�ʱ��(����,��00:00��ʼ)?
	gUint32 MessageGenerationTime;          //20~23 //unsigned  long??
	gInt16  NyquistVelocity;                //88~89Nyquist�ٶȣ���ʾ����ֵ/100. = ��/�룩
	gUint16 RadialCollectionDate;           //32~33
	gUint32 RadialCollectionTime;           //28~31���������ռ�ʱ��(����,��00:00��ʼ)??
	gInt16  ReflectivityGates;             //2~3  �����ʵľ������
	gUint16 SpectrumWidthPointer;          //68~69 �׿�����ָ�루ƫ���״�������Ϣͷ���ֽ�������ʾ��һ���׿����ݵ�λ��
	gUint16 VelocityPointer;               //66~67 �ٶ�����ָ�루ƫ���״�������Ϣͷ���ֽ�������ʾ��һ���ٶ����ݵ�λ��
	gInt16  VelocityResolution;            //70~71 2=0.5m/s,4=1.0m/s �������ٶȷֱ��� 2����ʾ0.5��/�� 4����ʾ1.0��/��
	/*  11����ˮģʽ��16������
	21����ˮģʽ��11������
	31�����ģʽ��8������
	32�����ģʽ��7������
	*/
}DATAINFO_TYPE;

typedef struct TArchive2
{	
	//Channel Terminal Manager 12bytes
	gUint8  CTM_message[12];
	gUint16 MessageSize;                    //12~13 RadarData; 
	gInt8   ChannelID;                      //14
	gInt8   MessageType;                    //15   1-��ʾ�״����� ??
	gInt16  IDSequence;                     //16~17
	gUint16 MessageGenerationDate;          //18~19���������ռ�ʱ��(����,��00:00��ʼ)?
	gUint32 MessageGenerationTime;          //20~23 //unsigned  long??
	gInt16  MessageSegmentsNumber;          //24~25
	gInt16  MessageSegmentNumber;           //26~27
	//Digital Radar Data  Header 100bytes
	gUint32 RadialCollectionTime;           //28~31���������ռ�ʱ��(����,��00:00��ʼ)??
	gUint16 RadialCollectionDate;           //32~33
	gUint16 UnambiguousRange;               //34~35��ģ�����루��ʾ����ֵ/10.=ǧ�ף�
	gUint16 AzimuthAngle;                   //36~37��λ�ǣ����뷽ʽ��[��ֵ/8.]*[180./4096.]=�ȣ�
	gInt16  AzimuthNumber;                  //38~39��ǰ�����ھ����������
	gInt16  AzimuthStatus;                   //40~41
	/*40~41��������״̬ 
		0�������ǵĵ�һ����������
		1���������м�ľ�������
		2�������ǵ����һ����������
		3����ɨ��ʼ�ĵ�һ����������
		4����ɨ���������һ����������
		*/
	gInt16  ElevationAngle;                //42~43 (value>>3)*0.043945(degree)  ���� �����뷽ʽ��[��ֵ/8.]*[180./4096.]=�ȣ�
	gInt16  ElevationNumber;               //44~45  ��ɨ�ڵ�������
	gInt16  FirstGateRangeOfRef;           //46~47  ���������ݵĵ�һ��������ʵ�ʾ���(��λ:��)
	gInt16  FirstGateRangeOfDoppler;       //48~49  ���������ݵĵ�һ��������ʵ�ʾ���(��λ:��)
	gInt16  ReflectivityGateSize;          //50~51  ���������ݵľ���ⳤ����λ���ף�
	gInt16  DopplerGateSize;               //52~53  ���������ݵľ���ⳤ����λ���ף�
	gInt16  ReflectivityGates;             //54~55  �����ʵľ������
	gInt16  DopplerGates;                  //56~57  �����յľ������
	gInt16  SectorNumber;                  //58~59  ������
	gFloat  CalibrationConstant;           //60~63 ϵͳ��������
	gUint16 ReflectivityPointer;           //64~65 ����������ָ�루ƫ���״�������Ϣͷ���ֽ�������ʾ��һ�����������ݵ�λ��
	gUint16 VelocityPointer;               //66~67 �ٶ�����ָ�루ƫ���״�������Ϣͷ���ֽ�������ʾ��һ���ٶ����ݵ�λ��
	gUint16 SpectrumWidthPointer;          //68~69 �׿�����ָ�루ƫ���״�������Ϣͷ���ֽ�������ʾ��һ���׿����ݵ�λ��
	gInt16  VelocityResolution;            //70~71 2=0.5m/s,4=1.0m/s �������ٶȷֱ��� 2����ʾ0.5��/�� 4����ʾ1.0��/��
	gInt16  VCP;                           //72~73	��ɨ��VCP��ģʽ
											/*  11����ˮģʽ��16������
												21����ˮģʽ��11������
												31�����ģʽ��8������
												32�����ģʽ��7������
												*/
	gInt8   reserved[8];                    //74~81����
	gUint16 RefPlaybackPointer;             //82~83���ڻطŵķ���������ָ�룬ͬ33
	gUint16 VelPlaybackPointer;             //84~85���ڻطŵ��ٶ�����ָ�룬ͬ34
	gUint16 SWPlaybackPointer;              //86~87���ڻطŵ��׿�����ָ�룬ͬ35
	gInt16  NyquistVelocity;                //88~89Nyquist�ٶȣ���ʾ����ֵ/100. = ��/�룩
	gInt16  AAF;                            //90~91 Atmospheric Attenuation Factor(0.001 dB/Km)
	gInt16  TOVER;                          //92~93 
	gInt8   unused[34];                     //94~127
	gUint8  dat[2300];                      //���ݣ�?
	gUint32 Trailer;
}TARCHIVE2;

typedef struct tagStructureofTiltINFO
{
	gInt16 start;
	gInt16 end;
	gInt16 total;
	gInt16 bad;
}TILTINFO;



template <class Data_Type > inline void byteswap( Data_Type &data )
{
	unsigned int i;
	unsigned int num_bytes = sizeof( Data_Type );
	char *char_data = reinterpret_cast< char * >( &data );
	char *temp = new char[ num_bytes ];

	for( i = 0; i < num_bytes; i++ )
	{
		temp[ i ] = char_data[ num_bytes - i - 1 ];
	}

	for( i = 0; i < num_bytes; i++ )
	{
		char_data[ i ] = temp[ i ];
	}

	delete [] temp;
	temp = NULL;
}

class SAProductWSR :public GenerationData
{
public:
	SAProductWSR();
	~SAProductWSR();
//protected:

public:

	gInt32 GetPPiRef(int tilt,gUint8 **rppi,TILTINFO * pTiltInfo,DATAINFO_TYPE * pDataInfo);

	gInt32 GetDoppler(int tilt,/*GELEVATIONDATAHEAD *pElevHead,*/gUint8 **vppi,gUint8 **wppi,TILTINFO * pTiltInfo,DATAINFO_TYPE * pDataInfo);

	void V_Normalization(int ilayer,GELEVATIONDATAHEAD * pElevHead,gUint8 **vppi,gInt16 VelReso,gInt16 ***datav);

	void W_Normalization(int ilayer,GELEVATIONDATAHEAD * pElevHead,gUint8 **wppi,gInt16 ***dataw);

	void GetBaseData(/* GRADARDATAHEADER * pRadarHead,*/GELEVATIONDATAHEAD * pElevHead,gInt16 ***dataz,gInt16 ***datav,gInt16 ***dataw,TILTINFO * pTiltInfo,DATAINFO_TYPE * pDataInfo);

	gInt16 RadialStatusCheck(gInt32 iOffSize,gInt16 iSwapon,gInt32 iTotalRadialNums,gInt16 &ElevationNumber,gInt16 & iRays,gInt16 & iRefGates,gInt16 & iDopGates,TILTINFO * pTiltInfo,DATAINFO_TYPE * pDataInfo);

	void  Close();

	TARCHIVE2 * GetTArchiveHeadP( gInt32 Index );
	//gUint8 * GetTArchiveRP(gInt32 ReflectivityPointer);//����Pointer��Ϊƫ���ļ�ͷ���ֽ���
	//gUint8 * GetTArchiveVP(gInt32 VelocityPointer);
	//gUint8 * GetTArchiveWP(gInt32 SpectrumWidthPointer);

private:
	inline TARCHIVE2 * GetTArchiveHead ( gInt32 Index );
	//inline gUint8 * GetTArchiveR(gInt32 ReflectivityPointer);
	//inline gUint8 * GetTArchiveV(gInt32 VelocityPointer);
	//inline gUint8 * GetTArchiveW(gInt32 SpectrumWidthPointer);
};
#pragma pack(pop)
#endif