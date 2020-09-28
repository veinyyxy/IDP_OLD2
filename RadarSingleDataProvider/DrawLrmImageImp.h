#pragma once
#include "drawlrmimage.h"
#include "LrmAlg.h"

class DrawLrmImageImp :
	public DrawLrmImage
{
public:
	DrawLrmImageImp(LrmAlg* pLA);
	virtual ~DrawLrmImageImp(void);

	virtual void SetPoints( float x1, float y1, float x2, float y2, float fx1, float fy1, float fx2, float fy2 );

	virtual void GetImages( QVector<QImage>* imageList );

	virtual void SetLayerList( const QStringList& strlist );

private:
	LrmAlg* m_LrmAlg;
};

