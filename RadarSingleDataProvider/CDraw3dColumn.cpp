#include "CDraw3dColumn.h"
#include <QPainter>

CDraw3dColumn::CDraw3dColumn(QWidget *parent)
	: QWidget(parent)
{
	//setAttribute(Qt::WA_DeleteOnClose);
	setWindowTitle(tr("面积统计"));
}

CDraw3dColumn::~CDraw3dColumn()
{
	m_nvx.clear();
	m_lstPercent.clear();
}
void   CDraw3dColumn::setPiePerCent(int ny,QList<PerColorPerInfo> &lstPercent,QVector<float> &nXVect,double aArea)
{
	m_lstPercent = lstPercent;
	m_nY = ny;
	m_nvx = nXVect;
	m_totalarea=aArea;
}
void  CDraw3dColumn::paintEvent(QPaintEvent *event)
{
	QPainter pt;
	int nx = m_lstPercent.size()*2;
	int ny = m_nY+2;
	pt.begin(this);
	int xt = this->width();
	int yt = this->height();
	int yu;
	int xf = 50;

	int  xu = (this->width()-70)/nx;
	yu = (yt/ny);
	//画Y轴
	pt.setPen(QPen(Qt::black,1,Qt::SolidLine,Qt::RoundCap));
	//画Y轴刻度
	for (int i = -1;i<=ny-2;i++)
	{
		int ty = yu*(i+3.0/2);
		int nk = ny-2-i;
		pt.drawLine(QLine(xf,ty,xf-4,ty));
		if(0!=nk) pt.drawLine(QLine(xf,ty,xf,ty+yu));
		pt.drawText(xf-30,ty,QString("%1%").arg(QString::number(nk*10)));
	}
	//画X轴
	pt.drawLine(QLine(xf,yu*(ny-1.0/2),50+xu*(nx),yu*(ny-1.0/2)));
	//drawtotalarea
	pt.drawText(this->width()-250,35,QString("总面积是：%1KM2").arg(QString::number(m_totalarea,'f',2)));
	//画X轴刻度
	int nv = 0;
	for (int i = 0;i<nx;i++)
	{
		int tx;
		xu=29;
		tx = xf+xu*i;
		pt.drawLine(QLine(tx,yu*(ny-1.0/2),tx,yu*(ny-1.0/2)+4));
		float xval=m_nvx.at(i);
		pt.drawText(tx,yu*(ny-1.0/2)+16,QString::number(xval));
		if (0==i%2)
		{	int nh = yu*10*m_lstPercent.at(nv).dpercent;
		pt.setBrush(m_lstPercent.at(nv).qcolor);
		pt.drawRect(tx,yu*(ny-1.0/2)-(nh),xu,(nh));
		QPoint  topPointS[4];
		topPointS[0].setX(tx);
		topPointS[0].setY(yu*(ny-1.0/2)-(nh));
		topPointS[1].setX(tx+xu);
		topPointS[1].setY(yu*(ny-1.0/2)-(nh));
		topPointS[3].setX(tx+xu/2);
		topPointS[3].setY(yu*(ny-1.0/2)-(nh)-xu/2);
		topPointS[2].setX(tx+xu+xu/2);
		topPointS[2].setY(yu*(ny-1.0/2)-(nh)-xu/2);
		pt.drawPolygon(topPointS,4);
		pt.drawText(tx+xu/1.5,yu*(ny-1.0/2)-(nh)-xu/2,QString("%1%").arg(QString::number(m_lstPercent.at(nv).dpercent*100,'f',2)));
		QPoint  RPointS[4];
		RPointS[0].setX(tx+xu);
		RPointS[0].setY(yu*(ny-1.0/2)-(nh));
		RPointS[1].setX(tx+xu+xu/2);
		RPointS[1].setY(yu*(ny-1.0/2)-(nh)-xu/2);
		RPointS[2].setX(tx+xu+xu/2);
		RPointS[2].setY(yu*(ny-1.0/2)-xu/2);
		RPointS[3].setX(tx+xu);
		RPointS[3].setY(yu*(ny-1.0/2));
		pt.drawPolygon(RPointS,4);
		nv++;
		}

	}
	//画柱标值
	pt.end();
}