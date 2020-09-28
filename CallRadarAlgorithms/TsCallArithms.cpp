#include <memory>
#include <map>
#include <cstdlib>
#include <fstream>
#include <vector>
#include "../GDataTypes/GDataFiles.h"
#include "gdatetime.h"
#include "../GDataTypes/StandardRadarDataX.h"
#include "RoseAlgoriHeadFiles.h"

ParamMref501::ParamMref501()
{
	GetRadarInfo();
	m_map_param["Cappi_High01"] = "5000";
	m_map_param["ScanDateTime"] = "20130829102910";
}

GHRESULT ParamMref501::LoadParamFunc(char* pKey, char *& pValue)
{
	map<string,string>::iterator ite = m_map_param.begin();
	ite = m_map_param.find(pKey);
	if (ite==m_map_param.end())
	{
		map<string, map<string,string> >::iterator s_ite;
		s_ite = m_StrInfoList.find(m_Site);

		if (s_ite!=m_StrInfoList.end())
		{
			map<string,string>::iterator ss_ite;
			ss_ite = s_ite->second.find(pKey);
			if (ss_ite != s_ite->second.end())
			{
				pValue = (char*)ss_ite->second.c_str();
				return GS_OK;
			}
		}
	}
	else
	{			
		pValue = (char*)ite->second.c_str();
		return GS_OK;
	}

	return GE_INVALIDARG;	//一个或多个参数无效
}

bool ParamMref501::GetRadarInfo()
{
	FILE * pfile = fopen(RADAR_CONFIG_FILE, "rb");
	if (pfile != 0)
	{
		fseek(pfile, 20, SEEK_SET);	
		int radnum = 0;
		fread(&radnum, sizeof(int), 1,  pfile);
		for (int cnt = 0; cnt < radnum; cnt ++)
		{
			CINRAD_NETINF radinfo;
			fread(&radinfo, sizeof(CINRAD_NETINF), 1, pfile);
			char buf[256]="";
			m_StrInfoList[radinfo.id]["SiteCode"]=radinfo.id;
			m_StrInfoList[radinfo.id]["SiteName"]=radinfo.sitename;
			m_StrInfoList[radinfo.id]["RadarType"]=radinfo.type;
			double dLat = ((double)(radinfo.r_lat))/10000;
			sprintf(buf,"%f",dLat);
			m_StrInfoList[radinfo.id]["RadarLat"]=buf;
			double dLon = ((double)(radinfo.r_lon))/10000;
			sprintf(buf,"%f",dLon);
			m_StrInfoList[radinfo.id]["RadarLon"]=buf;
			double dHei = ((double)(radinfo.r_hei))/10;
			sprintf(buf,"%f",dHei);
			m_StrInfoList[radinfo.id]["RadarHeight"]=buf;
			sprintf(buf,"%d",radinfo.tz);
			m_StrInfoList[radinfo.id]["TimeZone"]=buf;
		}
		fclose(pfile);
		return true;
	}
	else
	{
		return false;
	}
}
//////////////////////////////////////////////////////////////
#include <qlibrary.h>
typedef IArith* (*Create_arithms_Ins)(void);
static ParamMref501 g_param501;

TsCallArithms::TsCallArithms()
{
	m_TempFlagRadProd = false;
	m_pTempBaseData.clear();
	m_StDataList.clear();
	m_OutputList.clear();
	m_AfterQc.clear();
	m_ProdTempProdList.clear();
	m_ProductList.clear();
	m_TempRainData.clear();
}

TsCallArithms::~TsCallArithms()
{
}

int TsCallArithms::GdCallRoseArithms(const string& strSite, const vector<string>& vecBaseFileNames, const string& strDesExpDir,const string& strArithmsName, const string& strDepArithms, bool bQc)
{
	EXECALGORTHMSTYPE iDepAlgType = NONEDEPPROD;
	string strBaseFileName = "";
	int res = GS_FALSE;
	auto_ptr<IArith> pArithms;
	FILE* pLogFile = NULL;
	if ("" != strDepArithms)
	{
		iDepAlgType = FindWhatDepProdType(strDepArithms);
	}
	g_param501.m_Site = strSite;
	gChar * radartype = NULL;
	(g_param501)("RadarType",radartype);
	if (NULL == radartype) return GS_FALSE;
	m_start_localtime = GDateTime().getlocaltime().to_string();
	unsigned short iFileSizes = vecBaseFileNames.size();
	for(int i=0; i<iFileSizes; i++)
	{
		strBaseFileName = vecBaseFileNames[i];
		//测试文件
		//////////////////////////////////////////InputTestRadBaseFile//////////////////////////////////////////
		//////////////////////////////////////////InputTestRadBaseFile//////////////////////////////////////////
		BaseData_t *basedata = new BaseData_t;
		res = ReadBaseData(strSite.c_str(),strBaseFileName.c_str(), basedata);
		if (NULL == (pLogFile=fopen("./Log/CallArithms.txt", "a"))) return GS_FALSE;
		fprintf(pLogFile, "radartype = %s\r\nBaseDataFileName = %s\r\n read datasize = %d\n", radartype, strBaseFileName.c_str(),basedata->GetSize());
		if (GS_OK != res)
		{
			fprintf(pLogFile, "%s--------------数据读取失败-----------\r\n", m_start_localtime.c_str());
			delete basedata;
			return res;
		}

		m_pTempBaseData.AddTail();
		*(m_pTempBaseData.GetTail()) = *basedata;
		(g_param501)("RadarType",radartype);

		GXList<GenerationData> *pGenDatas = NULL;
		res = PretreatBaseDataProc(radartype, pLogFile, bQc, &pGenDatas);
		delete basedata;
		m_pTempBaseData.RemoveHead();

		if (GS_OK != res)
		{
			fprintf(pLogFile, "%s--------------格式转换或质控失败！-----------\r\n", m_start_localtime.c_str());
			return res;
		}
		m_StDataList.GetHead()->ClearAll();
		m_StDataList.RemoveHead();

		if ((pArithms=ResolveArithmsDll(strArithmsName)).get())
		{
			switch (iDepAlgType)
			{
			case NONEDEPPROD:
				res = ExecuteArithms(pArithms, pGenDatas, &m_ProductList, &g_param501);
				break;
			case DEPBASEPROD:
				res = ExecuteArithms(ResolveArithmsDll(strDepArithms), pGenDatas, &m_ProdTempProdList, &g_param501);
				if (GS_OK == res) 
				{
					res = ExecuteArithms(pArithms, &m_ProdTempProdList, &m_ProductList, &g_param501);
					m_ProdTempProdList.GetHead()->ClearAll();
					m_ProdTempProdList.RemoveHead();
				}
				break;
			case RAINFALLPROD:
				res = ExecuteArithms(ResolveArithmsDll(strDepArithms), pGenDatas, &m_ProdTempProdList, &g_param501);
				m_TempRainData.AddTail();
				*(m_TempRainData.GetTail()) = *(m_ProdTempProdList.GetTail());
				m_ProdTempProdList.GetHead()->ClearAll();
				m_ProdTempProdList.RemoveHead();
				break;
			default:
				break;
				
			}
			pGenDatas->GetHead()->ClearAll();
		}
	}
	if (RAINFALLPROD == iDepAlgType)
	{
		int iTempQprDataSize = m_TempRainData.GetCount();
		if (GS_OK == res &&  m_TempRainData.GetCount()== iFileSizes) 
		{
			res = ExecuteArithms(pArithms, &m_TempRainData, &m_ProductList, &g_param501);
			for (int i=0; i<iTempQprDataSize; i++)
			{
				m_TempRainData.GetHead()->ClearAll();
				m_TempRainData.RemoveHead();
			}
		}
	}
	if (GS_OK != res)
	{
		fprintf(pLogFile, "%s----------生成产品 %s 失败!----------\r\n", m_start_localtime.c_str(), strArithmsName.c_str());
	}
	else
	{
		fprintf(pLogFile, "%s----------生成产品 %s 成功!----------\r\n", m_start_localtime.c_str(), strArithmsName.c_str());
	}
	fclose(pLogFile);
	m_AfterQc.RemoveHead();
	m_OutputList.RemoveHead();
	if (GS_OK == res)
	{
		SaveRadProd(strDesExpDir, strArithmsName);
	}
    return res;
}

///////////////////////////////////////////////////////////////////////////
/*
   单站基数据读取
*/
GHRESULT TsCallArithms::ReadBaseData(const char* sitecode, const char* filename, BaseData_t* pBaseData)
{	
	GHRESULT res;
	if (GS_OK != (res=((GDataFiles*)pBaseData)->ReadFrom(const_cast<char*>(filename))))
	{
		return res;
	}
	g_param501.m_Site = sitecode;

	return res;
}

/*
   获取算法指针
*/
auto_ptr<IArith> TsCallArithms::ResolveArithmsDll(const string& ArithmsDllName) const
{
	Create_arithms_Ins pCreate_arithms_Ins = NULL;
	pCreate_arithms_Ins = (Create_arithms_Ins) QLibrary::resolve(ArithmsDllName.c_str(), "Create_arith");
	return auto_ptr<IArith>(pCreate_arithms_Ins());
}

/*
   格式统一和质控
*/
GHRESULT TsCallArithms::PretreatBaseDataProc(const gChar* radartype, FILE* pLogFile, bool bQc, GXList<GenerationData>** pGenDatas)
{
	if (!strcmp(radartype,"CINRAD/SA")||!strcmp(radartype,"CINRAD/SB"))
	{
		ExecuteArithms(ResolveArithmsDll("SAToSt.dll"), &m_pTempBaseData, &m_StDataList, &g_param501);
	}
	else if (!strcmp(radartype,"CINRAD/CC"))
	{
		ExecuteArithms(ResolveArithmsDll("CCToSt.dll"), &m_pTempBaseData, &m_StDataList, &g_param501);
	}
	else
	{
		return GS_FALSE;
	}

	int res = ExecuteArithms(ResolveArithmsDll("AziSort.dll"), &m_StDataList, &m_OutputList, &g_param501);
	if (GS_OK != res)
	{
		fprintf(pLogFile, "%s----------插值失败！----------\r\n", m_start_localtime.c_str());
		return res;
	}
	fprintf(pLogFile, "OutputList size = %d\r\n", m_OutputList.size());

	if (bQc)
	{
		res = ExecuteArithms(ResolveArithmsDll("QualityControl_SA.dll"), &m_OutputList, &m_AfterQc, &g_param501);
		if (GS_OK != res)
		{
			fprintf(pLogFile, "%s----------质量控制失败！----------\r\n", m_start_localtime.c_str());
		}
		m_OutputList.GetHead()->ClearAll();
		m_OutputList.RemoveHead();
		*pGenDatas = &m_AfterQc;
	}
	else
	{
		*pGenDatas = &m_OutputList;
	}
	return res;
}

/*
   执行算法生成产品
*/
GHRESULT TsCallArithms::ExecuteArithms(auto_ptr<IArith> pArith, GXList<GenerationData> *pDatas, GXList<GenerationData>* pOutputList, ReadParam* pParam)
{
	if (!pArith.get() || NULL == pDatas || NULL == pOutputList)
	{
		return GS_FALSE;
	}
	if (NULL == pOutputList || NULL == pParam)
	{
		return GE_INVALIDARG;
	}
	if (GS_OK != (pArith->Initialize()))
	{
		return GS_FALSE;
	}
	if (GS_OK != pArith->LoadData(pDatas,pParam))
	{
		return GS_FALSE;
	}
	if (GS_OK != pArith->OutPutData(pOutputList))
	{
		return GS_FALSE;
	}
	if (GS_OK !=  pArith->Execute())
	{
		return GS_FALSE;
	}
	if (GS_OK != pArith->UnInitialize())
	{
		return GS_FALSE;
	}
	return GS_OK;
}
/*
	临时加测试用，需要建立一个算法对照表，通过查表来找出相应的算法库
*/
GHRESULT TsCallArithms::GetArithmsDllNameFromTab(int GenCode, string& ArithmsDllName) const
{
   return GS_OK;
}

EXECALGORTHMSTYPE TsCallArithms::FindWhatDepProdType(const string& strDepArithms)
{
	if ("Arith_262_HBR_270_QPR" == strDepArithms)
	{
		return RAINFALLPROD;
	}
	else
	{
		return DEPBASEPROD;
	}
}
/*
	导出雷达产品
*/
void TsCallArithms::SaveRadProd(const string& strDesExpDir, const string& GenNameStr)
{
	m_exportFileNameList.clear();//清除保存雷达产品文件路径List
	GXList<GenerationData>::iterator ite = m_ProductList.begin();
	for (int iProdLsts=0; ite!=m_ProductList.end(); ++ite,++iProdLsts)
	{
		string filename(g_param501.m_Site);
		filename.insert(5, 1, '_');
		filename += GenNameStr;
		char LayFileName[256];
		memset(LayFileName, 0x0, sizeof(char)*256);
		sprintf(LayFileName,"%s_%s_%02d.bin.bz2",filename.c_str(), m_start_localtime.c_str(),iProdLsts);
		GDataFiles * pgdatafile = (GDataFiles *)&(*ite);
		string strExpFileName = strDesExpDir + "/" + LayFileName;
		if (GS_OK != (pgdatafile->Export(strExpFileName.c_str()/**/,COMPRESS_YES)))
		{
			break;
		}
		m_exportFileNameList.push_back(strExpFileName);
	}
	m_ProductList.GetHead()->ClearAll();
	m_ProductList.clear();
}

vector<string> TsCallArithms::ExportFileNameList()
{
	return m_exportFileNameList;
}
