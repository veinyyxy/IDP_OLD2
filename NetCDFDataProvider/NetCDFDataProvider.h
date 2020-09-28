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

	virtual HashElement GetAllElements(const QString& filePath) const; //获取所有要素信息
	virtual CElement*  GetElement(const QString&  elementName);//获取要素

	virtual bool StoreDetailValues( CElement* pElement);//存储相关要素所有数据
	virtual int Write(const QString& fileTypeName);//写文件
	virtual int Write(const QString& fileTypeName, QVector<CElement*> elements_v);//写文件
	virtual bool CloseFileAndRemoveElements(const QString& fileName);
	virtual QString GetDataDescription(const QString& fileName); //若是Micaps，返回数据描述信息，否则返回 QString（）空串
	virtual QStringList GetElementNames(const QString& fileName); ////若是Micaps，返回元素名称列表，否则返回 QStringList（）空列表
private:
	void ClearVars();
	bool InitElement(CElement* pElement, const NcVar* pVar);
	bool InitLongitudeElement(CElement* pElement, const NcVar* pVar);
	bool InitLatitudeElement(CElement* pElement, const NcVar* pVar);
	bool InitLevelElement(CElement* pElement, const NcVar* pVar);
	bool InitTimeElement(CElement* pElement, const NcVar* pVar);
	bool InitOtherElement(CElement* pElement, const NcVar* pVar);
	QDate TransToDate(int days, QDate baseDate = QDate(1,1,1)); // 从公元1年1月1日的天数转换为日期
	int  TransToDays(const QDate& da, QDate baseDate = QDate(1,1,1));  // 日期转换为从公元1年1月1日的天数

	//初始化维度
	bool InitDimensions(const QString &fileName, NcFile* pNcFile);
	//bool GetVariables(PNcFileStruct pFileStruct, NcFile* pNcFile);

	CIPAS_DIM_CATEGORY DimensionCategory(NcVar *pVar);
	CIPAS_DIM_CATEGORY DimensionCategory(const QString& name, const QString& longName, const QString &cartesian_axis);


private:
	HashElement m_elements;
	QString m_prefileName; // 前一个打开的文件名称
	NcFile* m_pNcFile;
	CLonLatRange m_lonRange;
	CLonLatRange m_latRange;
	CLevel m_levelRange;
	CPeriod m_dayRange;
	QHash<QString, long> m_fileLoaded; // filename  id
	//NcFilesMap m_NcFileMap; //NETCDF 文件名与数据结构的映射关系
	ElementDimsMap m_elementDimMap;
	NcNameVarMap m_nameVarmap; //当前文件中的名称与变量对应关系
private:
	void SetDeclaration(const QString& filePath);
};
#endif // NETCDFDATAPROVIDER_H
