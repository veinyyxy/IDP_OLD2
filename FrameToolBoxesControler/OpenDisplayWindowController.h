#pragma once
#include "Controller.h"
class OpenDisplayWindowController :
	public Controller
{
public:
	OpenDisplayWindowController(void);
	~OpenDisplayWindowController(void);

	virtual int Excute();

	virtual void GetName(std::string& strID );

	virtual void Input( const  FrameModel* pFM );

	virtual FrameModel* Output();

	virtual int PreExcute();

	virtual int PostExcute();

private:
	std::string m_MyName;
	//std::map<std::string, boost::any> m_OutParam;
	FrameModel* m_pFrameModel;
	void* m_pMap;
};

