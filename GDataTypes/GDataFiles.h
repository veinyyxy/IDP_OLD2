//////////////////////////////////////////////////////////////////////////
// GDataFiles.h
// created by ply 20080326
// ���ļ�����GData�����ݵĴ洢������ѹ����
////////////////////////////////////////////////////////////////////////// 
#pragma once

#ifndef GDATAFILES_H
#define	GDATAFILES_H

#include "GData.h"
#include "GDef.h"
#include <stdio.h>

enum CompressMode
{
	COMPRESS_YES,					//����ʱ��ѹ������ȡʱ���ѹ
	COMPRESS_NO,					//����/��ȡʱ��ֱ�ӽ���Ӧ���ݶ�ȡ/���룬�����н�ѹ/ѹ��
	COMPRESS_AUTO,					//��ȡʱ�Զ��ж��Ƿ�ѹ����bz2��ʽ�Ļ����ǿ�ͷ��BZ�ģ�������ʱ��ͬ��DEFAULT
    COMPRESS_DEFAULT=COMPRESS_AUTO	//����Ĭ�ϲ�����ʽ
};

enum CompressLevel
{
    COMPRESS_LEVEL_1 = 1,
    COMPRESS_LEVEL_2,
    COMPRESS_LEVEL_3,
    COMPRESS_LEVEL_4,
    COMPRESS_LEVEL_5,
    COMPRESS_LEVEL_6,
    COMPRESS_LEVEL_7,
    COMPRESS_LEVEL_8,
    COMPRESS_LEVEL_9,
    COMPRESS_LEVEL_HIGHEST=COMPRESS_LEVEL_9,
     COMPRESS_LEVEL_LOWEST = COMPRESS_LEVEL_1,
    COMPRESS_LEVEL_DEFAULT = COMPRESS_LEVEL_LOWEST,
   
};

class DLLEXP_GSYS GDataFiles:public GenerationData
{
  GHRESULT CompressBlock( char*		input, 
			  gInt32	inSize,
			  char*		output,
        gUint32* outSize,
        CompressLevel level);

	gInt32 DecompressBlock( char*	input,
		gUint32*	inSize,
		char*	output,
		gUint32*	outSize);

  bool     IsBZ2File(FILE* pfile);
  
  GHRESULT ReadGData(char* buf, gInt32 size, gInt32* avali_size);
  GHRESULT WriteGData(char* buf, gInt32 size);

public:
  GDataFiles();

    GHRESULT operator = (GenerationData& gdata);
  GHRESULT operator = (GDataFiles& gdfile);
  
  GHRESULT WriteTo( char*		fileName,
        CompressMode	mode = /*COMPRESS_AUTO*/COMPRESS_NO,  //modify by dqc 20080805
        CompressLevel   level = COMPRESS_LEVEL_DEFAULT);
  GHRESULT WriteTo( GenerationData &	Target,
        CompressMode	mode = /*COMPRESS_AUTO*/COMPRESS_NO,
        CompressLevel   level = COMPRESS_LEVEL_DEFAULT);

  GHRESULT ReadFrom( char*		fileName,
		     CompressMode	mode = COMPRESS_AUTO);
  
  GHRESULT ReadFrom( GenerationData & Source,
		     CompressMode	mode = COMPRESS_AUTO);

  GHRESULT Export( const char*	fileName,
        CompressMode mode = /*COMPRESS_AUTO*/COMPRESS_NO ,
        CompressLevel   level = COMPRESS_LEVEL_DEFAULT);
  
  GHRESULT Export( GenerationData& Source,
        CompressMode	   mode = /*COMPRESS_AUTO*/COMPRESS_NO,
        CompressLevel   level = COMPRESS_LEVEL_DEFAULT);
    
  GHRESULT Import( const char*	fileName,
		   CompressMode	mode = COMPRESS_AUTO);

  GHRESULT Import( GenerationData& Target,
		   CompressMode	   mode = COMPRESS_AUTO);
};
#endif