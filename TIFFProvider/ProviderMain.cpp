#include "QtCore/QLibrary"
#include "TIFFProvider.h"

TIFFProvider* tiffprovider = 0;

extern "C"
{
	goto_gis::DataProvider Q_DECL_EXPORT * DataProviderPluginMain()
	{

		tiffprovider = new TIFFProvider;
		return tiffprovider;

	}
}