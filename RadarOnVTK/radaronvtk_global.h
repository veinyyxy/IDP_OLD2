#ifndef RADARONVTK_GLOBAL_H
#define RADARONVTK_GLOBAL_H

#include <QtCore/qglobal.h>

#ifdef RADARONVTK_LIB
# define RADARONVTK_EXPORT Q_DECL_EXPORT
#else
# define RADARONVTK_EXPORT Q_DECL_IMPORT
#endif

#endif // RADARONVTK_GLOBAL_H
