#pragma once

#ifndef Q_WS_WIN

#else

#endif

#ifndef QE_LIB_IMPORT
#define XML_EXPORT_IMPORT Q_DECL_EXPORT
#else
#define XML_EXPORT_IMPORT Q_DECL_IMPORT
#endif