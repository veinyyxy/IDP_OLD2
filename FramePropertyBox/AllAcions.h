#pragma once
#include "MyActionInterface.h"

class MoveUpAction :
	public MyActionInterface
{
public:
	ADD_MYACTION_CONTSTRUCTED_DEFINITION(MoveUpAction);

	virtual void MyAction();

	virtual void InitMyAction();

	virtual void SetController( Controller* pController );

};

class MoveDownAction :
	public MyActionInterface
{
public:
	ADD_MYACTION_CONTSTRUCTED_DEFINITION(MoveDownAction);

	virtual void MyAction();

	virtual void InitMyAction();

	virtual void SetController( Controller* pController );

};

class AddBasicAction :
	public MyActionInterface
{
public:
	ADD_MYACTION_CONTSTRUCTED_DEFINITION(AddBasicAction);

	virtual void MyAction();

	virtual void InitMyAction();

	virtual void SetController( Controller* pController );

};

class ShowLayerPropertyAction :
	public MyActionInterface
{
public:
	ADD_MYACTION_CONTSTRUCTED_DEFINITION(ShowLayerPropertyAction);

	virtual void MyAction();

	virtual void InitMyAction();

	virtual void SetController( Controller* pController );

};

class DeleteLayerAction :
	public MyActionInterface
{
public:
	ADD_MYACTION_CONTSTRUCTED_DEFINITION(DeleteLayerAction);

	virtual void MyAction();

	virtual void InitMyAction();

	virtual void SetController( Controller* pController );

};

class ShowLayerAction :
	public MyActionInterface
{
public:
	ADD_MYACTION_CONTSTRUCTED_DEFINITION(ShowLayerAction);

	virtual void MyAction();

	virtual void InitMyAction();

	virtual void SetController( Controller* pController );

};

class HideLayerAction :
	public MyActionInterface
{
public:
	ADD_MYACTION_CONTSTRUCTED_DEFINITION(HideLayerAction);

	virtual void MyAction();

	virtual void InitMyAction();

	virtual void SetController( Controller* pController );

};