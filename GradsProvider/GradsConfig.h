#pragma once
#include "QT_Include.h"

#ifndef Q_WS_WIN

#else

#endif

#ifndef GRADS_IMPORT
#define GRADS_EXPORT_IMPORT Q_DECL_EXPORT
#else
#define GRADS_EXPORT_IMPORT Q_DECL_IMPORT
#endif