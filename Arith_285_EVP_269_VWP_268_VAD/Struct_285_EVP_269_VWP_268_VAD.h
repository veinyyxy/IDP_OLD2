//��Ʒ�ṹ��
typedef struct EVPElements
{
	int hight;//�߶�(��λ��10M)
	short Uh;//���٣���λ��0.01m/s��ȡֵ��Χ 0��12800��
	short Du;//���򣨵�λ��0.1�ȣ�ȡֵ��Χ 0��3600��
	int div;//ɢ�ȣ�����1000000����
	int stretch;//�����α䣨��λ��0.000001/s��
	int shear;//�����α䣨��λ��0.000001/s��
}EVP_ELEMENTS;
typedef struct  VwpElements
{
	int time;//δʹ��
	short hight;//�߶�(��λ��10M)
	short Uh;//���٣���λ��0.01m/s��ȡֵ��Χ 0��12800��
	short Du;//���򣨵�λ��0.1�ȣ�ȡֵ��Χ 0��3600��
	short RMS;//���پ��������λ��0.01m/s;ȡֵ��Χ 0��100��
}VWP_ELEMENTS;
typedef struct VadElements
{
	gInt16 hight;//�߶�(��λ��10M)
	gInt16 CF0;//����Ҷ��һϵ�� ����λ��0.01m/s��
	//gInt16 FIT[360];	//FIT=CF0-cos(Az-Du)*U;  ����λ��0.01m/s��
	gInt16 Vel[360];		//�������ٶ�	0.01m/s
	gInt16 RD[360];		//��������������   0.01DBZ
	gInt16 U; //����ٶȣ���λ��0.01m/s��
	gInt16 Uh;//���٣���λ��0.01m/s��
	gInt16 Du;//���򣨵�λ��0.1�ȣ�
}VAD_ELEMENTS;