#ifndef _ARITH_273_THP_HPP_
#define _ARITH_273_THP_HPP_

#include "IArith.h"
#include "GXList.h"
#include "GData.h"

class Arith_273_THP : public IArith
{
public:
	Arith_273_THP();
	~Arith_273_THP();

	SYSMETHOD(Initialize)();
	SYSMETHOD(LoadData)(void* pValue, ReadParam * ReadParameter);
	SYSMETHOD(OutPutData)(void* pValue);
	SYSMETHOD(Execute());
	SYSMETHOD(UnInitialize());

private:
	GXList<GenerationData *>* m_pInputList;
	GXList<GenerationData>* m_pOutputList;
	ReadParam *       m_fnParam;
};

#endif