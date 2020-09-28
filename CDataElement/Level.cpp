#include "Level.h"


//////////////////////////////////// CLevel 
/************************************************************************/
/* �������ܣ� ���캯��  						                        */
/* ����˵��:  NULL														*/
/* ��    ��:  ����													    */
/* ��    ��:  2011 �� �� �� �� ��										*/
/************************************************************************/
CLevel::CLevel()
{
	//m_valus = new QHash<int, float>();
	m_range = QPair<float, float>(0,0);
	m_count = 0;
	m_selectedCount = 0;
}

CLevel::CLevel(const CLevel &level)
{
	m_range = level.m_range;
	m_units = level.m_units;

	//clear
	m_valus.clear();
	m_selected.clear();
	//insert
	QHash<int, float>::ConstIterator it;
	for(it = level.m_valus.constBegin(); it != level.m_valus.constEnd(); ++it)
	{
		m_valus.insert(it.key(), it.value());
	}
	for(it = level.m_selected.constBegin(); it != level.m_selected.constEnd(); ++it)
	{
		m_selected.insert(it.key(), it.value());
	}
	m_count = level.m_count;
	m_selectedCount = level.m_selectedCount;
}
/************************************************************************/
/* �������ܣ� ���������						                        */
/* ����˵��:  NULL														*/
/* ��    ��:  ����													    */
/* ��    ��:  2011 �� �� �� �� ��										*/
/************************************************************************/
CLevel& CLevel::operator=(const CLevel &level)
{
	m_range = level.m_range;
	m_units = level.m_units;

	//clear
	m_valus.clear();
	m_selected.clear();
	//insert
	QHash<int, float>::ConstIterator it;
	for(it = level.m_valus.constBegin(); it != level.m_valus.constEnd(); ++it)
	{
		m_valus.insert(it.key(), it.value());
	}
	for(it = level.m_selected.constBegin(); it != level.m_selected.constEnd(); ++it)
	{
		m_selected.insert(it.key(), it.value());
	}

	return *this;

}
/************************************************************************/
/* �������ܣ� ��������  						                        */
/* ����˵��:  NULL														*/
/* ��    ��:  ����													    */
/* ��    ��:  2011 �� �� �� �� ��										*/
/************************************************************************/
CLevel::~CLevel()
{
	m_valus.clear();
	m_selected.clear();
}
//////////////////////////////////// end CLevel 

