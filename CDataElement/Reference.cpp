#include "Reference.h"
/************************************************************************/
/* �������ܣ� ���캯��  						                        */
/* ����˵��:  NULL														*/
/* ��    ��:  ����													    */
/* ��    ��:  2011 �� �� �� �� ��										*/
/************************************************************************/
CReference::CReference():pCount(new int(1))
{
}

CReference::CReference(const CReference& ref):pCount(ref.pCount)
{
	++*pCount;
}
/************************************************************************/
/* �������ܣ� ���������  						                        */
/* ����˵��:  NULL														*/
/* ��    ��:  ����													    */
/* ��    ��:  2011 �� �� �� �� ��										*/
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
/* �������ܣ� �����캯��  						                        */
/* ����˵��:  NULL														*/
/* ��    ��:  ����													    */
/* ��    ��:  2011 �� �� �� �� ��										*/
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
/* �������ܣ� ����pCount									            */
/* ����˵��:  NULL														*/
/* �� �� ֵ�� pCount			                                        */
/* ��    ��:  ����													    */
/* ��    ��:  2011 �� �� �� �� ��										*/
/************************************************************************/
bool CReference::Only()
{
	return *pCount == 1;
}
/************************************************************************/
/* �������ܣ� �ж�pCount									            */
/* ����˵��:  NULL														*/
/* �� �� ֵ�� bool				                                        */
/* ��    ��:  ����													    */
/* ��    ��:  2011 �� �� �� �� ��										*/
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