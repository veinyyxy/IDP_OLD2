#pragma once
#include "MyActionInterface.h"

class To2DMapAction :
	public MyActionInterface
{
public:
	ADD_MYACTION_CONTSTRUCTED_DEFINITION(To2DMapAction);
	~To2DMapAction(void);

	void MyAction();
	void InitMyAction();
	
};

class To3DMapAction :
	public MyActionInterface
{
public:
	ADD_MYACTION_CONTSTRUCTED_DEFINITION(To3DMapAction);
	~To3DMapAction(void);

	void MyAction();
	void InitMyAction();

};