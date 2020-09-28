//////////////////////////////////////////////////////////////////////////
// MultiProductBase.cpp
// created by ply 20080416
// 组网产品数据通用基类，GData的子类
//////////////////////////////////////////////////////////////////////////
#include "MultiProductBase.h"


GHRESULT MultiProductBase::CreateP()
{
	return Create();
}
GHRESULT MultiProductBase::AddSiteP(const SiteInf & SiteToAdd)
{
	return AddSite(SiteToAdd);
}

GHRESULT MultiProductBase::FramALL1(gInt16 SiteNum,gInt32 LayerReplayTimes, gInt16 NumberOfBlocks,...)
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

	gInt32 headsize = sizeof(AreaInfo) + sizeof(MultiDataHead) + sizeof(SiteInf)*SiteNum + sizeof(gInt32)*LayerReplayTimes;

	GHRESULT TemRet=SetDataSize( headsize+ LayerSize*LayerReplayTimes);
	if (TemRet!=GS_OK)
	{
		if (ptr!=NULL)
		{
			free(ptr);
		}
		return TemRet;
	}

	/*gInt16 OldnLayer = GetMultiHead()->NumberOfLayers;*/
	GetMultiHead()->NumberOfLayers = LayerReplayTimes;
	GetAreaInfo()->NumberOfRadars = SiteNum;

	for (int i= 0, j = 0;i<GetMultiHead()->NumberOfLayers;i++, j++)
	{
	
		GetLayerOffsetIndex()[i] = headsize +(gInt32)LayerSize*j;
		gInt32 numset = GetLayerOffsetIndex()[i] ;
		GetLayer(i)->NumberOfBlocks=CurrentBlockCount;
		GetLayer(i)->SNOfLayer=(gInt16)i;
		GetLayer(i)->LengthOfLayer=LayerSize;
		GetLayer(i)->IndexOfBlocks=sizeof(MultiLayerHead);
	MultiLayerHead * temo = GetLayer(i);


		for (gInt16 k=0;k<CurrentBlockCount;k++)
		{
			GetMultiBlock(i,k)->SNOfBlock = k;//块分割符
			switch(LayerBlockType[k])
			{
			case MULTI_BLOCKTYPE_GRID:
				GetMultiBlock(i,k)->LengthOfBlock=LayerBlockSize[k];
				GetMultiBlock(i,k)->TypeOfDataPackets=(gInt16)0x0B00;
				//((MultiGridHead*)GetMultiBlockData(i,k))->PacketCode=GetBlock(i,k)->TypeOfDataPackets;
				((MultiGridHead*)GetMultiBlockData(i,k))->NumberOfRows=Para1[k];
				((MultiGridHead*)GetMultiBlockData(i,k))->NumberOfCols=Para2[k];
				break;
			case MULTI_BLOCKTYPE_LONLATGRID:
				GetMultiBlock(i,k)->LengthOfBlock=LayerBlockSize[k];
				GetMultiBlock(i,k)->TypeOfDataPackets=(gInt16)0x0B01;
				//((MultiGridHead*)GetBlockData(i,k))->PacketCode=GetBlock(i,k)->TypeOfDataPackets;
				((MultiGridHead*)GetMultiBlockData(i,k))->NumberOfRows=Para1[k];
				((MultiGridHead*)GetMultiBlockData(i,k))->NumberOfCols=Para2[k];
				break;
			case MULTI_BLOCKTYPE_STRUCT:
				GetMultiBlock(i,k)->LengthOfBlock=LayerBlockSize[k];
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
MultiDataHead * MultiProductBase::GetMultiHeadP()
{
	return GetMultiHead();
}
SiteInf * MultiProductBase::GetSiteP(gInt16 RadarIndex)
{
	return GetSite(RadarIndex);
}
MultiLayerHead * MultiProductBase::GetLayerP(gInt16 LayerIndex)
{
	return GetLayer(LayerIndex);
}
//gInt16 * MultiProductBase::GetLayerDataP(gInt16 LayerIndex)
//{
	//return GetLayerData(LayerIndex);
//}
gInt16 * MultiProductBase::GetLineDataP(gInt16 LayerIndex,gInt16 RowIndex)
{
	return GetBlockDataAsGrid_Data(LayerIndex, 0, RowIndex);
}

AreaInfo * MultiProductBase::GetAreaInfoP()
{
	return GetAreaInfo();
}

MultiGridHead *	MultiProductBase::GetBlockDataAsGrid_HeadP(gInt32 LayerIndex,gInt32 BlockIndex)
{
	return GetBlockDataAsGrid_Head(LayerIndex,BlockIndex);
}
