#include "radarMosaicDataProvider.h"
#include "AreaStatistic.h"

AreaStatistic::AreaStatistic(void):QObject()
{
	ispoint = false;
}


AreaStatistic::~AreaStatistic(void)
{
}

void AreaStatistic::Swap( float &m_small, float &m_big )
{
	float temp = 0.0;
	if (m_small>m_big)
	{
		temp = m_big;
		m_big = m_small;
		m_small = temp;
	}
}

void AreaStatistic::AreaAdd(bool tArea,bool tPie,float tMin,float tMax,DataProvider* tDp)
{
	bool m_bAllArea = tArea;
	bool m_bPie = tPie;
	float m_vMin = tMin;
	float m_vMax = tMax;

	RadarMosaicDataProvider *tempDp = NULL;
	tempDp = dynamic_cast<RadarMosaicDataProvider*>(tDp);

	gInt16 proudcutCode=0;
	if(NULL!=tempDp->GetDrawProduct())
		tempDp->GetDrawProduct()->GetProductCode(proudcutCode);

	P_COLORTABLE_VECTOR pColorTable =tempDp->GetColorTable();

	int nNumOfcn = pColorTable->size();//颜色层个数
	int *AreaCout = new int[nNumOfcn];//用于存储每种颜色占的面积
	int drawAllAreabit = 0;//用于存储总面积
	float (*cn)[2];//存储颜色值的范围
	cn = new float[nNumOfcn][2];
	for (int i=0;i<nNumOfcn;i++)
	{
		AreaCout[i]=0;
		cn[i][0] = pColorTable->at(i)->fStart;
		cn[i][1] = pColorTable->at(i)->fEnd;
	}
	double			dGateOrReso[2];  //aadded by wyh for 获取库长或分辨率 20081224
	double dRealArea=0.0 ;//单位面积
	//获取产品类型，
	CProductDrawType nType=enProductUnkown;
	if(NULL!=tempDp->GetDrawProduct())
		nType=tempDp->GetDrawProduct()->GetTypeOfProduct();



	vector<vec_int> Areapels;
	if ((501 != proudcutCode) && (502 != proudcutCode) && (503 != proudcutCode) && (504 != proudcutCode))//单站
	{
		return;
		//tempDp->GetDrawProduct()->OnGetDiagramParametre(Areapels,nNumOfcn,cn , dGateOrReso,tempDp->GetRequsetLayer());//得到参数
	}
	else//组网//redo
	{
		//temDp->m_multiProductDraw->OnGetDiagramParametre(Areapels, nNumOfcn, pChildFrame->m_proInfo.proColorInfo.pcn, m_dGateOrReso, (int)requestLayer);//得到参数
		tempDp->GetDrawProduct()->OnGetDiagramParametre(Areapels,nNumOfcn,cn , dGateOrReso,tempDp->GetRequsetLayer());
	}
	if(nType!=enProductUnkown)
	{
		switch(nType)
		{
		case enProductGrid://栅格
			dRealArea = dGateOrReso[0]*dGateOrReso[1]; //栅格产品一小块的绝对面积

			if (m_bAllArea) //整个区域
			{
				for(vector<vec_int>::iterator it=Areapels.begin(); it!=Areapels.end(); it++)
				{
					for( vec_int::iterator colit=it->begin(); colit!=it->end(); colit++)
					{
						if ((*colit>=0) && (*colit<nNumOfcn))
						{
							AreaCout[*colit]++;
							drawAllAreabit++;
						}
					}
				}
			}
			else
			{
				QPointF pLU,pRD;//实际左上，实际右下
				pLU.setX(pPoint->at(0).x());
				pLU.setY(pPoint->at(0).y());
				pRD.setX(pPoint->at(1).x());
				pRD.setY(pPoint->at(1).y());
				float PointSmallX = pLU.x();
				float PointSmallY = pLU.y();
				float PointBigX = pRD.x();
				float PointBigY = pRD.y();
				Swap(PointSmallX,PointBigX);
				Swap(PointSmallY,PointBigY);

				int n = Areapels.size();
				vector<vec_int>::iterator it=Areapels.begin();
				int r = it->size();

				QStringList sl;
				sl.append(QString::number(tempDp->GetRect()->top()));
				sl.append(QString::number(tempDp->GetRect()->bottom()));
				sl.append(QString::number(tempDp->GetRect()->right()));
				sl.append(QString::number(tempDp->GetRect()->left()));

				float y_setp = (sl.at(0).toFloat()-sl.at(1).toFloat())/**0.0001*//n;
				float x_setp = (sl.at(2).toFloat()-sl.at(3).toFloat())/**0.0001*//r;
				float xS = sl.at(2).toFloat();//*0.0001;
				float xB = sl.at(3).toFloat();//*0.0001;
				float yS = sl.at(0).toFloat();//*0.0001;
				float yB = sl.at(1).toFloat();//*0.0001;
				Swap(xS,xB);
				Swap(yS,yB);
				int yNum = 0;
				for(it; it!=Areapels.end(); it++)
				{
					int xNum = 0;
					for( vec_int::iterator colit=it->begin(); colit!=it->end(); colit++)
					{
						float m_X = xS+xNum*x_setp;
						float m_Y = yS+yNum*y_setp;
						if ((*colit>=0) && (*colit<nNumOfcn))
						{
							if (m_X>=PointSmallX && m_X<=PointBigX && m_Y>=PointSmallY && m_Y<=PointBigY)
							{
								AreaCout[*colit]++;
								drawAllAreabit++;
							}

						}
						xNum++;
					}
					yNum++;
				}
			}
			break;
		case enProductStruct:
		default:
			break;
		}
	}
	//获取基础数据
	//计算各个颜色值所占面积比例
	//根据产品号获取颜色列表
	//绘制面积统计图
	if (drawAllAreabit != 0)
	{
		if (m_bPie)
		{

			QList<PerColorPercent> vlstPercent;
			QVector<float> vXvect;
			for(int i = 0;i<nNumOfcn;i++)
			{
				double dPercent = (((double)AreaCout[i]*dRealArea)/(drawAllAreabit*dRealArea));

				if (dPercent>0.000000001 && pColorTable->at(i)->fStart>=m_vMin && pColorTable->at(i)->fEnd<=m_vMax)
				{
					PerColorPercent temp;
					temp.qcolor.setRgb(pColorTable->at(i)->nRed,pColorTable->at(i)->nGreen,pColorTable->at(i)->nBlue);
					vXvect.push_back(pColorTable->at(i)->fStart);
					vXvect.push_back(pColorTable->at(i)->fEnd);
					temp.dpercent = dPercent;
					if (0.0== dPercent)
					{
						continue;
					}else
						vlstPercent.push_back(temp);
				}
			}
			double TotalArea = ((double)drawAllAreabit)*dRealArea; 
			if (vlstPercent.size()>0)
			{		
// 				if (m_pPieView != NULL)
// 				{
// 					delete m_pPieView;
// 					m_pPieView = NULL;
// 				}
				m_pPieView = new CDraw3dPieChart;
				m_pPieView->setPiePerCent(vlstPercent,vXvect,TotalArea);
				m_pPieView->setChartDepth(15);
				m_pPieView->setPieSize(280,200);
				connect(m_pPieView,SIGNAL(closewindow()),this,SLOT(closeWin()));
				m_pPieView->show();
			}
			else
			{
				//QMessageBox::warning(this,tr("信息提示"),tr("区域内没有选中范围的数据，\n无法进行面积统计."));
			}

		}
		else
		{

			QList<PerColorPerInfo> vlstPercent;
			QVector<float> vXvect;
			double tempv = 0.00000001;
			double TotalArea = ((double)drawAllAreabit)*dRealArea; 
			for(int i = 0;i<nNumOfcn;i++)
			{
				double dPercent = (((double)AreaCout[i]*dRealArea)/(drawAllAreabit*dRealArea));

				if (dPercent>0.000000001 && pColorTable->at(i)->fStart>=m_vMin && pColorTable->at(i)->fEnd<=m_vMax)
				{
					PerColorPerInfo temp;
					temp.qcolor.setRgb(pColorTable->at(i)->nRed,pColorTable->at(i)->nGreen,pColorTable->at(i)->nBlue);
					vXvect.push_back(pColorTable->at(i)->fStart);
					vXvect.push_back(pColorTable->at(i)->fEnd);
					temp.dpercent = dPercent;
					if (temp.dpercent>tempv)
					{
						tempv = temp.dpercent;
					}
					vlstPercent.push_back(temp);
				}
			}
			if (vlstPercent.size()>0)
			{		
				m_pColumn = new CDraw3dColumn;
				m_pColumn->setPiePerCent((int)(tempv*10),vlstPercent,vXvect,TotalArea);
				m_pColumn->show();
			}
			//pPoint->clear();
		}
	}
	
}
