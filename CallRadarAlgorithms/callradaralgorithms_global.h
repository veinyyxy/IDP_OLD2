#ifndef CALLRADARALGORITHMS_GLOBAL_H
#define CALLRADARALGORITHMS_GLOBAL_H

#include <QtCore/qglobal.h>

#ifdef CALLRADARALGORITHMS_LIB
# define CALLRADARALGORITHMS_EXPORT Q_DECL_EXPORT
#else
# define CALLRADARALGORITHMS_EXPORT Q_DECL_IMPORT
#endif

#endif // CALLRADARALGORITHMS_GLOBAL_H