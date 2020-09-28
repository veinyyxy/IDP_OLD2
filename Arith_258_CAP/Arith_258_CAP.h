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
	gUint8  r;  //CAPPI�ľ���
	gUint8  n;  //������� 0=�����㣬1=ȡ�²㣬2=���²��ֵ�� 3=ȡ�ϲ�

	gUint8  be; //�²�PPI���
	gInt16 bR; //б��(ǰ��λ��)
	gUint8  bh1; //�²�б��ǰ���ֵϵ�����Ŵ�100��
	gUint8  bh2; //�²�б�����ֵϵ�����Ŵ�100��
	gUint8 bf; //��ֱϵ�����Ŵ�100��
	
	gUint8  te; //�ϲ�PPI���
	gInt16  tR; //б��(���λ��)
	gUint8  th1; //�ϲ�б��ǰ���ֵϵ�����Ŵ�100��
	gUint8  th2; //�ϲ�б�����ֵϵ�����Ŵ�100��
	gUint8  tf; //�Ŵ�100��
} CAPSTRUCT;

class Arith_258_CAP:public IArith
{
public:
	Arith_258_CAP(void);
	~Arith_258_CAP(void);

	SYSMETHOD(Initialize)() ;			//��ʼ������
	SYSMETHOD(LoadData)	(void *pValue,ReadParam * /*ReadParameter*/) ;//��������
	SYSMETHOD(Execute) ();//ִ���㷨
	//SYSMETHOD(GetResultSize) (int &ResultSize) = 0;//ȡ�ý����С�Ա������ڴ�
	SYSMETHOD(OutPutData)(void * pValue) ;//ȡ���������
	SYSMETHOD(UnInitialize)	() ;//����ʼ��

private:

	int CAPPIMaker(int Height_Above_Radar,int NumofHeight, StandardRadarData * pGData,  Product_258_CAP* pRadialData);//�ȸ��淴�������ɺ���

	bool Initialization(int ppi_l,StandardRadarData * pGData);//�Եȸ���ṹ����г�ʼ��

	void CalculateCrossRange(float Height_Above_Radar,int ppi_l, StandardRadarData * pGData);//��ȸ������Բ׶��Ľ���ˮƽ���롢б��Ͷ�Ӧˮƽ�����е�

	void CreateCAPPITab(float Height_Above_Radar,int ppi_l,  StandardRadarData * pGData) ;//����ȸ����ֵϵ��

private:

	float *n_C;
	CAPSTRUCT *m_cap ;
	float Escale;

	GXList< GenerationData *> * m_pGenerateProductList;
	ReadParam * ReadP;
	GXList< GenerationData > * m_pOutputProductList;//����������ݵ��б�

};

#endif // !defined(_CAPPI_H_YANGHONGPING_INCLUDED_)
