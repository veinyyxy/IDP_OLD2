//////////////////////////////////////////////////////////////////////////
// StandardRadarData.cpp
// created by ply 20080325
// 本系统统一的雷达基数据格式
//////////////////////////////////////////////////////////////////////////

#include "StandardRadarDataX.h"
StandardRadarData::StandardRadarData()
{
	;
}

StandardRadarData::~StandardRadarData()
{
	;
}

GRADARDATAHEADER * StandardRadarData::GetHead()
{
	void * p = NULL;
	GetPointer ( 0, p );
	return ( GRADARDATAHEADER * ) p;
}

gInt32 StandardRadarData::GetRVWSize(gInt32 ElevationIndex)
{
	GELEVATIONDATAHEAD * ptr=GetElevationHead(ElevationIndex);
	if (ptr==NULL)
	{
		return -1;
	}
	return GetSize() <= 0 ? -1 : ( sizeof ( RVWDATAHEAD ) + ( sizeof ( gInt16 ) * ( ( ptr->ReflectivityGates ) + ( ptr->DopplerGates ) * 2 ) ) ) ;
}


gInt32 StandardRadarData::GetRoffset(gInt32 ElevationIndex)
{
	return  GetSize() <= 0 ? -1 : sizeof ( RVWDATAHEAD );
}

gInt32 StandardRadarData::GetVoffset(gInt32 ElevationIndex)
{
	GELEVATIONDATAHEAD * ptr=GetElevationHead(ElevationIndex);
	if (ptr==NULL)
	{
		return -1;
	}
	return  GetSize() <= 0 ? -1 : ( sizeof ( RVWDATAHEAD ) + ( sizeof ( gInt16 ) * ( ptr->ReflectivityGates ) ) ) ;
}

gInt32 StandardRadarData::GetWoffset(gInt32 ElevationIndex)
{
	GELEVATIONDATAHEAD * ptr=GetElevationHead(ElevationIndex);
	if (ptr==NULL)
	{
		return -1;
	}
	return  GetSize() <= 0 ? -1 : ( sizeof ( RVWDATAHEAD ) + ( sizeof ( gInt16 ) * ( ( ptr->ReflectivityGates ) + ( ptr->DopplerGates ) ) ) ) ;
}

GELEVATIONDATAHEAD * StandardRadarData::GetElevationHead ( gInt32 Index )//第Index层的层头地址
{
	if ( Index >= GetHead()->ElevationCount )
	{
		return ( GELEVATIONDATAHEAD * ) NULL;
	}
	void * p = NULL;
	GetPointer ( GetIndexOfElevationData()[Index], p );	

	return ( GELEVATIONDATAHEAD * ) p;
}

RVWDATAHEAD * StandardRadarData::GetRadarData ( gInt32 ElevationIndex, gInt32 AzimuthIndex )
{
	if ( ElevationIndex >= GetHead()->ElevationCount )
	{
		return ( RVWDATAHEAD * ) NULL;
	}
	GELEVATIONDATAHEAD * ptr=GetElevationHead(ElevationIndex);
	if (ptr==NULL)
	{
		return NULL;
	}
	if ( AzimuthIndex >= ptr->AzimuthNumber )
	{
		return ( RVWDATAHEAD * ) NULL;
	}

	void * p = NULL;
	GetPointer ( GetIndexOfElevationData()[ElevationIndex] + GetIndexOfAzimuthData(ElevationIndex)[AzimuthIndex], p );
	return ( RVWDATAHEAD * ) p;
}

gInt16 * StandardRadarData::GetRadarDataR ( gInt32 ElevationIndex, gInt32 AzimuthIndex )
{
	if ( ElevationIndex > GetHead()->ElevationCount )
	{
		return ( gInt16 * ) NULL;
	}
	GELEVATIONDATAHEAD * ptr=GetElevationHead(ElevationIndex);
	if (ptr==NULL)
	{
		return NULL;
	}
	if ( AzimuthIndex > ptr->AzimuthNumber )
	{
		return ( gInt16 * ) NULL;
	}
	void * p = NULL;
	GetPointer ( GetIndexOfElevationData()[ElevationIndex] +  GetIndexOfAzimuthData(ElevationIndex)[AzimuthIndex] + GetRoffset(ElevationIndex), p );
	return ( gInt16 * ) p;
}

gInt16 * StandardRadarData::GetRadarDataV ( gInt32 ElevationIndex, gInt32 AzimuthIndex )
{
	if ( ElevationIndex > GetHead()->ElevationCount )
	{
		return ( gInt16 * ) NULL;
	}
	GELEVATIONDATAHEAD * ptr=GetElevationHead(ElevationIndex);
	if (ptr==NULL)
	{
		return NULL;
	}
	if ( AzimuthIndex > ptr->AzimuthNumber )
	{
		return ( gInt16 * ) NULL;
	}

	void * p = NULL;
	GetPointer ( GetIndexOfElevationData()[ElevationIndex] +  GetIndexOfAzimuthData(ElevationIndex)[AzimuthIndex] + GetVoffset(ElevationIndex), p );
	return ( gInt16 * ) p;
}

gInt16 * StandardRadarData::GetRadarDataW ( gInt32 ElevationIndex, gInt32 AzimuthIndex )
{
	if ( ElevationIndex > GetHead()->ElevationCount )
	{
		return ( gInt16 * ) NULL;
	}
	GELEVATIONDATAHEAD * ptr=GetElevationHead(ElevationIndex);
	if (ptr==NULL)
	{
		return NULL;
	}
	if ( AzimuthIndex > ptr->AzimuthNumber )
	{
		return ( gInt16 * ) NULL;
	}

	void * p = NULL;
	GetPointer ( GetIndexOfElevationData()[ElevationIndex] + GetIndexOfAzimuthData(ElevationIndex)[AzimuthIndex] + GetWoffset(ElevationIndex), p );
	return ( gInt16 * ) p;
}

GRADARDATAHEADER * StandardRadarData::GetHeadP()
{
	return  GetHead();
}

GELEVATIONDATAHEAD * StandardRadarData::GetElevationHeadP ( gInt32 Index )
{
	return GetElevationHead ( Index );
}

RVWDATAHEAD * StandardRadarData::GetRadarDataP ( gInt32 ElevationIndex, gInt32 AzimuthIndex )
{
	return GetRadarData ( ElevationIndex, AzimuthIndex );
}
gInt32 * StandardRadarData::GetIndexOfElevationData()//层数据指针的偏差数组
{
	void * p = NULL;
	GetPointer(sizeof(GRADARDATAHEADER),p);
	return ( gInt32 * ) p;
}

gInt32 * StandardRadarData::GetIndexOfAzimuthData(gInt32 ElevationIndex)
{
	return (gInt32 *)GetPointer( GetIndexOfElevationData()[ElevationIndex]+sizeof(GELEVATIONDATAHEAD));
}

GHRESULT StandardRadarData::WriteHeadAndElevationHead( GRADARDATAHEADER * HeadToWrite , GELEVATIONDATAHEAD * ElevationHeadToWrite,gInt32 evlNum)
{
	GHRESULT TemRet;
	gInt32 headsize = sizeof(GRADARDATAHEADER);
	gInt32 totalsize =0;
	totalsize = headsize + (sizeof(GELEVATIONDATAHEAD)+ sizeof(gInt32))*evlNum;

	gInt32 *SizeOfEveryRadialOfLayer = NULL;
	SizeOfEveryRadialOfLayer = new gInt32[evlNum];

	for(int i=0; i<evlNum; i++)
	{
		SizeOfEveryRadialOfLayer[i] = ( sizeof ( RVWDATAHEAD ) + ( sizeof ( gInt16 ) * ( ( ElevationHeadToWrite[i].ReflectivityGates ) 
			+ ( ElevationHeadToWrite[i].DopplerGates ) * 2 ) ) );

		totalsize += ElevationHeadToWrite[i].AzimuthNumber * ( SizeOfEveryRadialOfLayer[i] + sizeof(gInt32) );
	}

	////////////////////
	if ((TemRet=SetDataSize(totalsize)!=GS_OK))
	{
		return TemRet;
	}

	if ((TemRet=WriteData(0,WRITE_TYPE_UNKNOWN,HeadToWrite,sizeof(GRADARDATAHEADER)))!=GS_OK)
	{
		return TemRet;
	}
	//m_TemInt[0]=sizeof(GRADARDATAHEADER);
	m_TemInt[0]=sizeof(GRADARDATAHEADER)+sizeof(gInt32)*evlNum;

	GRADARDATAHEADER HeadToWrite1;

	for(int i=0; i<evlNum; i++)
	{	
		if(i == 0)
			GetIndexOfElevationData()[i] = m_TemInt[0];//GetHead()->IndexOfElevationData[1]=m_TemInt[0];	
		else
		{
			gInt16 AzimuthNumber = ElevationHeadToWrite[i-1].AzimuthNumber;
			GetIndexOfElevationData()[i] = GetIndexOfElevationData()[i-1] + sizeof(GELEVATIONDATAHEAD)+sizeof(gInt32) * AzimuthNumber + AzimuthNumber * (SizeOfEveryRadialOfLayer[i-1] );
		}	//GetHead()->IndexOfElevationData[i+1] =GetHead()->IndexOfElevationData[i] + ElevationHeadToWrite[i-1].AzimuthNumber * (SizeOfEveryRadialOfLayer[i-1] ) +sizeof(GELEVATIONDATAHEAD); 
		/* gInt16 tee;
		if( i>=1)
		tee = ElevationHeadToWrite[i-1].AzimuthNumber;*/

		gInt32 tpsize = GetIndexOfElevationData()[i];
		/*HeadToWrite1 = *(GetHead());*/
		if(tpsize > totalsize)
			return GE_FAIL;

		if ((TemRet=WriteData(tpsize,WRITE_TYPE_UNKNOWN,&ElevationHeadToWrite[i],sizeof(GELEVATIONDATAHEAD)))!=GS_OK)
		{
			return TemRet;
		}
	}

	for (int i=0;i<evlNum;i++)
	{
		for( int j=0;j< ElevationHeadToWrite[i].AzimuthNumber;j++)
		{
			GetIndexOfAzimuthData(i)[j] = sizeof(GELEVATIONDATAHEAD)+sizeof(gInt32)*ElevationHeadToWrite[i].AzimuthNumber+(j)*SizeOfEveryRadialOfLayer[i];
		}
			//GetElevationHead(i)->IndexOfAzimuthData[j]=sizeof(GELEVATIONDATAHEAD)+(j-1)*SizeOfEveryRadialOfLayer[i];
	}
	if(SizeOfEveryRadialOfLayer !=NULL)
	{
		delete SizeOfEveryRadialOfLayer;
		SizeOfEveryRadialOfLayer = NULL;
	}
	return GS_OK;
}

GHRESULT StandardRadarData::WriteHead ( GRADARDATAHEADER * HeadToWrite )
{
	GHRESULT TemRet;
	gInt16 ElevationCount = HeadToWrite->ElevationCount;
	if ((TemRet=SetDataSize(sizeof(GRADARDATAHEADER)+sizeof(gInt32)*ElevationCount))!=GS_OK)
	{
		return TemRet;
	}
	if ((TemRet=WriteData(0,WRITE_TYPE_UNKNOWN,HeadToWrite,sizeof(GRADARDATAHEADER)))!=GS_OK)
	{
		return TemRet;
	}
	m_TemInt[0]=sizeof(GRADARDATAHEADER)+sizeof(gInt32)*ElevationCount;
	return GS_OK;
}
//remove
// GHRESULT StandardRadarDataX::ResizeFromHead()
// {
// 	GHRESULT TemRet;
// 	gInt32 ElevationSize=GetRVWSize()*(GetHead()->maxAziNum)+sizeof(GELEVATIONDATAHEADX);
// 	if ((TemRet=SetDataSize(GetSize()+GetHead()->NumberOfElevation*ElevationSize))!=GS_OK)
// 	{
// 		return TemRet;
// 	}
// 	for (int i=1;i<=GetHead()->NumberOfElevation;i++)
// 	{
// 		GetHead()->IndexOfElevationData[i]=sizeof(GRADARDATAHEADERX)+ElevationSize*i;
// 	}
// 	return GS_OK;
//}
//在这先分配头空间，填写头偏移，再写入后分配径向空间，填写径向偏移
GHRESULT StandardRadarData::WriteElevationHead (gInt32 Index , GELEVATIONDATAHEAD * ElevationHeadToWrite )
{
	GHRESULT TemRet;
	gInt16 AzimuthNumber = ElevationHeadToWrite->AzimuthNumber;
	if ((TemRet=SetDataSize(GetSize()+sizeof(GELEVATIONDATAHEAD)+sizeof(gInt32)*AzimuthNumber))!=GS_OK)
	{
		return TemRet;
	}
	GetIndexOfElevationData()[Index]=m_TemInt[0];
	if ((TemRet=WriteData(GetIndexOfElevationData()[Index],WRITE_TYPE_UNKNOWN,ElevationHeadToWrite,sizeof(GELEVATIONDATAHEAD)))!=GS_OK)
	{
		return TemRet;
	}
	gInt32 RVWSize=GetRVWSize(Index);
	GELEVATIONDATAHEAD * ptr=GetElevationHead(Index);
	if (ptr==NULL || RVWSize==-1)
	{
		return GS_FALSE;
	}
	gInt32 RVWCount=ptr->AzimuthNumber;
	if ((TemRet=SetDataSize(GetSize()+RVWSize*RVWCount))!=GS_OK)
	{
		return TemRet;
	}
	m_TemInt[0]+= (sizeof(GELEVATIONDATAHEAD)+sizeof(gInt32)*AzimuthNumber+RVWSize*RVWCount);
	for (int i=0;i<RVWCount;i++)
	{
		GetIndexOfAzimuthData(Index)[i]=sizeof(GELEVATIONDATAHEAD)+sizeof(gInt32)*AzimuthNumber+i*RVWSize;
	}
	return GS_OK;
}
GHRESULT StandardRadarData::WriteRVWHead ( gInt32 ElevationIndex, gInt32 AzimuthIndex , RVWDATAHEAD * RvwHeadToWrite )
{
	return WriteData(GetIndexOfElevationData()[ElevationIndex] + GetIndexOfAzimuthData(ElevationIndex)[AzimuthIndex],WRITE_TYPE_UNKNOWN,RvwHeadToWrite,sizeof(RVWDATAHEAD));
}
GHRESULT StandardRadarData::WriteRVWData ( gInt32 ElevationIndex, gInt32 AzimuthIndex , void * DataToWrite )
{
	return WriteData(GetIndexOfElevationData()[ElevationIndex] + GetIndexOfAzimuthData(ElevationIndex)[AzimuthIndex],WRITE_TYPE_UNKNOWN,DataToWrite, GetRVWSize(ElevationIndex));
}
GHRESULT StandardRadarData::WritRadarDataR ( gInt32 ElevationIndex, gInt32 AzimuthIndex ,gInt16 * RDataToWrite)
{
	return WriteData(GetIndexOfElevationData()[ElevationIndex] + GetIndexOfAzimuthData(ElevationIndex)[AzimuthIndex] + GetRoffset(ElevationIndex),WRITE_TYPE_UNKNOWN,RDataToWrite,( sizeof ( gInt16 ) * ( GetElevationHead(ElevationIndex)->ReflectivityGates ) ) );
}
GHRESULT StandardRadarData::WritRadarDataV ( gInt32 ElevationIndex, gInt32 AzimuthIndex ,gInt16 * VDataToWrite )
{
	return WriteData(GetIndexOfElevationData()[ElevationIndex] + GetIndexOfAzimuthData(ElevationIndex)[AzimuthIndex] + GetVoffset(ElevationIndex),WRITE_TYPE_UNKNOWN,VDataToWrite,( sizeof ( gInt16 ) * ( GetElevationHead(ElevationIndex)->DopplerGates ) ) );
}
GHRESULT StandardRadarData::WritRadarDataW ( gInt32 ElevationIndex, gInt32 AzimuthIndex ,gInt16 * WDataToWrite )
{
	return WriteData(GetIndexOfElevationData()[ElevationIndex] + GetIndexOfAzimuthData(ElevationIndex)[AzimuthIndex] + GetWoffset(ElevationIndex),WRITE_TYPE_UNKNOWN,WDataToWrite,( sizeof ( gInt16 ) * ( GetElevationHead(ElevationIndex)->DopplerGates ) ) );
}


gInt16    StandardRadarData::GetVCPDate ()  //获得体扫开始日期 add by dqc 20080422
{
	return ReadInt16(24*sizeof(gInt8));
}
gInt32    StandardRadarData::GetVCPTime ()  //获得体扫开始时间 add by dqc 20080422
{
	return ReadInt32(26*sizeof(gInt8));
}
GHRESULT  StandardRadarData::GetSiteCode(char * pSiteCode)  //获得雷达站号 add by dqc 20080422
{
	if (pSiteCode == NULL)
	{
		return GE_INVALIDARG;
	}

	gInt32 StartP =0;
	*pSiteCode++ = ( gChar )ReadInt8( StartP + 2*sizeof(gInt8)); 
	*pSiteCode++ = ( gChar )ReadInt8( StartP + 3*sizeof(gInt8)); 
	*pSiteCode++ = ( gChar )ReadInt8( StartP + 5*sizeof(gInt8)); 
	*pSiteCode++ = ( gChar )ReadInt8( StartP + 6*sizeof(gInt8)); 
	*pSiteCode++ = ( gChar )ReadInt8( StartP + 7*sizeof(gInt8)); 
	*pSiteCode++ = '\0'; 
	return GS_OK;

}
// gInt16 StandardRadarDataX::GetRadarType()         //获得雷达类型 add by dqc 20080422
// {
// 	return ReadInt16(sizeof(gInt32)+ 2*sizeof(gInt16) + 26*sizeof(gInt8) );
// }
