#pragma once
#include <QtGui/QIMage>

class DrawLrmImage
{
public:
	virtual void SetPoints(float x1, float y1, float x2, float y2, float fx1, float fy1, float fx2, float fy2) = 0;
	/*
		调用另外两个函数设置好参数后调用这个函数
	*/
	virtual void GetImages(QVector<QImage>* imageList) = 0;
	virtual void SetLayerList(const QStringList& strlist) = 0;
};

