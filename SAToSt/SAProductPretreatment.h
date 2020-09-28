//////////////////////////////////////////////////////////////////////////
// SAProductPretreatment.h
// created by zy 20080401
// SA雷达产品数据格式
//////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef SAPRODUCTPRETREATMENT_H
#define SAPRODUCTPRETREATMENT_H

#pragma pack(push,1)//这句和下面对应的pop一定要加上，保证对齐

#include "GData.h"
typedef struct TArchive//CINRAD-SA LEVEL II 数据格式
{
	gInt8   reserve1[14];               //1~14保留
	gInt16  RadarData;                  //15~16 1-表示雷达数据
	gInt8   reserve2[12];               //17~28保留
	gInt32  MessageGenerationTime;		//29~32径向数据收集时间(毫秒,自00:00开始)
	gUint16  MessageGenerationDate;		//33~34儒略日（Julian）表示
	gUint16 UnambiguousRange;			//35~36不模糊距离（表示：数值/10.=千米）
	gUint16 AzimuthAngle;				//37~38方位角（编码方式：[数值/8.]*[180./4096.]=度）
	gInt16  AzimuthNumber;              //39~40当前仰角内径向数据序号
	gInt16  AzimuthStatus;
	/*
	41~42径向数据状态 
	0：该仰角的第一条径向数据
	1：该仰角中间的径向数据
	2：该仰角的最后一条径向数据
	3：体扫开始的第一条径向数据
	4：体扫结束的最后一条径向数据
	*/
	gInt16  ElevationAngle;            //43~44仰角 （编码方式：[数值/8.]*[180./4096.]=度）
	gInt16  ElevationNumber;           //45~46体扫内的仰角数
	gInt16  FirstGateRangeOfRef;       //47~48反射率数据的第一个距离库的实际距离(单位:米)
	gInt16  FirstGateRangeOfDoppler;   //49~50多普勒数据的第一个距离库的实际距离(单位:米)
	gInt16  ReflectivityGateSize;      //51~52反射率数据的距离库长（单位：米）
	gInt16  DopplerGateSize;           //53~54多普勒数据的距离库长（单位：米）
	gInt16  ReflectivityGates;         //55~56反射率的距离库数
	gInt16  DopplerGates;              //57~58多普勒的距离库数
	gInt16  SectorNumber;              //59~60扇区号
	gInt32  CalibrationConstant;       //61~64系统订正常数
	gUint16 ReflectivityPointer;       //65~66反射率数据指针（偏离雷达数据信息头的字节数）表示第一个反射率数据的位置
	gUint16 VelocityPointer;           //67~68速度数据指针（偏离雷达数据信息头的字节数）表示第一个速度数据的位置
	gUint16 SpectrumWidthPointer;      //69~70谱宽数据指针（偏离雷达数据信息头的字节数）表示第一个谱宽数据的位置
	gInt16  VelocityResolution;        //71~72多普勒速度分辨率 2：表示0.5米/秒 4：表示1.0米/秒
	gInt16  VCP;
	/*
	73~74体扫（VCP）模式
	11：降水模式，16层仰角
	21：降水模式，11层仰角
	31：晴空模式，8层仰角
	32：晴空模式，7层仰角
	*/
	gInt8   reserve3[8];               //75~82保留
	gUint16 RefPlaybackPointer;        //83~84用于回放的反射率数据指针，同33
	gUint16 VelPlaybackPointer;        //85~86用于回放的速度数据指针，同34
	gUint16 SWPlaybackPointer;         //87~88用于回放的谱宽数据指针，同35
	gInt16  NyquistVelocity;           //89~90Nyquist速度（表示：数值/100. = 米/秒）
	gInt8   reserve4[38];              //91~128保留
	gUint8  RefData[460];
	/*
	129~588反射率
	距离库数：0-460
	编码方式：（数值-2）/2.-32 = DBZ
	当数值为0时，表示无回波数据（低于信噪比阀值）
	当数值为1时，表示距离模糊
	*/
	gUint8  VelData[920];
	/*
	589~1508速度
	距离库数：0-920
	编码方式：
	分辨率为0.5米/秒时
	（数值-2）/2.-63.5 = 米/秒
	分辨率为1.0米/秒时
	（数值-2）-127 = 米/秒
	当数值为0或1时，意义同上
	*/
	gUint8  SWData[920];
	/*
	1509~2428谱宽
	距离库数：0-920
	编码方式：
	（数值-2）/2.-63.5 = 米/秒
	当数值为0或1时，意义同上
	*/
	gInt8   reserve5[4];               //2429-2432保留
}TARCHIVE;

class SAProductPretreatment :	public GenerationData
{
	public:
		SAProductPretreatment();
		~SAProductPretreatment();

		TARCHIVE * GetTArchiveHeadP( gInt32 Index );
		gUint8 * GetTArchiveRP(gInt32 ReflectivityPointer);//以下Pointer均为偏移文件头的字节数
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