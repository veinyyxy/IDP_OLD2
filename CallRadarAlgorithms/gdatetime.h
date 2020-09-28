////////////////////////////////////////////////////////////////////////////////
// gdatetime.h
// \u00e7\u00bb\u0084\u00e7\u00bd\u0091\u00e6\u008b\u00bc\u00e5\u009b\u00be\u00e8\u00b0\u0083\u00e5\u00ba\u00a6 \u00e7\u00ae\u0080\u00e6\u0098\u0093\u00e6\u0097\u00a5\u00e6\u009c\u009f\u00e6\u0097\u00b6\u00e9\u0097\u00b4\u00e5\u00a4\u0084\u00e7\u0090\u0086\u00e7\u00b1\u00bb
// chenxiang 20101220
// \u00e9\u0083\u00a8\u00e5\u0088\u0086\u00e7\u00a4\u00ba\u00e4\u00be\u008b:
//
// GDateTime ptime1(2009,6,17,8,30);   //\u00e7\u0094\u00a8\u00e5\u00b9\u00b4\u00e6\u009c\u0088\u00e6\u0097\u00a5\u00e6\u0097\u00b6\u00e5\u0088\u0086\u00e7\u00a7\u0092\u00e6\u009e\u0084\u00e9\u0080\u00a0
// GDateTime ptime2("200906");         //\u00e4\u00bb\u008e\u00e5\u00ad\u0097\u00e7\u00ac\u00a6\u00e4\u00b8\u00b2\u00e6\u009e\u0084\u00e9\u0080\u00a0(\u00e5\u00ae\u008c\u00e6\u0095\u00b4\u00e6\u0088\u0096\u00e4\u00b8\u008d\u00e5\u00ae\u008c\u00e6\u0095\u00b4\u00e7\u009a\u0084\u00e6\u0097\u00a0\u00e5\u0088\u0086\u00e9\u009a\u0094\u00e7\u00ac\u00a6\u00e6\u00a0\u00bc\u00e5\u00bc\u008f\u00e5\u00ad\u0097\u00e7\u00ac\u00a6\u00e4\u00b8\u00b220090617083059)
// GDateTime ptime3(GDate(2000,2,29),GTime(0,10,0,1));//\u00e7\u0094\u00a8\u00e6\u0097\u00a5\u00e6\u009c\u009f\u00e5\u0092\u008c\u00e6\u0097\u00b6\u00e9\u0097\u00b4\u00e6\u009e\u0084\u00e9\u0080\u00a0
// GDateTime ptime4(ptime3);           //\u00e6\u008b\u00b7\u00e8\u00b4\u009d\u00e6\u009e\u0084\u00e9\u0080\u00a0

// cout<<ptime1.to_string_all()<<endl; //\u00e8\u00bd\u00ac\u00e6\u0088\u0090\u00e5\u00ad\u0097\u00e7\u00ac\u00a6\u00e4\u00b8\u00b2\u00e5\u008c\u0085\u00e6\u008b\u00ac\u00e6\u00af\u00ab\u00e7\u00a7\u009220090617083000000
// cout<<ptime2.to_string()<<endl;     //\u00e8\u00bd\u00ac\u00e5\u00ad\u0097\u00e7\u00ac\u00a6\u00e4\u00b8\u00b2\u00e6\u00a0\u00bc\u00e5\u00bc\u008f20090617083059
// cout<<ptime2.to_string_ex()<<endl;  //\u00e8\u00bd\u00ac\u00e5\u00ad\u0097\u00e7\u00ac\u00a6\u00e4\u00b8\u00b2\u00e6\u00a0\u00bc\u00e5\u00bc\u008f2009-06-17 08:30:59

// ptime2.from_string("2009061712");   //\u00e4\u00bb\u008e\u00e5\u00ad\u0097\u00e7\u00ac\u00a6\u00e4\u00b8\u00b2\u00e8\u008e\u00b7\u00e5\u008f\u0096\u00e6\u0097\u00b6\u00e9\u0097\u00b4 \u00e5\u0092\u008c\u00e6\u009e\u0084\u00e9\u0080\u00a0\u00e5\u0087\u00bd\u00e6\u0095\u00b0\u00e4\u00b8\u0080\u00e6\u00a0\u00b7
// ptime3 -= GMinute(11);              //\u00e5\u00af\u00b9\u00e6\u0097\u00b6\u00e9\u0097\u00b4\u00e8\u00bf\u0090\u00e7\u00ae\u0097 \u00e5\u0087\u008f11\u00e5\u0088\u0086\u00e9\u0092\u009f
// ptime3 += GSecond(9*60);            //\u00e5\u008a\u00a09*60\u00e7\u00a7\u0092
// ptime3 += GHour(2);                 //\u00e5\u008a\u00a02\u00e5\u00b0\u008f\u00e6\u0097\u00b6
// ptime3.getlocaltime();              //\u00e5\u008f\u0096\u00e6\u009c\u00ac\u00e5\u009c\u00b0\u00e6\u0097\u00b6\u00e9\u0097\u00b4
// GMinute m = GSecond(3600);          //\u00e6\u0097\u00b6\u00e9\u0097\u00b4\u00e7\u00b1\u00bb\u00e5\u009e\u008b\u00e4\u00ba\u0092\u00e7\u009b\u00b8\u00e8\u00bd\u00ac\u00e6\u008d\u00a2

// GDateTime().getlocaltime().to_string();//\u00e8\u008e\u00b7\u00e5\u008f\u0096\u00e6\u009c\u00ac\u00e5\u009c\u00b0\u00e6\u0097\u00a5\u00e6\u009c\u009f\u00e6\u0097\u00b6\u00e9\u0097\u00b4\u00e5\u00ad\u0097\u00e7\u00ac\u00a6\u00e4\u00b8\u00b2
//////////////////////////////////////////////////////////////////////////////////

#ifndef _GDATETIME_H_
#define _GDATETIME_H_

#include <string>
#include <ctime>
#include <cstdio>
#include <cassert>
#include <utility>
#include <memory.h>
#include <stdlib.h>

using std::string;
using namespace std::rel_ops;

//\u00e5\u0084\u0092\u00e7\u0095\u00a5\u00e6\u0097\u00a5
namespace GJueDay
{
	//\u00e5\u0084\u0092\u00e7\u0095\u00a5\u00e6\u0097\u00a5\u00e8\u00bd\u00ac\u00e5\u00b9\u00b4\u00e6\u009c\u0088\u00e6\u0097\u00a5  (\u00e5\u0084\u0092\u00e7\u0095\u00a5\u00e6\u0097\u00a5\u00e6\u0098\u00af1970\u00e5\u00b9\u00b41\u00e6\u009c\u00881\u00e6\u0097\u00a5\u00e4\u00b8\u0080\u00e6\u009d\u00a5\u00e6\u0089\u0080\u00e7\u00bb\u008f\u00e8\u00bf\u0087\u00e7\u009a\u0084\u00e5\u00a4\u00a9\u00e6\u0095\u00b0 1 -> 1970 01 01)
	static bool Jul2Greg( int JulDay,int* Year,int* Month,int* Day ) 
	{
		int t1 = int( JulDay ) + 2509157 -1;
		int t2 = 4 * t1 / 146097;
		t1 -= ( 146097 * t2 + 3 ) / 4;
		int yy = 4000 * ( t1 + 1 ) / 1461001;
		t1 = t1 - 1461 * yy / 4 + 31;
		int mm = 80 * t1 / 2447;
		*Day = int( t1 - 2447 * mm / 80 );
		t1 = mm / 11;
		*Month = int( mm + 2 - 12 * t1 );
		*Year = int( 100 * ( t2 - 49 ) + yy + t1 );
		return true;
	}

	// \u00e6\u0097\u00a5\u00e6\u009c\u009f\u00e8\u00bd\u00ac\u00e5\u0084\u0092\u00e7\u0095\u00a5\u00e6\u0097\u00a5 (1970 01 01 ->1)
	static int Greg2Jul( int Year,int Month,int Day ) 
	{
		int y = Year;
		int m = Month;
		int d = Day;	
		int JulDay = int ( d + 1461 *( y + ( m - 14 ) / 12 ) / 4
			+ 367 *(m - 2 - ( m - 14 ) / 12 * 12 ) / 12
			- 3 *(( y + 100 + ( m - 14 ) / 12 ) / 100 ) / 4 - 719499 + 1 );
		return JulDay;
	}
	
	// \u00e6\u00af\u00ab\u00e7\u00a7\u0092\u00e8\u00bd\u00ac\u00e6\u0097\u00b6\u00e9\u0097\u00b4(86399999-> 23 59 59 999)
	static bool MilliSecond2Time(int MilliSecond,int* Hour,int* Min,int* Sec,int* MSecond) 
	{
		if( MilliSecond < 0 || MilliSecond >= ( 24 * 3600 * 1000 ) ) 
		{
			return false;						// \u00e6\u00af\u00ab\u00e7\u00a7\u0092\u00e6\u0097\u00b6\u00e9\u0097\u00b4\u00e8\u008c\u0083\u00e5\u009b\u00b4
		}
		*Hour = int( MilliSecond / 3600 / 1000 );
		MilliSecond -= *Hour * 3600 * 1000;
		*Min = int( MilliSecond / 60 / 1000 );
		MilliSecond -= *Min * 60 * 1000;
		*Sec = int( MilliSecond / 1000 );
		*MSecond = int( MilliSecond - *Sec * 1000 );	
		return true;
	}
	//\u00e8\u008e\u00b7\u00e5\u008f\u0096\u00e5\u00bd\u0093\u00e5\u0089\u008d\u00e6\u0097\u00b6\u00e9\u0097\u00b4\u00e7\u00bb\u0093\u00e6\u009e\u0084\u00e4\u00bd\u0093
	static struct tm get_local_time()
	{
		time_t NowTime;
		struct tm timestruct;
		memset( &timestruct,0,sizeof( struct tm ) );
		NowTime = time( NULL );
		timestruct = *localtime( &NowTime );
		return timestruct;
	}
};


//\u00e6\u0097\u00b6\u00e9\u0097\u00b4\u00e6\u0097\u00a5\u00e6\u009c\u009f\u00e5\u009f\u00ba\u00e7\u00b1\u00bb
class GBaseTime
{
public:
	GBaseTime(int x=0)//\u00e4\u00bb\u008e\u00e6\u0095\u00b4\u00e6\u0095\u00b0\u00e6\u009e\u0084\u00e9\u0080\u00a0
		:m_data(x)
	{

	}
	GBaseTime(const string& str)//\u00e4\u00bb\u008e\u00e5\u00ad\u0097\u00e7\u00ac\u00a6\u00e4\u00b8\u00b2\u00e6\u009e\u0084\u00e9\u0080\u00a0
	{
		from_string(str);//not virtual //\u00e4\u00bb\u008e\u00e5\u00ad\u0097\u00e7\u00ac\u00a6\u00e4\u00b8\u00b2\u00e5\u008f\u0096\u00e6\u0097\u00b6\u00e9\u0097\u00b4
	}
	~GBaseTime() //\u00e6\u009e\u0090\u00e6\u009e\u0084
	{

	}
	GBaseTime(const GBaseTime& other) //\u00e6\u008b\u00b7\u00e8\u00b4\u009d\u00e6\u009e\u0084\u00e9\u0080\u00a0
	{
		m_data = other.m_data;
	}
	GBaseTime& operator=(const GBaseTime& other) //\u00e8\u00b5\u008b\u00e5\u0080\u00bc
	{
		if(this != &other)
		{
			m_data = other.m_data;
		}
		return *this;
	}
	virtual string to_string() const //\u00e8\u00bd\u00ac\u00e5\u00ad\u0097\u00e7\u00ac\u00a6\u00e4\u00b8\u00b2
	{
		char buf[32];
		sprintf(buf,"%02d",m_data);
		return buf;
	}
	virtual size_t from_string(const string& str, size_t len =2, int default_val=0)//\u00e4\u00bb\u008e\u00e5\u00ad\u0097\u00e7\u00ac\u00a6\u00e4\u00b8\u00b2\u00e5\u008f\u0096\u00e6\u0097\u00b6\u00e9\u0097\u00b4 
	{
		if(str.length()>=len)//\u00e9\u0095\u00bf\u00e5\u00ba\u00a6\u00e6\u0098\u00af\u00e5\u0090\u00a6\u00e5\u00a4\u009f
		{
			m_data = atoi(str.substr(0,len).c_str());
			return len;
		}
		else
		{
			if (str.empty())
			{
				m_data = default_val;//\u00e9\u00bb\u0098\u00e8\u00ae\u00a4\u00e5\u0080\u00bc
				return 0;
			}
			else
			{
				m_data = atoi(str.c_str());//\u00e8\u00bd\u00ac\u00e6\u0095\u00b4\u00e6\u0095\u00b0
				return str.length();
			}
		}
	}

//protected:
	int m_data;//\u00e6\u0095\u00b0\u00e6\u008d\u00ae 
};

//\u00e5\u00b9\u00b4
struct GYear:public GBaseTime
{	
	GYear(int x=1970) //\u00e6\u009e\u0084\u00e9\u0080\u00a0
		:GBaseTime(x)
	{

	}
	virtual string to_string() const//\u00e8\u00bd\u00ac\u00e5\u00ad\u0097\u00e7\u00ac\u00a6\u00e4\u00b8\u00b2
	{
		char buf[32];
		sprintf(buf,"%04d",m_data);
		return buf;
	}
	virtual size_t from_string(const string& str, size_t len =4, int default_val=1970)//\u00e4\u00bb\u008e\u00e5\u00ad\u0097\u00e7\u00ac\u00a6\u00e4\u00b8\u00b2\u00e5\u008f\u0096\u00e6\u0097\u00b6\u00e9\u0097\u00b4
	{
		return GBaseTime::from_string(str,len,default_val);
	}
};

//\u00e6\u009c\u0088
struct GMonth:public GBaseTime
{
	GMonth(int x=1) //\u00e6\u009e\u0084\u00e9\u0080\u00a0
		:GBaseTime(x)
	{

	}
	virtual size_t from_string(const string& str, size_t len =2, int default_val=1)//\u00e4\u00bb\u008e\u00e5\u00ad\u0097\u00e7\u00ac\u00a6\u00e4\u00b8\u00b2\u00e5\u008f\u0096\u00e6\u0097\u00b6\u00e9\u0097\u00b4
	{
		return GBaseTime::from_string(str,len,default_val);
	}
};

//\u00e6\u0097\u00a5
struct GDay:public GBaseTime
{
	GDay(int x=1) //\u00e6\u009e\u0084\u00e9\u0080\u00a0
		:GBaseTime(x)
	{

	}
	virtual size_t from_string(const string& str, size_t len =2, int default_val=1)//\u00e4\u00bb\u008e\u00e5\u00ad\u0097\u00e7\u00ac\u00a6\u00e4\u00b8\u00b2\u00e5\u008f\u0096\u00e6\u0097\u00b6\u00e9\u0097\u00b4
	{
		return GBaseTime::from_string(str,len,default_val);
	}
};

//\u00e6\u00af\u00ab\u00e7\u00a7\u0092
struct GMilliSecond:public GBaseTime
{
	GMilliSecond(int x=0) //\u00e6\u009e\u0084\u00e9\u0080\u00a0
		:GBaseTime(x)
	{

	}
	virtual string to_string() const //\u00e8\u00bd\u00ac\u00e5\u00ad\u0097\u00e7\u00ac\u00a6\u00e4\u00b8\u00b2
	{
		char buf[32];
		sprintf(buf,"%03d",m_data);
		return buf;
	}
	virtual size_t from_string(const string& str, size_t len =3)//\u00e4\u00bb\u008e\u00e5\u00ad\u0097\u00e7\u00ac\u00a6\u00e4\u00b8\u00b2\u00e5\u008f\u0096\u00e6\u0097\u00b6\u00e9\u0097\u00b4
	{
		return GBaseTime::from_string(str,len);
	}
	GMilliSecond operator+(const GMilliSecond& other) const //\u00e6\u00af\u00ab\u00e7\u00a7\u0092\u00e7\u009b\u00b8\u00e5\u008a\u00a0
	{
		return GMilliSecond(m_data + other.m_data);//\u00e5\u00b0\u008f\u00e5\u00bf\u0083\u00e6\u00ba\u00a2\u00e5\u0087\u00ba
	}
	GMilliSecond operator-() const//\u00e7\u009b\u00b8\u00e5\u008f\u008d\u00e6\u0095\u00b0(\u00e5\u008f\u0096\u00e8\u00b4\u009f\u00e6\u0095\u00b0)
	{
		return GMilliSecond(-m_data);
	}
};

//\u00e5\u00b0\u008f\u00e6\u0097\u00b6
struct GHour:public GBaseTime
{	
	GHour(int x=0) //\u00e6\u009e\u0084\u00e9\u0080\u00a0
		:GBaseTime(x)
	{

	}
	operator GMilliSecond() const //\u00e5\u00bc\u00ba\u00e5\u0088\u00b6\u00e8\u00bd\u00ac\u00e6\u008d\u00a2\u00e6\u0088\u0090\u00e6\u00af\u00ab\u00e7\u00a7\u0092
	{
		return GMilliSecond(m_data*60*60*1000);
	}
};

//\u00e5\u0088\u0086\u00e9\u0092\u009f
struct GMinute:public GBaseTime
{
	GMinute(int x=0) //\u00e6\u009e\u0084\u00e9\u0080\u00a0
		:GBaseTime(x)
	{

	}
	operator GMilliSecond() const //\u00e5\u00bc\u00ba\u00e5\u0088\u00b6\u00e8\u00bd\u00ac\u00e6\u008d\u00a2\u00e6\u0088\u0090\u00e6\u00af\u00ab\u00e7\u00a7\u0092
	{
		return GMilliSecond(m_data*60*1000);
	}
};

//\u00e7\u00a7\u0092
struct GSecond:public GBaseTime
{
	GSecond(int x=0) //\u00e6\u009e\u0084\u00e9\u0080\u00a0
		:GBaseTime(x) 
	{

	}
	operator GMilliSecond() const //\u00e5\u00bc\u00ba\u00e5\u0088\u00b6\u00e8\u00bd\u00ac\u00e6\u008d\u00a2\u00e6\u0088\u0090\u00e6\u00af\u00ab\u00e7\u00a7\u0092
	{
		return GMilliSecond(m_data*1000);
	}
};

//\u00e6\u0097\u00a5\u00e6\u009c\u009f
class GDate:public GBaseTime
{
public:
	GDate(int y=1970, int m=1, int d=1)//\u00e6\u009e\u0084\u00e9\u0080\u00a0
		:m_year(y)
		,m_month(m)
		,m_day(d)
	{

	}
	GDate(const string& str) //\u00e4\u00bb\u008e\u00e5\u00ad\u0097\u00e7\u00ac\u00a6\u00e4\u00b8\u00b2\u00e6\u009e\u0084\u00e9\u0080\u00a0
	{
		from_string(str);//\u00e4\u00bb\u008e\u00e5\u00ad\u0097\u00e7\u00ac\u00a6\u00e4\u00b8\u00b2\u00e5\u008f\u0096\u00e6\u0097\u00b6\u00e9\u0097\u00b4
	}
	~GDate() //\u00e6\u009e\u0090\u00e6\u009e\u0084
	{

	}
	GDate(const GDate& other) //\u00e6\u008b\u00b7\u00e8\u00b4\u009d\u00e6\u009e\u0084\u00e9\u0080\u00a0
	{
		m_year = other.m_year;
		m_month = other.m_month;
		m_day = other.m_day;
	}
	GDate& operator=(const GDate& other)
	{
		if(this != &other)
		{
			m_year = other.m_year;
			m_month = other.m_month;
			m_day = other.m_day;
		}
		return *this;
	}

	string to_string() const
	{
		return (m_year.to_string()+m_month.to_string()+m_day.to_string());
	}
	string to_string_ex() const
	{
		return (m_year.to_string()+"-"+m_month.to_string()+"-"+m_day.to_string());
	}
	size_t from_string(const string& str)//\u00e4\u00bb\u008e\u00e5\u00ad\u0097\u00e7\u00ac\u00a6\u00e4\u00b8\u00b2\u00e5\u008f\u0096\u00e6\u0097\u00b6\u00e9\u0097\u00b4
	{
		size_t len = 0;
		len += m_year.from_string(str.substr(len));
		len += m_month.from_string(str.substr(len));
		len += m_day.from_string(str.substr(len));
		return len;
	}
	GDate& operator-=(const GDay& day)//\u00e5\u0087\u008f\u00e5\u0087\u00a0\u00e5\u00a4\u00a9
	{
		//\u00e8\u00bd\u00ac\u00e5\u0084\u0092\u00e7\u0095\u00a5\u00e6\u0097\u00a5\u00e6\u009d\u00a5\u00e5\u0087\u008f
		int jday = GJueDay::Greg2Jul(m_year.m_data, m_month.m_data, m_day.m_data);
		jday -= day.m_data;
		GJueDay::Jul2Greg(jday,&m_year.m_data, &m_month.m_data, &m_day.m_data);
		return *this;
	}
	GDate& operator+=(const GDay& day)//\u00e5\u008a\u00a0\u00e5\u0087\u00a0\u00e5\u00a4\u00a9
	{
		//\u00e8\u00bd\u00ac\u00e5\u0084\u0092\u00e7\u0095\u00a5\u00e6\u0097\u00a5\u00e6\u009d\u00a5\u00e5\u0087\u008f
		int jday = GJueDay::Greg2Jul(m_year.m_data, m_month.m_data, m_day.m_data);
		jday += day.m_data;
		GJueDay::Jul2Greg(jday,&m_year.m_data, &m_month.m_data, &m_day.m_data);
		return *this;
	}

	GDay operator-(const GDate& other)//\u00e4\u00b8\u00a4\u00e4\u00b8\u00aa\u00e6\u0097\u00a5\u00e6\u009c\u009f\u00e7\u009b\u00b8\u00e5\u0087\u008f\u00e5\u00be\u0097\u00e5\u0088\u00b0\u00e5\u00a4\u00a9\u00e6\u0095\u00b0
	{
		//\u00e8\u00bd\u00ac\u00e5\u0084\u0092\u00e7\u0095\u00a5\u00e6\u0097\u00a5\u00e6\u009d\u00a5\u00e5\u0087\u008f
		int jday = GJueDay::Greg2Jul(m_year.m_data, m_month.m_data, m_day.m_data);
		int otherjday = GJueDay::Greg2Jul(other.m_year.m_data, other.m_month.m_data, other.m_day.m_data);
		jday -= otherjday;
		return GDay(jday);
	}

	GDate& getlocaltime() 
	{
		struct tm timestruct = GJueDay::get_local_time();
		*this = GDate( timestruct.tm_year + 1900, timestruct.tm_mon + 1, timestruct.tm_mday ); // \u00e5\u00b0\u0086\u00e8\u008e\u00b7\u00e5\u008f\u0096\u00e5\u0088\u00b0\u00e7\u009a\u0084\u00e5\u00bd\u0093\u00e5\u0089\u008d\u00e5\u00b9\u00b4\u00e6\u009c\u0088\u00e6\u0097\u00a5\u00e8\u00bd\u00ac\u00e4\u00b8\u00ba\u00e5\u0084\u0092\u00e7\u0095\u00a5\u00e6\u0097\u00a5
		return *this;
	}

//protected:
	GYear  m_year;
	GMonth m_month;
	GDay   m_day;
};

//\u00e6\u0097\u00b6\u00e9\u0097\u00b4\u00e5\u00a4\u0084\u00e7\u0090\u0086
class GTime:public GBaseTime
{
public:
	GTime(int h=0, int m=0, int s=0, int ms=0)
		:m_hour(h)
		,m_minute(m)
		,m_second(s)
		,m_msecond(ms)
	{

	}
	GTime(const string& str)
	{
		from_string(str);//\u00e4\u00bb\u008e\u00e5\u00ad\u0097\u00e7\u00ac\u00a6\u00e4\u00b8\u00b2\u00e5\u008f\u0096\u00e6\u0097\u00b6\u00e9\u0097\u00b4
	}
	~GTime()
	{

	}
	GTime(const GTime& other)
	{
		m_hour = other.m_hour;
		m_minute = other.m_minute;
		m_second = other.m_second;
		m_msecond = other.m_msecond;
	}
	GTime& operator=(const GTime& other)
	{
		if(this != &other)
		{
			m_hour = other.m_hour;
			m_minute = other.m_minute;
			m_second = other.m_second;
			m_msecond = other.m_msecond;
		}
		return *this;
	}

	string to_string() const
	{
		return (m_hour.to_string()+m_minute.to_string()+
                m_second.to_string()/*+m_msecond.to_string()*/);
	}	
	string to_string_ex() const
	{
		return (m_hour.to_string() + ":" +
			    m_minute.to_string() + ":" +
			    m_second.to_string());
	}
	string to_string_all() const
	{
		return (to_string() + m_msecond.to_string());
	}
	size_t from_string(const string& str)//\u00e4\u00bb\u008e\u00e5\u00ad\u0097\u00e7\u00ac\u00a6\u00e4\u00b8\u00b2\u00e5\u008f\u0096\u00e6\u0097\u00b6\u00e9\u0097\u00b4
	{
		size_t len = 0;
		len += m_hour.from_string(str.substr(len));
		len += m_minute.from_string(str.substr(len));
		len += m_second.from_string(str.substr(len));
		len += m_msecond.from_string(str.substr(len));
		return len;
	}
	operator GMilliSecond() const //
	{
		return (GMilliSecond(m_hour)   + GMilliSecond(m_minute)+
				GMilliSecond(m_second) + m_msecond);
	}
	void from_msecond(int msecond) //\u00e4\u00bb\u008e\u00e6\u00af\u00ab\u00e7\u00a7\u0092\u00e8\u008e\u00b7\u00e5\u008f\u0096
	{
		m_hour    =GHour(msecond/60/60/1000);
		m_minute  =GMinute((msecond/60/1000)%60);
		m_second  =GSecond((msecond/1000)%60);
		m_msecond =GMilliSecond(msecond%1000);	
	}
	int add_msecond(GMilliSecond millisecond) //\u00e5\u008a\u00a0\u00e6\u00af\u00ab\u00e7\u00a7\u0092
	{
		int this_ms = GMilliSecond(*this).m_data;
		int msecond = millisecond.m_data;
		const int oneday_ms = (24*60*60*1000);
		int ms =0;
		int day = 0;

        if (msecond>=0 ||
			msecond<0  && this_ms + msecond >= 0)
		{
			ms  = (this_ms + msecond) % oneday_ms;
			day = (this_ms + msecond) / oneday_ms;
		}
		else
		{
			ms  = oneday_ms + (this_ms + msecond) % oneday_ms;
			day = (this_ms + msecond) / oneday_ms -1;
		}

		this->from_msecond(ms);
		return day;
	}
	GTime& getlocaltime() //\u00e5\u008f\u0096\u00e6\u009c\u00ac\u00e5\u009c\u00b0\u00e6\u0097\u00b6\u00e9\u0097\u00b4
	{
		struct tm timestruct = GJueDay::get_local_time();
		*this = GTime( timestruct.tm_hour,timestruct.tm_min,timestruct.tm_sec,0 ); // \u00e5\u00b0\u0086\u00e8\u008e\u00b7\u00e5\u008f\u0096\u00e5\u0088\u00b0\u00e7\u009a\u0084\u00e5\u00bd\u0093\u00e5\u0089\u008d\u00e6\u0097\u00b6\u00e9\u0097\u00b4\u00e8\u00bd\u00ac\u00e4\u00b8\u00ba\u00e6\u00af\u00ab\u00e7\u00a7\u0092\u00e8\u00ae\u00a1\u00e6\u0097\u00b6
		return *this;
	}

//protected:
	GHour        m_hour;    //\u00e5\u00b0\u008f\u00e6\u0097\u00b6
	GMinute      m_minute;  //\u00e5\u0088\u0086\u00e9\u0092\u009f
	GSecond      m_second;  //\u00e7\u00a7\u0092
	GMilliSecond m_msecond; //\u00e6\u00af\u00ab\u00e7\u00a7\u0092
};

//\u00e6\u0097\u00b6\u00e9\u0097\u00b4\u00e6\u0097\u00a5\u00e6\u009c\u009f\u00e5\u00a4\u0084\u00e7\u0090\u0086
class GDateTime:public GBaseTime
{
public:
	GDateTime(const GDate& date, const GTime& time) //\u00e4\u00bb\u008e\u00e6\u0097\u00a5\u00e6\u009c\u009f\u00e6\u0097\u00b6\u00e9\u0097\u00b4\u00e6\u009e\u0084\u00e9\u0080\u00a0
		:m_date(date)
		,m_time(time)
	{

	}
	GDateTime(int y=0, int m=0, int d=0,
		      int hh=0, int mm=0, int ss=0, int ms=0) //\u00e4\u00bb\u008e\u00e6\u0095\u00b4\u00e6\u0095\u00b0 \u00e5\u00b9\u00b4\u00e6\u009c\u0088\u00e6\u0097\u00a5\u00e6\u0097\u00b6\u00e5\u0088\u0086\u00e7\u00a7\u0092\u00e6\u00af\u00ab\u00e7\u00a7\u0092\u00e6\u009e\u0084\u00e9\u0080\u00a0
		:m_date(y,m,d)
		,m_time(hh,mm,ss,ms)
	{

	}
	GDateTime(const string& str)//\u00e4\u00bb\u008e\u00e5\u00ad\u0097\u00e7\u00ac\u00a6\u00e4\u00b8\u00b2\u00e6\u009e\u0084\u00e9\u0080\u00a0
	{
		from_string(str);//\u00e4\u00bb\u008e\u00e5\u00ad\u0097\u00e7\u00ac\u00a6\u00e4\u00b8\u00b2\u00e5\u008f\u0096\u00e6\u0097\u00b6\u00e9\u0097\u00b4
	}
	~GDateTime()//\u00e6\u009e\u0090\u00e6\u009e\u0084
	{

	}
	GDateTime(const GDateTime& other) //\u00e6\u008b\u00b7\u00e8\u00b4\u009d\u00e6\u009e\u0084\u00e9\u0080\u00a0
	{
		m_date = other.m_date;
		m_time = other.m_time;
	}
	GDateTime& operator=(const GDateTime& other) //\u00e8\u00b5\u008b\u00e5\u0080\u00bc
	{
		if(this != &other)
		{
			m_date = other.m_date;
			m_time = other.m_time;
		}
		return *this;
	}
	bool operator<(const GDateTime& other)const //\u00e5\u0088\u00a4\u00e6\u0096\u00ad\u00e6\u0097\u00b6\u00e9\u0097\u00b4\u00e6\u0097\u00a5\u00e6\u009c\u009f\u00e7\u009a\u0084\u00e5\u00a4\u00a7\u00e5\u00b0\u008f
	{
		return (to_string() < other.to_string());
	}
	bool operator==(const GDateTime& other)const//\u00e5\u0088\u00a4\u00e6\u0096\u00ad\u00e6\u0097\u00b6\u00e9\u0097\u00b4\u00e6\u0097\u00a5\u00e6\u009c\u009f\u00e6\u0098\u00af\u00e5\u0090\u00a6\u00e7\u009b\u00b8\u00e7\u00ad\u0089
	{
		return (to_string() == other.to_string());
	}

	GDateTime& operator+=(const GMilliSecond& ms)//\u00e6\u0097\u00b6\u00e9\u0097\u00b4\u00e7\u00b1\u00bb\u00e5\u009e\u008b\u00e5\u008d\u00b3\u00e5\u008f\u00af \u00e5\u008f\u00af\u00e8\u0087\u00aa\u00e5\u008a\u00a8\u00e8\u00bd\u00ac\u00e5\u009e\u008b
	{
		int day = this->m_time.add_msecond(ms);
		m_date += day;
		return *this;
	}
	GDateTime& operator-=(const GMilliSecond& ms)//\u00e6\u0097\u00b6\u00e9\u0097\u00b4\u00e7\u00b1\u00bb\u00e5\u009e\u008b\u00e5\u008d\u00b3\u00e5\u008f\u00af \u00e5\u008f\u00af\u00e8\u0087\u00aa\u00e5\u008a\u00a8\u00e8\u00bd\u00ac\u00e5\u009e\u008b
	{
		int day = this->m_time.add_msecond(-ms);
		m_date += day;
		return *this;
	}

	GDateTime& operator+=(const GDay& day)//\u00e5\u008a\u00a0\u00e5\u0087\u00a0\u00e5\u00a4\u00a9
	{
		m_date += day;
		return *this;
	}
	GDateTime& operator-=(const GDay& day) //\u00e5\u0087\u008f\u00e5\u0087\u00a0\u00e5\u00a4\u00a9
	{
		m_date -= day;
		return *this;
	}

	GDateTime operator-(const GDateTime& datetime)//\u00e4\u00b8\u00a4\u00e4\u00b8\u00aa\u00e6\u0097\u00a5\u00e6\u009c\u009f\u00e7\u009b\u00b8\u00e5\u0087\u008f\u00e5\u00be\u0097\u00e5\u0088\u00b0\u00e5\u00a4\u00a9\u00e6\u0095\u00b0\u00e4\u00b8\u008e\u00e6\u0097\u00b6\u00e9\u0097\u00b4
	{
		GDateTime tmp(*this);
		tmp -= datetime.m_time;
		GDay day = tmp.m_date - datetime.m_date;

		return GDateTime(GDate(0,0,day.m_data),tmp.m_time);
	}

	GDate& date() //\u00e5\u008f\u0096\u00e6\u0097\u00a5\u00e6\u009c\u009f
	{
		return m_date;
	}
	GTime& time_of_day() //\u00e5\u008f\u0096\u00e6\u0097\u00b6\u00e9\u0097\u00b4
	{
		return m_time;
	}
	string to_string() const //\u00e8\u00bd\u00ac\u00e5\u00ad\u0097\u00e7\u00ac\u00a6\u00e4\u00b8\u00b2 \u00e5\u00a6\u008220090617083000
	{
		return (m_date.to_string() + m_time.to_string());
	}
	string to_string_ex() const //\u00e8\u00bd\u00ac\u00e5\u00ad\u0097\u00e7\u00ac\u00a6\u00e4\u00b8\u00b2\u00e5\u008c\u0085\u00e5\u0090\u00ab\u00e5\u0088\u0086\u00e9\u009a\u0094\u00e7\u00ac\u00a6 \u00e5\u00a6\u00822009-06-17 08:30:59
	{
		return (m_date.to_string_ex() + " " + m_time.to_string_ex());
	}
	string to_string_all() const //\u00e8\u00bd\u00ac\u00e5\u00ad\u0097\u00e7\u00ac\u00a6\u00e4\u00b8\u00b2\u00e5\u008c\u0085\u00e5\u0090\u00ab\u00e6\u00af\u00ab\u00e7\u00a7\u0092 \u00e5\u00a6\u008220090617083000000
	{
		return (m_date.to_string() + m_time.to_string_all());
	}
	size_t from_string(const string& str)//\u00e4\u00bb\u008e\u00e5\u00ad\u0097\u00e7\u00ac\u00a6\u00e4\u00b8\u00b2\u00e5\u008f\u0096\u00e6\u0097\u00b6\u00e9\u0097\u00b4
	{
		size_t len = 0;
		len += m_date.from_string(str.substr(len));
		len += m_time.from_string(str.substr(len));
		return len;
	}
	GDateTime& getlocaltime() //\u00e5\u008f\u0096\u00e6\u009c\u00ac\u00e5\u009c\u00b0\u00e6\u0097\u00b6\u00e9\u0097\u00b4 //\u00e5\u008f\u00af\u00e4\u00bb\u00a5\u00e8\u00ae\u00be\u00e8\u00ae\u00a1\u00e6\u0088\u0090\u00e9\u009d\u0099\u00e6\u0080\u0081\u00e5\u0087\u00bd\u00e6\u0095\u00b0
	{
		m_date.getlocaltime();
		m_time.getlocaltime();
		return *this;
	}

//protected:
	GDate m_date;//\u00e6\u0097\u00a5\u00e6\u009c\u009f
	GTime m_time;//\u00e6\u0097\u00b6\u00e9\u0097\u00b4
};


#endif //_GDATETIME_H_
