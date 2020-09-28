// VelUnfold.h: interface for the QualityControl_VelocityDealiasing class.
// AS by yangys for QualityControl_VelocityDealiasing 20080427
//////////////////////////////////////////////////////////////////////

#ifndef _QUALITYCONTROL_VELOCITYDEALIASING_
#define _QUALITYCONTROL_VELOCITYDEALIASING_
#include "../GDataTypes/IArith.h"
#include "../GDataTypes/GDef.h"
#include "../GDataTypes/GXList.h"
#include "../GDataTypes/GData.h"
#include "../GDataTypes/StandardRadarDataX.h"
#include <math.h>


class  DLLEXP_GSYS QualityControl_VelocityDealiasing:public IArith
{
public:

	QualityControl_VelocityDealiasing(void);
	virtual ~QualityControl_VelocityDealiasing(void);

	SYSMETHOD(Initialize)() ;			//初始化对象
	SYSMETHOD(LoadData)	(void *pValue,ReadParam * ReadParameter) ;//读入数据
	SYSMETHOD(Execute) ();//执行算法
	SYSMETHOD(OutPutData)(void * pValue) ;//取出结果数据
	SYSMETHOD(UnInitialize)	() ;//反初始化

private:

	void unfold(StandardRadarData *pGData,gInt32 El_Tilte);
	void search_1st_beam( StandardRadarData *pGData,gInt32 El_Tilte, gInt32 &RadialNum,gInt16 &AverageRadialVelocity);
	void unfold1(gInt16** ProcessMark ,gInt32 RadialNum,StandardRadarData *pGData,gInt32 El_Tilte);
	void unfold2(gInt32 Round_Num,gInt16** ProcessMark ,gInt32 RadialNum,StandardRadarData *pGData,gInt32 El_Tilte);
	void chkvel(StandardRadarData *pGData,gInt32 El_Tilte);
	void chgbeam( gInt32 AzimuthNumber,gInt16* ReferenceAverageRadialVelocity,StandardRadarData *pGData,gInt32 El_Tilte);
	void shearb1s_initial(gInt16** ProcessMark ,gInt32 AzimuthNumber,gInt16* ReferenceAverageRadialVelocity,StandardRadarData *pGData,gInt32 El_Tilte);
	void shearb1s(bool Flag, gInt16** ProcessMark ,gInt32 AzimuthNumber,gInt16* ReferenceAverageRadialVelocity,StandardRadarData *pGData,gInt32 El_Tilte);
	void shearb1s1(gInt32 Round_Num,bool Flag,gInt16** ProcessMark ,gInt32 AzimuthNumber,gInt16* ReferenceAverageRadialVelocity,StandardRadarData *pGData,gInt32 El_Tilte );
	void rfold(gInt16& Data_V,gInt16 ReferenceAverageRadialVelocity,gFloat Scale);

    void unfold_tilt_driver(StandardRadarData *pGData,gInt32 El_Tilte);//主函数

private:
	gInt16 m_missing;
	gInt16 m_range_fold;
	gInt16 m_velny;
	gFloat m_scale;
	gInt32 Quality_tol;

	GXList< GenerationData > * m_pGenerateProductList;
    GXList< GenerationData > * m_pOutputProductList;//用来输出数据的列表
    ReadParam * ReadP;
 };

#endif
