#pragma once

#include <QStringList>

#include "InterpolationRadarDataInterface.h"

#define REarth 6371300 // ����뾶����λ ��

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
	int       m_eleCnt;  //���ǲ���
	int       m_azimuthNum;  //��λ��
	int       m_count_gate; //�������
	int       m_len_gate;  //����ⳤ��
	int		  m_radarHeight;  //�״�߶ȣ�08.01
	std::list<gInt32> *m_eleList;
	int ***m_sectionArr;//���ڴ��"����-�������-��λ��"����ά���� limei,090105
	double    *m_ration;  //���ÿһ������ر�
	spoint m_Sectonstar;
	spoint m_Sectonend;
	double ProductCenter_X ;//��Ļ����=double( ScreenSize.GetWidth())/2.0+ m_size.GetWidth() +m_oldsize.GetWidth() + mx_changetuozhuai;
	double ProductCenter_Y ;//��Ļ����= double(ScreenSize.GetHeight())/2.0 +m_size.GetHeight() +m_oldsize.GetHeight() + my_changetuozhuai;
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
	double dRatio; //�ֱ���

	bool m_bAllArea;
	bool m_bPie;
	float m_vMin;
	float m_vMax;
};

