#include "Interpolation_Triangle.h"


InterpolationTriangle::InterpolationTriangle(void)
{
	m_PolyNum = 0;
	m_pPolygonTin = NULL;
	m_z = 0;
	m_color = QVector4D(0,0,0,0);
}

InterpolationTriangle::~InterpolationTriangle(void)
{
}

bool InterpolationTriangle::Execute(XFILLPOINT *pfillpoint)
{
	CBlockTIN pBlockTIN;
	CBlockContour pBlockContour;
	ClearArray();
	if (!GetContourFromFillPoint(pfillpoint,&pBlockContour)) return false;
	if (!m_tin.Delaunay(&pBlockContour,&pBlockTIN)) return false;
	if (!GetGISPolyFromTIN(&pBlockTIN)) return false;
}

bool InterpolationTriangle::Execute( goPolygon *pGISPolygon)
{
	CBlockTIN pBlockTIN;
	CBlockContour pBlockContour;
	ClearArray();
	if (!GetContourFromGISPoly(pGISPolygon,&pBlockContour)) return false;
	if (!m_tin.Delaunay(&pBlockContour,&pBlockTIN)) return false;
	if (!GetGISPolyFromTIN(&pBlockTIN)) return false;
}

bool InterpolationTriangle::GetGISPolyFromTIN(CBlockTIN* pBlockTIN)
{
	//if (pBlockTIN == NULL) return false;

	//int i;
	//m_PolyNum = pBlockTIN->lTriangleCount;
	//m_pPolygonTin = new goPolygon[pBlockTIN->lTriangleCount];
	//for (i=0; i < pBlockTIN->lTriangleCount; i++)
	//{
	//	long p0Numb=pBlockTIN->pTriangles[i].pt[0];
	//	long p1Numb=pBlockTIN->pTriangles[i].pt[1];
	//	long p2Numb=pBlockTIN->pTriangles[i].pt[2];

	//	QVector3DArray* pArray = (m_pPolygonTin + i)->getVectexArray();
	//	QVector4DArray* pColor = (m_pPolygonTin + i)->getColorArray();

	//	pArray->push_back(QVector3D(pBlockTIN->pPoints[p0Numb].x,pBlockTIN->pPoints[p0Numb].y,m_z));
	//	pColor->push_back(m_color);

	//	pArray->push_back(QVector3D(pBlockTIN->pPoints[p1Numb].x,pBlockTIN->pPoints[p1Numb].y,m_z));
	//	pColor->push_back(m_color);

	//	pArray->push_back(QVector3D(pBlockTIN->pPoints[p2Numb].x,pBlockTIN->pPoints[p2Numb].y,m_z));
	//	pColor->push_back(m_color);
	//	
	//	(m_pPolygonTin + i)->SetBakData();
	//	(m_pPolygonTin + i)->SetColorArray();
	//}
	return true;
}

bool InterpolationTriangle::GetContourFromFillPoint(XFILLPOINT *pfillpoint, CBlockContour *pBlockContour)
{
	if (pfillpoint == NULL || pBlockContour == NULL) return false;

	long j;
	pBlockContour->lContourCount=1;

	pBlockContour->oBoundBox.xmin=0.0;
	pBlockContour->oBoundBox.ymin=-90.0;
	pBlockContour->oBoundBox.zmin=-9999.0;

	pBlockContour->oBoundBox.xmax=360.0;
	pBlockContour->oBoundBox.ymax=90.0;
	pBlockContour->oBoundBox.zmax=9999.0;

	//get the contours
	CContour* pContour;
	if (pfillpoint->n > 0)
	{
		pBlockContour->pContours = (CContour*)calloc(1, sizeof(CContour));

		//the no.i contour
		pContour = &pBlockContour->pContours[0];
		pContour->nPointCount=pfillpoint->n;
		pContour->z=pfillpoint->fShadeValue;
		pContour->pPoints = (CPoint2D*)calloc(pContour->nPointCount, sizeof(CPoint2D));
		for (j = 0; j < pContour->nPointCount; j++)
		{
			pContour->pPoints[j].x=pfillpoint->pPoint[j].x;
			pContour->pPoints[j].y=pfillpoint->pPoint[j].y;
		}
	}
	return true;
}

bool InterpolationTriangle::GetContourFromGISPoly( goPolygon *pGISPolygon, CBlockContour *pBlockContour )
{
	//if (pGISPolygon == NULL || pBlockContour == NULL) return false;

	//long i;
	//pBlockContour->lContourCount=1;

	//pBlockContour->oBoundBox.xmin=0.0;
	//pBlockContour->oBoundBox.ymin=-90.0;
	//pBlockContour->oBoundBox.zmin=-9999.0;

	//pBlockContour->oBoundBox.xmax=360.0;
	//pBlockContour->oBoundBox.ymax=90.0;
	//pBlockContour->oBoundBox.zmax=9999.0;

	////get the contours
	//CContour* pContour;
	//if (pGISPolygon->getVectexArray()->count() > 0)
	//{
	//	pBlockContour->pContours = (CContour*)calloc(1, sizeof(CContour));
	//	pContour = &pBlockContour->pContours[0];
	//	pContour->nPointCount=pGISPolygon->getVectexArray()->count();
	//	pContour->z=0;
	//	pContour->pPoints = (CPoint2D*)calloc(pContour->nPointCount, sizeof(CPoint2D));
	//	for (i = 0; i < pContour->nPointCount; i++)
	//	{
	//		pContour->pPoints[i].x=pGISPolygon->getVectexArray()->at(i).x();
	//		pContour->pPoints[i].y=pGISPolygon->getVectexArray()->at(i).y();
	//	}
	//}
	return true;
}

void InterpolationTriangle::ClearArray()
{
	if (m_pPolygonTin != NULL)
	{
		//delete[] m_pPolygonTin;
		m_pPolygonTin = NULL;
	}
}
