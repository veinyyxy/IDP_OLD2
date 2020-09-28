#pragma once
#include "idpdrawgeometrydata.h"
#include "idp_control_global.h"
#include <osg/AutoTransform>

class IDP_CONTROL_EXPORT MoveSymbol :
	public IDPDrawGeometryData
{
public:
	MoveSymbol(void);
	virtual ~MoveSymbol(void);
	inline void MoveSymbolByName(const std::string& strN){m_strName = strN;}
	virtual bool handle( const osg::Vec3& pos, const osgGA::GUIEventAdapter& ea );

private:
	std::string m_strName;
	osg::AutoTransform* m_pLockNode;
	bool m_bMoveLockNode;
};

