//////////////////////////////////////////////////////////////////////////
// SingleProductBase.cpp
// created by ply 20080401
// 产品数据通用基类，GData的子类
//////////////////////////////////////////////////////////////////////////
#include "SingleProductBase.h"
#include <malloc.h>

ProductBase::ProductBase()
{
	;
}
ProductBase::~ProductBase()
{
	;
}

PDBlock * ProductBase::GetPDBlockP()
{
	return GetPDBlock();
}

tagRadarInfor * ProductBase::GetRadarInfoP()
{
	return GetRadarInfo();
}

GHRESULT ProductBase::FrameAll(gInt16 NumberOfLayers,.../*ONCE_LAYER,NumberOfBlocks,BLOCKTYPE_RADIAL,NumberOfRadials,NumberOfGates...BLOCKTYPE_GRID,NumberOfRows,NumberOfCols...,ONCE_LAYER,NumberOfBlocks,BLOCKTYPE_STRUCT,NumbersOfStruct,LengthOfEachStruct,...*/)
{
	va_list argp;
	va_start( argp, NumberOfLayers );
	BlockType CurrentBlockType;
	gInt16 CurrentPara1,CurrentPara2;
	gInt16 * Para1=NULL;
	gInt16 * Para2=NULL;
	gInt16 CurrentBlockCount;
	gInt32 * LayerBlockSize=NULL;
	BlockType * LayerBlockType=NULL;
	gInt32 LayerSize=0;
	gInt32 LayerReplayTimes=0;
	gInt32 CurrentSize=0;
	void * p=NULL;

	//分配头空间，写入头部分数据
	//SetDataSize(GetSizeOfRadarInfo()+GetSizeOfPDBlock()+GetSizeOfOffsetOfEachLayerBlock(NumberOfLayers));
	//GetPDBlock()->NumberOfLayers=NumberOfLayers;
	//GetPDBlock()->IndexOfLayers=GetSize();
	//GetPDBlock()->OffsetToGraphic=-1;
	//GetPDBlock()->OffsetToTabluar=-1;
	//GetPDBlock()->MessageCode=250;

	////层偏移区默认值设置
	//for (int i=0;i<NumberOfLayers;i++)
	//{
	//	GetLayerOffsetIndex()[i]=-1;
	//}

	//每一层
	for (gInt16 i=0;i<NumberOfLayers;i++)
	{
		LayerSize=0;

#if defined (WIN32)|| defined(WIN64)
		CurrentBlockType = va_arg(argp, BlockType);
#else
		CurrentBlockType = (BlockType)va_arg(argp, int);
#endif
		if (CurrentBlockType==ONCE_LAYER)//层重复种类判断
		{
			LayerReplayTimes=1;
		}
		else if (CurrentBlockType==MUTI_LAYER)
		{
#if defined (WIN32)|| defined(WIN64)
			LayerReplayTimes=va_arg(argp, gInt32);
#else
			LayerReplayTimes =(gInt32) va_arg(argp, int);
#endif
		}
		else
		{
			if (LayerBlockSize!=NULL)
			{
				free(LayerBlockSize);
			}
			if (LayerBlockType!=NULL)
			{
				free(LayerBlockType);
			}
			if (Para1!=NULL)
			{
				free(Para1);
			}
			if (Para2!=NULL)
			{
				free(Para2);
			}
			va_end( argp ); 
			return GE_INVALIDARG;
		}
		LayerSize=sizeof(DILayer);


#if defined (WIN32)|| defined(WIN64)
		CurrentBlockCount= va_arg(argp, gInt16);
#else
		CurrentBlockCount = (gInt16)va_arg(argp, int);
#endif

		if (CurrentBlockCount>CurrentSize)//分配临时变量空间
		{
			CurrentSize=CurrentBlockCount;

			try
			{
				p = realloc((void *)LayerBlockSize,sizeof(gInt32)*CurrentBlockCount);
			}
			catch (...)
			{
				p = NULL;
			}
			if (p!=NULL)
			{
				LayerBlockSize=(gInt32*)p;
			} 
			else
			{
				if (LayerBlockSize!=NULL)
				{
					free(LayerBlockSize);
				}
				if (LayerBlockType!=NULL)
				{
					free(LayerBlockType);
				}
				if (Para1!=NULL)
				{
					free(Para1);
				}
				if (Para2!=NULL)
				{
					free(Para2);
				}
				va_end( argp ); 
				return GE_NOENOUGHMEMORY;
			}
			try
			{
				p = realloc((void *)LayerBlockType,sizeof(BlockType)*CurrentBlockCount);
			}
			catch (...)
			{
				p = NULL;
			}
			if (p!=NULL)
			{
				LayerBlockType=(BlockType*)p;
			}
			else
			{
				if (LayerBlockSize!=NULL)
				{
					free(LayerBlockSize);
				}
				if (LayerBlockType!=NULL)
				{
					free(LayerBlockType);
				}
				if (Para1!=NULL)
				{
					free(Para1);
				}
				if (Para2!=NULL)
				{
					free(Para2);
				}
				va_end( argp ); 
				return GE_NOENOUGHMEMORY;
			}
			try
			{
				p = realloc((void *)Para1,sizeof(gInt16)*CurrentBlockCount);
			}
			catch (...)
			{
				p = NULL;
			}
			if (p!=NULL)
			{
				Para1=(gInt16*)p;
			}
			else
			{
				if (LayerBlockSize!=NULL)
				{
					free(LayerBlockSize);
				}
				if (LayerBlockType!=NULL)
				{
					free(LayerBlockType);
				}
				if (Para1!=NULL)
				{
					free(Para1);
				}
				if (Para2!=NULL)
				{
					free(Para2);
				}
				va_end( argp ); 
				return GE_NOENOUGHMEMORY;
			}
			try
			{
				p = realloc((void *)Para2,sizeof(gInt16)*CurrentBlockCount);
			}
			catch (...)
			{
				p = NULL;
			}
			if (p!=NULL)
			{
				Para2=(gInt16*)p;
			}
			else
			{
				if (LayerBlockSize!=NULL)
				{
					free(LayerBlockSize);
				}
				if (LayerBlockType!=NULL)
				{
					free(LayerBlockType);
				}
				if (Para1!=NULL)
				{
					free(Para1);
				}
				if (Para2!=NULL)
				{
					free(Para2);
				}
				va_end( argp ); 
				return GE_NOENOUGHMEMORY;
			}
		}

		for (gInt16 j=0;j<CurrentBlockCount;j++)//读取每块类型参数
		{
#if defined (WIN32)|| defined(WIN64)
			CurrentBlockType = va_arg(argp, BlockType);
#else
			CurrentBlockType = (BlockType)va_arg(argp, int);
#endif

			LayerBlockType[j]=CurrentBlockType;

			LayerBlockSize[j]=sizeof(DLIBlock);
			switch(CurrentBlockType)
			{
			case BLOCKTYPE_RADIAL:
#if defined (WIN32)|| defined(WIN64)
				CurrentPara1=va_arg(argp, gInt16);
				CurrentPara2=va_arg(argp, gInt16);
#else
				CurrentPara1=(gInt16)va_arg(argp, int);
				CurrentPara2=(gInt16)va_arg(argp, int);
#endif
				Para1[j]=CurrentPara1;
				Para2[j]=CurrentPara2;
				LayerBlockSize[j]+=GetSizeOfRadialBlock(CurrentPara1,CurrentPara2);
				break;
			case BLOCKTYPE_GRID:
#if defined (WIN32)|| defined(WIN64)
				CurrentPara1=va_arg(argp, gInt16);
				CurrentPara2=va_arg(argp, gInt16);
#else
				CurrentPara1=(gInt16)va_arg(argp, int);
				CurrentPara2=(gInt16)va_arg(argp, int);
#endif
				Para1[j]=CurrentPara1;
				Para2[j]=CurrentPara2;
				LayerBlockSize[j]+=GetSizeOfGridBlock(CurrentPara1,CurrentPara2);
				break;
			case BLOCKTYPE_LONLATGRID:
#if defined (WIN32)|| defined(WIN64)
				CurrentPara1=va_arg(argp, gInt16);
				CurrentPara2=va_arg(argp, gInt16);
#else
				CurrentPara1=(gInt16)va_arg(argp, int);
				CurrentPara2=(gInt16)va_arg(argp, int);
#endif
				Para1[j]=CurrentPara1;
				Para2[j]=CurrentPara2;
				LayerBlockSize[j]+=GetSizeOfGridBlock(CurrentPara1,CurrentPara2);
				break;
			case BLOCKTYPE_STRUCT:
#if defined (WIN32)|| defined(WIN64)
				CurrentPara1=va_arg(argp, gInt16);
				CurrentPara2=va_arg(argp, gInt16);
#else
				CurrentPara1=(gInt16)va_arg(argp, int);
				CurrentPara2=(gInt16)va_arg(argp, int);
#endif
				Para1[j]=CurrentPara1;
				Para2[j]=CurrentPara2;
				LayerBlockSize[j]+=GetSizeOfStructBlock(CurrentPara1,CurrentPara2);
			    break;
			default:
				if (LayerBlockSize!=NULL)
				{
					free(LayerBlockSize);
				}
				if (LayerBlockType!=NULL)
				{
					free(LayerBlockType);
				}
				if (Para1!=NULL)
				{
					free(Para1);
				}
				if (Para2!=NULL)
				{
					free(Para2);
				}
				va_end( argp ); 
				return GE_INVALIDARG;
			    break;
			}
			LayerSize+=LayerBlockSize[j];

		}
		//gInt32 TemOffset=GetSize();
		//GHRESULT nResult = DataSizeAdd(LayerSize*LayerReplayTimes);//分配层空间
		gInt32 hendsize = GetSizeOfRadarInfo()+GetSizeOfPDBlock()+GetSizeOfOffsetOfEachLayerBlock(NumberOfLayers);
		GHRESULT nResult = SetDataSize(LayerSize*LayerReplayTimes + hendsize);//分配层空间
	
		//层偏移区默认值设置
		for (int ii=0;ii<NumberOfLayers;ii++)
		{
			GetLayerOffsetIndex()[ii]=-1;
		}
		GetPDBlock()->NumberOfLayers=NumberOfLayers;
		GetPDBlock()->IndexOfLayers=hendsize;
		gInt32 TemOffset = hendsize;
		if(nResult != GS_OK)
		{
			return nResult;
		}

		//处理重复的每一层
		for (int j=0;j<LayerReplayTimes;j++)
		{
			if (i>=NumberOfLayers)
			{
				if (LayerBlockSize!=NULL)
				{
					free(LayerBlockSize);
				}
				if (LayerBlockType!=NULL)
				{
					free(LayerBlockType);
				}
				if (Para1!=NULL)
				{
					free(Para1);
				}
				if (Para2!=NULL)
				{
					free(Para2);
				}
				va_end( argp ); 
				return GE_INVALIDARG;
			}

			//写层头
			GetLayerOffsetIndex()[i]=TemOffset+j*LayerSize;
			//GetLayer(i)->LayerDivider=-1;//层分割符
			GetLayer(i)->NumberOfBlocks=CurrentBlockCount;
			GetLayer(i)->SNOfLayer=i;
			GetLayer(i)->LengthOfLayer=LayerSize;
			GetLayer(i)->IndexOfBlocks = TemOffset+j*LayerSize;

			//写每一块的空间相关数据
			for (gInt16 k=0;k<CurrentBlockCount;k++)
			{
				//GetBlock(i,k)->BlockDivider=-1;//块分割符
				switch(LayerBlockType[k])
				{
				case BLOCKTYPE_RADIAL:
					GetBlock(i,k)->LengthOfBlock=/*sizeof(DLIBlock)+*/LayerBlockSize[k];
					GetBlock(i,k)->TypeOfDataPackets=(gInt16)0x0A00;
					//((RadialHead *)GetBlockData(i,k))->PacketCode=GetBlock(i,k)->TypeOfDataPackets;
					((RadialHead *)GetBlockData(i,k))->NumberOfRadials=Para1[k];
					((RadialHead *)GetBlockData(i,k))->NumberOfGates=Para2[k];
					break;
				case BLOCKTYPE_GRID:
					GetBlock(i,k)->LengthOfBlock=/*sizeof(DLIBlock)+*/LayerBlockSize[k];
					GetBlock(i,k)->TypeOfDataPackets=(gInt16)0x0B00;
					//((GridHead*)GetBlockData(i,k))->PacketCode=GetBlock(i,k)->TypeOfDataPackets;
					((GridHead*)GetBlockData(i,k))->NumberOfRows=Para1[k];
					((GridHead*)GetBlockData(i,k))->NumberOfCols=Para2[k];
					break;
				case BLOCKTYPE_LONLATGRID:
					GetBlock(i,k)->LengthOfBlock=/*sizeof(DLIBlock)+*/LayerBlockSize[k];
					GetBlock(i,k)->TypeOfDataPackets=(gInt16)0x0B01;
					//((GridHead*)GetBlockData(i,k))->PacketCode=GetBlock(i,k)->TypeOfDataPackets;
					((GridHead*)GetBlockData(i,k))->NumberOfRows=Para1[k];
					((GridHead*)GetBlockData(i,k))->NumberOfCols=Para2[k];
					break;
				case BLOCKTYPE_STRUCT:
					GetBlock(i,k)->LengthOfBlock=/*sizeof(DLIBlock)+*/LayerBlockSize[k];
					GetBlock(i,k)->TypeOfDataPackets=(gInt16)0x0C00;
					//((StructHead*)GetBlockData(i,k))->PacketCode=GetBlock(i,k)->TypeOfDataPackets;
					((StructHead*)GetBlockData(i,k))->NumbersOfData=Para1[k];
					((StructHead*)GetBlockData(i,k))->LengthOfDataType=Para2[k];
					break;
				default:
					if (LayerBlockSize!=NULL)
					{
						free(LayerBlockSize);
					}
					if (LayerBlockType!=NULL)
					{
						free(LayerBlockType);
					}
					if (Para1!=NULL)
					{
						free(Para1);
					}
					if (Para2!=NULL)
					{
						free(Para2);
					}
					va_end( argp ); 
					return GE_INVALIDARG;
					break;
				}
			}
			//层数增加
			i++;
		}
	}



	if (LayerBlockSize!=NULL)
	{
		free(LayerBlockSize);
	}
	if (LayerBlockType!=NULL)
	{
		free(LayerBlockType);
	}
	if (Para1!=NULL)
	{
		free(Para1);
	}
	if (Para2!=NULL)
	{
		free(Para2);
	}
	va_end( argp ); 

	return GS_OK;
}

//GHRESULT ProductBase::GetSiteCode(char * pSiteCode)
//{
//	if (pSiteCode == NULL)
//	{
//		return GE_INVALIDARG;
//	}
//
//	gInt32 StartP =0;
//	*pSiteCode++ = ( gChar )ReadInt8( StartP + 2*sizeof(gInt8)); 
//	*pSiteCode++ = ( gChar )ReadInt8( StartP + 3*sizeof(gInt8)); 
//	*pSiteCode++ = ( gChar )ReadInt8( StartP + 5*sizeof(gInt8)); 
//	*pSiteCode++ = ( gChar )ReadInt8( StartP + 6*sizeof(gInt8)); 
//	*pSiteCode++ = ( gChar )ReadInt8( StartP + 7*sizeof(gInt8)); 
//	*pSiteCode++ = '\0'; 
//	return GS_OK;
//}