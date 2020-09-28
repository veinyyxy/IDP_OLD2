#include "InterfaceFunction.h"
#include "ExtendMapFactory.h"

extern "C" __declspec( dllexport ) void* GetGisExtendFactory()
{
	return ExtendMapFactory::Instantiate();
}
