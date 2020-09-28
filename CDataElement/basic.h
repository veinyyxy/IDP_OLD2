#pragma once
/*============================================================ 
项目名： 人机交互平台
作 者： 韩猛
单 位： 中国软件与技术服务股份有限公司
最后修改时间： 2011年5月17日
版权声明： 版权所有2010-2018 中软气象事业部
保留所有权利
Copyright (c) 2010-2018  CHINA NATIONAL SOFTWARE &SERVICE CO.,LTD.
All rights reserved 
重要声明： 1. 类库使用范围为气象事业部软件开发组成员，未经许可不得擅自传播。
           2. 类库使用者不得修改本文件内容，否则会导致类库使用错误。
------------------------------------------------------------------------------------- 
文件名：basic.h
类 名： 无
父 类： 无
子 类： 无 
功能说明： 基本枚举类型定义
调用说明： 无
============================================================*/
#ifndef  PRODUCT_TYPE
#define PRODUCT_TYPE
#include <QString>
class CElement;
struct ProductType
{
	CElement *pData1;
	CElement *pData2;
	int product;
	QString fileName;
};

typedef struct ProductType  ProductType;
#endif // PRODUCT_TYPE

#ifndef DATA_SOURCE
#define DATA_SOURCE
//*  ProductType 输出产品 类型及数据 */
//* (0) 文本输出，不绘图 */
//* (1)2D单变量等值线	(2)2D单变量色斑图						*/
//* (3)2D双变量矢量图	(4)2D双变量风羽图	(5)2D双变量流线图	*/
//* (6)MICAPS3站点		(7)MICPAS3等值线	(8)MICPAS3色斑图	*/
//* (9)MICAPS1站点		(10)MICPAS2站点		(11)MICPAS4格点		*/
//* (12)时间剖面图		(13)空间剖面图		                    */
enum ENUM_DATA_SOURCE
{
	enum_database,
	enum_localfile,
	enum_memory
};
#endif //DATA_SOURCE

#ifndef DATA_ORDER
#define DATA_ORDER
enum Enum_DataOrder
{
	TZXY = 1,
	TXYZ = 2,
	XYZT = 3,
	XYTZ = 4,
	ZTXY = 5,
	ZXYT = 6,
};
#endif  // DATA_ORDER
