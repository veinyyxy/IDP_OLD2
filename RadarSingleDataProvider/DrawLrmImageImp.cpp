#include "DrawLrmImageImp.h"


DrawLrmImageImp::DrawLrmImageImp(LrmAlg* pLA) : m_LrmAlg(pLA)
{
}


DrawLrmImageImp::~DrawLrmImageImp(void)
{
}

void DrawLrmImageImp::SetPoints( float x1, float y1, float x2, float y2, float fx1, float fy1, float fx2, float fy2 )
{
	if(m_LrmAlg)
		m_LrmAlg->SetRectPoint(x1, y1, x2, y2, fx1, fy1, fx2, fy2);
}

void DrawLrmImageImp::GetImages( QVector<QImage>* imageList )
{
	if(m_LrmAlg)
	{
		m_LrmAlg->SetImageList(imageList);
		m_LrmAlg->Execute();
	}
}

void DrawLrmImageImp::SetLayerList( const QStringList& strlist )
{
	if(m_LrmAlg) m_LrmAlg->SetLayerList(strlist);
}
