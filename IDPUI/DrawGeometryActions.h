#pragma once

class DrawLine;
class DrawRect;
class DrawCircle;
class DrawPolygonalLine;
class DrawSymbol;
class DrawWindSymbol;

#include "ColdFront.h"
#include "WarmFront.h"
#include "OcclusionFront.h"

#include "MyActionInterface.h"

class HandAction : public MyActionInterface
{
public:
	ADD_MYACTION_CONTSTRUCTED_DEFINITION(HandAction);
	void MyAction();
	void InitMyAction();
	inline void SetController( Controller* pController ){;}
};

class DrawLineAction : public MyActionInterface
{
public:
	ADD_MYACTION_CONTSTRUCTED_DEFINITION(DrawLineAction);
	void MyAction();
	void InitMyAction();
	inline void SetController( Controller* pController ){;}
private:
	DrawLine* m_DrawLine;
};

class DrawRectAction : public MyActionInterface
{
public:
	ADD_MYACTION_CONTSTRUCTED_DEFINITION(DrawRectAction);
	void MyAction();
	void InitMyAction();
	inline DrawRect* GetDrawRect(){return m_DrawRect;}
	inline void SetController( Controller* pController ){;}
private:
	DrawRect* m_DrawRect;
};

class DrawCircleAction : public MyActionInterface
{
public:
	ADD_MYACTION_CONTSTRUCTED_DEFINITION(DrawCircleAction);
	void MyAction();
	void InitMyAction();
	inline void SetController( Controller* pController ){;}
private:
	DrawCircle* m_DrawCircle;
};

class DrawPolygonalLineAction : public MyActionInterface
{
public:
	ADD_MYACTION_CONTSTRUCTED_DEFINITION(DrawPolygonalLineAction);
	void MyAction();
	void InitMyAction();
	inline void SetController( Controller* pController ){;}
private:
	DrawPolygonalLine* m_DrawPolyLine;
};

class DrawSymbol1Action : public MyActionInterface
{
public:
	ADD_MYACTION_CONTSTRUCTED_DEFINITION(DrawSymbol1Action);
	void MyAction();
	void InitMyAction();
	inline void SetController( Controller* pController ){;}
private:
	DrawSymbol* m_DrawSymbol;
};

class DrawColdFrontAction : public MyActionInterface
{
public:
	ADD_MYACTION_CONTSTRUCTED_DEFINITION(DrawColdFrontAction);
	void MyAction();
	void InitMyAction();
	inline void SetController( Controller* pController ){;}
private:
	DrawPolygonalLine* m_DrawPolyLine;
	CColdFront m_ColdFront;
};

class DrawWarmFrontAction : public MyActionInterface
{
public:
	ADD_MYACTION_CONTSTRUCTED_DEFINITION(DrawWarmFrontAction);
	void MyAction();
	void InitMyAction();
	inline void SetController( Controller* pController ){;}
private:
	DrawPolygonalLine* m_DrawPolyLine;
	CWarmFront m_WarmFront;
};

class DrawOcclusionFrontAction : public MyActionInterface
{
public:
	ADD_MYACTION_CONTSTRUCTED_DEFINITION(DrawOcclusionFrontAction);
	void MyAction();
	void InitMyAction();
	inline void SetController( Controller* pController ){;}
private:
	DrawPolygonalLine* m_DrawPolyLine;
	COcclusionFront m_OcclusionFront;
};

class DrawWindAction : public MyActionInterface
{
public:
	ADD_MYACTION_CONTSTRUCTED_DEFINITION(DrawWindAction);
	void MyAction();
	void InitMyAction();
	inline void SetWindSpeed(int iWS){m_iWindSpeed = iWS;}
	inline void SetController( Controller* pController ){;}
private:
	osg::ref_ptr<DrawWindSymbol> m_DrawWind;
	int m_iWindSpeed;
};

class DeleteSymbolAction : public MyActionInterface
{
public:
	ADD_MYACTION_CONTSTRUCTED_DEFINITION(DeleteSymbolAction);
	void MyAction();
	void InitMyAction();
	inline void SetController( Controller* pController ){;}
};

class MoveSymbolAction : public MyActionInterface
{
public:
	ADD_MYACTION_CONTSTRUCTED_DEFINITION(MoveSymbolAction);
	void MyAction();
	void InitMyAction();
	inline void SetController( Controller* pController ){;}
};