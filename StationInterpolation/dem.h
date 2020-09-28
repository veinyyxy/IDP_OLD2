///////////////////////////////////////////////////////////
//dem.h
////////////////////////////////////////////////////////////
#ifndef _DEM2_H_
#define _DEM2_H_

#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <assert.h>
#include <memory.h>

//#define USENCC
#ifdef USENCC
namespace NCCGIS{
#endif

typedef unsigned char BYTE;

#define UndefineValue  -99999.000   // no value of grid in DEM 
#define factor_dem         1000         // scale factor_dem of the value of the data
#define MaxDouble      99999999.0;  // max value of double type
#define PI             3.141592653589793 
#define ZERO           0.000000001  

// The rectangle of th objects
struct CBoundBox{
	double xmin, ymin, zmin;
	double xmax, ymax, zmax;
	CBoundBox()
	{
		xmin = ymin =  MaxDouble;
		xmax = ymax = -MaxDouble;
		zmin =         MaxDouble;
		zmax =        -MaxDouble;
	}
	CBoundBox(double x1, double y1, double x2, double y2)
	{
		xmin = x1;
		ymin = y1;
		xmax = x2;
		ymax = y2;
		zmin =  MaxDouble;
		zmax = -MaxDouble;
	}
	CBoundBox(CBoundBox* boundbox)
	{
		xmin = boundbox->xmin;
		ymin = boundbox->ymin;
		xmax = boundbox->xmax;
		ymax = boundbox->ymax;
		zmin = boundbox->zmin;
		zmax = boundbox->zmax;
	}
	CBoundBox(double x1, double y1, double x2, double y2, double z1, double z2)
	{
		xmin = x1;
		ymin = y1;
		xmax = x2;
		ymax = y2;
		zmin = z1;
		zmax = z2;
	}
	CBoundBox& operator =(const CBoundBox& boundbox)
	{
		xmin = boundbox.xmin;
		ymin = boundbox.ymin;
		zmin = boundbox.zmin;
		xmax = boundbox.xmax;
		ymax = boundbox.ymax;
		zmax = boundbox.zmax;
		return *this;
	};
	CBoundBox& operator /(double num)
	{
		xmin /= num;
		ymin /= num;
		zmin /= num;
		xmax /= num;
		ymax /= num;
		zmax /= num;
		return *this;
	};
	CBoundBox& operator *(double num)
	{
		xmin *= num;
		ymin *= num;
		zmin *= num;
		xmax *= num;
		ymax *= num;
		zmax *= num;
		return *this;
	};
};

// 3D point data structure 
struct CPoint3D 
{		
	long id;				//the id of the point.
	double x,y,z;			//the point cordinate.
	
	CPoint3D()
	{
		x = 0;
		y = 0;
		z = 0;	
	};
	
	CPoint3D(double x1,double y1,double z1)
	{
		x = x1;
		y = y1;
		z = z1;
	};

	CPoint3D(long id1, double x1,double y1,double z1)
	{
		id = id1;
		x  = x1;
		y  = y1;
		z  = z1;
	};	

	CPoint3D& operator =(const CPoint3D& point)
	{
		id = point.id;
		x  = point.x;
		y  = point.y;
		z  = point.z;
		return *this;
	};
	
	CPoint3D& operator +=(const CPoint3D& point)
	{
		x += point.x;
		y += point.y;
		z += point.z;
		return *this;
	};
	
	CPoint3D& operator -=(const CPoint3D& point)
	{
		x -= point.x;
		y -= point.y;
		z -= point.z;
		return *this;
	};

	CPoint3D operator*(double num)
	{
		return CPoint3D(x * num, y * num, z * num);
	};
	CPoint3D operator/(double num)
	{
		return CPoint3D(x / num, y / num, z / num);
	};
	
	bool operator == (CPoint3D& point)
	{
		double delta_x, delta_y, delta_z;
		delta_x = x - point.x;
		delta_y = y - point.y;
		delta_z = z - point.z;
		
		if( fabs(delta_x - 0) < ZERO
			&& fabs(delta_y - 0) < ZERO
			&& fabs(delta_z - 0) < ZERO
			)
			return true;
		else
			return false;
	}; 
	
	
	CPoint3D operator + (const CPoint3D& v)
	{
		return CPoint3D((x+v.x), (y+v.y), (z+v.z));
	}; 
	
	CPoint3D operator - (const CPoint3D& v)
	{
		return CPoint3D((x-v.x), (y-v.y), (z-v.z));
	}; 
};

//the delaunay triangle struct.
struct CTriangle
{	
	long id;
	CTriangle *tri[3];				//the three triangles which border the triangle
	CPoint3D  *pt[3];               //the three points which consist of the triangle 
	
	CTriangle()
	{
		id = -1;
		pt[0]  = pt[1]  = pt[2]  = NULL;
		tri[0] = tri[1] = tri[2] = NULL;
	};

	CTriangle(CTriangle *pTri)
	{
		id = pTri->id;
		for (BYTE i = 0; i < 3; i++)
		{
			pt[i]  = pTri->pt[i];
			tri[i] = pTri->tri[i];
		}
	};
};



//2D Point;
struct CPoint2D
{
	double x, y;
	CPoint2D()
	{
		x = y = - MaxDouble;
	};
	CPoint2D(double xx, double yy)
	{
		x = xx;
		y = yy;
	};
	bool operator == (CPoint2D point)
	{
		if( fabs(x - point.x) <= ZERO && fabs(y - point.y) <= ZERO)
			return true;
		else
			return false;
	};
	bool operator != (CPoint2D point)
	{
		if( fabs(x - point.x) < ZERO && fabs(y - point.y) < ZERO)
			return false;
		else
			return true;
	};
};

// the structrue of th contour
struct CContour
{
	long nPointCount;
	double z;
	CPoint2D *pPoints;
	CContour()
	{
		z           = - MaxDouble;
		pPoints     = NULL;
		nPointCount = 0;
		
	};
};

//the constrained line structrue.
struct CConstrainLine
{		
	long frompt;      // The first number of the point
	long endpt;		  // The last   numbef ot the point
};

//the triangle used for the io process.
struct CIOTriangle
{
	long id;
	long tri[3];
	long pt[3];

	CIOTriangle()
	{
		id    = tri[0] = tri[1] = tri[2] = -1;
		pt[0] = pt[1]  = pt[2]  = -1;
	}

	CIOTriangle(CTriangle* pTri)
	{
		id = pTri->id;
		for (BYTE i = 0; i < 3; i++)
		{
			pt[i] = pTri->pt[i]->id;
			if (pTri->tri[i])
				
				tri[i] = pTri->tri[i]->id;
			else
				tri[i] = -1;
		}
	}
};



// The points with constrain lines 
struct CBlockPoint
{
	long lBlockID;
	CBoundBox oBoundBox;
	long lPointCount;
	CPoint3D *pPoints;
	long lConstrainLineCount;
	CConstrainLine *pConstrainLines;
	
	CBlockPoint()
	{
		lBlockID            = 0;
		lPointCount         = 0;
		lConstrainLineCount = 0;
		pPoints             = NULL;
		pConstrainLines     = NULL;
	};

	CBlockPoint(CBoundBox BoundBox, long PointCount, CPoint3D *Points, long ConstrainLineCount, CConstrainLine *Lines)
	{
		lBlockID    = 0;
		oBoundBox   = BoundBox;
		lPointCount = PointCount;
		pPoints     = Points;
		
		pConstrainLines     = Lines;
		lConstrainLineCount = ConstrainLineCount;
	};

	~CBlockPoint()
	{
		if(pPoints)
		{
			free(pPoints);
			pPoints = NULL;
		}

		if(pConstrainLines)
		{
			free(pConstrainLines);
			pConstrainLines = NULL;
		}
	}

};


// the TIN structrue  
struct CBlockTIN
{
	long        lBlockID;					//\x{5757}\x{7D22}\x{5F15}\x{53F7}\x{FF1B}
	CBoundBox   oBoundBox;			//\x{5916}\x{5305}\x{6846}\x{FF1B}
	long        lPointCount;				//\x{70B9}\x{6570}\x{FF1B}
	CPoint3D    *pPoints;				//\x{70B9}\x{6570}\x{7EC4}\x{FF1B}
	long        lTriangleCount;			//\x{4E09}\x{89D2}\x{5F62}\x{6570}\x{FF1B}
	CIOTriangle *pTriangles;			//\x{4E09}\x{89D2}\x{5F62}\x{6570}\x{7EC4}\x{FF1B}
	CBlockTIN()
	{
		lBlockID        = 0;
		lPointCount     = 0;
		lTriangleCount  = 0;
		pPoints         = NULL;
		pTriangles      = NULL;
	};

	~CBlockTIN()
	{
		if (pPoints)
		{
			free(pPoints);
			pPoints = NULL;
		}
		if (pTriangles)
		{
			free(pTriangles);
			pTriangles = NULL;
		}
	};
};

// the Grid structure 
struct CBlockGrid
{
	long        lBlockID;			//
	CBoundBox   oBoundBox;	//
	long        lCellColCount;		//
	long        lCellRowCount;		//
	double      dCellSize;		//
	double      dNoDataValue;	//
	double      *dCellValues;

	CBlockGrid()
	{
		lBlockID      = 0;
		lCellColCount = lCellRowCount = 0;
		dCellSize     = dNoDataValue  = 0;
		dCellValues   = NULL;
	};

	CBlockGrid(long CellColCount, long CellRowCount, double CellSize, double NoDataValue, CBoundBox BoundBox)
	{
		lCellColCount = CellColCount;
		lCellRowCount = CellRowCount;
		dCellSize     = CellSize;
		dNoDataValue  = NoDataValue;
		oBoundBox     = BoundBox;
		dCellValues = NULL;
	};

	~CBlockGrid()
	{
		if(dCellValues)
		{
			free(dCellValues);
			dCellValues = NULL;
		}
	}

};

//The Contour structrue
struct CBlockContour
{
	CBoundBox oBoundBox;
	long      lPointCount;
	long      lContourCount;
	long      lConstrainLineCount;
	
	CPoint3D       *pPoints;
	CContour       *pContours;
	CConstrainLine *pConstrainLines;
	CBlockContour()
	{
		lPointCount = lContourCount = lConstrainLineCount = 0;
		pPoints     = NULL;
		pContours   = NULL;
		pConstrainLines = NULL;
	};

	CBlockContour(CBoundBox BoundBox, long PointCount, CPoint3D* Points, long ContourCount, CContour* Contours, long LineCount, CConstrainLine* Lines)
	{
		oBoundBox            = BoundBox;
		lPointCount          = PointCount;
		lContourCount        = ContourCount;
		lConstrainLineCount  = LineCount;
		pPoints              = Points;
		pContours            = Contours;
		pConstrainLines      = Lines;
	};
		
	~CBlockContour()
	{
		if(pPoints)
		{
			free(pPoints);
			pPoints = NULL;
		}
		if(pContours)
		{
			for (long i = 0; i < lContourCount; i++)
			{
				if(pContours[i].pPoints)
				{
					free(pContours[i].pPoints);
					pContours[i].pPoints = NULL;
				}
			}
			free(pContours);
			pContours = NULL;
		}
		if(pConstrainLines)
		{
			free(pConstrainLines);
			pConstrainLines = NULL;
		}
	}
	
};

//bool PointCreateTIN(CBlockPoint * pBlockPoint, CBlockTIN *pBlockTIN);
//bool ContourCreateTIN(CBlockContour* pBlockContour, CBlockTIN* pBlockTIN);
//bool TINCreateGrid(CBlockTIN *pBlockIOTIN, CBlockGrid *pBlockGrid);
//bool TINCreateGrid(CBlockTIN *pBlockTIN, CBlockGrid *pBlockGrid, double SmoothWidth);
//bool PointCreateGrid(CBlockPoint * pBlockPoint, CBlockGrid *pBlockGrid, bool m_bLimitSearchScal , long MAXSCALE, int m_nSample);

#ifdef USENCC
}
#endif

#endif
