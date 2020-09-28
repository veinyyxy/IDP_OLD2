//////////////////////////////////////////////////////////////////////////
// IBase.h
// created by xm-zhang 20080311
//////////////////////////////////////////////////////////////////////////

#pragma once

#ifndef IBASE_H
#define	IBASE_H

#include <assert.h>
#include "GDef.h"

typedef	gUint32 GHRESULT ;//�ӿں�������ֵ 32λ�޷�������

#define GS_OK								(( GHRESULT )0x00000000L)	//�ɹ�����
#define GS_FALSE							(( GHRESULT )0x00000001L)	//��Чִ�з���
#define GE_ABORT							(( GHRESULT )0x00000002L)	//������ֹ
#define GE_OUTOFMEMORY						(( GHRESULT )0x00000004L)	//�����ڴ�й¶
#define GE_INVALIDARG						(( GHRESULT )0x00000008L)	//һ������������Ч
#define GE_NOINTERFACE						(( GHRESULT )0x00000010L)	//��֧�����ֽӿ�
#define GE_FAIL								(( GHRESULT )0x00000020L)	//δ�������
#define GE_NOTIMPL							(( GHRESULT )0x00000040L)	//δʵ��
#define GE_UNEXPECTED						(( GHRESULT )0x00000080L)	//�����Դ���
#define GE_NOENOUGHMEMORY					(( GHRESULT )0x00000100L)	//���㹻�ڴ�
#define GE_OBJECTUNUSEFUL					(( GHRESULT )0x00000200L)	//����������Ч
#define GE_OBJECTEXIST						(( GHRESULT )0x00000400L)	//���������Ѵ���
#define GE_SHORTOFINDATA					(( GHRESULT )0x00000800L)	//�����б������٣������ϲ�Ʒ�����������㷨�ܾ�����
#define GE_OUTOFDATARANG					(( GHRESULT )0x00001000L)	//���㷨��Ҫ����Ĳ���������ȷ��Χ�ڣ��㷨ֹͣ��������
#define GE_WRONGOFINPUTDATA					(( GHRESULT )0x00002000L)	//�����㷨list�е����ݴ�С�쳣����Ϊ���ݲ��ԣ��㷨ֹͣ��������
#define GE_INVALHEADOFINDATA				(( GHRESULT )0x00004000L)	//�������㷨list�е�����,��ȡ����ĳ����ͷ�ṹ�����ݵ�ʱ���޷���ȷ��ȡ�ṹ��ָ��
#define GE_INVALOFINDATA					(( GHRESULT )0x00008000L)	//�������㷨�б��е�����,��ȡ���о������ݣ����������R��V��W��ֵ���ֻ��߲�Ʒ�����о�����ֵ���֣�ʱ���޷���ȷ��ȡ����ָ��
#define GE_INVALHEADOFOUTDATA				(( GHRESULT )0x00010000L)	//������㷨�б��е�����,��ȡ����ĳ����ͷ�ṹ��������ݵ�ʱ���޷���ȷ��ȡ�ṹ��ָ��
#define GE_INVALOFOUTDATA					(( GHRESULT )0x00020000L)	//������㷨�б��е�����,��ȡ���о�������ָ�����������дʱ�����������R��V��W��ֵ���ֻ��߲�Ʒ�����о�����ֵ���֣����޷���ȷ��ȡ����ָ��
#define GE_FAILOFNEWTEMPMEM					(( GHRESULT )0x00040000L)	//�㷨�ڲ�������ʱ�ڴ�ʧ�ܣ��㷨��ִֹ��
#define GE_FAILOFNEWPRODUMEM				(( GHRESULT )0x00080000L)	//��Ʒ�ռ����벻�����㷨ֹͣ��������



#define		SYSMETHOD(method)       virtual GHRESULT STDCALL_GSYS method	//�ӿں������غ�, ͳһΪ__stdcall����,����ֵ����GHRESULT

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
