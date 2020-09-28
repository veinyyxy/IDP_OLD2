//////////////////////////////////////////////////////////////////////////
// SingleProductBase.inl
// created by ply 20080402
// 产品数据通用基类，GData的子类
//////////////////////////////////////////////////////////////////////////

#include <stdarg.h> 

inline RadarInfor * ProductBase::GetRadarInfo()
{
	void * p = NULL;
	GetPointer(0, p);
	return(RadarInfor * )p;
}

inline PDBlock * ProductBase::GetPDBlock()
{
	void * p = NULL;
	GetPointer (GetSizeOfRadarInfo(), p );
	return ( PDBlock * ) p;
}

inline void ProductBase::GetProductCode(gInt16& nProductCode)
{
	nProductCode = GetPDBlock()->ProductCode;
}

inline void ProductBase::GetScanTime(gInt32& sTime)
{
	sTime = GetPDBlock()->ProductTime;
}

inline void ProductBase::GetScanDate(gInt16& sDate)
{
	sDate = GetPDBlock()->ProductDate;
}


inline gInt32 * ProductBase::GetLayerOffsetIndex()
{
	return (gInt32 *)GetPointer(GetSizeOfRadarInfo() + GetSizeOfPDBlock());
}

inline gInt32 ProductBase::GetOffsetOfLayer(gInt32 LayerIndex)
{
	/*
	if (LayerIndex<GetPDBlock()->NumberOfLayers)
	{
		return ReadInt32(GetSizeOfPDBlock()+sizeof(gInt32)*LayerIndex)
	}
	else
	{
		return -1;
	}
	*/

	return (LayerIndex<GetPDBlock()->NumberOfLayers&&LayerIndex>=0)?ReadInt32(GetSizeOfRadarInfo()+GetSizeOfPDBlock()+sizeof(gInt32)*LayerIndex):-1;
}

inline DILayer * ProductBase::GetLayer(gInt32 LayerIndex)
{
	void * p = NULL;
	if (GetPointer ( GetOffsetOfLayer(LayerIndex), p )==GS_OK)
	{
		return ( DILayer * ) p;
	} 
	else
	{
		return NULL;
	}
}

inline gInt32 ProductBase::GetOffsetOfBlock(gInt32 LayerIndex,gInt32 BlockIndex)
{
	if(LayerIndex>=GetPDBlock()->NumberOfLayers||LayerIndex<0)
	{
		return -1;
	}
	if (BlockIndex>=GetLayer(LayerIndex)->NumberOfBlocks||BlockIndex<0)
	{
		return -1;
	}
	gInt32 BlockOffset=GetOffsetOfLayer(LayerIndex)+sizeof(DILayer);
	for (int i=0;i<BlockIndex;i++)
	{
		BlockOffset+=((DLIBlock*)GetPointer(BlockOffset))->LengthOfBlock;//这里错误检查不太好，以后改。
	}
	return BlockOffset;
}

inline DLIBlock * ProductBase::GetBlock(gInt32 LayerIndex,gInt32 BlockIndex)
{
	return (DLIBlock*)GetPointer(GetOffsetOfBlock(LayerIndex,BlockIndex));
}

inline gInt32 ProductBase::GetOffsetOfBlockData(gInt32 LayerIndex,gInt32 BlockIndex)
{
	return GetOffsetOfBlock(LayerIndex,BlockIndex)+sizeof(DLIBlock);
}
inline void *  ProductBase::GetBlockData(gInt32 LayerIndex,gInt32 BlockIndex)
{
	return GetPointer(GetOffsetOfBlockData(LayerIndex,BlockIndex));
}

inline BlockType ProductBase::GetBlockType(gInt32 LayerIndex,gInt32 BlockIndex)
{
	gInt16 CurrentType=GetBlock(LayerIndex,BlockIndex)->TypeOfDataPackets;
	if (CurrentType==0x0A00)
	{
		return BLOCKTYPE_RADIAL;
	}
	else if (CurrentType==0x0B00)
	{
		return BLOCKTYPE_GRID;
	}
	else if (CurrentType==0x0B01)
	{
		return BLOCKTYPE_LONLATGRID;
	}
	else if (CurrentType==0x0C00)
	{
		return BLOCKTYPE_STRUCT;
	}
	else
	{
		return BLOCKTYPE_UNDEFINED;
	}
}

inline gInt32 ProductBase::GetSizeOfRadarInfo()
{
	return sizeof(RadarInfor);
}

inline gInt32 ProductBase::GetSizeOfPDBlock()
{
	return sizeof(PDBlock);
}

inline gInt32 ProductBase::GetSizeOfOffsetOfEachLayerBlock(gInt32 NumberOfLayers)
{
	return sizeof(gInt32)*NumberOfLayers;
}

inline gInt32 ProductBase::GetSizeOfRadialBlock(gInt16 NumberOfRadials,gInt16 NumberOfGates)
{
	return sizeof(RadialHead)+NumberOfRadials*(sizeof(RadialData)+sizeof(gInt16)*NumberOfGates);
}

inline gInt32 ProductBase::GetSizeOfGridBlock(gInt16 NumberOfRows,gInt16 NumberOfCols)
{
	return sizeof(GridHead)+NumberOfRows*(sizeof(gInt16)*(1+NumberOfCols));
}

inline gInt32 ProductBase::GetSizeOfStructBlock(gInt16 NumbersOfStruct,gInt16 LengthOfEachStruct)
{
	return sizeof(StructHead)+LengthOfEachStruct*NumbersOfStruct;
}


inline RadialHead * ProductBase::GetBlockDataAsRadial_Head(gInt32 LayerIndex,gInt32 BlockIndex)
{
	if (GetBlockType(LayerIndex,BlockIndex)!=BLOCKTYPE_RADIAL)
	{
		return NULL;
	}
	return (RadialHead *)GetPointer(GetOffsetOfBlockData(LayerIndex,BlockIndex));
}
inline RadialData * ProductBase::GetBlockDataAsRadial_RadialData(gInt32 LayerIndex,gInt32 BlockIndex,gInt32 RadialIndex)
{
	if (GetBlockType(LayerIndex,BlockIndex)!=BLOCKTYPE_RADIAL)
	{
		return NULL;
	}
	return (RadialData *)GetPointer(GetOffsetOfBlockData(LayerIndex,BlockIndex)+sizeof(RadialHead)+(sizeof(RadialData)+sizeof(gInt16)*GetBlockDataAsRadial_Head(LayerIndex,BlockIndex)->NumberOfGates)*RadialIndex);
}
inline gInt16 * ProductBase::GetBlockDataAsRadial_Data(gInt32 LayerIndex,gInt32 BlockIndex,gInt32 RadialIndex)
{
	if (GetBlockType(LayerIndex,BlockIndex)!=BLOCKTYPE_RADIAL)
	{
		return NULL;
	}
	return (gInt16 *)GetPointer(GetOffsetOfBlockData(LayerIndex,BlockIndex)+sizeof(RadialHead)+(sizeof(RadialData)+sizeof(gInt16)*GetBlockDataAsRadial_Head(LayerIndex,BlockIndex)->NumberOfGates)*RadialIndex+sizeof(RadialData));
}

inline GridHead * ProductBase::GetBlockDataAsGrid_Head(gInt32 LayerIndex,gInt32 BlockIndex)
{
	if ((GetBlockType(LayerIndex,BlockIndex)!=BLOCKTYPE_GRID)&&(GetBlockType(LayerIndex,BlockIndex)!=BLOCKTYPE_LONLATGRID))
	{
		return NULL;
	}
	return (GridHead *)GetPointer(GetOffsetOfBlockData(LayerIndex,BlockIndex));
}
inline GridRowData * ProductBase::GetBlockDataAsGrid_RowData(gInt32 LayerIndex,gInt32 BlockIndex,gInt32 RowIndex)
{
	if ((GetBlockType(LayerIndex,BlockIndex)!=BLOCKTYPE_GRID)&&(GetBlockType(LayerIndex,BlockIndex)!=BLOCKTYPE_LONLATGRID))
	{
		return NULL;
	}
	return (GridRowData *)GetPointer(GetOffsetOfBlockData(LayerIndex,BlockIndex)+sizeof(tagGridHead)+(sizeof(GridRowData)+sizeof(gInt16)*GetBlockDataAsGrid_Head(LayerIndex,BlockIndex)->NumberOfCols)*RowIndex);
}
inline gInt16 * ProductBase::GetBlockDataAsGrid_Data(gInt32 LayerIndex,gInt32 BlockIndex,gInt32 RowIndex)
{
	if ((GetBlockType(LayerIndex,BlockIndex)!=BLOCKTYPE_GRID)&&(GetBlockType(LayerIndex,BlockIndex)!=BLOCKTYPE_LONLATGRID))
	{
		return NULL;
	}
	return (gInt16 *)GetPointer(GetOffsetOfBlockData(LayerIndex,BlockIndex)+sizeof(tagGridHead)+(sizeof(GridRowData)+sizeof(gInt16)*GetBlockDataAsGrid_Head(LayerIndex,BlockIndex)->NumberOfCols)*RowIndex+sizeof(GridRowData));
}

inline StructHead * ProductBase::GetBlockDataAsStruct_Head(gInt32 LayerIndex,gInt32 BlockIndex)
{
	if (GetBlockType(LayerIndex,BlockIndex)!=BLOCKTYPE_STRUCT)
	{
		return NULL;
	}
	return (StructHead *)GetPointer(GetOffsetOfBlockData(LayerIndex,BlockIndex));
}
inline void * ProductBase::GetBlockDataAsStruct_Data(gInt32 LayerIndex,gInt32 BlockIndex)
{
	if (GetBlockType(LayerIndex,BlockIndex)!=BLOCKTYPE_STRUCT)
	{
		return NULL;
	}
	return (void *)GetPointer(GetOffsetOfBlockData(LayerIndex,BlockIndex)+sizeof(StructHead));
}

