#ifndef TMUI_GLOBAL_H
#define TMUI_GLOBAL_H

#include <QtCore/qglobal.h>

#ifndef TMUI_LIB
# define TMUI_EXPORT Q_DECL_EXPORT
#else
# define TMUI_EXPORT Q_DECL_IMPORT
#endif

#endif // TMUI_GLOBAL_H
