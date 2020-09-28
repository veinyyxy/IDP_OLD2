#pragma once

#include <QStringList>

class CData;
class CElement;

typedef struct CressmanParam
{
	int isiz;			// ���� --lon
	int jsiz;			// ����-- lat
	int nrad;			// cressman ������Χ�뾶��Ŀ
	float lonmin;		// ������Сֵ
	float lonmax;		// �������ֵ
	float latmin;		// γ����Сֵ
	float latmax;		// γ�����ֵ
	float fUndef;		// ��Чֵ
	float rads[5];		// cressman ������Χ�뾶����

	CressmanParam():isiz(60),jsiz(60),nrad(5),lonmin(115),lonmax(121),latmin(23),latmax(29),fUndef(-999.0)
	{
		rads[0] = 10;
		rads[1] = 7;
		rads[2] = 5.6;
		rads[3] = 2;
		rads[4] = 1;
	}
}CressmanParam;

class CressmanInterpolation
{
public:
	CressmanInterpolation(void);
	~CressmanInterpolation(void);

	/**************************************************************************/
    /* ��    �ܣ� ��Micaps3�����ݽ���Cressman��ֵ
    /* �� �� ֵ:  ��
	/* ��    �ߣ� Liu Xinsheng				
    /* ��    �ڣ� 2011-05-11
	/****************************************************************************/
	void Execute(/*CressmanParam *structParam, */CData *pDataArray, double **dArrayValue);

	/**************************************************************************/
    /* ��    �ܣ� 
    /* �� �� ֵ:  ��
	/* ��    �ߣ� 
    /* ��    �ڣ� 2011-05-11
	/****************************************************************************/
	bool SetCressmanParam( QString cellSizeList,QStringList noValueList,QStringList radList,QStringList rangeList );

	/**************************************************************************/
    /* ��    �ܣ� 
    /* �� �� ֵ:  ��
	/* ��    �ߣ�
    /* ��    �ڣ� 2011-05-11
	/****************************************************************************/
	CressmanParam * GetCressMenParam(){return &m_cressmanParam;};

	/**************************************************************************/
    /* ��    �ܣ� ����Micaps 3������Element��Ϣ����
    /* �� �� ֵ:  ��
	/* ��    �ߣ� Liu Xinsheng				
    /* ��    �ڣ� 2011-05-12
	/****************************************************************************/
	//void SetElementInfo(CElement *pElem/*, CressmanParam *structParam*/);

	void SetMissingValue(double missValue){m_cressmanParam.fUndef = missValue;};

private:
	CressmanParam m_cressmanParam;
};

