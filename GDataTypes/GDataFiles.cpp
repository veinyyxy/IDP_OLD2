//////////////////////////////////////////////////////////////////////////
// GDataFiles.cpp
// created by ply 20080326
// 本文件负责GData中数据的存储。包括压缩。
//////////////////////////////////////////////////////////////////////////

#include "IBase.h"
#include "GDataFiles.h"
#include <stdio.h>
#include <list>
#include <vector>
#include <assert.h>
#include "bzlib.h"

#if defined (WIN32)|| defined(WIN64)
#else

#include <sys/stat.h>

#endif

using namespace std;

#define MAX_FILE_BUFFER_SIZE 12*1024*1024
#define MAX_INPUT_BUF_SIZE 12*1024*1024
#define MAX_OUTPUT_BUF_SIZE 12*1024*1024
#define MIN_OUTPUT_BUF_SIZE 512*1024


GDataFiles::GDataFiles()
{
    m_readOffset = m_DataSize = 0;
};

///////////////////////////////////
//Check file header.
//Weak checking.
///////////////////////////////////
bool GDataFiles::IsBZ2File(FILE* pfile)
{
	char header[4];

	fseek(pfile, 0, SEEK_SET);
	fread(header, 1, 2, pfile);
	fseek(pfile, 0, SEEK_SET);

	// Check the 1st and the 2nd byte.
	if(header[0] == 'B' && header[1] == 'Z')
	{
		return true;
	}

	return false;
};

////////////////////////////////////////////////////////////////////////
// Import data from file.
// Non-BZip2 file and BZip2 file.
// Mode:
// COMPRESS_AUTO:			
//		 Non-BZip2 file will be import without decompressing.
//		 BZip2 file will be decompressed.
// COMPRESS_YES :
//		 Any file imported will be looked as a BZip2 file.
// COMPRESS_NO  :
//		Read data without any compression or decompression.
// COMPRESS_DEFAULT=COMPRESS_YES
///////////////////////////////////////////////////////////////////////
GHRESULT GDataFiles::Import(const char* fileName,
														CompressMode mode)
{
	FILE* pfile;

	GfOpen(pfile, fileName, "rb");	// Must be Read-Binary Mode.

	if(!pfile)
	{
		printf("Open file faild. Import operation aborded.\n");
		return GE_ABORT;
	}

	BZFILE* pbz = NULL;

	char	buf[128*1024];
	int	nBuf = sizeof(buf);
	int	bz2Error = 0;
	gUint32	nRead = 0;
#if defined(_MEMORY_POOL)
#if defined(_SHOWSYS_MEMORY_POOL)
		vector<char> fileBuffer;
#endif

#define BufSize 512*1024

	gUint32 BufferCapacity = (gUint32)BufSize;

	char dataname[32] = {'\0'};
	g_pMemoryPoolManager->CreateNewName(dataname);
	char *pFileBuffer = NULL;
	//pFileBuffer = (char*)g_pMemoryPoolManager->NewItem(BufSize,dataname);
#else
	vector<char> fileBuffer;
#endif
	bool bBZ2File = false;

	// Check mode.
	bBZ2File = IsBZ2File(pfile);

	// AUTO mode and current file is BZip format.
	if(COMPRESS_AUTO == mode && bBZ2File) mode = COMPRESS_YES;

	// Force using BZip mode.
	if(COMPRESS_YES == mode)
	{
        pbz = BZ2_bzReadOpen(&bz2Error, pfile, 0, 0, NULL, 0);

		if(bz2Error != BZ_OK)
		{
			printf("BZ2_bzWriteOpen Failed.\n");
			BZ2_bzReadClose(&bz2Error, pbz);
            fclose(pfile);
            m_format = DATA_FMT_BAD;
#if defined(_MEMORY_POOL)
			//g_pMemoryPoolManager->DeleteItem(dataname);
#endif
			return GE_ABORT;
		}
	}
	else	// BZip mode off.
	{
		mode = COMPRESS_NO;
		pbz = NULL;
	}
#if defined(_MEMORY_POOL)
	char * pBytes;
#if defined(_SHOWSYS_MEMORY_POOL)
	try
	{
		// Initialize input file buffer, 512KB.
		fileBuffer.resize(512*1024);
	}
	catch (...)
	{
		return GE_NOENOUGHMEMORY;
	}


	// Get buffer pointer.
	pBytes = &(fileBuffer[0]);
#endif
	//char * pBytes = pFileBuffer;
#else
	try
	{
		// Initialize input file buffer, 512KB.
		fileBuffer.resize(512*1024);
	}
	catch (...)
	{
		return GE_NOENOUGHMEMORY;
	}


	// Get buffer pointer.
	char* pBytes = &(fileBuffer[0]);

#endif

	GHRESULT temRe= GE_FAIL;
	switch(mode)
	{
	case COMPRESS_YES:	// BZip mode, bzip data will be compressed.
		{
			gInt32 writeOffset = 0;

			gUint32 total_read = 0;
			gUint32 total_dataSize = 0;
			bz2Error = BZ_OK;

			while(bz2Error == BZ_RUN)
			{
				nRead = BZ2_bzRead(&bz2Error, pbz, buf, sizeof(buf));

				total_dataSize += nRead;
			}

			BZ2_bzReadClose(&bz2Error, pbz);
			fclose(pfile);
			GfOpen(pfile, fileName, "rb");
			pbz = BZ2_bzReadOpen(&bz2Error, pfile, 0, 0, NULL, 0);
			bz2Error = BZ_OK;
#if defined(_MEMORY_POOL)
#if defined(_SHOWSYS_MEMORY_POOL)
			if (total_dataSize>256*1024*1024)
			{
				try
				{
					// No more space.
					fileBuffer.resize(total_dataSize);
				}
				catch (...)
				{
					return GE_NOENOUGHMEMORY;
				}
				// Update write pointer.
				pBytes = &(fileBuffer[0]);
			}
			else
			{
				pFileBuffer = (char*)g_pMemoryPoolManager->RenewItem(total_dataSize,dataname);
				if (pFileBuffer==NULL)
				{
					return GE_NOENOUGHMEMORY;
				}
				pBytes = pFileBuffer;
			}
#else
			pFileBuffer = (char*)g_pMemoryPoolManager->RenewItem(total_dataSize,dataname);
			if (pFileBuffer==NULL)
			{
				return GE_NOENOUGHMEMORY;
			}
			pBytes = pFileBuffer;

#endif

#else
			try
			{
				// No more space.
				fileBuffer.resize(total_dataSize);
			}
			catch (...)
			{
				return GE_NOENOUGHMEMORY;
			}

			// Update write pointer.
			pBytes = &(fileBuffer[0]);
#endif
			while(bz2Error == BZ_RUN)
			{
				nRead = BZ2_bzRead(&bz2Error, pbz, buf, sizeof(buf));

				if(bz2Error == BZ_IO_ERROR)
				{
					BZ2_bzReadClose(&bz2Error, pbz);

                    m_format = DATA_FMT_BAD;

                    fclose(pfile);
#if defined(_MEMORY_POOL)
					g_pMemoryPoolManager->DeleteItem(dataname);
#endif
					return GE_FAIL;
				}

				memcpy(pBytes + writeOffset, buf, nRead);

				writeOffset += nRead;

				total_read += nRead;
		
			}


			if(bz2Error != BZ_STREAM_END)
			{
                printf("BZ_STREAM_END not found!\n");
				// Error handling here.
				BZ2_bzReadClose(&bz2Error, pbz);
				fclose(pfile);
#if defined(_MEMORY_POOL)
				g_pMemoryPoolManager->DeleteItem(dataname);
#endif
				m_format = DATA_FMT_BAD;
				return GE_ABORT;
			}

			BZ2_bzReadClose(&bz2Error, pbz);

            fclose(pfile);

            m_format = DATA_FMT_BZIP_COMPRESSED;
#if defined(_MEMORY_POOL)
#if defined(_SHOWSYS_MEMORY_POOL)
			if (total_dataSize > 256*1024*1024)
			{
				if ((temRe=GenerationData::Resize(total_read)) != GS_OK)
				{
					return temRe;
				}

				// Fill it.
				if ((temRe=GenerationData::WriteData(pBytes, total_read)) != GS_OK)
				{
					return temRe;
				}
			}
			else
			{
                GenerationData::GetpDataByName(dataname,true);
			}
#else
	GenerationData::GetpDataByName(dataname,true);
#endif
#else
			// Create local data buffer.
			if ((temRe=GenerationData::Resize(total_read)) != GS_OK)
			{
				return temRe;
			}

			// Fill it.
			if ((temRe=GenerationData::WriteData(pBytes, total_read)) != GS_OK)
			{
				return temRe;
			}
#endif
		}
		break;
	case COMPRESS_NO:
		{
			gInt32 writeOffset = 0;
			
			gUint32 total_read = 0;

			// Get file buffer entry.
#if defined(_MEMORY_POOL)
			//pBytes = pFileBuffer;
#else
			gUint32 capacity = (gUint32)fileBuffer.capacity();
			pBytes = &(fileBuffer[0]);
			
			gUint32 size = (gUint32)fileBuffer.size();
#endif

#if defined (WIN32)|| defined(WIN64)
			HFILE hhFile = _lopen(fileName,OF_READ);
			DWORD FileSizeHigh;
			gUint32 SizeOfFile=GetFileSize((HANDLE*)hhFile,&FileSizeHigh);
			_lclose(hhFile);
#else
			struct stat FileStatBuf;
			stat(fileName, &FileStatBuf);
			gUint32 SizeOfFile=FileStatBuf.st_size;
#endif

#if defined(_MEMORY_POOL)
#if defined(_SHOWSYS_MEMORY_POOL)
			if (SizeOfFile>256*1024*1024)
			{
				try
				{
					// Need more space.
					fileBuffer.resize(SizeOfFile);
				}
				catch (...)
				{
					return GE_NOENOUGHMEMORY;
				}
				pBytes = &(fileBuffer[0]);
			}
			else
			{
				pFileBuffer = (char*)g_pMemoryPoolManager->RenewItem(SizeOfFile,dataname);
				if (pFileBuffer==NULL)
				{
					return GE_NOENOUGHMEMORY;
				}
				pBytes = pFileBuffer;
			}
#else
			pFileBuffer = (char*)g_pMemoryPoolManager->RenewItem(SizeOfFile,dataname);
			if (pFileBuffer==NULL)
			{
				return GE_NOENOUGHMEMORY;
			}
			pBytes = pFileBuffer;
#endif
#else
			try
			{
				// Need more space.
				fileBuffer.resize(SizeOfFile);
			}
			catch (...)
			{
				return GE_NOENOUGHMEMORY;
			}
			pBytes = &(fileBuffer[0]);
#endif
			while(!feof(pfile))
			{
				nRead = (gUint32)fread(buf, sizeof(char), nBuf, pfile);

				memcpy(pBytes + writeOffset, buf, nRead);

				writeOffset += nRead;

				total_read += nRead;
			}

			printf("Total read : %d.\n", total_read);

			fclose(pfile);

            m_format = DATA_FMT_RAW;
#if defined(_MEMORY_POOL)
			GenerationData::GetpDataByName(dataname,true);
#else
			// Create local data buffer.
			if ((temRe=GenerationData::Resize(total_read)) != GS_OK)
			{
				return temRe;
			}

			// Fill it.
			if ((temRe=GenerationData::WriteData(pBytes, total_read)) != GS_OK)
			{
				return temRe;
			}
#endif
		}
		break;
	}
#if defined(_MEMORY_POOL)
	g_pMemoryPoolManager->DeleteItem(dataname);
#endif
	return GS_OK;
};

GHRESULT GDataFiles::Export(const char* fileName,
							CompressMode mode,
                            CompressLevel level)
{
	FILE* pfile = NULL;

	// Reset read offset to 0.
	ResetReadOffset();

	GfOpen(pfile, fileName, "wb");

	if(!pfile) 
	{
		printf("Open file faild.Import operation aborted.\n");
		return GE_ABORT;
	}

	BZFILE* pbz = NULL;

	char	buf[128*1024];
	gInt32	nBuf = (gInt32)sizeof(buf);
	gInt32	bz2Error = 0;
	gUint32	avali_size = 0;
	gInt32	nWritten = 0;

	// Auto mode = BZip mode.
	if(COMPRESS_AUTO == mode)  mode = COMPRESS_YES;

	if(COMPRESS_YES == mode)
	{
		// BZip mode, compressing and writing data to BZip file.
        pbz = BZ2_bzWriteOpen(&bz2Error, pfile, level, 0, 30);
		
		if(bz2Error != BZ_OK)
		{
			switch(bz2Error)
			{
			case BZ_CONFIG_ERROR:
				printf("BZ_CONFIG_ERROR\n");
				break;
			case BZ_PARAM_ERROR:
				printf("BZ_PARAM_ERROR\n");
				break;
			case BZ_IO_ERROR:
				printf("BZ_IO_ERROR: %d\n", ferror(pfile));
				break;
			case BZ_MEM_ERROR:
				printf("BZ_MEM_ERROR\n");
				break;
			}
			BZ2_bzWriteClose(&bz2Error, pbz, 1, NULL, NULL);
            fclose(pfile);
			return GE_ABORT;
		}
	}
	else
	{
		mode = COMPRESS_NO;
		pbz = NULL;
	}

	switch(mode)
	{
	case COMPRESS_YES:
		{
			// Read data from local buffer.
			while(ReadDataCopy(buf, nBuf, &avali_size) == GS_OK)
			{
				BZ2_bzWrite(&bz2Error, pbz, buf, avali_size);

				if(bz2Error == BZ_IO_ERROR)
				{
					unsigned int bytesIn = 0;
					unsigned int bytesOut = 0;
					BZ2_bzWriteClose(&bz2Error, pbz, 1, &bytesIn, &bytesOut);
					printf("BZ_IO_ERROR.\n");

                    m_format = DATA_FMT_BAD;
					return GE_FAIL;
				}
			}

			BZ2_bzWriteClose(&bz2Error, pbz, 0, 0, 0);
            fclose(pfile);

            m_format = DATA_FMT_BZIP_COMPRESSED;
		}
		break;
	case COMPRESS_NO:
		{
			// Write data directly to file.
			gUint32 total_count = 0;

			while(ReadDataCopy(buf, nBuf, &avali_size) == GS_OK)
			{
				nWritten = (gInt32)fwrite(buf, 1, avali_size, pfile);

				total_count += nWritten;
			}

			fclose(pfile);
            m_format = DATA_FMT_RAW;
		}
		break;
	};

	return GS_OK;
};


GHRESULT GDataFiles::Import(GenerationData& src,
			    CompressMode mode)
{
	gInt32 srcFormat = src.m_format;

	// Reset read offset to zero.
	src.ResetReadOffset();

	if(mode == COMPRESS_AUTO)
	{
		if(DATA_FMT_BZIP_COMPRESSED == srcFormat)
		{
			mode = COMPRESS_YES;
		}
		else 
		  {
			mode = COMPRESS_NO;
		  }
	}

	// Create local buffer. 
	// MAX_FILE_BUFFER_SIZE now is 12MB.
	//GenerationData::Resize(MAX_FILE_BUFFER_SIZE);

	switch(mode)
	{
	case COMPRESS_YES:
		{
			char*	buf = new(::std::nothrow) char[src.GetDataSize()];
			if (buf ==NULL)
			{
				GE_NOENOUGHMEMORY;
			}
			gUint32 dataSize;
			gUint32	nRead = 0;
			dataSize = src.GetDataSize();
			char*	outBuf = new(::std::nothrow) char[MAX_INPUT_BUF_SIZE];
			if (outBuf == NULL)
			{
				delete buf;
				GE_NOENOUGHMEMORY;
			}
			gUint32	nOutBytes = 0;

			if(src.ReadDataCopy(buf, dataSize, &nRead) == GS_OK && nRead == dataSize)
			{
				nOutBytes = MAX_INPUT_BUF_SIZE;

				GHRESULT ret = DecompressBlock(buf, &nRead, outBuf, &nOutBytes);

                if(ret == GS_OK)
                {
                    Realloc(nOutBytes);
					WriteData(outBuf, nOutBytes);
                }
				else
				{
					delete buf;
					delete outBuf;
					return GS_FALSE;
				}
			}
			else
			{
				delete buf;
				delete outBuf;
				return GS_FALSE;
			}
			delete buf;
			delete outBuf;
		}
		break;
	case COMPRESS_NO:
		{
			// Copy data directly to local buffer.
            (*this) = src;
		}
		break;
	}

	// Set format flag.
	GenerationData::m_format = srcFormat;

	src.ResetReadOffset();

	return GS_OK;
};


GHRESULT GDataFiles::Export(GenerationData& dst,
                            CompressMode mode,
                            CompressLevel level)
{
	ResetReadOffset();
	
    //dst.m_format = m_format;
    //if(dst.GetTrueSize() < GenerationData::m_AvailSize)
    //{
    //    // No more space in dest GenerationData object.
    //    dst.Resize(GenerationData::m_AvailSize*2);
    //}

	if(COMPRESS_AUTO == mode)
	{
		mode = COMPRESS_NO;
	};

	switch(mode)
	{
	case COMPRESS_YES:
		{
			char* buf = (char*)malloc(this->GetDataSize());
			gInt32 nBuf = this->GetDataSize();
			gUint32 nRead = 0;
			char* outBuf = (char*)malloc(this->GetDataSize());
			gUint32 nOut = this->GetDataSize();

			if(ReadDataCopy(buf, nBuf, &nRead) == GS_OK)
			{
                CompressBlock(buf, nRead, outBuf, &nOut, level);

				dst.ClearAll();
				dst.Resize(nOut);

                dst.WriteData(outBuf, nOut);
            }
			else 
				return GS_FALSE;
            dst.m_format = 1;
			free( buf);
			free( outBuf);
        }
        break;
    case COMPRESS_NO:
        {
            GDataFiles* p = (GDataFiles*)(&dst);

            (*p) = *this;
        }
        break;
    };

    return GS_OK;
};

GHRESULT GDataFiles::CompressBlock( char*	input,
				    gInt32	inSize,
				    char*	output,
                                   gUint32*	outSize,
                                   CompressLevel level)
{
	int ret = BZ2_bzBuffToBuffCompress(output, outSize,
					   input,	 inSize,
        level, 0, 30);

	if(ret != BZ_OK)
	{
		// Error handling.
		return GE_FAIL;
	};

	return GS_OK;
};

gInt32  GDataFiles::DecompressBlock( char*	input,
				     gUint32*	inSize,
				     char*	output,
				     gUint32*	outSize)
{
	int ret = BZ2_bzBuffToBuffDecompress(output, outSize,
					     input,  inSize,
					     0,0);
	
	if(ret != BZ_OK)
	{
		// Error handling.

		return GE_FAIL;
	}

	return GS_OK;
};


GHRESULT GDataFiles::operator =(GenerationData& gdata)
{
    if(gdata.GetTrueSize() <= 0) return GE_FAIL;

    if(gdata.GetDataSize() > m_AvailSize)
    {
        // Data in RH object is bigger than LH available size(not true size).

        if(gdata.GetDataSize() <= GetTrueSize())
        {
            // RH obj data size is not bigger than LH one's true size.
            m_AvailSize = gdata.GetDataSize();
        }
        else 
        {
            Realloc(gdata.GetTrueSize());
            m_AvailSize = gdata.GetDataSize();
        }
    }

    m_format = gdata.m_format;

    m_readOffset = gdata.m_readOffset;
    m_DataSize = gdata.GetDataSize();

    memcpy(GetPointer(0), gdata.GetPointer(0), m_DataSize);

    return GS_OK;
}

GHRESULT GDataFiles::operator =(GDataFiles& gdfile)
{
    if(gdfile.GetTrueSize() <= 0) return GE_FAIL;

    if(gdfile.GetDataSize() > m_AvailSize)
    {
        // Data in RH object is bigger than LH available size(not true size).

        if(gdfile.GetDataSize() <= GetTrueSize())
        {
            // RH obj data size is not bigger than LH one's true size.
            m_AvailSize = gdfile.GetDataSize();
        }
        else 
        {
            Realloc(gdfile.GetTrueSize());
            m_AvailSize = gdfile.GetDataSize();
        }
    }

    m_format = gdfile.m_format;

    m_readOffset = gdfile.m_readOffset;
    m_DataSize = gdfile.GetDataSize();

    memcpy(GetPointer(0), gdfile.GetPointer(0), gdfile.GetDataSize());

    return GS_OK;
}


GHRESULT GDataFiles::WriteTo( char*		fileName,
                             CompressMode	mode,
                             CompressLevel level)
{
    return Export(fileName, mode,level);
}
GHRESULT GDataFiles::WriteTo( GenerationData &	Target,
                             CompressMode	mode ,
                             CompressLevel level)
{
	return Export(Target, mode,level);
}

GHRESULT GDataFiles::ReadFrom( char*		fileName,
									CompressMode	mode )
{
	return Import(fileName, mode);
}

GHRESULT GDataFiles::ReadFrom( GenerationData & Source,
									CompressMode	mode )
{
	return Import(Source, mode);
}
