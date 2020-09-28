//////////////////////////////////////////////////////////////////////////
// SAProductPretreatment.cpp
// created by zy 20080401
// SA雷达产品数据格式
//////////////////////////////////////////////////////////////////////////
#include "SAProductPretreatment.h"

SAProductPretreatment::SAProductPretreatment(void)
{
	;
}

SAProductPretreatment::~SAProductPretreatment(void)
{
	;
}

TARCHIVE * SAProductPretreatment::GetTArchiveHead( gInt32 Index )
{
	if((Index * sizeof(TARCHIVE)) >=  (size_t)GetSize () )
		return  ( TARCHIVE * ) NULL;

	void * p = NULL;
	GetPointer ( sizeof(TARCHIVE) * Index, p);
	return ( TARCHIVE * ) p;
}
gUint8 * SAProductPretreatment::GetTArchiveR(gInt32 ReflectivityPointer)
{
	void * p = NULL;
	GetPointer ((gInt32)ReflectivityPointer , p);
	return (gUint8 * )p;
}
gUint8 * SAProductPretreatment::GetTArchiveV(gInt32 VelocityPointer)
{
	void * p = NULL;
	GetPointer ((gInt32)VelocityPointer , p);
	return (gUint8 * )p;
}
gUint8 * SAProductPretreatment::GetTArchiveW(gInt32 SpectrumWidthPointer)
{
	void * p = NULL;
	GetPointer ((gInt32)SpectrumWidthPointer , p);
	return (gUint8 * )p;
}




TARCHIVE * SAProductPretreatment::GetTArchiveHeadP ( gInt32 Index )
{
	return GetTArchiveHead ( Index );
}
gUint8 * SAProductPretreatment::GetTArchiveRP(gInt32 ReflectivityPointer)
{
	return GetTArchiveR(ReflectivityPointer);
}
gUint8 * SAProductPretreatment::GetTArchiveVP(gInt32 VelocityPointer)
{
	return GetTArchiveV(VelocityPointer);
}
gUint8 * SAProductPretreatment::GetTArchiveWP(gInt32 SpectrumWidthPointer)
{
	return GetTArchiveW(SpectrumWidthPointer);
}