#pragma once
#include "SymbolLibraryInterface.h"

using namespace goto_gis;

class EarthQuakeSympolLibrary :
	public SymbolLibraryInterface
{
public:
	EarthQuakeSympolLibrary(void);
	~EarthQuakeSympolLibrary(void);

	virtual int GetSymbolColor( double dvalue, osg::Vec4* v4Color );

	virtual int GetSymbolSize( double dvalue, double* size );

	virtual int GetSymbolStyle( double dvalue, osg::Geometry** pGeometry );

	virtual int GetSymbolStyle( double dvalue, int* FontID );

	virtual int GetSymbolStyle( double dvalue, osg::Node** pNode );

	virtual int InitSymbolLibrary( std::string strLibName );
private:
	//osg::Node* m_pNode;
	int m_iLibType;
	std::map< std::pair<double, double>, char > m_CharLibList;
	//std::map< std::pair<double, double>, osg::Geometry*> m_GeomtryLibList;
	std::map<double, osg::Node*> m_NodeLibList;
};

