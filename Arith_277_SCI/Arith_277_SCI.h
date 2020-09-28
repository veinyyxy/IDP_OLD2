//////////////////////////////////////////////////////////////////////////
//modify by yangys 080610
//////////////////////////////////////////////////////////////////////////
#ifndef ARITH_277_SCI_H
#define ARITH_277_SCI_H
#include "IArith.h"
#include "GDef.h"
#include "GXList.h"
#include "GData.h"
#include "StandardRadarDataX.h"
//#include "PRM.h"
#include "Product_277_SCI.h"
#include "Struct_277_SCI.h"
#include <vector>

#if _MSC_VER > 1000
#pragma once
#endif
// PCS = potential Cell Segment
#define PCS_NONE       -1
#define PCS_BEGIN      2
#define PCS_CONTINUE   1
#define PCS_END        0

#define MAX_THRESHOLD_REF 15
#define MAX_CELLS         100
#define UNKNOWN_STORM_TOP 28
#define MAX_TILTS_NUM     14
#define IR                1.21//����������
#define RE                6371//����뾶KM
#define MRM               80  ///dBZ
#define PIE               1.37///��ǿָ����dBZe
#define MMF               486 ///�����˷����ӣ�mm6/m3
#define MWF               53000///����Ȩ�����ӣ�hr.kg/km2m2

struct tagXML_Index
{
	gInt32 Threshold_Dropout_Count;//��̭������ֵ
	gInt32 Threshold_Dropout_DifRef;//��̭�����ʲ���ֵ
	gFloat Threshold_Segment_Length;//�籩�γ�����ֵ
	gInt32 THRESHOLD_REF_NUMBER;//��С��������ֵ�ĸ���
	gInt32 THRESHOLD_REF_VALUES[15];//��С��������ֵ
	gInt32 Threshold_AZ_SEP;//��С��λ����ֵ
	gFloat Threshold_OVERLAP;//��С�ص���ֵ
	gInt32 ThresholdOfSegmentNumber;//һ���籩�庬�����ٷ籩�θ�����ֵ
	gFloat Threshold_AreaCC;//��С�ص������ֵ
	gFloat Threshold_Radii[3];//��С����������ֵ
	gInt32 search_Radii_Num;//��С����������ֵ����
	gFloat Threshold_DIS_Delete;//���ڷ籩���ˮƽɾ����ֵ ��λ��KM
	gInt32 Threshold_DDE_Delete;//���ڷ籩��ĸ߶�ɾ����ֵ
	gFloat Threshold_DIS_Merge;//�жϷ籩�ϲ���ˮƽ������ֵ��km
	gFloat Threshold_HEI_Merge;//�жϷ籩�ϲ��ĸ߶���ֵ��km
	gInt32 Threshold_ELE_Merge;//�жϷ籩�ϲ���������ֵ
};

typedef struct tagStormCellSegmentStructure
{  //total 36 bytes
	gInt16 RF_dBZ ;  // dBZ of reflectivity factor threshold
	gInt16 az;  // Azimuth angle 0.1degree
	gInt16 SVbeg, SVend;
	gFloat RSbeg, RSend;
	gFloat len;
	gFloat maxZ;
	gFloat MWL;       //The MASS WEIGHTED LEGNTH(Segment)
	gFloat MWLS;      //The MASS WEIGHTED LEGNTH SQUARED(Segment)
	gInt16 ICP;       //the i-th potential component
	gInt16 ITR;       //the i-th threshold reflectivity
}STORM_CELLSEGMENT;


class Arith_277_SCI : public IArith
{
public:
	Arith_277_SCI(void);
	~Arith_277_SCI(void);

	SYSMETHOD(Initialize)() ;
	SYSMETHOD(LoadData)(void *pValue,ReadParam * ReadParameter) ;
	SYSMETHOD(Execute)();
	SYSMETHOD(OutPutData)(void * pValue) ;
	SYSMETHOD(UnInitialize)();
private:

	gInt32 THRESHOLD_REF_NUMBER ;
	gInt32* THRESHOLD_REF_VALUES;

	gInt16 Threshold_Dropout_Count;
	gInt16 Threshold_Dropout_DifRef ;    //5dBZ
	gFloat Threshold_Segment_Length;  //Km;

	gInt16* CSpos;

	gInt16* NumofSeg;
	//gInt16* MEMOFSEG;//maximum=3072

	//----- attributes for Cell Components -------//

	// Threshlods for cell component

	gInt16 Threshold_AZ_SEP;     //0.1 degree
	gInt16 ThresholdOfSegmentNumber ;      //
	gFloat Threshold_OVERLAP;
	gFloat Threshold_AreaCC ;     // Km*km


	gInt32* NumofCpn;
	gInt16* MEMOFCPN;     // maximum=512

	//---------- attributes for storm Cells ------------------//
	gInt32   NumofCell;   // the number of 3D storm cell identified
	gFloat daz;    // average seperation of azimuth on an elevation
	gFloat SVL_km;  // length of a sample volume, in KM
	gFloat Threshold_DIS_Delete;
	gInt16 Threshold_DDE_Delete;
	gFloat Threshold_DIS_Merge;
	gFloat Threshold_HEI_Merge;
	gInt16 Threshold_ELE_Merge;
	gFloat Threshold_Radii[3];
	gInt32 search_Radii_Num;
public:
	gFloat Temp_Length;

private:

	bool Init_ref_threshold() ;

	void ComMassWeiLenSquSegment(gInt32 SVbeg,gInt32 SVend,gFloat *DBZE,gFloat *RS,gFloat &MWLS);

	void ComMassWeiLenSegment(gInt32 SVbeg,gInt32 SVend,gFloat *DBZE,gFloat *RS,gFloat &MWL);

	gInt32  CellSegments(StandardRadarData * pGData, gInt32 tilt,vector<STORM_CELLSEGMENT> &m_VolCS) ;

	gInt32  Cell2DComponents(StandardRadarData * pGData, gInt32 tilt, vector<STORM_CELLSEGMENT> m_VolCS, vector<STORM_CELLCOMPONENT> &m_VolCC);

	gInt32  StormID_correlated(StandardRadarData * pGData, vector<vector<STORM_CELLCOMPONENT> >  m_VolCCG, vector<STORM_CELLATTRIBUTES> &m_Cell);

	void CellLocation(StandardRadarData * pGData, /*vector<vector<STORM_CELLCOMPONENT>>  m_VolCCG,*/ STORM_CELLATTRIBUTES *pCell);

	void Cell_VIL(StandardRadarData * pGData,/* vector<vector<STORM_CELLCOMPONENT>>  m_VolCCG,*/ STORM_CELLATTRIBUTES *pCell);

	gInt32  StormID_merger(StandardRadarData * pGData,/*vector<vector<STORM_CELLCOMPONENT>>  m_VolCCG,*/ vector<STORM_CELLATTRIBUTES> &m_Cell );

	GHRESULT StormCell_ID( StandardRadarData * pGData,tagXML_Index index, Product_277_SCI* Product);//�����ú���
private:

	GXList< GenerationData *> * m_pGenerateProductList;
	ReadParam * ReadP;
	GXList< GenerationData > * m_pOutputProductList;

};

#endif
