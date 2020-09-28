//////////////////////////////////////////////////////////////////////////
// GData.cpp
// created by ply 20080324
// 所有储存数据的格式都 必须 是这个类的子类
//////////////////////////////////////////////////////////////////////////

#include "GData.h"
#if defined(_MEMORY_POOL)
#include <ace/SString.h>
#endif

GenerationData::GenerationData()
{
	//m_MaxValue = 0;

	//m_radius = 0;
	//m_resolution = 0;
	//m_productcode = 0;
	Initialize();
}
GenerationData::GenerationData(const GenerationData & GDataSource )
{
	Initialize();
	if(this==&GDataSource)
	{
		return;
	}

	void * p = NULL;

	if(GDataSource.m_TrueSize >0)
	{
#if defined(_MEMORY_POOL)
		if (ACE_OS::strcmp(GDataSource.m_DataName,"")==0)
		{
			if ( ( p = malloc ( GDataSource.m_TrueSize ) ) == NULL )
			{
				return;
				//throw no enough memory error ...
			}
		}
		else
		{
#if defined (_SHOWSYS_MEMORY_POOL)
			if (GDataSource.m_TrueSize< 256*1024*1024)
			{
				g_pMemoryPoolManager->CreateNewName(&(m_DataName[0]));
				ACE_CString str(m_DataName);
				if((p=g_pMemoryPoolManager->ResizeItem(GDataSource.m_TrueSize, str.c_str()))== NULL)
				{
					return;
				}
			}
			else
			{
				ACE_DEBUG ((LM_DEBUG,ACE_TEXT ("m_pData size is add \n")));
				if ( ( p = malloc ( GDataSource.m_TrueSize ) ) == NULL )
				{
					return;
					//throw no enough memory error ...
				}
				strcpy(m_DataName, "");			}

#else
			g_pMemoryPoolManager->CreateNewName(&(m_DataName[0]));
			ACE_CString str(m_DataName);
			if((p=g_pMemoryPoolManager->ResizeItem(GDataSource.m_TrueSize, str.c_str()))== NULL)
			{
				return;
			}
#endif
		}
		m_TrueSize = GDataSource.m_TrueSize;
		m_pData = p;
#else
		if ( ( p = malloc ( GDataSource.m_TrueSize ) ) == NULL )
		{
			return;
			//throw no enough memory error ...
		}

		//拷贝Size
		m_TrueSize = GDataSource.m_TrueSize;
		//使用新分配空间
		m_pData = p;
		//拷贝其他成员变量
#endif
	}
	m_readOffset=0;
	m_format=GDataSource.m_format;
	m_AvailSize=GDataSource.m_AvailSize;
	m_ID=GDataSource.m_ID;
	for (int i=0;i<20;i++)
	{
		m_TemInt[i]=GDataSource.m_TemInt[i];
	}
	m_DataSize=GDataSource.m_DataSize;
	void* pData=(void*)m_pData;
	void* pData1=(void*)GDataSource.m_pData;
	memcpy ( pData, pData1, GDataSource.m_TrueSize );
	m_ID = GDataSource.m_ID;
}
void GenerationData::ClearAll()
{
	UnInitialize();
}

GenerationData::~GenerationData()
{
	UnInitialize();
}
/*
GenerationData::GenerationData(const GenerationData & GDataSource)
{
	if (this==&GDataSource)
	{
		return;
	}

	void * p = NULL;
	if(GDataSource.m_TrueSize > m_TrueSize)
	{

		if ( ( p = malloc ( GDataSource.m_TrueSize ) ) == NULL )
		{
			return;
			//throw no enough memory error ...
		}

		//拷贝Size
		m_TrueSize = GDataSource.m_TrueSize;
		//使用新分配空间
		m_pData = p;
		//拷贝其他成员变量
	}
	m_readOffset=0;
	m_format=GDataSource.m_format;
	m_AvailSize=GDataSource.m_AvailSize;
	m_ID=GDataSource.m_ID;
	for (int i=0;i<20;i++)
	{
		m_TemInt[i]=GDataSource.m_TemInt[i];
	}
	m_DataSize=GDataSource.m_DataSize;

	//拷贝内存
	memcpy ( m_pData, GDataSource.m_pData, GDataSource.m_TrueSize );

	//拷贝ID
	m_ID = GDataSource.m_ID;

}
*/
#include <string>
using namespace std;
#include <stdlib.h>

#include <stdio.h>
#include <time.h>

#ifdef _WIN32
#include <Windows.h>

#else
#include <unistd.h>
#endif

GenerationData  & GenerationData::operator = (const GenerationData & GDataSource )
{
	//检查对象
	//if (GDataSource.m_pData==NULL||GDataSource.m_DataSize==0)
	//{
	//	return GE_OBJECTUNUSEFUL;
	//}

	//按照GDataSource分配内存
	if (this==&GDataSource)
	{
		return *this;
	}
	void * p = NULL;
	if(GDataSource.m_TrueSize > m_TrueSize)
    {

/*		if ( ( p = malloc ( GDataSource.m_TrueSize ) ) == NULL )
		{
			//throw no enough memory error ...
		}
*/
		int k = 0;
		while (p== NULL)
		{
#if defined(_MEMORY_POOL)
			if (g_pMemoryPoolManager==NULL || ACE_OS::strcmp(GDataSource.m_DataName,"")==0)
			{
				try
				{
					p = malloc(GDataSource.m_TrueSize );
				}
				catch (...)
				{
					p=NULL;
				}
			} 
			else
			{
#if defined (_SHOWSYS_MEMORY_POOL)
				if (GDataSource.m_TrueSize <256*1024*1024)
				{
					if (ACE_OS::strcmp(m_DataName,"")==0)
					{
						g_pMemoryPoolManager->CreateNewName(&(m_DataName[0]));
					}
					ACE_CString str(m_DataName);
					p=g_pMemoryPoolManager->ResizeItem(GDataSource.m_TrueSize, str.c_str());
				}
				else
				{
					ACE_DEBUG ((LM_DEBUG,ACE_TEXT ("m_pData size is add \n")));
					strcpy(m_DataName, "");
					try
					{
						p = malloc(GDataSource.m_TrueSize );
					}
					catch (...)
					{
						p=NULL;
					}
				}

#else
				if (ACE_OS::strcmp(m_DataName,"")==0)
				{
					g_pMemoryPoolManager->CreateNewName(&(m_DataName[0]));
				}
				ACE_CString str(m_DataName);
				p=g_pMemoryPoolManager->ResizeItem(GDataSource.m_TrueSize, str.c_str());
#endif
			}
#else
			try
			{
				p = malloc(GDataSource.m_TrueSize );
			}
			catch (...)
			{
				p=NULL;
			}
#endif
			
			if (p==NULL)
			{
				k++;
#ifdef _WIN32
				Sleep(10000);
#else
				sleep(10);
#endif

				if (k > 30)
				{

#ifdef WIN32
					FILE *fp = fopen("c:\\memory.txt", "w+");
#else
					FILE *fp = fopen("/tmp/memory.txt", "w+");
#endif
					char   c[10];   
					sprintf(c,   "%d",   int(GDataSource.m_TrueSize));   

					if (fp)
					{
						fwrite("if this file is not empty ;\nnow :\nThe memory is over!!", strlen("if this file is not empty ;\nnow :\nThe memory is over!!"), 1, fp);
						fwrite(c,strlen(c),1,fp);
						fclose(fp);
					}
					throw(::std::bad_alloc());
					exit(0);
				}
			}
		}
		void* pData=(void*)m_pData;
		//释放自己内存
		if ( pData != NULL )
		{
#if defined(_MEMORY_POOL)
			if (g_pMemoryPoolManager==NULL)
			{
				free ( pData ) ;
			} 
			pData = NULL;
#else
			free ( pData ) ;
			pData = NULL;
#endif
		}

		//拷贝Size
		m_TrueSize = GDataSource.m_TrueSize;
		//使用新分配空间
		m_pData = p;
		//拷贝其他成员变量
	}
	m_readOffset=0;
	m_format=GDataSource.m_format;
	m_AvailSize=GDataSource.m_AvailSize;
	m_ID=GDataSource.m_ID;
	for (int i=0;i<20;i++)
	{
		m_TemInt[i]=GDataSource.m_TemInt[i];
	}
	m_DataSize=GDataSource.m_DataSize;

	//拷贝内存
	void* pData=(void*)m_pData;
	void* pData1=(void*)GDataSource.m_pData;
	memcpy ( pData, pData1, GDataSource.m_TrueSize );

	//拷贝ID
	m_ID = GDataSource.m_ID;

	return *this;
}


bool  GenerationData::operator == (const GenerationData & GDataSource ) const
{
	if (m_TrueSize != GDataSource.m_TrueSize)
	{
		return false;
	}
	void* pData=(void*)m_pData;
	void* pData1=(void*)GDataSource.m_pData;
	return !memcmp( pData, pData1, m_TrueSize );
}

bool  GenerationData::operator != (const GenerationData & GDataSource ) const
{
	if (m_TrueSize != GDataSource.m_TrueSize)
	{
		return true;
	}

	void* pData=(void*)m_pData;
	void* pData1=(void*)GDataSource.m_pData;
#if defined (WIN32)|| defined(WIN64)
#pragma warning(push)
#pragma warning(disable:4800)
#endif

	return memcmp( pData, pData1, m_TrueSize );

#if defined (WIN32)|| defined(WIN64)
#pragma warning(pop)
#endif

}


GenerationData  & GenerationData::operator << ( GenerationData & GDataSource )
{
	void* pData=(void*)GDataSource .m_pData;
	this->m_pData=pData;
	GDataSource.m_pData=NULL;

	this->m_AvailSize=GDataSource.m_AvailSize;
	GDataSource.m_AvailSize=0;

	this->m_DataSize=GDataSource.m_DataSize;
	GDataSource.m_DataSize=0;

	this->m_format=GDataSource.m_format;
	GDataSource.m_format=0;
	this->m_ID=GDataSource.m_ID;
	GDataSource.m_ID=0;

	this->m_readOffset=GDataSource.m_readOffset;
	GDataSource.m_readOffset = 0;

	this->m_TrueSize=GDataSource.m_TrueSize;
	GDataSource.m_TrueSize	= 0;

	strcpy(this->m_DataName, GDataSource.m_DataName);
	strcpy(GDataSource.m_DataName, "");

	for ( int i = 0;i < 20;i++ )
	{
		this->m_TemInt[i]=GDataSource.m_TemInt[i];
		GDataSource.m_TemInt[i] = -1;
	}

	return *this;
}

gInt32 GenerationData::GetID()
{
	return m_ID;
}

gInt32 GenerationData::GetTrueSize()
{
	return m_TrueSize;
}
gInt32 GenerationData::GetSize()
{
	return m_AvailSize;
}

void GenerationData::SetID ( gInt32 NewID )
{
	m_ID = NewID;

}

// Added by CYL 20080402 S
void GenerationData::ResetReadOffset()
{
	m_readOffset = 0;
};
void GenerationData::ResetWriteOffset()
{
	m_DataSize = 0;
};

GHRESULT GenerationData::ReadDataCopy( void *  pbuf, gInt32 size, gUint32* avail_size)
{
	// Reading offset is at or over the end of local buffer.
    if(m_DataSize <= 0) return GE_FAIL;
	if(m_readOffset != 0 && m_readOffset >= m_DataSize)
	{
		m_readOffset = m_DataSize;
		return GE_FAIL;
	}

	// Cal byte count to be red.
	*avail_size = ((m_DataSize - m_readOffset) > size)? size : (m_DataSize- m_readOffset);

	void* pData=(void*)m_pData;
	// Copy that.
	memcpy(pbuf, (char*)pData + m_readOffset, *avail_size);

	m_readOffset += *avail_size;

	return GS_OK;
};

GHRESULT GenerationData::WriteData(void* buf, gInt32 size)
{
	if(m_DataSize + size > m_AvailSize)
	{
		m_DataSize = m_AvailSize;
		return GE_FAIL;
	}

	void* pData=(void*)m_pData;
	memcpy((char*)pData + m_DataSize, buf, size);

	m_DataSize += size;

	return GS_OK;
};
// Added by CYL 20080402 E
