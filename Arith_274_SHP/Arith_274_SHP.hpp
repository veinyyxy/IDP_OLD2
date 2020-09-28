#ifndef _ARITH_274_SHP_HPP_
#define _ARITH_274_SHP_HPP_

#include "IArith.h"
#include "GXList.h"
#include "GData.h"

class Arith_274_SHP : public IArith
{
public:
	Arith_274_SHP();
	~Arith_274_SHP();

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