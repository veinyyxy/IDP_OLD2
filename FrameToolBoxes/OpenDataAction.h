#pragma once
#include "MyActionInterface.h"
#include "Controller.h"

class OpenDataAction :
	public MyActionInterface
{
public:
	ADD_MYACTION_CONTSTRUCTED_DEFINITION(OpenDataAction);
	virtual void MyAction();

	virtual void InitMyAction();

	virtual void SetController( Controller* pController );
private:
	Controller* m_pController;
};

class CloseSystemAction :
	public MyActionInterface
{
public:
	ADD_MYACTION_CONTSTRUCTED_DEFINITION(CloseSystemAction);

	virtual void MyAction();

	virtual void InitMyAction();

	virtual void SetController( Controller* pController );

};