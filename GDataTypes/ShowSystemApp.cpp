#include "ShowSystemApp.h"


ShowSystemApp::ShowSystemApp(void)
{
}


ShowSystemApp::~ShowSystemApp(void)
{
}

void ShowSystemApp::GetProdctAttr( proAttr& attr, int proNum, GenerationData *pGata , int requestLayer,bool showflag )
{
	//proAttr attr;
	if (pGata == NULL)
	{
		return;
	}
	if (proNum<500)
	{
		cDrawProduct *pSingleGdata = NULL;
		pSingleGdata = (cDrawProduct *)pGata;
		pSingleGdata->ReadInfo(&attr, requestLayer);//默认读取第0层
		if (showflag == false)
		{
			return;
		}
		int width = 0;
		pSingleGdata->GetWight(width, requestLayer);
		attr.width = width;

		if ((proNum == 285) || (proNum == 269) || (proNum == 268))
		{
			float dbzmax[3] = {0};
			pSingleGdata->GetMaxValue(dbzmax,requestLayer);

			attr.Uh_evp_vwp = dbzmax[0];	//风速
			attr.Du_evp_vwp = dbzmax[2];	//风向
			attr.height_evp_vwp = dbzmax[1];//风速均方根差
		}
		else
		{
			float dbzmax = 0.0;
			pSingleGdata->GetDbzMax(dbzmax, requestLayer);
			attr.max_value = (double)dbzmax;
		}
		int num = attr.pro_code;
		//wxString proNum = wxString::Format(wxT("%d"), num);
		//attr.pro_name = CommonMethod::FindAbFromID(proNum);
	}
	else
	{
		cDrawMultiProduct *pMosaicGdata = NULL;
		pMosaicGdata = (cDrawMultiProduct *)pGata;
		string regionName;
		string regionSite;
		string proName;
		string proCName;
		int pAttr[14] = {0};
		float  maxvalue;

		pMosaicGdata->ReadInfo(pAttr,regionName,regionSite,proName,proCName,requestLayer);
		
		memcpy_s(attr.site_code, 255, regionSite.data(), regionSite.length());
		memcpy_s(attr.staName, 255, regionName.data(), regionName.length());
		
		attr.pro_code = pAttr[0];
		attr.scan_date = pAttr[1];
		attr.scan_time = pAttr[2];
		attr.generation_date = pAttr[3];
		attr.generation_time = pAttr[4];
		attr.latitude = pAttr[5];
		attr.latitude2 = pAttr[6];
		attr.longitude = pAttr[7];
		attr.longitude2 = pAttr[8];
		attr.NumberOfRadars = pAttr[9];
		attr.ResolutionOfRow = pAttr[10];
		attr.ResolutionOfCol = pAttr[11];
		attr.HeightOfElevation = pAttr[12];
		attr.numberoflayers = pAttr[13];
		if (showflag == false)
		{
			return;
		}
		maxvalue = pMosaicGdata->GetMaxValue(requestLayer);
		attr.max_value = (double) maxvalue;
		int num = attr.pro_code;
		//wxString proNum = wxString::Format(wxT("%d"), num);
		//attr.pro_name = CommonMethod::FindAbFromID(proNum);
		//m_attr = attr;
	}
}
