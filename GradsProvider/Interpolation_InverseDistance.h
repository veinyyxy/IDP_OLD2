
#ifndef HEADER_INCLUDED__Interpolation_InverseDistance_H
#define HEADER_INCLUDED__Interpolation_InverseDistance_H

//#include "Interpolation.h"
#include "QT_Include.h"
#include "../CDataElement/CElement.h"

typedef struct IDWParam
{
	int weight; //��Ȩ����
	int power;
	int bandwidth;
	int mode;
	int pointMax;
	float radius;

	int isiz;			// ���� --lon
	int jsiz;			// ����-- lat
	float lonmin;		// ������Сֵ
	float lonmax;		// �������ֵ
	float latmin;		// γ����Сֵ
	float latmax;		// γ�����ֵ
	float fUndef;		// ��Чֵ
	QString strange;
	//weight(0),power(2),bandwidth(1),mode(0),pointMax(12),radius(60),isiz(60),jsiz(60)
	IDWParam():weight(0),power(2),bandwidth(1),mode(0),pointMax(12),radius(60),isiz(60),jsiz(60),lonmin(115),lonmax(121),latmin(23),latmax(29),fUndef(-999.0),strange("������")
	{		

	}
}IDWParam;

inline double Get_Distance(double x,double y,double ix,double iy)
{
	return sqrtf((x-ix)*(x-ix)+(y-iy)*(y-iy));
}

class InverseDistanceInterpolation 
{
public:
	InverseDistanceInterpolation(void);


	void     SetIDWParameters(int weight ,double power,double bandwidth,int mode,int PointsMax,int Radius,int Xsize,int Ysize,float lonmin,float lonmax,float latmin,float latmax,float undef,QString strSearchRange);

	void     Execute(CData *pDataArray, double **dArrayValue);

	void    SetMissingValue(double missValue) {m_IDWParam.fUndef = missValue;}

	void    SetElementInfo(CElement *pElem);

protected:

	virtual bool		GetValue			(double x, double y, double &z);	

	virtual bool		On_InitializePoints			(void)							{	return( true );	}

	virtual bool		On_Finalize				(void)							{	return( true );	}

private:
/*
	int				m_Weighting, m_nPoints_Max, m_Mode;

	double			m_Power, m_Bandwidth, m_Radius;

	float m_lonmin;		// ������Сֵ

	float m_lonmax;		// �������ֵ

	float m_latmin;		// γ����Сֵ

	float m_latmax;		// γ�����ֵ

	float m_fUndef;		// ��Чֵ
*/
	inline double	Get_Weight			(double Distance);

	inline int				Select_Nearest_Points	(double x, double y, int maxPoints, double Radius = 0.0, int iQuadrant = -1);

	inline bool			Get_Selected_Point		(int i, double &x, double &y, double &z) const ;
	
private:
	inline void AllocateMemory(double **p,int x, int y)
	{ 	
		p = new double* [x];
		for (int i = 0;i < x;++i)
		{
			(p)[i] = new double[y];
			memset((p)[i],0,y*sizeof(double));
		}
	}

	inline void ReleaseMemory(double **p, int x, int y)
	{
		for(int i = 0;i < x;++i) 
		{ 
			delete [] (p)[i];
		}
		delete [] p;
		p=NULL;
	}
private:

	QVector<QVector3D> m_DataArray;

//	QVector<float> m_PorpertyArray;

	QVector<QVector3D> m_SelectedData;	
	QList<QVector3D> m_SelectedDataList;

	typedef QVector<QVector<QVector3D>*> selectdatarry;

	selectdatarry datarry;
	IDWParam m_IDWParam;

};


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
#endif // #ifndef HEADER_INCLUDED__Interpolation_InverseDistance_H
