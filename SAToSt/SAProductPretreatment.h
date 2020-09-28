//////////////////////////////////////////////////////////////////////////
// SAProductPretreatment.h
// created by zy 20080401
// SA�״��Ʒ���ݸ�ʽ
//////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef SAPRODUCTPRETREATMENT_H
#define SAPRODUCTPRETREATMENT_H

#pragma pack(push,1)//���������Ӧ��popһ��Ҫ���ϣ���֤����

#include "GData.h"
typedef struct TArchive//CINRAD-SA LEVEL II ���ݸ�ʽ
{
	gInt8   reserve1[14];               //1~14����
	gInt16  RadarData;                  //15~16 1-��ʾ�״�����
	gInt8   reserve2[12];               //17~28����
	gInt32  MessageGenerationTime;		//29~32���������ռ�ʱ��(����,��00:00��ʼ)
	gUint16  MessageGenerationDate;		//33~34�����գ�Julian����ʾ
	gUint16 UnambiguousRange;			//35~36��ģ�����루��ʾ����ֵ/10.=ǧ�ף�
	gUint16 AzimuthAngle;				//37~38��λ�ǣ����뷽ʽ��[��ֵ/8.]*[180./4096.]=�ȣ�
	gInt16  AzimuthNumber;              //39~40��ǰ�����ھ����������
	gInt16  AzimuthStatus;
	/*
	41~42��������״̬ 
	0�������ǵĵ�һ����������
	1���������м�ľ�������
	2�������ǵ����һ����������
	3����ɨ��ʼ�ĵ�һ����������
	4����ɨ���������һ����������
	*/
	gInt16  ElevationAngle;            //43~44���� �����뷽ʽ��[��ֵ/8.]*[180./4096.]=�ȣ�
	gInt16  ElevationNumber;           //45~46��ɨ�ڵ�������
	gInt16  FirstGateRangeOfRef;       //47~48���������ݵĵ�һ��������ʵ�ʾ���(��λ:��)
	gInt16  FirstGateRangeOfDoppler;   //49~50���������ݵĵ�һ��������ʵ�ʾ���(��λ:��)
	gInt16  ReflectivityGateSize;      //51~52���������ݵľ���ⳤ����λ���ף�
	gInt16  DopplerGateSize;           //53~54���������ݵľ���ⳤ����λ���ף�
	gInt16  ReflectivityGates;         //55~56�����ʵľ������
	gInt16  DopplerGates;              //57~58�����յľ������
	gInt16  SectorNumber;              //59~60������
	gInt32  CalibrationConstant;       //61~64ϵͳ��������
	gUint16 ReflectivityPointer;       //65~66����������ָ�루ƫ���״�������Ϣͷ���ֽ�������ʾ��һ�����������ݵ�λ��
	gUint16 VelocityPointer;           //67~68�ٶ�����ָ�루ƫ���״�������Ϣͷ���ֽ�������ʾ��һ���ٶ����ݵ�λ��
	gUint16 SpectrumWidthPointer;      //69~70�׿�����ָ�루ƫ���״�������Ϣͷ���ֽ�������ʾ��һ���׿����ݵ�λ��
	gInt16  VelocityResolution;        //71~72�������ٶȷֱ��� 2����ʾ0.5��/�� 4����ʾ1.0��/��
	gInt16  VCP;
	/*
	73~74��ɨ��VCP��ģʽ
	11����ˮģʽ��16������
	21����ˮģʽ��11������
	31�����ģʽ��8������
	32�����ģʽ��7������
	*/
	gInt8   reserve3[8];               //75~82����
	gUint16 RefPlaybackPointer;        //83~84���ڻطŵķ���������ָ�룬ͬ33
	gUint16 VelPlaybackPointer;        //85~86���ڻطŵ��ٶ�����ָ�룬ͬ34
	gUint16 SWPlaybackPointer;         //87~88���ڻطŵ��׿�����ָ�룬ͬ35
	gInt16  NyquistVelocity;           //89~90Nyquist�ٶȣ���ʾ����ֵ/100. = ��/�룩
	gInt8   reserve4[38];              //91~128����
	gUint8  RefData[460];
	/*
	129~588������
	���������0-460
	���뷽ʽ������ֵ-2��/2.-32 = DBZ
	����ֵΪ0ʱ����ʾ�޻ز����ݣ���������ȷ�ֵ��
	����ֵΪ1ʱ����ʾ����ģ��
	*/
	gUint8  VelData[920];
	/*
	589~1508�ٶ�
	���������0-920
	���뷽ʽ��
	�ֱ���Ϊ0.5��/��ʱ
	����ֵ-2��/2.-63.5 = ��/��
	�ֱ���Ϊ1.0��/��ʱ
	����ֵ-2��-127 = ��/��
	����ֵΪ0��1ʱ������ͬ��
	*/
	gUint8  SWData[920];
	/*
	1509~2428�׿�
	���������0-920
	���뷽ʽ��
	����ֵ-2��/2.-63.5 = ��/��
	����ֵΪ0��1ʱ������ͬ��
	*/
	gInt8   reserve5[4];               //2429-2432����
}TARCHIVE;

class SAProductPretreatment :	public GenerationData
{
	public:
		SAProductPretreatment();
		~SAProductPretreatment();

		TARCHIVE * GetTArchiveHeadP( gInt32 Index );
		gUint8 * GetTArchiveRP(gInt32 ReflectivityPointer);//����Pointer��Ϊƫ���ļ�ͷ���ֽ���
		gUint8 * GetTArchiveVP(gInt32 VelocityPointer);
		gUint8 * GetTArchiveWP(gInt32 SpectrumWidthPointer);
		
	private:
		inline TARCHIVE * GetTArchiveHead ( gInt32 Index );
		inline gUint8 * GetTArchiveR(gInt32 ReflectivityPointer);
		inline gUint8 * GetTArchiveV(gInt32 VelocityPointer);
		inline gUint8 * GetTArchiveW(gInt32 SpectrumWidthPointer);
};


#pragma pack(pop)
#endif