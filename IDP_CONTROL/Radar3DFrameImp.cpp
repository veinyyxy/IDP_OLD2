#include <QtCore/QString>
#include <osg/Switch>

#include "Radar3DFrameImp.h"


Radar3DFrameImp::Radar3DFrameImp(void)
{
}


Radar3DFrameImp::~Radar3DFrameImp(void)
{
}

void Radar3DFrameImp::Play()
{
	if(!m_pAnimaticFrameGroup) return;

	//m_pAnimaticFrameGroup->setValue(m_FrameID + 1, true);
	m_pAnimaticFrameGroup->setAllChildrenOff();
	m_pAnimaticFrameGroup->setValue(m_FrameID, true);

	return;
}
