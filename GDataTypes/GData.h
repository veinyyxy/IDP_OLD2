//////////////////////////////////////////////////////////////////////////
// GData.h
// created by ply 20080317
// ���д������ݵĸ�ʽ�� ���� ������������
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

	GenerationData  & operator = (const GenerationData & GDataSource );						//����=������pData�����ݡ�
	bool  operator == (const GenerationData & GDataSource ) const;						//����==���Ƚ�pData�����ݡ�
	bool  operator != (const GenerationData & GDataSource ) const;						//����!=���Ƚ�pData�����ݡ�
	GenerationData  & operator << ( GenerationData & GDataSource );						//����>>���ƶ����ݡ�
	//Ҳ����Ҫ�ṩ����������ĺ�����

	gInt32 STDCALL_GSYS GetID();
	// Get true size of internal buffer.
	gInt32 STDCALL_GSYS GetSize();
	gInt32 STDCALL_GSYS GetTrueSize();
	void STDCALL_GSYS SetID ( gInt32 NewID ) ;
	// Added by CYL 20080402 S

	// ����,�޸����ݴ�С��ǰ���뱣��֮ǰ�������
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

	inline void * STDCALL_GSYS GetPointer ( gInt32 StartP)const;//���á�
//	Added by CYL 20080402 E
//  template <typename _T1>
//  inline typename _T1 * UsingAs();
protected:
	inline GHRESULT STDCALL_GSYS SetDataSize ( gInt32 size );									//����/�޸����ݴ�С��ǰ���뱣��֮ǰ�������
	inline GHRESULT STDCALL_GSYS DataSizeAdd ( gInt32 size );
	inline GHRESULT STDCALL_GSYS CleanData();													//�������
	inline GHRESULT STDCALL_GSYS ReadData ( gInt32 StartP, READTYPE ReadType, void * pReadTo, gInt32 Size = 0 );	//��Start�ֽڿ�ʼ��ȡReadType���͵����ݵ�pReadToָ����ָ��λ�á�
	inline GHRESULT STDCALL_GSYS WriteData ( gInt32 StartP, WRITETYPE WriteType, void * pWriteFrom, gInt32 Size = 0 );//��Start�ֽڿ�ʼд��WriteType���͵����ݴ�pWriteFromָ����ָ��λ�á�
	inline GHRESULT STDCALL_GSYS GetPointer ( gInt32 StartP, void * & pToGet );//���á�
	inline gInt8 STDCALL_GSYS ReadInt8 ( gInt32 StartP );
	inline gInt16 STDCALL_GSYS ReadInt16 ( gInt32 StartP );
	inline gInt32 STDCALL_GSYS ReadInt32 ( gInt32 StartP );
	inline gInt64 STDCALL_GSYS ReadInt64 ( gInt32 StartP );

private:
	inline GHRESULT STDCALL_GSYS Initialize ();															//��ʼ������
	inline GHRESULT STDCALL_GSYS UnInitialize ();	
	//GenerationData(const GenerationData & pGdata); //modify by wp 11 10		//����ʼ��//�������ͷ��ڴ棬���ұ�֤��������������������á�

public:
	inline GHRESULT  STDCALL_GSYS GetpDataByName(const char* pItemName, bool bAddRef=false, gInt32 DataSize=-1);
public:
	// Added by CYL 20080402 S
	gInt32 m_readOffset;//�������
	// 0 : raw data.
	// 1 : bzip2 compressed.
	gInt32 m_format;//�������
	gInt32 m_AvailSize;//�������
	// Added by CYL 20080402 E
	//add by zhangyin S for�ڴ��
	//�������Ǹ�GDataָ�����ռ�󶨵����֣�Ϊ�����ʾ���ڴ治�Ǵ��ڴ�ط����
	gChar m_DataName[32];
	//add by zhangyin E

	gInt32 m_TemInt[20];		//�ṩ���������ڴ��һЩ��ʱ����
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

