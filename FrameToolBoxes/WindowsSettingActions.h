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

	virtual void SetController( Controller* pController );

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

	virtual void SetController( Controller* pController );

	QMdiArea* m_pMdiArea;
};

class OverlapAction :
	public MyActionInterface
{
public:
	ADD_MYACTION_CONTSTRUCTED_DEFINITION(OverlapAction);

	void MyAction();
	void InitMyAction();

	virtual void SetController( Controller* pController );

	QMdiArea* m_pMdiArea;
};

class TileAction :
	public MyActionInterface
{
public:
	ADD_MYACTION_CONTSTRUCTED_DEFINITION(TileAction);

	void MyAction();
	void InitMyAction();

	virtual void SetController( Controller* pController );

	QMdiArea* m_pMdiArea;
};

class CloseAllAction :
	public MyActionInterface
{
public:
	ADD_MYACTION_CONTSTRUCTED_DEFINITION(CloseAllAction);

	void MyAction();
	void InitMyAction();

	virtual void SetController( Controller* pController );

	QMdiArea* m_pMdiArea;
};

class SystemSettingAction :
	public MyActionInterface
{
public:
	ADD_MYACTION_CONTSTRUCTED_DEFINITION(SystemSettingAction);

	void MyAction();
	void InitMyAction();

	virtual void SetController( Controller* pController );

	QMdiArea* m_pMdiArea;
};

class FullScreenWindowAction :
	public MyActionInterface
{
public:
	ADD_MYACTION_CONTSTRUCTED_DEFINITION(FullScreenWindowAction);

	void MyAction();
	void InitMyAction();

	virtual void SetController( Controller* pController );

	QMdiArea* m_pMdiArea;
};