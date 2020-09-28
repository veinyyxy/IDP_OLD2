//////////////////////////////////////////////////////////////////////////
// SAProductPretreatment.h
// created by zy 20080401
// SA雷达产品数据格式
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

#pragma pack(push,1)//这句和下面对应的pop一定要加上，保证对齐

#include "GData.h"
#include "StandardRadarDataX.h"

typedef struct DATAINFO_TYPE
{	
	gUint16 AzimuthAngle;                   //36~37方位角（编码方式：[数值/8.]*[180./4096.]=度）
	gInt16  AzimuthStatus;                 //0~1
	gInt16  DopplerGates;                  //4~5  多普勒的距离库数
	gInt16  ElevationAngle;                //42~43 (value>>3)*0.043945(degree)  仰角 （编码方式：[数值/8.]*[180./4096.]=度）
	gUint16 MessageGenerationDate;          //18~19径向数据收集时间(毫秒,自00:00开始)?
	gUint32 MessageGenerationTime;          //20~23 //unsigned  long??
	gInt16  NyquistVelocity;                //88~89Nyquist速度（表示：数值/100. = 米/秒）
	gUint16 RadialCollectionDate;           //32~33
	gUint32 RadialCollectionTime;           //28~31径向数据收集时间(毫秒,自00:00开始)??
	gInt16  ReflectivityGates;             //2~3  反射率的距离库数
	gUint16 SpectrumWidthPointer;          //68~69 谱宽数据指针（偏离雷达数据信息头的字节数）表示第一个谱宽数据的位置
	gUint16 VelocityPointer;               //66~67 速度数据指针（偏离雷达数据信息头的字节数）表示第一个速度数据的位置
	gInt16  VelocityResolution;            //70~71 2=0.5m/s,4=1.0m/s 多普勒速度分辨率 2：表示0.5米/秒 4：表示1.0米/秒
	/*  11：降水模式，16层仰角
	21：降水模式，11层仰角
	31：晴空模式，8层仰角
	32：晴空模式，7层仰角
	*/
}DATAINFO_TYPE;

typedef struct TArchive2
{	
	//Channel Terminal Manager 12bytes
	gUint8  CTM_message[12];
	gUint16 MessageSize;                    //12~13 RadarData; 
	gInt8   ChannelID;                      //14
	gInt8   MessageType;                    //15   1-表示雷达数据 ??
	gInt16  IDSequence;                     //16~17
	gUint16 MessageGenerationDate;          //18~19径向数据收集时间(毫秒,自00:00开始)?
	gUint32 MessageGenerationTime;          //20~23 //unsigned  long??
	gInt16  MessageSegmentsNumber;          //24~25
	gInt16  MessageSegmentNumber;           //26~27
	//Digital Radar Data  Header 100bytes
	gUint32 RadialCollectionTime;           //28~31径向数据收集时间(毫秒,自00:00开始)??
	gUint16 RadialCollectionDate;           //32~33
	gUint16 UnambiguousRange;               //34~35不模糊距离（表示：数值/10.=千米）
	gUint16 AzimuthAngle;                   //36~37方位角（编码方式：[数值/8.]*[180./4096.]=度）
	gInt16  AzimuthNumber;                  //38~39当前仰角内径向数据序号
	gInt16  AzimuthStatus;                   //40~41
	/*40~41径向数据状态 
		0：该仰角的第一条径向数据
		1：该仰角中间的径向数据
		2：该仰角的最后一条径向数据
		3：体扫开始的第一条径向数据
		4：体扫结束的最后一条径向数据
		*/
	gInt16  ElevationAngle;                //42~43 (value>>3)*0.043945(degree)  仰角 （编码方式：[数值/8.]*[180./4096.]=度）
	gInt16  ElevationNumber;               //44~45  体扫内的仰角数
	gInt16  FirstGateRangeOfRef;           //46~47  反射率数据的第一个距离库的实际距离(单位:米)
	gInt16  FirstGateRangeOfDoppler;       //48~49  多普勒数据的第一个距离库的实际距离(单位:米)
	gInt16  ReflectivityGateSize;          //50~51  反射率数据的距离库长（单位：米）
	gInt16  DopplerGateSize;               //52~53  多普勒数据的距离库长（单位：米）
	gInt16  ReflectivityGates;             //54~55  反射率的距离库数
	gInt16  DopplerGates;                  //56~57  多普勒的距离库数
	gInt16  SectorNumber;                  //58~59  扇区号
	gFloat  CalibrationConstant;           //60~63 系统订正常数
	gUint16 ReflectivityPointer;           //64~65 反射率数据指针（偏离雷达数据信息头的字节数）表示第一个反射率数据的位置
	gUint16 VelocityPointer;               //66~67 速度数据指针（偏离雷达数据信息头的字节数）表示第一个速度数据的位置
	gUint16 SpectrumWidthPointer;          //68~69 谱宽数据指针（偏离雷达数据信息头的字节数）表示第一个谱宽数据的位置
	gInt16  VelocityResolution;            //70~71 2=0.5m/s,4=1.0m/s 多普勒速度分辨率 2：表示0.5米/秒 4：表示1.0米/秒
	gInt16  VCP;                           //72~73	体扫（VCP）模式
											/*  11：降水模式，16层仰角
												21：降水模式，11层仰角
												31：晴空模式，8层仰角
												32：晴空模式，7层仰角
												*/
	gInt8   reserved[8];                    //74~81保留
	gUint16 RefPlaybackPointer;             //82~83用于回放的反射率数据指针，同33
	gUint16 VelPlaybackPointer;             //84~85用于回放的速度数据指针，同34
	gUint16 SWPlaybackPointer;              //86~87用于回放的谱宽数据指针，同35
	gInt16  NyquistVelocity;                //88~89Nyquist速度（表示：数值/100. = 米/秒）
	gInt16  AAF;                            //90~91 Atmospheric Attenuation Factor(0.001 dB/Km)
	gInt16  TOVER;                          //92~93 
	gInt8   unused[34];                     //94~127
	gUint8  dat[2300];                      //数据？?
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
	//gUint8 * GetTArchiveRP(gInt32 ReflectivityPointer);//以下Pointer均为偏移文件头的字节数
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