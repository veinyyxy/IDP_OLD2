//////////////////////////////////////////////////////////////////////////
// MultiProductBase.inl
// created by ply 20080416
// 组网产品数据通用基类，GData的子类
//////////////////////////////////////////////////////////////////////////
#include <malloc.h>
#include <wchar.h>
#include <stdio.h>
#include "memset32.h"
#include <stdarg.h> 


inline GHRESULT MultiProductBase::Create()
{
	/*GHRESULT TemRet=SetDataSize(sizeof(MultiDataHead) + sizeof(AreaInfo));
	if (TemRet!=GS_OK)
	{
		return TemRet;
	}*/

	//GetAreaInfo()->AreaCode[0] = '\0';
	memset(GetAreaInfo()->AreaCode, 0,8);
	GetAreaInfo()->LongitudeOfWest = 0;
	GetAreaInfo()->LatitudeOfNorth = 0;
	GetAreaInfo()->LongitudeOfEast = 0;
	GetAreaInfo()->LatitudeOfSouth = 0;
	//GetAreaInfo()->NumberOfRadars = 0;
	memset(GetAreaInfo()->RadarSiteInfo ,0x00,sizeof(gInt32)*32);

	//GetMultiHead()->ProductCode = 249;
	GetMultiHead()->ProductDate = 0;
	GetMultiHead()->ProductTime = 0;
	GetMultiHead()->GenerationDateOfProduct =0 ;
	GetMultiHead()->GenerationTimeOfProduct = 0;
	GetMultiHead()->ProductDependentCount = 0;
	memset(GetMultiHead()->ProductDependent,0,sizeof(gInt16)*20);
	/*GetMultiHead()->NumberOfLayers = 0;*/
	GetMultiHead()->IndexOfLayers = sizeof(AreaInfo) + sizeof(MultiDataHead);
	return GS_OK;
}
inline GHRESULT MultiProductBase::AddSite(const SiteInf & SiteToAdd)
{
	if (SiteToAdd.SiteCode[0]!= gChar('Z')||SiteToAdd.SiteCode[1]!=gChar('9'))
	{
		return GE_INVALIDARG;
	}
	//GHRESULT TemRet=DataSizeAdd(sizeof(SiteInf));
	//if (TemRet!=GS_OK)
	//{
	//	return TemRet;
	//}
	//void *p=NULL;
	//int MoveSize=0;
	//if (GetMultiHead()->NumberOfLayers!=0)
	//{
	//	MoveSize=GetDataSize() - GetSizeBeforeOffsetIndex() - sizeof(SiteInf); ////////////////////////////???????????????????????
	//	try
	//	{
	//		p=malloc(MoveSize);
	//	}
	//	catch (...)
	//	{
	//		p = NULL;
	//	}
	//	if (p==NULL)
	//	{
	//		return GE_NOENOUGHMEMORY;
	//	}
	//	ReadData(GetSizeBeforeOffsetIndex(),READ_TYPE_UNKNOWN,p,MoveSize);
	//}
	gInt32 RadarNo=-1;

	Gsscanf( (&(SiteToAdd.SiteCode[2])), "%d", &RadarNo);

	if (RadarNo==-1)
	{
		return GE_INVALIDARG;
	}
	GetAreaInfo()->RadarSiteInfo[RadarNo/32]|=(0x00000001<<(RadarNo%32));

	gInt32 radarIndex =0;
	radarIndex = (GetMultiHead()->IndexOfLayers - GetOffsetOfSiteInf())/sizeof(SiteInf);
	*(GetSite(radarIndex)) = SiteToAdd;
	//*(GetSite(GetAreaInfo()->NumberOfRadars ++))=SiteToAdd;
	GetMultiHead()->IndexOfLayers+=sizeof(SiteInf);        

	/*if (GetMultiHead()->NumberOfLayers!=0)
	{
		WriteData(GetSizeBeforeOffsetIndex(),WRITE_TYPE_UNKNOWN,p,MoveSize);
		free(p);
	}*/

	/*for (gInt16 LayerIndex = 0; LayerIndex < GetMultiHead()->NumberOfLayers; LayerIndex ++)
	{
		GetLayerOffsetIndex()[LayerIndex] += sizeof(SiteInf);
	}*/

	return GS_OK;
}

inline GHRESULT MultiProductBase::AddLayer(gInt32 LayerReplayTimes, gInt16 NumberOfBlocks,.../*MULTI_BLOCKTYPE_RADIAL,gInt16 NumberOfRadials,gInt16 NumberOfGates...MULTI_BLOCKTYPE_GRID,gInt16 NumberOfRows,gInt16 NumberOfCols...,MUTI_LAYER,gInt32 LayerReplayTimes,gInt16 NumberOfBlocks,gInt16 MULTI_BLOCKTYPE_STRUCT,gInt16 NumbersOfStruct,gInt16 LengthOfEachStruct,...*/)
{
	va_list argp;
	va_start( argp, NumberOfBlocks);
	MultiBlockType CurrentBlockType;
	gInt16 CurrentPara1,CurrentPara2;
	gInt16 * Para1=NULL;
	gInt16 * Para2=NULL;
	gInt16 CurrentBlockCount;
	gInt32 * LayerBlockSize=NULL;
	MultiBlockType * LayerBlockType=NULL;
	gInt32 LayerSize=0;
	//gInt32 LayerReplayTimes=0;
	gInt32 CurrentSize=0;
	void * p=NULL;

	gInt16 LayerOffsetIndexAddSize=0;
	bool IsNeedMove=false;
	void * ptr=NULL;
	gInt32 MoveSize=0;

	if (GetMultiHead()->NumberOfLayers==0)
	{
		LayerOffsetIndexAddSize=((LayerReplayTimes+64)/64)*64*sizeof(gInt32);//不太明白原来+64的意思，是为了防止AddLayer时频繁的realloc吗？
	}
	else if ((GetLayerOffsetIndex()[0]-GetSizeBeforeOffsetIndex())<(GetMultiHead()->NumberOfLayers+LayerReplayTimes)*(gInt32)(sizeof(gInt32)))
	{
		IsNeedMove=true;
		LayerOffsetIndexAddSize=((GetMultiHead()->NumberOfLayers+LayerReplayTimes+64)/64)*64*sizeof(gInt32);
		MoveSize=GetDataSize()-GetLayerOffsetIndex()[0];
		try
		{
			ptr=malloc(MoveSize);
		}
		catch (...)
		{
			ptr = NULL;
		}
		if (ptr==NULL)
		{
			return GE_NOENOUGHMEMORY;
		}
		ReadData(GetLayerOffsetIndex()[0],READ_TYPE_UNKNOWN,ptr,MoveSize);
	}
	gInt32 OldSize=GetDataSize();
	LayerSize = sizeof(MultiLayerHead);
	CurrentBlockCount = NumberOfBlocks;
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
			p = realloc((void *)LayerBlockType,sizeof(MultiBlockType)*CurrentBlockCount);
		}
		catch (...)
		{
			p = NULL;
		}
		if (p!=NULL)
		{
			LayerBlockType=(MultiBlockType*)p;
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
		CurrentBlockType = va_arg(argp, MultiBlockType);
#else
		CurrentBlockType = (MultiBlockType)va_arg(argp, int);
#endif

		LayerBlockType[j]=CurrentBlockType;
		LayerBlockSize[j]=sizeof(MultiDLIBlock);

		switch(CurrentBlockType)
		{
		case MULTI_BLOCKTYPE_GRID:
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
		case MULTI_BLOCKTYPE_LONLATGRID:
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
		case MULTI_BLOCKTYPE_STRUCT:
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

	GHRESULT TemRet=DataSizeAdd(LayerOffsetIndexAddSize + LayerSize*LayerReplayTimes);
	if (TemRet!=GS_OK)
	{
		if (ptr!=NULL)
		{
			free(ptr);
		}
		return TemRet;
	}

	if (IsNeedMove)
	{
		for (int i=0;i<GetMultiHead()->NumberOfLayers;i++)
		{
			GetLayerOffsetIndex()[i] += sizeof(gInt32)*LayerReplayTimes; 
		}
		WriteData(GetLayerOffsetIndex()[0],WRITE_TYPE_UNKNOWN,ptr,MoveSize);
		free(ptr);
	}

	gInt16 OldnLayer = GetMultiHead()->NumberOfLayers;
	GetMultiHead()->NumberOfLayers += LayerReplayTimes;

	for (int i= OldnLayer, j = 0;i<GetMultiHead()->NumberOfLayers;i++, j++)
	{
		GetLayerOffsetIndex()[i] = OldSize+LayerOffsetIndexAddSize+LayerSize*j;
		GetLayer(i)->NumberOfBlocks=CurrentBlockCount;
		GetLayer(i)->SNOfLayer=i;
		GetLayer(i)->LengthOfLayer=LayerSize;
		GetLayer(i)->IndexOfBlocks=sizeof(MultiLayerHead);

		for (gInt16 k=0;k<CurrentBlockCount;k++)
		{
			GetMultiBlock(i,k)->SNOfBlock = k;//块分割符
			switch(LayerBlockType[k])
			{
			case MULTI_BLOCKTYPE_GRID:
				GetMultiBlock(i,k)->LengthOfBlock=/*sizeof(MultiDLIBlock)+*/LayerBlockSize[k];
				GetMultiBlock(i,k)->TypeOfDataPackets=(gInt16)0x0B00;
				//((MultiGridHead*)GetMultiBlockData(i,k))->PacketCode=GetBlock(i,k)->TypeOfDataPackets;
				((MultiGridHead*)GetMultiBlockData(i,k))->NumberOfRows=Para1[k];
				((MultiGridHead*)GetMultiBlockData(i,k))->NumberOfCols=Para2[k];
				break;
			case MULTI_BLOCKTYPE_LONLATGRID:
				GetMultiBlock(i,k)->LengthOfBlock=/*sizeof(MultiDLIBlock)+*/LayerBlockSize[k];
				GetMultiBlock(i,k)->TypeOfDataPackets=(gInt16)0x0B01;
				//((MultiGridHead*)GetBlockData(i,k))->PacketCode=GetBlock(i,k)->TypeOfDataPackets;
				((MultiGridHead*)GetMultiBlockData(i,k))->NumberOfRows=Para1[k];
				((MultiGridHead*)GetMultiBlockData(i,k))->NumberOfCols=Para2[k];
				break;
			case MULTI_BLOCKTYPE_STRUCT:
				GetMultiBlock(i,k)->LengthOfBlock=/*sizeof(MultiDLIBlock)+*/LayerBlockSize[k];
				GetMultiBlock(i,k)->TypeOfDataPackets=(gInt16)0x0C00;
				//((StructHead*)GetBlockData(i,k))->PacketCode=GetBlock(i,k)->TypeOfDataPackets;
				((MultiStructHead*)GetMultiBlockData(i,k))->NumbersOfData=Para1[k];
				((MultiStructHead*)GetMultiBlockData(i,k))->LengthOfDataType=Para2[k];
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

inline void MultiProductBase::GetProductCode(gInt16& nProductCode)
{
	nProductCode = GetMultiHeadP()->ProductCode;
}

inline AreaInfo * MultiProductBase::GetAreaInfo()
{
	return (AreaInfo *)GetPointer(0);
}
inline MultiDataHead * MultiProductBase::GetMultiHead()
{
	return (MultiDataHead *)GetPointer(sizeof(AreaInfo));
}
inline SiteInf *MultiProductBase::GetSite(gInt16 RadarIndex)
{
	return (SiteInf *)GetPointer(GetOffsetOfSiteInf() + sizeof(SiteInf)*RadarIndex);
}
inline MultiLayerHead * MultiProductBase::GetLayer(gInt16 LayerIndex)
{
	gInt32 temp =  GetLayerOffsetIndex()[LayerIndex];
	return (MultiLayerHead *)GetPointer(GetLayerOffsetIndex()[LayerIndex]);
}

inline gInt32 MultiProductBase::GetOffsetOfSiteInf()
{
	return sizeof(MultiDataHead) + sizeof(AreaInfo);
}

inline gInt32 MultiProductBase::GetSizeOfGridBlock(gInt16 NumberOfRows,gInt16 NumberOfCols)
{
	return sizeof(MultiGridHead)+NumberOfRows*(sizeof(gInt16)*NumberOfCols + sizeof(MultiGridData));
}

inline gInt32 MultiProductBase::GetSizeOfStructBlock(gInt16 NumbersOfStruct,gInt16 LengthOfEachStruct)
{
	return sizeof(MultiStructHead)+LengthOfEachStruct*NumbersOfStruct;
}

inline gInt32 * MultiProductBase::GetLayerOffsetIndex()
{
	gInt16 temp = GetOffsetOfSiteInf() + sizeof(SiteInf) * GetAreaInfo()->NumberOfRadars;
	return (gInt32 *)GetPointer(GetOffsetOfSiteInf() + sizeof(SiteInf) * GetAreaInfo()->NumberOfRadars);
}

inline gInt32 MultiProductBase::GetSizeBeforeOffsetIndex()
{
	return GetOffsetOfSiteInf() + sizeof(SiteInf) * GetAreaInfo()->NumberOfRadars;
}

inline gInt32 MultiProductBase::GetOffsetOfLayer(gInt32 LayerIndex)
{
	return (LayerIndex<GetMultiHead()->NumberOfLayers&&LayerIndex>=0)?ReadInt32(GetSizeBeforeOffsetIndex()+sizeof(gInt32)*LayerIndex):-1;
}

inline MultiDLIBlock * MultiProductBase::GetMultiBlock(gInt32 LayerIndex,gInt32 BlockIndex)
{
	return (MultiDLIBlock*)GetPointer(GetOffsetOfBlock(LayerIndex,BlockIndex));
}

inline gInt32 MultiProductBase::GetOffsetOfBlock(gInt32 LayerIndex,gInt32 BlockIndex)
{
	if(LayerIndex>=GetMultiHead()->NumberOfLayers||LayerIndex<0)
	{
		return -1;
	}
	MultiLayerHead *temp = GetLayer(LayerIndex);
	if (BlockIndex>=GetLayer(LayerIndex)->NumberOfBlocks||BlockIndex<0)
	{
		return -1;
	}
	gInt32 BlockOffset=GetOffsetOfLayer(LayerIndex)+sizeof(MultiLayerHead);
	for (int i=0;i<BlockIndex;i++)
	{
		BlockOffset+=((MultiDLIBlock*)GetPointer(BlockOffset))->LengthOfBlock;//这里错误检查不太好，以后改。
	}
	return BlockOffset;
}

inline void *  MultiProductBase::GetMultiBlockData(gInt32 LayerIndex,gInt32 BlockIndex)
{
	return GetPointer(GetOffsetOfBlockData(LayerIndex,BlockIndex));
}

inline gInt32 MultiProductBase::GetOffsetOfBlockData(gInt32 LayerIndex,gInt32 BlockIndex)
{
	return GetOffsetOfBlock(LayerIndex,BlockIndex)+sizeof(MultiDLIBlock);
}

inline MultiGridHead * MultiProductBase::GetBlockDataAsGrid_Head(gInt32 LayerIndex,gInt32 BlockIndex)
{
	if ((GetMultiBlockType(LayerIndex,BlockIndex)!=MULTI_BLOCKTYPE_GRID)&&(GetMultiBlockType(LayerIndex,BlockIndex)!=MULTI_BLOCKTYPE_LONLATGRID))
	{
		return NULL;
	}
	return (MultiGridHead *)GetPointer(GetOffsetOfBlockData(LayerIndex,BlockIndex));
}
inline MultiGridData * MultiProductBase::GetBlockDataAsGrid_RowData(gInt32 LayerIndex,gInt32 BlockIndex,gInt32 RowIndex)
{
	if ((GetMultiBlockType(LayerIndex,BlockIndex)!=MULTI_BLOCKTYPE_GRID)&&(GetMultiBlockType(LayerIndex,BlockIndex)!=MULTI_BLOCKTYPE_LONLATGRID))
	{
		return NULL;
	}
	return (MultiGridData *)GetPointer(GetOffsetOfBlockData(LayerIndex,BlockIndex)+sizeof(MultiGridHead)+(sizeof(MultiGridData)+sizeof(gInt16)*GetBlockDataAsGrid_Head(LayerIndex,BlockIndex)->NumberOfCols)*RowIndex);
}
inline gInt16 * MultiProductBase::GetBlockDataAsGrid_Data(gInt32 LayerIndex,gInt32 BlockIndex,gInt32 RowIndex)
{
	if ((GetMultiBlockType(LayerIndex,BlockIndex)!=MULTI_BLOCKTYPE_GRID)&&(GetMultiBlockType(LayerIndex,BlockIndex)!=MULTI_BLOCKTYPE_LONLATGRID))
	{
		return NULL;
	}
	return (gInt16 *)GetPointer(GetOffsetOfBlockData(LayerIndex,BlockIndex)+sizeof(MultiGridHead)+(sizeof(MultiGridData)+sizeof(gInt16)*GetBlockDataAsGrid_Head(LayerIndex,BlockIndex)->NumberOfCols)*RowIndex+sizeof(MultiGridData));
}
inline MultiStructHead * MultiProductBase::GetBlockDataAsStruct_Head(gInt32 LayerIndex,gInt32 BlockIndex)
{
	if (GetMultiBlockType(LayerIndex,BlockIndex)!=MULTI_BLOCKTYPE_STRUCT)
	{
		return NULL;
	}
	return (MultiStructHead *)GetPointer(GetOffsetOfBlockData(LayerIndex,BlockIndex));
}
inline void * MultiProductBase::GetBlockDataAsStruct_Data(gInt32 LayerIndex,gInt32 BlockIndex)
{
	if (GetMultiBlockType(LayerIndex,BlockIndex)!=MULTI_BLOCKTYPE_STRUCT)
	{
		return NULL;
	}
	return (void *)GetPointer(GetOffsetOfBlockData(LayerIndex,BlockIndex)+sizeof(MultiStructHead));
}
inline MultiBlockType MultiProductBase::GetMultiBlockType(gInt32 LayerIndex,gInt32 BlockIndex)
{
	gInt16 CurrentType=GetMultiBlock(LayerIndex,BlockIndex)->TypeOfDataPackets;
	if (CurrentType==0x0B00)
	{
		return MULTI_BLOCKTYPE_GRID;
	}
	else if (CurrentType==0x0B01)
	{
		return MULTI_BLOCKTYPE_LONLATGRID;
	}
	else if (CurrentType==0x0C00)
	{
		return MULTI_BLOCKTYPE_STRUCT;
	}
	else
	{
		return MULTI_BLOCKTYPE_UNDEFINED;
	}
}
