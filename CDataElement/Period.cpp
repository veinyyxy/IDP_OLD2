#include "Period.h"

//////////////////////////////////// begin CPeriod 
/************************************************************************/
/* �������ܣ� ���캯��  						                        */
/* ����˵��:  NULL														*/
/* ��    ��:  ����													    */
/* ��    ��:  2011 �� �� �� �� ��										*/
/************************************************************************/
CPeriod::CPeriod():m_count(0)
{
}

CPeriod::CPeriod(const CPeriod &p)
{
	m_begin = p.m_begin;
	m_end = p.m_end;
	m_count = p.m_count;
	m_timeScale = p.m_timeScale;
	m_hashDate.clear();
	QHash<long, QDateTime>::const_iterator it;
	for(it = p.m_hashDate.constBegin(); it != p.m_hashDate.constEnd(); ++it)
	{
		m_hashDate.insert(it.key(), it.value());
	}
}
/************************************************************************/
/* �������ܣ� ���������  						                        */
/* ����˵��:  NULL														*/
/* ��    ��:  ����													    */
/* ��    ��:  2011 �� �� �� �� ��										*/
/************************************************************************/
CPeriod& CPeriod::operator = (const CPeriod &p)
{
	m_begin = p.m_begin;
	m_end = p.m_end;
	m_count = p.m_count;
	m_timeScale = p.m_timeScale;
	m_hashDate.clear();
	QHash<long, QDateTime>::const_iterator it;
	for(it = p.m_hashDate.constBegin(); it != p.m_hashDate.constEnd(); ++it)
	{
		m_hashDate.insert(it.key(), it.value());
	}
	return *this;
}
/************************************************************************/
/* �������ܣ� ��������  						                        */
/* ����˵��:  NULL														*/
/* ��    ��:  ����													    */
/* ��    ��:  2011 �� �� �� �� ��										*/
/************************************************************************/
CPeriod::~CPeriod()
{
	m_hashDate.clear();
}

long CPeriod::Index(const QDateTime& dateTime ) const
{
	//year
	//if(!m_timeScale.Valid())
	//	return 0;

	if(m_timeScale.GetUnit() == 1)
	{
		QDate date(dateTime.date().year(),1,1);
		QDateTime newTime(date, QTime(0,0));
		int index =  m_hashDate.key(newTime);
		return index;
	}
	//month
	if(m_timeScale.GetUnit() == 2)
	{
		QDate date(dateTime.date().year(),dateTime.date().month(),1);
		QDateTime newTime(date, QTime(0,0));
		int index =  m_hashDate.key(newTime);
		return index;
	}
	//day
	if(m_timeScale.GetUnit() == 3)
	{
		QDate date(dateTime.date());
		QDateTime newTime(date, QTime(0,0));
		int index =  m_hashDate.key(newTime);
		return index;

	}
	//hour
	if(m_timeScale.GetUnit() == 4)
	{
		QDate date(dateTime.date());
		int hour = dateTime.time().hour();
		QDateTime newTime(date, QTime(hour,0));
		int index =  m_hashDate.key(newTime);
		return index;
	}
	//minute
	if(m_timeScale.GetUnit() == 5)
	{
		QDate date(dateTime.date());
		int hour = dateTime.time().hour();
		int munite = dateTime.time().minute();
		QDateTime newTime(date, QTime(hour, munite));
		int index =  m_hashDate.key(newTime);
		return index;
	}
	//second
	if(m_timeScale.GetUnit() == 6)
	{
	/*	QDate date(dateTime.date());
		QDateTime newTime(date, QTime(0,0));*/
		int index =  m_hashDate.key(dateTime);
		return index;
	}
	//others
	return 0;
}
QDateTime CPeriod::Value(long index)
{
	//year
	if(m_timeScale.GetUnit() == 1)
	{
		return m_begin.addYears(index * m_timeScale.GetValue());
	}
	//month
	if(m_timeScale.GetUnit() == 2)
	{
		return m_begin.addMonths(index * m_timeScale.GetValue());
	}
	//day
	if(m_timeScale.GetUnit() == 3)
	{
		return m_begin.addDays(index * m_timeScale.GetValue());
	}
	//hour
	if(m_timeScale.GetUnit() == 4)
	{
		return m_begin.addSecs(3600*index * m_timeScale.GetValue());
	}
	//minute
	if(m_timeScale.GetUnit() == 5)
	{
		return m_begin.addSecs(60*index * m_timeScale.GetValue());
	}
	//second
	if(m_timeScale.GetUnit() == 6)
	{
		return m_begin.addSecs(index * m_timeScale.GetValue());
	}
	//others
	return m_begin;
}

/************************************************************************/
/* �������ܣ� ʱ��߶�										            */
/* ����˵��:  NULL														*/
/* �� �� ֵ�� m_timeScale		                                        */
/* ��    ��:  ����													    */
/* ��    ��:  2011 �� �� �� �� ��										*/
/************************************************************************/
CTimeScale& CPeriod::GetTimeScale()
{
	return m_timeScale;
}
/************************************************************************/
/* �������ܣ� ����ʱ��										            */
/* ����˵��:  NULL														*/
/* �� �� ֵ�� NULL				                                        */
/* ��    ��:  ����													    */
/* ��    ��:  2011 �� �� �� �� ��										*/
/************************************************************************/
void CPeriod::SetTimeScale(const CTimeScale& ts)
{
	m_timeScale = ts;
}
/************************************************************************/
/* �������ܣ� ����ʱ��										            */
/* ����˵��:  NULL														*/
/* �� �� ֵ�� index2 - index1 +1                                        */
/* ��    ��:  ����													    */
/* ��    ��:  2011 �� �� �� �� ��										*/
/************************************************************************/
void CPeriod::ResetCount()
{
	int index1 = Index(m_begin);
	int index2 = Index(m_end);
	m_count = index2 - index1 +1;
}
//////////////////////////////////// end CPeriod 
