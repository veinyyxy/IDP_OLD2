#include "CommonFunction.h"
#include <QTime>


/************************************************************************
 功能说明：nc多维数据转置
 参    数：数据源 ，转置后数据，下标1最大值，下标二最大值，下标三最大值，数据排列顺序  
 作      者：陈志学
 完成日期：2010年4月28日
 更      新：陈志学
 更新日期：
 更新说明：

************************************************************************/

bool DestData( float *sourData , float* destData,const int rangeA,const int rangeB,const int rangeC, Enum_DataOrder dataorder)
{
	 switch (dataorder)
	 {
		 case TZXY:
			 destData = sourData;
			 return true;
			 break;

		 case TXYZ:
			 //T Z XY  to T XY Z  (123 to 132)
			 for (int i = 0; i<rangeA;i++)
			 {
				 for(int j = 0; j<rangeB; j++)
				 {
					 for (int k = 0; k<rangeC;k++)
					 {
						 destData[i*rangeB*rangeC+ k*rangeB+ j] = sourData[i*rangeB*rangeC +j*rangeC + k];
					 }
				 }
			 }
			 return true;
		

		 case XYZT:
			 //T Z XY  to  XY Z T (123 to 321)
			 for (int i = 0; i<rangeA;i++)
			 {
				 for(int j = 0; j<rangeB; j++)
				 {
					 for (int k = 0; k<rangeC;k++)
					 {
						 destData[k*rangeA*rangeB+ j*rangeA+ i] = sourData[i*rangeB*rangeC +j*rangeC + k];
					 }
				 }
			 }
			 return true;
			 break;
		 case XYTZ:
			 //T Z XY  to  XY T Z (123 to 312)
			 for (int i = 0; i<rangeA;i++)
			 {
				 for(int j = 0; j<rangeB; j++)
				 {
					 for (int k = 0; k<rangeC;k++)
					 {
						 destData[k*rangeA*rangeB+ i*rangeB+ j] = sourData[i*rangeB*rangeC +j*rangeC + k];
					 }
				 }
			 }
			 return true;
			 break;
		 case ZTXY:
			 //T Z XY  to   Z T XY (123 to 213)
			 for (int i = 0; i<rangeA;i++)
			 {
				 for(int j = 0; j<rangeB; j++)
				 {
					 for (int k = 0; k<rangeC;k++)
					 {
						 destData[j*rangeA*rangeC + i*rangeC + k] = sourData[i*rangeB*rangeC +j*rangeC + k];
					 }
				 }
			 }
			 return true;
			 break;
		 case ZXYT:
			 //T Z XY  to  Z XY T (123 to 231)
			 for (int i = 0; i<rangeA;i++)
			 {
				 for(int j = 0; j<rangeB; j++)
				 {
					 for (int k = 0; k<rangeC;k++)
					 {
						 destData[j*rangeA*rangeC+ k*rangeA+ i] = sourData[i*rangeB*rangeC +j*rangeC + k];
					 }
				 }
			 }
			 return true;
			 break;
		 default:
			 return false;
	 }

 }

/************************************************************************
 功能说明：nc时间转换
 参    数：datetime 转换后的时间
		   timeScale 时间尺度
		   time 时间描述， 如 “days since 2009-01-01 00:00:00 ”， “hours since 1-1-1 00:00:0.0”
 作    者：韩猛
 完成日期：2010年5月19日
 更新人员：
 更新日期：
 更新说明：

************************************************************************/
bool  GetDateInfoFromUnit(QDateTime& datetime, CTimeScale &timeScale, const QString& time)
{
	//时间尺度为日
	if(time.contains("day") && time.contains("-"))
	{
		timeScale = CTimeScale(1,3);
	}
	else if(time.contains("hour"))
	{
		timeScale = CTimeScale(1,4);
	}
	else
	{
		timeScale = CTimeScale(1,2);
	}

	QRegExp rg("\\d{1,4}-\\d{1,2}-\\d{1,2}");
	int index =  rg.indexIn(time);
	if(index > -1)
	{
		QString sDate = rg.cap(0);
		QDate date;
		char split = '-';
		int year;
		int month;
		int day;
		//year
		index = sDate.indexOf(split);
		if(index > 0)
		{
			year = sDate.left(index).toInt();
			sDate = sDate.right(sDate.length() - index -1);
		}
		//month
		index = sDate.indexOf(split);
		if(index > 0)
		{
			month = sDate.left(index).toInt();
			sDate = sDate.right(sDate.length() - index -1);
			//day
			day = sDate.toInt();
		}
	
		if(year > 0 && month > 0 && day > 0)
		{
			date = QDate(year, month, day);
		}

	/*	if(sDate.length() > 8)
		{
			date = QDate::fromString(sDate, "yyyy-MM-dd");
		}
		else
		{

			sDate = QString("0%1").arg(sDate);
			date = QDate::fromString("0001-01-01", "yyyy-MM-dd");
		}*/

		datetime = QDateTime(date, QTime(0,0));
		return true;
	}

	return false;
}

bool  SetDateTime(QDateTime& dateTime, const QDateTime& begin, const CTimeScale &timeScale, long delt)
{
	if(!timeScale.Valid() || !begin.isValid())
	{
		return false;
	}

	switch(timeScale.GetUnit())
	{
	case 1: //year
		dateTime = begin.addYears(delt);
		break;
	case 2: // month
		dateTime = begin.addMonths(delt);
		break;
	case 3: // day
		dateTime = begin.addDays(delt);
		break;
	case 4: //hour
		{
			long days = delt/24;
			int seconds = (delt % 24)* 3600;
			dateTime = begin.addDays(days);
			dateTime = dateTime.addSecs(seconds);
			break;
		}

	case 5: //minute
		{
			long days = delt/(24*60);
			int seconds = (delt % (24*60)) * 60;
			dateTime = begin.addDays(days);
			dateTime = dateTime.addSecs(seconds);
			break;
		}

	case 6: // second
		{
			long days = delt/(24*3600);
			int seconds = delt%(24*3600);
			dateTime = begin.addDays(days);
			dateTime = dateTime.addSecs(seconds);
			break;
		}
	case CTimeScale::Season: // 季度
		{
			dateTime = begin.addMonths(delt*3);
			break;
		}
	case CTimeScale::TenDays: //旬
		{
			dateTime = begin.addDays(delt * 10);
			break;
		}
	case CTimeScale::FiveDays: // 侯
		{
			dateTime = begin.addDays(delt * 5);
			break;
		}
	case CTimeScale::FiveDaysOfChina: //中国侯
		{
			dateTime = begin.addDays(delt * 5);
			break;
		}
	case CTimeScale::Week: //周
		{
			dateTime = begin.addDays(delt * 7);
			break;
		}
	case CTimeScale::Decade: //年代
		{
			dateTime = begin.addYears(delt * 10);
			break;
		}
	case CTimeScale::Century:
		{
			dateTime = begin.addYears(delt * 100);
			break;
		}
	default:
		return false;
		break;
	}

	return true;
}

//获得时间间隔
bool  GetSteps(long &delt, const QDateTime& dateTime, const QDateTime& begin, const CTimeScale &timeScale)
{
	if(!dateTime.isValid() || !begin.isValid() || !timeScale.Valid())
	{
		return false;
	}

	switch(timeScale.GetUnit())
	{
	case 1: //year
		delt = dateTime.date().year() - begin.date().year();
		break;
	case 2: // month
		delt = dateTime.date().month();
		delt += (dateTime.date().year() - begin.date().year() -1) * 12;
		delt += (12 - begin.date().month() + 1);
		break;
	case 3: // day
		delt = begin.daysTo(dateTime);
		break;
	case 4: //hour
		delt = begin.daysTo(dateTime)*24;
		delt += dateTime.time().hour();
		delt += (24 - begin.time().hour());
		delt -= 24;
		break;
	case 5: //minute
		delt = begin.daysTo(dateTime)*24*60;
		delt += begin.time().secsTo(dateTime.time())/60;
		break;
	case 6: //second
		delt = begin.daysTo(dateTime)*24*3600;
		delt += begin.time().secsTo(dateTime.time());
		break;
	case CTimeScale::Season: // 季度
		{
			delt = dateTime.date().month();
			delt += (dateTime.date().year() - begin.date().year() -1) * 12;
			delt += (12 - begin.date().month() + 1);
			delt /= 3;
			break;
		}
	case CTimeScale::TenDays: //旬
		{
			//dateTime = begin.addDays(delt * 10);
			break;
		}
	case CTimeScale::FiveDays: // 侯
		{
			//dateTime = begin.addDays(delt * 5);
			break;
		}
	case CTimeScale::FiveDaysOfChina: //中国侯
		{
			//dateTime = begin.addDays(delt * 5);
			break;
		}
	case CTimeScale::Week: //周
		{
			//dateTime = begin.addDays(delt * 7);
			break;
		}
	case CTimeScale::Decade: //年代
		{
			//dateTime = begin.addYears(delt * 10);
			break;
		}
	case CTimeScale::Century:
		{
			//dateTime = begin.addYears(delt * 100);
			break;
		}
	default:
		return false;
		break;
	}

	return true;
}

/************************************************************************
 功能说明：将时间转换为对应的时间尺度下的字符串， 并按从小到大排序
 参    数：日期、时间尺度  
 作      者：韩猛
 完成日期：2010年6月13日
 更新日期：
 更新说明：

************************************************************************/
QStringList  GetDateStringList(const QHash<long, QDateTime> &dates, const CTimeScale &ts)
{
	QStringList dateList;
	if(dates.isEmpty() || !ts.Valid())
	{
		return dateList;
	}

	QString formats;
	switch(ts.GetUnit())
	{
	case 1:
		formats = "yyyy";
		break;

	case 2:
		formats = "yyyy-MM";
		break;

	case 3:
		formats = "yyyy-MM-dd";
		break;

	case 4:
		formats = "yyyy-MM-dd HH";
		break;

	case 5:
		formats = "yyyy-MM-dd HH:mm";
		break;

	case 6:
		formats = "yyyy-MM-dd HH:mm:ss";
		break;
	default:
		formats = "yyyy-MM-dd HH:mm:ss";
		break;

	}

	QHash<long, QDateTime>::const_iterator it = dates.constBegin();
	for(; it != dates.constEnd(); ++it)
	{
		dateList.append(it.value().toString(formats));
	}

	dateList.sort();

	return dateList;
}
