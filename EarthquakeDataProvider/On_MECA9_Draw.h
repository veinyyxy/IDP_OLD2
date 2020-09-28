#pragma once
#include "earthquakeproductiondraw.h"
class On_MECA9_Draw :
	public EQProductionDraw
{
public:
	On_MECA9_Draw(void);
	virtual ~On_MECA9_Draw(void);

	virtual EARTHQUAKE_DATA_TYPE GetTypeOfProduct();

	virtual void LoadFile( const QString& filename );

	virtual bool DumpAll( Layer* layer,P_E_COLORTABLE_VECTOR pColorTable = 0 );

};
