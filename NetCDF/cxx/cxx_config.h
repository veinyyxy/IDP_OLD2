#ifndef CXX_CONFIG_H_
#define CXX_CONFIG_H_

#include <QtCore/qglobal.h>

#ifdef CXX_EXPORT
	#define CXX_API  Q_DECL_EXPORT
#else
	#define CXX_API  Q_DECL_IMPORT
#endif

#endif //CXX_CONFIG_H_