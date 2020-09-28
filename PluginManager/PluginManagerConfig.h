#pragma once
#include "QtCore/QtPlugin"
#include "QtCore/QLibrary"
#include "QtCore/QDir"
#include "QtCore/QMap"

#ifndef PLUGIN_MANAGER_IMPORT
#define PLUGIN_MANAGER_IMPORT_EXPORT Q_DECL_EXPORT
#else
#define PLUGIN_MANAGER_IMPORT_EXPORT Q_DECL_IMPORT
#endif