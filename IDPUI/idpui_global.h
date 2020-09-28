#ifndef IDPUI_GLOBAL_H
#define IDPUI_GLOBAL_H

#include <QtCore/qglobal.h>
//#include "goConfig.h"

#ifndef IDPUI_LIB
# define IDPUI_EXPORT Q_DECL_EXPORT
#else
# define IDPUI_EXPORT Q_DECL_IMPORT
#endif

#endif // IDPUI_GLOBAL_H
