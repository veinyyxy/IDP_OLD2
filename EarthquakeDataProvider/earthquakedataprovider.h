/** @file    earthquakedataprovider.h   earthquakedataprovider.cpp、
* @brief     雷达数据提供器对外接口类文件
* @author    陈志学
* @date      2013年8月4
* @version   1.0
  @note      2013年8月14 杨晓宇
*/ 
#ifndef EARTHQUAKEDATAPROVIDER_H
#define EARTHQUAKEDATAPROVIDER_H

#include "earthquakedataprovider_global.h"
#include "DataProvider.h"
#include <QStringList>
#include <QtCore/QFile>
#include "Layer.h"
#include "EarthquakeProductionDraw.h"
using namespace goto_gis ;

/** @class  EarthquakeDataProvider
    @brief  地震数据提供器
    @author  陈志学
	@note    基类DataProvider，地震数据提供器，外部调用接口
	*/ 
class EARTHQUAKEDATAPROVIDER_EXPORT EarthquakeDataProvider :public DataProvider
{

public:
	EarthquakeDataProvider();
	~EarthquakeDataProvider();
	/** @brief 加载数据 */ 
	virtual bool LoadFile(const std::string& filename);
	void AreaAdd(bool tArea,bool tPie,float tMin,float tMax,osg::Vec3Array* p) {}
	virtual QString GetDataUnit(){return "";};//获得数据计量单位
	virtual float GetPositionValue(double lon,double lat,double height){return 0.0;};//获取鼠标位置的数据数值
	//virtual bool LoadFile(const std::list<std::string>&filenameList);
private: 
	/** @brief 绘制数据 */
	void DumpProduct(Layer *pLayer);
	/** @brief 获取特定的产品绘图类指针，基类指针 */
	EQProductionDraw* GetProductDrawInstance(EARTHQUAKE_DATA_TYPE nProductCode);
	/** @brief 释放产品绘图类指针，基类指针 */
	void ReleaseProductDrawInstance(EQProductionDraw* lpInstance);
	/** @brief 获得文件类型 */
	EARTHQUAKE_DATA_TYPE GetFileType(QFile &f);
private:
	QStringList    m_fileNameList;//多个文件
	QString m_fileName;//单个文件
	EQProductionDraw * pProductDraw;

};

#endif // EARTHQUAKEDATAPROVIDER_H
