//////////////////////////////////////////////////////////////////////////
// GData.inl
// created by ply 20080401
// 所有储存数据的格式都 必须 是这个类的子类
// 用于保存GData的inline函数
//////////////////////////////////////////////////////////////////////////

#if defined(_MEMORY_POOL)
#include <ace/SString.h>
#endif

inline gInt16 GenerationData::ReadInt16 ( gInt32 StartP )
{
	void* pData=(void*)m_pData;
	return ( * ( gInt16* ) ( ( ( gInt8* ) pData ) + StartP ) );
}


inline GHRESULT GenerationData::GetPointer ( gInt32 StartP, void * & PointerToGet )
{
    // if ( StartP >= m_DataSize || StartP < 0 )
    if ( StartP >= m_AvailSize || StartP < 0 )
	{
		return GE_INVALIDARG;
	}
	void* pData=(void*)m_pData;
	PointerToGet = ( ( void* ) ( ( ( gInt8* ) pData ) + StartP ) );
	return GS_OK;
}
inline void * STDCALL_GSYS GenerationData::GetPointer ( gInt32 StartP)const
{
	//if ( StartP >= m_DataSize||StartP<0 )
	//{
		if ( StartP >= m_AvailSize || StartP < 0 )
			return NULL;
		else
		{
			void* pData=(void*)m_pData;
			return ( ( void* ) ( ( ( gInt8* ) pData ) + StartP ) );
		}
	/*}
	else
	{
		return NULL;
	}*/
}

inline GHRESULT GenerationData::ReadData ( gInt32 StartP, READTYPE ReadType, void *  pReadTo, gInt32 Size )
{

	void* pData=(void*)m_pData;
	switch ( ReadType )
	{
	case READ_TYPE_BIT_8:
        if ( StartP + 1 > m_AvailSize)
		{
			return GE_INVALIDARG;
		}
		( * ( gInt8* ) pReadTo ) = ( * ( gInt8* ) ( ( ( gInt8* ) pData ) + StartP ) );
		break;

	case READ_TYPE_BIT_16:
        if ( StartP + 2 > m_AvailSize)
		{
			return GE_INVALIDARG;
		}
		( * ( gInt16* ) pReadTo ) = ( * ( gInt16* ) ( ( ( gInt8* ) pData ) + StartP ) );
		break;

	case READ_TYPE_BIT_32:
        if ( StartP + 4 > m_AvailSize)
		{
			return GE_INVALIDARG;
		}
		( * ( gInt32* ) pReadTo ) = ( * ( gInt32* ) ( ( ( gInt8* ) pData ) + StartP ) );
		break;

	case READ_TYPE_BIT_64:
        if ( StartP + 8 > m_AvailSize)
		{
			return GE_INVALIDARG;
		}
		( * ( gInt64* ) pReadTo ) = ( * ( gInt64* ) ( ( ( gInt8* ) pData ) + StartP ) );
		break;

	case READ_TYPE_UNKNOWN:
        if ( Size < 0 || StartP + Size > m_AvailSize)
		{
			return GE_INVALIDARG;
		}
		memcpy ( pReadTo, ( ( ( gInt8* ) pData ) + StartP ), Size );
		break;
	default:
		return GE_INVALIDARG;
		break;
	}
	return GS_OK;
}



inline GHRESULT GenerationData::WriteData ( gInt32 StartP, WRITETYPE WriteType, void *  pWriteFrom, gInt32 Size )
{
	void* pData=(void*)m_pData;
	GHRESULT RetTem;
	switch ( WriteType )
	{
	case WRITE_TYPE_BIT_8:
        if ( StartP + 1 > m_AvailSize  )
		{
			if ( ( RetTem = Realloc ( StartP + 1 * 128 ) ) != GS_OK )
			{
				return RetTem;
			}
		}
		( * ( gInt8* ) ( ( ( gInt8* ) pData ) + StartP ) ) = ( * ( gInt8* ) pWriteFrom );
		if (m_DataSize< StartP + 1 )
		{
			m_DataSize= StartP + 1 ;
		}
		break;

	case WRITE_TYPE_BIT_16:
        if ( StartP + 2 > m_AvailSize  )
		{
			if ( ( RetTem = Realloc ( StartP + 2 * 128 ) ) != GS_OK )
			{
				return RetTem;
			}
		}
		( * ( gInt16* ) ( ( ( gInt8* ) pData ) + StartP ) ) = ( * ( gInt16* ) pWriteFrom );
		if (m_DataSize< StartP + 2 )
		{
			m_DataSize= StartP + 2 ;
		}
		break;

	case WRITE_TYPE_BIT_32:
        if ( StartP + 4 > m_AvailSize )
		{
			if ( ( RetTem = Realloc ( StartP + 4 * 128) ) != GS_OK )
			{
				return RetTem;
			}
		}
		( * ( gInt32* ) ( ( ( gInt8* ) pData ) + StartP ) ) = ( * ( gInt32* ) pWriteFrom );
		if (m_DataSize< StartP + 4 )
		{
			m_DataSize= StartP + 4 ;
		}
		break;

	case WRITE_TYPE_BIT_64:
        if ( StartP + 8 > m_AvailSize )
		{
			if ( ( RetTem = Realloc ( StartP + 8 * 128 ) ) != GS_OK )
			{
				return RetTem;
			}
		}
		( * ( gInt64* ) ( ( ( gInt8* ) pData ) + StartP ) ) = ( * ( gInt64* ) pWriteFrom );
		if (m_DataSize< StartP + 8 )
		{
			m_DataSize= StartP + 8 ;
		}
		break;

	case WRITE_TYPE_UNKNOWN:
		if (Size < 0 )
		{
			return GE_INVALIDARG;
		}
        if ( StartP + Size > m_AvailSize )
		{
			if ( ( RetTem = Realloc ( StartP + Size * 2 ) ) != GS_OK )
			{
				return RetTem;
			}
		}
		memcpy ( ( ( ( gInt8* ) pData ) + StartP ), pWriteFrom, Size );
		if (m_DataSize< StartP + Size )
		{
			m_DataSize= StartP + Size ;
		}
		break;
	default:
		return GE_INVALIDARG;
		break;
	}
	return GS_OK;
}

/*
inline GHRESULT GenerationData::ReadData ( gInt32 StartP, READTYPE ReadType, void *  pReadTo, gInt32 Size )
{

	switch ( ReadType )
	{
	case READ_TYPE_BIT_8:
		if ( StartP + 1 > m_DataSize )
		{
			return GE_INVALIDARG;
		}
		( * ( gInt8* ) pReadTo ) = ( * ( gInt8* ) ( ( ( gInt8* ) m_pData ) + StartP ) );
		break;

	case READ_TYPE_BIT_16:
		if ( StartP + 2 > m_DataSize )
		{
			return GE_INVALIDARG;
		}
		( * ( gInt16* ) pReadTo ) = ( * ( gInt16* ) ( ( ( gInt8* ) m_pData ) + StartP ) );
		break;

	case READ_TYPE_BIT_32:
		if ( StartP + 4 > m_DataSize )
		{
			return GE_INVALIDARG;
		}
		( * ( gInt32* ) pReadTo ) = ( * ( gInt32* ) ( ( ( gInt8* ) m_pData ) + StartP ) );
		break;

	case READ_TYPE_BIT_64:
		if ( StartP + 8 > m_DataSize )
		{
			return GE_INVALIDARG;
		}
		( * ( gInt64* ) pReadTo ) = ( * ( gInt64* ) ( ( ( gInt8* ) m_pData ) + StartP ) );
		break;

	case READ_TYPE_UNKNOWN:
		if ( Size < 0 || StartP + Size > m_DataSize )
		{
			return GE_INVALIDARG;
		}
		memcpy ( pReadTo, ( ( ( gInt8* ) m_pData ) + StartP ), Size );
		break;
	default:
		return GE_INVALIDARG;
		break;
	}
	return GS_OK;
}

inline GHRESULT GenerationData::WriteData ( gInt32 StartP, WRITETYPE WriteType, void *  pWriteFrom, gInt32 Size )
{
	GHRESULT RetTem;
	switch ( WriteType )
	{
	case WRITE_TYPE_BIT_8:
		if ( StartP + 1 > m_DataSize )
		{
			if ( ( RetTem = SetDataSize ( StartP + 1 ) ) != GS_OK )
			{
				return RetTem;
			}
		}
		( * ( gInt8* ) ( ( ( gInt8* ) m_pData ) + StartP ) ) = ( * ( gInt8* ) pWriteFrom );
		break;

	case WRITE_TYPE_BIT_16:
		if ( StartP + 2 > m_DataSize )
		{
			if ( ( RetTem = SetDataSize ( StartP + 2 ) ) != GS_OK )
			{
				return RetTem;
			}
		}
		( * ( gInt16* ) ( ( ( gInt8* ) m_pData ) + StartP ) ) = ( * ( gInt16* ) pWriteFrom );
		break;

	case WRITE_TYPE_BIT_32:
		if ( StartP + 4 > m_DataSize )
		{
			if ( ( RetTem = SetDataSize ( StartP + 4 ) ) != GS_OK )
			{
				return RetTem;
			}
		}
		( * ( gInt32* ) ( ( ( gInt8* ) m_pData ) + StartP ) ) = ( * ( gInt32* ) pWriteFrom );
		break;

	case WRITE_TYPE_BIT_64:
		if ( StartP + 8 > m_DataSize )
		{
			if ( ( RetTem = SetDataSize ( StartP + 8 ) ) != GS_OK )
			{
				return RetTem;
			}
		}
		( * ( gInt64* ) ( ( ( gInt8* ) m_pData ) + StartP ) ) = ( * ( gInt64* ) pWriteFrom );
		break;

	case WRITE_TYPE_UNKNOWN:
		if ( Size < 0 || StartP + Size > m_DataSize )
		{
			if ( ( RetTem = SetDataSize ( StartP + Size ) ) != GS_OK )
			{
				return RetTem;
			}
		}
		memcpy ( ( ( ( gInt8* ) m_pData ) + StartP ), pWriteFrom, Size );
		break;
	default:
		return GE_INVALIDARG;
		break;
	}
	return GS_OK;
}
*/

inline gInt8 GenerationData::ReadInt8 ( gInt32 StartP )
{
	void* pData=(void*)m_pData;
	return ( * ( gInt8* ) ( ( ( gInt8* ) pData ) + StartP ) );
}

inline gInt32 GenerationData::ReadInt32 ( gInt32 StartP )
{
	void* pData=(void*)m_pData;
	return ( * ( gInt32* ) ( ( ( gInt8* ) pData ) + StartP ) );
}
inline gInt64 GenerationData:: ReadInt64 ( gInt32 StartP )
{
	void* pData=(void*)m_pData;
	return ( * ( gInt64* ) ( ( ( gInt8* ) pData ) + StartP ) );
}


// Modified by CYL 20080408 S
inline GHRESULT GenerationData::SetDataSize ( gInt32 size )
{
    // Require more space.
    if(size > m_TrueSize)
    {
		void * p = NULL;
#if defined(_MEMORY_POOL)
		if (g_pMemoryPoolManager==NULL)
		{
			void* pData=(void*)m_pData;
			try
			{
				p = realloc ( pData, size );
			}
			catch (...)
			{
				p = NULL;
			}
		}
		else
		{
			void* pData= NULL;
#if defined (_SHOWSYS_MEMORY_POOL)

			if (ACE_OS::strcmp(m_DataName,"")==0)
			{
				g_pMemoryPoolManager->CreateNewName(&(m_DataName[0]));
				pData=(void*)m_pData;
			}
			ACE_CString str(m_DataName);
			p=g_pMemoryPoolManager->RenewItem(size, str.c_str());
#else
			if (ACE_OS::strcmp(m_DataName,"")==0)
			{
				g_pMemoryPoolManager->CreateNewName(&(m_DataName[0]));
				pData=(void*)m_pData;
			}
			ACE_CString str(m_DataName);
			p=g_pMemoryPoolManager->RenewItem(size, str.c_str());
#endif
		}
#else
		try
		{
			p = realloc ( m_pData, size );
		}
		catch (...)
		{
			p = NULL;
		}
#endif
        if ( p != NULL )
        {
            m_pData	= p;
            m_AvailSize = size;
            m_DataSize = size;
            m_TrueSize = size;

            return GS_OK;
        }
        else
        {
            return GE_NOENOUGHMEMORY;
        }
    }
    else	// size <= m_TrueSize, no realloc.
    {
        if(size > m_AvailSize)
        {
            m_AvailSize = size;
            m_DataSize = size;
        }
        else if(size > m_DataSize && size < m_AvailSize)
        {
            m_DataSize = size;
        }
        else if(size < m_DataSize)
        {
            m_DataSize = size;
        }
    }

    return GS_OK;
}
/*
inline GHRESULT GenerationData::SetDataSize ( gInt32 size )
{
	if ( void * p = realloc ( m_pData, size ) )
	{
		m_pData		= p;
		m_DataSize	= size;
		return GS_OK;
	}
	else
	{
		return GE_NOENOUGHMEMORY;
	}
}*/
// Modified by CYL 20080408 E
// Added by CYL 20080408 S
inline GHRESULT GenerationData::Resize(gInt32 size)
{
	void* pData;
	if(size <= 0)
	{
		return GE_FAIL;
	}
	else
	{
        if(size > m_TrueSize)
		{
#if defined(_MEMORY_POOL)
			if (g_pMemoryPoolManager==NULL)
			{
				pData=(void*)m_pData;
				free(pData);
				try
				{
					pData = malloc(size);
				}
				catch (...)
				{
					m_DataSize = 0;
					m_AvailSize = 0;
					m_TrueSize = 0;
					return GE_NOENOUGHMEMORY;
				}
			}
			else
			{
#if defined (_SHOWSYS_MEMORY_POOL)
				if (ACE_OS::strcmp(m_DataName,"")==0)
				{
					g_pMemoryPoolManager->CreateNewName(&(m_DataName[0]));
				}
				ACE_CString str(m_DataName);
				pData=g_pMemoryPoolManager->ResizeItem(size, str.c_str());
#else
				if (ACE_OS::strcmp(m_DataName,"")==0)
				{
					g_pMemoryPoolManager->CreateNewName(&(m_DataName[0]));
				}
				ACE_CString str(m_DataName);
				pData=g_pMemoryPoolManager->ResizeItem(size, str.c_str());
#endif
			}
#else
			free(m_pData);
			try
			{
				pData = malloc(size);
			}
			catch (...)
			{
				m_DataSize = 0;
				m_AvailSize = 0;
				m_TrueSize = 0;
				return GE_NOENOUGHMEMORY;
			}
#endif

			if (pData==NULL)
			{
				m_DataSize = 0;
				m_AvailSize = 0;
				m_TrueSize = 0;
				return GE_NOENOUGHMEMORY;
			}
			m_pData=reinterpret_cast<void *> (pData);
            m_DataSize = 0;
            m_AvailSize = size;
            m_TrueSize = size;
		}
		else
		{
            m_AvailSize = size;
		}
	}

	void* tmpPtr=(void*)m_pData;
    memset(tmpPtr, 0x00, m_AvailSize);

	return GS_OK;
};

inline GHRESULT GenerationData::Realloc(gInt32 size)
{
    // Require more space.
    if(size > m_TrueSize)
    {
		void * p = NULL;
#if defined(_MEMORY_POOL)
		if (g_pMemoryPoolManager==NULL)
		{
			void* pData=(void*)m_pData;
			try
			{
				p = realloc ( pData, size );
			}
			catch (...)
			{
				p = NULL;
			}
		}
		else
		{
#if defined (_SHOWSYS_MEMORY_POOL)
			void* pData= NULL;
			if (ACE_OS::strcmp(m_DataName,"")==0)
			{
				g_pMemoryPoolManager->CreateNewName(&(m_DataName[0]));
				pData=(void*)m_pData;
			}
			ACE_CString str(m_DataName);
			p=g_pMemoryPoolManager->RenewItem(size, str.c_str());

#else
			void* pData= NULL;
			if (ACE_OS::strcmp(m_DataName,"")==0)
			{
				g_pMemoryPoolManager->CreateNewName(&(m_DataName[0]));
				pData=(void*)m_pData;
			}
			ACE_CString str(m_DataName);
			p=g_pMemoryPoolManager->RenewItem(size, str.c_str());
#endif
		}
#else
		try
		{
			p = realloc ( m_pData, size );
		}
		catch (...)
		{
			p = NULL;
		}
#endif

		if ( p != NULL )
        {
            m_pData	= p;
            m_AvailSize = size;
            m_TrueSize = size;

            return GS_OK;
        }
        else
        {
            return GE_NOENOUGHMEMORY;
        }
    }
    else	// size <= m_TrueSize, no realloc.
    {
        if(size > m_AvailSize)
        {
            m_AvailSize = size;
        }
        else if(size > m_DataSize && size < m_AvailSize)
        {
            m_AvailSize = size;
        }
        else if(size < m_DataSize)
        {
            m_DataSize = size;
            m_AvailSize = size;
        }
    }

    return GS_OK;
};
// Added by CYL 20080408 E

inline GHRESULT STDCALL_GSYS GenerationData::DataSizeAdd ( gInt32 size )
{
	return SetDataSize(m_DataSize+size);
}

// Modified by CYL 20080408 S
inline GHRESULT GenerationData::Initialize()
{
	m_pData		= NULL;
	m_DataSize	= 0;
    m_AvailSize	= 0;
    m_TrueSize	= 0;
	m_format	= 0;
	m_ID		= 0;
    m_readOffset = 0;
	strcpy(m_DataName, "");

	for ( int i = 0;i < 20;i++ )
	{
		m_TemInt[i] = -1;
	}
	return GS_OK;
}

/*
inline GHRESULT GenerationData::Initialize()
{
	m_pData		= NULL;
	m_DataSize	= 0;
	for ( int i = 0;i < 20;i++ )
	{
		m_TemInt[i] = -1;
	}
	return GS_OK;
}
*/

inline GHRESULT GenerationData::UnInitialize()
{
#if defined(_MEMORY_POOL)
	void* pData=(void*)m_pData;
	if ( pData != NULL )
	{
		if (g_pMemoryPoolManager==NULL || (strcmp(m_DataName,"")==0))
		{
			ACE_DEBUG ((LM_DEBUG,ACE_TEXT ("m_pData size is del \n")));
			//free ( m_pData );
		}
		else
		{
			ACE_CString str(m_DataName);
			g_pMemoryPoolManager->DeleteItem(str.c_str());
		}
	}
#else
	if ( m_pData != NULL )
	{
		free ( m_pData );
	}
#endif
	strcpy(m_DataName,"");
	m_pData = NULL;
	m_AvailSize = m_DataSize = m_TrueSize = 0;
	m_format = 0;
	m_readOffset = 0;
	return GS_OK;
}

// Modified by CYL 20080408 S
inline GHRESULT GenerationData::CleanData()
{
#if defined(_MEMORY_POOL)
	void* pData=(void*)m_pData;
	//释放自己内存
	if ( pData != NULL )
	{
		if (g_pMemoryPoolManager==NULL || (strcmp(m_DataName,"")==0))
		{
			ACE_DEBUG ((LM_DEBUG,ACE_TEXT ("m_pData size is del 2\n")));
			//free ( m_pData ) ;//
		}
		else
		{
			ACE_CString str(m_DataName);
			g_pMemoryPoolManager->DeleteItem(str.c_str());
		}
	}
	m_pData = NULL;
#else
	if ( m_pData != NULL )
	{
		free ( m_pData ) ;
		m_pData = NULL;
	}
#endif
	strcpy(m_DataName,"");
	m_DataSize = 0;
    m_AvailSize = 0;
    m_TrueSize = 0;
    m_readOffset = 0;
    m_format = 0;
	for ( int i = 0;i < 20;i++ )
	{
		m_TemInt[i] = -1;
	}
	return GS_OK;
}
// Modified by CYL 20080408 E
/*
inline GHRESULT GenerationData::CleanData()
{
	//释放自己内存
	if ( m_pData != NULL )
	{
		free ( m_pData ) ;
		m_pData = NULL;
	}
	m_DataSize = 0;
	for ( int i = 0;i < 20;i++ )
	{
		m_TemInt[i] = -1;
	}
	return GS_OK;
}
*/
gInt32 GenerationData::GetDataSize()const
{
    return m_DataSize;
};


//template <typename _T1>
//inline typename _T1 * GenerationData::UsingAs()
//{
//	return (typename _T1 *)this;
//}
inline GHRESULT GenerationData::GetpDataByName(const char* pItemName, bool bAddRef, gInt32 DataSize)
{
#if defined(_MEMORY_POOL)
	if (g_pMemoryPoolManager==NULL || pItemName==NULL || ACE_OS::strcmp(pItemName, "")==0)
	{
		return GS_FALSE;
	}
	CleanData();
	ACE_OS::strcpy(m_DataName, pItemName);
	void* ptr=NULL;
	for (int i=0;i<6&&ptr==NULL;i++)
	{
		g_pMemoryPoolManager->Sync();
		g_pMemoryPoolManager->FindItem(pItemName, ptr);
		if(ptr!=NULL)
		{
			break;
		}
		ACE_ERROR((LM_ERROR, ACE_TEXT ("GetpDataByName failed %d times.\n"),i));
		ACE_OS::sleep(5);
	}
	if (ptr==NULL)
	{
		ACE_OS::strcpy(m_DataName, "");
		ACE_ERROR((LM_ERROR, ACE_TEXT ("GetpDataByName failed.\n")));
		return GE_FAIL;
	}
	m_pData=ptr;
	m_TrueSize = g_pMemoryPoolManager->GetItemSize(pItemName);
	if (DataSize==-1)
	{
		m_AvailSize = m_DataSize = m_TrueSize;
	}
	else
	{
		m_AvailSize = m_DataSize = DataSize;
	}

	if (bAddRef)
	{
		g_pMemoryPoolManager->AddRefCount(pItemName);
	}
	return GS_OK;
#else
	return GE_NOINTERFACE;
#endif
}
