////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

#ifndef _TSCALLARITHMS_H_
#define _TSCALLARITHMS_H_

#include <vector>
#include "../GDataTypes/GData.h"
#include "../GDataTypes/GXList.h"
#include "../GDataTypes/IArith.h"//参数
extern "C"{
#include <stdlib.h>
}
#include "callradaralgorithms_global.h"

using std::vector;
typedef GenerationData BaseData_t;//基数据类型
#define RADAR_CONFIG_FILE    "./config/CINRAD_classB.dat"

//数据类型
typedef struct tagCINRAD_netinfo 
{   
	char  id[8]; // radar id string such as 'Z9270'
	char  sitename[20]; // radar site name, in Chinese
	char  provname[20]; // province name, in Chinese
	char  type[12];     // CINRAD/XX
	int   r_lat;      // units: 0.0001deg
	int   r_lon;     // units: 0.0001deg
	int   r_hei;    // units: 0.1 meter
	int   data_range ; // units in 1 km
	int   tz;     // time clock of products generator
	int   wlon;   // westest of LatLonGrid of radar coverage, in 0.001deg
	int   elon;   // eastest of LatLonGrid of radar coverage, in 0.001deg
	int   nlat;   // northest of LatLonGrid of radar coverage, in 0.001deg
	int   slat;   // southest of LatLonGrid of radar coverage, in 0.001deg
	char  spare[4]; // tail;
}CINRAD_NETINF;

class ParamMref501 : public ReadParam
{
public:
	ParamMref501();
	virtual GHRESULT LoadParamFunc(char* pKey, char *& pValue);

private:
	bool GetRadarInfo();

public:	
	string m_Site;//站点
	map<string, string> m_map_param;
	map<string, map<string,string> > m_StrInfoList;
};

typedef enum
{
	NONEDEPPROD = 0,
	DEPBASEPROD,
	RAINFALLPROD
}EXECALGORTHMSTYPE;

class CALLRADARALGORITHMS_EXPORT TsCallArithms
{
public:
	TsCallArithms();
	~TsCallArithms();
	int GdCallRoseArithms(const string& strSite, const vector<string>& vecBaseFileNames, const string& strDesExpDir, const string& strArithmsName, const string& strDepArithm, bool bQc);
	vector<string> ExportFileNameList();
protected:
	EXECALGORTHMSTYPE FindWhatDepProdType(const string& strDepArithms);
	GHRESULT ReadBaseData(const char* sitecode, const char* filename, BaseData_t*  pBaseData);
	GHRESULT PretreatBaseDataProc(const gChar* radartype, FILE* pLogFile, bool bQc, GXList<GenerationData>** pGenDatas);
	auto_ptr <IArith> ResolveArithmsDll(const string& ArithmsDllName) const;
	GHRESULT GetArithmsDllNameFromTab(int GenCode, string& ArithmsDllName) const;
	GHRESULT ExecuteArithms(auto_ptr<IArith> pArith, GXList<GenerationData> *Datas, GXList<GenerationData> * pOutputList, ReadParam* pParam);
	void SaveRadProd(const string& strDesExpDir, const string& GenNameStr);

private:
	string m_start_localtime;
	bool m_TempFlagRadProd;
	vector<string> m_exportFileNameList; //导出的文件列表
	GXList<GenerationData> m_pTempBaseData;
	GXList<GenerationData> m_StDataList;
	GXList<GenerationData> m_OutputList;
	GXList<GenerationData> m_AfterQc;
	GXList<GenerationData> m_ProdTempProdList;
	GXList<GenerationData> m_ProductList;

	GXList<GenerationData> m_TempRainData;
};
#endif//_TSCALLARITHMS_H_

