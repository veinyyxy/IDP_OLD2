#pragma once

#include <QStringList>

#include "InterpolationRadarDataInterface.h"

#define REarth 6371300 // 地球半径，单位 米

struct spoint
{
	double x;
	double y;
};

class InterpolationRadarData : public InterpolationRadarDataInterface
{
public:
	InterpolationRadarData(void);
	virtual ~InterpolationRadarData(void);
	void Profiles();
	bool GetSectionParam();
	double		Get_Angle_Of_Direction(double dx, double dy);
	double		Get_Length(double dx, double dy);
	void SetStartEnd(double* x1, double* y1, double* x2, double* y2);
	void SetStartEnd(double* x1, double* y1, double* x2, double* y2,double dR,double pc_x,double pc_y);
	inline void SetDrawProdunt(cDrawProduct * pDP){m_produceDraw = pDP;}
public:
	cDrawProduct *m_produceDraw;
	int       m_eleCnt;  //仰角层数
	int       m_azimuthNum;  //方位角
	int       m_count_gate; //距离库数
	int       m_len_gate;  //距离库长度
	int		  m_radarHeight;  //雷达高度，08.01
	std::list<gInt32> *m_eleList;
	int ***m_sectionArr;//用于存放"仰角-距离库数-方位角"的三维数组 limei,090105
	double    *m_ration;  //存放每一层的象素比
	spoint m_Sectonstar;
	spoint m_Sectonend;
	double ProductCenter_X ;//屏幕坐标=double( ScreenSize.GetWidth())/2.0+ m_size.GetWidth() +m_oldsize.GetWidth() + mx_changetuozhuai;
	double ProductCenter_Y ;//屏幕坐标= double(ScreenSize.GetHeight())/2.0 +m_size.GetHeight() +m_oldsize.GetHeight() + my_changetuozhuai;
	bool			m_flagsection;
	bool tf;
	double scanR;
	int dh;
	int ds;
	int** buf;
	int max;
	double maxH;
	int maxAz;
	int maxR;
	QStringList AzList;
	QStringList RList;
	int proCode;
	double dRatio; //分辨率

	bool m_bAllArea;
	bool m_bPie;
	float m_vMin;
	float m_vMax;
};

