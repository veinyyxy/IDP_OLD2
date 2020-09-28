//产品结构体
typedef struct EVPElements
{
	int hight;//高度(单位：10M)
	short Uh;//风速（单位：0.01m/s，取值范围 0－12800）
	short Du;//风向（单位：0.1度，取值范围 0－3600）
	int div;//散度（扩大1000000倍）
	int stretch;//拉伸形变（单位：0.000001/s）
	int shear;//切伸形变（单位：0.000001/s）
}EVP_ELEMENTS;
typedef struct  VwpElements
{
	int time;//未使用
	short hight;//高度(单位：10M)
	short Uh;//风速（单位：0.01m/s，取值范围 0－12800）
	short Du;//风向（单位：0.1度，取值范围 0－3600）
	short RMS;//风速均方根差（单位：0.01m/s;取值范围 0－100）
}VWP_ELEMENTS;
typedef struct VadElements
{
	gInt16 hight;//高度(单位：10M)
	gInt16 CF0;//傅里叶第一系数 （单位：0.01m/s）
	//gInt16 FIT[360];	//FIT=CF0-cos(Az-Du)*U;  （单位：0.01m/s）
	gInt16 Vel[360];		//多普勒速度	0.01m/s
	gInt16 RD[360];		//基本反射率因子   0.01DBZ
	gInt16 U; //相对速度（单位：0.01m/s）
	gInt16 Uh;//风速（单位：0.01m/s）
	gInt16 Du;//风向（单位：0.1度）
}VAD_ELEMENTS;