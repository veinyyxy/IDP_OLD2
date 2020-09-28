
//////////////////////////////////////////////////////////////////////////
// CCProductPretreatment.h
// created by sunqc 20090327
// CC雷达产品数据格式,
//注意与SA/SB/CB雷达基数据保存格式（每条径向的格式）不同，这里的格式是整个体扫格式（整个体扫就一个头文件）。
//////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef CCPRODUCTPRETREATMENT_H
#define CCPRODUCTPRETREATMENT_H
#include "../GDataTypes/GData.h"
#include "../GDataTypes/GDef.h"


#pragma pack(push,1)//这句和下面对应的pop一定要加上，保证对齐
//tagVPPISCANPARAMETER仰角层的结构
typedef struct 
{
	gUint16 usMaxV;			//最大可测速度(厘米/秒)
	gUint16 usMaxL;			//最大可测距离(10米)
	gUint16 usBindWidth;		//库长(米)
	gUint16 usBinNumber;		//每径向库数
	gUint16 usRecordNumber;	        //本圈径向数
	gUint16 usArotate;		//本圈转速(0.01度/秒)
	gUint16 usPrf1;			//本圈第一次重复频率(0.1Hz)
	gUint16 usPrf2;			//本圈第二次重复频率(0.1Hz)
	gUint16 usSpulseW;		//本圈脉宽(微秒)
	gInt16  usAngle;			//仰角(0.01度)
	gUint8  cSweepStatus;		//1=单要素	2=三要素(单重频)	3=三要素(双重频)
	gUint8 cAmbiguousp;		//0=无软件退模糊	1=软件退模糊
}VPPISCANPARAMETER;
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

typedef struct 
{
	gChar cFileType[16];    //3830数据标识("CINRADC")
	//973试验标识("973")
	gChar cCountry[30];     //国家名
	gChar cProvince[20];    //省名
	gChar cStation[40];     //站名
	gChar cStationNumber[10];//区站号
	gChar cRadarType[20];   //雷达型号
	gChar cLongitude[16];   //天线所在经度
	gChar cLatitude[16];    //天线所在纬度
	gInt32 lLongitudeValue;  //具体经度  uint 1/1000 s
	gInt32 lLatitudeValue;   //具体纬度 uint 1/1000 s
	gInt32 lHeight;	       //天线海拔高度  uint 1/1000 m
	gInt16 sMaxAngle;       //地物阻挡最大仰角
	gInt16 sOptAngle;       //最佳观测仰角
	gUint8 ucSYear1;	//观测开始时间的年千百位(19-20)
	gUint8 ucSYear2;	//观测开始时间的年十个位(00-99)
	gUint8 ucSMonth;	//观测开始时间的月(1-12)
	gUint8 ucSDay;  	//观测开始时间的日(1-31)
	gUint8 ucSHour; 	//观测开始时间的时(0-23)
	gUint8 ucSMinute;//观测开始时间的分(0-59)
	gUint8 ucSSecond;//观测开始时间的秒(0-59)
	gUint8 ucTimeFrom;//时间来源 0-计算机时钟(1天内未对时)
	//		   1-计算机时钟(1天内已对时)
	//		   2-GPS
	//		   3-其它
	gUint8 ucEYear1;	//观测结束时间的年千百位(19-20)
	gUint8 ucEYear2;	//观测结束时间的年十个位(00-99)
	gUint8 ucEMonth;	//观测结束时间的月(1-12)
	gUint8 ucEDay;  	//观测结束时间的日(1-31)
	gUint8 ucEHour; 	//观测结束时间的时(0-23)
	gUint8 ucEMinute;//观测结束时间的分(0-59)
	gUint8 ucESecond;//观测结束时间的秒(0-59)
	gUint8 ucScanMode;//扫描方式  1-RHI  10-PPI和ZPPI
	//  1XX=VPPI(XX为扫描圈数)
	gUint32 ulSmilliSecond;//以微秒为单位表示的秒的小数位
	gUint16 usRHIA;	     //RHI所在的方位角(0.01度为单位) PPI和
	//VPPI时为FFFF
	gInt16 sRHIL;		 //RHI所在的最低仰角(0.01度为单位) PPI和
	//VPPI时为FFFF
	gInt16 sRHIH;		//RHI所在的最高仰角(0.01度为单位) PPI和
	//VPPI时为FFFF
	gUint16 usEchoType;//回波类型  0x405a-Z  0x406a-V
	// 0x407a-W  0x408a-ZVW三要素
	gUint16 usProdCode; //数据类型  0x8001-PPI数据
	//  0x8002-RHI数据
	//  0x8003-VPPI数据
	//   0x8004-单强度PPI数据
	//   0x8005-CAPPI数据
	gUint8 ucCalibration;//标校状态  0-无  1-自动
	//  2-1星期内人工  3-1月内人工
	gUint8 remain1[3]; //保留字
	VPPISCANPARAMETER  vsp[30]; //对PPI和RHI文件:只有1层结构数据; 对VPPI文件:有N层结构数据
	gInt32 lAntennaG;	//天线增益(0.001dB)
	gInt32 lPower;  	//峰值功率(瓦)
	gInt32 lWavelength;  //波长(微米)
	gUint16 usBeamH;	//垂直波束宽度(秒)
	gUint16 usBeamL;	//水平波束宽度(秒)
	gUint16 usPolarization;//极化状态 0-水平 1-垂直 2-双偏振
	// 3-圆偏振 4-其它
	gUint16 usLogA; //对数动态范围(0.01dB)
	gUint16 usLineA;	//线性动态范围(0.01dB)
	gUint16 usAGCP; //AGC延迟量(微秒)
	gUint16 usFreqMode;//频率方式	1-单重复频率
	//  2-双重复频率3:2  3-双重复频率4:3
	gUint16 usFreqRepeat; //重复频率
	gUint16 usPPPPulse;  //PPP脉冲数
	gUint16 usFFTPoint;//FFT间隔点数
	gUint16 usProcessType;//信号处理方式	1-PPP
	//	2-全程FFT  3-单程FFT
	gUint8 ucClutterT;//杂波消除阀值(即STC)
	gChar cSidelobe;	 	//第一旁瓣(dB)
	gUint8 ucVelocityT; //速度门限
	gUint8 ucFilderP;//地物消除方式	0-无
	//1-IIR滤波器1	2-IIR滤波器2
	//3-IIR滤波器3	4-IIR滤波器4
	gUint8 ucNoiseT;	//噪声消除阀值(即强度门限)
	gUint8 ucSQIT;	//SQI门限
	gUint8 ucIntensityC;//DVIP强度值估算采用的通道
	// 1-对数通道 2-线性通道
	gUint8 ucIntensityR; //强度值估算是否距离订正
	// 0-无(dB) 1-已订正(dBZ)
	gUint8 ucCalNoise;//噪声系数标定值
	gUint8 ucCalPower;//发射功率标定值
	gUint8 ucCalPulseWidth; //脉冲宽度标定值
	gUint8 ucCalWorkFreq; //工作频率标定值
	gUint8 ucCalLog;	//对数斜率标定值
	gChar remain3[95]; //保留字 原始头文件为char remain3[92]，
	//如果保留字节为92，在VC中读出的文件头长1021字节，数据偏移地址为0，
	//而在C++ Builder中读出的文件头长1024字节，数据偏移地址为1024,
	//如果保留字节为95，在VC中读出的文件头长1024字节，数据偏移地址为1024

	gUint32 liDataOffset;	//数据偏移地址
}CC_DATAHEADER;
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//3.数据排列
//对于非973格式的文件：
typedef struct tagCC_DAT
{
	gInt16 z[500];//Z数据(500个字) ---- 放大10倍放置的dBz数据
	gInt16 v[500];//V数据(500个字) ---- 放大10倍放置的m/s数据
	gInt16 w[500];//W数据(500个字) ---- 放大10倍放置的m/s数据
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
