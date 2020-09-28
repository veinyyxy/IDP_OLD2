//////////////////////////////////////////////////////////////////////////
// IBase.h
// created by xm-zhang 20080311
//////////////////////////////////////////////////////////////////////////

#pragma once

#ifndef IBASE_H
#define	IBASE_H

#include <assert.h>
#include "GDef.h"

typedef	gUint32 GHRESULT ;//接口函数返回值 32位无符号整型

#define GS_OK								(( GHRESULT )0x00000000L)	//成功返回
#define GS_FALSE							(( GHRESULT )0x00000001L)	//无效执行返回
#define GE_ABORT							(( GHRESULT )0x00000002L)	//操作中止
#define GE_OUTOFMEMORY						(( GHRESULT )0x00000004L)	//存在内存泄露
#define GE_INVALIDARG						(( GHRESULT )0x00000008L)	//一个或多个参数无效
#define GE_NOINTERFACE						(( GHRESULT )0x00000010L)	//不支持这种接口
#define GE_FAIL								(( GHRESULT )0x00000020L)	//未定义错误
#define GE_NOTIMPL							(( GHRESULT )0x00000040L)	//未实现
#define GE_UNEXPECTED						(( GHRESULT )0x00000080L)	//灾难性错误
#define GE_NOENOUGHMEMORY					(( GHRESULT )0x00000100L)	//无足够内存
#define GE_OBJECTUNUSEFUL					(( GHRESULT )0x00000200L)	//操作对象无效
#define GE_OBJECTEXIST						(( GHRESULT )0x00000400L)	//操作对象已存在
#define GE_SHORTOFINDATA					(( GHRESULT )0x00000800L)	//输入列表数据少，不符合产品生成条件，算法拒绝生成
#define GE_OUTOFDATARANG					(( GHRESULT )0x00001000L)	//此算法需要输入的参数不在正确范围内，算法停止继续计算
#define GE_WRONGOFINPUTDATA					(( GHRESULT )0x00002000L)	//输入算法list中的数据大小异常，认为数据不对，算法停止继续计算
#define GE_INVALHEADOFINDATA				(( GHRESULT )0x00004000L)	//对输入算法list中的数据,提取其中某部分头结构体数据的时，无法正确获取结构体指针
#define GE_INVALOFINDATA					(( GHRESULT )0x00008000L)	//对输入算法列表中的数据,提取其中具体数据（如基数据中R、V、W数值部分或者产品数据中具体数值部分）时，无法正确获取数据指针
#define GE_INVALHEADOFOUTDATA				(( GHRESULT )0x00010000L)	//对输出算法列表中的数据,获取其中某部分头结构体填充数据的时，无法正确获取结构体指针
#define GE_INVALOFOUTDATA					(( GHRESULT )0x00020000L)	//对输出算法列表中的数据,获取其中具体数据指针进行数据填写时（如基数据中R、V、W数值部分或者产品数据中具体数值部分），无法正确获取数据指针
#define GE_FAILOFNEWTEMPMEM					(( GHRESULT )0x00040000L)	//算法内部申请临时内存失败，算法终止执行
#define GE_FAILOFNEWPRODUMEM				(( GHRESULT )0x00080000L)	//产品空间申请不到，算法停止继续计算



#define		SYSMETHOD(method)       virtual GHRESULT STDCALL_GSYS method	//接口函数返回宏, 统一为__stdcall导出,返回值类型GHRESULT

enum READTYPE
{
	READ_TYPE_UNKNOWN=0,
	READ_TYPE_BIT_8,
	READ_TYPE_BIT_16,
	READ_TYPE_BIT_32,
	READ_TYPE_BIT_64,


	READ_TYPE_BYTE=READ_TYPE_BIT_8,
};

enum WRITETYPE
{
	WRITE_TYPE_UNKNOWN=0,
	WRITE_TYPE_BIT_8,
	WRITE_TYPE_BIT_16,
	WRITE_TYPE_BIT_32,
	WRITE_TYPE_BIT_64,


	WRITE_TYPE_BYTE=WRITE_TYPE_BIT_8,
};

#define RADAR_DATA_NODATA ((gInt16)-32768)
#define RADAR_DATA_NOECHO ((gInt16)-32767)
#define RADAR_DATA_OUTOFRANGE ((gInt16)-32766)


#endif
