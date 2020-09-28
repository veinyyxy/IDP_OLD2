#pragma once
#include "SymbolLibraryInterface.h"

class SymbolLibraryInterfaceImp : public goto_gis::SymbolLibraryInterface
{
public:
	SymbolLibraryInterfaceImp(void);
	virtual ~SymbolLibraryInterfaceImp(void);

	virtual int InitSymbolLibrary( std::string strLibName );

};

