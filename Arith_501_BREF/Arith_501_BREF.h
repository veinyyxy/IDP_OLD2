//////////////////////////////////////////////////////////////////////////
// Arith_501_BREF.h
// created by ChenXiang 20080424
// 501�Ų�Ʒ,�����㷨,BREF
//////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef ARITH_501_BREF_H
#define ARITH_501_BREF_H

#include "IArith.h"
#include "GXList.h"
#include "Product_501_BREF.h"

typedef struct 
{
	gUint8  n;							//������� 0=�����㣬1=ȡ�²㣬2=���²��ֵ�� 3=ȡ�ϲ�

	gUint8  be;							//�²�PPI���///�������Ǻ���
	gInt16   bR; 						//б��(ǰ��λ��)///�²�����///����
	gUint8  bh1; 						//�²�б��ǰ���ֵϵ�����Ŵ�100��
	gUint8  bh2;						//�²�б�����ֵϵ�����Ŵ�100��
	gUint8  bf; 						//�´�ֱϵ�����Ŵ�100��
	
	gUint8  te; 						//�ϲ�PPI���///�������Ǻ���
	gInt16   tR;						//б��(ǰ��λ��)///�ϲ�����///����
	gUint8  th1;						//�ϲ�б��ǰ���ֵϵ�����Ŵ�100��
	gUint8  th2;						//�ϲ�б�����ֵϵ�����Ŵ�100��
	gUint8  tf; 						//�ϴ�ֱϵ�� �Ŵ�100��

	gInt16  mR;							//Ŀ��������ĵľ������(б��)
} BREFCAPSTRUCT;

class DLLEXP_GSYS Arith_501_BREF: public IArith
{
private:
	gInt32	m_StartBin;					//CAPPI����ʼ�����
	gInt32	m_EndBin;					//CAPPI�Ľ��������
	gInt16	m_SiteNum;					//�״�վ������
	bool	m_Valid;					//�ж�ִ���Ƿ�ɹ�
	BREFCAPSTRUCT *m_pCappiCoefficient;	//�ṹ������
	gFloat	m_Escale;					//�Ƕ�ת����ϵ��
	gUint16 *m_pMultifWeight;			//Ȩ�ش�����    ��
	gInt32	*m_pMultiRW;				//�����ʳ�Ȩ�� ������ ��
	gInt16	***m_pRadarDataR;			//���淴�������ݾ���ָ��Ķ�λ����
	gInt16	**m_el_tilts;				//��վ����ֵ��ά����

	gInt32	*m_SiteLatNBorderline;		//վ�㱱�߽�����
	gInt32	*m_SiteLatSBorderline;		//վ���ϱ߽�����
	gInt32	*m_SiteLonWBorderline;		//վ�����߽�����
	gInt32	*m_SiteLonEBorderline;		//վ�㶫�߽�����
	gInt32	*m_pSiteLon;				//վ�㾭������ָ��
	gInt32	*m_pSiteLat;				//վ��γ������ָ��
	gInt16	*m_pSiteHigh;				//վ��߶�����ָ��
	gUint8	*m_pPPI_L;					//��վ��������
	gInt16	*m_pCap_nrays;				//��վ�ķ�λ����
	gInt32	(*m_pCap_gates)[20];		//��վ�Ŀ���
	gInt32	*m_pGateLen;				//��վ�Ŀⳤ
	gInt32	**m_ppLonLatData;			//�洢վ�㼫����ת��γ������ָ��
	gInt32  m_ResLonLat;				//��γ����ֱ���

	gInt32	m_StartLon;					//��ʼ���� ��//0.00001�� (������)
	gInt32	m_EndLon;					//�������� ��//0.00001�� (������)
	gInt32	m_StartLat;					//��ʼγ�� ��//0.00001�� (������)
	gInt32	m_EndLat;					//����γ�� ��//0.00001�� (������)
	gInt32 m_M;							//�������� ���򳤶� ��Ӧy ���㶼һ��
	gInt32 m_N;							//�������� ���򳤶� ��Ӧx ���㶼һ��
	bool    *m_ErrorData;				//��Ч��������
	gInt16 m_ScaleOfBaseData;			//���������ű���
	gInt16 m_ScaleOfMultiData;			//��Ʒ�������ű���

	GXList< GenerationData *> * m_pGenerateProductList;
	ReadParam * m_pParameter;
	GXList< GenerationData > * m_pOutputProductList;//����������ݵ��б�
	Product_501_BREF* m_pOut;

public:
	Arith_501_BREF(void);
	~Arith_501_BREF(void);

	SYSMETHOD(Initialize)();				//��ʼ������
	SYSMETHOD(LoadData)(void *pValue,ReadParam * ReadParameter);
	SYSMETHOD(OutPutData)(void * pValue);	//�������������ݵ�pValue
	SYSMETHOD(Execute)();					//ִ���㷨
	SYSMETHOD(UnInitialize)();				//����ʼ��

protected:	
	GHRESULT SingleProduce(	gInt16 CAPPI_High,gInt16 *PolCappiLayerR,gUint8	*PolCappiLayerWeight,gInt16 pos);//��վ����漫����
	GHRESULT LayerMultiProduce(gInt16 CAPPI_High,gInt16 Layer);				 //���������� �澭γ���� �ļ���Ȩ�����鲢���յ��ļ�

	gInt32 Max(gInt32* p,gInt16 Num);		//ȡ�����������������
	gInt32 Min(gInt32* p,gInt16 Num);		//ȡ����������С������
	gInt16 Min(gInt16* p,gInt16 Num);		//ȡ16λ����������С������
	GHRESULT CalculateCrossRange(gInt32 SiteCompCappi_H,gInt16 *el_tilts,gInt16 pos,gInt32* EI_Distance);
	GHRESULT CreateCappiTab(gInt32 SiteCompCappi_H,gInt16 *el_tilts,gInt16 pos,gInt32* EI_Distance);

};

#endif //ARITH_501_BREF_H
