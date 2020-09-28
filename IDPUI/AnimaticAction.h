#pragma once
/** @file 
* @brief    ����������ͷ�ļ�
* @author    ������
* @date      2013��3��
* @version   version
  @note      2013�� 6��
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
    @brief   ����������ͷ�ļ�
    @author  ������
    @note    ����������Ķ���ӿ�
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