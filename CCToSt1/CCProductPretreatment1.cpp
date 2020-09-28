//////////////////////////////////////////////////////////////////////////
// SCProductPretreatment.cpp
// created by zy 20080401
// SC雷达产品数据格式
//////////////////////////////////////////////////////////////////////////
#include "CCProductPretreatment1.h"

CCProductPretreatment1::CCProductPretreatment1(void)
{
	;
}

CCProductPretreatment1::~CCProductPretreatment1(void)
{
	;
}

TARCHIVE * CCProductPretreatment1::GetTArchiveHead( gInt32 Index )
{
	if((Index * sizeof(TARCHIVE)) >=  (size_t)GetSize () )
		return  ( TARCHIVE * ) NULL;

	void * p = NULL;
	GetPointer ( sizeof(TARCHIVE) * Index, p);
	return ( TARCHIVE * ) p;
}
gUint8 * CCProductPretreatment1::GetTArchiveR(gInt32 ReflectivityPointer)
{
	void * p = NULL;
	GetPointer ((gInt32)ReflectivityPointer , p);
	return (gUint8 * )p;
}
gUint8 * CCProductPretreatment1::GetTArchiveV(gInt32 VelocityPointer)
{
	void * p = NULL;
	GetPointer ((gInt32)VelocityPointer , p);
	return (gUint8 * )p;
}
gUint8 * CCProductPretreatment1::GetTArchiveW(gInt32 SpectrumWidthPointer)
{
	void * p = NULL;
	GetPointer ((gInt32)SpectrumWidthPointer , p);
	return (gUint8 * )p;
}




TARCHIVE * CCProductPretreatment1::GetTArchiveHeadP ( gInt32 Index )
{
	return GetTArchiveHead ( Index );
}
gUint8 * CCProductPretreatment1::GetTArchiveRP(gInt32 ReflectivityPointer)
{
	return GetTArchiveR(ReflectivityPointer);
}
gUint8 * CCProductPretreatment1::GetTArchiveVP(gInt32 VelocityPointer)
{
	return GetTArchiveV(VelocityPointer);
}
gUint8 * CCProductPretreatment1::GetTArchiveWP(gInt32 SpectrumWidthPointer)
{
	return GetTArchiveW(SpectrumWidthPointer);
}