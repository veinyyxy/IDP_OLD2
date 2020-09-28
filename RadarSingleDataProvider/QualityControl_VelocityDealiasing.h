// VelUnfold.h: interface for the QualityControl_VelocityDealiasing class.
// AS by yangys for QualityControl_VelocityDealiasing 20080427
//////////////////////////////////////////////////////////////////////

#ifndef _QUALITYCONTROL_VELOCITYDEALIASING_
#define _QUALITYCONTROL_VELOCITYDEALIASING_
#include <math.h>
#include "../GDataTypes/GDef.h"
#include "../GDataTypes/cDrawProduct.h"

class   QualityControl_VelocityDealiasing
{
public:

	QualityControl_VelocityDealiasing(void);
	virtual ~QualityControl_VelocityDealiasing(void);

    void unfold_tilt_driver(cDrawProduct* pGData,int El_Tilte);//Ö÷º¯Êý
	
private:
	void Init();

	void unfold(cDrawProduct *pGData);

	void search_1st_beam( cDrawProduct *pGData, int &RadialNum,int &AverageRadialVelocity);

	void unfold1(int** ProcessMark ,int RadialNum,cDrawProduct *pGData);

	void unfold2(int Round_Num,int** ProcessMark ,int RadialNum,cDrawProduct *pGData);

	void chkvel(cDrawProduct *pGData);

	void chgbeam( int AzimuthNumber,gInt16* ReferenceAverageRadialVelocity,cDrawProduct *pGData);

	void shearb1s_initial(int** ProcessMark ,int AzimuthNumber,gInt16* ReferenceAverageRadialVelocity,cDrawProduct *pGData); 

	void shearb1s(bool Flag, int** ProcessMark ,int AzimuthNumber,gInt16* ReferenceAverageRadialVelocity,cDrawProduct *pGData);

	void shearb1s1(int Round_Num,bool Flag,int** ProcessMark ,int AzimuthNumber,gInt16* ReferenceAverageRadialVelocity,cDrawProduct *pGData);


	void rfold(gInt16& Data_V,int ReferenceAverageRadialVelocity,gFloat Scale); 



private:
	int  m_missing;
	int  m_range_fold;
	int  m_velny;
	float m_scale;
	int  Quality_tol;
	int    m_layer;
	int  m_AzimuthNumber;
	int  m_DopplerGates;
	int  m_Elevation;
 };

#endif 
