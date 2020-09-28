#pragma once
#include "FrameModel.h"

class ParamSwapModel :
	public FrameModel
{
public:
	typedef std::map<std::string, boost::any> NAME_MAP_PARAM;
	ParamSwapModel(void);
	~ParamSwapModel(void);

	virtual boost::any* GetInterface( const std::string& name );

	virtual void SetInterface( const std::string& name, const boost::any* pI );

	virtual boost::any* GetParameter( const std::string& name );

	virtual void SetParameter( const std::string& name, const boost::any* pI );
private:
	NAME_MAP_PARAM m_OutParam;
	boost::any* m_pAnyPointer;
};

