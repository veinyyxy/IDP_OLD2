#include "radarsingledataprovider.h"
#include "AreaStatistic.h"
#include <QDebug>
AreaStatistic::AreaStatistic(void):QObject()
{
	pPoint = NULL;
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

void AreaStatistic::AreaAdd(bool tArea,bool tPie,float tMin,float tMax,goto_gis::DataProvider* tDp)
{
	bool m_bAllArea = tArea;
	bool m_bPie = tPie;
	float m_vMin = tMin;
	float m_vMax = tMax;

	RadarSingleDataProvider *tempDp = NULL;
	tempDp = dynamic_cast<RadarSingleDataProvider*>(tDp);
	gInt16 proudcutCode=0;
	if(NULL!=tempDp->GetDrawProduct())
		tempDp->GetDrawProduct()->GetProductCode(proudcutCode);
	P_COLORTABLE_VECTOR pColorTable =tempDp->GetColorTable();

	int nNumOfcn = pColorTable->size();//��ɫ�����
	int *AreaCout = new int[nNumOfcn];//���ڴ洢ÿ����ɫռ�����
	int drawAllAreabit = 0;//���ڴ洢�����
	float (*cn)[2];//�洢��ɫֵ�ķ�Χ
	cn = new float[nNumOfcn][2];
	for (int i=0;i<nNumOfcn;i++)
	{
		AreaCout[i]=0;
		cn[i][0] = pColorTable->at(i)->fStart;
		cn[i][1] = pColorTable->at(i)->fEnd;
	}
	double			dGateOrReso[2];  //aadded by wyh for ��ȡ�ⳤ��ֱ��� 20081224
	double dRealArea=0.0 ;//��λ���
	//��ȡ��Ʒ���ͣ�
	CProductDrawType nType=enProductUnkown;
	if(NULL!=tempDp->GetDrawProduct())
		nType=tempDp->GetDrawProduct()->GetTypeOfProduct();
	vector<vec_int> Areapels;
	if ((501 != proudcutCode) && (502 != proudcutCode) && (503 != proudcutCode) && (504 != proudcutCode))//��վ
	{
		tempDp->GetDrawProduct()->OnGetDiagramParametre(Areapels,nNumOfcn,cn , dGateOrReso,tempDp->GetRequsetLayer());//�õ�����
	}
	else
	{
		qDebug()<<"�������proudcutCode"+proudcutCode;
		return; //�������proudcutCode
	}
	if(nType!=enProductUnkown)
	{
		switch(nType)
		{
		case enProductRadial://����
			dRealArea = dGateOrReso[0]*dGateOrReso[0]*M_PI/360.0;
			//m_dGateOrReso[0]*m_dGateOrReso[0]*M_PI/360.0;
			//double TotalArea = ((double)drawAllAreabit)*dRealArea; //�����

			if (m_bAllArea) //��������
			{
				//��ȡ���о�����ÿ�����Ӧ�Ĳ��ֵ	
				for(vector<vec_int>::iterator it = Areapels.begin(); it != Areapels.end(); it++)
				{
					int ii = 0; //��¼�����ϵڼ�����
					int jj = 0; //��ͬ����������ԣ�

					for( vec_int::iterator colit = it->begin(); colit != it->end(); colit++)
					{
						jj = 2*ii + 1;

						if (*colit<nNumOfcn&&*colit>=0)
						{
							AreaCout[*colit] = AreaCout[*colit] + jj; //һ����ɫ��ռ�����
							drawAllAreabit = drawAllAreabit + jj; //������ɫ��ռ�����//reto
						}
						ii++;
					}
				}
			}
			else //ָ������
			{
				int m_alpha = 0;//�Ƕ�
				QPointF pLU,pRD;//ʵ�����ϣ�ʵ������
				pLU.setX(pPoint->at(0).x()/1000.0);
				pLU.setY(pPoint->at(0).y()/1000.0);
				pRD.setX(pPoint->at(1).x()/1000.0);
				pRD.setY(pPoint->at(1).y()/1000.0);
				float PointSmallX = pLU.x();
				float PointSmallY = pLU.y();
				float PointBigX = pRD.x();
				float PointBigY = pRD.y();
				Swap(PointSmallX,PointBigX);
				Swap(PointSmallY,PointBigY);

				for(vector<vec_int>::iterator it = Areapels.begin(); it != Areapels.end(); it++)
				{
					int ii = 0; //��¼�����ϵڼ�����
					int jj = 0; //��ͬ����������ԣ�


					for( vec_int::iterator colit = it->begin(); colit != it->end(); colit++)
					{
						jj = 2*ii + 1;
						float m_R = ii*dGateOrReso[0];
						float m_X = m_R*cos(m_alpha*1.0/180.0*M_PI);
						float m_Y = m_R*sin(m_alpha*1.0/180.0*M_PI);
					
						if (m_X>=PointSmallX && m_X<=PointBigX && m_Y>=PointSmallY && m_Y<=PointBigY)
						{
							if (*colit<nNumOfcn&&*colit>=0)
							{
								AreaCout[*colit] = AreaCout[*colit] + jj; //һ����ɫ��ռ�����
								drawAllAreabit = drawAllAreabit + jj; //������ɫ��ռ�����//reto
							}

						}
						ii++;
					}
					m_alpha++;
				}
			}
			break;

		default:
			break;
		}
	}
	//��ȡ��������
	//���������ɫֵ��ռ�������
	//���ݲ�Ʒ�Ż�ȡ��ɫ�б�
	//�������ͳ��ͼ
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
				m_pPieView = new CDraw3dPieChart;
				m_pPieView->setPiePerCent(vlstPercent,vXvect,TotalArea);
				m_pPieView->setChartDepth(15);
				m_pPieView->setPieSize(280,200);
				m_pPieView->show();

			}
			else
			{
				//QMessageBox::warning(this,tr("��Ϣ��ʾ"),tr("������û��ѡ�з�Χ�����ݣ�\n�޷��������ͳ��."));
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
			if (pPoint)
			{
				if (pPoint->size() > 0)
				{
					pPoint->clear();
				}
			}
			
		}
	}

	
}