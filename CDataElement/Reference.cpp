#include "Reference.h"
/************************************************************************/
/* 函数功能： 构造函数  						                        */
/* 参数说明:  NULL														*/
/* 作    者:  韩猛													    */
/* 日    期:  2011 年 ？ 月 ？ 日										*/
/************************************************************************/
CReference::CReference():pCount(new int(1))
{
}

CReference::CReference(const CReference& ref):pCount(ref.pCount)
{
	++*pCount;
}
/************************************************************************/
/* 函数功能： 重载运算符  						                        */
/* 参数说明:  NULL														*/
/* 作    者:  韩猛													    */
/* 日    期:  2011 年 ？ 月 ？ 日										*/
/************************************************************************/
CReference& CReference::operator = (const CReference& ref)
{
	if(--*pCount == 0)
	{
		delete pCount;
	}

	pCount = ref.pCount;
	++*pCount;
	return *this;
}
/************************************************************************/
/* 函数功能： 析构造函数  						                        */
/* 参数说明:  NULL														*/
/* 作    者:  韩猛													    */
/* 日    期:  2011 年 ？ 月 ？ 日										*/
/************************************************************************/
CReference::~CReference(void)
{
	if(--*pCount == 0)
	{
		delete pCount;
		pCount = NULL;
	}
}

bool CReference::Reattach(const	CReference& ref)
{
	++*ref.pCount;
	if(--*pCount)
	{
		delete pCount;
		pCount = ref.pCount;
		return true;
	}
	pCount = ref.pCount;
	return false;
}
/************************************************************************/
/* 函数功能： 设置pCount									            */
/* 参数说明:  NULL														*/
/* 返 回 值： pCount			                                        */
/* 作    者:  韩猛													    */
/* 日    期:  2011 年 ？ 月 ？ 日										*/
/************************************************************************/
bool CReference::Only()
{
	return *pCount == 1;
}
/************************************************************************/
/* 函数功能： 判断pCount									            */
/* 参数说明:  NULL														*/
/* 返 回 值： bool				                                        */
/* 作    者:  韩猛													    */
/* 日    期:  2011 年 ？ 月 ？ 日										*/
/************************************************************************/
bool CReference::MakeOnly()
{
	if(*pCount == 1)
	{
		return false;
	}
	--*pCount;
	pCount = new int(1);
	return true;
}

const int CReference::GetRefCount()
{
	return *pCount;
}