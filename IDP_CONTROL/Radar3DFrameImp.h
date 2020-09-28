#pragma once
#include "animaticframe.h"

class Map;

class Radar3DFrameImp :
	public AnimaticFrame
{
public:
	Radar3DFrameImp(void);
	virtual ~Radar3DFrameImp(void);
	virtual void Play();
	inline void setAnimaticSwitch(osg::Switch* pM){m_pAnimaticFrameGroup = pM;}
	inline void setFrameName(const QString strName){m_FrameName = strName;}
	inline void setFrameID(unsigned int ifd){m_FrameID = ifd;}

private:
	osg::Switch* m_pAnimaticFrameGroup;
	QString m_FrameName;
	unsigned int m_FrameID; 
};

