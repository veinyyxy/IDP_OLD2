#pragma once
#include "VolumeImage.h"
#include "../GDataTypes/cDrawProduct.h"
#include "InterpolationAlgorithm.h"
class VolumeImagePrepar :
	public VolumeImage
{
public:
	VolumeImagePrepar(void);
	~VolumeImagePrepar(void);
	virtual void GetImages(QVector<QImage>* imageList);
	virtual void Cutting(osg::Vec3& p1, osg::Vec3& p2, osg::Vec3& p3, osg::Vec3& p4, osg::Node* pClipNode,goto_gis::CoordinateTransform* proj);
	inline void SetDrawProdunt(cDrawProduct * pDP){interpol->setcDrawProduct(pDP);m_produceDraw = pDP;}
	//inline void SetColorTable(P_COLORTABLE_VECTOR color){m_ColorTable = color;};
	inline void SetColorHash(QHash<int,QVector4D> &colorhash){m_hColorTableHash = colorhash;};
private:
	cDrawProduct *m_produceDraw;
	//P_COLORTABLE_VECTOR m_ColorTable;
	QHash<int,QVector4D>        m_hColorTableHash;
	InterpolationAlgorithm  *interpol;


};

