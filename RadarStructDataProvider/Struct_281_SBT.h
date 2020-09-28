#ifndef STRUCT_279_RCT_H
#define STRUCT_279_RCT_H

//#include "GDef.h"

//����ͷ�Ľṹ�壻
typedef struct BoundHead
{
	gUint32 len; //�������ݵĳ��ȣ����ݵ�ĸ�����
	gInt16 lastIndex;	// ������0ӳ��һ����ɨ��ƥ������������
	gInt16 curIndex;   // �����ڵ�ǰ��ɨ�е����������
	gInt32 cen_lon;	// ��������  �Ŵ�10000
	gInt32 cen_lat;	// ��������  �Ŵ�10000
	gFloat azi;	// �������ķ�λ( ��  )
	gFloat area;	// �������( �� )
	gUint32 OffsetOfPoint;
}BoundHead;

//һ��������ͷ��Ϣ��
typedef struct _BoundFileHeader
{
	gUint16 cntBound;	// ��������
	gUint16	VolumeScanDate;		//ɨ������
	gInt32	VolumeScanStartTime; //ɨ��ʱ��
	gInt16 Width;	//ͼƬ��ȣ���λ������
	gInt16 Height;	//ͼƬ�߶ȣ���λ������
	gInt16 ikuchang;//�ⳤ����λ����
}tagBoundFileHeader;

//��һ��������ɵ�һ��ͼ�Ľṹ�壻
//typedef struct _OutBound
//{
//	tagBoundHeader head;  //����ͷ
//	gUint32 X[MAX_POINTS];		//������X����
//	gUint32 Y[MAX_POINTS];		//������Yγ��
//}OutBound;


typedef struct l_Point
{
	gInt32 lon;		//������X���� �Ŵ�10000
	gInt32 lat;		//������Yγ�� �Ŵ�10000
}l_Point;

#endif
