//////////////////////////////////////////////////////////////////////////
// StandardRadarData.h
// created by ply 20080325
// 本系统统一的雷达基数据格式
//////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef STANDARDRADARDATA_H
#define STANDARDRADARDATA_H

#pragma pack(push,1)//这句和下面对应的pop一定要加上，保证对齐

#include "GData.h"
typedef struct tagGRadarDataHeader			//体扫文件头数据格式
{
//雷达站信息描述
	gInt8	SiteCode[8];
	/*
		5位字符编码，前2位字符表示区号，采用字母、数字组合表示；后三位表示站号，采用数字表示
		前4字节表示区号，高位补零，保留
		后4字节表示站号，高位补零，保留
	*/
	gInt8	RadarType[4];
	/*
		4字节8位数字编码
		第一个字节：表示雷达类型，01：多普勒天气雷达，其它雷达类型（相控阵、风廓线等）编码待定
		第二个字节：高4位表示雷达部署方式（1：表示固定式雷达、2：表示移动式雷达），低4位表示子类型（固定式雷达分1：地基雷达，机动式雷达分1：车载、2：船载、3：机载）
		第三个字节：表示雷达波段，01：K波段，02：S波段，03：C波段，04：X波段
		第四个字节：表示雷达型号，高4位表示雷达厂商编号，低4位表示雷达型号编码
	*/
	gInt32  RadarLon;//Longitude;						// 雷达站经度，单位：.001度
	gInt32  RadarLat;//Latitude;						//雷达站纬度，单位：.001度
	gInt32	RadarHeight;//Altitude;						// 海拔高度，单位：.01米

//体扫数据描述
	gUint16	VCPDate;//SingleScanDate;					//儒略日（Julian）表示，自1970年1月1日开始
	gInt32  VCPTime;//SingleScanTime;					//单位：毫秒，自00:00开始
	gInt8   OperationMode[2];
	/*
		二字节二位数字编码
		01：降水
		02：晴空
	*/
	gInt8   ScanMode[2];
	/*
		2字节3位二进制数编码
		编码规则：最高位表示仰角数：0-单仰角，1-多仰角；第二位表示方位数：0-单方位，1-多方位；最低位表示水平扫描或垂直扫描：0-水平扫描，1-垂直扫描
		扫描方式：
		00X：定位
		010：PPI
		101：RHI
		110：VCP
		111：未定义
	*/
	gInt8	VCPMode[4];
	/*
		4字节8位数字编码
		第一个字节：适用天气编码，高4位代表十位数、低4位代表个位数，取值范围1~99
		第二个字节：体扫内仰角数编码，高4位代表十位数、低4位代表个位数，取值范围1~99
		第三个字节：体扫内扫描次数编码，高4位代表十位数、低4位代表个位数，取值范围体扫内仰角数~99
		第四个字节：体扫周期，高4位代表十位数、低4位代表个位数，取值范围1~60，单位：分钟
	*/
	gInt16  SectorCode;						//扇区号：雷达做水平扇区扫描时使用扇区号表示该扫描的基数据的方位角范围  取值范围：1<SectorCode≤360
	gInt16	SizeOfSector;					//扇区大小：0度~360度  单位：度，取值范围：0度<SizeOfSector≤360度
	gInt16  ReflectivityGateSize;//GateRangeOfRef;					//反射率数据的距离库长（单位：米）单位：米
	gInt16  DopplerGateSize;//GateRangeOfDoppler;				//多普勒数据（平均径向速度和谱宽）的距离库长（单位：米）单位：米
	gInt32  ConstantOfSystemCorrect;		//系统订正常数：与天气过程相关
	gInt16  ElevationCount;//NumberOfElevation;				//体扫内的仰角数
	//gInt32  IndexOfElevationData[20];		//仰角数据指针[仰角数]，相对于基数据起始地址的偏移量 第0个不使用
	//下面存每个仰角的数据
}GRADARDATAHEADER;
	
//gInt32  IndexOfElevationData[仰角数];		//仰角数据指针[仰角数]，相对于基数据起始地址的偏移量 

typedef struct tagGElevationDataHeader			//仰角数据描述
{
	gInt16  SNOfElevationData;				//从1开始编号
	gInt16  UnambiguousRange;				//最大不模糊距离（单位：0.1千米，编码方式：数值/10.=千米）
	gInt16  NyquistVelocity;				//最大不模糊（Nyquist）速度(单位：0.01米/秒，编码方式：数值/100.=米/秒)
	gInt16	ReflectivityResolution;			//反射率分辨率  单位：0.01dBZ，编码方式：数值/100.=dBZ
	gInt16  VelocityResolution;				//多普勒速度分辨率  单位：0.01米/秒，编码方式：数值/100.=米/秒
	gInt16  SpectrumWidthOfVelocityResolution;//谱宽分辨率  单位：0.01米/秒，编码方式：数值/100.=米/秒
	gInt16  FirstGateRangeOfRef;//DistanceOfFirstReflectivityGate;//反射率数据的第一个距离库的距雷达的实际距离（单位：米），即位置
	gInt16  ReflectivityGates;//NumberOfReflectivityGates;		//反射率数据的距离库数
	gInt16  FirstGateRangeOfDoppler;//DistanceOfFirstDopplerGate;		//多普勒数据（平均径向速度和谱宽）的第一个距离库的距雷达的实际距离（单位：米），即位置
	gInt16  DopplerGates;//NumberOfDopplerGates;			//多普勒数据（平均径向速度和谱宽）的距离库数
	gInt16  Elevation;						//仰角  单位：0.01度，编码方式：数值/100.=度
	gInt16  AzimuthNumber;//NumberOfAzimuth;				//仰角内的方位数
	//gInt32  IndexOfAzimuthData[600];		//径向数据指针[方位数][R、V、W]，相对于仰角数据起始地址的偏移量 第0个不使用
}GELEVATIONDATAHEAD;

//gInt32  IndexOfAzimuthData[方位数];		//径向数据指针[方位数][R、V、W]，相对于仰角数据起始地址的偏移量 

//径向数据描述
typedef struct tagRVWDataHead			//本结构体经过编码转换，结构体中的数据是DBZ值
{
	gInt16	SNOfRadialData ;				//当前仰角内径向数据的序号  从1开始编号
	gUint16 Date;//DateOfRadialData;				//径向数据日期（儒略日（Julian）表示，自1970年1月1日开始）
	gInt32	Time;//TimeOfRadialData;				//径向数据收集时间（毫秒，自00:00开始）
	gUint16	Azimuth;						//方位角  单位：0.01度，编码方式：数值/100.=度
	gInt16	Elevation;//ActualElevation;				//实际仰角  单位：0.01度，编码方式：数值/100.=度
	/*
			//这些数据是紧跟在上面之后的
		gInt16 R[maxRefgatenum];
			//单位：0.01dBZ，编码方式：数值/100.=dBZ
			//取值范围：-32768~32767
			//当数值为-32768时，表示探测范围以外
			//当数值为-32767时，表示无回波数据（低于信噪比阈值）
			//当数值为-32766时，表示距离模糊
		gInt16 V[maxVelgatenum];
			//单位：0.01米/秒，编码方式：数值/100.=米/秒
			//取值范围：-32768~32767
			//当数值为-32768，-32767或-32766时，意义同上
		gInt16 W[maxVelgatenum];
			//单位：0.01米/秒，编码方式：数值/100.=米/秒
			//取值范围：-32768~32767
			//当数值为-32768，-32767或-32766时，意义同上
	*/
}RVWDATAHEAD;

//算法中用的雨量计数据结构体
#ifndef _GAUGESITE_DATA_STRUCT_
#define _GAUGESITE_DATA_STRUCT_
typedef struct tagGaugeInformation
{
	char   name[20];
	float  flon;  //站点位置
	float  flat;  //站点位置
	float  OHR;  //一小时站点雨量-1.0 = NODATA
}GAUGEINFO;
#endif 

//从数据库里查找到的雨量计结构体limei
//typedef struct Result  
//{
//	string StationNum; //站点号
//	string StationName; //站点名
//	string PreciptnAmnt; //雨量值
//	string Longitude; //经度
//	string Latitude;//纬度
//	string CodePro; //行政区代号
//};

//传入参数仰角号和径向号全部从0开始计数
class DLLEXP_GSYS StandardRadarData : public GenerationData
{
public:
		StandardRadarData();
		~StandardRadarData();

	//read functions
		GRADARDATAHEADER * GetHeadP();
		gInt16    GetVCPDate ();  //获得体扫开始日期 add by dqc 20080422
		gInt32    GetVCPTime ();  //获得体扫开始时间 add by dqc 20080422
		GHRESULT  GetSiteCode(char * pSiteCode);  //获得雷达站号 add by dqc 20080422
//		gInt16    GetRadarType();                 //获得雷达类型 add by dqc 20080422
	//read/write functions
		GELEVATIONDATAHEAD * GetElevationHeadP ( gInt32 Index );
		RVWDATAHEAD * GetRadarDataP ( gInt32 ElevationIndex, gInt32 AzimuthIndex );
		gInt16 * GetRadarDataR ( gInt32 ElevationIndex, gInt32 AzimuthIndex );
		gInt16 * GetRadarDataV ( gInt32 ElevationIndex, gInt32 AzimuthIndex );
		gInt16 * GetRadarDataW ( gInt32 ElevationIndex, gInt32 AzimuthIndex );
	//write functions
	//必须先WriteHead再按顺序WriteElevationHead再写径向数据  每个仰角WriteElevationHead只能调用一次
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