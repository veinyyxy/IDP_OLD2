#include "MosaicDataProvider.h"
#include "goQuadrilateral.h"
#include "goGeometry.h"
#include <QtCore/QCoreApplication>

#define NO_OBSERVE_DATA  -32768
#define DATA_INVALID     -32766

typedef struct tagLINE_MAXREF_INFO
{
	float fStartLon;			//开始 度
	float fEndLon;			    //结束 度
	gInt16 fMaxRef;				//最大反射率的值 dbZ
	float fMaxHgt;				//高度 km
	float fLat;				    //（度）
	unsigned short iLatGrids;   //纬向上第几个格点
}LINE_MAXREF_INFO;

typedef struct tagVecMaxRefInfo
{
	int idx;					//索引
	std::vector<int> vecPos;
	std::vector<LINE_MAXREF_INFO>  VecLines;
}VEC_MAXREF_INFO;


MosaicDataProvider::MosaicDataProvider()
{
}

MosaicDataProvider::~MosaicDataProvider()
{}

void MosaicDataProvider::ProdDataProvider( Layer * pLayer,int requestLayer,double &maxValue,QHash<int,QVector4D>colorHash )

{
	CProductDrawType nType=this->GetTypeOfProduct();
	if(nType!=enProductUnkown)
	{
		switch(nType)
		{
		case enProductGrid:
			MosGetGridData(pLayer,requestLayer,maxValue,colorHash);
			break;
		case enProductStruct:
		case enProductUnkown:
		default:
			//some err report for file invalide.
			break;
		}
	}
	//return DataMap;
}

CProductDrawType MosaicDataProvider::GetTypeOfProduct()//判断产品绘图类型
{
	CProductDrawType nRet=enProductUnkown;
	MultiDLIBlock* dblock = GetMultiBlock(0,0);
	if(dblock!=NULL)
	{
		switch(dblock->TypeOfDataPackets )
		{
		case BLOCKTYPE_RADIAL:
			nRet=enProductRadial;
			break;
		case BLOCKTYPE_GRID:
			nRet=enProductGrid;
			break;
		case BLOCKTYPE_STRUCT:
			nRet=enProductStruct;
			break;
		case BLOCKTYPE_LONLATGRID:
			nRet=enProductGrid;
			break;
		default:
			nRet=enProductUnkown;
			break;
		};
	}
	return nRet;
}

void MosaicDataProvider::MosGetGridData( Layer * pLayer,int requestLayer,double &maxValue,QHash<int,QVector4D>colorHash  )
{
	QMap <int,osg::ref_ptr <osg::Vec3Array>> dataMap;
	QMap <int,osg::ref_ptr <osg::Vec4Array>> colorMap;
	osg::ref_ptr <osg::Vec3Array> temp_array = NULL  ;//数据数组
	osg::ref_ptr <osg::Vec4Array> temp_color_array =NULL;//颜色数组

	QHash<int,QVector4D> ::iterator iter;
	for (iter= colorHash.begin() ; iter != colorHash.end(); ++iter )
	{
		int key = iter.key();
		temp_array = new osg::Vec3Array  ;//数据数组
		temp_color_array = new osg::Vec4Array;//颜色数组
		dataMap.insert(key,temp_array);
		colorMap.insert(key,temp_color_array);
	}

	temp_array = new osg::Vec3Array  ;//数据数组
	temp_color_array = new osg::Vec4Array;//颜色数组
	dataMap.insert(-32766,temp_array);
	colorMap.insert(-32766,temp_color_array);
	Layer::MgoGeometry* pMG = pLayer->GetMgoGeometry();
	AreaInfo * pAreaInfo = GetAreaInfoP();
	gInt32 lon = pAreaInfo->/*LongitudeOfEast*/LongitudeOfWest;
	gInt32 lat = pAreaInfo->/*LatitudeOfSouth*/LatitudeOfNorth;
	MultiDataHead *pMDblock = GetMultiHeadP();
	MultiGridHead *pMGHead  = (MultiGridHead *)GetBlockDataAsGrid_Head(0, 0);

	if(pMDblock==NULL)
		return;

	osg::Vec3 ps[4];
   // osg::Vec3 psmax[4];

	requestLayer = 0;
	
	float nwidth = pMGHead->ResolutionOfRow/10000.0;	
	float nheight = pMGHead->ResolutionOfCol/10000.0;

	MultiLayerHead* layer = GetLayerP(requestLayer);
	if(layer == NULL)
		return;

	osg::Vec3 p1,p2,p3,p4;
	osg::Vec3 p2_next, p3_next;

	for (int ii = 0;ii < pMGHead->NumberOfRows ; ii++)
	{	
		p1 = osg::Vec3(lon/10000.0+nwidth/10,lat/10000.0-ii*nheight/10,0);
		p2 = osg::Vec3(lon/10000.0+nwidth/10,lat/10000.0-ii*nheight/10,0);
		p3 = osg::Vec3(lon/10000.0+nwidth/10,lat/10000.0-(ii+1)*nheight/10,0);
		p4 = osg::Vec3(lon/10000.0+nwidth/10,lat/10000.0-(ii+1)*nheight/10,0);

		gInt16 *arr = GetLineDataP(requestLayer,ii);
		for (int jj =0 ;jj <pMGHead->NumberOfCols ; jj++)
		{
			p1 = p2;
			p4 = p3;

			
			if (arr[jj] == -32768 ||arr[jj] == -32767||arr[jj] == -32766)
			{
				p2.set((lon/10000.0+jj * nwidth/10),(lat/10000.0-ii * nheight/10),0);
				p3.set((lon/10000.0+jj * nwidth/10),(lat/10000.0-(ii+1)*nheight/10),0);

				ps[0].set(p1);
				ps[1].set(p2);
				ps[2].set(p3);
				ps[3].set(p4);
				continue;
			}
			p2.set((lon/10000.0+jj * nwidth/10),(lat/10000.0-ii * nheight/10),0);
			p3.set((lon/10000.0+jj * nwidth/10),(lat/10000.0-(ii+1)*nheight/10),0);
			ps[0].set(p1);
			ps[1].set(p2);
			ps[2].set(p3);
			ps[3].set(p4);
			
			double val = arr[jj]/100.0;
			maxValue = val>maxValue ? val:maxValue;
			osg::Vec4 tempcolor;
			//if ( -32766 == val )
			//{
			//	int key = -32766;
			//	tempcolor.set(0.5f,0,0.5f,1);
			//	osg::ref_ptr <osg::Vec3Array> temp_data_arr = dataMap.value(key);
			//	osg::ref_ptr <osg::Vec4Array> temp_color_arr = colorMap.value(key);
			//	temp_data_arr->push_back(osg::Vec3(ps[0].x(),ps[0].y(),0));
			//	temp_data_arr->push_back(osg::Vec3(ps[1].x(),ps[1].y(),0));
			//	temp_data_arr->push_back(osg::Vec3(ps[2].x(),ps[2].y(),0));
			//	temp_data_arr->push_back(osg::Vec3(ps[3].x(),ps[3].y(),0));
			//	temp_color_arr->push_back(tempcolor);
			//	temp_color_arr->push_back(tempcolor);
			//	temp_color_arr->push_back(tempcolor);
			//	temp_color_arr->push_back(tempcolor);
			//}				
			//else 
			{
				int key = val/5;
				if (key >colorHash.size()||key < 0)
				{
					continue;
				}
				QVector4D tempcolor_QT = colorHash.value(key);
				tempcolor.set(tempcolor_QT.x() ,tempcolor_QT.y(),tempcolor_QT.z(),0.7);
				osg::ref_ptr <osg::Vec3Array> temp_data_arr = dataMap.value(key);
				if (NULL ==temp_data_arr)
				{
					continue;
				}
				osg::ref_ptr <osg::Vec4Array> temp_color_arr = colorMap.value(key);
				if (NULL ==temp_color_arr)
				{
					continue;
				}
				temp_data_arr->push_back(osg::Vec3(ps[0].x(),ps[0].y(),0));
				temp_data_arr->push_back(osg::Vec3(ps[1].x(),ps[1].y(),0));
				temp_data_arr->push_back(osg::Vec3(ps[2].x(),ps[2].y(),0));
				temp_data_arr->push_back(osg::Vec3(ps[3].x(),ps[3].y(),0));
				temp_color_arr->push_back(tempcolor);
				temp_color_arr->push_back(tempcolor);
				temp_color_arr->push_back(tempcolor);
				temp_color_arr->push_back(tempcolor);
			}
    	}
	}

 //   if((maxValue != -32768) && (maxValue != -32767) && (maxValue != -32766) && (maxValue != -32765))
	//{
	//	maxValue /= pMDblock->ScaleOfData;
	//	double val = maxValue/100.0;
	//	for (int i = 0;i < pColorTable->count();i++)
	//	{
	//		P_COLORNODE_PRODUCT color = pColorTable->at(i);
	//		osg::Vec4 tempcolor;
	//		if (val >= color->fStart && val <= color->fEnd)
	//		{
	//			QString key = QString::number(color->fStart) + QString("-") + QString::number(color->fEnd);
	//			tempcolor.set(color->nRed/255.0 ,color->nGreen/255.0,color->nBlue/255.0,0.7);
	//			osg::ref_ptr <osg::Vec3Array> temp_data_arr = dataMap.value(key);
	//			osg::ref_ptr <osg::Vec4Array> temp_color_arr = colorMap.value(key);

	//			temp_data_arr->push_back(osg::Vec3(ps[0].x(),ps[0].y(),0));
	//			temp_data_arr->push_back(osg::Vec3(ps[1].x(),ps[1].y(),0));
	//			temp_data_arr->push_back(osg::Vec3(ps[2].x(),ps[2].y(),0));
	//			temp_data_arr->push_back(osg::Vec3(ps[3].x(),ps[3].y(),0));

	//			temp_color_arr->push_back(tempcolor);
	//			temp_color_arr->push_back(tempcolor);
	//			temp_color_arr->push_back(tempcolor);
	//			temp_color_arr->push_back(tempcolor);
	//			break;
	//		}
	//		else if ( -32766 == val )
	//		{
	//			QString key = QString::number(-32766);
	//			tempcolor.set(0.5f,0,0.5f,1);
	//			osg::ref_ptr <osg::Vec3Array> temp_data_arr = dataMap.value(key);
	//			osg::ref_ptr <osg::Vec4Array> temp_color_arr = colorMap.value(key);
	//			temp_data_arr->push_back(osg::Vec3(ps[0].x(),ps[0].y(),0));
	//			temp_data_arr->push_back(osg::Vec3(ps[1].x(),ps[1].y(),0));
	//			temp_data_arr->push_back(osg::Vec3(ps[2].x(),ps[2].y(),0));
	//			temp_data_arr->push_back(osg::Vec3(ps[3].x(),ps[3].y(),0));
	//			temp_color_arr->push_back(tempcolor);
	//			temp_color_arr->push_back(tempcolor);
	//			temp_color_arr->push_back(tempcolor);
	//			temp_color_arr->push_back(tempcolor);
	//			break;
	//		}

	//	}	
	//}
	QMap<int,osg::ref_ptr <osg::Vec3Array>> ::iterator it;
	int pNum = 0;
	for (it= dataMap.begin() ; it != dataMap.end(); ++it )
	{
		goQuadrilateral* gpl = new goQuadrilateral();
		gpl->SetCoordinateTransform(pLayer->CoordTrans());
		gpl->SetOsgArray(it.value());
		gpl->SetOsgColorArray(*(colorMap.value(it.key())));
		gpl->SetParentLayer(pLayer);


		pMG->insert(Layer::MgoGeometryPair(pNum,gpl));
		pNum++;
	}
}

int MosaicDataProvider::ProductCode()
{

	AreaInfo * pAreaInfo = GetAreaInfoP();
	if (!pAreaInfo)
	{
		return 0;
	}
	gInt32 lon = pAreaInfo->/*LongitudeOfEast*/LongitudeOfWest;
	gInt32 lat = pAreaInfo->/*LatitudeOfSouth*/LatitudeOfNorth;
	MultiDataHead *pMDblock = GetMultiHeadP();
	MultiGridHead *pMGHead  = (MultiGridHead *)GetBlockDataAsGrid_Head(0, 0);

	int productcode = pMDblock->ProductCode;
	return productcode;
}



QStringList MosaicDataProvider::GetRange()
{
	QStringList sl;
	AreaInfo * pAreaInfo = GetAreaInfoP();
	sl.append(QString::number(pAreaInfo->LatitudeOfNorth));
	sl.append(QString::number(pAreaInfo->LatitudeOfSouth));
	sl.append(QString::number(pAreaInfo->LongitudeOfEast));
	sl.append(QString::number(pAreaInfo->LongitudeOfWest));
	return sl;
}

QRectF *MosaicDataProvider::GetRect()
{
	QRectF *rect = new QRectF();
	AreaInfo * pAreaInfo = GetAreaInfoP();
	rect->setLeft(pAreaInfo->LongitudeOfWest/10000.0);
	rect->setRight(pAreaInfo->LongitudeOfEast/10000.0);
	rect->setTop(pAreaInfo->LatitudeOfNorth/10000.0);
	rect->setBottom(pAreaInfo->LatitudeOfSouth/10000.0);

	return rect;
}

void MosaicDataProvider::DrawPic( QString& filePath,int requestLayer,P_COLORTABLE_VECTOR pColor )
{
	CProductDrawType nType=this->GetTypeOfProduct();
	if(nType!=enProductUnkown)
	{
		switch(nType)
		{
		case enProductGrid:
			DrawGridPic(filePath,requestLayer,pColor);
			break;
		case enProductStruct:
		case enProductUnkown:
		default:
			//some err report for file invalide.
			break;
		}
	}
}

void MosaicDataProvider::DrawGridPic( QString& filePath,int requestLayer,P_COLORTABLE_VECTOR pColorTable )
{
	AreaInfo * pAreaInfo = GetAreaInfoP();
	gInt32 lon = pAreaInfo->/*LongitudeOfEast*/LongitudeOfWest;
	gInt32 lat = pAreaInfo->/*LatitudeOfSouth*/LatitudeOfNorth;
	MultiDataHead *pMDblock = GetMultiHeadP();
	MultiGridHead *pMGHead  = (MultiGridHead *)GetBlockDataAsGrid_Head(0, 0);
	//QRectF *rect = GetRect();
	//int wighth = rect->right() - rect->left();
	//int hight = rect->top() - rect->bottom();
	if(pMDblock==NULL)
		return;

	osg::Vec3 ps[4];
	osg::Vec3 psmax[4];

	requestLayer = 0;

	float nwidth = pMGHead->ResolutionOfRow/10000.0;	
	float nheight = pMGHead->ResolutionOfCol/10000.0;

	MultiLayerHead* layer = GetLayerP(requestLayer);
	if(layer == NULL)
		return;

	osg::Vec3 p1,p2,p3,p4;
	osg::Vec3 p2_next, p3_next;
	QString backPic =  QCoreApplication::applicationDirPath() + "/Config/BackGround.png";
	QImage backimg(backPic);
    QImage image = backimg.copy(0,0,pMGHead->NumberOfRows,pMGHead->NumberOfCols);
	QRgb value;
	value = qRgb(255,255,255);
	//image.fill(value);

	for (int ii = 0;ii < pMGHead->NumberOfRows ; ii++)
	{	
		p1 = osg::Vec3(0,ii*nheight,0);
		p2 = osg::Vec3(0,ii*nheight,0);
		p3 = osg::Vec3(0,(ii+1)*nheight,0);
		p4 = osg::Vec3(0,(ii+1)*nheight,0);

		gInt16 *arr = GetLineDataP(requestLayer,ii);
		for (int jj =0 ;jj <pMGHead->NumberOfCols ; jj++)
		{
			p1 = p2;
			p4 = p3;
			p2.set((lon/10000.0+jj * nwidth/10),(lat/10000.0-ii * nheight/10),0);
			p3.set((lon/10000.0+jj * nwidth/10),(lat/10000.0-(ii+1)*nheight/10),0);

			ps[0].set(p1.x(),p1.y(),0);
			ps[1].set(p2.x(),p2.y(),0);
			ps[2].set(p3.x(),p3.y(),0);
			ps[3].set(p4.x(),p4.y(),0);

			if (arr[jj] != -32768)
			{		

				double val = arr[jj]/100.0;
				for (int i = 0;i < pColorTable->count();i++)
				{
					P_COLORNODE_PRODUCT color = pColorTable->at(i);
					if (val >= color->fStart && val <= color->fEnd)
					{
						value = qRgb(color->nRed,color->nGreen,color->nBlue);
						image.setPixel(jj,ii, value);
						image.setPixel(jj,ii+1, value);
						image.setPixel(jj+1,ii, value);
						image.setPixel(jj+1,ii+1, value);

					}
					else if ( -32766 == val )
					{
						value = qRgb(color->nRed,color->nGreen,color->nBlue);
						image.setPixel(jj,ii, value);
						image.setPixel(jj,ii+1, value);
						image.setPixel(jj+1,ii, value);
						image.setPixel(jj+1,ii+1, value);
						break;
					}

				}	
			}
		}
	}

	image.save(filePath);
}

void MosaicDataProvider::LoadWeaEleAlgorithm(std::vector<SAVEELE_INFO> & savele_info)
{
	AreaInfo* pAreaInfo = GetAreaInfoP();
	MultiDataHead *pMDblock = GetMultiHeadP();
	gInt16 NumofLayers = pMDblock->NumberOfLayers;
	gInt16* m_pCap;
	gInt32* m_pCapHgt;
	for (gInt16 i=0; i<NumofLayers; i++)
	{
		MultiGridHead *pMGHead  = (MultiGridHead *)GetBlockDataAsGrid_Head(0, 0);
		MultiLayerHead* layer = GetLayerP(i);
		if (layer == NULL)
			return ;
		gInt32 iHgt = layer->HeightOfElevation;
		gInt16 GridRows = pMGHead->NumberOfRows;
		gInt16 GridCols = pMGHead->NumberOfCols;
		m_pCap = new gInt16[GridRows * GridCols];
		memset(m_pCap, NO_OBSERVE_DATA, sizeof(gInt16)*GridRows*GridCols);
		m_pCapHgt = new gInt32[GridRows * GridCols];
		memset(m_pCapHgt, 0x0, sizeof(gInt16)*GridRows*GridCols);
		for (int ii=0; ii<GridRows; ii++)
		{
			gInt16 *arr = GetLineDataP(i, ii);
			for (int jj =0; jj<GridCols; jj++)
			{
				if (NO_OBSERVE_DATA == arr[jj] || DATA_INVALID == arr[jj])
				{
					continue;
				}
				if (arr[jj] > m_pCap[ii*GridCols+jj])
				{
					m_pCap[ii*GridCols+jj] = arr[jj]/100;
					//qDebug()<< m_pCap[ii*GridCols+jj];
					m_pCapHgt[ii*GridCols+jj] = iHgt;
					//qDebug()<<m_pCapHgt[ii*GridCols+jj];
				}
			}
		}
	}

	gInt32 lon = pAreaInfo->LongitudeOfWest;
	gInt32 lat = pAreaInfo->LatitudeOfNorth;
    LoadSaveEleInfo(lon, lat,m_pCap,m_pCapHgt,savele_info);


}

void MosaicDataProvider::LoadSaveEleInfo( gInt32 lon, gInt32 lat,gInt16*m_pCap,gInt32*m_pCapHgt ,std::vector<SAVEELE_INFO> & savele_info)
{
	const int THRE_REF = 20;
	const unsigned short THRE_DOWN_REF_DIF = 5;
	const unsigned short DOWNCOUNT_THRE = 3;
	const double THRE_LINELEN = 0.019;
	const float REF_AVG_FAC = 4;
	const float ZEROHGT_FAC = 3.;
	const float THRE_LAT_STEP = 1.;
	const float THRE_LON_OVERLAP = 0.01;
	const unsigned short THRE_NUMOFSEG = 2;
	const float THRE_COM_AREA = 1.;


	MultiGridHead *pMGHead  = (MultiGridHead *)GetBlockDataAsGrid_Head(0, 0);
	float nwidth = pMGHead->ResolutionOfRow/10000.0;	
	float nheight = pMGHead->ResolutionOfCol/10000.0;
	gInt16 GridRows = pMGHead->NumberOfRows;
	gInt16 GridCols = pMGHead->NumberOfCols;

	std::vector<LINE_MAXREF_INFO> VecMaxRefInfo;
	for (gInt16 i=0; i<GridRows; i++)
	{
		bool bStartFlag = false;
		bool bEndFlag = false;
		for (gInt16 j=0; j<GridCols; j++)
		{
			double fBegRge=0, fEndRge=0;
			unsigned short iBegGrid=0, iEndGrid=0;
			int iDownCount =0;
			gInt16 iVal = m_pCap[i*GridCols + j];
			if (iVal >= THRE_REF)
			{
				if (!bStartFlag)
				{
					fBegRge = i*nwidth - nwidth/2;//统计的起始距离
					iBegGrid = i;                 //起始距离库数
					bStartFlag = true;            
					bEndFlag = false;             
					iDownCount = 0;               //复位
				}
				fEndRge = i*nwidth + nwidth/2;    //统计的终止距离
				iEndGrid = i;                     //终止的距离库数
			}
			else if (iVal >= THRE_REF-THRE_DOWN_REF_DIF && bStartFlag)
			{
				iDownCount++;
				if	(iDownCount > DOWNCOUNT_THRE)//越界
				{
					bEndFlag = true;             //终止统计
				}
			}
			else if (bStartFlag)                
			{
				bEndFlag = true;                 //终止统计
			}
			else
			{
				continue;
			}
			if (bStartFlag && bEndFlag)
			{
				bStartFlag = false;
				bEndFlag = false;

				double fLen = fEndRge - fBegRge;
				if (fLen >= THRE_LINELEN)
				{
					LINE_MAXREF_INFO MaxRefInfo;
					double fTempMaxValue = 0.;
					float fTempMaxValueHgt = 0.;
					for (gInt16 l=iBegGrid; l<=iEndGrid; l++)
					{
						float fRefVal;
						float fHgtVal;
						if (l>=REF_AVG_FAC/2 && l<GridCols-REF_AVG_FAC/2)
						{
							fRefVal = 0;
							fHgtVal = 0.;
							for (int m=l-(int)(REF_AVG_FAC/2); m<=l+(int)(REF_AVG_FAC/2); m++)
							{
								fRefVal += m_pCap[i*GridCols + m];
								fHgtVal += m_pCapHgt[i*GridCols + m]*0.001;
							}
							fRefVal /= REF_AVG_FAC+1;
							fHgtVal /= REF_AVG_FAC+1;
						}
						else
						{
							fRefVal = m_pCap[i*GridCols + l];
							fHgtVal = m_pCapHgt[i*GridCols + l]*0.001;
						}
						if(fRefVal>fTempMaxValue) fTempMaxValue = fRefVal;
						fTempMaxValueHgt = (fHgtVal>fTempMaxValueHgt) ? fHgtVal : fTempMaxValueHgt;
					}
					if (fTempMaxValueHgt < ZEROHGT_FAC) continue;
					MaxRefInfo.iLatGrids = i;
					MaxRefInfo.fStartLon = fBegRge;
					MaxRefInfo.fEndLon = fEndRge;
					MaxRefInfo.fMaxRef = fTempMaxValue;
					MaxRefInfo.fMaxHgt = fTempMaxValueHgt;
					MaxRefInfo.fLat = (GridRows-i-1)*nwidth;
					VecMaxRefInfo.push_back(MaxRefInfo);
				}
			}
		}
	}

	std::vector<VEC_MAXREF_INFO> VecMerInfo;
	LINE_MAXREF_INFO CurInfo,NextInfo;
	int iNumRefInfo = VecMaxRefInfo.size();
	int* nFlag = new int[iNumRefInfo];
	memset(nFlag, -1, sizeof(int)*iNumRefInfo);
	bool bMerge = false;
	int nIndex = -1;
	for (int k=0; k<iNumRefInfo; k++)
	{
		CurInfo = VecMaxRefInfo[k];
		for (unsigned short w=0; w<VecMerInfo.size(); w++)
		{
			bMerge = false;
			int iSize =  VecMerInfo[w].VecLines.size();
			for (int t=0; t< iSize; t++)
			{
				NextInfo = VecMerInfo[w].VecLines[t];
				float fComStartLon = (CurInfo.fStartLon >= NextInfo.fEndLon) ? CurInfo.fStartLon : NextInfo.fStartLon;
				float fComEndLon = (CurInfo.fEndLon <= NextInfo.fEndLon) ? CurInfo.fEndLon : NextInfo.fEndLon;
				float fRange_OverLap = fComEndLon - fComStartLon;	
				float flat_Step = fabs((float)CurInfo.iLatGrids - NextInfo.iLatGrids);
				if ((flat_Step <= THRE_LAT_STEP) && (fRange_OverLap >= THRE_LON_OVERLAP))
				{
					if (-1 == nFlag[k])
					{
						VecMerInfo[w].VecLines.push_back(CurInfo);
						VecMerInfo[w].vecPos.push_back(k);
						nFlag[k] = VecMerInfo[w].idx;
					}
					else
					{
						if (nFlag[k] != VecMerInfo[w].idx)
						{
							bMerge = true;
						}
					}
					break;
				}//end of if
			}//end of t
			if (bMerge)
			{
				for (unsigned short icom=0; icom<VecMerInfo.size(); icom++)
				{
					if (VecMerInfo[icom].idx == nFlag[k])
					{
						for (unsigned short t=0; t<VecMerInfo[w].VecLines.size(); t++)
						{
							int n_pos = VecMerInfo[w].vecPos[t];
							nFlag[n_pos] = nFlag[k];
							LINE_MAXREF_INFO tempInfo = VecMerInfo[w].VecLines[t];

							VecMerInfo[icom].VecLines.push_back(tempInfo);
							VecMerInfo[icom].vecPos.push_back(k);
						}
						VecMerInfo[w].vecPos.clear();
						VecMerInfo[w].VecLines.clear();
						VecMerInfo[w].idx = -1;
					}//end of if
				}//end of n_com
			}//if merge
		}//end of w
		if (-1 == nFlag[k])
		{
			VEC_MAXREF_INFO tempVectInfo;
			nIndex ++ ;
			tempVectInfo.idx = nIndex;
			tempVectInfo.vecPos.push_back(k);
			tempVectInfo.VecLines.push_back(CurInfo);
			VecMerInfo.push_back (tempVectInfo);
			nFlag[k] = nIndex;
		}
	}
	delete []nFlag;
	nFlag = NULL;

	std::vector<VEC_MAXREF_INFO>::iterator CurIterator;
	std::vector<VEC_MAXREF_INFO>::iterator nIterator;
	CurIterator = VecMerInfo.begin();
	int iTotal =  VecMerInfo.size();
	for (int nCounter=0; nCounter<iTotal; nCounter++)
	{
		if (0 == CurIterator->VecLines.size())
		{
			nIterator = VecMerInfo.erase(CurIterator);
			CurIterator = nIterator;
		}
		else
		{
			CurIterator++;
		}
	}

	double fArea = 0.;
	float fAvgLon = 0.; 
	float fAvgLat = 0.;
	float fAvgHgt = 0.;
	int iSegCount = 0;
	int iNumCom = VecMerInfo.size();
	float fLon = lon*0.001;
	float fLat = lat*0.001;
	savele_info.clear();
	for (int kk=0; kk<iNumCom; kk++)
	{
		iSegCount = VecMerInfo[kk].VecLines.size();
		if (iSegCount < THRE_NUMOFSEG) continue;
		for (gInt16 i=0; i<iSegCount; i++)
		{
			CurInfo = VecMerInfo[kk].VecLines[i];
			double fTemp = fabs(CurInfo.fEndLon-CurInfo.fStartLon);
			fAvgLon += (CurInfo.fStartLon + CurInfo.fEndLon)*0.5;
			fAvgLat += (CurInfo.fLat)*0.5;
			fAvgHgt += (CurInfo.fMaxHgt)*0.5;
			fArea += fTemp*1.;
		}
		if (fArea >= THRE_COM_AREA)
		{
			fAvgLon /= iSegCount;
			fAvgLat /= iSegCount;
			fAvgHgt /= iSegCount;
			SAVEELE_INFO tempEleInfo;
			tempEleInfo.id = kk;
			tempEleInfo.Lon = fLon + fAvgLon;
			tempEleInfo.Lat = fLat + fAvgLat;
			tempEleInfo.Hgt = fAvgHgt;
			savele_info.push_back(tempEleInfo);
		}
	}
	if (NULL != m_pCap)
	{
		delete []m_pCap;
		m_pCap = NULL;
	}
	if (NULL != m_pCapHgt)
	{
		delete []m_pCapHgt;
		m_pCapHgt = NULL;
	}
}

double MosaicDataProvider::GetValue( double lon,double lat,double height,CoordinateTransform *proj )
{
	AreaInfo * pAreaInfo = GetAreaInfoP();
	if (NULL == pAreaInfo)
	{
		return 0.0;
	}
	gInt32 lons = pAreaInfo->/*LongitudeOfEast*/LongitudeOfWest;
	gInt32 lats = pAreaInfo->/*LatitudeOfSouth*/LatitudeOfNorth;
	MultiDataHead *pMDblock = GetMultiHeadP();
	MultiGridHead *pMGHead  = (MultiGridHead *)GetBlockDataAsGrid_Head(0, 0);

	if(pMDblock==NULL)
		return 0.0;

	osg::Vec3 ps[4];
	// osg::Vec3 psmax[4];

	float nwidth = pMGHead->ResolutionOfRow/10000.0;	
	float nheight = pMGHead->ResolutionOfCol/10000.0;
	int x = abs(lon -lons/10000.0)/nwidth *10;
	int y = abs(lats/10000.0- lat)/nheight *10;
	if (x>pMGHead->NumberOfCols||y>=pMGHead->NumberOfRows)
	{
		return 0.0; 
	}
	gInt16 *arr = GetLineDataP(0,y);
	if (NULL == arr)return 0.0;

	double val = arr[x];
	if (val == -32768 ||val == -32767||val == -32766)
		return 0.0;

	return val/100.0;
	
}

