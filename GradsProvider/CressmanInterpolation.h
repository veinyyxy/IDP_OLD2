#pragma once

#include <QStringList>

class CData;
class CElement;

typedef struct CressmanParam
{
	int isiz;			// 列数 --lon
	int jsiz;			// 行数-- lat
	int nrad;			// cressman 搜索范围半径数目
	float lonmin;		// 经度最小值
	float lonmax;		// 经度最大值
	float latmin;		// 纬度最小值
	float latmax;		// 纬度最大值
	float fUndef;		// 无效值
	float rads[5];		// cressman 搜索范围半径长度

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
    /* 功    能： 对Micaps3类数据进行Cressman插值
    /* 返 回 值:  无
	/* 作    者： Liu Xinsheng				
    /* 日    期： 2011-05-11
	/****************************************************************************/
	void Execute(/*CressmanParam *structParam, */CData *pDataArray, double **dArrayValue);

	/**************************************************************************/
    /* 功    能： 
    /* 返 回 值:  无
	/* 作    者： 
    /* 日    期： 2011-05-11
	/****************************************************************************/
	bool SetCressmanParam( QString cellSizeList,QStringList noValueList,QStringList radList,QStringList rangeList );

	/**************************************************************************/
    /* 功    能： 
    /* 返 回 值:  无
	/* 作    者：
    /* 日    期： 2011-05-11
	/****************************************************************************/
	CressmanParam * GetCressMenParam(){return &m_cressmanParam;};

	/**************************************************************************/
    /* 功    能： 设置Micaps 3类数据Element信息描述
    /* 返 回 值:  无
	/* 作    者： Liu Xinsheng				
    /* 日    期： 2011-05-12
	/****************************************************************************/
	//void SetElementInfo(CElement *pElem/*, CressmanParam *structParam*/);

	void SetMissingValue(double missValue){m_cressmanParam.fUndef = missValue;};

private:
	CressmanParam m_cressmanParam;
};

