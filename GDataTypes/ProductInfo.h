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

class DLLEXP_GSYS proAttr	//��Ʒ������Ϣ
{
public:
	proAttr();
	~proAttr();
public:
	int    NumberOfRadars;          //����վ����    ������Ʒ����
	int		 pro_code;				//��Ʒ��
	char site_code[255];				//վ��
	char staName[255];				//վ��
	
	char pro_name[255];				//��Ʒ��Ӣ����д
	int		 scan_date;				//��Ʒ��ɨ��ʼ����
	int		 scan_time;				//��Ʒ��ɨ��ʼʱ��
	int		 generation_date;		//��������
	int		 generation_time;		//����ʱ��
	int		 altitude;				//���θ߶�
	int		 longitude;				//����
	int		 longitude2;			//��ֹ����  ֻ��������Ʒ����
	int		 latitude;				//γ��
	int		 latitude2;			 //��ֹγ��  ֻ��������Ʒ����
	int		 operation_mode;		//����ģʽ
	int		 vcp_mode;				//vcp mode
	double	 elevation;				//����
	double	 max_value;				//���ֵ
	double	 max_value2;			 //EVP��VWP���ֵ2
	double	 max_value3;			//EVP��VWP���ֵ3
	double	 width;					//�״�ɨ��뾶
	int		 HeightOfElevation;		//�������ݵĸ߶Ȼ����Ƕ��������ݲ�Ʒ����
	int		 numberoflayers;		//�ܲ���
	int		 param1;				//�ٶ���ģ������ default:50
	int		 param2;				//�ٶ���ģ������ default:0
	float	 height_evp_vwp;		//�߶�  ֻ���EVP VWP��Ч
	float	 Uh_evp_vwp;			//����  ֻ���EVP VWP��Ч
	float    Du_evp_vwp;			//����	ֻ���EVP VWP��Ч
	int      ResolutionOfRow;   // ֻ��������Ʒ����
	int      ResolutionOfCol;  // ֻ��������Ʒ����
};