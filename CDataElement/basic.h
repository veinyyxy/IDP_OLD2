#pragma once
/*============================================================ 
��Ŀ���� �˻�����ƽ̨
�� �ߣ� ����
�� λ�� �й�����뼼������ɷ����޹�˾
����޸�ʱ�䣺 2011��5��17��
��Ȩ������ ��Ȩ����2010-2018 ����������ҵ��
��������Ȩ��
Copyright (c) 2010-2018  CHINA NATIONAL SOFTWARE &SERVICE CO.,LTD.
All rights reserved 
��Ҫ������ 1. ���ʹ�÷�ΧΪ������ҵ������������Ա��δ����ɲ������Դ�����
           2. ���ʹ���߲����޸ı��ļ����ݣ�����ᵼ�����ʹ�ô���
------------------------------------------------------------------------------------- 
�ļ�����basic.h
�� ���� ��
�� �ࣺ ��
�� �ࣺ �� 
����˵���� ����ö�����Ͷ���
����˵���� ��
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
//*  ProductType �����Ʒ ���ͼ����� */
//* (0) �ı����������ͼ */
//* (1)2D��������ֵ��	(2)2D������ɫ��ͼ						*/
//* (3)2D˫����ʸ��ͼ	(4)2D˫��������ͼ	(5)2D˫��������ͼ	*/
//* (6)MICAPS3վ��		(7)MICPAS3��ֵ��	(8)MICPAS3ɫ��ͼ	*/
//* (9)MICAPS1վ��		(10)MICPAS2վ��		(11)MICPAS4���		*/
//* (12)ʱ������ͼ		(13)�ռ�����ͼ		                    */
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
