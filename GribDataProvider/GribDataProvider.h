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
	int year;					//��
	int month_1;					//��
	int	day_1;					//��
	int hour_1;					//ʱ
	int production_centre_id;  //��Ʒ��������id
	int process_id;            //��Ʒ����id
	int background_process_id; //�����ӹ����̱�ʶ
	int type_field;            //������
	int property_time;         //ʱ������
	int scope;                 //����
	int element_code;          //Ҫ�ش���
	double resolution;	       //�ֱ���
	int type_layer;            //�������
	int layer_one;             //���1
	int layer_two;             //���2
	int limitation;            //ʱЧ
	int grib_version;          //GRIB����汾��
	char filename[512];            //ԭ�ļ���
	char reserved_fields1[100];   //�����ֶ�1
	char reserved_fields2[100];   //�����ֶ�2
	char reserved_fields3[100];   //�����ֶ�3
	int grib_number_latitude;  //γ������
	int grib_number_longitude; //��������
	double first_latitude;     //��ʼγ��
	double last_latitude;      //��ֹγ��
	double first_longtitude;   //��ʼ����
	double last_longitude;     //��ֹ����
	double incremental_latitude;//γ������
	double incremental_longitude;//��������
	int resolution_or_component_symbol;//�ֱ��ʺͷ�����־
	int scanning_type;           //ɨ�跽ʽ
	int parameter_indicator;     //����ָʾ��
	int layer_type_indicator;    //�������ָʾ��
	int pressure_layer;          //��ѹ��
	int centennial_year;         //������
	int month_2;				//��
	int	day_2;					//��
	int hour_2;					//ʱ
	int minute;                  //��
	int time_unit_indicator;     //ʱ�䵥λָʾ��
	int time_period_one;         //ʱ������1
	int time_period_two;         //ʱ������2
	int time_range_indicator;    //ʱ�䷶Χָʾ��
	int participate_time_unit_number; //����ƽ�����ۼӵ�ʱ�䵥Ԫ��
	int nonparticipate_time_unit_number; //δ����ƽ�����ۼӵ�ʱ�䵥Ԫ��
	int info_reference_centry;    //���ϲ������ڵ�����
	int sub_centre_id;            //�����ı�ʶ
	int scale_factor_decimal;     //ʮ���Ʊ�������
	int reserved_fields;         //����λ
}Grib1_INFO;
typedef struct
{
	int year;                  //��
	int month;				  //��
	int	day;                  //��
	int hour;                 //ʱ
	int production_centre_id;  //���ϼӹ����ı�ʶ
	int process_id;            //���ϼӹ����̱�ʶ
	int background_process_id_1; //�����ӹ����̱�ʶ
	int type_field;            //������
	int property_time;         //ʱ������
	int scope;                 //����
	int element_code;          //Ҫ�ش���
	double resolution;	       //�ֱ���
	int type_layer;            //�������
	int layer_one;             //���1
	int layer_two;             //���2
	int limitation;            //ʱЧ
	int grib_version;          //GRIB����汾��
	char filename[512];            //ԭ�ļ���
	char reserved_fields1[100];   //�����ֶ�1
	char reserved_fields2[100];   //�����ֶ�2
	char reserved_fields3[100];   //�����ֶ�3
	int earth_shape;           //������״
	int earth_radius_ratio;    //�������뾶�ı�߱���
	int earth_radius_value;    //�������뾶�ı궨ֵ
	int earth_major_axis_ratio;//����״��������ı�߱���
	int earth_major_axis_value;//����״��������ı궨ֵ
	int earth_short_axis_ratio;//����״�������ı�߱���
	int earth_short_axis_value;//����״�������ı궨ֵ
	int grib_number_latitude;  //γ������
	int grib_number_longitude; //��������
	int basic_angle;           //��ʼ��Ʒ��Ļ����Ƕ�
	int basic_angle_subdivide; //�����Ƕȵ�ϸ��
	double first_latitude;     //��ʼγ��
	double last_latitude;      //��ֹγ��
	double first_longtitude;   //��ʼ����
	double last_longitude;     //��ֹ����
	double incremental_latitude;//γ������
	double incremental_longitude;//��������
	int latitude_number_between_pole_equator;//����ͳ��֮���γȦ��
	int resolution_or_component_symbol;//�ֱ��ʺͷ�����־
	int scanning_type;           //ɨ�跽ʽ
	int science;                 //ѧ��
	int parameters_kind;        //��������
	int parameter_code;         //�������
	int type_process;           //�ӹ����̵�����
	int background_process_id_2;  //�����ӹ����̱�ʶ
	int analysis_id;            //������Ԥ���ӹ����̱�ʶ
	int expire_hour;            //�ض�Сʱ
	int expire_minute;          //�ضϷ���
	int time_unit_indicator;//ʱ�䷶Χ��λָʾ��
	int forecast_time;          //Ԥ��ʱ��
	int type_first_fixed_plane; //��һ�̶��������
	int first_fixed_plane_ratio;//��һ�̶���ı�߱���
	int first_fixed_plane_value;//��һ�̶���ı궨ֵ
	int type_second_fixed_plane; //��2�̶��������
	int second_fixed_plane_ratio;//��2�̶���ı�߱���
	int second_fixed_plane_value;//��2�̶���ı궨ֵ
	int whole_forecast_type;     //����Ԥ������
	int variation_number;        //������
	int forecast_number;         //Ԥ����
	int reference_value;         //����ֵ
	int scale_factor_binary;     //2���Ʊ�������
	int scale_factor_decimal;     //ʮ���Ʊ�������
	int compression_ratio;       //ѹ��������
	int type_original_field;     //ԭʼ��ֵ����
}Grib2_INFO;

typedef struct grib2_layer
{
	int index  ;              //�ò����ļ��е����� 
	long offset ;             //ƫ���� 
	float level;                //���
	QString var_name;          //������		
	QDateTime layer_datetime;
	int grib_number_latitude;      //γ������
	int grib_number_longitude;     //��������
	double first_latitude;         //��ʼγ��
	double last_latitude;          //��ֹγ��
	double first_longtitude;        //��ʼ����
	double last_longitude;          //��ֹ����
	double incremental_latitude;    //γ������
	double incremental_longitude;   //��������	

	//int basic_angle;               //��ʼ��Ʒ��Ļ����Ƕ�
	//int basic_angle_subdivide;     //�����Ƕȵ�ϸ��
	//int scanning_type;              //ɨ�跽ʽ
	//int expire_hour;            //�ض�Сʱ
	//int expire_minute;          //�ضϷ���
	//int time_unit_indicator;//ʱ�䷶Χ��λָʾ��
	//int forecast_time;          //Ԥ��ʱ��
	//int type_first_fixed_plane; //��һ�̶��������
	//int first_fixed_plane_ratio;//��һ�̶���ı�߱���
	//int first_fixed_plane_value;//��һ�̶���ı궨ֵ
	//int type_second_fixed_plane; //��2�̶��������
	//int second_fixed_plane_ratio;//��2�̶���ı�߱���
	//int second_fixed_plane_value;//��2�̶���ı궨ֵ
	//int whole_forecast_type;     //����Ԥ������

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
	virtual bool LoadFile(const std::string& filePath);       //װ������Դ�ļ�����ʼ�����ָ�����Ϣ
	virtual HashElement GetAllElements(const QString& filePath) const;            //��ȡ����Ҫ����Ϣ
	virtual CElement*  GetElement(const QString&  filePath);//��ȡҪ��
	virtual bool StoreDetailValues( CElement* pElement);   //�洢���Ҫ����������
	virtual int Write(const QString& fileTypeName);//д�ļ�
	virtual bool CloseFileAndRemoveElements(const QString& fileName);
private:

	bool InitElement(CElement* pElement, GDALRasterBandH h,Grib2_Layer* layer);
	//bool DataOrdering(long* scan_mode,double *datain,float *dataout,long num_lat,long num_lon,int lon_begin_index,int lat_begin_index);  //���ݴ����򶫣��ӱ����ϣ�����ɨ�裬������ͬ����������
	bool InitOtherElement(CElement* pElement, GDALRasterBandH h,Grib2_Layer* layer);
	char *tochar(QString value, int &len);  //QString to char*
	QString GetLeveName(QString &firstName,QString &description);
	virtual QString GetDataDescription(const QString& filePath) {return NULL;}; //����Micaps����������������Ϣ�����򷵻� QString�����մ�
	virtual QStringList GetElementNames(const QString& filePath) {return QStringList();}; ////����Micaps������Ԫ�������б����򷵻� QStringList�������б�
private:
	HashElement m_elements;
	QString m_prefileName; // ǰһ���򿪵��ļ�����
	QString m_fileName;
	Grib2_index m_indexHash;
	GDALDriverH		pDriver;
	char              **papszExtraMDDomains, **papszFileList;
	char		      *pszProjection;
private:
	void SetDeclaration(const QString& filePath);

};

#endif // GRIBDATAPROVIDER_H
