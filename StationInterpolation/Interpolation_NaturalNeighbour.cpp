/**********************************************************
 * Version $Id: Interpolation_NaturalNeighbour.cpp 911 2011-02-14 16:38:15Z reklov_w $
 *********************************************************/

///////////////////////////////////////////////////////////
//                                                       //
//                         SAGA                          //
//                                                       //
//      System for Automated Geoscientific Analyses      //
//                                                       //
//                    Module Library:                    //
//                     Grid_Gridding                     //
//                                                       //
//-------------------------------------------------------//
//                                                       //
//          Interpolation_NaturalNeighbour.cpp           //
//                                                       //
//                 Copyright (C) 2003 by                 //
//                      Olaf Conrad                      //
//                                                       //
//-------------------------------------------------------//
//                                                       //
// This file is part of 'SAGA - System for Automated     //
// Geoscientific Analyses'. SAGA is free software; you   //
// can redistribute it and/or modify it under the terms  //
// of the GNU General Public License as published by the //
// Free Software Foundation; version 2 of the License.   //
//                                                       //
// SAGA is distributed in the hope that it will be       //
// useful, but WITHOUT ANY WARRANTY; without even the    //
// implied warranty of MERCHANTABILITY or FITNESS FOR A  //
// PARTICULAR PURPOSE. See the GNU General Public        //
// License for more details.                             //
//                                                       //
// You should have received a copy of the GNU General    //
// Public License along with this program; if not,       //
// write to the Free Software Foundation, Inc.,          //
// 59 Temple Place - Suite 330, Boston, MA 02111-1307,   //
// USA.                                                  //
//                                                       //
//-------------------------------------------------------//
//                                                       //
//    e-mail:     oconrad@saga-gis.org                   //
//                                                       //
//    contact:    Olaf Conrad                            //
//                Institute of Geography                 //
//                University of Goettingen               //
//                Goldschmidtstr. 5                      //
//                37077 Goettingen                       //
//                Germany                                //
//                                                       //
///////////////////////////////////////////////////////////

//---------------------------------------------------------


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
#include "Interpolation_NaturalNeighbour.h"

#include "nn/nn.h"


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
CInterpolation_NaturalNeighbour::CInterpolation_NaturalNeighbour(void)
{
// 	Set_Name		(_TL("Natural Neighbour"));
// 
// 	Set_Author		(SG_T("O. Conrad (c) 2008"));
// 
// 	Set_Description	(_TW(
// 		"Natural Neighbour method for grid interpolation from irregular distributed points.")
// 	);
// 
// 	Parameters.Add_Value(
// 		NULL	, "SIBSON"	, _TL("Sibson"),
// 		_TL(""),
// 		PARAMETER_TYPE_Bool	, true
// 	);
}


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
bool CInterpolation_NaturalNeighbour::Execute(CData *pDataArray, double **dArrayValue)
{
	int		n = 0;
	double		zMin, zMax;
	double lon,lat,val; //经度，纬度，值

	int weight,power,bandwidth,mode,pointMax;
	float radius;
	int isiz,jsiz,inum,stnnum,siz;
	float lonmin,lonmax,latmin,latmax,fUndef;
	int ix,iy;
	double x,y,z;

	isiz = m_NNIParam.isiz ;
	jsiz = m_NNIParam.jsiz ;
	lonmin = m_NNIParam.lonmin;
	lonmax = m_NNIParam.lonmax;
	latmin = m_NNIParam.latmin;
	latmax = m_NNIParam.latmax;
	fUndef = m_NNIParam.fUndef;

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

//	TSG_Point	p;
	
//	nn_rule		= Parameters("SIBSON")->asBool() ? SIBSON : NON_SIBSONIAN;
	stnnum = pDataArray[0].DataBlockDimensionSize(0,0); 

	//-----------------------------------------------------
	nnpoint	*pSrc	= (nnpoint  *)malloc( stnnum * sizeof(nnpoint));
	double	*zSrc	= (double *)malloc(stnnum * sizeof(double));

	for(int i=0; i<stnnum; i++)
	{
		//CSG_Shape	*pShape	= m_pShapes->Get_Shape(i);
		lon = pDataArray[0].GetDataArray(0)[i];
		lat = pDataArray[1].GetDataArray(0)[i];
		val = zSrc[n] = pDataArray[2].GetDataArray(0)[i];

		if( val!=m_NNIParam.fUndef)
		{
			pSrc[n].x	= lon;
			pSrc[n].y	= lat;
			pSrc[n].z	= val;	

			if( n == 0 )
				zMin	= zMax	= pSrc[n].z;
			else if( zMin > pSrc[n].z )
				zMin	= pSrc[n].z;
			else if( zMax < pSrc[n].z )
				zMax	= pSrc[n].z;

			n++;
		}
	}

//	Process_Set_Text(_TL("triangulating"));
	delaunay	*pTIN	= delaunay_build(n, pSrc, 0, NULL, 0, NULL);

	if (!pTIN)
	{
		//printf(QObject::tr("构建三角网失败!"));
		return false;
	}
	
	//-----------------------------------------------------
	double	*xDst	= (double *)malloc(siz * sizeof(double));
	double	*yDst	= (double *)malloc(siz * sizeof(double));
	double	*zDst	= (double *)malloc(siz * sizeof(double));

	for(iy=0,y=latmin; iy<jsiz; iy++, y+=dy)
	{
		for(ix=0, x=lonmin; ix<isiz; ix++, x+=dx)
		{		
			xDst[ix+iy*jsiz]	= x;
			yDst[ix+iy*jsiz]	= y;
		//	zDst[ix+iy*jsiz]	= NaN;	
			zDst[ix+iy*jsiz]	= 0;	
		}	
	}	

	//-----------------------------------------------------
//	Process_Set_Text(_TL("creating interpolator"));
	nnai	*pNN	= nnai_build(pTIN, siz, xDst, yDst);

 //   Process_Set_Text(_TL("interpolating"));
    nnai_interpolate(pNN, zSrc, zDst);

	//-----------------------------------------------------
	for(int y=0; y<jsiz ; y++)
	{
		for(int x=0; x< isiz; x++)
		{
			double	z	= zDst[y*jsiz+x];

			if( zMin <= z && z <= zMax )
		//	if(z!=fUndef)
			{
				//m_pGrid->Set_Value(x, y, z);
				*arrayvalue = z;	
			//	*arrayvalue = 200;	
			}
			else
			{
				*arrayvalue = fUndef;
			//	*arrayvalue = 100;
			}
			arrayvalue++;
		}
	}

	//-----------------------------------------------------
	nnai_destroy(pNN);

	delaunay_destroy(pTIN);
	//*/
	//-----------------------------------------------------
	return( true );
}

void CInterpolation_NaturalNeighbour::SetNNIParameters(int Xsize,int Ysize,float lonmin,float lonmax,float latmin,float latmax,float undef)
{	
	m_NNIParam.isiz = Xsize;
	m_NNIParam.jsiz = Ysize;
	m_NNIParam.lonmin = lonmin;
	m_NNIParam.lonmax = lonmax;
	m_NNIParam.latmin = latmin;
	m_NNIParam.latmax = latmax;
	m_NNIParam.fUndef = undef;
}

void CInterpolation_NaturalNeighbour::SetElementInfo(CElement *pElem)
{
	if(pElem == NULL )
	{
		printf("POINTER IS NULL. ERROR.\n");
		return;
	}

	pElem->ResetSelectedLat(m_NNIParam.latmin,m_NNIParam.latmax);
	pElem->ResetSelectedLon(m_NNIParam.lonmin,m_NNIParam.lonmax);
	pElem->SelectedLon().m_count = m_NNIParam.isiz;
	pElem->SelectedLat().m_count = m_NNIParam.jsiz;
	pElem->SelectedLon().m_delt = (m_NNIParam.lonmax - m_NNIParam.lonmin) / (m_NNIParam.isiz - 1);
	pElem->SelectedLat().m_delt = (m_NNIParam.latmax - m_NNIParam.latmin) / (m_NNIParam.jsiz - 1);

	QDateTime date = pElem->ActualPeriod().m_hashDate.value(0);
	pElem->ResetSelectedPeriod(date,date);
	CTimeScale timescale(1,3);
	pElem->SelectedPeriod().SetTimeScale(timescale);     // 设置时间尺度 默认1dy
}

///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
