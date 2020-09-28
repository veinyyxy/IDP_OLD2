#ifndef NETCDFDATAPROVIDER_H
#define NETCDFDATAPROVIDER_H

#include <QObject>
#include "../NetCDF/cxx/netcdfcpp.h"
#include "../CDataElement/CElement.h"
#include "../include/IDataProvider/IDataProvider.h"

class NcFile;
class NcVar;
class NcAtt;
class NcDim;

enum CIPAS_DIM_CATEGORY

{
	CIPAS_LON, 
	CIPAS_LAT,
	CIPAS_LEVEL,
	CIPAS_TIME,
	CIPAS_NOTVALID
};
typedef struct  NcFileStruct  NcFileStruct;
typedef struct  NcFileStruct*  PNcFileStruct;

class  NetCDFDataProvider:  public QObject, public IDataProvider
{
	//Q_OBJECT
	Q_INTERFACES(IDataProvider)

public:
	NetCDFDataProvider();
	~NetCDFDataProvider();
	virtual bool LoadFile(const std::string& filePath);
public:
	//typedef double  FLOAT;
	typedef QPair<int, int> IndexRange;
	typedef QPair<int, int> DayRange;
	typedef QPair<double, double> LevelRange;
	typedef QHash<QString, PNcFileStruct> NcFilesMap;
	typedef QHash<CElement*, CIPAS_DIM_CATEGORY> ElementDimsMap;
	typedef QHash<QString, NcVar*> NcNameVarMap;

	virtual  QStringList keys();

	virtual HashElement GetAllElements(const QString& filePath) const; //��ȡ����Ҫ����Ϣ
	virtual CElement*  GetElement(const QString&  elementName);//��ȡҪ��

	virtual bool StoreDetailValues( CElement* pElement);//�洢���Ҫ����������
	virtual int Write(const QString& fileTypeName);//д�ļ�
	virtual int Write(const QString& fileTypeName, QVector<CElement*> elements_v);//д�ļ�
	virtual bool CloseFileAndRemoveElements(const QString& fileName);
	virtual QString GetDataDescription(const QString& fileName); //����Micaps����������������Ϣ�����򷵻� QString�����մ�
	virtual QStringList GetElementNames(const QString& fileName); ////����Micaps������Ԫ�������б����򷵻� QStringList�������б�
private:
	void ClearVars();
	bool InitElement(CElement* pElement, const NcVar* pVar);
	bool InitLongitudeElement(CElement* pElement, const NcVar* pVar);
	bool InitLatitudeElement(CElement* pElement, const NcVar* pVar);
	bool InitLevelElement(CElement* pElement, const NcVar* pVar);
	bool InitTimeElement(CElement* pElement, const NcVar* pVar);
	bool InitOtherElement(CElement* pElement, const NcVar* pVar);
	QDate TransToDate(int days, QDate baseDate = QDate(1,1,1)); // �ӹ�Ԫ1��1��1�յ�����ת��Ϊ����
	int  TransToDays(const QDate& da, QDate baseDate = QDate(1,1,1));  // ����ת��Ϊ�ӹ�Ԫ1��1��1�յ�����

	//��ʼ��ά��
	bool InitDimensions(const QString &fileName, NcFile* pNcFile);
	//bool GetVariables(PNcFileStruct pFileStruct, NcFile* pNcFile);

	CIPAS_DIM_CATEGORY DimensionCategory(NcVar *pVar);
	CIPAS_DIM_CATEGORY DimensionCategory(const QString& name, const QString& longName, const QString &cartesian_axis);


private:
	HashElement m_elements;
	QString m_prefileName; // ǰһ���򿪵��ļ�����
	NcFile* m_pNcFile;
	CLonLatRange m_lonRange;
	CLonLatRange m_latRange;
	CLevel m_levelRange;
	CPeriod m_dayRange;
	QHash<QString, long> m_fileLoaded; // filename  id
	//NcFilesMap m_NcFileMap; //NETCDF �ļ��������ݽṹ��ӳ���ϵ
	ElementDimsMap m_elementDimMap;
	NcNameVarMap m_nameVarmap; //��ǰ�ļ��е������������Ӧ��ϵ
private:
	void SetDeclaration(const QString& filePath);
};
#endif // NETCDFDATAPROVIDER_H
