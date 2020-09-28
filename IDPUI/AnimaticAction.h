#pragma once
/** @file 
* @brief    动画播放类头文件
* @author    杨晓宇
* @date      2013年3月
* @version   version
  @note      2013年 6月
*/
#include "myactioninterface.h"
//------------------------------------------

class RadarAnimatic;


class ControlAnimatic
{
public:
	ControlAnimatic();
	bool GetOptionModel();
	static RadarAnimatic* GetControlAnimaticor();
protected:
	//static RadarAnimatic s_RadarAnimatic;
	//int m_FrameNumber;
};
//------------------------------------------

/** @class   PlayAction
    @brief   动画播放类头文件
    @author  杨晓宇
    @note    动画播放类的对外接口
*/ 
class PlayAction :
	public MyActionInterface/*, ControlAnimatic*/
{
public:
	ADD_MYACTION_CONTSTRUCTED_DEFINITION(PlayAction);
	~PlayAction(void);

	void MyAction();
	void InitMyAction();

	inline void SetController( Controller* pController ){;}

	AnimaticControler* m_pAnimaticControler;
};
//------------------------------------------
class PauseAction :
	public MyActionInterface/*, ControlAnimatic*/
{
public:
	ADD_MYACTION_CONTSTRUCTED_DEFINITION(PauseAction);
	~PauseAction(void);

	void MyAction();
	void InitMyAction();
	inline void SetController( Controller* pController ){;}

	AnimaticControler* m_pAnimaticControler;
};
//------------------------------------------
class StopAction :
	public MyActionInterface/*, ControlAnimatic*/
{
public:
	ADD_MYACTION_CONTSTRUCTED_DEFINITION(StopAction);
	~StopAction(void);

	void MyAction();
	void InitMyAction();
	inline void SetController( Controller* pController ){;}

	AnimaticControler* m_pAnimaticControler;
};
//------------------------------------------
class speeddownAction :
	public MyActionInterface/*, ControlAnimatic*/
{
public:
	ADD_MYACTION_CONTSTRUCTED_DEFINITION(speeddownAction);
	~speeddownAction(void);

	void MyAction();
	void InitMyAction();
	inline void SetController( Controller* pController ){;}

	AnimaticControler* m_pAnimaticControler;
};
//------------------------------------------
class speedupAction :
	public MyActionInterface/*, ControlAnimatic*/
{
public:
	ADD_MYACTION_CONTSTRUCTED_DEFINITION(speedupAction);
	~speedupAction(void);

	void MyAction();
	void InitMyAction();
	inline void SetController( Controller* pController ){;}

	AnimaticControler* m_pAnimaticControler;
};
//------------------------------------------
class PreAction :
	public MyActionInterface/*, ControlAnimatic*/
{
public:
	ADD_MYACTION_CONTSTRUCTED_DEFINITION(PreAction);
	~PreAction(void);

	void MyAction();
	void InitMyAction();
	inline void SetController( Controller* pController ){;}

	AnimaticControler* m_pAnimaticControler;
};
//------------------------------------------
class NextAction :
	public MyActionInterface/*, ControlAnimatic*/
{
public:
	ADD_MYACTION_CONTSTRUCTED_DEFINITION(NextAction);
	~NextAction(void);

	void MyAction();
	void InitMyAction();
	inline void SetController( Controller* pController ){;}

	AnimaticControler* m_pAnimaticControler;
};
//------------------------------------------
class LoopAction :
	public MyActionInterface/*, ControlAnimatic*/
{
public:
	ADD_MYACTION_CONTSTRUCTED_DEFINITION(LoopAction);
	~LoopAction(void);

	void MyAction();
	void InitMyAction();
	inline void SetController( Controller* pController ){;}

	AnimaticControler* m_pAnimaticControler;
};
//------------------------------------------