// CAPPI.h: interface for the CCAPPI class.
//create by yangys 20080417
//////////////////////////////////////////////////////////////////////

#ifndef ARITH_258_CAP_H
#define ARITH_258_CAP_H
#include "IArith.h"
#include "GDef.h"
#include "GXList.h"
#include "GData.h"
#include "StandardRadarDataX.h"
#include "Product_258_CAP.h"

typedef struct 
{
	gUint8  r;  //CAPPI的距离
	gUint8  n;  //层数标记 0=不计算，1=取下层，2=上下层插值， 3=取上层

	gUint8  be; //下层PPI序号
	gInt16 bR; //斜距(前库位置)
	gUint8  bh1; //下层斜距前库插值系数，放大100倍
	gUint8  bh2; //下层斜距后库插值系数，放大100倍
	gUint8 bf; //垂直系数，放大100倍
	
	gUint8  te; //上层PPI序号
	gInt16  tR; //斜距(后库位置)
	gUint8  th1; //上层斜距前库插值系数，放大100倍
	gUint8  th2; //上层斜距后库插值系数，放大100倍
	gUint8  tf; //放大100倍
} CAPSTRUCT;

class Arith_258_CAP:public IArith
{
public:
	Arith_258_CAP(void);
	~Arith_258_CAP(void);

	SYSMETHOD(Initialize)() ;			//初始化对象
	SYSMETHOD(LoadData)	(void *pValue,ReadParam * /*ReadParameter*/) ;//读入数据
	SYSMETHOD(Execute) ();//执行算法
	//SYSMETHOD(GetResultSize) (int &ResultSize) = 0;//取得结果大小以便申请内存
	SYSMETHOD(OutPutData)(void * pValue) ;//取出结果数据
	SYSMETHOD(UnInitialize)	() ;//反初始化

private:

	int CAPPIMaker(int Height_Above_Radar,int NumofHeight, StandardRadarData * pGData,  Product_258_CAP* pRadialData);//等高面反射率生成函数

	bool Initialization(int ppi_l,StandardRadarData * pGData);//对等高面结构体进行初始化

	void CalculateCrossRange(float Height_Above_Radar,int ppi_l, StandardRadarData * pGData);//求等高面与各圆锥面的交点水平距离、斜距和对应水平距离中点

	void CreateCAPPITab(float Height_Above_Radar,int ppi_l,  StandardRadarData * pGData) ;//计算等高面插值系数

private:

	float *n_C;
	CAPSTRUCT *m_cap ;
	float Escale;

	GXList< GenerationData *> * m_pGenerateProductList;
	ReadParam * ReadP;
	GXList< GenerationData > * m_pOutputProductList;//用来输出数据的列表

};

#endif // !defined(_CAPPI_H_YANGHONGPING_INCLUDED_)
