/**************************************************************************************/
/*项目名： 人机交互平台                                                               */
/*作 者： 王佳梁                                                                      */
/*单 位： 中国软件与技术服务股份有限公司                                              */
/*最后修改时间： 2011年？月？日                                                       */
/*版权声明： 版权所有2010-2018 中软气象事业部保留所有权利                             */
/*Copyright (c) 2010-2018  CHINA NATIONAL SOFTWARE &SERVICE CO.,LTD.All rights reserved */
/*重要声明： 1. 类库使用范围为气象事业部软件开发组成员，未经许可不得擅自传播。        */
/*           2. 类库使用者不得修改本文件内容，否则会导致类库使用错误。                */
/**************************************************************************************/
/*文件名：GeneralBarb.h GeneralBarb.cpp                                 */
/*类 名： GeneralBarb                                                   */
/*父 类： MeteorologyProducts                                           */
/*子 类： 无                                                            */
/*功能说明：产品算法生成                                                */
/*调用说明：                                                            */
/************************************************************************/
/*V 1.0                                                                 */
/*原作者 ：WANGJL                                                       */
/*完成日期：2011年6月22日                                               */
/************************************************************************/
#pragma once
#include "meteorologyproducts.h"

#include "gxX.h"
#include "gauser.h"

class GeneralVector : public MeteorologyProducts
{
public:
	GeneralVector(void);
	~GeneralVector(void);

protected:
	void InitCommand();
	void ScaleDataLine();

	char m_Command[256];			//脚本命令

	bool m_flag4ClearVolume;		//是否清除卷的标示
	QString m_strArriChanged;		//改变的属性的记录

	QColor m_cBitColor;

	//CDivisionData m_divObject;							//用于切割对象
};

