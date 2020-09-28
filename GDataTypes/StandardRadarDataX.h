//////////////////////////////////////////////////////////////////////////
// StandardRadarData.h
// created by ply 20080325
// ��ϵͳͳһ���״�����ݸ�ʽ
//////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef STANDARDRADARDATA_H
#define STANDARDRADARDATA_H

#pragma pack(push,1)//���������Ӧ��popһ��Ҫ���ϣ���֤����

#include "GData.h"
typedef struct tagGRadarDataHeader			//��ɨ�ļ�ͷ���ݸ�ʽ
{
//�״�վ��Ϣ����
	gInt8	SiteCode[8];
	/*
		5λ�ַ����룬ǰ2λ�ַ���ʾ���ţ�������ĸ��������ϱ�ʾ������λ��ʾվ�ţ��������ֱ�ʾ
		ǰ4�ֽڱ�ʾ���ţ���λ���㣬����
		��4�ֽڱ�ʾվ�ţ���λ���㣬����
	*/
	gInt8	RadarType[4];
	/*
		4�ֽ�8λ���ֱ���
		��һ���ֽڣ���ʾ�״����ͣ�01�������������״�����״����ͣ�����󡢷����ߵȣ��������
		�ڶ����ֽڣ���4λ��ʾ�״ﲿ��ʽ��1����ʾ�̶�ʽ�״2����ʾ�ƶ�ʽ�״����4λ��ʾ�����ͣ��̶�ʽ�״��1���ػ��״����ʽ�״��1�����ء�2�����ء�3�����أ�
		�������ֽڣ���ʾ�״ﲨ�Σ�01��K���Σ�02��S���Σ�03��C���Σ�04��X����
		���ĸ��ֽڣ���ʾ�״��ͺţ���4λ��ʾ�״ﳧ�̱�ţ���4λ��ʾ�״��ͺű���
	*/
	gInt32  RadarLon;//Longitude;						// �״�վ���ȣ���λ��.001��
	gInt32  RadarLat;//Latitude;						//�״�վγ�ȣ���λ��.001��
	gInt32	RadarHeight;//Altitude;						// ���θ߶ȣ���λ��.01��

//��ɨ��������
	gUint16	VCPDate;//SingleScanDate;					//�����գ�Julian����ʾ����1970��1��1�տ�ʼ
	gInt32  VCPTime;//SingleScanTime;					//��λ�����룬��00:00��ʼ
	gInt8   OperationMode[2];
	/*
		���ֽڶ�λ���ֱ���
		01����ˮ
		02�����
	*/
	gInt8   ScanMode[2];
	/*
		2�ֽ�3λ������������
		����������λ��ʾ��������0-�����ǣ�1-�����ǣ��ڶ�λ��ʾ��λ����0-����λ��1-�෽λ�����λ��ʾˮƽɨ���ֱɨ�裺0-ˮƽɨ�裬1-��ֱɨ��
		ɨ�跽ʽ��
		00X����λ
		010��PPI
		101��RHI
		110��VCP
		111��δ����
	*/
	gInt8	VCPMode[4];
	/*
		4�ֽ�8λ���ֱ���
		��һ���ֽڣ������������룬��4λ����ʮλ������4λ�����λ����ȡֵ��Χ1~99
		�ڶ����ֽڣ���ɨ�����������룬��4λ����ʮλ������4λ�����λ����ȡֵ��Χ1~99
		�������ֽڣ���ɨ��ɨ��������룬��4λ����ʮλ������4λ�����λ����ȡֵ��Χ��ɨ��������~99
		���ĸ��ֽڣ���ɨ���ڣ���4λ����ʮλ������4λ�����λ����ȡֵ��Χ1~60����λ������
	*/
	gInt16  SectorCode;						//�����ţ��״���ˮƽ����ɨ��ʱʹ�������ű�ʾ��ɨ��Ļ����ݵķ�λ�Ƿ�Χ  ȡֵ��Χ��1<SectorCode��360
	gInt16	SizeOfSector;					//������С��0��~360��  ��λ���ȣ�ȡֵ��Χ��0��<SizeOfSector��360��
	gInt16  ReflectivityGateSize;//GateRangeOfRef;					//���������ݵľ���ⳤ����λ���ף���λ����
	gInt16  DopplerGateSize;//GateRangeOfDoppler;				//���������ݣ�ƽ�������ٶȺ��׿��ľ���ⳤ����λ���ף���λ����
	gInt32  ConstantOfSystemCorrect;		//ϵͳ�����������������������
	gInt16  ElevationCount;//NumberOfElevation;				//��ɨ�ڵ�������
	//gInt32  IndexOfElevationData[20];		//��������ָ��[������]������ڻ�������ʼ��ַ��ƫ���� ��0����ʹ��
	//�����ÿ�����ǵ�����
}GRADARDATAHEADER;
	
//gInt32  IndexOfElevationData[������];		//��������ָ��[������]������ڻ�������ʼ��ַ��ƫ���� 

typedef struct tagGElevationDataHeader			//������������
{
	gInt16  SNOfElevationData;				//��1��ʼ���
	gInt16  UnambiguousRange;				//���ģ�����루��λ��0.1ǧ�ף����뷽ʽ����ֵ/10.=ǧ�ף�
	gInt16  NyquistVelocity;				//���ģ����Nyquist���ٶ�(��λ��0.01��/�룬���뷽ʽ����ֵ/100.=��/��)
	gInt16	ReflectivityResolution;			//�����ʷֱ���  ��λ��0.01dBZ�����뷽ʽ����ֵ/100.=dBZ
	gInt16  VelocityResolution;				//�������ٶȷֱ���  ��λ��0.01��/�룬���뷽ʽ����ֵ/100.=��/��
	gInt16  SpectrumWidthOfVelocityResolution;//�׿�ֱ���  ��λ��0.01��/�룬���뷽ʽ����ֵ/100.=��/��
	gInt16  FirstGateRangeOfRef;//DistanceOfFirstReflectivityGate;//���������ݵĵ�һ�������ľ��״��ʵ�ʾ��루��λ���ף�����λ��
	gInt16  ReflectivityGates;//NumberOfReflectivityGates;		//���������ݵľ������
	gInt16  FirstGateRangeOfDoppler;//DistanceOfFirstDopplerGate;		//���������ݣ�ƽ�������ٶȺ��׿��ĵ�һ�������ľ��״��ʵ�ʾ��루��λ���ף�����λ��
	gInt16  DopplerGates;//NumberOfDopplerGates;			//���������ݣ�ƽ�������ٶȺ��׿��ľ������
	gInt16  Elevation;						//����  ��λ��0.01�ȣ����뷽ʽ����ֵ/100.=��
	gInt16  AzimuthNumber;//NumberOfAzimuth;				//�����ڵķ�λ��
	//gInt32  IndexOfAzimuthData[600];		//��������ָ��[��λ��][R��V��W]�����������������ʼ��ַ��ƫ���� ��0����ʹ��
}GELEVATIONDATAHEAD;

//gInt32  IndexOfAzimuthData[��λ��];		//��������ָ��[��λ��][R��V��W]�����������������ʼ��ַ��ƫ���� 

//������������
typedef struct tagRVWDataHead			//���ṹ�徭������ת�����ṹ���е�������DBZֵ
{
	gInt16	SNOfRadialData ;				//��ǰ�����ھ������ݵ����  ��1��ʼ���
	gUint16 Date;//DateOfRadialData;				//�����������ڣ������գ�Julian����ʾ����1970��1��1�տ�ʼ��
	gInt32	Time;//TimeOfRadialData;				//���������ռ�ʱ�䣨���룬��00:00��ʼ��
	gUint16	Azimuth;						//��λ��  ��λ��0.01�ȣ����뷽ʽ����ֵ/100.=��
	gInt16	Elevation;//ActualElevation;				//ʵ������  ��λ��0.01�ȣ����뷽ʽ����ֵ/100.=��
	/*
			//��Щ�����ǽ���������֮���
		gInt16 R[maxRefgatenum];
			//��λ��0.01dBZ�����뷽ʽ����ֵ/100.=dBZ
			//ȡֵ��Χ��-32768~32767
			//����ֵΪ-32768ʱ����ʾ̽�ⷶΧ����
			//����ֵΪ-32767ʱ����ʾ�޻ز����ݣ������������ֵ��
			//����ֵΪ-32766ʱ����ʾ����ģ��
		gInt16 V[maxVelgatenum];
			//��λ��0.01��/�룬���뷽ʽ����ֵ/100.=��/��
			//ȡֵ��Χ��-32768~32767
			//����ֵΪ-32768��-32767��-32766ʱ������ͬ��
		gInt16 W[maxVelgatenum];
			//��λ��0.01��/�룬���뷽ʽ����ֵ/100.=��/��
			//ȡֵ��Χ��-32768~32767
			//����ֵΪ-32768��-32767��-32766ʱ������ͬ��
	*/
}RVWDATAHEAD;

//�㷨���õ����������ݽṹ��
#ifndef _GAUGESITE_DATA_STRUCT_
#define _GAUGESITE_DATA_STRUCT_
typedef struct tagGaugeInformation
{
	char   name[20];
	float  flon;  //վ��λ��
	float  flat;  //վ��λ��
	float  OHR;  //һСʱվ������-1.0 = NODATA
}GAUGEINFO;
#endif 

//�����ݿ�����ҵ��������ƽṹ��limei
//typedef struct Result  
//{
//	string StationNum; //վ���
//	string StationName; //վ����
//	string PreciptnAmnt; //����ֵ
//	string Longitude; //����
//	string Latitude;//γ��
//	string CodePro; //����������
//};

//����������Ǻź;����ȫ����0��ʼ����
class DLLEXP_GSYS StandardRadarData : public GenerationData
{
public:
		StandardRadarData();
		~StandardRadarData();

	//read functions
		GRADARDATAHEADER * GetHeadP();
		gInt16    GetVCPDate ();  //�����ɨ��ʼ���� add by dqc 20080422
		gInt32    GetVCPTime ();  //�����ɨ��ʼʱ�� add by dqc 20080422
		GHRESULT  GetSiteCode(char * pSiteCode);  //����״�վ�� add by dqc 20080422
//		gInt16    GetRadarType();                 //����״����� add by dqc 20080422
	//read/write functions
		GELEVATIONDATAHEAD * GetElevationHeadP ( gInt32 Index );
		RVWDATAHEAD * GetRadarDataP ( gInt32 ElevationIndex, gInt32 AzimuthIndex );
		gInt16 * GetRadarDataR ( gInt32 ElevationIndex, gInt32 AzimuthIndex );
		gInt16 * GetRadarDataV ( gInt32 ElevationIndex, gInt32 AzimuthIndex );
		gInt16 * GetRadarDataW ( gInt32 ElevationIndex, gInt32 AzimuthIndex );
	//write functions
	//������WriteHead�ٰ�˳��WriteElevationHead��д��������  ÿ������WriteElevationHeadֻ�ܵ���һ��
		GHRESULT WriteHeadAndElevationHead( GRADARDATAHEADER * HeadToWrite , GELEVATIONDATAHEAD * ElevationHeadToWrite,gInt32 evlNum);
		GHRESULT WriteHead ( GRADARDATAHEADER * HeadToWrite );
		GHRESULT WriteElevationHead (gInt32 Index , GELEVATIONDATAHEAD * ElevationHeadToWrite );
		GHRESULT WriteRVWData ( gInt32 ElevationIndex, gInt32 AzimuthIndex , void * DataToWrite );
		GHRESULT WriteRVWHead ( gInt32 ElevationIndex, gInt32 AzimuthIndex , RVWDATAHEAD * RvwHeadToWrite );
		GHRESULT WritRadarDataR ( gInt32 ElevationIndex, gInt32 AzimuthIndex ,gInt16 * RDataToWrite);
		GHRESULT WritRadarDataV ( gInt32 ElevationIndex, gInt32 AzimuthIndex ,gInt16 * VDataToWrite );
		GHRESULT WritRadarDataW ( gInt32 ElevationIndex, gInt32 AzimuthIndex ,gInt16 * WDataToWrite );
private:
	//read functions
		inline GRADARDATAHEADER * GetHead();
	//read/write functions
		inline GELEVATIONDATAHEAD * GetElevationHead ( gInt32 Index );
		inline RVWDATAHEAD * GetRadarData ( gInt32 ElevationIndex, gInt32 AzimuthIndex );
		inline gInt32 * GetIndexOfElevationData();
		inline gInt32 * GetIndexOfAzimuthData(gInt32 ElevationIndex);
		inline gInt32 GetRVWSize(gInt32 ElevationIndex);
		inline gInt32 GetRoffset(gInt32 ElevationIndex);
		inline gInt32 GetVoffset(gInt32 ElevationIndex);
		inline gInt32 GetWoffset(gInt32 ElevationIndex);
	//write functions
		//inline GHRESULT ResizeFromHead();//remove

};



#pragma pack(pop)
#endif