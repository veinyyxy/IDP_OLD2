#include "TimeScale.h"


//////////////////////////////////// CTimeScale 
/************************************************************************/
/* �������ܣ� ���캯��  						                        */
/* ����˵��:  NULL														*/
/* ��    ��:  ����													    */
/* ��    ��:  2011 �� �� �� �� ��										*/
/************************************************************************/
CTimeScale::CTimeScale():m_value(0), m_unit(0)
{
}

CTimeScale::CTimeScale(int value, int unit):m_value(value), m_unit(unit)
{
}


CTimeScale::CTimeScale(QString sTime)
{
	if(sTime.isNull() || sTime.isEmpty())
	{
		m_unit = 0;
		m_value = 0;
		return ;
	}

	//year yyyy-mm
	if(4 < sTime.length())
	{
		QString sUnit = sTime.left(4);
		bool ok;
		int iValue = sUnit.toInt(&ok);
		if(ok && iValue)
		{
			m_unit = 1;
			m_value = iValue;
			return;
		}
	}
	// month mm-dd
	sTime = sTime.right(sTime.length()-5);
	if(2 < sTime.length())
	{
		QString sUnit = sTime.left(2);
		bool ok;
		int iValue = sUnit.toInt(&ok);
		if(ok && iValue)
		{
			m_unit = 2;
			m_value = iValue;
			return;
		}
	}

	// 3 day dd hh
	sTime = sTime.right(sTime.length()-3);
	if(2 < sTime.length())
	{
		QString sUnit = sTime.left(2);
		bool ok;
		int iValue = sUnit.toInt(&ok);
		if(ok && iValue)
		{
			m_unit = 3;
			m_value = iValue;
			return;
		}
	}
	// 4 hour hh:mi
	sTime = sTime.right(sTime.length()-3);
	if(2 < sTime.length())
	{
		QString sUnit = sTime.left(2);
		bool ok;
		int iValue = sUnit.toInt(&ok);
		if(ok && iValue)
		{
			m_unit = 4;
			m_value = iValue;
			return;
		}
	}	
	// 5 minute mi:ss
	sTime = sTime.right(sTime.length()-3);
	if(2 < sTime.length())
	{
		QString sUnit = sTime.left(2);
		bool ok;
		int iValue = sUnit.toInt(&ok);
		if(ok && iValue)
		{
			m_unit = 3;
			m_value = iValue;
			return;
		}
	}	
	// 6 second ss
	sTime = sTime.right(sTime.length()-3);
	if(2 < sTime.length())
	{
		QString sUnit = sTime.left(2);
		bool ok;
		int iValue = sUnit.toInt(&ok);
		if(ok && iValue)
		{
			m_unit = 3;
			m_value = iValue;
			return;
		}
	}	
}

CTimeScale::CTimeScale(const CTimeScale &ts):m_unit(ts.m_unit), m_value(ts.m_value)
{
}
/************************************************************************/
/* �������ܣ� ���������  						                        */
/* ����˵��:  NULL														*/
/* ��    ��:  ����													    */
/* ��    ��:  2011 �� �� �� �� ��										*/
/************************************************************************/
CTimeScale& CTimeScale::operator =(const CTimeScale &ts)
{
	m_value = ts.m_value;
	m_unit = ts.m_unit;
	return *this;
}
/************************************************************************/
/* �������ܣ� ����������Ϣ									            */
/* ����˵��:  NULL														*/
/* �� �� ֵ�� m_value  m_unit                                           */
/* ��    ��:  ����													    */
/* ��    ��:  2011 �� �� �� �� ��										*/
/************************************************************************/
QString CTimeScale::Description() 
{
	QString sUnit = GetUnitDescription();
	if(m_value == 1)
	{
		return QString("%1 %2").arg(m_value).arg(m_unit);
	}
	else
	{
		return QString("%1 %2s").arg(m_value).arg(m_unit);
	}
}

QString CTimeScale::GetUnitDescription()
{
	QString sUnit;
	switch(m_unit)
	{
	case 1:
		sUnit = "year";
		break;
	case 2:
		sUnit = "month";
		break;
	case 3:
		sUnit = "day";
		break;
	case 4:
		sUnit = "hour";
		break;
	case 5:
		sUnit = "minute";
		break;
	case 6:
		sUnit = "second";
		break;
	case 7:
		sUnit = "season";
		break;
	case 8:
		sUnit = "tenDays";
		break;
	case 9:
		sUnit = "fiveDays";
		break;
	case 10:
		sUnit = "fiveDaysOfChina";
		break;
	case 11:
		sUnit = "week";
		break;
	case 12:
		sUnit = "decade";
		break;
	case 13:
		sUnit = "century";
		break;
	default:
		sUnit = "UnKnown";
		break;
	}
	return sUnit;
}
/************************************************************************/
/* �������ܣ� ���ֵ										            */
/* ����˵��:  NULL														*/
/* �� �� ֵ�� m_value		                                            */
/* ��    ��:  ����													    */
/* ��    ��:  2011 �� �� �� �� ��										*/
/************************************************************************/
int CTimeScale::GetValue()const
{
	return m_value;
}
/************************************************************************/
/* �������ܣ� ��õ�λ										            */
/* ����˵��:  NULL														*/
/* �� �� ֵ�� m_unit		                                            */
/* ��    ��:  ����													    */
/* ��    ��:  2011 �� �� �� �� ��										*/
/************************************************************************/
int  CTimeScale::GetUnit() const
{
	return m_unit;
}
bool CTimeScale::Valid() const
{
	return m_unit <= 13 && m_unit >0 && m_value;
}
//////////////////////////////////// end CTimeScale 
