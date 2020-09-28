#pragma once
#include <QtGui/QIMage>

class DrawLrmImage
{
public:
	virtual void SetPoints(float x1, float y1, float x2, float y2, float fx1, float fy1, float fx2, float fy2) = 0;
	/*
		�������������������úò���������������
	*/
	virtual void GetImages(QVector<QImage>* imageList) = 0;
	virtual void SetLayerList(const QStringList& strlist) = 0;
};

