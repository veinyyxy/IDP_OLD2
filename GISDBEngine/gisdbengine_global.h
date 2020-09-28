#ifndef GISDBENGINE_GLOBAL_H
#define GISDBENGINE_GLOBAL_H

#include <QtCore/qglobal.h>

#ifndef GISDBENGINE_LIB
# define GISDBENGINE_EXPORT Q_DECL_EXPORT //__declspec(dllexport)
#else
# define GISDBENGINE_EXPORT Q_DECL_IMPORT //__declspec(dllimport)
#endif

#endif // GISDBENGINE_GLOBAL_H
