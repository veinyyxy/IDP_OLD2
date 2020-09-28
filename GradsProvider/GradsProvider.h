#pragma once

#include "GradsConfig.h"

#include "../GeosOnTheOsg/DataProvider.h"

#include "CressmanInterpolation.h"
#include "Interpolation_InverseDistance.h"
//#include "NinePointSmoother.h"

#include "grads.h"
#include "galloc.h"
#include "gagx.h"
#include "gauser.h"
#include "gxX.h"

#include "GeneralContour.h"
#include "GeneralShaded.h"
#include "GeneralVector.h"
#include "GeneralBarb.h"
#include "GeneralStream.h"
#include "GeneralGird.h"

using namespace goto_gis;

class DataInterface;

typedef struct GridInfo   // 格网信息
{
	int nt;                        // 时间层
	int nz;                       //  高度层
	int nx;                       //  列数 --lon
	int ny;                       //  行数-- lat
	float f_lonstart;    //  起始经度
	float f_lonstep;    //  经度步长 
	float f_latstart;     //  起始纬度
	float f_latstep;     //   纬度步长

	GridInfo():nt(1),nz(1),nx(141),ny(81),f_lonstart(70),f_lonstep(0.5),f_latstart(15),f_latstep(0.5)
	{

	}
}GridInfo;

class GRADS_EXPORT_IMPORT GradsProvider : public DataProvider
{

public:
	GradsProvider();
	~GradsProvider();
	virtual bool LoadFile( const std::string& filename );
	void AreaAdd(bool tArea,bool tPie,float tMin,float tMax,osg::Vec3Array* p) {}
	//int GenerateCommand4ContourLine(); //绘制等值线
	//int GenerateCommand4ContourShaded(); //绘制色斑图

	int MakeProduct(MeteorologyProducts* pMP, int iDirect, const std::string& strCmd = "display Va");

	inline void SetPrecision(int Precision){m_nPrecision = Precision;};
	int GradsProvider::CElement2GradsData(
		size_t iLonNCCount, 
		size_t iLatNCCount,  
		size_t iTimeNCCount, 
		size_t iLevelNCCount, 
		float fLonBegin, 
		float fLonEnd,
		float fLonDelta,
		float fLatBegin, 
		float fLatEnd,
		float fLatDelta,
		float fMissingValue,
		float fLevelBegin,
		float fLevelEnd,
		float fLevelDelta, 
		std::vector<float>& fLevelVector, 
		std::string& strTimeBegin,
		std::string& strTimeEnd,
		int iTimeDelta,
		double* dData, 
		int nModel, 
		int useNineP);
	int SetCtlFile(const std::string& ctlFile, const std::string& dataFile);
	void ClearVector();
	void Initialize();

private :
	//int NetCDF2Grads(CElement * pElem1=NULL, CElement * pElem2=NULL, unsigned int nOrder=0);
	//int CElement2GradsData(QList<CElement*> *pListCElement, unsigned int nOrder=0, QString strCalculate = "",int nModel = 1, bool useNineP = false);

	//bool Array4NormalElement(QList<CElement*> *pListCElement,int nx,int ny,int nz,int nt,float f_missingdata,bool NorthToSouth);


	//inline int GraphicType(){return m_graphicType;};
	//inline void GraphicType(int iType){m_graphicType = iType;};
	//inline MeteorologyProducts *GetProduct(){return m_pProduct;};

	//CressmanInterpolation m_Cressman;
	//InverseDistanceInterpolation m_IDWInterpolation;
	//NinePointSmoother m_NinePoint;

	bool m_isRain;

	//QString m_file;					//文件及路径
	int m_graphicType;				//绘图类型
	char m_strCTL[200][200];		//存储*.ctl文件描述内容							Added by Liu Xinsheng 2011-05-06
	int m_nLonNCCount;				//记录数据经度个数
	int m_nLatNCCount;				//记录数据纬度个数
	int m_nTimeNCCount;				//记录数据时间个数
	int m_nLevelNCCount;			//记录数据层级个数
	float m_fLonNCBegin;			//记录数据起始经度
	float m_fLonNCDelta;			//记录数据经度间隔
	float m_fLatNCBegin;			//记录数据起始纬度
	float m_fLatNCDelta;			//记录数据纬度间隔
	float m_fMissingV;				//记录数据缺失值
	int m_nElementNum;				//元素数量
	//QStringList m_strTreeName;		//记录树节点名称 嘉祥用的过度变量

	//int m_FontShow;
	//QColor m_FontColor;
	//unsigned int m_nPixmapIndex;

	//unsigned int m_nSymbolSize;
	//float m_fSymbolAngle;
	//bool m_bShowValue;
	//float m_fMaxValue;
	//float m_fMinValue;

	MeteorologyProducts		* m_pProduct;

	int m_nPrecision;	//小数后有效位数

	double *m_dArrayValue;			//存贮NetCDF2Array()函数中的数组内容			Added by Liu Xinsheng 2011-05-06
	//char m_Command[512];			//脚本命令
	//CElement *m_pTempElement;

	DataInterface* m_pDataInterface;
};