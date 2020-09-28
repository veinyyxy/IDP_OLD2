#include "Draw3dPieChart.h"
#include <QPainter>
#include <math.h> 
/*--------------------------------------------------------------------------*/ 
/*-------------------------- Constant Definition ---------------------------*/ 
/*--------------------------------------------------------------------------*/ 

CDraw3dPieChart::CDraw3dPieChart(QWidget *parent) 
	:QWidget(parent) 
{ 
	//setAttribute(Qt::WA_DeleteOnClose);
	setWindowTitle(tr("面积统计"));
	m_iPosX = this->width()/3; 
	m_iPosY = this->height()/2; 
	m_iWidth = 100; 
	m_iHeight = 100; 
	m_iDepth = 0; 
	m_iDistence = 0; 
	m_lstPercent.clear(); 
	m_lstSpanAgl.clear(); 
	m_lstStartAgl.clear(); 
	this->setUpdatesEnabled(true);
} 

CDraw3dPieChart::~CDraw3dPieChart(void) 
{ 
	m_lstPercent.clear(); 
	m_lstSpanAgl.clear(); 
	m_lstStartAgl.clear();
	
} 

void CDraw3dPieChart::setPiePos(int iPosX, int iPosY) 
{  
	m_iPosX = iPosX; 
	m_iPosY = iPosY; 
} 

void CDraw3dPieChart::setPieSize(int iWidth, int iHeight) 
{ 
	m_iWidth = iWidth; 
	m_iHeight = iHeight; 
} 

void CDraw3dPieChart::setChartDepth(int iDepth) 
{ 
	m_iDepth = iDepth; 
} 

void CDraw3dPieChart::setChartDistance(int iDistence) 
{ 
	m_iDistence = iDistence; 
} 

void CDraw3dPieChart::setPiePerCent(QList<PerColorPercent> &lstPercent,QVector<float> &nXVect,double aArea) 
{ 
	m_lstPercent = lstPercent; 
	m_nvx = nXVect;
	m_totalarea=aArea;
} 

void CDraw3dPieChart::paintEvent(QPaintEvent *event) 
{ 
	Q_UNUSED(event); 

	m_iPosX = this->width()/4; 
	m_iPosY = this->height()/4;

	m_iWidth = this->width()/3;
	m_iHeight = this->height()/3;

	QPainter painter; 
	painter.begin(this); 
	m_lstSpanAgl.clear(); 
	m_lstStartAgl.clear(); 
	m_lstStartAgl.append(60); 
	if (m_lstPercent.size() <= 0) 
	{ 
		return; 
	} 
	for (int i = 0; i < m_lstPercent.size(); i++) 
	{ 
		m_lstSpanAgl.append(ceil(360*m_lstPercent.at(i).dpercent)); 
		if (i == 0) 
		{ 
			continue; 
		} 
		m_lstStartAgl.append(m_lstSpanAgl.at(i-1) + m_lstStartAgl.at(i-1)); 
	} 
	painter.setRenderHint(QPainter::Antialiasing, true); //设置平滑
	painter.setPen(Qt::NoPen); 

	//下部分循环m_iDepth次
	for (int i = 0; i < m_iDepth; i++) 
	{ 
		QRect rectDown(m_iPosX-100, m_iPosY-i, m_iWidth, m_iHeight); 
		for (int j = 0; j < m_lstSpanAgl.size(); j++) 
		{ 
			painter.setBrush(m_lstPercent.at(j).qcolor);
			if (i == m_iDepth-1 || i == 0)
			{
				painter.setPen(QPen(Qt::black,0.2,Qt::SolidLine,Qt::RoundCap));
			}

			painter.drawPie(rectDown, m_lstStartAgl.at(j)*16, m_lstSpanAgl.at(j)*16); 
		} 
	} 
	painter.drawText(this->width()-250,35,QString("总面积是：%1KM2").arg(QString::number(m_totalarea,'f',2)));	
	//最上部分画一次（不同颜色）
	QRect rectUp(m_iPosX-100, m_iPosY-m_iDepth, m_iWidth, m_iHeight);
	for (int i = 0,ii=0; i < m_lstPercent.size(); i++) 
	{ 
		painter.setBrush(m_lstPercent.at(i).qcolor); 
		painter.drawRect(m_iPosX+m_iWidth+40,(m_iPosY-m_iHeight/4+(i*m_iWidth/16)),m_iWidth/20,m_iWidth/20);
		painter.setPen(QPen(Qt::black,0.2,Qt::SolidLine,Qt::RoundCap));
		float Sval=m_nvx.at(ii);float Eval=m_nvx.at(ii+1);
		QString Stempstr = QString::number(Sval);QString Etempstr = QString::number(Eval);
		QString tempstr=QString(tr("跨度为：%1-%2 占据")).arg(Stempstr).arg(Etempstr);
		painter.drawText(m_iPosX+m_iWidth+60,(m_iPosY-m_iHeight/4+(i*m_iWidth/16)+12),tempstr);
		painter.drawText(m_iPosX+m_iWidth+210,(m_iPosY-m_iHeight/4+(i*m_iWidth/16)+12),QString("%1%").arg(QString::number(m_lstPercent.at(i).dpercent*100,'f',2)));
		painter.drawText(m_iPosX+m_iWidth+254,(m_iPosY-m_iHeight/4+(i*m_iWidth/16)+12),QString("(%1KM2)").arg(QString::number(m_totalarea*m_lstPercent.at(i).dpercent,'f',2)));
		painter.drawPie(rectUp, m_lstStartAgl.at(i)*16, m_lstSpanAgl.at(i)*16); 
		ii=ii+2;
	} 
	painter.end(); 
} 

void CDraw3dPieChart::closeEvent( QCloseEvent *event )
{
	emit closewindow();
}

