#ifndef GRIBDATAPROVIDER_H
#define GRIBDATAPROVIDER_H


#include <gdal.h>
#include "gdal_priv.h"
#include <gdal_rat.h> 
#include "ogr_srs_api.h"
#include "cpl_string.h"
#include "cpl_conv.h"
#include "cpl_multiproc.h"

#include <qdatetime.h>
#include <qtimer.h>
#include <QDate>
#include <QHash>
#include <QList>


#include "..\include\IDataProvider\IDataProvider.h"
#include "../CDataElement/CElement.h"


#define MAX_KEY_LEN  255
#define MAX_VAL_LEN  1024

typedef struct
{
	int year;					//年
	int month_1;					//月
	int	day_1;					//日
	int hour_1;					//时
	int production_centre_id;  //产品制作中心id
	int process_id;            //产品处理id
	int background_process_id; //背景加工过程标识
	int type_field;            //场类型
	int property_time;         //时间属性
	int scope;                 //区域
	int element_code;          //要素代码
	double resolution;	       //分辨率
	int type_layer;            //层次类型
	int layer_one;             //层次1
	int layer_two;             //层次2
	int limitation;            //时效
	int grib_version;          //GRIB编码版本号
	char filename[512];            //原文件名
	char reserved_fields1[100];   //保留字段1
	char reserved_fields2[100];   //保留字段2
	char reserved_fields3[100];   //保留字段3
	int grib_number_latitude;  //纬向格点数
	int grib_number_longitude; //经向格点数
	double first_latitude;     //起始纬度
	double last_latitude;      //终止纬度
	double first_longtitude;   //起始经度
	double last_longitude;     //终止经度
	double incremental_latitude;//纬向增量
	double incremental_longitude;//经向增量
	int resolution_or_component_symbol;//分辨率和分量标志
	int scanning_type;           //扫描方式
	int parameter_indicator;     //参数指示码
	int layer_type_indicator;    //层次类型指示码
	int pressure_layer;          //气压层
	int centennial_year;         //世纪年
	int month_2;				//月
	int	day_2;					//日
	int hour_2;					//时
	int minute;                  //分
	int time_unit_indicator;     //时间单位指示码
	int time_period_one;         //时间周期1
	int time_period_two;         //时间周期2
	int time_range_indicator;    //时间范围指示码
	int participate_time_unit_number; //参与平均或累加的时间单元数
	int nonparticipate_time_unit_number; //未参与平均或累加的时间单元数
	int info_reference_centry;    //资料参照所在的世纪
	int sub_centre_id;            //子中心标识
	int scale_factor_decimal;     //十进制比例因子
	int reserved_fields;         //保留位
}Grib1_INFO;
typedef struct
{
	int year;                  //年
	int month;				  //月
	int	day;                  //日
	int hour;                 //时
	int production_centre_id;  //资料加工中心标识
	int process_id;            //资料加工过程标识
	int background_process_id_1; //背景加工过程标识
	int type_field;            //场类型
	int property_time;         //时间属性
	int scope;                 //区域
	int element_code;          //要素代码
	double resolution;	       //分辨率
	int type_layer;            //层次类型
	int layer_one;             //层次1
	int layer_two;             //层次2
	int limitation;            //时效
	int grib_version;          //GRIB编码版本号
	char filename[512];            //原文件名
	char reserved_fields1[100];   //保留字段1
	char reserved_fields2[100];   //保留字段2
	char reserved_fields3[100];   //保留字段3
	int earth_shape;           //地球形状
	int earth_radius_ratio;    //球面地球半径的标尺比数
	int earth_radius_value;    //球面地球半径的标定值
	int earth_major_axis_ratio;//扁球状地球主轴的标尺比数
	int earth_major_axis_value;//扁球状地球主轴的标定值
	int earth_short_axis_ratio;//扁球状地球短轴的标尺比数
	int earth_short_axis_value;//扁球状地球短轴的标定值
	int grib_number_latitude;  //纬向格点数
	int grib_number_longitude; //经向格点数
	int basic_angle;           //初始产品域的基本角度
	int basic_angle_subdivide; //基本角度的细分
	double first_latitude;     //起始纬度
	double last_latitude;      //终止纬度
	double first_longtitude;   //起始经度
	double last_longitude;     //终止经度
	double incremental_latitude;//纬向增量
	double incremental_longitude;//经向增量
	int latitude_number_between_pole_equator;//极点和赤道之间的纬圈数
	int resolution_or_component_symbol;//分辨率和分量标志
	int scanning_type;           //扫描方式
	int science;                 //学科
	int parameters_kind;        //参数种类
	int parameter_code;         //参数编号
	int type_process;           //加工过程的类型
	int background_process_id_2;  //背景加工过程标识
	int analysis_id;            //分析或预报加工过程标识
	int expire_hour;            //截断小时
	int expire_minute;          //截断分钟
	int time_unit_indicator;//时间范围单位指示码
	int forecast_time;          //预报时间
	int type_first_fixed_plane; //第一固定面的类型
	int first_fixed_plane_ratio;//第一固定面的标尺比数
	int first_fixed_plane_value;//第一固定面的标定值
	int type_second_fixed_plane; //第2固定面的类型
	int second_fixed_plane_ratio;//第2固定面的标尺比数
	int second_fixed_plane_value;//第2固定面的标定值
	int whole_forecast_type;     //整体预报类型
	int variation_number;        //变异数
	int forecast_number;         //预报数
	int reference_value;         //参照值
	int scale_factor_binary;     //2进制比例因子
	int scale_factor_decimal;     //十进制比例因子
	int compression_ratio;       //压缩比特数
	int type_original_field;     //原始场值类型
}Grib2_INFO;

typedef struct grib2_layer
{
	int index  ;              //该层在文件中的索引 
	long offset ;             //偏移量 
	float level;                //层次
	QString var_name;          //变量名		
	QDateTime layer_datetime;
	int grib_number_latitude;      //纬向格点数
	int grib_number_longitude;     //经向格点数
	double first_latitude;         //起始纬度
	double last_latitude;          //终止纬度
	double first_longtitude;        //起始经度
	double last_longitude;          //终止经度
	double incremental_latitude;    //纬向增量
	double incremental_longitude;   //经向增量	

	//int basic_angle;               //初始产品域的基本角度
	//int basic_angle_subdivide;     //基本角度的细分
	//int scanning_type;              //扫描方式
	//int expire_hour;            //截断小时
	//int expire_minute;          //截断分钟
	//int time_unit_indicator;//时间范围单位指示码
	//int forecast_time;          //预报时间
	//int type_first_fixed_plane; //第一固定面的类型
	//int first_fixed_plane_ratio;//第一固定面的标尺比数
	//int first_fixed_plane_value;//第一固定面的标定值
	//int type_second_fixed_plane; //第2固定面的类型
	//int second_fixed_plane_ratio;//第2固定面的标尺比数
	//int second_fixed_plane_value;//第2固定面的标定值
	//int whole_forecast_type;     //整体预报类型

}Grib2_Layer;
class  GribDataProvider : public QObject, public IDataProvider
{
	//Q_OBJECT
	Q_INTERFACES(IDataProvider)
public:
	GribDataProvider();
	~GribDataProvider();

	typedef QPair<int, int> IndexRange;
	typedef QPair<int, int> DayRange;
	typedef QPair<double, double> LevelRange;
	typedef QHash<int ,Grib2_Layer>Grib2_index;
	virtual  QStringList keys();
	virtual bool LoadFile(const std::string& filePath);       //装载数据源文件，初始化各种辅助信息
	virtual HashElement GetAllElements(const QString& filePath) const;            //获取所有要素信息
	virtual CElement*  GetElement(const QString&  filePath);//获取要素
	virtual bool StoreDetailValues( CElement* pElement);   //存储相关要素所有数据
	virtual int Write(const QString& fileTypeName);//写文件
	virtual bool CloseFileAndRemoveElements(const QString& fileName);
private:

	bool InitElement(CElement* pElement, GDALRasterBandH h,Grib2_Layer* layer);
	//bool DataOrdering(long* scan_mode,double *datain,float *dataout,long num_lat,long num_lon,int lon_begin_index,int lat_begin_index);  //依据从西向东，从北向南，按行扫描，相邻行同向排列数据
	bool InitOtherElement(CElement* pElement, GDALRasterBandH h,Grib2_Layer* layer);
	char *tochar(QString value, int &len);  //QString to char*
	QString GetLeveName(QString &firstName,QString &description);
	virtual QString GetDataDescription(const QString& filePath) {return NULL;}; //若是Micaps，返回数据描述信息，否则返回 QString（）空串
	virtual QStringList GetElementNames(const QString& filePath) {return QStringList();}; ////若是Micaps，返回元素名称列表，否则返回 QStringList（）空列表
private:
	HashElement m_elements;
	QString m_prefileName; // 前一个打开的文件名称
	QString m_fileName;
	Grib2_index m_indexHash;
	GDALDriverH		pDriver;
	char              **papszExtraMDDomains, **papszFileList;
	char		      *pszProjection;
private:
	void SetDeclaration(const QString& filePath);

};

#endif // GRIBDATAPROVIDER_H
