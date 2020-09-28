#pragma once
#include "singleproductbase.h"
#include "MultiProductBase.h"
#include "ProductRead.h"
#include "ProductInfo.h"
//#include "../RadarProductDataProvider/SingleDataProvider.h"
typedef enum tgProductType
{
	enProductRadial,
	enProductGrid,
	enProductStruct,
	enProductUnkown
}CProductDrawType;

class DLLEXP_GSYS CProductDraw
{
public:
	CProductDraw();
	//virtual ~CProductDraw();
public:
	virtual CProductDrawType GetTypeOfProduct()=0;//�жϲ�Ʒ��ͼ����
	virtual int GetCountOfLayer(CProductRead* lpProduct = NULL)=0;//�жϲ�Ʒ��ͼ������������ͼ
//0309ע�͵�	virtual bool DrawLayer(wxBitmap* lpBitmap,CProductRead* lpProduct, CProductDrawParam* lpParam,int requestLayer = 0)=0;//����ƷͼƬ��ָ��ĳ��ͼƬ
};

class DLLEXP_GSYS cDrawProduct : public ProductBase
{
public:
	cDrawProduct(void);
	~cDrawProduct(void);

public :
	//��ͼ��أ�����DrawLayer��
//0309ע�͵�	void Draw(wxBitmap* lpBitmap, CProductDrawParam* lpParam );//����ƷͼƬ����һ��
public :
	//��ͼ��ء�
	CProductDrawType GetTypeOfProduct();//�жϲ�Ʒ��ͼ����
	int GetCountOfLayer();//�жϲ�Ʒ��ͼ������������ͼ
	int GetProCode();
//0309ע�͵�	void DrawLayer(wxBitmap* lpBitmap, CProductDrawParam* lpParam,int requestLayer = -1);//����ƷͼƬ��ָ��ĳ��ͼƬ

public :
	//��ȡ����ͳ����Ϣ���Ҳര�ڵ�ʹ�á�
	int TransToOperationalModeInt(const gInt8* lpOperationalMode);//add by ljg 20081224 for OperationalMode changes type.
	int TransToVCPsn(const gInt8* lpVCPMode);//add by ljg 20081224 for VCP --> VCPMode changes.
	string TransToRadarCode5(string strRadarCode);//add by ljg 20081223 for radarcode changes.
//0309ע�͵�	string TransToRadarName(string strRadarCode);//add by ljg 20081219 for productbase changes.
//0309ע�͵�	string TransToProductName(string strProductCode);//add by ljg 20081219 for productbase changes.
	int GetDbzMax(float & dbzMax,int requestLayer = -1);//add by ljg 20080825 for functions recharge.
	int GetWight(int & wight,/*int & realwight,*/int requestLayer = -1);//add by ljg 20080825 for functions recharge.
	//int GetRealWight(int requestLayer = 0);//add by ljg 20080825 for functions recharge.
	void GetGeo(float * geo,int requestLayer = 0);//add by ljg 20080825 for functions recharge.
	void GetDist(int * dist,int requestLayer = 0,int width = 0);//add by ljg 20080825 for functions recharge.
//0309ע�͵�	void GetIsolineParamGrid(map_y_x_z &map_yxz, int &max_x, int &max_y, int requestlayer=0);	//add by lyj for get isoline param from the grid product "x,y,z"
//0309ע�͵�	void map2array(map_y_x_z &map_yxz, int **arr, int max_x, int max_y);
//0309ע�͵�	void GetIsolineParamRadial(IsolineRadialList &isolineRadialList/*out*/, int &radius/*out*/, wxPoint &origin/*out*/, int bmp_width/*in*/, int bmp_height/*in*/, int requestlayer = 0/*in*/);	//add by lyj for Ϊ��ֵ�߻�þ����Ʒ����
private :
	//��֧�����֣�DrawLayer���ڲ�ʹ�á�
	CProductDraw* GetInstanceOfProductDraw(int nProductCode);//��ȡ�ض��Ĳ�Ʒ��ͼ��ָ�룬����ָ��
	void ReleaseInstanceOfProductDraw(CProductDraw* lpInstance);//�ͷŲ�Ʒ��ͼ��ָ�룬����ָ��
//0309ע�͵�	void DrawRadial(wxBitmap* lpBitmap,CProductDrawParam* lpParam ,int requestLayer = 0);//����ƷͼƬ������
//0309ע�͵�	void DrawGrid(wxBitmap* lpBitmap,CProductDrawParam* lpParam ,int requestLayer = 0);//����ƷͼƬ������ͨ�ù���
//0309ע�͵�	void DrawStruct(wxBitmap* lpBitmap,CProductDrawParam* lpParam ,int requestLayer = 0);//����ƷͼƬ���ṹ��

	//add by ljg 20081024 for RHI��Ҫ����������
//0309ע�͵�	void DrawGridDefault(wxBitmap* lpBitmap,CProductDrawParam* lpParam ,int requestLayer = 0);//����ƷͼƬ������,Ĭ�Ϲ��̣�ֻ��ͼ��

	//AE by ljg 20080822 for functions recharge.

public :
	//DS by ljg 20080827 for using new interface.
	//void DrawImageOnRadial(wxImageList* imagelist, const wxColour colorRibbon[], int cn[][2]);
	//void DrawImageOnRadial(wxImageList* imagelist, const wxColour colorRibbon[], int cn[][2] , int memberofcn);                            //Ϊ�˶�̬����cn����ʾ  ѡ������  
	//void DrawImageOnRadial(wxImageList* imagelist, const wxColour colorRibbon[], int cn[][2],int & dbzMax ,int & wight,int & realwight);////wxImageList& imagelist,
	//void DrawImageOnRadial(wxImageList* imagelist, const wxColour colorRibbon[], int cn[][2],
	//	int memberofcn/* = 15*/,int & dbzMax ,int & wight,int & realwight,
	//	float * geo,int *dist,wxString FileName);     //Ϊ�˶�̬����cn����ʾ��ѡ������ 
	////void DrawImageOnRadial(wxImageList* imagelist, const wxColour colorRibbon[], int cn[][2],int memberofcn = 15,int* lpdbzMax=NULL ,int* lpwight=NULL,int* lprealwight=NULL,float * geo=NULL,int *dist=NULL,wxString FileName=wxT(""));     //Ϊ�˶�̬����cn����ʾ��ѡ������ 
	//void DrawLayer(wxBitmap* lpBitmap, int &currentLayer, int &totalLayer,
	//	const wxColour colorRibbon[], int cn[][2],int memberofcn,int & dbzMax ,
	//	int & wight,int & realwight,float * geo,int *dist,wxString FileName,
	//	int requestLayer = 0);     //add by ljg 20080820 for ֻ����ĳһ�� 
	//DE by ljg 20080827 for using new interface.
public :
	void OnGetDiagramParametre(std::vector<vec_int> &theArray, int memberofcn, float cn[][2], double *dGateOrReso, int requestLayer=0); //���ͳ�Ʊ�����Ʒ���� added by zyf 2008-8-22    
//0309ע�͵�	void OnGetDiagramParametre(std::vector<vec_float> &theArray, int requestLayer=0);
	//void OnGetTypeOfprpduct(gInt16 & type); //�õ���Ʒ�����͡� ��Ϊ�е������ǲ��� ���ͳ�Ƶġ�//delete by wyh for ������� 20090108
	//void ReadInfo(int* p,wxString& statName ,double & elevation,wxString &statsit);//modify by ljg 20080821 for surpport multilayer files.
	//bool ReadInfo(int* p,wxString& statName ,double & elevation,wxString &statsit,int nLayer = 0);//modify by ljg 20080821 for surpport multilayer files.
	/************************************************************************/
//0309ע�͵�	bool ReadInfo(proAttr *pAttr, int nLayer = -1);	//add by lyj 20090326
	bool ReadInfo(proAttr *pAttr, int nLayer = -1);
//0309ע�͵�	void GetSCIInfo(QString& strVal);	//add by lyj 20090326                                                                      
	/************************************************************************/

	void GetDBBlockDate(double angle, double distance,double &data,double &averageData ,int requestLayer = 0); //added by Limh for    20080514 //modify by Limh for ��Ʒ�ļ��ṹ�ı����Ӳ������� 20080827
//0309ע�͵�	void GetRowCol(double angle, double distance,int &iRow,int &iCol ,int requestLayer);
	void ReadInfo(vector<vec_int>& theArray, int& num_aziumth, int& num_gate, gInt32& elevation, int& height,int requestLayer,int &len_gate,int bmpHei,float zoom,double *rate);	//��ȡ����������Ϣ ,��Ӳ������״�߶ȣ����ǲ� limei��08.21
	//void GetElevation(double& elevation);//modify by ljg 20080821 for surpport multilayer files.
//0309ע�͵�	void GetElevation(double& elevation,int nLayer = 0);//modify by ljg 20080821 for surpport multilayer files.
	void * ReadStructData();//added by wyh for ��VWP��Ʒʱ��ȡ�ṹ���� 20080804
//0309ע�͵�	void CreateSCIProduct(wxTextCtrl *pDrawTextCtrl);//added by yourb 20080812
	//void DrawLRM(wxBitmap &bitmap, const wxColour colorRibbon[], float cn[][2],int memberofcn, 
	//float & dbzMax,float * geo);//added by yourb 20080822
//0309ע�͵�	void GetMaxValue(float * fValue,int layer); //added by wyh for ��ȡ����������پ������� 20080825 //VAD�ֲ�
	void GetMaxValue(float * fValue,int layer); //added by wyh for ��ȡ����������پ������� 20080825 //VAD�ֲ�
	void ReadInformaton(int Layer, int &Elevation,int &NyquistVelocity, int &Azi_Num,int &Gate_Num, int &Num);// add by yangys for ��ȡ���ǣ����ģ���ٶȣ��뷽λ������������,��ģ������
	gInt16* GetDataV ( int ElevationIndex, int AzimuthIndex );//add by yangys for �õ��������ٶ�ֵ
	void SetPara(int NyquistVelocity,int Number );//�ı���ģ��������Сģ���ٶ�ֵ����ģ��������
};

class DLLEXP_GSYS cDrawMultiProduct : public MultiProductBase
{
public :
	cDrawMultiProduct(){;};
	~cDrawMultiProduct(){;};

	//0313ע�͵�void DrawImageOnRadial(wxBitmap& bitmap, const wxColour colorRibbon[], float cn[][2], int &currentLayer, int &totalLayer, int *platitude_longitude_array,int memberofcn, float &maxValue);
	//0313ע�͵�void DrawImageOnRadial(wxBitmap& bitmap, const wxColour colorRibbon[], float cn[][2], int requestLayer, int *platitude_longitude_array,int mentberofcn, float &maxValue);
	void ReadInfo(int* p,string& regionName ,string &regionSite, string &proName, string &proCName,int layer=-1);   //Ĭ��Ϊ�м��
	void GetDBBlockDate(int iCurLayer,double dLongitude, double dLatitude,double &data,double &averageData);//added by Limh for �ɲ�����γ�ȵ�DBZֵ  20080619
	//0313ע�͵�void DrawMLRM(wxBitmap& bitmap, const wxColour colorRibbon[], float cn[][2], int *platitude_longitude_array,int mentberofcn);//added by yourb
	void OnGetDiagramParametre(std::vector<vec_int> & theArray,int memberofcn, float cn[][2], double *dGateOrReso, int requestLayer=-1); //���ͳ��(������Ʒ����) added by wyh 20081020
	void OnGetDiagramParametre(std::vector<vec_float> & theArray, int requestLayer=-1);
	//0313ע�͵�void GetRowCol(int iCurLayer,double dLongitude, double dLatitude, int &iRow,int &iCol); //���ͳ��(�õ�ѡ��������к���) added by wyh 20081020
	void GetMax_MLRM(float &iMaxValue);
	//void GetMax_MLRM(int &iMaxValue);
	float  GetMaxValue(int Layer);//modify by ljg 20090410
	//int  GetMaxValue(int Layer);
	string TransToRadarName(string strRadarCode);
public :
	//Get�ຯ��
	// void GetMaxValue(int &maxValue,int requestLayer = 0);//add by ljg 20090219 for ��ȡ���ֵ 
	void GetRange(int *platitude_longitude_array);//add by ljg 20090219 for ��ȡ��Ʒ���Ƿ�Χ���ĽǾ�γ�� 

public :
	//��ͼ��ء�
	CProductDrawType GetTypeOfProduct();//�жϲ�Ʒ��ͼ����//add by ljg 20090219
	//0313ע�͵�int GetCountOfLayer();//�жϲ�Ʒ��ͼ������������ͼ//add by ljg 20090219
	//0313ע�͵�void DrawLayer(wxBitmap* lpBitmap, CProductDrawParam* lpParam,int requestLayer = -1);//����ƷͼƬ��ָ��ĳ��ͼƬ//add by ljg 20090219
private :
	//��֧�����֣�DrawLayer���ڲ�ʹ�á�//add by ljg 20090219
	//0313ע�͵�CProductDrawMosac* GetInstanceOfProductDraw(int nProductCode);//��ȡ�ض��Ĳ�Ʒ��ͼ��ָ�룬����ָ��//add by ljg 20090219
	//0313ע�͵�void ReleaseInstanceOfProductDraw(CProductDrawMosac* lpInstance);//�ͷŲ�Ʒ��ͼ��ָ�룬����ָ��//add by ljg 20090219
	//0313ע�͵�void DrawStructDefault(wxBitmap* lpBitmap,CProductDrawParam* lpParam ,int requestLayer = 0);//����ƷͼƬ���ṹ��//add by ljg 20090219
	//0313ע�͵�void DrawGridDefault(wxBitmap* lpBitmap,CProductDrawParam* lpParam ,int requestLayer = -1);//����ƷͼƬ������,Ĭ�Ϲ��̣�ֻ��ͼ��//add by ljg 20090219
};

