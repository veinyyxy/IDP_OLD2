#pragma once
#include "radarsingledataprovider_global.h"

#include <QtGui/QWidget>

typedef struct PerColorPercent
{
	QColor qcolor;
	double dpercent;
}PerColorPercent,*pPColorPercent;

class RADARSINGLEDATAPROVIDER_EXPORT  CDraw3dPieChart :public QWidget
{
	Q_OBJECT

public:
	CDraw3dPieChart(QWidget *parent = 0);
	~CDraw3dPieChart(void);
	void   setPiePos(int iPosX, int iPosY); 
	void   setPieSize(int iWidth, int iHeight); 
	void   setPiePerCent(QList<PerColorPercent> &lstPercent,QVector<float> &nXVect,double aArea); 
	void   setChartDepth(int iDepth); 
	void   setChartDistance(int iDistence);// 实现小部分扇形插入效果
	void   paintEvent(QPaintEvent *event); // override 
	void closeEvent(QCloseEvent *event);	//关闭窗口

Q_SIGNALS:
	void closewindow();

private: 
	int    m_iPosX; 
	int    m_iPosY; 
	int    m_iWidth; 
	int    m_iHeight; 
	int    m_iDepth; 
	int    m_iDistence; 
	double m_totalarea;
	QList<PerColorPercent> m_lstPercent; 
	QVector<float> m_nvx;

	QList<int>  m_lstSpanAgl; 
	QList<int>  m_lstStartAgl; 
};

