#pragma once

#ifndef Q_WS_WIN

#else

#endif

#ifndef GRADS_IMPORT
#define Radar_EXPORT_IMPORT Q_DECL_EXPORT
#else
#define Radar_EXPORT_IMPORT Q_DECL_IMPORT
#endif