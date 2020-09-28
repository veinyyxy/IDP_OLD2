#pragma once
#include "MyActionInterface.h"

class DisplaySettingsAction :
	public MyActionInterface
{
public:
	ADD_MYACTION_CONTSTRUCTED_DEFINITION(DisplaySettingsAction);

	virtual void MyAction();

	virtual void InitMyAction();

	virtual void SetController( Controller* pController );

};

class MapSettingsAction :
	public MyActionInterface
{
public:
	ADD_MYACTION_CONTSTRUCTED_DEFINITION(MapSettingsAction);

	virtual void MyAction();

	virtual void InitMyAction();

	virtual void SetController( Controller* pController );

};

class AddLatLonNetAction :
	public MyActionInterface
{
public:
	ADD_MYACTION_CONTSTRUCTED_DEFINITION(AddLatLonNetAction);

	virtual void MyAction();

	virtual void InitMyAction();

	virtual void SetController( Controller* pController );

};

class CenterProjectionAction : public MyActionInterface
{
public:
	ADD_MYACTION_CONTSTRUCTED_DEFINITION(CenterProjectionAction);

	virtual void MyAction();

	virtual void InitMyAction();

	virtual void SetController( Controller* pController );

};

class ProjectionSettingAction : public MyActionInterface
{
public:
	ADD_MYACTION_CONTSTRUCTED_DEFINITION(ProjectionSettingAction);

	virtual void MyAction();

	virtual void InitMyAction();

	virtual void SetController( Controller* pController );

};

class MakeProfessionalChartAction : public MyActionInterface
{
public:
	ADD_MYACTION_CONTSTRUCTED_DEFINITION(MakeProfessionalChartAction);

	virtual void MyAction();

	virtual void InitMyAction();

	virtual void SetController( Controller* pController );

};

class MakeProfessionalReportAction : public MyActionInterface
{
public:
	ADD_MYACTION_CONTSTRUCTED_DEFINITION(MakeProfessionalReportAction);

	virtual void MyAction();

	virtual void InitMyAction();

	virtual void SetController( Controller* pController );

};