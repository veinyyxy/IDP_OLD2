/**********************************************************
* Version $Id: Interpolation_InverseDistance.cpp 911 2011-02-14 16:38:15Z reklov_w $
*********************************************************/

#include "Interpolation_InverseDistance.h"
#include <QTextCodec> 
#include <limits>
//bool is_nan(double dVal)
//{　　
//	double dNan = std::numeric_limits<double>::quiet_NaN();
//	if (dVal==dNan)
//		return true;
//	return false;
//}

InverseDistanceInterpolation::InverseDistanceInterpolation(void)
{

}

//---------------------------------------------------------
double InverseDistanceInterpolation::Get_Weight(double Distance)
{
	switch( m_IDWParam.weight )
	{
	case 0:	    return( Distance > 0.0 ? pow(Distance, - m_IDWParam.power) : -1.0 );
	case 1:		return( Distance <  m_IDWParam.radius ? (1.0 - Distance /  m_IDWParam.radius) : 0.0 );
	case 2:		return( exp(-Distance /  m_IDWParam.bandwidth) );
	case 3:		return( exp(-0.5 * (Distance /  m_IDWParam.bandwidth)*(Distance /  m_IDWParam.bandwidth)) );
	}
}

//---------------------------------------------------------
bool InverseDistanceInterpolation::GetValue(double x, double y, double &z)
{
	double	w, ix, iy, iz;

	z	= 0.0;
	w	= 0.0;

	//-----------------------------------------------------
	if( m_IDWParam.pointMax > 0 || m_IDWParam.radius > 0.0 )	// using search engine
	{
		if (m_IDWParam.strange == QObject::tr("最大点数")) 
		{
			int		nPoints	= Select_Nearest_Points(x, y, m_IDWParam.pointMax, m_IDWParam.radius, m_IDWParam.mode == 0 ? -1 : 4);

			for(int iPoint=0; iPoint<nPoints; iPoint++)
			{
				if( Get_Selected_Point(iPoint, ix, iy, iz) ) //根据id获取邻近点，并得到改点的坐标值
				{      			
					double d = Get_Weight(Get_Distance(x,y,ix,iy));					

					if( d <= 0.0 )
					{
						z	= iz;

						return( true );
					}

					z	+= d * iz;
					w	+= d;									
				}
			}


		}

		if (m_IDWParam.strange == QObject::tr("所有点")) 
		{
			int		nPoints = m_DataArray.count();

			for(int iPoint=0; iPoint<nPoints; iPoint++)
			{
				//if( Get_Selected_Point(iPoint, ix, iy, iz) ) //根据id获取邻近点，并得到该点的坐标值								
				{    
					ix = m_DataArray.at(iPoint).x();
					iy = m_DataArray.at(iPoint).y();
					iz = m_DataArray.at(iPoint).z();

					double d = Get_Weight(Get_Distance(x,y,ix,iy));	
					//	double d = 1.0f;
					//double d = 1.0f/Get_Distance(x,y,ix,iy);

					if( d <= 0.0 )
					{
						z	= iz;

						return( true );
					}

					z	+= d * iz;
					w	+= d;
				}
			}
		}		

	}	

	//-----------------------------------------------------
	if( w > 0.0 )
	{
		z	/= w;

		return( true );
	}

	return  false;
}

int InverseDistanceInterpolation::Select_Nearest_Points(double x, double y, int maxPoints, double Radius, int iQuadrant)
{
	m_SelectedData.clear();
	//datarry.clear();
	/*
	int Step = (int)Radius;
	int* nCount= (int *)malloc(Step * sizeof(int));

	for (int i=0;i<Step;i++)
	{
	nCount[i] = 0;
	}

	for (int i=0;i<Step;i++)
	{
	QVector<QVector3D>* tempdata = new QVector<QVector3D>;
	datarry.push_back(tempdata);	
	}	

	double dismax,dismin;

	for (int i=0;i<m_DataArray.count();i++)
	{
	QVector3D temp = m_DataArray.at(i);
	float tempz = m_DataArray.at(i).z();

	double tempdis = Get_Distance(x,y,temp.x(),temp.y());

	if (tempdis<=Radius && tempz!=m_IDWParam.fUndef) 
	{
	for (int j=0;j<Step;j++)
	{
	if (tempdis>= j*Radius/float(Step) && tempdis<=(j+1)*Radius/float(Step))
	{					
	datarry.at(j)->push_back(temp);
	nCount[j]++;
	}
	}
	}				
	}	

	int countSum = 0;
	int istep = 0;

	for (istep=0;istep<Step;istep++)
	{		
	//countSum
	//	istep = i;
	if (nCount[istep]<maxPoints)
	{
	countSum +=nCount[istep];
	}
	if (countSum>maxPoints)
	{		
	countSum -=nCount[istep];
	break;
	}
	}

	if(istep>0)
	{
	for (int i=0;i<istep;i++)
	{
	if (datarry.at(i)->count()>0)
	{
	for (int j=0;j<datarry.at(i)->count();j++)
	{
	m_SelectedData.push_back(datarry.at(i)->at(j));
	}
	}			

	}
	}
	*/
	//	return countSum;
	int nCount = 0;

	for (int i=0;i<m_DataArray.count();i++)
	{
		QVector3D temp = m_DataArray.at(i);
		float tempz = m_DataArray.at(i).z();

		double tempdis = Get_Distance(x,y,temp.x(),temp.y());

		if (tempdis<=Radius && tempz!=m_IDWParam.fUndef && m_SelectedData.count()<maxPoints) 
		{
			m_SelectedData.push_back(temp);
			nCount++;
		}				
	}	
	return nCount;
}

bool InverseDistanceInterpolation::Get_Selected_Point(int i, double &x, double &y, double &z) const 
{	
	if (m_SelectedData.count()>0 && i>=0 && i<m_SelectedData.count())
	{
		x	= m_SelectedData.at(i).x();
		y	= m_SelectedData.at(i).y();
		z	= m_SelectedData.at(i).z();			
		return true;	
	}
	else 
		return  false;
}

void  InverseDistanceInterpolation::SetIDWParameters(int weight ,double power,double bandwidth,int mode,int PointsMax,int Radius,int Xsize,int Ysize,float lonmin,float lonmax,float latmin,float latmax,float undef,QString strSearchRange )
{
	m_IDWParam.weight = weight;
	m_IDWParam.power = power;
	m_IDWParam.bandwidth = bandwidth;
	m_IDWParam.mode = mode;
	m_IDWParam.pointMax = PointsMax;
	m_IDWParam.radius = Radius;
	m_IDWParam.isiz = Xsize;
	m_IDWParam.jsiz = Ysize;
	m_IDWParam.lonmin = lonmin;
	m_IDWParam.lonmax = lonmax;
	m_IDWParam.latmin = latmin;
	m_IDWParam.latmax = latmax;
	m_IDWParam.fUndef = undef;
	m_IDWParam.strange = strSearchRange;
}


void InverseDistanceInterpolation::Execute(CData *pDataArray, double **dArrayValue)
{
	int weight,power,bandwidth,mode,pointMax;
	float radius;
	int isiz,jsiz,inum,stnnum,siz;
	float lonmin,lonmax,latmin,latmax,fUndef;

	// 	double *stnlon,*stnlat;
	// 	float *stnval;
	float lon,lat,val; //经度，纬度，值

	int ix,iy;
	double x,y,z;

	weight = m_IDWParam.weight ;
	power = m_IDWParam.power ;
	bandwidth = m_IDWParam.bandwidth;
	mode = m_IDWParam.mode ;
	pointMax = m_IDWParam.pointMax ;
	radius = m_IDWParam.radius ;
	isiz = m_IDWParam.isiz ;
	jsiz = m_IDWParam.jsiz ;
	lonmin = m_IDWParam.lonmin;
	lonmax = m_IDWParam.lonmax;
	latmin = m_IDWParam.latmin;
	latmax = m_IDWParam.latmax;
	fUndef = m_IDWParam.fUndef;


	stnnum = pDataArray[0].DataBlockDimensionSize(0,0);
	siz = isiz * jsiz;

	float dx = (lonmax-lonmin)/isiz;
	float dy = (latmax-latmin)/jsiz;

	if(*dArrayValue != NULL)
	{
		free(*dArrayValue);
		*dArrayValue = NULL;
	}

	*dArrayValue = (double *)malloc(siz * sizeof(double));
	double* arrayvalue = *dArrayValue ;

	for(inum = 0; inum < stnnum; inum++)
	{
		lon = pDataArray[0].GetDataArray(0)[inum];
		if (lon < 0) 
			lon = lon + 360;
		lat = pDataArray[1].GetDataArray(0)[inum];
		val = pDataArray[2].GetDataArray(0)[inum];

		m_DataArray.push_back(QVector3D(lon,lat,val)); //存入站点数据以及Z值
	}

	for(iy=0, y=latmin; iy<jsiz; iy++, y+=dy)
	{
		for(ix=0, x=lonmin; ix<isiz; ix++, x+=dx)
		{
			if( GetValue(x,y,z))
			{
				*arrayvalue = z;
				arrayvalue++;
			}
			else
			{
				*arrayvalue = fUndef;
				arrayvalue++;
			}
		}
	}
}


void InverseDistanceInterpolation::SetElementInfo(CElement *pElem)
{
	if(pElem == NULL )
	{
		printf("POINTER IS NULL. ERROR.\n");
		return;
	}

	pElem->ResetSelectedLat(m_IDWParam.latmin,m_IDWParam.latmax);
	pElem->ResetSelectedLon(m_IDWParam.lonmin,m_IDWParam.lonmax);
	pElem->SelectedLon().m_count = m_IDWParam.isiz;
	pElem->SelectedLat().m_count = m_IDWParam.jsiz;
	pElem->SelectedLon().m_delt = (m_IDWParam.lonmax - m_IDWParam.lonmin) / (m_IDWParam.isiz - 1);
	pElem->SelectedLat().m_delt = (m_IDWParam.latmax - m_IDWParam.latmin) / (m_IDWParam.jsiz - 1);

	QDateTime date = pElem->ActualPeriod().m_hashDate.value(0);
	pElem->ResetSelectedPeriod(date,date);
	CTimeScale timescale(1,3);
	pElem->SelectedPeriod().SetTimeScale(timescale);     // 设置时间尺度 默认1dy
}

/*
bool InverseDistanceInterpolation::Interpolate()
{
if( On_InitializePoints() )
{
int		ix, iy;
double	x, y, z;

if (m_pGrid!=NULL)
{
for(iy=0, y=m_pGrid->Get_YMin(); iy<m_pGrid->Get_NY(); iy++, y+=m_pGrid->Get_Cellsize())
{
for(ix=0, x=m_pGrid->Get_XMin(); ix<m_pGrid->Get_NX(); ix++, x+=m_pGrid->Get_Cellsize())
{
if( GetValue(x, y, z) )
{
m_pGrid->Set_Value(ix, iy, z);
}
else
{
m_pGrid->Set_NoData(ix, iy);
}
}
}
}		

On_Finalize();

return( true );
}

return( false );
}
*/




///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
