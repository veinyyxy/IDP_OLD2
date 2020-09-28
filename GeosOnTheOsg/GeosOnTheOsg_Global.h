#pragma  once

#ifndef GEOSONTHEOSG_LIB
# define GEOSONTHEOSG_EXPORT __declspec(dllexport)
#else
# define GEOSONTHEOSG_EXPORT __declspec(dllimport)
#endif