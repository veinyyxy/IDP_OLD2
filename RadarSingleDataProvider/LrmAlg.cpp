#include <QObject>
#include <QMessageBox>
#include "LrmAlg.h"

LrmAlg::LrmAlg(void)
{
	m_LayerPixmap = 0;
	m_Output_Poitn = new osg::Vec4Array;
}


LrmAlg::~LrmAlg(void)
{
}

void LrmAlg::Initialize( cDrawProduct* pData, P_COLORTABLE_VECTOR pColorTable)
{
	m_produceDraw = pData;
	
	m_Point = new osg::Vec3Array;

	m_color = pColorTable;
	//Execute();
}

void LrmAlg::Execute()
{
	if(!m_LayerPixmap) return;

	m_LayerPixmap->clear();
	num_of_req_H = m_HighOfLay.size();
	for(int i = 0;i<num_of_req_H;i++)
	{
		int iTestValue = m_HighOfLay.at(i).toInt();
		LayerHight.append(m_HighOfLay.at(i).toInt());
	}
	//point已经有了，是以实际坐标的形式存在
	int m_eleCnt = m_produceDraw->GetCountOfLayer();//仰角层数
	gInt16 maxRefgatenum = 0;
	double scanR = 0;
	osg::ref_ptr<osg::Vec4Array> Output_Poitn = new osg::Vec4Array;
	int minElev = 9000,Elevation,NyquistVelocity,Azi_Num,Gate_Num,Num;
	for(int i = 0;i<m_eleCnt;i++)
	{
		m_produceDraw->ReadInformaton(i,Elevation,NyquistVelocity,Azi_Num,Gate_Num,Num);//获取仰角，最大不模糊速度，与方位角数与距离库数,退模糊次数
		if (maxRefgatenum < Gate_Num)
		{
			maxRefgatenum = Gate_Num;
		}
		if (Elevation<minElev)
		{
			minElev = Elevation;
		}
	}
	scanR = maxRefgatenum;
	//有一个点不在范围内就退出
	if (Get_Length(m_Point->at(2).x(),m_Point->at(2).y())>scanR)
	{
		return;
	}
	if (Get_Length(m_Point->at(3).x(),m_Point->at(3).y())>scanR)
	{
		return;
	}
	if (Get_Length(m_Point->at(2).x(),m_Point->at(3).y())>scanR)
	{
		return;
	}
	if (Get_Length(m_Point->at(3).x(),m_Point->at(2).y())>scanR)
	{
		return;
	}
	double dRatio = abs(Get_Length(m_Point->at(2).x()-m_Point->at(3).x(),m_Point->at(2).y()-m_Point->at(3).y()))/abs(Get_Length(m_Point->at(0).x()-m_Point->at(1).x(),m_Point->at(0).y()-m_Point->at(1).y()));

	QPointF pLU,pRD;//实际左上，实际右下
	pLU.setX(m_Point->at(0).x());
	pLU.setY(m_Point->at(0).y());
	pRD.setX(m_Point->at(1).x());
	pRD.setY(m_Point->at(1).y());
	float x1 = pLU.x();
	float y1 = pLU.y();
	float x2 = pRD.x();
	float y2 = pRD.y();
	Swap(x1,x2);
	Swap(y1,y2);
	x_normal = x1;
	y_normal = y2;
	pLU.setX(m_Point->at(2).x());
	pLU.setY(m_Point->at(2).y());
	pRD.setX(m_Point->at(3).x());
	pRD.setY(m_Point->at(3).y());
	float PointSmallX = pLU.x();
	float PointSmallY = pLU.y();
	float PointBigX = pRD.x();
	float PointBigY = pRD.y();
	Swap(PointSmallX,PointBigX);
	Swap(PointSmallY,PointBigY);
	x_long = abs(m_Point->at(0).x()-m_Point->at(1).x());
	y_long = abs(m_Point->at(0).y()-m_Point->at(1).y());
	int num_of_gate		= 0;
	int elevation = 0;
	int bmpHei = 200;
	double    *m_ration = new double[m_eleCnt];
	float projection = cos(minElev*0.01/180.0*M_PI);
	for (int i = 0;i<num_of_req_H;i++)//请求的高度
	{
		vector<vec_int> paramArray;
		int layhigh = LayerHight.at(i);
		for (int j = 0;j<m_eleCnt;j++)//层数
		{
			m_produceDraw->ReadInfo(paramArray, m_azimuthNum, num_of_gate, elevation,m_radarHeight,j,m_len_gate,bmpHei,dRatio,m_ration);//数据，径向数，库数，本层仰角，雷达站高度，层号，距离库长度，剩下的没用
			for (int gate = 0;gate<=num_of_gate;gate++)//距离库
			{
				float lh = gate*m_len_gate*sin(elevation*0.01/180.0*M_PI)+m_radarHeight*0.01;
				if (lh == layhigh)//高度正好相等的时候
				{
					float m_R = gate*m_len_gate*cos(elevation*0.01/180.0*M_PI);
					m_R = m_R/1000;
					for (int k = 0;k<m_azimuthNum;k++)//方位角
					{
						int m_X = m_R*cos(k*1.0/180.0*M_PI);
						int m_Y = m_R*sin(k*1.0/180.0*M_PI);
						if (m_X>=PointSmallX*projection && m_X<=PointBigX*projection && m_Y>=PointSmallY*projection && m_Y<=PointBigY*projection)
						{
							x_coor = m_X/dRatio;
							y_coor = m_Y/dRatio;
							transcoor(x_coor,y_coor);
							osg::Vec4 temp;
							vec_int it = paramArray.at(k);
							int res_color = it.at(gate);
							if (res_color != -32767)
							{
								temp.set(x_coor, y_coor, i, res_color);
								Output_Poitn->push_back(temp);
							}
						}
					}
				}
				else if (lh<layhigh && gate+1<=num_of_gate && (gate+1)*m_len_gate*sin(elevation*0.01/180.0*M_PI)+m_radarHeight*0.01>layhigh)//两个点跨着请求高度的时候
				{
					float m_R = ((gate*m_len_gate*cos(elevation*0.01/180.0*M_PI))+((gate+1)*m_len_gate*cos(elevation*0.01/180.0*M_PI)))/2;
					m_R = m_R/1000;
					for (int k = 0;k<m_azimuthNum;k++)//方位角
					{
						float m_X = m_R*cos(k*1.0/180.0*M_PI);
						float m_Y = m_R*sin(k*1.0/180.0*M_PI);
						if (m_X>=PointSmallX*projection && m_X<=PointBigX*projection && m_Y>=PointSmallY*projection && m_Y<=PointBigY*projection)
						{
							x_coor = m_X/dRatio;
							y_coor = m_Y/dRatio;
							transcoor(x_coor,y_coor);
							osg::Vec4 temp;
							vec_int it = paramArray.at(k);
							if (it.at(gate) != -32767 && it.at(gate+1 != -32767))
							{
								int res_color = int((it.at(gate)+it.at(gate+1))/2);
								temp.set(x_coor, y_coor, i, res_color);
								//temp.setZ(i);temp.setY(y_coor);temp.setX(x_coor);temp.setW(res_color);
								Output_Poitn->push_back(temp);
							}
						}
					}
				}
			}
			paramArray.clear();
		}
	}
	vec_int t_vec;
	if (Output_Poitn->size() == 0)
	{
		QMessageBox::warning(NULL,"提示","没有符合条件的数据", QMessageBox::Ok,QMessageBox::Ok);
		return;
	}
	for (int z = 0;z<num_of_req_H;z++)//补全无效值
	{
		for (int y = 0+y_normal;y>y_normal-y_long;y--)
		{
			for (int x = 0+x_normal;x<x_normal+x_long;x++)
			{
				for (int i = 0;i<Output_Poitn->size();i++)//遍历所有数据，有就写上，没有就写个无效值
				{
					if (Output_Poitn->at(i).x() == x && Output_Poitn->at(i).y() == y && Output_Poitn->at(i).z() == z /*&& Output_Poitn.at(i).w() != -32767*/)
					{
						t_vec.push_back(Output_Poitn->at(i).w());
						//break;
					}
					else if (i == Output_Poitn->size()-1 && t_vec.size() == 0)
					{
						//t_vec.push_back(-32767);
// 						QVector4D temp;
// 						temp.setZ(z);temp.setY(y);temp.setX(x);temp.setW(-32767);
// 						Output_Poitn.push_back(temp);
						t_vec.push_back(-32767);
					}
				}
				int tatal = 0,nn = 0;
				for (int vs = 0;vs<t_vec.size();vs++)
				{
					if (t_vec.at(vs) != -32767 && t_vec.size()>1)
					{
						tatal = tatal+t_vec.at(vs);
						nn++;
					}
					else
					{
						tatal = -32767;
						nn = 1;
					}
				}
				tatal = tatal/nn;
				osg::Vec4 temp;
				temp.set(x, y, z, tatal);
				//temp.setZ(z);temp.setY(y);temp.setX(x);temp.setW(tatal);
				m_Output_Poitn->push_back(temp);
				t_vec.clear();
			}
		}
	}
	Output_Poitn->clear();
	*Output_Poitn = *m_Output_Poitn;
	m_Output_Poitn->clear();
	t_vec.clear();
	for (int z = 0;z<num_of_req_H;z++)//按X方向插值
	{
		for (int y = 0+y_normal;y>y_normal-y_long;y--)
		{
			for (int x = 0+x_normal;x<x_normal+x_long;x++)
			{
				
				for (int i = 0;i<Output_Poitn->size();i++)
				{
					if (Output_Poitn->at(i).x() == x && Output_Poitn->at(i).y() == y && Output_Poitn->at(i).z() == z)
					{
						t_vec.push_back(Output_Poitn->at(i).w());
						break;
					}
				}
			}
			int i1 = x_normal,ss = 0;
			if (t_vec.size() == 0)
			{
				return;
			}
			for(vec_int::iterator colit = t_vec.begin();colit != t_vec.end();colit++,i1++,ss++)
			{
				osg::Vec4 temp;

				temp.set(i1, y, z, 0);
				//temp.setZ(z);temp.setY(y);temp.setX(i1);
				if (*colit != -32767)
				{
					temp.set(i1, y, z, *colit);
					//temp.setW(*colit);
					m_Output_Poitn->push_back(temp);
				}
				else
				{
					if(ss-1 >=0 && t_vec.at(ss-1) != -32767)//前一个不是无效值
					{
						for (int ii = ss;ii<x_long;ii++)//往后倒，按距离权重插值
						{
							if(t_vec.at(ii) != -32767)
							{
								int tl = ii-ss+1;
								int c = t_vec.at(ss-1)/tl*(tl-1)+t_vec.at(ii)/tl;
								//temp.setW(c);
								temp.set(i1, y, z, c);
								m_Output_Poitn->push_back(temp);
								t_vec.at(ss) = c;
								break;
							}
							else if (ii == x_long-1)
							{
								//temp.setW(-32767);
								temp.set(i1, y, z, -32767);
								m_Output_Poitn->push_back(temp);
								break;
							}
						}
					}
					else
					{
						//temp.setW(-32767);
						temp.set(i1, y, z, -32767);
						m_Output_Poitn->push_back(temp);
					}
				}
			}
			t_vec.clear();
		}
	}

	Output_Poitn->clear();
	*Output_Poitn = *m_Output_Poitn;
	m_Output_Poitn->clear();
	t_vec.clear();
	for (int z = 0;z<num_of_req_H;z++)//按Y方向插值
	{
		for (int x = 0+x_normal;x<x_long+x_normal;x++)
		{
			for (int y = 0+y_normal;y>y_normal-y_long;y--)
			{
				for (int i = 0;i<Output_Poitn->size();i++)
				{
					if (Output_Poitn->at(i).x() == x && Output_Poitn->at(i).y() == y && Output_Poitn->at(i).z() == z)
					{
						t_vec.push_back(Output_Poitn->at(i).w());
						break;
					}
				}
			}
			int i1 = y_normal,ss = 0;
			if (t_vec.size() == 0)
			{
				return;
			}
			for(vec_int::iterator colit = t_vec.begin();colit != t_vec.end();colit++,i1++,ss++)
			{
				osg::Vec4 temp;
				//temp.setZ(z);temp.setY(i1);temp.setX(x);
				temp.set(x, i1, z, 0);
				if (*colit != -32767)
				{
					//temp.setW(*colit);
					temp.set(x, i1, z, *colit);
					m_Output_Poitn->push_back(temp);
				}
				else
				{
					if(ss-1 >=0 && t_vec.at(ss-1) != -32767)//前一个不是无效值
					{
						for (int ii = ss;ii<x_long;ii++)//往后倒，按距离权重插值
						{
							if(t_vec.at(ii) != -32767)
							{
								int tl = ii-ss+1;
								int c = t_vec.at(ss-1)/tl*(tl-1)+t_vec.at(ii)/tl;
								//temp.setW(c);
								temp.set(x, i1, z, c);
								m_Output_Poitn->push_back(temp);
								t_vec.at(ss) = c;
								break;
							}
							else if (ii == y_long-1)
							{
								//temp.setW();
								temp.set(x, i1, z, -32767);
								m_Output_Poitn->push_back(temp);
								break;
							}
						}
					}
					else
					{
						//temp.setW(-32767);
						temp.set(x, i1, z, -32767);
						m_Output_Poitn->push_back(temp);
					}
				}
			}
			t_vec.clear();
		}
	}
	DrawLrm(*m_Output_Poitn,num_of_req_H,x_normal,y_normal,x_long,y_long,m_color,LayerHight);
	//cDrawLRMImage *DRI = new cDrawLRMImage;
	//DRI->DrawLrm(m_Output_Poitn,num_of_req_H,x_normal,y_normal,x_long,y_long,m_color,LayerHight);
	//DRI->show();
}

double LrmAlg::Get_Length( double dx, double dy )
{
	return( sqrt(dx*dx + dy*dy) );
}

void LrmAlg::Swap( float &m_small, float &m_big )
{
	float temp = 0.0;
	if (m_small>m_big)
	{
		temp = m_big;
		m_big = m_small;
		m_small = temp;
	}
}

void LrmAlg::transcoor( int &tsx,int &tsy )
{
	tsx = tsx+x_normal+x_long/2;
	tsy = y_normal-tsy-y_long/2;
}

void LrmAlg::DrawLrm( osg::Vec4Array& m_p4DArray,int m_lay,int m_x_normal,int m_y_normal,int m_x_long,int m_y_long,P_COLORTABLE_VECTOR m_ColorTable ,QList<int> m_highlay)
{
	for (int z = 0;z<m_lay;z++)
	{
		QImage image(m_x_long,m_y_long, QImage::Format_RGB32);
		QRgb value;
		value = qRgb(255,255,255);
		image.fill(value);
		for (int x = m_x_normal, xx = 0;x<m_x_normal+m_x_long;x++,xx++)
		{
			for (int y = m_y_normal,yy = m_y_long;y<m_y_normal+m_y_long;y++,yy--)
			{
				for (int i = 0;i<m_p4DArray.size();i++)
				{
					if (m_p4DArray.at(i).x() == x && m_p4DArray.at(i).y() == y && m_p4DArray.at(i).z() == z)
					{
						for(int j=0;j<m_ColorTable->size();j++)
						{
							if (m_p4DArray.at(i).w()<=m_ColorTable->at(j)->fEnd && m_p4DArray.at(i).w()>m_ColorTable->at(j)->fStart)
							{
								value = qRgb(m_ColorTable->at(j)->nRed,m_ColorTable->at(j)->nGreen,m_ColorTable->at(j)->nBlue);
								image.setPixel(xx, yy, value);
							}
						}
					}
				}
			}
		}
		/*
		QPixmap *res = new QPixmap(m_x_long,m_y_long);
		QPainter pt(res);
		pt.begin(res);
		pt.drawImage(0,0,image);
		pt.drawRect(0,0,m_x_long-1,m_y_long-1);
		pt.end();

		QPixmap pix = *res;*/
		if(m_LayerPixmap)
			m_LayerPixmap->push_back(image);
	}
}

void LrmAlg::SetRectPoint( float x1, float y1, float x2, float y2, float fx1, float fy1, float fx2, float fy2 )
{
	m_Point->push_back(osg::Vec3(x1, y1, 0));
	m_Point->push_back(osg::Vec3(x2, y2, 0));
	m_Point->push_back(osg::Vec3(fx1, fy1, 0));
	m_Point->push_back(osg::Vec3(fx2, fy2, 0));
}

void LrmAlg::SetLayerList( const QStringList& strList )
{
	m_HighOfLay = strList;
}

void LrmAlg::SetImageList( QVector<QImage>* pList )
{
	m_LayerPixmap = pList;
}
