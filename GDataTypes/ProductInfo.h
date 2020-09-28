#include "vector"
#include "string"

using namespace std;
typedef vector<int> vec_int;
typedef vector<float> vec_float;
#define M_PI 3.1415926535897932384626433832795
#include "GDef.h"
//template class DLLEXP_GSYS std::basic_string<char, char_traits<char>, allocator<char> >;
////DLLEXP_GSYS std::basic_string<char, char_traits<char>, allocator<char> >::_Alval;
//template class DLLEXP_GSYS std::_String_val<char, allocator<char> >;
//template class DLLEXP_GSYS std::allocator<char>;

class DLLEXP_GSYS proAttr	//产品属性信息
{
public:
	proAttr();
	~proAttr();
public:
	int    NumberOfRadars;          //组网站点数    组网产品才有
	int		 pro_code;				//产品号
	char site_code[255];				//站号
	char staName[255];				//站名
	
	char pro_name[255];				//产品名英文缩写
	int		 scan_date;				//产品体扫开始日期
	int		 scan_time;				//产品体扫开始时间
	int		 generation_date;		//生成日期
	int		 generation_time;		//生成时间
	int		 altitude;				//海拔高度
	int		 longitude;				//经度
	int		 longitude2;			//终止经度  只有组网产品才有
	int		 latitude;				//纬度
	int		 latitude2;			 //终止纬度  只有组网产品才有
	int		 operation_mode;		//工作模式
	int		 vcp_mode;				//vcp mode
	double	 elevation;				//仰角
	double	 max_value;				//最大值
	double	 max_value2;			 //EVP、VWP最大值2
	double	 max_value3;			//EVP、VWP最大值3
	double	 width;					//雷达扫描半径
	int		 HeightOfElevation;		//本层数据的高度或仰角度数，根据产品区分
	int		 numberoflayers;		//总层数
	int		 param1;				//速度退模糊参数 default:50
	int		 param2;				//速度退模糊参数 default:0
	float	 height_evp_vwp;		//高度  只针对EVP VWP有效
	float	 Uh_evp_vwp;			//风速  只针对EVP VWP有效
	float    Du_evp_vwp;			//风向	只针对EVP VWP有效
	int      ResolutionOfRow;   // 只有组网产品才有
	int      ResolutionOfCol;  // 只有组网产品才有
};