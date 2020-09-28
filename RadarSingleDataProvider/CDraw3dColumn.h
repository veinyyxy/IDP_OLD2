#pragma once
#include "radarsingledataprovider_global.h"

#include <QWidget>

typedef struct PerColorPerInfo
{
	QColor qcolor;
	double dpercent;
}PerColorPerInfo,*pPerColorPerInfo;
class RADARSINGLEDATAPROVIDER_EXPORT CDraw3dColumn : public QWidget
{
public:
	CDraw3dColumn(QWidget *parent =0);
	~CDraw3dColumn(void);
	void   setPiePerCent(int ny,QList<PerColorPerInfo> &lstPercent,QVector<float> &nXVect,double aArea);
	void   paintEvent(QPaintEvent *event); // override 
private:
	int m_nY;
	double m_totalarea;
	QVector<float> m_nvx;
	QList<PerColorPerInfo> m_lstPercent; 
};

