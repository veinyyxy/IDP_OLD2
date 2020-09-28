#pragma  once
#include <string>
#include <vector>

#define INVALID_DATA 999999
#define M_PI 3.14159265358979
#define D2R (M_PI/180.0)
#define R2D (180.0 / M_PI)

#ifndef EARTH_RADIUS
#define EARTH_RADIUS 6378.137
#endif


namespace FMDATA
{
using namespace std;

/** @brief VGrid参数*/
struct VGridPara
{
	int nx,ny,nz;
	float f1,f2,f3;
	float f21,f22,f23; 
};
/** @brief 格点边界*/
struct GridBound
{
	float m_fxmin,m_fxmax;
    float m_fymin,m_fymax;
    float m_fzmin,m_fzmax;
};

/** @brief 剖面数据 */
struct ProfileData
{
	string strtype; //Z,EW,NS
	int n_u,n_v;
	float umin,umax,vmin,vmax;
	float fmax,fmin;
	vector<float> m_data;
};

/** @class  
    @brief 三维格点信息
    @author  author
    @note    detailed description
*/ 
class VGrid
{
public:
	VGrid(int nx,int ny,int nz);
	~VGrid(){};
public:
	void init(int nx,int ny,int nz);
	
	void setdata(int ix,int iy,int iz,float data);
	bool append(float data);
	float Data(int index) const; 
    float Data(int ix,int iy,int iz) const; 

	long size() const;
	int nLon() const {return m_nx;}
	int nLat() const {return m_ny;}
	int nDepth() const {return m_nz;}
	void SetDomain(float xmin,float xmax,float ymin,float ymax,float zmin,float zmax);
	void SetDomain(VGridPara vpar);
	/** 
		* @brief    brief description     
		* @author  list of authors   
		* @param [in] indexx  点在经度方向的索引
		* @param [int] indexy  点在纬度方向的索引
		* @param [int] indexz  点在深度方向的索引
		* @param [out] vector<vector<float>>  返回剖面数据
		* @return   return data
		* @note     detailed description 修改人  修改时间  修改内容
		* @date     create date                                
		* @exception  exception object exception description  
		* @remarks remark text               
		* @deprecated   description            
		* @since when time or version           
		* @see references            
		*/ 
	vector<ProfileData> GetProfile(int indexx,int indexY,int indexZ);

	bool GetProfile(ProfileData& data,string profiletype,float fSliceValue);

private:
	int m_nx,m_ny,m_nz;
	vector<float> m_grid;
    float m_fxmin,m_fxmax,m_fymin,m_fymax,m_fzmin,m_fzmax;
};

/** @class  
    @brief   三维格点数据列表
    @author  author
    @note    detailed description
*/ 
class VGrids
{
public:
	VGrids(const char* filename);
	VGrids(){};
	~VGrids();
public:
	bool readVgridData(const char* filename);
	bool readVgridRef(const char* filename);
	int GridSize() {return m_vGridVector.size();}
    VGrid* GetVGrid(int index);
private:
	int m_nBlock,m_nmiss;
	vector<VGridPara> m_paralist;
	vector<VGrid> m_vGridVector;
};

//////////////////////////////////////////////////////////////////////////
typedef vector<string> stringvector;

/** @class  
    @brief   Grid3dg文件数据头
    @author  author
    @note    detailed description
*/ 
/*
class Grid3dgHead
{
 public:
    Grid3dgHead() {};
    ~Grid3dgHead() {}; 
public:
    void read(std::ifstream& fin);
   	void write(FILE* fp);
	int LayerSize() const {return m_layersize;}
	float Getflonmin() const {return m_flonmin;}
	float Getflonmax() const {return m_flonmax;}
	float Getflatmin() const {return m_flatmin;}
	float Getflatmax() const {return m_flatmax;}
	float Getfdepthmin() const {return m_fdepthmin;}
	float Getfdepthmax() const {return m_fdepthmax;}
private: 
    stringvector m_strHead; //信息头
	int m_layersize;
	float m_flonmin,m_flatmin,m_flonmax,m_flatmax,m_fdepthmin,m_fdepthmax;
};
*/
/** @class  
    @brief   速度格网属性
    @author  author
    @note    detailed description
*/ 
/*
class VelocityGrid
{
public:
	VelocityGrid() {};
	~VelocityGrid() {}; 
public:
	void read(std::ifstream& fin);
	void write(FILE* fp);
private: 
	stringvector m_strContend; //速度格网信息
	int m_Nspikes;
	string m_strVelocityfile;
};
*/
/** @class  间断面属性
    @brief   brief description
    @author  author
    @note    detailed description
*/ 
//
//typedef class interfaceProPerty
//{
//public:
//	interfaceProPerty() {};
//	~interfaceProPerty() {};
//	void read(std::ifstream& fin);
//	void write(FILE* fp);
//private:
//	
//	stringvector m_strContend;  /** @brief 速度格网信息 */
//	//string vectormodelfile; //速度模型文件
//	int m_checkBord; /** @brief 是否添加检测板，1为添加，0为不添加*/
//	int m_Nspikes;
//	float m_depthNW; /** @brief 间断面1北西点深度*/
//	float m_depthNE; /** @brief 间断面1东北点深度 */
//	float m_depthSW; /** @brief 间断面1西南点深度*/
//};

//////////////////////////////////////////////////////////////////////////
/** @class  
    @brief  格网参数文件
    @author  author
    @note    detailed description
*/ 
/*
class Grid3dg
{
public:
	Grid3dg(const char* filename);
	Grid3dg(){};
	~Grid3dg(){};
    bool read(const char* filename);
	bool write(const char* filename);
private:
    GridBound m_bound;
    int m_nLayerNum;
   // string m_VelocityModelString; 
    Grid3dgHead m_grid3dghead;
	vector<VelocityGrid> m_vGrid;
	vector<interfaceProPerty> m_iGrid;
	int m_nNS,m_nEW;
	string m_vgridendstr;
    stringvector m_strInterfaceHead; //interface信息头
	stringvector m_strInterfaceHead2; //interface信息头
   // vector<stringvector> m_strInterfaces; //interface 信息块
    
};
*/
//////////////////////////////////////////////////////////////////////////
/** @class  
    @brief  传播路径信息
    @author  author
    @note    detailed description
*/ 
//class PathSegments
//{
//public:
//	PathSegments() {};
//	~PathSegments() {};
//	void Read(std::ifstream& fin);
//	void Write(std::ifstream& fin);
//	void Write(FILE* fp);
//private:
//	stringvector m_StrdataAll;
//};
//
///** @class  
//    @brief   震相数据文件信息
//    @author  author
//    @note    detailed description
//*/ 
//class SourceFileInfo
//{
//public:
//	SourceFileInfo() {};
//	~SourceFileInfo() {};
//    void Read(std::ifstream& fin);
//	void Write(FILE* fp);
//private:
//	stringvector m_StrdataAll;
//	string m_filename;
//	int m_pathsegNum;
//	vector<PathSegments> m_PathSegs;
//};
//
//
///** @class  
//    @brief   Obsdata数据文件
//    @author  author
//    @note    detailed description
//*/ 
//class Obsdata
//{
//public:
//	Obsdata(){};
//	~Obsdata() {};
//    bool read(const char* filename);
//	void Write(const char* filename);
//	void clear();
//private:
//	stringvector m_flehead;
//	vector<SourceFileInfo> m_srcfleInfo;
//	int m_sourcefileNum;
//
//
//	string m_strOutSourcefile;
//	string m_strOutRecieverfile;	
//	int m_NearPhaseCount;
//	int m_FarPhaseCount;
//};

}
