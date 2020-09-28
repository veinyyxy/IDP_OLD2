#include "StdAfx.h"
#include "ParamSwapModel.h"


ParamSwapModel::ParamSwapModel(void)
{
	m_pAnyPointer = new boost::any;
}


ParamSwapModel::~ParamSwapModel(void)
{
	delete m_pAnyPointer;
}

boost::any* ParamSwapModel::GetInterface( const std::string& name )
{
	NAME_MAP_PARAM::iterator ifind = m_OutParam.find(name);

	if(ifind != m_OutParam.end())
	{
		return &(ifind->second);
	}
	else
		return 0;
}

void ParamSwapModel::SetInterface( const std::string& name, const boost::any* pI )
{
	m_OutParam[name] = *pI;
}

boost::any* ParamSwapModel::GetParameter( const std::string& name )
{
	NAME_MAP_PARAM::iterator ifind = m_OutParam.find(name);
	
	if(ifind != m_OutParam.end())
	{
		return &(ifind->second);
	}
	else
		return 0;
}

void ParamSwapModel::SetParameter( const std::string& name, const boost::any* pI )
{
	m_OutParam[name] = *pI;
}
