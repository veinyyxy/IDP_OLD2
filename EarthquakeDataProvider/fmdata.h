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

/** @brief VGrid����*/
struct VGridPara
{
	int nx,ny,nz;
	float f1,f2,f3;
	float f21,f22,f23; 
};
/** @brief ���߽�*/
struct GridBound
{
	float m_fxmin,m_fxmax;
    float m_fymin,m_fymax;
    float m_fzmin,m_fzmax;
};

/** @brief �������� */
struct ProfileData
{
	string strtype; //Z,EW,NS
	int n_u,n_v;
	float umin,umax,vmin,vmax;
	float fmax,fmin;
	vector<float> m_data;
};

/** @class  
    @brief ��ά�����Ϣ
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
		* @param [in] indexx  ���ھ��ȷ��������
		* @param [int] indexy  ����γ�ȷ��������
		* @param [int] indexz  ������ȷ��������
		* @param [out] vector<vector<float>>  ������������
		* @return   return data
		* @note     detailed description �޸���  �޸�ʱ��  �޸�����
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
    @brief   ��ά��������б�
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
    @brief   Grid3dg�ļ�����ͷ
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
    stringvector m_strHead; //��Ϣͷ
	int m_layersize;
	float m_flonmin,m_flatmin,m_flonmax,m_flatmax,m_fdepthmin,m_fdepthmax;
};
*/
/** @class  
    @brief   �ٶȸ�������
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
	stringvector m_strContend; //�ٶȸ�����Ϣ
	int m_Nspikes;
	string m_strVelocityfile;
};
*/
/** @class  ���������
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
//	stringvector m_strContend;  /** @brief �ٶȸ�����Ϣ */
//	//string vectormodelfile; //�ٶ�ģ���ļ�
//	int m_checkBord; /** @brief �Ƿ���Ӽ��壬1Ϊ��ӣ�0Ϊ�����*/
//	int m_Nspikes;
//	float m_depthNW; /** @brief �����1���������*/
//	float m_depthNE; /** @brief �����1��������� */
//	float m_depthSW; /** @brief �����1���ϵ����*/
//};

//////////////////////////////////////////////////////////////////////////
/** @class  
    @brief  ���������ļ�
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
    stringvector m_strInterfaceHead; //interface��Ϣͷ
	stringvector m_strInterfaceHead2; //interface��Ϣͷ
   // vector<stringvector> m_strInterfaces; //interface ��Ϣ��
    
};
*/
//////////////////////////////////////////////////////////////////////////
/** @class  
    @brief  ����·����Ϣ
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
//    @brief   ���������ļ���Ϣ
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
//    @brief   Obsdata�����ļ�
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
