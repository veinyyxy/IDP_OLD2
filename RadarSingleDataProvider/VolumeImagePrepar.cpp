#include "VolumeImagePrepar.h"



VolumeImagePrepar::VolumeImagePrepar(void)
{
	interpol = new InterpolationAlgorithm;
}


VolumeImagePrepar::~VolumeImagePrepar(void)
{
}

void VolumeImagePrepar::GetImages( QVector<QImage>* imageList )
{
	interpol->DrawImge(imageList,m_hColorTableHash);
}

void VolumeImagePrepar::Cutting( osg::Vec3& p1, osg::Vec3& p2, osg::Vec3& p3, osg::Vec3& p4, osg::Node* pClipNode,goto_gis::CoordinateTransform* proj )
{
	interpol->Cutting(p1,p2,p3,p4,pClipNode,m_hColorTableHash,proj);
}

