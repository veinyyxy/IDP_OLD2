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
	virtual CProductDrawType GetTypeOfProduct()=0;//判断产品绘图类型
	virtual int GetCountOfLayer(CProductRead* lpProduct = NULL)=0;//判断产品绘图数量，共几张图
//0309注释掉	virtual bool DrawLayer(wxBitmap* lpBitmap,CProductRead* lpProduct, CProductDrawParam* lpParam,int requestLayer = 0)=0;//画产品图片，指定某张图片
};

class DLLEXP_GSYS cDrawProduct : public ProductBase
{
public:
	cDrawProduct(void);
	~cDrawProduct(void);

public :
	//绘图相关，调用DrawLayer。
//0309注释掉	void Draw(wxBitmap* lpBitmap, CProductDrawParam* lpParam );//画产品图片，第一张
public :
	//绘图相关。
	CProductDrawType GetTypeOfProduct();//判断产品绘图类型
	int GetCountOfLayer();//判断产品绘图数量，共几张图
	int GetProCode();
//0309注释掉	void DrawLayer(wxBitmap* lpBitmap, CProductDrawParam* lpParam,int requestLayer = -1);//画产品图片，指定某张图片

public :
	//获取各种统计信息，右侧窗口等使用。
	int TransToOperationalModeInt(const gInt8* lpOperationalMode);//add by ljg 20081224 for OperationalMode changes type.
	int TransToVCPsn(const gInt8* lpVCPMode);//add by ljg 20081224 for VCP --> VCPMode changes.
	string TransToRadarCode5(string strRadarCode);//add by ljg 20081223 for radarcode changes.
//0309注释掉	string TransToRadarName(string strRadarCode);//add by ljg 20081219 for productbase changes.
//0309注释掉	string TransToProductName(string strProductCode);//add by ljg 20081219 for productbase changes.
	int GetDbzMax(float & dbzMax,int requestLayer = -1);//add by ljg 20080825 for functions recharge.
	int GetWight(int & wight,/*int & realwight,*/int requestLayer = -1);//add by ljg 20080825 for functions recharge.
	//int GetRealWight(int requestLayer = 0);//add by ljg 20080825 for functions recharge.
	void GetGeo(float * geo,int requestLayer = 0);//add by ljg 20080825 for functions recharge.
	void GetDist(int * dist,int requestLayer = 0,int width = 0);//add by ljg 20080825 for functions recharge.
//0309注释掉	void GetIsolineParamGrid(map_y_x_z &map_yxz, int &max_x, int &max_y, int requestlayer=0);	//add by lyj for get isoline param from the grid product "x,y,z"
//0309注释掉	void map2array(map_y_x_z &map_yxz, int **arr, int max_x, int max_y);
//0309注释掉	void GetIsolineParamRadial(IsolineRadialList &isolineRadialList/*out*/, int &radius/*out*/, wxPoint &origin/*out*/, int bmp_width/*in*/, int bmp_height/*in*/, int requestlayer = 0/*in*/);	//add by lyj for 为等值线获得径向产品数据
private :
	//分支处理部分，DrawLayer等内部使用。
	CProductDraw* GetInstanceOfProductDraw(int nProductCode);//获取特定的产品绘图类指针，基类指针
	void ReleaseInstanceOfProductDraw(CProductDraw* lpInstance);//释放产品绘图类指针，基类指针
//0309注释掉	void DrawRadial(wxBitmap* lpBitmap,CProductDrawParam* lpParam ,int requestLayer = 0);//画产品图片，径向
//0309注释掉	void DrawGrid(wxBitmap* lpBitmap,CProductDrawParam* lpParam ,int requestLayer = 0);//画产品图片，网格，通用过程
//0309注释掉	void DrawStruct(wxBitmap* lpBitmap,CProductDrawParam* lpParam ,int requestLayer = 0);//画产品图片，结构体

	//add by ljg 20081024 for RHI需要绘制坐标轴
//0309注释掉	void DrawGridDefault(wxBitmap* lpBitmap,CProductDrawParam* lpParam ,int requestLayer = 0);//画产品图片，网格,默认过程（只画图）

	//AE by ljg 20080822 for functions recharge.

public :
	//DS by ljg 20080827 for using new interface.
	//void DrawImageOnRadial(wxImageList* imagelist, const wxColour colorRibbon[], int cn[][2]);
	//void DrawImageOnRadial(wxImageList* imagelist, const wxColour colorRibbon[], int cn[][2] , int memberofcn);                            //为了动态控制cn的显示  选择重载  
	//void DrawImageOnRadial(wxImageList* imagelist, const wxColour colorRibbon[], int cn[][2],int & dbzMax ,int & wight,int & realwight);////wxImageList& imagelist,
	//void DrawImageOnRadial(wxImageList* imagelist, const wxColour colorRibbon[], int cn[][2],
	//	int memberofcn/* = 15*/,int & dbzMax ,int & wight,int & realwight,
	//	float * geo,int *dist,wxString FileName);     //为了动态控制cn的显示，选择重载 
	////void DrawImageOnRadial(wxImageList* imagelist, const wxColour colorRibbon[], int cn[][2],int memberofcn = 15,int* lpdbzMax=NULL ,int* lpwight=NULL,int* lprealwight=NULL,float * geo=NULL,int *dist=NULL,wxString FileName=wxT(""));     //为了动态控制cn的显示，选择重载 
	//void DrawLayer(wxBitmap* lpBitmap, int &currentLayer, int &totalLayer,
	//	const wxColour colorRibbon[], int cn[][2],int memberofcn,int & dbzMax ,
	//	int & wight,int & realwight,float * geo,int *dist,wxString FileName,
	//	int requestLayer = 0);     //add by ljg 20080820 for 只绘制某一层 
	//DE by ljg 20080827 for using new interface.
public :
	void OnGetDiagramParametre(std::vector<vec_int> &theArray, int memberofcn, float cn[][2], double *dGateOrReso, int requestLayer=0); //面积统计遍历产品数据 added by zyf 2008-8-22    
//0309注释掉	void OnGetDiagramParametre(std::vector<vec_float> &theArray, int requestLayer=0);
	//void OnGetTypeOfprpduct(gInt16 & type); //得到产品的类型。 因为有的类型是不做 面积统计的。//delete by wyh for 多余代码 20090108
	//void ReadInfo(int* p,wxString& statName ,double & elevation,wxString &statsit);//modify by ljg 20080821 for surpport multilayer files.
	//bool ReadInfo(int* p,wxString& statName ,double & elevation,wxString &statsit,int nLayer = 0);//modify by ljg 20080821 for surpport multilayer files.
	/************************************************************************/
//0309注释掉	bool ReadInfo(proAttr *pAttr, int nLayer = -1);	//add by lyj 20090326
	bool ReadInfo(proAttr *pAttr, int nLayer = -1);
//0309注释掉	void GetSCIInfo(QString& strVal);	//add by lyj 20090326                                                                      
	/************************************************************************/

	void GetDBBlockDate(double angle, double distance,double &data,double &averageData ,int requestLayer = 0); //added by Limh for    20080514 //modify by Limh for 产品文件结构改变增加层数参数 20080827
//0309注释掉	void GetRowCol(double angle, double distance,int &iRow,int &iCol ,int requestLayer);
	void ReadInfo(vector<vec_int>& theArray, int& num_aziumth, int& num_gate, gInt32& elevation, int& height,int requestLayer,int &len_gate,int bmpHei,float zoom,double *rate);	//读取剖面所需信息 ,添加参数，雷达高度，仰角层 limei，08.21
	//void GetElevation(double& elevation);//modify by ljg 20080821 for surpport multilayer files.
//0309注释掉	void GetElevation(double& elevation,int nLayer = 0);//modify by ljg 20080821 for surpport multilayer files.
	void * ReadStructData();//added by wyh for 画VWP产品时读取结构数据 20080804
//0309注释掉	void CreateSCIProduct(wxTextCtrl *pDrawTextCtrl);//added by yourb 20080812
	//void DrawLRM(wxBitmap &bitmap, const wxColour colorRibbon[], float cn[][2],int memberofcn, 
	//float & dbzMax,float * geo);//added by yourb 20080822
//0309注释掉	void GetMaxValue(float * fValue,int layer); //added by wyh for 读取最大风速与风速均方根差 20080825 //VAD分层
	void GetMaxValue(float * fValue,int layer); //added by wyh for 读取最大风速与风速均方根差 20080825 //VAD分层
	void ReadInformaton(int Layer, int &Elevation,int &NyquistVelocity, int &Azi_Num,int &Gate_Num, int &Num);// add by yangys for 获取仰角，最大不模糊速度，与方位角数与距离库数,退模糊次数
	gInt16* GetDataV ( int ElevationIndex, int AzimuthIndex );//add by yangys for 得到径向上速度值
	void SetPara(int NyquistVelocity,int Number );//改变退模糊后的最大小模糊速度值与退模糊次数。
};

class DLLEXP_GSYS cDrawMultiProduct : public MultiProductBase
{
public :
	cDrawMultiProduct(){;};
	~cDrawMultiProduct(){;};

	//0313注释掉void DrawImageOnRadial(wxBitmap& bitmap, const wxColour colorRibbon[], float cn[][2], int &currentLayer, int &totalLayer, int *platitude_longitude_array,int memberofcn, float &maxValue);
	//0313注释掉void DrawImageOnRadial(wxBitmap& bitmap, const wxColour colorRibbon[], float cn[][2], int requestLayer, int *platitude_longitude_array,int mentberofcn, float &maxValue);
	void ReadInfo(int* p,string& regionName ,string &regionSite, string &proName, string &proCName,int layer=-1);   //默认为中间层
	void GetDBBlockDate(int iCurLayer,double dLongitude, double dLatitude,double &data,double &averageData);//added by Limh for 由层数经纬度得DBZ值  20080619
	//0313注释掉void DrawMLRM(wxBitmap& bitmap, const wxColour colorRibbon[], float cn[][2], int *platitude_longitude_array,int mentberofcn);//added by yourb
	void OnGetDiagramParametre(std::vector<vec_int> & theArray,int memberofcn, float cn[][2], double *dGateOrReso, int requestLayer=-1); //面积统计(遍历产品数据) added by wyh 20081020
	void OnGetDiagramParametre(std::vector<vec_float> & theArray, int requestLayer=-1);
	//0313注释掉void GetRowCol(int iCurLayer,double dLongitude, double dLatitude, int &iRow,int &iCol); //面积统计(得到选定区域的行和列) added by wyh 20081020
	void GetMax_MLRM(float &iMaxValue);
	//void GetMax_MLRM(int &iMaxValue);
	float  GetMaxValue(int Layer);//modify by ljg 20090410
	//int  GetMaxValue(int Layer);
	string TransToRadarName(string strRadarCode);
public :
	//Get类函数
	// void GetMaxValue(int &maxValue,int requestLayer = 0);//add by ljg 20090219 for 获取最大值 
	void GetRange(int *platitude_longitude_array);//add by ljg 20090219 for 获取产品覆盖范围，四角经纬度 

public :
	//绘图相关。
	CProductDrawType GetTypeOfProduct();//判断产品绘图类型//add by ljg 20090219
	//0313注释掉int GetCountOfLayer();//判断产品绘图数量，共几张图//add by ljg 20090219
	//0313注释掉void DrawLayer(wxBitmap* lpBitmap, CProductDrawParam* lpParam,int requestLayer = -1);//画产品图片，指定某张图片//add by ljg 20090219
private :
	//分支处理部分，DrawLayer等内部使用。//add by ljg 20090219
	//0313注释掉CProductDrawMosac* GetInstanceOfProductDraw(int nProductCode);//获取特定的产品绘图类指针，基类指针//add by ljg 20090219
	//0313注释掉void ReleaseInstanceOfProductDraw(CProductDrawMosac* lpInstance);//释放产品绘图类指针，基类指针//add by ljg 20090219
	//0313注释掉void DrawStructDefault(wxBitmap* lpBitmap,CProductDrawParam* lpParam ,int requestLayer = 0);//画产品图片，结构体//add by ljg 20090219
	//0313注释掉void DrawGridDefault(wxBitmap* lpBitmap,CProductDrawParam* lpParam ,int requestLayer = -1);//画产品图片，网格,默认过程（只画图）//add by ljg 20090219
};

