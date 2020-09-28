
//////////////////////////////////////////////////////////////////////////
// CCProductPretreatment.h
// created by sunqc 20090327
// CC雷达产品数据格式,
//注意与SA/SB/CB雷达基数据保存格式（每条径向的格式）不同，这里的格式是整个体扫格式（整个体扫就一个头文件）。
//////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef CCPRODUCTPRETREATMENT2_H
#define CCPRODUCTPRETREATMENT2_H
#include "GData.h"
#include "GDef.h"


#pragma pack(push,1)//这句和下面对应的pop一定要加上，保证对齐

typedef struct tagLayerParam
{
	// 本层观测要素
	unsigned char DataType;       //1=单强度, 2=三要素 单PRF, 3=三要素 双PRF
                                  //4=双线偏振, 5=双线偏振多普勒, 6=双波长（不同天线),7=双波长（共用天线）
	
    //本层退速度模糊状态
	unsigned char Ambiguousp ;	  //0 = 无退速度模糊状态,1 = 软件退速度模糊, 2 = 双T退速度模糊
								  //3 = 批式退速度模糊, 4 = 双T + 软件退速度模糊, 5 = 批式 + 软件退速度模糊
								  //6 = 双PPI退速度模糊, 9 = 其他方式,
	unsigned short Arotate;       //  本层天线转速,计数单位为0.01度/秒，
	                              // 当扫描方式为RHI或PPI时，只在第一个元素中填写，其他元素为0。				      (2-3)
	unsigned short PRF1 ;         // 本层第一种脉冲重复频率,计数单位:1/10Hz (4-5)
	unsigned short PRF2	;         //  本层第二种脉冲重复频率,计数单位:1/10Hz (6-7)
	unsigned short SpulseW;       //  本层的脉冲宽度,计数单位为微秒		  (8-9)
	unsigned short MaxV ;         //  本层的最大可测速度,计数单位为厘米/秒	(10-11)
	unsigned short MaxL	;         //  本层的最大可测距离,以10米为计数单位	(12-13)
	unsigned short ZBinWidth ;    //  本层强度数据的库长,以1/10米为计数单位(14-15)
	unsigned short VBinWidth ;    //  本层速度数据的库长,以1/10米为计数单位(16-17)
	unsigned short WBinWidth ;    //  本层谱宽数据的库长,以1/10米为计数单位(18-19)
	unsigned short ZBinNumber ;   //  本层扫描强度径向的库数				(20-21)
	unsigned short VBinNumber ;   //  本层扫描速度径向的库数				(22-23)
	unsigned short WBinNumber ;	  //  本层扫描谱宽径向的库数				(24-25)
	unsigned short RecordNumber ; //  本层扫描径向个数						(26-27)
	short SwpAngles ;             //  本层的仰角,计数单位为1/100度.
								  //  当扫描方式:RHI不填此数组，PPI时第一个元素为做PPI时的仰角，计数单位为1/100度，其他元素填写-32768。		(28-29)
	char DataForm ;               //  本层径向中的数据排列方式:	
					  // 11单要素排列： CorZ
					  // 12单要素排列： UnZ
					  // 13单要素排列： V
					  // 14单要素排列： W
					  // 21按要素排列：CorZ+ UnZ
					  // 22按要素排列：CorZ+ V+ W
					  // 23按要素排列：UnZ+ V+ W
					  // 24按要素排列：CorZ+UnZ+ V+ W
					  // 4x双偏振按要素排列模式
					  // 6x双偏振多普勒按要素排列模式
					  // 8x双波长按要素排列模式
	unsigned long int Dbegin ;    //  本层数据记录开始位置(字节数)
}LAYERPARAM ;

typedef struct tagBinParam
{
	short Code;   	//			   强度变库长结构代码			
	short Begin;  	//			   开始库的距离，以10米为计数单位
	short End;     	//			   结束库的距离，以10米为计数单位	(4-5)
	short BinLength; //			   库长，以1/10米为计数单位		(6-7)
}BINPARAM ;

typedef struct tagFileVolumn //原始数据文件标识（12字节）
{
	char   FileID[4];		    // 'RD' 为雷达原始数据,'GD'为雷达衍生数据
	float  VersionNo;		    // 表示数据格式的版本号 2.0
	long int HeaderLength;  // 表示文件头的长度
} FILEVOLUMN;

typedef  struct  RadDataHeader  //原始数据文件头（2048字节）
{
         //站址基本情况数据结构,共168个字节
	char Country[30];         	//  国家名，文本格式输入	
	char Province[20];       	//  省名，文本格式输入	
	char Station[40];          	//  站名，文本格式输入
	char StationNumber[10];    	//	区站号，文本格式输入
	char RadarType[20];         //	雷达型号，文本格式输入
	char Longitude[16];       	//	天线所在经度: E115°32'12"
	char Latitude[16];          //  天线所在纬度：N35°30'15"	
	long int LongitudeValue;    //  天线经度，以1/1000度为计数单位,东经为正，西经为负
	long int LatitudeValue ;    //  天线纬度，以1/1000度为计数单位,北纬为正，南纬为负
	long int Height;        	//	天线海拔高度以毫米为计数单位
	short MaxAngle;             //  测站四周地物最大遮挡仰角（以1/100度为计数单位）						(176-177)
	short OptiAngle;            //  测站的最佳观测仰角，地物回波强度<10dBZ，以1/100度为计数单位	(178-179)

  		 //性能参数（共36个字节）
	long int AntennaG ;         // 天线增益以0.001dB为计数单位	
	unsigned short VerBeamW ;   // 垂直波束宽度以1/100度为计数单位
	unsigned short HorBeamW ;   // 水平波束宽度以1/100度为计数单位
	unsigned char Polarizations; // 偏振状况(0=水平,1=垂直,2=双线偏振,3=圆偏振,4=其他)
	unsigned short SideLobe;    // 第一旁瓣，以0.01dB为计数单位
	long int Power;             // 雷达脉冲峰值功率，以瓦为计数单位
	long int WaveLength;        // 波长，以微米为计数单位
	unsigned short LogA;        // 对数接收机动态范围，以0.01dB为计数单位
	unsigned short LineA;    	// 线性接收机动态范围，以0.01dB为计数单位
	unsigned short AGCP;        // AGC延迟量，以微秒为计数单位
	unsigned short LogMinPower; // 对数接收机最小可测功率，计数单位为0.01dBm
	unsigned short LineMinPower; // 线性接收机最小可测功率，计数单位为0.01dBm
	unsigned char ClutterT;     // 杂波消除阈值，计数单位为0.01dB
	unsigned char VelocityP;    // 速度处理方式(0=无速度处理,1=PPP,2=FFT)
	unsigned char FilterP;      // 地物杂波消除方式:  0=无地物杂波消除
								//	  1=地物杂波图扣除法
								//	  2=地物杂波图+滤波器处理
								//	  3=滤波器处理
								//	  4=谱分析处理
								//	  5=其它处理法
	unsigned char NoiseT;       // 噪声消除阈值（0-255）
	unsigned char SQIT;         // SQI阈值，以0.01为记数单位 
	unsigned char IntensityC ;  // RVP强度值估算采用的通道(1=对数通道,2=线性通道)
	unsigned char IntensityR ;  // 强度估算是否进行了距离订正(0=无,1=已进行了距离订正)

		//观测参数（共1282个字节） 
	unsigned char SType;        // 扫描方式(1=RHI,10=PPI,1XX=VOL，XX为扫描层数)
	unsigned short SYear;       // 观测记录开始时间的年（2001）
	unsigned char SMonth;		// 观测记录开始时间的月（1-12）
	unsigned char SDay;			// 观测记录开始时间的日（1-31）
	unsigned char SHour;		// 观测记录开始时间的时（00-23）
	unsigned char SMinute;		// 观测记录开始时间的分（00-59）
	unsigned char SSecond;		// 观测记录开始时间的秒（00-59）
	unsigned char TimeP; 		// 时间来源
								// 0=计算机时钟，但一天内未进行对时
								// 1=计算机时钟，一天内已进行对时
								// 2=GPS
								// 3=其他
	unsigned long int SMillisecond; // 秒的小数位（计数单位微秒）
	unsigned char Calibration; 		// 标校状态
						// 0=无标校
						// 1=自动标校
						// 2=一星期内人工标校
						// 3=一月内人工标校
						// 其他码不用
	unsigned char IntensityI; 	// 强度积分次数（32-128）
	unsigned char  VPSamples;	// 速度处理样本（31-255）
	unsigned short ZStartBin; 	// 强度有效数据开始库数
	unsigned short VStartBin;  	// 速度有效数据开始库数
	unsigned short WStartBin;  	// 谱宽有效数据开始库数
	LAYERPARAM LayerParam[32];      // 层参数数据结构(各圈扫描状态设置)(238-1357)
	unsigned short RHIA;		// RHI为方位角，计数单位为1/100度，PPI和立体扫描时为65535
	short RHIL;			// RHI为最低仰角，计数单位为1/100度，做其他扫描时为-32768
	short RHIH;			// RHI为最高仰角，计数单位为1/100度，做其他扫描时为-32768
	unsigned short EYear;		// 观测记录结束时间的年（2000 -   ）
	unsigned char EMonth;		// 观测记录结束时间的月（1-12）
	unsigned char EDay;		// 观测记录结束时间的日（1-31）
	unsigned char EHour;		// 观测记录结束时间的时（00-23）
	unsigned char EMinute;		// 观测记录结束时间的分（00-59）
	unsigned char ESecond;		// 观测记录结束时间的秒（00-59）
	unsigned char ETenth;		// 观测记录结束时间的1/100秒（00-99）
	unsigned short ZBinByte;      	// 原始强度数据中库长无变化填0,有变化填占用字节数
    BINPARAM BinRange1[5];          // 5个8字节(强度库长无变化为空字节)
	unsigned short VBinByte;        // 原始速度数据中库长无变化填0,有变化填占用字节数
	BINPARAM BinRange2[5];			// 5个8字节（速度库长无变化为空字节）(1416-1455)
	unsigned short WBinByte;        // 原始谱宽数据中库长无变化填0,有变化填占用字节数
	BINPARAM BinRange3[5];			// 5个8字节（谱宽库长无变化为空字节）(1458-1497)

		//其它信息参数（共562个字节）
	char StationID[4] ;             // 台站代码
	char Reserved[558] ;            // 备用字节560个

}RADDATAHEADER ;

typedef struct tagRadialHeader   
{
	//数据记录块的结构头：
	         short int Elev	;	// 仰角，计数单位1/100度
	unsigned short int Azimuth;		// 方位，计数单位1/100度
	     unsigned char hour;		// 时
	     unsigned char min;		// 分
	     unsigned char sec;		// 秒
	unsigned  long int millionsec;		// 秒的小数（计数单位为微秒）
}RADIALHEADER;

typedef struct tagRadialDatas   
{
	//数据记录块的结构头：
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
