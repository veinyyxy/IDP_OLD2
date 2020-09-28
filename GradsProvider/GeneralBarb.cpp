//#include "SymbolPluginInterface.h"
#include "GeneralBarb.h"
//#include "WindSymbo.h"

extern QVector<WindBarbData*> WindBarbDataArray;			// 风标位置数组

GeneralBarb::GeneralBarb(void)
{
	m_cBitColor = QColor(255,255,255);
}

GeneralBarb::~GeneralBarb(void)
{
}

#if 0

int GeneralBarb::Draw( P_LAYER_ATTRIBUTE pLayerAttribute)
{
	InitCommand();							// 用命令行去除绘制等值线过程中产生的：图形边框、坐标格网、边框左侧及下侧外围的短线

	strcpy(m_Command,"set gxout barb");
	gcmn.sig = 0;
	gacmd(m_Command, &gcmn,0);  

	gcmn.sig = 0;
	if(gacmd("display va;vb",&gcmn,0))
	{
		if(gcmn.str_errorinfo != NULL)
		{
			//CCIPASMessageBox::CIPASCritical(NULL, //CCIPASMessageBox::CIPAS_OK,tr("错误"),gcmn.str_errorinfo);
		}
		return 1;
	}
	if (gcmn.error == true)
	{
		//CCIPASMessageBox::CIPASCritical(NULL, //CCIPASMessageBox::CIPAS_OK,tr("错误"),gcmn.str_errorinfo);
		return 1;
	}

	ScaleDataWind();

	QStringList strRange,n;
	strRange.append("inValid");
	ILayerAttribute *aColor = new ILayerAttribute("aColor","a",tr("绘制设置"),"",QVariant::Invalid, 0, 0 ,strRange);
	ILayerAttribute *aGColor = new ILayerAttribute("aGColor","ag",tr("颜色"),QColor(255,255,255), QVariant::Color,0,15,n);
	aColor->InsertSubItem(aGColor);
	pLayerAttribute->insert("aColor",aColor);

	return 0;
}
#endif

int GeneralBarb::Dump()
{
#if 0
	/******************Wind barb symbol***********/
	if (WindBarbDataArray.count()>0)
	{
		QVector<SymbolPoint *> pSymbolPointArray;
		for (int m = 1;m<11;m++)
		{
			SymbolPoint * pSymbolpoint = new SymbolPoint;
			/*QVector<Line*>* vLines = ((CWindSymbo*)pSymbolpoint->getSymbol())->GetWindLines();
			if (NULL != vLines)
			{
			for (int i = 0;i < vLines->size();i++)
			{
			Line *pLine = vLines->at(i);
			QVector4DArray *pColor = pLine->getColorArray();
			QVector3DArray *pArray = pLine->getVectexArray();
			pColor->Clear();
			for(int j = 0 ; j < pArray->count(); j++)
			{
			pColor->push_back(QVector4D(m_cBitColor.red()/255.0f,m_cBitColor.green()/255.0f,m_cBitColor.blue()/255.0f,1));
			}
			pLine->SetColorArray();
			}
			}*/
			
			//pSymbolpoint->getColorArray()->DataUsage(GL_COLOR_ARRAY);
			//pSymbolpoint->getColorArray()->clear();
			//pSymbolpoint->SetArrayData(pSymbolpoint->getColorArray());
			//pSymbolpoint->getColorArray()->push_back(QVector4D(m_cBitColor.red()/255.0f,m_cBitColor.green()/255.0f,m_cBitColor.blue()/255.0f,1));
			Symbol *pSymbol = m_pSymbolPlugin->GetSymbol(m+100);
			if (pSymbol)
			{
				pSymbolpoint->SetPointStyle(pSymbol);
				pSymbolpoint->CoordTrans((Proj4Transform*)m_cTransform);
				pSymbol->SetNormalColor(QVector4D(m_cBitColor.red()/255.0f\
					,m_cBitColor.green()/255.0f,m_cBitColor.blue()/255.0f,1));
			}
			pSymbolPointArray.push_back(pSymbolpoint);
		}
		for (int k= 0;k<WindBarbDataArray.count();k++)
		{
			WindBarbData *pTempWindData = WindBarbDataArray.at(k);
			int nspd = (int)pTempWindData->spd;
			QVector3D glPoint(pTempWindData->x,pTempWindData->y,0.0f);
			pSymbolPointArray.at(nspd)->SetPosition(glPoint);
			pSymbolPointArray.at(nspd)->SetAngle(pTempWindData->dir);
			
		}
		for (int mm = 0;mm<10;mm++)
		{	
			pSymbolPointArray.at(mm)->SetBakData();
			container->InsertRenderable(pSymbolPointArray.at(mm));
		}
	}
#endif
	return 0;
}

void GeneralBarb::InitCommand()
{
	gcmn.sig = 0;
	gacmd("set csmooth on",&gcmn,0);
	gcmn.sig = 0;
	gacmd("set cterp on",&gcmn,0);
	gcmn.sig = 0;
	gacmd("set grid off",&gcmn,0);
	gcmn.sig = 0;
	gacmd("set mpdraw off",&gcmn,0);
	gcmn.sig = 0;
	gacmd("set frame off",&gcmn,0);
	gcmn.sig = 0;
	gacmd("set clab off",&gcmn,0);
	gcmn.sig = 0;
	gacmd("set grads off",&gcmn,0);
	gcmn.sig = 0;
	gacmd("set xlab off",&gcmn,0);
	gcmn.sig = 0;
	gacmd("set ylab off",&gcmn,0);
	gcmn.sig = 0;
	gacmd("set arrlab off",&gcmn,0);
	gcmn.sig = 0;
	gacmd("set lon 0 360",&gcmn,0);   // added by Liu Xinsheng 2011-05-02
	gcmn.sig = 0;
	gacmd("set lat -89 89",&gcmn,0);     // added by WangJL 2011-05-02
	gcmn.sig = 0;
	gacmd("set cstyle 1",&gcmn,0);
}