/****************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of Nokia Corporation and its Subsidiary(-ies) nor
**     the names of its contributors may be used to endorse or promote
**     products derived from this software without specific prior written
**     permission.
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
** $QT_END_LICENSE$
**
****************************************************************************/

#include <QtGui>
#include "helper.h"
#include <math.h>


extern "C"{
	#include "utilmeca.h"
};
//! [0]
Helper::Helper()
{
  //  QLinearGradient gradient(QPointF(50, -20), QPointF(80, 20));
  //  gradient.setColorAt(0.0, Qt::white);
 //   gradient.setColorAt(1.0, QColor(0xa6, 0xce, 0x39));
	m_R = 100;
}

Helper::~Helper()
{

}

bool Helper::IsInPolygon(QPointF point,const QVector<QPointF>& points)
{
	int nCross = 0;
	int nCount = points.size();

	for (int i = 0; i < points.size(); i++)
	{

		QPointF p1 = points[i];
		QPointF p2 = points[(i + 1) % nCount];

		// 求解 y=p.y 与 p1p2 的交点
		if ( p1.y() == p2.y() ) // p1p2 与 y=p0.y平行
			continue;

		if ( point.y() < min(p1.y(), p2.y()) ) // 交点在p1p2延长线上
			continue;

		if ( point.y() >= max(p1.y(), p2.y()) ) // 交点在p1p2延长线上
			continue;

		// 求交点的 X 坐标 --------------------------------------------------------------
		double x = (double)(point.y() - p1.y()) * (double)(p2.x() - p1.x()) / (double)(p2.y() - p1.y()) + p1.x();

		if ( x > point.x() )
			nCross++; // 只统计单边交点
	}

	// 单边交点为偶数，点在多边形之外 ---
	return (nCross % 2 == 1);
}

void Helper::ConvertToImage(QVector<QPointF>& points,bool Fill)
{
	static int count = 0;
	//默认为多边形
	 
	QPointF pCenter;
	int xmin,xmax,ymin,ymax;
	xmin = xmax = points.at(0).x();
	ymin = ymax = points.at(0).y();
	for (int i=1;i<points.size();i++)
	{
		if(xmin>points.at(i).x()) xmin = points.at(i).x();
		if(xmax<points.at(i).x()) xmax = points.at(i).x();
		if(ymin>points.at(i).y()) ymin = points.at(i).y();
		if(ymax<points.at(i).y()) ymax = points.at(i).y();
	}

	if (Fill && m_imglist1.size()<2)
	{
		QImage img = QImage(m_R*2,m_R*2,QImage::Format_ARGB32);
		img.fill(qRgba(255, 255, 255,0));

		for (int i=0;i<m_R*2;i++)
		{
			for (int j=0;j<m_R*2;j++)
			{
				if(i>xmin && i<xmax && j>ymin && j<ymax && IsInPolygon(QPointF(i,j),points))
				{
					img.setPixel(i,j,qRgba(255,0,0,255));
				}
			}
		}
		
	//	img.save(QString::number(count++) + ".png");
		m_imglist1.push_back(img);	
		
	}
	
	if(!Fill && m_imglist2.size()<3)
	{
		QImage img = QImage(m_R*2,m_R*2,QImage::Format_ARGB32);
		img.fill(qRgba(255, 255, 255,0));

		for (int i=0;i<points.size();i++)
		{
			img.setPixel(QPoint(points.at(i).x(),points.at(i).y()),qRgb(0,0,0));
			img.setPixel(QPoint(points.at(i).x(),points.at(i).y()-1),qRgb(0,0,0));
			img.setPixel(QPoint(points.at(i).x(),points.at(i).y()+1),qRgb(0,0,0));
			img.setPixel(QPoint(points.at(i).x()-1,points.at(i).y()),qRgb(0,0,0));
			img.setPixel(QPoint(points.at(i).x()+1,points.at(i).y()),qRgb(0,0,0));
		}
		
	//	img.save(QString::number(count++) + ".png");
		m_imglist2.push_back(img);				
	}		
}

void Helper::MergeImage()
{
	QImage img = QImage(m_R*2,m_R*2,QImage::Format_ARGB32);
	img.fill(qRgba(255, 255, 255,0));

	QImage imgTemp;
	//merge unfilling image
	for (int i=0;i<m_imglist2.size();i++)
	{
		imgTemp = m_imglist2.at(i);
		for (int iWidth=0;iWidth<imgTemp.width();iWidth++)
		{
			for (int iHeight=0;iHeight<imgTemp.height();iHeight++)
			{
				if (imgTemp.pixel(iWidth,iHeight)!=qRgba(255, 255, 255,0) && img.pixel(iWidth,iHeight)==qRgba(255, 255, 255,0))
				{
					img.setPixel(iWidth,iHeight,imgTemp.pixel(iWidth,iHeight));
				}
			}
		}
	}
	//merge filling image
	for (int i=0;i<m_imglist1.size();i++)
	{
		imgTemp = m_imglist1.at(i);
		for (int iWidth=0;iWidth<imgTemp.width();iWidth++)
		{
			for (int iHeight=0;iHeight<imgTemp.height();iHeight++)
			{
				if (imgTemp.pixel(iWidth,iHeight)!=qRgba(255, 255, 255,0) && img.pixel(iWidth,iHeight)==qRgba(255, 255, 255,0))
				{
					img.setPixel(iWidth,iHeight,imgTemp.pixel(iWidth,iHeight));
				}
			}
		}
	}
	m_img = img;
//	m_img.save("merge.png");
}

QImage Helper::DrawMechanism(double str1,double dip1,double rake1,double str2,double dip2,double rake2,double mw)
{
	MECHANISM ms;
	ms.NP1.str = str1;
	ms.NP1.dip = dip1;
	ms.NP1.rake = rake1;
	ms.NP2.str = str2;
	ms.NP2.dip = dip2;
	ms.NP2.rake = rake2;
	ms.magms = mw;
	return DrawMechanism(ms);
}

QImage Helper::DrawMechanism(st_me meca)
{
	m_imglist1.clear();
	m_imglist2.clear();

	double x0;
	double y0;
	x0 = y0 = 100;
	int size = 200;
	m_R = size/2;

	double x[1000], y[1000];
	double pos_NP1_NP2 = sind(meca.NP1.str - meca.NP2.str);
	double fault = (GMT_IS_ZERO (meca.NP1.rake) ? meca.NP2.rake / fabs(meca.NP2.rake) : meca.NP1.rake / fabs(meca.NP1.rake));
	double radius_size;
	double str, radius, increment;
	double si, co;

	GMT_LONG lineout = 1, i;
	GMT_LONG npoints;

	struct AXIS N_axis;

/* compute null axis strike and dip */
	N_axis.dip = null_axis_dip(meca.NP1.str, meca.NP1.dip, meca.NP2.str, meca.NP2.dip);
	if (fabs(90. - N_axis.dip) < EPSIL)
		N_axis.str = meca.NP1.str;
	else
		N_axis.str = null_axis_strike(meca.NP1.str, meca.NP1.dip, meca.NP2.str, meca.NP2.dip);

/* compute radius size of the bubble */
	radius_size = size * 0.5;

/*  argument is DIAMETER!!*/
//	ps_circle(x0, y0, radius_size*2., ergb, lineout);
 
	QVector<QPointF> points;
	float posx,posy;
	for (int i=0;i<360;i++)
	{
		posx = x0 + m_R*cosf(D2R*i);
		posy = y0 + m_R*sinf(D2R*i);
	//	points.push_back(QPointF(posx-1,posy));
		points.push_back(QPointF(posx,posy));
	//	points.push_back(QPointF(posx+1,posy));
	//	points.push_back(QPointF(posx,posy-1));
	//	points.push_back(QPointF(posx,posy+1));
	}
	ConvertToImage(points,false);

	if (fabs(pos_NP1_NP2) < EPSIL) {
/* pure normal or inverse fault (null axis strike is determined
   with + or - 180 degrees. */
		/* first nodal plane part */
		 QVector<QPointF> points;
		i = -1;
		increment = 1.;
		str = meca.NP1.str;
		while (str <= meca.NP1.str + 180. + EPSIL) {
			i++;
			radius = proj_radius(meca.NP1.str, meca.NP1.dip, str) * radius_size;
			sincosd (str, &si, &co);
			x[i] = x0 + radius * si;
			y[i] = y0 + radius * co;
			points.push_back(QPointF(x[i],y[i]));
			str += increment;
		}
		if (fault < 0.) {
			/* normal fault, close first compressing part */
			str = meca.NP1.str + 180.;
			while (str >= meca.NP1.str - EPSIL) {
				i++;
				sincosd (str, &si, &co);
				x[i] = x0 + si * radius_size;
				y[i] = y0 + co * radius_size;
				points.push_back(QPointF(x[i],y[i]));
				str -= increment;
			}
			npoints = i + 1;
		//	ps_polygon (x, y, npoints, rgb, outline);
		//	painter->drawPolygon(points);
			ConvertToImage(points,true);
			ConvertToImage(points,false);
			i = -1;
		}
		/* second nodal plane part */
		str = meca.NP2.str;
		while (str <= meca.NP2.str + 180. + EPSIL) {
			i++;
			radius = proj_radius(meca.NP2.str, meca.NP2.dip, str) * radius_size;
			sincosd (str, &si, &co);
			x[i] = x0 + radius * si;
			y[i] = y0 + radius * co;
			points.push_back(QPointF(x[i],y[i]));
			str += increment;
		}
		if (fault > 0.) {
			/* inverse fault, close compressing part */
			npoints = i+1;
	//		ps_polygon(x, y, npoints, rgb, outline);
		//	painter->drawPolygon(points);
			ConvertToImage(points,true);
			ConvertToImage(points,false);
		}
		else {
			/* normal fault, close second compressing part */
			str = meca.NP2.str + 180.;
			while (str >= meca.NP2.str - EPSIL) {
				i++;
				sincosd (str, &si, &co);
				x[i] = x0 + si * radius_size;
				y[i] = y0 + co * radius_size;
				points.push_back(QPointF(x[i],y[i]));
				str -= increment;
			}
			npoints = i + 1;
		//	painter->drawPolygon(points);
			ConvertToImage(points,true);
			ConvertToImage(points,false);
	//		ps_polygon(x, y, npoints, rgb, outline);
		}
	}
/* pure strike-slip */
	else if ((90. - meca.NP1.dip) < EPSIL && (90. - meca.NP2.dip) < EPSIL) {
		increment = fabs(meca.NP1.rake) < EPSIL ? 1. : -1.;
		/* first compressing part */
		 QVector<QPointF> points;
		i = 0;
		str = meca.NP1.str;
		while (increment > 0. ? str <= meca.NP1.str + 90. : str >= meca.NP1.str - 90.) {
			sincosd (str, &si, &co);
			x[i] = x0 + si * radius_size;
			y[i] = y0 + co * radius_size;
			points.push_back(QPointF(x[i],y[i]));
			str += increment;
			i++;
		}
		x[i] = x0;
		y[i] = y0;
		points.replace(i,QPointF(x0,y0));
		npoints = i + 1;
	//	ps_polygon(x, y, npoints, rgb, outline);
	//	painter->drawPolygon(points);
		ConvertToImage(points,true);
		ConvertToImage(points,false);

		/* second compressing part */		
		points.clear();
		i = 0;
		str = meca.NP1.str + 180.;
		while (increment > 0. ?  str <= meca.NP1.str + 270. + EPSIL : str >= meca.NP1.str + 90. - EPSIL) {
			sincosd (str, &si, &co);
			x[i] = x0 + si * radius_size;
			y[i] = y0 + co * radius_size;
			points.push_back(QPointF(x[i],y[i]));
			str += increment;
			i++;
		}
		x[i] = x0;
		y[i] = y0;
		points.replace(i,QPointF(x0,y0));
		npoints = i + 1;
	//	ps_polygon(x, y, npoints, rgb, outline);
	//	painter->drawPolygon(points);
		ConvertToImage(points,true);
		ConvertToImage(points,false);
	}
	else {
/* other cases */
		/* first nodal plane till null axis */
		QVector<QPointF> points;
		i = -1;
		increment = 1.;
		if (meca.NP1.str > N_axis.str)
			meca.NP1.str -= 360.;
		str = meca.NP1.str;
		while (fabs(90. - meca.NP1.dip) < EPSIL ? str <= meca.NP1.str + EPSIL : str <= N_axis.str + EPSIL) {
			i++;
			radius = proj_radius(meca.NP1.str, meca.NP1.dip, str) * radius_size;
			sincosd (str, &si, &co);
			x[i] = x0 + radius * si;
			y[i] = y0 + radius * co;
			points.push_back(QPointF(x[i],y[i]));
			str += increment;
		}

		/* second nodal plane from null axis */
		meca.NP2.str += (1. + fault) * 90.;
		if (meca.NP2.str >= 360.) meca.NP2.str -= 360.;
		increment = fault;
		if (fault * (meca.NP2.str - N_axis.str) < -EPSIL) meca.NP2.str += fault * 360.;
		str = fabs(90. - meca.NP2.dip) < EPSIL ? meca.NP2.str : N_axis.str;
		while (increment > 0. ? str <= meca.NP2.str + EPSIL : str >= meca.NP2.str - EPSIL) {
			i++;
			radius = proj_radius(meca.NP2.str - (1. + fault) * 90., meca.NP2.dip, str) * radius_size;
			sincosd (str, &si, &co);
			x[i] = x0 + radius * si;
			y[i] = y0 + radius * co;
			points.push_back(QPointF(x[i],y[i]));
			str += increment;
		}

		/* close the first compressing part */
		meca.NP1.str = zero_360(meca.NP1.str);
		meca.NP2.str = zero_360(meca.NP2.str);
		increment = pos_NP1_NP2 >= 0. ? -fault : fault;
		if (increment * (meca.NP1.str - meca.NP2.str) < - EPSIL) meca.NP1.str += increment * 360.;
		str = meca.NP2.str;
		while (increment > 0. ? str <= meca.NP1.str + EPSIL : str >= meca.NP1.str - EPSIL) {
			i++;
			sincosd (str, &si, &co);
			x[i] = x0 + si * radius_size;
			y[i] = y0 + co * radius_size;
			points.push_back(QPointF(x[i],y[i]));
			str += increment;
		}

		npoints = i + 1;
	//	ps_polygon(x, y, npoints, rgb, outline);
	//	painter->drawPolygon(points);
		ConvertToImage(points,true);
		ConvertToImage(points,false);

		/* first nodal plane till null axis */
		points.clear();
		i = -1;
		meca.NP1.str = zero_360(meca.NP1.str + 180.);
		if (meca.NP1.str - N_axis.str < - EPSIL) meca.NP1.str += 360.;
		increment = -1.;
		str = meca.NP1.str;
		while (fabs(90. - meca.NP1.dip) < EPSIL ? str >= meca.NP1.str -EPSIL : str >= N_axis.str - EPSIL) {
			i++;
			radius = proj_radius(meca.NP1.str - 180., meca.NP1.dip, str) * radius_size;
			sincosd (str, &si, &co);
			x[i] = x0 + radius * si;
			y[i] = y0 + radius * co;
			points.push_back(QPointF(x[i],y[i]));
			str += increment;
		}

		/* second nodal plane from null axis */
		meca.NP2.str = zero_360(meca.NP2.str + 180.);
		increment = -fault;
		if (fault * (N_axis.str - meca.NP2.str) < - EPSIL) meca.NP2.str -= fault * 360.;
		str = fabs(90. - meca.NP2.dip) < EPSIL ? meca.NP2.str : N_axis.str;
		while (increment > 0. ? str <= meca.NP2.str + EPSIL : str >= meca.NP2.str - EPSIL) {
			i++;
			radius = proj_radius(meca.NP2.str - (1. - fault) * 90., meca.NP2.dip, str) * radius_size;
			sincosd (str, &si, &co);
			x[i] = x0 + radius * si;
			y[i] = y0 + radius * co;
			points.push_back(QPointF(x[i],y[i]));
			str += increment;
		}

		/* close the second compressing part */
		meca.NP1.str = zero_360(meca.NP1.str);
		meca.NP2.str = zero_360(meca.NP2.str);
		increment = pos_NP1_NP2 >= 0. ? -fault : fault;
		if (increment * (meca.NP1.str - meca.NP2.str) < - EPSIL) meca.NP1.str += increment * 360.;
		str = meca.NP2.str;
		while (increment > 0. ? str <= meca.NP1.str + EPSIL : str >= meca.NP1.str - EPSIL) {
			i++;
			sincosd (str, &si, &co);
			x[i] = x0 + si * radius_size;
			y[i] = y0 + co * radius_size;
			points.push_back(QPointF(x[i],y[i]));
			str += increment;
		}

		npoints = i + 1;
	//	ps_polygon(x, y, npoints, rgb, outline);
	//	painter->drawPolygon(points);
		ConvertToImage(points,true);
		ConvertToImage(points,false);
	}

	MergeImage();
	return m_img;
}

