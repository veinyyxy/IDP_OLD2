#ifndef IDP_CONTROL_GLOBAL_H
#define IDP_CONTROL_GLOBAL_H

#include <QtCore/qglobal.h>

#ifndef IDP_CONTROL_LIB
# define IDP_CONTROL_EXPORT Q_DECL_EXPORT
#else
# define IDP_CONTROL_EXPORT Q_DECL_IMPORT
#endif

#endif // IDP_CONTROL_GLOBAL_H
