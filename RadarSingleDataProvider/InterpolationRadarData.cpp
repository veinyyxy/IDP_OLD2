#include <list>
#include <vector>
#include <QtCore/QTextStream>
#include <QtCore/QFile>
#include "../GDataTypes/cDrawProduct.h"
#include "InterpolationRadarData.h"


InterpolationRadarData::InterpolationRadarData(void)
{
	m_eleList = new std::list<gInt32>;
	tf = false;
}


InterpolationRadarData::~InterpolationRadarData(void)
{
}

void InterpolationRadarData::Profiles()
{
	//缩放时候以及移动时候，图像中心的相对于打开的源图像的中心位置                                                             AS  zyf    20080723
	//dRatio = 1;//= temp->GetProductRatio()/1000;
	std::vector<int> vpj;
	int pj = 0;
	bool bopen;
	if (m_produceDraw != NULL)
	{
		bopen= GetSectionParam();
		tf = true;
	}
	else{
		bopen = false;
		/*QMessageBox msg;
		msg.setText("没有打开可用数据！");
		msg.exec();*/
		tf = false;
		return;
	}
	double distance_test = (m_Sectonstar.y-m_Sectonend.y)*ProductCenter_X+(m_Sectonend.x-m_Sectonstar.x)*ProductCenter_Y
		+m_Sectonstar.x*m_Sectonend.y-m_Sectonend.x*m_Sectonstar.y;
	if(distance_test<0)
	{
		distance_test = -distance_test;
	}
	distance_test = distance_test/sqrt((double)((m_Sectonstar.y-m_Sectonend.y)*(m_Sectonstar.y-m_Sectonend.y)
		+(m_Sectonend.x-m_Sectonstar.x)*(m_Sectonend.x-m_Sectonstar.x)));

	double tempResult = (m_Sectonend.x-m_Sectonstar.x)*(m_Sectonend.x-m_Sectonstar.x)+(m_Sectonend.y-m_Sectonstar.y)*(m_Sectonend.y-m_Sectonstar.y);
	ds = sqrt(tempResult);   //剖面线的长

	int tempDS = ds;
	int tempLen = ds*dRatio;

	//m_sectionArr = new int **[m_eleCnt];
	//m_sectionArr[i] = new int *[m_azimuthNum];
	//m_sectionArr[i][ii] = new int [m_count_gate];

	if(tempLen <= scanR){tempDS = scanR/dRatio;}
	else {tempDS = scanR*2/dRatio-100;}
	//ME by qutao for 修改原保存横坐标轴固定代码问题（原来改变的分辨率）
	double angle1 = Get_Angle_Of_Direction(m_Sectonstar.x-ProductCenter_X,	m_Sectonstar.y-ProductCenter_Y); //modified by Limh for 中心移动后求角度 20080516
	double distance1 = Get_Length(m_Sectonstar.y-ProductCenter_Y,m_Sectonstar.x-ProductCenter_X)*dRatio;  //起点实际距离雷达中心的距离 20080516
	int azL = (int)(angle1 * 180.00/M_PI);
	//wxPoint posL = wxPoint(azL,(int)distance1);
	spoint posL;
	posL.x = azL;posL.y = int(distance1);
	AzList.append(QString("%1").arg(azL));
	RList.append(QString("%1").arg((int)distance1));
	double angle2 = Get_Angle_Of_Direction(m_Sectonend.x-ProductCenter_X,	m_Sectonend.y-ProductCenter_Y); //modified by Limh for 中心移动后求角度 20080516
	double distance2 = Get_Length(m_Sectonend.y-ProductCenter_Y,m_Sectonend.x-ProductCenter_X)*dRatio;  //终点实际距离雷达中心的距离 20080516
	int azR = (int)(angle2 * 180.00/M_PI);
	//wxPoint posR = wxPoint(azR,(int)(distance2 +0.5));
	// 	spoint posR;
	// 	posR.x = azR;posR.y = int(distance2+0.5);
	double x_res =0.0;
	AzList.append(QString("%1").arg(azR));
	RList.append(QString("%1").arg(int(distance2+0.5)));
	//确定图像宽度和分辨率
	if(ds<=50){x_res = 0.125;}
	else if(ds<=100){x_res=0.25;}   //为了显示的好看，把像素放大，}
	else if(ds<=200){x_res=0.5;}
	else if (ds <= 300){x_res = 0.75;}
	else if (ds <= 1000){x_res=1.0;}
	else{x_res = 1.5;}

	ds = (int)(ds/x_res);   //放大以后的屏幕距离
	int vh=400; //400<=>20Km
	double dx = 0.0;double dy = 0.0;
	dx = (double)(m_Sectonend.x -m_Sectonstar.x)/(double)ds;
	dy =(double)(m_Sectonend.y -m_Sectonstar.y)/(double)ds;

	double x = (double)(m_Sectonstar.x -ProductCenter_X)/x_res;             //起始点到移动放大后中心点   MOdified      by zyf
	double y = (double)(ProductCenter_Y -m_Sectonstar.y)/x_res;   //像素放大以后的屏幕坐标

	if (bopen == false){
		m_flagsection = false;
		/*QMessageBox msg;
		msg.setText("不是多层数据或不是基本产品\n无法进行剖面图绘制！");
		msg.exec();*/
		tf = false;
		return;}
	else
	{
		m_flagsection = true;
		dh = 0;max =0;maxAz =0;maxR = 0;
		maxH = 0.0;
		spoint posM;posM.x = 0;posM.y = 0;
		int t = m_eleList->size();
		int radHei = m_radarHeight*0.1;  //雷达高度 ，limei，08.01
		double y_res=(double)20.0/vh;
		dh = 400;

		buf= new int*[ds];
		for (int k =0 ;k <ds ;k++)
		{
			buf[k] = new int[dh];
		}
		for (int k =0 ;k< ds ;k++)
		{
			for (int z =0 ;z < dh ;z++)
			{
				buf[k][z]= -32768;
			}
		}
		int* vb = (int*)malloc(sizeof(int)*t);
		int* kh = (int*)malloc(sizeof(int)*t);
		int* nazis = (int*)malloc(sizeof(int)*t);  //存放各ds下每个仰角层的方位角和距离库，最大值用，limei,04.07
		int* ngates = (int*)malloc(sizeof(int)*t);

		for(int k=0;k<t;k++)
		{
			vb[k] = -32768;
			kh[k] = -32768;
			nazis[k] = -32768;
			ngates[k] = -32768;
		}

		int middle = (int)ds/2;          //代码优化，提到for外
		for(int i=0;i<ds;i++)
		{
			double si,azi;
			si = sqrt(x*x+y*y);
			if(si<1)
			{
				si=0;
				azi=0;
			}
			else
			{
				azi = asin(x/si);
				if(y>0&&x<0) azi=2*M_PI+azi;
				else if(y<0) azi=M_PI-azi;
			}
			int n = 0;

			int nazi = (int)(azi * 18000.00/M_PI);
			if (i == middle)
			{
				//posM = QPoint((int)nazi/100,(int)(si*x_res*dRatio));  //实际距离
				posM.x = (int)nazi/100;
				posM.y = (int)(si*x_res*dRatio);

				AzList.append(QString("%1").arg((int)nazi/100));
				RList.append(QString("%1").arg((int)(si*x_res*dRatio)));
			}

			for (list<gInt32>::iterator iter =m_eleList->begin(); iter!=m_eleList->end(); ++iter)
			{
				vb[n]=0;
				int laz,raz;

				int j=0;

				for(int k=0;k<360;k++)
				{
					raz = k*100;

					if( k==0 ) laz=35900;
					else laz = (k-1);

					if( laz>30000&&raz<3000)
					{
						raz+=36000;
						if( nazi< 500 )  nazi += 36000;
					}

					if(laz<=nazi  && raz>nazi )
					{
						j=k;
						break;
					}
				}
				if (pj != j)//参加运算的径向
				{
					pj = j;
					vpj.push_back(pj);
				}
				//AS 雷达波束宽度为0.95度
				//gInt32 temp1 = *iter;
				double DetaDegree = 0.95/2;
				double temp1;
				if(n==0)
				{
					temp1 = (double)*iter - DetaDegree*100;  //第一个仰角层减半个波束宽度
					if(temp1<0.0)
						temp1 = 0.0;
				}
				else if(n==t-1)
				{
					temp1 = (double)*iter + DetaDegree*100;  //最后一个仰角层加半个波束宽度
				}
				else
				{
					temp1 = (double)*iter;
				}

				//AE 第一个仰角和最后一个仰角把波束宽度算进去

				double si2 = si*x_res*dRatio*1000;  //si*x_res转为实际像素，*(temp->m_ratio)转为实际距离（单位米）
				double beta = si2/REarth;       //double beta = si/(REarth*0.001);   //过地心的剖切面与竖直方向的夹角，si单位千米，
				double dnow = (REarth + radHei)*0.001*cos(temp1*M_PI/18000)/cos(beta+temp1*M_PI/18000) - REarth*0.001; //剖切面与锥面的交点到地心的距离，REarth 、 radHei单位米
				int ngate =(int)((dnow*1000+REarth)/(m_len_gate)*sin(beta)/cos(temp1*M_PI/18000));   //R的一个距离库是1000m，而V,W的一个距离库是250m
				if(ngate<m_count_gate-1)                       //添加判断，防止ngate超出数组边界，limei，08.28 //R的距离数是460，而V,W的距离库数是920
				{

					int ndata = m_sectionArr[n][j][ngate];
					int ndata2 = m_sectionArr[n][j][ngate+1];
					if (ndata != -32768 && ndata != -32767 && ndata != -32766 && ndata2 != -32768 && ndata2 != -32767 && ndata2 != -32766)
					{
						vb[n]=m_sectionArr[n][j][ngate]/2 +m_sectionArr[n][j][ngate+1]/2;
					}
					else
					{
						vb[n]=m_sectionArr[n][j][ngate];
					}

					int dn =(int) (dnow/y_res);
					if (dn <= dh)
					{
						kh[n] = (int)dh -dn;

						nazis[n] = nazi/100;  //把当前的方位角和距离库存起来，求最大值时用，limei,04.07
						ngates[n] = ngate;
						n++;
					}
					else
					{
						break;
					}
				}
				else  //如果距离库数大于最大距离库数
				{
					kh[n] = -1;
					n++;
				}
			}
			//垂直方向的插值

			for(int j=0;j< n-1;j++)
			{
				if(vb[j]&&vb[j+1] && vb[j]!=-32768 && vb[j]!=-32767 && vb[j]!=-32766 && vb[j+1]!=-32768 && vb[j+1]!=-32767 && vb[j+1]!=-32766)
				{
					for(int k=kh[j];k>kh[j+1];k--)
					{
						if(k>0 && k< 400)
						{
							buf[i][k] =((kh[j]-k)*vb[j+1]+(k-kh[j+1])*vb[j])/(kh[j]-kh[j+1]);

							int value = buf[i][k];
							if (value > max)  //插值之后求最大值，以防最大值存在，但插值之后可能被掩盖掉
							{
								max   = value;
								maxAz = nazis[j];
								maxR  = ngates[j] * m_len_gate/1000;
								maxH  = (double)(400-k)/(double)20;
							}
						}
					}
				}
				//else
			}
			x+= dx; y -=dy;
		}

		//水平方向插值
		for(int j=0;j<dh;j++)
		{
			for(int i=1;i<ds-1;i++)
			{
				if( buf[i][j]==-32768 && buf[i-1][j]!= -32768&& buf[i+1][j]!= -32768)
				{
					if(buf[i][j] == max)   //水平插值也可能把最大值修改了
					{
						buf[i][j] = (buf[i-1][j]+buf[i+1][j])/2;
						max = buf[i][j];
					}
					else buf[i][j] = (buf[i-1][j]+buf[i+1][j])/2;
				}
			}
		}
		spoint posMax;posMax.x = maxAz;posMax.y = maxR;

		if (vb != NULL)
		{
			free(vb);
		}
		if (kh !=NULL)
		{
			free(kh);
		}
		if (nazis != NULL)
		{
			free(nazis);
		}
		if (ngates !=NULL)
		{
			free(ngates);
		}
		if(m_ration!=NULL)
		{
			delete []m_ration;
			m_ration = NULL;
		}
		if(m_sectionArr !=NULL)
		{
			for(int n1 = 0; n1 < m_eleCnt; n1++)
			{
				for(int n2 = 0; n2 < m_azimuthNum; n2++)
				{
					delete [] m_sectionArr[n1][n2];
				}
				delete [] m_sectionArr[n1];
			}
			delete []m_sectionArr;
			m_sectionArr =NULL;
		}
	}

	QFile textFile("C:\\buff.txt");
	textFile.open(QIODevice::WriteOnly);
	QTextStream ts(&textFile);


}

bool InterpolationRadarData::GetSectionParam()
{
	bool bRet=false;
	long n_elevation = 0.0;
	m_count_gate = 0;
	try
	{
		m_eleList->clear();
		vector<vec_int> paramArray;

		int num_of_gate		= 0;
		int elevation = 0;
		int bmpHei = 200;// = GetShowWindow()->GetProductSize().GetHeight();
		float zoom = dRatio;// = GetShowWindow()->GetZoom();

		m_eleCnt = m_produceDraw->GetCountOfLayer();  //limei,08.28,得到总的仰角层数
		if (m_eleCnt<=5)	//产品仰角数<=5,不作处理
		{
			return false;
		}
		m_ration = new double[m_eleCnt];
		m_sectionArr = new int **[m_eleCnt];
		if(m_sectionArr!=NULL)//add by ljg 20090205 for 防止内存泄露
		{
			memset(m_sectionArr,-32768,m_eleCnt*sizeof(int **));
		}

		for(int i =0; i<m_eleCnt; i++ )
		{
			n_elevation = i;
			m_produceDraw->ReadInfo(paramArray, m_azimuthNum, num_of_gate, elevation,m_radarHeight,i,m_len_gate,bmpHei,zoom,m_ration);
			proCode = m_produceDraw->GetProCode();
			m_eleList->push_back(elevation);


			if(num_of_gate>m_count_gate)
			{
				m_count_gate = num_of_gate;   //最大距离库数
			}
			scanR = m_count_gate*m_len_gate/1000;
			m_sectionArr[i] = new int *[m_azimuthNum];
			if(m_sectionArr[i]!=NULL)//add by ljg 20090205 for 防止内存泄露
			{
				memset(m_sectionArr[i],-32768,m_azimuthNum*sizeof(int *));
			}

			int ii = 0;

			for(vector<vec_int>::iterator it = paramArray.begin(); it != paramArray.end() && ii<m_azimuthNum; it++,ii++ )
			{
				m_sectionArr[i][ii] = new int [m_count_gate];

				if(m_sectionArr[i][ii]!=NULL)//add by ljg 20090205 for 防止内存泄露
				{
					memset(m_sectionArr[i][ii],-32768,m_count_gate*sizeof(int ));
				}

				int jj = 0;
				for( vec_int::iterator colit = it->begin(); colit != it->end() && jj<num_of_gate; colit++,jj++)
				{
					m_sectionArr[n_elevation][ii][jj] = *colit;
				}
			}
			paramArray.clear();
		}

		m_eleList->sort();
		bRet=true;
	}
	catch(exception e)//'new' has chance to throw exception.
	{
		bRet=false;
		//wxString str=_("Catch an Exception in:");
		//str+=wxT("ssProduct_Splitter_Pane::GetSectionParam,for:");
		//wxString strFor(e.what(),wxConvLibc);
		//str+=strFor;
		//wxMessageBox(str,_("Tips"));

		//del by ljg 20090206 for 总是崩掉。
		if(m_sectionArr !=NULL)//add by ljg 20090205 for 防止内存泄露
		{
			for(int n1 = 0; n1 < m_eleCnt; n1++)
			{
				for(int n2 = 0; n2 < m_azimuthNum; n2++)
				{
					delete [] m_sectionArr[n1][n2];
				}
				delete [] m_sectionArr[n1];
			}
			delete [] m_sectionArr;
			m_sectionArr =NULL;
		}

	};
	return bRet;
}

double InterpolationRadarData::Get_Angle_Of_Direction( double dx, double dy )
{
	//MS by Limh for 根据x,y求角度
		if ( dx == 0 && dy>0 )
		{
			return M_PI;
		}
		if ( dx>0 && dy==0)
		{
			return M_PI / 2.0;
		}
		if ( dx<0 && dy==0 )
		{
			return M_PI * 3.0 / 2.0;
		}
		if ( dx>0 && dy<0 )
		{
			return atan2(dx,-dy);
		}
		if ( dx>0 && dy >0)
		{
			return M_PI - atan2(dx,dy);
		}
		if ( dx<0 && dy<0 )
		{
			return 2*M_PI - atan2(-dx,-dy);
		}
		if ( dx<0 && dy>0 )
		{
			return M_PI + atan2(-dx,dy);
		}
		return 0;
}

double InterpolationRadarData::Get_Length( double dx, double dy )
{
	return( sqrt(dx*dx + dy*dy) );
}

void InterpolationRadarData::SetStartEnd( double* x1, double* y1, double* x2, double* y2 )
{
	m_Sectonstar.x = *x1;
	m_Sectonstar.y = *y1;

	m_Sectonend.x = *x2;
	m_Sectonend.y = *y2;
}

void InterpolationRadarData::SetStartEnd( double* x1, double* y1, double* x2, double* y2 ,double dR,double pc_x,double pc_y)
{
	m_Sectonstar.x = *x1;
	m_Sectonstar.y = *y1;

	m_Sectonend.x = *x2;
	m_Sectonend.y = *y2;

	ProductCenter_X = pc_x;
	ProductCenter_Y = pc_y;
	dRatio = dR;
}