#include "CCProductPretreatment.h"

CCProductPretreatment::CCProductPretreatment(void)
{
}

CCProductPretreatment::~CCProductPretreatment(void)
{
}
CC_DATAHEADER * CCProductPretreatment::GetTArchiveCCHeadP( )
{
	return GetTArchiveCCHead();
}

gInt16 * CCProductPretreatment::GetTArchiveCCRP(gInt16 LayIndex,gInt16 RadialIndex)//
{

	return GetTArchiveCCR(LayIndex,RadialIndex);
}
 DATA_CC * CCProductPretreatment::GetTArchiveCC_RVW(gInt16 LayIndex)
{
		CC_DATAHEADER * pCCheader = GetTArchiveCCHeadP();

		gUint8 TiltsNum =0;
		if( pCCheader->vsp[0].usRecordNumber  != 512 ) 
			return 0;

		//  确定平扫层数
		if ( pCCheader->ucScanMode == 10 )
			TiltsNum=1;
		else if ( pCCheader->ucScanMode > 100 )
			TiltsNum=pCCheader->ucScanMode-100;
		if( TiltsNum < 1 || TiltsNum > 20 )
			return 0;
		if( (pCCheader->usEchoType != 0x408a)||(pCCheader->usProdCode != 0x8003 ) ) 
			return 0;

		void *p = NULL;
		gInt16 offset = sizeof(CC_DATAHEADER);
		pCCheader->liDataOffset = offset;

		if( (pCCheader->liDataOffset+LayIndex*512*sizeof(DATA_CC)) > m_DataSize)
			return NULL;
		GetPointer((pCCheader->liDataOffset+LayIndex*512*sizeof(DATA_CC)),p);
		return (DATA_CC*)p;
}
DATA_CC * CCProductPretreatment::GetTArchiveCC_RVWP(gInt16 LayIndex)
{
	return GetTArchiveCC_RVW(LayIndex);
}
gInt16 * CCProductPretreatment::GetTArchiveCCVP(gInt16 LayIndex,gInt16 RadialIndex)
{

	return GetTArchiveCCV(LayIndex,RadialIndex);
}
gInt16 * CCProductPretreatment::GetTArchiveCCWP(gInt16 LayIndex,gInt16 RadialIndex)
{

	return GetTArchiveCCW(LayIndex,RadialIndex);
}

inline CC_DATAHEADER * CCProductPretreatment::GetTArchiveCCHead( )
{

	return (CC_DATAHEADER*)GetPointer(0);
}
inline gInt16 * CCProductPretreatment::GetTArchiveCCR(gInt16 LayIndex,gInt16 RadialIndex)
{
	CC_DATAHEADER * pCCheader = GetTArchiveCCHead();
	void *p = NULL;
	gInt16 offset = sizeof(CC_DATAHEADER);
	pCCheader->liDataOffset = offset;
	GetPointer(pCCheader->liDataOffset+sizeof(DATA_CC)*512*LayIndex + sizeof(gInt16)*1500*RadialIndex,p);
	return (gInt16*)p;
}
inline gInt16 * CCProductPretreatment::GetTArchiveCCV(gInt16 LayIndex,gInt16 RadialIndex)
{
	CC_DATAHEADER * pCCheader = GetTArchiveCCHead();
	void *p = NULL;
	gInt16 offset = sizeof(CC_DATAHEADER);
	pCCheader->liDataOffset = offset;

	GetPointer(pCCheader->liDataOffset+sizeof(DATA_CC)*512*LayIndex + sizeof(gInt16)*1500*RadialIndex+sizeof(gInt16)*500,p);
	return (gInt16*)p;
}
inline gInt16 * CCProductPretreatment::GetTArchiveCCW(gInt16 LayIndex,gInt16 RadialIndex)
{
	CC_DATAHEADER * pCCheader = GetTArchiveCCHead();
	void *p = NULL;
	gInt16 offset = sizeof(CC_DATAHEADER);
	pCCheader->liDataOffset = offset;
	GetPointer(pCCheader->liDataOffset+sizeof(DATA_CC)*512*LayIndex + sizeof(gInt16)*1500*RadialIndex+sizeof(gInt16)*1000,p);
	return (gInt16*)p;
}
gInt16 CCProductPretreatment::GetLayerNubP()
{
	return GetLayerNub();
}
gInt16 CCProductPretreatment::GetLayerNub()
{
	CC_DATAHEADER * pCCheader = GetTArchiveCCHeadP();

	gUint8 TiltsNum =0;
	if( pCCheader->vsp[0].usRecordNumber  != 512 ) 
		return 0;

	//  确定平扫层数
	if ( pCCheader->ucScanMode == 10 )
		TiltsNum=1;
	else if ( pCCheader->ucScanMode > 100 )
		TiltsNum=pCCheader->ucScanMode-100;
	if( TiltsNum < 1 || TiltsNum > 20 )
		return 0;

	return TiltsNum;
}