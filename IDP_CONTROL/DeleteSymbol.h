#pragma once
#include "idp_control_global.h"
#include "idpdrawgeometrydata.h"
class IDP_CONTROL_EXPORT DeleteSymbol :
	public IDPDrawGeometryData
{
public:
	DeleteSymbol(void);
	virtual ~DeleteSymbol(void);

	virtual bool handle( const osg::Vec3& pos, const osgGA::GUIEventAdapter& ea );
	inline void DeleteSymbolByName(const std::string& strN){m_strName = strN;}
	inline void DeleteSymbolByAddress(const osg::Drawable* pD){m_pDelObject = const_cast<osg::Drawable*>(pD);}
private:
	std::string m_strName;
	osg::Drawable* m_pDelObject;
};

