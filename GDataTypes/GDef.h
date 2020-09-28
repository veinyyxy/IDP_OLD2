//////////////////////////////////////////////////////////////////////////
// GDef.h
// created by ply 20080314
//////////////////////////////////////////////////////////////////////////

#pragma once

#ifndef GDEF_H
#define	GDEF_H

#if defined (WIN32)|| defined(WIN64)

#else
#endif


//s8bit
typedef char gChar;
typedef signed char gInt8;
typedef unsigned char gUint8;
typedef gUint8 gByte;
//f8bit
//s16bit
typedef signed short gInt16;
typedef unsigned short gUint16;
typedef gUint16 gWord;
typedef wchar_t gWchar ;
//f16bit

/*
  things are getting more interesting with ints, longs and pointers

  there are several different standard data models described by this table:

  +-----------+----------------------------+
  |type\model | LP64 ILP64 LLP64 ILP32 LP32|
  +-----------+----------------------------+
  |char       |  8     8     8     8     8 |
  |short      | 16    16    16    16    16 |
  |int        | 32    64    32    32    16 |
  |long       | 64    64    32    32    32 |
  |long long  |             64             |
  |void *     | 64    64    64    32    32 |
  +-----------+----------------------------+

  Win16 used LP32 (but we don't support it any longer), Win32 obviously used
  ILP32 and Win64 uses LLP64 (a.k.a. P64)

  Under Unix LP64 is the most widely used (the only I've ever seen, in fact)


  //these words was in wxWidgets --ply
 */
//s32bit 64bit
#if defined(WIN32) || defined(WIN64)
	typedef __int32 gInt32;
	typedef unsigned __int32 gUint32;
	typedef __int64 gInt64;
	typedef unsigned __int64 gUint64;
#elif defined(__LP64__)
	typedef int gInt32;
	typedef unsigned int gUint32;
	typedef long int gInt64;
	typedef unsigned long int gUint64;
#else //linux 32
	typedef int gInt32;
	typedef unsigned int gUint32;
	typedef long long gInt64;
	typedef unsigned long long gUint64;
#endif
//f32bit 64bit

typedef float gFloat;
typedef double gDouble;

#if defined (WIN32)|| defined(WIN64)
	#define		STDCALL_GSYS		__stdcall
	#define		CDECL_GSYS			__cdecl
	#define		FASTCALL_GSYS		__fastcall
	#define		DLLEXP_GSYS			__declspec(dllexport)
#else
	#define		STDCALL_GSYS		__attribute__((stdcall))
	#define		CDECL_GSYS			__attribute__((cdecl))
	#define		FASTCALL_GSYS		__attribute__((fastcall))
	#define		DLLEXP_GSYS			__attribute__ ((visibility("default")))		//gcc 编译增加选项-fvisibility=hidden
#endif


#ifndef NULL
	#ifdef __cplusplus
		#define NULL    0
	#else
		#define NULL    ((void *)0)
	#endif
#endif

inline gInt8 fabs(gInt8 &_X)
{return (_X>0?_X:0-_X); }
inline gInt16 fabs(gInt16 &_X)
{return (_X>0?_X:0-_X); }
inline gInt32 fabs(gInt32 &_X)
{return (_X>0?_X:0-_X); }
inline gInt64 fabs(gInt64 &_X)
{return (_X>0?_X:0-_X); }


#if defined (WIN32)|| defined(WIN64)
#define GfOpen(fHandle,fName,fMode) fopen_s(&fHandle,fName,fMode)
#else
#define GfOpen(fHandle,fName,fMode) fHandle=fopen(fName,fMode)
#endif

#if defined (WIN32)|| defined(WIN64)
#define Gsscanf sscanf_s
#else
#define Gsscanf sscanf
#endif

#endif	//GDEF_H
