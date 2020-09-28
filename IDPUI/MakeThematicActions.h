#pragma once
#include "MyActionInterface.h"

class QMdiArea;

class ThematicProductAction :
	public MyActionInterface
{
public:
	ADD_MYACTION_CONTSTRUCTED_DEFINITION(ThematicProductAction);
	~ThematicProductAction(void);

	void MyAction();
	void InitMyAction();
	QMdiArea* m_pMdiArea;
};