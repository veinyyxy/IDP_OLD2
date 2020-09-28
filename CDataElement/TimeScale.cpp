#include "TimeScale.h"


//////////////////////////////////// CTimeScale 
/************************************************************************/
/* 函数功能： 构造函数  						                        */
/* 参数说明:  NULL														*/
/* 作    者:  韩猛													    */
/* 日    期:  2011 年 ？ 月 ？ 日										*/
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
/* 函数功能： 重载运算符  						                        */
/* 参数说明:  NULL														*/
/* 作    者:  韩猛													    */
/* 日    期:  2011 年 ？ 月 ？ 日										*/
/************************************************************************/
CTimeScale& CTimeScale::operator =(const CTimeScale &ts)
{
	m_value = ts.m_value;
	m_unit = ts.m_unit;
	return *this;
}
/************************************************************************/
/* 函数功能： 返回描述信息									            */
/* 参数说明:  NULL														*/
/* 返 回 值： m_value  m_unit                                           */
/* 作    者:  韩猛													    */
/* 日    期:  2011 年 ？ 月 ？ 日										*/
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
/* 函数功能： 获得值										            */
/* 参数说明:  NULL														*/
/* 返 回 值： m_value		                                            */
/* 作    者:  韩猛													    */
/* 日    期:  2011 年 ？ 月 ？ 日										*/
/************************************************************************/
int CTimeScale::GetValue()const
{
	return m_value;
}
/************************************************************************/
/* 函数功能： 获得单位										            */
/* 参数说明:  NULL														*/
/* 返 回 值： m_unit		                                            */
/* 作    者:  韩猛													    */
/* 日    期:  2011 年 ？ 月 ？ 日										*/
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
