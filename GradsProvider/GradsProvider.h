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

typedef struct GridInfo   // ������Ϣ
{
	int nt;                        // ʱ���
	int nz;                       //  �߶Ȳ�
	int nx;                       //  ���� --lon
	int ny;                       //  ����-- lat
	float f_lonstart;    //  ��ʼ����
	float f_lonstep;    //  ���Ȳ��� 
	float f_latstart;     //  ��ʼγ��
	float f_latstep;     //   γ�Ȳ���

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
	//int GenerateCommand4ContourLine(); //���Ƶ�ֵ��
	//int GenerateCommand4ContourShaded(); //����ɫ��ͼ

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

	//QString m_file;					//�ļ���·��
	int m_graphicType;				//��ͼ����
	char m_strCTL[200][200];		//�洢*.ctl�ļ���������							Added by Liu Xinsheng 2011-05-06
	int m_nLonNCCount;				//��¼���ݾ��ȸ���
	int m_nLatNCCount;				//��¼����γ�ȸ���
	int m_nTimeNCCount;				//��¼����ʱ�����
	int m_nLevelNCCount;			//��¼���ݲ㼶����
	float m_fLonNCBegin;			//��¼������ʼ����
	float m_fLonNCDelta;			//��¼���ݾ��ȼ��
	float m_fLatNCBegin;			//��¼������ʼγ��
	float m_fLatNCDelta;			//��¼����γ�ȼ��
	float m_fMissingV;				//��¼����ȱʧֵ
	int m_nElementNum;				//Ԫ������
	//QStringList m_strTreeName;		//��¼���ڵ����� �����õĹ��ȱ���

	//int m_FontShow;
	//QColor m_FontColor;
	//unsigned int m_nPixmapIndex;

	//unsigned int m_nSymbolSize;
	//float m_fSymbolAngle;
	//bool m_bShowValue;
	//float m_fMaxValue;
	//float m_fMinValue;

	MeteorologyProducts		* m_pProduct;

	int m_nPrecision;	//С������Чλ��

	double *m_dArrayValue;			//����NetCDF2Array()�����е���������			Added by Liu Xinsheng 2011-05-06
	//char m_Command[512];			//�ű�����
	//CElement *m_pTempElement;

	DataInterface* m_pDataInterface;
};