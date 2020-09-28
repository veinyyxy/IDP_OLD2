#pragma once
#include "MyActionInterface.h"
class QMdiArea;

class FloatingWindowsAction :
	public MyActionInterface
{
public:
	ADD_MYACTION_CONTSTRUCTED_DEFINITION(FloatingWindowsAction);
	~FloatingWindowsAction(void);

	void MyAction();
	void InitMyAction();
	QMdiArea* m_pMdiArea;
};

class DockingWindowsAction :
	public MyActionInterface
{
public:
	ADD_MYACTION_CONTSTRUCTED_DEFINITION(DockingWindowsAction);
	~DockingWindowsAction(void);

	void MyAction();
	void InitMyAction();
	QMdiArea* m_pMdiArea;
};