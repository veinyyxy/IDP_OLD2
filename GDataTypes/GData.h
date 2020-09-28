//////////////////////////////////////////////////////////////////////////
// GData.h
// created by ply 20080317
// 所有储存数据的格式都 必须 是这个类的子类
//////////////////////////////////////////////////////////////////////////

#pragma once

#ifndef GDATA_H
#define	GDATA_H

#include "GDef.h"
#include "IBase.h"
#include <malloc.h>
#include <memory.h>
#if defined(_MEMORY_POOL)
#include "AceMemoryPoolManager.h"
#endif

#include<iostream> 
using namespace std; 

// Added by CYL 20080402 S
enum 
{
	DATA_FMT_BAD = 0,
	DATA_FMT_RAW = 1,
	DATA_FMT_BZIP_COMPRESSED,
};
// Added by CYL 20080402 E
class DLLEXP_GSYS GenerationData
{
public:

	GenerationData();
	GenerationData(const GenerationData & GDataSource );
	~GenerationData();

	GenerationData  & operator = (const GenerationData & GDataSource );						//重载=，复制pData中数据。
	bool  operator == (const GenerationData & GDataSource ) const;						//重载==，比较pData中数据。
	bool  operator != (const GenerationData & GDataSource ) const;						//重载!=，比较pData中数据。
	GenerationData  & operator << ( GenerationData & GDataSource );						//重载>>，移动数据。
	//也许需要提供流输入输出的函数。

	gInt32 STDCALL_GSYS GetID();
	// Get true size of internal buffer.
	gInt32 STDCALL_GSYS GetSize();
	gInt32 STDCALL_GSYS GetTrueSize();
	void STDCALL_GSYS SetID ( gInt32 NewID ) ;
	// Added by CYL 20080402 S

	// 设置,修改数据大小，前对齐保留之前存的数据
	inline GHRESULT STDCALL_GSYS Realloc(gInt32 size);
	// Resize buffer, all data will be lost.
	inline GHRESULT STDCALL_GSYS Resize(gInt32 size);

	////////////////////////////////////////////////////////////////////
	// Objective:
	//		Read(Copy) data from internal buffer like stream.
	// Return : Content
	// G_FAIL : Reading offset is at or over the end of local buffer.
	// GS_OK  : Well, nothing is wrong.
	// Param  :	buf - Output buffer.
	//		size - Max size of buf.
	//		avali_size - Copied byte count.
	////////////////////////////////////////////////////////////////////
	GHRESULT STDCALL_GSYS ReadDataCopy(void* buf, gInt32 size, gUint32* avali_size);
	////////////////////////////////////////////////////////////////////
	// Objective: 
	//		Write data to internal buffer like stream.
	// RETURN : Content
	// G_FAIL : No more memory.
	// GS_OK  : Well, nothing is wrong.
	////////////////////////////////////////////////////////////////////
	GHRESULT STDCALL_GSYS WriteData(void* buf, gInt32 size);
	inline gInt32 GetDataSize()const;
	void	STDCALL_GSYS ResetReadOffset();
	void   STDCALL_GSYS ResetWriteOffset();
	void   STDCALL_GSYS ClearAll();

	inline void * STDCALL_GSYS GetPointer ( gInt32 StartP)const;//慎用。
//	Added by CYL 20080402 E
//  template <typename _T1>
//  inline typename _T1 * UsingAs();
protected:
	inline GHRESULT STDCALL_GSYS SetDataSize ( gInt32 size );									//设置/修改数据大小，前对齐保留之前存的数据
	inline GHRESULT STDCALL_GSYS DataSizeAdd ( gInt32 size );
	inline GHRESULT STDCALL_GSYS CleanData();													//清空数据
	inline GHRESULT STDCALL_GSYS ReadData ( gInt32 StartP, READTYPE ReadType, void * pReadTo, gInt32 Size = 0 );	//从Start字节开始读取ReadType类型的数据到pReadTo指针所指的位置。
	inline GHRESULT STDCALL_GSYS WriteData ( gInt32 StartP, WRITETYPE WriteType, void * pWriteFrom, gInt32 Size = 0 );//从Start字节开始写入WriteType类型的数据从pWriteFrom指针所指的位置。
	inline GHRESULT STDCALL_GSYS GetPointer ( gInt32 StartP, void * & pToGet );//慎用。
	inline gInt8 STDCALL_GSYS ReadInt8 ( gInt32 StartP );
	inline gInt16 STDCALL_GSYS ReadInt16 ( gInt32 StartP );
	inline gInt32 STDCALL_GSYS ReadInt32 ( gInt32 StartP );
	inline gInt64 STDCALL_GSYS ReadInt64 ( gInt32 StartP );

private:
	inline GHRESULT STDCALL_GSYS Initialize ();															//初始化对象
	inline GHRESULT STDCALL_GSYS UnInitialize ();	
	//GenerationData(const GenerationData & pGdata); //modify by wp 11 10		//反初始化//在这里释放内存，并且保证这个函数被析构函数调用。

public:
	inline GHRESULT  STDCALL_GSYS GetpDataByName(const char* pItemName, bool bAddRef=false, gInt32 DataSize=-1);
public:
	// Added by CYL 20080402 S
	gInt32 m_readOffset;//不许操作
	// 0 : raw data.
	// 1 : bzip2 compressed.
	gInt32 m_format;//不许操作
	gInt32 m_AvailSize;//不许操作
	// Added by CYL 20080402 E
	//add by zhangyin S for内存池
	//此名字是该GData指针分配空间绑定的名字，为空则表示该内存不是从内存池分配的
	gChar m_DataName[32];
	//add by zhangyin E

	gInt32 m_TemInt[20];		//提供给子类用于存放一些临时数据
protected:
	gInt32 m_ID;
	gInt32 m_DataSize;
private:
	void * m_pData;
	gInt32 m_TrueSize;

//public:
//	gFloat m_MaxValue;
//	gDouble m_radius;
//	gDouble m_resolution;
//	gInt32 m_productcode;
};

#include "GData.inl"

#endif

