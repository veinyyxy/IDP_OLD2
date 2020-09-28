#ifndef STRUCT_516_MSBT_H
#define STRUCT_516_MSBT_H

#include "..\GDataTypes\GDef.h"
#define MAX_POINTS 3000
//����ͷ�Ľṹ�壻
typedef struct _BoundHeader
{
	gUint32 len; //�������ݵĳ��ȣ����ݵ�ĸ�����
	gInt16 lastIndex;	// ������0ӳ��һ����ɨ��ƥ������������
	gInt16 curIndex;   // �����ڵ�ǰ��ɨ�е����������
	gInt32 cenX;	// �������� X
	gInt32 cenY;	// �������� Y
	//gFloat dis;	// �������ľ���( ��:km )
	//gFloat azi;	// �������ķ�λ( ��  )
	gFloat area;	// �������( �� )
	gFloat d;	// ����Ҷ����ӵ�ƥ��ֵ
	gFloat p;	// ��λ����
	gInt32 iVol;
	//tm time;	// bound��ʱ��
}tagBoundHeader;

//һ��������ͷ��Ϣ��
typedef struct _BoundFileHeader
{
	gUint16 cntBound;	// ��������
	gUint16	VolumeScanDate;		//ɨ������
	gInt32	VolumeScanStartTime; //ɨ��ʱ��
	gInt16 Width;	//ͼƬ��ȣ���λ������
	gInt16 Height;	//ͼƬ�߶ȣ���λ������
}tagBoundFileHeader;

//��һ��������ɵ�һ��ͼ�Ľṹ�壻
typedef struct _OutBound
{
	tagBoundHeader head;  //����ͷ
	gInt16 X[MAX_POINTS];		//������X����
	gInt16 Y[MAX_POINTS];		//������Y����
}OutBound;

#endif
