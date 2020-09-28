#include "LonLatRange.h"

//////////////////////////////////// begin CLonLatRange 
/************************************************************************/
/* �������ܣ� ���캯��  						                        */
/* ����˵��:  NULL														*/
/* ��    ��:  ����													    */
/* ��    ��:  2011 �� �� �� �� ��										*/
/************************************************************************/
CLonLatRange::CLonLatRange()
	:m_begin(0), m_end(0), m_delt(0), m_count(0), m_precision(0.01)
{

}
CLonLatRange::CLonLatRange(ValueType begin, ValueType end, ValueType delt, long int count, QString units, ValueType precision)
	//:m_begin(begin), m_end(end), m_delt(delt), m_count(count)
{
	if( ((end - begin) - (count -1)* delt) < precision)
	{
		m_begin = begin;
		m_end = end;
		m_delt = delt;
		m_count = count;
		m_precision = precision;
		m_units = units;
	}
	else
	{
		m_begin = 0;
		m_end = 0;
		m_delt = 1;
		m_count = 0;
		m_precision = 0.01;

	}

}
CLonLatRange::CLonLatRange(const CLonLatRange&range)
{
	//if(range.Valid())//�Ը���ע��
	{
		m_begin = range.m_begin;
		m_end = range.m_end;
		m_delt = range.m_delt;
		m_count = range.m_count;
		m_precision = m_precision;
		m_units = range.m_units;	
	}
}
/************************************************************************/
/* �������ܣ� ���������  						                        */
/* ����˵��:  NULL														*/
/* ��    ��:  ����													    */
/* ��    ��:  2011 �� �� �� �� ��										*/
/************************************************************************/
CLonLatRange& CLonLatRange::operator = (const CLonLatRange& range)
{
	//if(range.Valid())//�Ը���ע��
	{
		m_begin = range.m_begin;
		m_end = range.m_end;
		m_delt = range.m_delt;
		m_count = range.m_count;
		m_precision = m_precision;
		m_units = range.m_units;
	}
	return *this;
}
/************************************************************************/
/* �������ܣ� ���� (end - begin) == delt * (count -1);		            */
/* ����˵��:  NULL														*/
/* �� �� ֵ�� ��				                                        */
/* ��    ��:  ����													    */
/* ��    ��:  2011 �� �� �� �� ��										*/
/************************************************************************/
bool CLonLatRange::Valid() const // return (end - begin) == delt * (count -1);
{
	return qAbs(((m_end - m_begin) - (m_count -1)* m_delt)) < m_precision*10;
}

long CLonLatRange::Index(const ValueType value) const
{
	if(m_begin < m_end)
	{
		if(value < m_begin || value > m_end || m_delt == 0)
		{	
			return -1;
		}
	}
	if(m_begin > m_end)
	{
		if(value > m_begin || value < m_end || m_delt == 0)
		{	
			return -1;
		}
	}

	long index = (value - m_begin) / m_delt;
	return index;
}
/************************************************************************/
/* �������ܣ� �������ֵ		       			                        */
/* ����˵��:  index														*/
/* �� �� ֵ�� value				                                        */
/* ��    ��:  ����													    */
/* ��    ��:  2011 �� �� �� �� ��										*/
/************************************************************************/
CLonLatRange::ValueType CLonLatRange::Value(const long index) const
{
	if(index < 0 || index >= m_count || m_delt == 0)
	{	
		return -1;
	}

	CLonLatRange::ValueType value = m_begin + index * m_delt;
	return value;
}

void CLonLatRange::ResetCount()
{
	if(qAbs(m_delt) < m_precision )
	{
		return;
	}

	m_count = (m_end - m_begin)/m_delt + 1;

}

bool CLonLatRange:: operator == (const CLonLatRange & other)
{
	if(m_begin == other.m_begin && m_end == other.m_end && (m_delt == other.m_delt || m_count == other.m_count))
	{
		return true;
	}

	return false;
}

bool CLonLatRange:: operator != (const CLonLatRange & other)
{
	//if(m_begin == other.m_begin && m_end == other.m_end && (m_delt == other.m_delt || m_count == other.m_count))
	//{
	//	return true;
	//}

	//return false;


	return !(*this == other);
}
//////////////////////////////////// end CLonLatRange 
