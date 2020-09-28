//////////////////////////////////////////////////////////////////////////
// GDataStream.inl
// created by zhangluyuan 20080815
// for GDataStream
//////////////////////////////////////////////////////////////////////////
#include <fstream>
inline GenerationDataStream::GenerationDataStream()
{
	;
}

inline GenerationDataStream::~GenerationDataStream()
{
	;
}

inline bool GenerationDataStream::CreateFromList(const std::list<GenerationData*> & GDataList)
{
	ofstream logfile;
	logfile.open("List_log.txt",ios::app);

	gInt16 listnum = (gInt16)GDataList.size();
	if ( SetDataSize(sizeof(gInt16)) != GS_OK )
	{
		logfile<<"SetDataSize false. "<<listnum<<endl;
		return false;
	}
	if ( WriteData(0, WRITE_TYPE_UNKNOWN, (void*)&listnum, sizeof(gInt16)) != GS_OK )
	{
		logfile<<"WriteData1 false. "<<listnum<<endl;
		return false;
	}
	gInt32 AddSize = 0;
	std::list<GenerationData*>::const_iterator GDataIter = GDataList.begin();
	for (gInt16 i=0; i<listnum; i++)
	{
		gInt32 GDataSize = (*GDataIter)->GetDataSize();
		AddSize+=GDataSize;
		GDataIter++;
	}
	if ( DataSizeAdd(sizeof(gInt32)*listnum+AddSize) != GS_OK )
	{
		logfile<<"DataSizeAdd false. "<<listnum<<"  ADDSIZE IS "<<AddSize<<endl;
		return false;
	}
	gInt32 StartP = (gInt32)sizeof(gInt16);
	GDataIter = GDataList.begin();
	for (gInt16 i=0; i<listnum; i++)
	{
		gInt32 GDataSize = (*GDataIter)->GetDataSize();
		if ( WriteData(StartP, WRITE_TYPE_UNKNOWN, (void*)&GDataSize, sizeof(gInt32)) != GS_OK )
		{
			logfile<<"WriteData2 false. "<<listnum<<"i is "<<i<<"  GDataSize IS "<<GDataSize<<endl;
			return false;
		}
		StartP += (gInt32)sizeof(gInt32);
		if ( WriteData(StartP, WRITE_TYPE_UNKNOWN, (*GDataIter)->GetPointer(0), GDataSize) != GS_OK )
		{
			logfile<<"WriteData3 false. "<<listnum<<"i is "<<i<<"  GDataSize IS "<<GDataSize<<endl;
			return false;
		}
		StartP += GDataSize;
		GDataIter++;
	}
	return true;
}

inline GHRESULT GenerationDataStream::RevertToList(std::list<GenerationData*> & GDataList) const
{
	gInt16 listnum = 0;
	void * pToGet = NULL;
	pToGet = GetPointer(0);
	if (pToGet == NULL)
	{
		return GS_FALSE;
	}
	listnum = *(gInt16*)pToGet;
	gInt32 GDataSize = 0;
	gInt32 StartP = (gInt32)sizeof(gInt16);
	for (gInt16 i=0; i<listnum; i++)
	{
		pToGet = GetPointer(StartP);
		if (pToGet == 0)
		{
			return false;
		}
		GDataSize = *(gInt32*)pToGet;
		StartP += (gInt32)sizeof(gInt32);
		pToGet = GetPointer(StartP);
		if (pToGet == 0)
		{
			return GS_FALSE;
		}
		GenerationData* tempGData = new(::std::nothrow) GenerationData();
		if ( tempGData == NULL )
		{
			return GE_NOENOUGHMEMORY;
		}
		if ( tempGData->Realloc(GDataSize) != GS_OK )
		{
			if (tempGData)
			{
				delete tempGData;
				tempGData = NULL;
			}
			return GE_NOENOUGHMEMORY;
		}

		GHRESULT tmpRe;
		if ( (tmpRe=tempGData->WriteData(pToGet,GDataSize)) != GS_OK )
		{
			if (tempGData)
			{
				delete tempGData;
				tempGData = NULL;
			}
			return tmpRe;
		}
		GDataList.push_back(tempGData);
		StartP += GDataSize;
	}
	return GS_OK;
}

inline void GenerationDataStream::ReleaseGDataList(std::list<GenerationData*> & GDataList)
{
	int listnum = (int)GDataList.size();
	for (int i=0;i<listnum; i++)
	{
		GenerationData* tempGData = *GDataList.begin();
		delete tempGData;
		tempGData = NULL;
		GDataList.erase(GDataList.begin());
	}
}

inline bool GenerationDataStream::AddGData(const GenerationData* pGData)
{
	void * pToGet = NULL;
	pToGet = GetPointer(0);
	if (pToGet == NULL)
	{
		return false;
	}
	gInt16 listnum = *(gInt16*)pToGet;
	listnum++;
	if ( WriteData(0, WRITE_TYPE_UNKNOWN, (void*)&listnum, sizeof(gInt16)) != GS_OK )
	{
		return false;
	}
	gInt32 StartP = GetDataSize();
	gInt32 AddSize = pGData->GetDataSize();
	if ( DataSizeAdd(AddSize+sizeof(gInt32)) != GS_OK)
	{
		return false;
	}
	if ( WriteData(StartP, WRITE_TYPE_UNKNOWN, (void*)&AddSize, sizeof(gInt32)) != GS_OK )
	{
		return false;
	}
	StartP += sizeof(gInt32);
	if ( WriteData(StartP, WRITE_TYPE_UNKNOWN, pGData->GetPointer(0), AddSize) != GS_OK )
	{
		return false;
	}
	return true;
}

inline bool GenerationDataStream::AddList(const std::list<GenerationData*> & GDataList)
{
	int listnum = (int)GDataList.size();
	std::list<GenerationData*>::const_iterator Iter = GDataList.begin();
	for (int i=0; i<listnum; i++)
	{
		if ( !AddGData(*Iter))
		{
			return false;
		}
		Iter++;
	}
	return true;
}
