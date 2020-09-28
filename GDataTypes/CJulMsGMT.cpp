 /////////////////////////////////////////////////////// 
 // CJulMsGMT.cpp 
 // created by chenxiang 20080407
 // forʱ�����ڴ���
 /////////////////////////////////////////////////////// 

#include "CJulMsGMT.h"
#include  <time.h> 
#include  <string.h>

bool CJulMsGMT::Jul2Greg( gUint16 JulDay,gInt32& Year,gInt32& Month,gInt32& Day ) 
	{
		 // ת��Julian����Ϊ�������ڣ�Gregorian��
		 // ���������
		if( JulDay < 1 || JulDay > 65533 )			//MS by chenxiang for 1to19700101 20080428
		{
			return false;							// ���������С��������65533�򷵻�false
		}
		gInt32 t1 = gInt32( JulDay ) + 2509157 -1;	//MS by chenxiang for 1to19700101 20080428
		gInt32 t2 = 4 * t1 / 146097;
		t1 -= ( 146097 * t2 + 3 ) / 4;
		gInt32 yy = 4000 * ( t1 + 1 ) / 1461001;
		t1 = t1 - 1461 * yy / 4 + 31;
		gInt32 mm = 80 * t1 / 2447;
		Day = gInt32( t1 - 2447 * mm / 80 );
		t1 = mm / 11;
		Month = gInt32( mm + 2 - 12 * t1 );
		Year = gInt32( 100 * ( t2 - 49 ) + yy + t1 );
		return true;
	}

gUint16 CJulMsGMT::Greg2Jul( gInt32 Year,gInt32 Month,gInt32 Day ) 
	{
		 // ת���������ڣ�Gregorian��Ϊ������������( Julian )   // �˺���ͬʱ�������ڼ�鹦�� ������ڲ��Ϸ���2��30���򷵻�ֵΪ65535 // ����Jul2Greg()
		 // �������
		if( Year < 1970 || Year > 2148 )  
		{
			return 65535;						// ���С��1970�����2148�򷵻�65535
		}
		if( Month < 1 || Month > 12 )  
		{
			return 65535;						// �·ݲ���1��12��֮���򷵻�65535
		}
		if( Day < 0 || Day > 31 )  
		{
			return 65535;						// ���ڲ�������Χ�������ȸ����ķ�Χ ���滹���ж�
		}
		gInt32 y = Year;						// �򻯹�ʽ ��ͬ
		gInt32 m = Month;
		gInt32 d = Day;	
		gUint16 JulDay = gUint16 ( d + 1461 *( y + ( m - 14 ) / 12 ) / 4
						   + 367 *(m - 2 - ( m - 14 ) / 12 * 12 ) / 12
						   - 3 *(( y + 100 + ( m - 14 ) / 12 ) / 100 ) / 4 - 719499 + 1 ); // ����ת������//MS by chenxiang for 19700101to1 and clear up warning 20080428
		gInt32 y1 = y;							// ������ʱ������ ���ڱȽ�
		gInt32 m1 = m;
		gInt32 d1 = d;
		if( !Jul2Greg( JulDay,y1,m1,d1 ) )		// �ѵõ���������ת���������Ƚ���,����������Ƿ�Ϸ�
		{
			return 65535;
		}
		if( y1 != y || m1 != m || d1 != d )		// ���ת���������ڲ�����ԭ��������˵�����ڲ��Ϸ���4��31�ջ���5��1��˵������4��31�ղ��Ϸ�������65535
		{
			return 65535;
		}
		return JulDay;
	 }

gInt32 CJulMsGMT::Time2MilliSecond( gInt32 T_Hour,gInt32 T_Minute,gInt32 T_Second,gInt32 T_MilliSecond ) 
	{
		 // ʱ��ת����  // �˺���ͬʱ����ʱ���鹦�� ���ʱ�䲻�Ϸ���25��61���򷵻�ֵΪ - 1
		 // ���ʱ���Ƿ�Ϸ�
		if( T_Hour < 0 || T_Hour >= 24 )  
		{
			return -1;
		}
		if( T_Minute < 0 || T_Minute >= 60 )  
		{
			return -1;
		}
		if( T_Second < 0 || T_Second >= 60 ) 
		{
			return -1;
		}
		if( T_MilliSecond < 0 || T_MilliSecond >= 1000 )  
		{
			return -1;
		}
		return T_Hour * 3600 * 1000 + T_Minute * 60 * 1000 + T_Second * 1000 + T_MilliSecond;
	}

bool CJulMsGMT::MilliSecond2Time( gInt32 MilliSecond,gInt32& T_Hour,gInt32& T_Minute,gInt32& T_Second,gInt32& T_MilliSecond ) 
	{
		 // ����תʱ��
		if( MilliSecond < 0 || MilliSecond >= ( 24 * 3600 * 1000 ) ) 
		{
			return false;						// ����ʱ�䷶Χ
		}
		T_Hour = gInt32( MilliSecond / 3600 / 1000 );
		MilliSecond -= T_Hour * 3600 * 1000;
		T_Minute = gInt32( MilliSecond / 60 / 1000 );
		MilliSecond -= T_Minute * 60 * 1000;
		T_Second = gInt32( MilliSecond / 1000 );
		T_MilliSecond = gInt32( MilliSecond - T_Second * 1000 );	
		return true;
	}

bool CJulMsGMT::BeijingTime2GMT( gInt32& Year,gInt32& Month,gInt32& Day,gInt32& T_Hour ) 
	{
		 // ����ʱ��ת����ʱ��  // ����Jul2Greg()��Greg2Jul()
		if( T_Hour < 0 || T_Hour >= 24 )  
		{
			return false;						// ���Сʱ�Ƿ�Ϸ�
		}
		if( Greg2Jul( Year,Month,Day ) == 65535 )  
		{
			return false;						// ��Greg2Jul��������������Ƿ�Ϸ�
		}
		if( T_Hour >= 8 ) 
		{
			T_Hour -= 8;						// Сʱֱ�Ӽ�8
		}
		else
		{
			if( gInt32( Greg2Jul( Year,Month,Day ) ) - 1 < 0 ) 
			{
				return false;					 // �������ʱ��תΪ����ʱ������������1970��1��1���򷵻�false
			}
			Jul2Greg( Greg2Jul( Year,Month,Day ) - 1,Year,Month,Day ); // ��������һ �Ȱ�����תΪ�������ټ�һ�ٰѼ�һ���������ת������
			T_Hour = T_Hour + 24 - 8;			// Сʱ��24��8
		}
		return true;
	}

bool CJulMsGMT::GMT2BeijingTime( gInt32& Year,gInt32& Month,gInt32& Day,gInt32& T_Hour ) 
	{
		 // ����ʱ��ת����ʱ��  // ����Jul2Greg() ��Greg2Jul() 
		if( T_Hour < 0 || T_Hour >= 24 )  
		{
			return false;						// ���Сʱ�Ƿ�Ϸ�
		}
		if( Greg2Jul( Year,Month,Day ) == 65535 )  
		{
			return false;						// ��Greg2Jul��������������Ƿ�Ϸ�
		}
		if( T_Hour < 16 ) 
		{
			T_Hour += 8;						// Сʱֱ�Ӽ�8
		}
		else
		{
			T_Hour = T_Hour + 8 - 24;			// Сʱ��8��24
			Jul2Greg( Greg2Jul( Year,Month,Day ) + 1, Year, Month, Day ); // ��������һ �Ȱ�����תΪ�������ټ�һ�ٰѼ�һ���������ת������ // �����ռ�һ���ᳬ��65535
		}
		return true;
	}

bool CJulMsGMT::JulMsBJT2GMT( gUint16& JulDay,gInt32& MilliSecond ) 
	{
		 // ���������պ���ת���������պ���
		if( JulDay < 1 || JulDay > 65533 ) 
		{
			return false;						// ������С��������65533 // �޷��Ų���С��0Ϊ�˱������
		}
		if( MilliSecond < 0 || MilliSecond >= 24 * 3600 * 1000 ) 
		{
			return false;						// ���뷶Χ����
		}
		if( MilliSecond >= 8 * 3600 * 1000 ) 
		{
			MilliSecond -= 8 * 3600 * 1000;		// ���ڰ�Сʱֱ�Ӽ���Сʱ��Ӧ�ĺ���
		}
		else
		{
			--JulDay;
			if( JulDay < 1 || JulDay > 65533 ) 
			{
				return false;					// ת�������ʱ��������С��1�����65533�򷵻�false
			}
			MilliSecond += 16 * 3600 * 1000;	// Сʱ��24��8���ڼ�16Сʱ��Ӧ�ĺ�����
		}	
		return true;
	}
bool CJulMsGMT::JulMsGMT2BJT( gUint16& JulDay,gInt32& MilliSecond ) 
	{
		 // ���������պ���ת���������պ���
		if( JulDay < 1 || JulDay > 65533 ) 
		{
			return false;						// ������С��1�����65533
		}
		if( MilliSecond < 0 || MilliSecond >= 24 * 3600 * 1000 ) 
		{
			return false;						// ���뷶Χ����
		}
		if( MilliSecond < 16 * 3600 * 1000 ) 
		{
			MilliSecond += 8 * 3600 * 1000;		// С�ڰ�Сʱֱ�ӼӰ�Сʱ��Ӧ�ĺ���
		}
		else
		{
			JulDay++;							// ��������һ // �����Ѿ����� ���Բ��ᳬ��65535
			MilliSecond -= 16 * 3600 * 1000;	// Сʱ��8��24���ڼ�16Сʱ��Ӧ�ĺ�����
		}	
		return true;
	}
bool CJulMsGMT::GetLocalJulMs( gUint16& JulDay,gInt32& MilliSecond ) 
{
		// ��ȡ��ǰ�����պ��뺯�� ����"��" û�о�ȷ������ ������� // ����Greg2Jul() ��Time2Millisecond() 
		time_t NowTime;
        struct tm TimeStruct;
        memset( &TimeStruct,0,sizeof( struct tm ) ); // ��tmstrTmp�ṹ��������ȫ��'0'
        NowTime = time( NULL );

		#if defined ( WIN32 ) || defined ( WIN64 )
			localtime_s( &TimeStruct,&NowTime ); // ����ȡ������ʱ�����Ϣ����TimeStruct��(�������������)
		#else
			TimeStruct = *localtime( &NowTime );
		#endif

		JulDay = Greg2Jul( TimeStruct.tm_year + 1900, TimeStruct.tm_mon + 1, TimeStruct.tm_mday ); // ����ȡ���ĵ�ǰ������תΪ������
		MilliSecond = Time2MilliSecond( TimeStruct.tm_hour,TimeStruct.tm_min,TimeStruct.tm_sec,0 ); // ����ȡ���ĵ�ǰʱ��תΪ�����ʱ
		return true;
}
