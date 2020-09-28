#include "PlotDialog.h"

PlotDialog::PlotDialog(QWidget *parent): QDialog(parent)
{
	ui.setupUi(this);
	InitDialog();
	InitSlots();
}

PlotDialog::~PlotDialog()
{

}

void PlotDialog::InitDialog()
{

}

void PlotDialog::InitSlots()
{

}

void PlotDialog::Slots_PushButtonClicked()
{

}

void PlotDialog::paintEvent( QPaintEvent *event )
{
	QPainter painter(this);
	painter.setWindow(0,0,(w+120),(h+50));
	painter.setBackground(Qt::white);
	painter.setFont(QFont(tr("宋体"),6));
	painter.drawText(1,10,QString("高度 KM  最大值=%1dBZ  高度=%2KM  (方位角/距离)=%3/%4").arg(pmax).arg(pmaxH).arg(pmaxAz).arg(pmaxR));
	painter.drawLine(1,15,w,15);
	painter.drawText(10,h+15,QString("KM"));
	painter.drawText(10,h+35,QString("(方位角/距离)   (%1/%2)   (%3/%4)   (%5/%6)").arg(AzList.at(0)).arg(RList.at(0)).arg(AzList.at(2)).arg(RList.at(2)).arg(AzList.at(1)).arg(RList.at(1)));
	painter.setFont(QFont(tr("宋体"),5));
	//y
	painter.drawLine(QLine(30,1,30,h+15));
	int lux = h/10;
	int dx = h;
	for (int i=0;i<9;i++)
	{
		dx-=lux;
		painter.drawLine(QLine(30,1+dx,w-2,1+dx));
		painter.drawText(10,1+dx,QString("%1").arg((h-dx)/20));
	}
	//画X轴
	painter.drawLine(QLine(1,h-2,w+4,h-2));
	int luy = w/5;
	int dy = 0;
	for (int i=0;i<4;i++)
	{
		dy+=luy;
		painter.drawLine(QLine(1+dy+25,h,1+dy+25,h-3));
		painter.drawText(1+dy+25,h+15,QString("%1").arg(dy));
	}
	dy+=luy;
	painter.drawLine(QLine(1+dy,h,1+dy,15));
	for (int ii = 0; ii< h ;ii++)
	{
		QPointF p1(25,ii);
		QPointF p2(25,ii);
		QPointF p3(25,ii);
		QPointF p4(25,ii);
		//改变X的值，把这四个的X全设置成一样的

		int r1 = 1;
		QPointF ps[4];

		for (int jj = 0 ;jj < w ; jj++)
		{
			r1 = jj ;
			p1=p2;
			p4=p3;

			p2.setX(r1);
			p2.setY(ii);
			p3.setX(r1);
			p3.setY(ii+1);

			ps[0].setX((int)p1.x());
			ps[0].setY((int)p1.y());
			ps[1].setX((int)p2.x());
			ps[1].setY((int)p2.y());
			ps[2].setX((int)p3.x());
			ps[2].setY((int)p3.y());
			ps[3].setX((int)p4.x());
			ps[3].setY((int)p4.y());


			if(buf[jj][ii] != -32768 && buf[jj][ii] != -32767 )
			{
				if (buf[jj][ii] == -32766)
				{
					painter.setPen(Qt::black);
					painter.setBrush(Qt::black);
					painter.drawPolygon(ps,4);
				}
				else
				{
					int t = buf[jj][ii];
					for (int num = 0;num<m_pTableColor->size();num++)
					{
						if (t<=m_pTableColor->at(num)->fEnd&&t>=m_pTableColor->at(num)->fStart)
						{
							painter.setPen(QColor(m_pTableColor->at(num)->nRed,m_pTableColor->at(num)->nGreen,m_pTableColor->at(num)->nBlue));							
							painter.setBrush(QColor(m_pTableColor->at(num)->nRed,m_pTableColor->at(num)->nGreen,m_pTableColor->at(num)->nBlue));
							painter.drawPolygon(ps,4);
							break;
						}
					}
				}
			}
		}
	}
	//右边的属性及图例
	painter.setPen(Qt::black);
	painter.setFont(QFont(tr("仿宋"),7));
	QString tempname;
	if (InfoList.at(0).contains(tr("基本反射率(R)"))|| InfoList.at(0).contains(tr("经质量控制的基本反射率(QR)")))
	{
		tempname = "强度垂直剖面(RCS)";
	}
	else if (InfoList.at(0).contains(("基本速度(V)"))|| InfoList.at(0).contains(tr("经质量控制的基本速度(QV)")))
	{
		tempname = "速度垂直剖面(VCS)";
	}
	else if (InfoList.at(0).contains(("基本谱宽(W)"))|| InfoList.at(0).contains(tr("经质量控制的基本谱宽(QW)")))
	{
		tempname = "谱宽垂直剖面(WCS)";
	}
	painter.drawText(w+10,10,tr("产品名称："));
	painter.drawText(w+10,20,tempname);
	int n=3;
	for (int i = 1;i<14;i++)
	{
		if (InfoList.at(i) == " ")
		{
			continue;
		}
		if (InfoList.at(i).size()<13)
		{
			painter.drawText(w+10,(n++)*10,InfoList.at(i));
		}
		else
		{
			tempname = InfoList.at(i).split("：").at(0);
			tempname = tempname.append("：");
			painter.drawText(w+10,(n++)*10,tempname);
			tempname = InfoList.at(i).split("：").at(1);
			painter.drawText(w+10,(n++)*10,tempname);
		}

	}
	n++;
	painter.drawText(w+10,(n++)*10,QString("起点:%1deg %2km").arg(AzList.at(0)).arg(RList.at(0)));
	painter.drawText(w+10,(n++)*10,QString("终点:%1deg %2km").arg(AzList.at(1)).arg(RList.at(1)));

	painter.setPen(QColor(145,145,145));
	for (int i = 0;i<m_pTableColor->size();i++)
	{
		float tempf = m_pTableColor->at(i)->fStart; 
		//if((filename.contains(tr("OSR"))||filename.contains(tr("WSR"))||filename.contains(tr("TSR")))&&tempf<40) continue;
		painter.setBrush(QColor(m_pTableColor->at(i)->nRed,m_pTableColor->at(i)->nGreen,m_pTableColor->at(i)->nBlue));
		painter.drawRect(w+10,(n++)*10,20,10);
		painter.drawLine(w+30,(n-1)*10,w+35,(n-1)*10);
		painter.drawText(w+30,(n)*10,QString(" %1 ").arg(QString::number(tempf,'f',1)));
	}
	if ((InfoList.at(0).contains("基本速度(V)")||InfoList.at(0).contains("经质量控制的基本速度(QV)")||InfoList.at(0).contains("基本谱宽(W)")||InfoList.at(0).contains("经质量控制的基本谱宽(QW)")))
	{
		painter.drawRect(w+60,(n-m_pTableColor->size())*10,20,10);
		painter.drawLine(w+80,(n-m_pTableColor->size())*10,w+85,(n-m_pTableColor->size())*10);
		painter.setFont(QFont(tr("宋体"),5));
		painter.drawText(w+85,(n-m_pTableColor->size()+1)*10,tr("距离折叠"));
	}
}

void PlotDialog::GetArray(QString proname, int ds,int dh,int** buffer,int m_max,double m_maxH,int m_maxAz,int m_maxR,P_COLORTABLE_VECTOR pTableColor,QStringList m_AzList,QStringList m_RList,QStringList m_InfoList )
{
	w = ds,h = dh,pmax = m_max,pmaxAz = m_maxAz,pmaxR = m_maxR;
	InfoList = m_InfoList;
	buf= new int*[ds];
	pmaxH = m_maxH/1.00;
	for (int k =0 ;k <ds ;k++)
	{
		buf[k] = new int[dh];
	}
	buf = buffer;
	m_pTableColor = pTableColor;
	m_proname = proname;
	AzList = m_AzList;
	RList = m_RList;
}

void PlotDialog::GetImage( QImage* pImage )
{
	QPainter painter(pImage);
	
	for (int ii = 0; ii< h ;ii++)
	{
		QPointF p1(25,ii);
		QPointF p2(25,ii);
		QPointF p3(25,ii);
		QPointF p4(25,ii);
		//改变X的值，把这四个的X全设置成一样的

		int r1 = 1;
		QPointF ps[4];

		for (int jj = 0 ;jj < w ; jj++)
		{
			r1 = jj ;
			p1=p2;
			p4=p3;

			p2.setX(r1);
			p2.setY(ii);
			p3.setX(r1);
			p3.setY(ii+1);

			ps[0].setX((int)p1.x());
			ps[0].setY((int)p1.y());
			ps[1].setX((int)p2.x());
			ps[1].setY((int)p2.y());
			ps[2].setX((int)p3.x());
			ps[2].setY((int)p3.y());
			ps[3].setX((int)p4.x());
			ps[3].setY((int)p4.y());


			if(buf[jj][ii] != -32768 && buf[jj][ii] != -32767 )
			{
				if (buf[jj][ii] == -32766)
				{
					painter.setPen(Qt::black);
					painter.setBrush(Qt::black);
					painter.drawPolygon(ps,4);
				}
				else
				{
					int t = buf[jj][ii];
					for (int num = 0;num<m_pTableColor->size();num++)
					{
						if (t<=m_pTableColor->at(num)->fEnd&&t>=m_pTableColor->at(num)->fStart)
						{
							painter.setPen(QColor(m_pTableColor->at(num)->nRed,m_pTableColor->at(num)->nGreen,m_pTableColor->at(num)->nBlue));							
							painter.setBrush(QColor(m_pTableColor->at(num)->nRed,m_pTableColor->at(num)->nGreen,m_pTableColor->at(num)->nBlue));
							painter.drawPolygon(ps,4);
							break;
						}
					}
				}
			}
		}
	}
}
