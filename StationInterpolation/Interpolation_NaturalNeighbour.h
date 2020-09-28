/**********************************************************
 * Version $Id: Interpolation_NaturalNeighbour.h 911 2011-02-14 16:38:15Z reklov_w $
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
//           Interpolation_NaturalNeighbour.h            //
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
//			Interpolation_NaturalNeighbour.h			 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
#ifndef HEADER_INCLUDED__Interpolation_NaturalNeighbour_H
#define HEADER_INCLUDED__Interpolation_NaturalNeighbour_H


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
//#include "Interpolation.h"
#include "stationinterpolation_global.h"
#include "QT_Include.h"
#include "Data.h"
#include "CElement.h"

///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------

typedef struct NNIParam
{
	int isiz;			// 列数 --lon
	int jsiz;			// 行数-- lat
	float lonmin;		// 经度最小值
	float lonmax;		// 经度最大值
	float latmin;		// 纬度最小值
	float latmax;		// 纬度最大值
	float fUndef;		// 无效值

	NNIParam():isiz(141),jsiz(81),lonmin(70),lonmax(140),latmin(15),latmax(55),fUndef(-999.0)
	{		

	}
}NNIParam;

class STATIONINTERPOLATION_EXPORT CInterpolation_NaturalNeighbour 
{
public:
	CInterpolation_NaturalNeighbour(void);

	void   SetNNIParameters(int Xsize,int Ysize,float lonmin,float lonmax,float latmin,float latmax,float undef);
	void   SetMissingValue(double missValue) {m_NNIParam.fUndef = missValue;}
	void   SetElementInfo(CElement *pElem);
	virtual bool Execute(CData *pDataArray, double **dArrayValue);

private:
	NNIParam m_NNIParam;
};


///////////////////////////////////////////////////////////
//														 //
//														 //
//														 //
///////////////////////////////////////////////////////////

//---------------------------------------------------------
#endif // #ifndef HEADER_INCLUDED__Interpolation_NaturalNeighbour_H
