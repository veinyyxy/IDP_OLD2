#include "CCProductPretreatment2.h"

CCProductPretreatment2::CCProductPretreatment2(void)
{
}

CCProductPretreatment2::~CCProductPretreatment2(void)
{
}
FILEVOLUMN * CCProductPretreatment2::GetTArchiveCCFileVolumnP( )
{
	return GetTArchiveCCFileVolumn();
}

RADDATAHEADER * CCProductPretreatment2::GetTArchiveCCRadDataHeaderP()
{
	return GetTArchiveCCRadDataHeader();
}

RADIALHEADER * CCProductPretreatment2::GetTArchiveCCRVWP(gInt16 LayIndex,gInt16 RadialIndex)//
{

	return GetTArchiveCCRVW(LayIndex,RadialIndex);
}
gInt16 CCProductPretreatment2::GetLayerNubP()
{
	return GetLayerNub();
}
unsigned char * CCProductPretreatment2::GetTArchiveCCRP(gInt16 LayIndex,gInt16 RadialIndex)
{
	return GetTArchiveCCR(LayIndex, RadialIndex);
}
unsigned char * CCProductPretreatment2::GetTArchiveCCVP(gInt16 LayIndex,gInt16 RadialIndex)
{
	return GetTArchiveCCV(LayIndex, RadialIndex);
}
unsigned char * CCProductPretreatment2::GetTArchiveCCWP(gInt16 LayIndex,gInt16 RadialIndex)
{
	return GetTArchiveCCW(LayIndex, RadialIndex);
}

inline FILEVOLUMN * CCProductPretreatment2::GetTArchiveCCFileVolumn( )
{

	return (FILEVOLUMN*)GetPointer(0);
}
inline RADDATAHEADER * CCProductPretreatment2::GetTArchiveCCRadDataHeader()
{
	return (RADDATAHEADER *)GetPointer(sizeof(FILEVOLUMN));
}
inline RADIALHEADER * CCProductPretreatment2::GetTArchiveCCRVW(gInt16 LayIndex,gInt16 RadialIndex)
{
	RADDATAHEADER * radDataHead = GetTArchiveCCRadDataHeader();
	LAYERPARAM * layerParam = NULL;
	layerParam = &(radDataHead->LayerParam[0]);
	int offset = layerParam->Dbegin;
	for(int i=0;i<LayIndex;i++)
	{
		layerParam = &(radDataHead->LayerParam[i]);
		offset += layerParam->RecordNumber*(sizeof(RADIALHEADER)+1000*4);
	}
	offset += RadialIndex*(sizeof(RADIALHEADER)+1000*4);
	return (RADIALHEADER*)GetPointer(offset);
}

gInt16 CCProductPretreatment2::GetLayerNub()
{
	RADDATAHEADER * radDataHead = GetTArchiveCCRadDataHeader();
	LAYERPARAM * layerParam = NULL;
	layerParam = &(radDataHead->LayerParam[0]);
	unsigned char preDataType = layerParam->DataType;
	char preDataForm = layerParam->DataForm;

	for (int i=1;i<32;i++)
	{
		layerParam = &(radDataHead->LayerParam[i]);
		if(layerParam->DataType!=preDataType || layerParam->DataForm != preDataForm)
		{
			return i;
		}
		else
		{
			preDataType = layerParam->DataType;
			preDataForm = layerParam->DataForm;
		}
	}
	return 0;
}
unsigned char * CCProductPretreatment2::GetTArchiveCCR(gInt16 LayIndex,gInt16 RadialIndex)
{
	RADDATAHEADER * radDataHead = GetTArchiveCCRadDataHeader();
	LAYERPARAM * layerParam = NULL;
	layerParam = &(radDataHead->LayerParam[0]);
	int offset = layerParam->Dbegin;
	for(int i=0;i<LayIndex;i++)
	{
		layerParam = &(radDataHead->LayerParam[i]);
		offset += layerParam->RecordNumber*(sizeof(RADIALHEADER)+1000*4);
	}
	offset += RadialIndex*(sizeof(RADIALHEADER)+1000*4);
	offset += sizeof(RADIALHEADER);
	return (unsigned char *)GetPointer(offset);
}
unsigned char * CCProductPretreatment2::GetTArchiveCCV(gInt16 LayIndex,gInt16 RadialIndex)
{
	RADDATAHEADER * radDataHead = GetTArchiveCCRadDataHeader();
	LAYERPARAM * layerParam = NULL;
	layerParam = &(radDataHead->LayerParam[0]);
	int offset = layerParam->Dbegin;
	for(int i=0;i<LayIndex;i++)
	{
		layerParam = &(radDataHead->LayerParam[i]);
		offset += layerParam->RecordNumber*(sizeof(RADIALHEADER)+1000*4);
	}
	offset += RadialIndex*(sizeof(RADIALHEADER)+1000*4);
	offset += sizeof(RADIALHEADER)+2000;
	return (unsigned char *)GetPointer(offset);
}
unsigned char * CCProductPretreatment2::GetTArchiveCCW(gInt16 LayIndex,gInt16 RadialIndex)
{
	RADDATAHEADER * radDataHead = GetTArchiveCCRadDataHeader();
	LAYERPARAM * layerParam = NULL;
	layerParam = &(radDataHead->LayerParam[0]);
	int offset = layerParam->Dbegin;
	for(int i=0;i<LayIndex;i++)
	{
		layerParam = &(radDataHead->LayerParam[i]);
		offset += layerParam->RecordNumber*(sizeof(RADIALHEADER)+1000*4);
	}
	offset += RadialIndex*(sizeof(RADIALHEADER)+1000*4);
	offset += sizeof(RADIALHEADER)+3000;
	return (unsigned char *)GetPointer(offset);
}

