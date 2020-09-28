//////////////////////////////////////////////////////////////////////////
// RequestMessage.inl
// created by ply 20080729
// for 	Request Message
//////////////////////////////////////////////////////////////////////////

inline void RequestMessageHeader::InitializeNoParam()
{
	requestType = Product_Request_None;
	length = -1;
	for (int i=0;i<8;i++)
	{
		to[i]='\0';
		from[i]='\0';
	}
}
inline void RoutineRequestMessagePublic::InitializeNoParam()
{
	for (int i=0;i<8;i++)
	{
		RadarSite[i]='\0';
	}
}
inline void RoutineRequestMessage::InitializeNoParam()
{
	ProductCode = -1;
	Interval = -1;
}
inline void AlarmRequestMessagePublic::InitializeNoParam()
{
	for (int i=0;i<8;i++)
	{
		RadarSite[i]='\0';
	}
	AlarmEndDate = -1;
	AlarmEndTime = -1;
	AreaType = RectangleArea;
}
inline void OnceRequestMessagePublic::Initialize()
{
	for (int i=0;i<8;i++)
	{
		RadarSite[i]='\0';
	}
	NumberOfParameter = 0;
	HistoryOrCurrent = 0;
	ScanDateOrRepeatTime = -1;
	ScanTimeOrInterval = -1;
	IsQualityControl = 0;
}
inline void OnceRequestMessage::Initialize()
{
	ProductCode = 0;
	NumberOfParameter = 0;
}
inline void AlarmRequestMessage::Initialize()
{
	ProductCode = 0;
	NumberOfParameter = 0;
}
inline void AlarmACA::Initialize()
{
	NumberOfParameter = 0;
}
inline void IpAddr::Initialize()
{
	for (int i=0;i<16;i++)
	{
		IpAddr[i]='\0';
	}
	
}
inline void SingleRequestMessage::Initialize()
{
	ProductCode=0;
	for (int i=0;i<8;i++)
	{
		SiteCode[i]='\0';
	}
	VCPDate=0;
	VCPTime=0;

}
//////////////////////////////////////////////////////////////////////////

template<typename T>
inline void RequestMessageData<T>::Init()
{
	Resize(sizeof(T));
	T Message;
	Message.Initialize();
	WriteData(&Message, sizeof(T));	//ssy：WriteData中会增加m_DataSize
}

template<typename T>
inline RequestMessageData<T>::RequestMessageData()
{
	Init();
}

template<typename T>
inline RequestMessageData<T>::~RequestMessageData()
{
	;
}

template<typename T>
inline T * RequestMessageData<T>::GetRequestMessage()
{
	return (T *)GetPointer(0);
}

template<typename T>
inline RequestParametersMap RequestMessageData<T>::GetParametersMap()
{
	RequestParametersMap TemRetRPM;
	RequestMessageData<T> * TmpThis=this;
	TemRetRPM.LoadParameters(TmpThis);
	return TemRetRPM;
}

template<typename T>
inline GHRESULT RequestMessageData<T>::ReplaceParametersFromMap(const RequestParametersMap & ParamMap)
{
	RequestMessageData<T> * TmpThis=this;
	return ParamMap.SaveParameters(TmpThis);
}

template<typename T>
inline void * RequestMessageData<T>::GetParametersPointer(gInt16 Idx) 
{
	return Idx<GetRequestMessage()->NumberOfParameter ? GetPointer(ReadInt32(sizeof(T)+sizeof(gInt32)*Idx)) : NULL;
}

template<typename T>
inline gInt16 * RequestMessageData<T>::GetParametersNumber()
{
	T * pT = GetRequestMessage();
	return pT==NULL ? NULL : pT->NumberOfParameter;
}

template<typename T>
inline void RequestMessageData<T>::ClearParam()
{
	if (GetDataSize()<sizeof(T))
	{
		Init();
		return ;
	}
	else
	{
		//Resize(sizeof(T)); // ssy
		Realloc(sizeof(T));
		T * pMessage=GetRequestMessage();
		pMessage->NumberOfParameter = 0;
		return ;
	}
}


//////////////////////////////////////////////////////////////////////////

template<typename T>
inline GHRESULT RequestParametersMap::LoadParameters(RequestMessageData<T> * pRequestMessageData)
//load parameters from pRequestMessageData
{
	if (pRequestMessageData==NULL)
	{
		return GE_INVALIDARG;
	}
	T * pRMH = NULL;
	if ((pRMH = pRequestMessageData->GetRequestMessage())==NULL)
	{
		return GE_INVALIDARG;
	}
	clear();
	gInt16 ParamCount=pRMH->NumberOfParameter;

	for (gInt16 i=0;i<ParamCount;i++)
	{
		gByte * p;
		RequestParameterHead * pParamH ;
		char * pName;
		std::vector<char> TmpVct;

		pParamH =(RequestParameterHead *) (p =(gByte *) pRequestMessageData->GetParametersPointer(i));
		pName=(char *)(p+sizeof(RequestParameterHead));
		size_t VctSize=pParamH->ValueSize;
		TmpVct.resize(VctSize);
		memcpy(&(TmpVct[0]),p+sizeof(RequestParameterHead)+pParamH->NameSize,VctSize);
		insert(value_type(pName,std::make_pair(pParamH->ValueType,TmpVct)));

	}

	return GS_OK;
}

template<typename T>
inline GHRESULT RequestParametersMap::SaveParameters(RequestMessageData<T> * pRequestMessageData)const
//save parameters to pRequestMessageData
{
	if (pRequestMessageData==NULL)
	{
		return GE_INVALIDARG;
	}
	T * pRMH = NULL;
	if ((pRMH = pRequestMessageData->GetRequestMessage())==NULL)
	{
		return GE_INVALIDARG;
	}

	int ParamCount=GetParamCount();
	int CurrentOffset;
	int CurrentParamSize;
	int CurrentNameSize;
	RequestParameterType CurrentParamType;
	int CurrentParamDataSize;
	void * pCurrentParamData;
	RequestParameterHead * pCurrentParamHead;
	gInt32 * pParamOffsets;

	int ParamHeadSize=sizeof(RequestParameterHead);

	size_t sizeWill = sizeof(T) + GetParameterDataSize() + ParamCount*sizeof(gInt32);
	pRequestMessageData->ClearParam();
	pRequestMessageData->SetDataSize(sizeWill);
	pRMH = pRequestMessageData->GetRequestMessage();	// ssy 0827 reget the pointer

	CurrentOffset=sizeof(T)+ParamCount*sizeof(gInt32);
	pParamOffsets=(gInt32 *)(pRequestMessageData->GetPointer(sizeof(T)));
	//pParamOffsets = pRequestMessageData->GetParametersOffset();	// ssy

	for (const_iterator Iter=begin();Iter!=end();Iter++)
	{
		pRMH->NumberOfParameter++;
		CurrentNameSize=Iter->first.length()+1;
		CurrentParamDataSize=Iter->second.second.size();
		CurrentParamType=(RequestParameterType)(Iter->second.first);
		CurrentParamSize=ParamHeadSize+CurrentNameSize+CurrentParamDataSize; // with param head	///.

		*(pParamOffsets++) =CurrentOffset;
		pCurrentParamHead=(RequestParameterHead *)pRequestMessageData->GetPointer(CurrentOffset);
		pCurrentParamHead->NameSize=CurrentNameSize;
		pCurrentParamHead->TotalSize=CurrentParamSize;
		pCurrentParamHead->ValueSize=CurrentParamDataSize;
		pCurrentParamHead->ValueType=CurrentParamType;
		pCurrentParamData=pRequestMessageData->GetPointer(CurrentOffset+ParamHeadSize);
		memcpy(pCurrentParamData, Iter->first.c_str(), CurrentNameSize);	///.
		memcpy((void*)((char*)pCurrentParamData+CurrentNameSize), &(Iter->second.second[0]), CurrentParamDataSize);

		CurrentOffset+=CurrentParamSize;
	}
	return GS_OK;
}

inline gInt32 RequestParametersMap::GetParameterDataSize()const
{
	int ParamDataSize=0;
	int ParamHeadSize=sizeof(RequestParameterHead);

	for (const_iterator Iter=begin();Iter!=end();Iter++)
	{
		ParamDataSize+=ParamHeadSize;
		ParamDataSize+=Iter->first.length()+1;
		ParamDataSize+=Iter->second.second.size();
	}
	return ParamDataSize;
}

inline void * RequestParametersMap::GetParamPointer(const char * ParamName)const
{
	const_iterator c_Iter=find(ParamName);
	return c_Iter==end() ? NULL : (void *)&(c_Iter->second.second[0]);
}

inline RequestParameterType RequestParametersMap::GetParamType(const char * ParamName)const
{
	const_iterator c_Iter=find(ParamName);
	return c_Iter==end() ? RPT_TYPE_ERROR : (RequestParameterType)(c_Iter->second.first);
}

inline gInt32 RequestParametersMap::GetParamSize(const char * ParamName)const
{
	const_iterator c_Iter=find(ParamName);
	return (c_Iter->second.second.size());
}

inline gInt32 RequestParametersMap::GetParamArrayLength(const char * ParamName)const
{
	const_iterator c_Iter=find(ParamName);
	return c_Iter==end() ? -1 : (c_Iter->second.second.size())/GetParamTypeSize((RequestParameterType)(c_Iter->second.first));
	//return GetParamSize(ParamName)/GetParamTypeSize(GetParamType(ParamName));
}

inline gInt32 RequestParametersMap::GetParamTypeSize(const RequestParameterType & ParamType)const
{
	switch(ParamType)
	{
	case RPT_TYPE_ERROR:return 0;
		break;
	case RPT_gChar:return 1;
		break;
	case RPT_gInt16:return 2;	// ssy 0826
		break;
	case RPT_gInt32:return 4;
		break;
	case RPT_gFloat:return 4;	// ssy 0826
		break;
	case RPT_gDouble:return 8;
		break;
	case RPT_gChar8:return 8;
		break;
	default:return 0;
	    break;
	}
	//return 0;
}

inline gInt32 RequestParametersMap::GetParamCount()const
{
	return size();
}

inline GHRESULT RequestParametersMap::AddParam(char * ParamName,
												const RequestParameterType & ParamType,
												gInt32 ArrayLength,
												void * pDataArray/*=NULL*/)
{
	if (find(ParamName)!=end())
	{
		//already exist
		return GE_OBJECTEXIST;
	}

	std::vector<char> TmpVct;
	int VctSize =ArrayLength*GetParamTypeSize(ParamType);
	TmpVct.resize(VctSize);

	if (pDataArray!=NULL)
	{
		memcpy(&(TmpVct[0]),pDataArray,VctSize);
	}
	insert(value_type(ParamName,std::make_pair(ParamType,TmpVct)));

	return GS_OK;
}

inline GHRESULT RequestParametersMap::AddParamOne(char * ParamName,
											   const RequestParameterType & ParamType,
											   void * pDataArray/*=NULL*/)
{
	int VctSize = GetParamTypeSize(ParamType);
	iterator mitr = find(ParamName);
	if (mitr == end())
	{
		//not exist
		std::vector<char> TmpVct;
		TmpVct.resize(VctSize);
		if (pDataArray != NULL)
		{
			memcpy(&(TmpVct[0]),pDataArray,VctSize);
		}
		insert(value_type(ParamName,std::make_pair(ParamType,TmpVct)));
	}
	else
	{
		//already get mitr
		int sizenow = mitr->second.second.size();
		mitr->second.second.resize(sizenow + VctSize);
		if (pDataArray != NULL)
		{
			memcpy(&(mitr->second.second[sizenow]), pDataArray, VctSize);
		}
	}

	return GS_OK;
}

inline GHRESULT RequestParametersMap::RemoveParam(const char * ParamName)
{
	if (erase(ParamName))
	{
		return GS_OK;
	}
	else
	{
		return GE_INVALIDARG;
	}
}


//////////////////////////////////////////////////////////////////////////

template<typename T>
inline void RequestMessageDataNoParam<T>::Init()
{
	Resize(sizeof(T));
	T Message;
	Message.InitializeNoParam();
	WriteData(&Message, sizeof(T));
}

template<typename T>
inline RequestMessageDataNoParam<T>::RequestMessageDataNoParam()
{
	Init();
}

template<typename T>
inline RequestMessageDataNoParam<T>::~RequestMessageDataNoParam()
{
	;
}

template<typename T>
inline T * RequestMessageDataNoParam<T>::GetRequestMessage()
{
	return (T *)GetPointer(0);
}

