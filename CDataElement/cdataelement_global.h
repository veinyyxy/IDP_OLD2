#ifndef CDATAELEMENT_GLOBAL_H
#define CDATAELEMENT_GLOBAL_H

#include <QtCore/qglobal.h>

#ifdef CDATAELEMENT_LIB
# define CDATAELEMENT_EXPORT Q_DECL_EXPORT
#else
# define CDATAELEMENT_EXPORT Q_DECL_IMPORT
#endif

#endif // CDATAELEMENT_GLOBAL_H