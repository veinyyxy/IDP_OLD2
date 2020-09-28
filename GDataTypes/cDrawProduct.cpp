#include "cDrawProduct.h"
#include "windows.h"

cDrawProduct::cDrawProduct(void)
{
}


cDrawProduct::~cDrawProduct(void)
{
}

CProductDrawType cDrawProduct::GetTypeOfProduct()
{
	CProductDrawType nRet=enProductUnkown;
	DLIBlock* dblock = GetBlock(0,0);
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


int cDrawProduct::GetCountOfLayer()
{
 	bool bFlag=false;
 	int nCount=0;
 	gInt16 nProductCode=0;
 	CProductDrawType nType=GetTypeOfProduct();
 	GetProductCode(nProductCode);


	//MS by ljg 20080826
	switch(nProductCode)
	{
		case 278:
			nCount = 1;
			bFlag=true;
			break;
		default:
			bFlag=false;
			break;
	}

	if(nType==enProductStruct)
	{
		CProductDraw* p=GetInstanceOfProductDraw(nProductCode);
		if(p!=NULL)
		{
			nCount=p->GetCountOfLayer((CProductRead*)this);
			bFlag=true;
			ReleaseInstanceOfProductDraw(p);
		}
	}
	//ME by ljg 20080826
	if(bFlag!=true)
	{
		RadarInfor * pRadarInfo = GetRadarInfo();
		PDBlock* blok = GetPDBlock();
		if(blok!=NULL )
		{
			nCount=blok->NumberOfLayers;
		}
	}
	return nCount;
}

int cDrawProduct::TransToOperationalModeInt( const gInt8* lpOperationalMode )
{
	int nRet=-1;
	nRet=((lpOperationalMode[0]<<8)+lpOperationalMode[1]);
	switch(nRet)
	{
	case 01:
		nRet=2;//降水
		break;
	case 02:
		nRet=1;//晴空
		break;
	default:
		break;
	}
	return nRet;
}

int cDrawProduct::TransToVCPsn(const gInt8* lpVCPMode)
{
	int nRet=-1;
	if(lpVCPMode!=NULL)
	{
		nRet=((lpVCPMode[0]<<24)+(lpVCPMode[1]<<16)+(lpVCPMode[2]<<8)+lpVCPMode[3]);
		switch(nRet)
		{
		case 0x01141605:
			nRet=0;//VCP11
			break;
		case 0x02091106:
			nRet=1;//VCP21
			break;
		case 0x03050810:
			nRet=2;//VCP31
			break;
		case 0x03050710:
			nRet=3;//VCP32
			break;
		default:
			break;
		}
	}
	return nRet;
};//add by ljg 20081224 for VCP --> VCPMode changes.

string cDrawProduct::TransToRadarCode5(string strRadarCode)
{
	string strRadarCode5 = strRadarCode;
	//modify by ljg 20090413 for 字符串非零结尾。
	int n=strRadarCode.length();
	int i=strRadarCode.find("Z9");
	if(n>=8&&i==2)//00Z90010
	{
		strRadarCode5="Z9"+strRadarCode.substr(i+3,i+3+3);
	}
	else if(n>=5&&i==0)//Z9010
	{
 		strRadarCode5=strRadarCode.erase(5);
	}

	//if(strRadarCode.Length()==8)
	//{
	//	strRadarCode5=wxT("Z9")+strRadarCode.Right(3);
	//}
	return strRadarCode5;
};//add by ljg 20081223 for radarcode changes.

int cDrawProduct::GetDbzMax(float & dbzMax,int requestLayer/* = -1 */)
{
	dbzMax = 0.0 ;
	PDBlock* blok = GetPDBlock();
	if(blok == NULL)
		return dbzMax;
	int productCode = blok->ProductCode;//294产品跟其他产品不一样的地方 added by haodemei 20090206

	int numberOfLayers = (int)blok->NumberOfLayers;
	if(requestLayer<0)
	{
		if (productCode == 258)
		{
			requestLayer = numberOfLayers/2;
		}
		else
			requestLayer=0;
	}

	for (int i = requestLayer ; i < blok->NumberOfLayers ; i++)
	{
		DILayer* layer = GetLayer(i);
		if(layer == NULL)
			return dbzMax;
		for (int j = 0;j < layer->NumberOfBlocks ; j++)
		{
			DLIBlock* dblock = GetBlock(i,j);
			if(dblock == NULL)
				return dbzMax;
			if(dblock->TypeOfDataPackets == BLOCKTYPE_RADIAL)
			{
				RadialHead* radialhead = GetBlockDataAsRadial_Head(i,j); 
				for (int k = 0; k < radialhead->NumberOfRadials ; k++)//
				{
					RadialData* radialdata = GetBlockDataAsRadial_RadialData(i,j,k);
					gInt16* pArray = GetBlockDataAsRadial_Data(i,j,k);
					for (int z  = 0 ;z < radialhead->NumberOfGates ;z++)
					{				
						if (pArray[z] != -32768 && pArray[z] != -32767)
						{
							if (pArray[z] == -32766)
							{
							}
							else
							{
								if ((float)pArray[z]/blok->ScaleOfData > dbzMax)
								{  									
									dbzMax = (float)pArray[z]/blok->ScaleOfData;									
								}
							}
						}
					}
				}
			}
			else if (dblock->TypeOfDataPackets == BLOCKTYPE_GRID)
			{
				GridHead* pgridhead = GetBlockDataAsGrid_Head(i,j);

				for (int ii = 0; ii< pgridhead->NumberOfRows ;ii++)
				{					
					gInt16 *arr = GetBlockDataAsGrid_Data(i,j,ii);

					for (int jj = 0 ;jj < pgridhead->NumberOfCols ; jj++)
					{						
						//float ftemp =((float)(arr[jj])/(float)(blok->ScaleOfData));   //delete by haodemei 20090206
						float ftemp;     //AS by haodemei 20090206
						if(productCode == 294)       
						{
							ftemp =(float)(arr[jj]);
						}
						else
						{
							if(productCode == 301)
							{
								if((arr[jj]!=-32768)&&(arr[jj]!=-32767)&&(arr[jj]!=-32766)&&(arr[jj]!=-32765))
								{
									if((arr[jj]>=-2700)&&(arr[jj]<=2700)) //正常数据不需处理
									{
										ftemp =((float)(arr[jj])/(float)(blok->ScaleOfData));
									}
									else if(arr[jj] <-15000)
									{
										arr[jj] = arr[jj] +15000;
										ftemp =((float)(arr[jj])/(float)(blok->ScaleOfData));
									}
									else if(arr[jj] > 15000)
									{
										arr[jj] = arr[jj] -15000;
										ftemp =((float)(arr[jj])/(float)(blok->ScaleOfData));
									}
									else
									{
										ftemp =((float)(arr[jj])/(float)(blok->ScaleOfData));
									}									
								}
								else
								{
									ftemp =((float)(arr[jj])/(float)(blok->ScaleOfData));
								}
							}
							else
							{
								ftemp =((float)(arr[jj])/(float)(blok->ScaleOfData));
							}							
						}                //ES by haodemei 20090206

						if (ftemp > dbzMax)
						{
							dbzMax = ftemp;
						}
					}
				}
			}
		}

		if (blok->ProductCode != 280)
		{
			break;
		}
	}

	return dbzMax;
};//add by ljg 20080825 for functions recharge.

int cDrawProduct::GetWight(int & wight,/*int & realwight,*/int requestLayer/* = -1 */)
{
	wight = 0;
	PDBlock* blok = GetPDBlock();
	if(blok == NULL)
	{
		return wight;
	}
	int numberOfLayer = blok->NumberOfLayers;
	if (requestLayer<0)
	{
		gInt16 nProductcode = 0;
		GetProductCode(nProductcode);
		if (nProductcode==258)
		{
			requestLayer = numberOfLayer/2;
		}
		else
		{
			requestLayer = 0;
		}
	}


	for (int i = requestLayer ; i < blok->NumberOfLayers ; i++)
	{
		DILayer* layer = GetLayer(i);
		if(layer == NULL)
			return wight;
		for (int j = 0;j < layer->NumberOfBlocks ; j++)
		{
			DLIBlock* dblock = GetBlock(i,j);
			if (dblock ==NULL)
			{
				return wight;
			}
			if(dblock->TypeOfDataPackets == BLOCKTYPE_RADIAL)
			{
				RadialHead* radialhead = GetBlockDataAsRadial_Head(i,j);
				wight = radialhead->NumberOfGates*radialhead->LengthOfGate +radialhead->DistanceOfFirstGate;	

			}
			else if (dblock->TypeOfDataPackets == BLOCKTYPE_GRID)
			{
				GridHead* pgridhead = GetBlockDataAsGrid_Head(i,j);

				if((pgridhead->ResolutionOfRow <= 0) || (pgridhead->ResolutionOfCol <= 0)) //有问题的产品不打开 by hdm
				{
					return -111;					
				}
				double nwidth = pgridhead->ResolutionOfRow/pgridhead->ResolutionOfRow;//modified by Limh for 修改原来谁画图的低级错误int改double 20090313
				double nheight = pgridhead->ResolutionOfCol/pgridhead->ResolutionOfCol;//modified by Limh for 修改原来谁画图的低级错误int改double 20090313
				int ws = nwidth*(pgridhead->NumberOfRows)/2.0;
				int wh = nheight*(pgridhead->NumberOfCols)/2.0;
				if (ws> wh)
				{
					wight = wh *pgridhead->ResolutionOfCol;
					//realwight = wh;
				}
				else
				{
					wight = ws *pgridhead->ResolutionOfRow;
					//realwight = ws;
				}
			}
			else if (dblock->TypeOfDataPackets == BLOCKTYPE_STRUCT)
			{				
				if((blok->ProductCode == 278) || (blok->ProductCode == 281) || (blok->ProductCode == 292) || (blok->ProductCode == 293)|| (blok->ProductCode == 295)|| (blok->ProductCode == 296))//画SCT产品//by ljg 20080807,293龙卷涡旋
				{
					wight=blok->ProductDependent[1]*1000.0;
				}
			}
		}
		break;
	}
	return wight;
};//add by ljg 20080825 for functions recharge.

void cDrawProduct::GetGeo(float * geo,int requestLayer/* = 0 */)
{
	RadarInfor * pRadarInfo = GetRadarInfo();
	PDBlock* blok = GetPDBlock();
	geo[0] = pRadarInfo->Latitude;     //中心经度
	geo[1] = pRadarInfo->Longitude;
	geo[2] = pRadarInfo->Altitude;
};//add by ljg 20080825 for functions recharge.

void cDrawProduct::GetDist(int * dist,int requestLayer/* = 0 */,int width)
{
	if(requestLayer<0)
	{
		requestLayer=0;
	}	
	//MS by ljg 20080826 for improve speed.
	CProductDrawType nType=GetTypeOfProduct();
	//PDBlock* blok = GetPDBlock();
	//if(requestLayer<blok->NumberOfLayers)
	{
		DLIBlock* dblock = GetBlock(requestLayer,0);
		if(dblock == NULL)
		{
			return;
		}
		if(dblock->TypeOfDataPackets == BLOCKTYPE_RADIAL)
		{
			RadialHead* radialhead = GetBlockDataAsRadial_Head(requestLayer,0); 
			int nR = (radialhead->NumberOfGates	* radialhead->LengthOfGate + radialhead->DistanceOfFirstGate);
			for(int i=0;i<4;i++)
			{
				dist[i]=nR;
			}
		}
		else if (dblock->TypeOfDataPackets == BLOCKTYPE_GRID)
		{
			GridHead* pgridhead = GetBlockDataAsGrid_Head(requestLayer,0);
			double row_width = pgridhead->ResolutionOfRow*pgridhead->NumberOfRows*0.5;
			double col_width = pgridhead->ResolutionOfCol*pgridhead->NumberOfCols*0.5;
			dist[0] = row_width;
			dist[1] = col_width;
			dist[2] = row_width;
			dist[3] = col_width;
		}
		else if (dblock->TypeOfDataPackets == BLOCKTYPE_STRUCT)
		{
			GridHead* pgridhead = GetBlockDataAsGrid_Head(requestLayer,0);
			double row_width = width*0.5*1000.0;
			for (int i = 0;i < 4; i++)
			{
				dist[i] = row_width;
			}
		}
	}
	//ME by ljg 20080826 for improve speed.
};//add by ljg 20080825 for functions recharge.

CProductDraw* cDrawProduct::GetInstanceOfProductDraw( int nProductCode )
{
	CProductDraw* p=NULL;
// 	switch(nProductCode)
// 	{
// 	case 268:
// 		p=new CProductStructDraw_VAD;   //added by hdm 20090211
// 		break;
// 	case 269:
// 		p=new CProductStructDraw_VWP;
// 		break;
// 	case 278:
// 		p=new CProductStructDraw_SCT;
// 		break;
// 	case 280:
// 		p=new CProductStructDraw_LRM;
// 		break;
// 	case 281:
// 		p=new CProductStructDraw_SBT;
// 		break;
// 	case 285:
// 		p=new CProductStructDraw_EVP;
// 		break;
// 	case 288:
// 		p=new CProductStructDraw_SWP;
// 		break;
// 	case 291:
// 		p=new CProductStructDraw_HI;
// 		break;
// 	case 292:
// 		p=new CProductStructDraw_M;
// 		break;
// 	case 295:
// 		p=new CProductStructDraw_CL;
// 		break;
// 	case 296:
// 		p=new CProductStructDraw_TITAN;
// 		break;
// 	case 293:
// 		p=new CProductStructDraw_TVS;
// 		break;
// 	default:
// 		p=NULL;
// 		break;
//	}
	return p;

}

void cDrawProduct::ReleaseInstanceOfProductDraw( CProductDraw* lpInstance )
{
	CProductDraw* p=lpInstance;
	if(p!=NULL)
	{
		delete p;
		p=NULL;
	}
}

void cDrawProduct::GetDBBlockDate( double angle, double distance,double &data,double &averageData ,int requestLayer /*= 0*/ )
{
	double temp  = angle*100.00;
	int temangele = (int)temp;


	PDBlock* blok = GetPDBlock();	                   //得到产品数据头/产品信息

	//判断指针正确性
	if ( blok==NULL )
	{
		data = -32768;                                           //显示中的第一个参数
		averageData = -32768;                                    //显示中的第二个参数
		return;
	}

	   //DS by Limh for 修改文件结构把多个仰角文件存为一个文件 20080827
	//for (int i = 0 ; i < blok->NumberOfLayers ; i++)           //根据  数据层个数 得到数据层
	//{
	//int i = requestLayer;
	   //DE by Limh for 修改文件结构把多个仰角文件存为一个文件 20080827
		DILayer* layer = GetLayer(requestLayer);	                    //   得到数据层	

		//判断指针正确性
		if ( layer==NULL )
		{
			data = -32768;                                           //显示中的第一个参数
			averageData = -32768;                                    //显示中的第二个参数
			return;
		}
		
		//double cosElevation=1;
		//if ( layer->HeightOfElevation == -1 )            //    ElevationNumber;		//仰角序号//从零开始的仰角序号，针对单仰角的产品有效	
		//{
		//	cosElevation = 1;
		//}
		//else
		//{
		//	cosElevation =cos(layer->HeightOfElevation/100.0 * M_PI / 180);
		//}

		//distance = distance * cosElevation;

		for (int j = 0;j < layer->NumberOfBlocks ; j++)         //根据  //数据层中的层数 得到 数据块
		{
			DLIBlock* dblock = GetBlock(requestLayer,j);                 //得到 数据块

			//判断指针正确性
			if ( dblock==NULL )
			{
				data = -32768;                                           //显示中的第一个参数
				averageData = -32768;                                    //显示中的第二个参数
				return;
			}

			if(dblock->TypeOfDataPackets == BLOCKTYPE_RADIAL)        //判断   数据块类型 
			{
				RadialHead* radialhead = GetBlockDataAsRadial_Head(requestLayer,j); 

				//判断指针正确性
				if ( radialhead==NULL )
				{
					data = -32768;                                           //显示中的第一个参数
					averageData = -32768;                                    //显示中的第二个参数
					return;
				}
			//	RadialData * pFistRadialdata;  //去掉，未引用  李梅。06.25 


				for (int k = 0; k < radialhead->NumberOfRadials ; k++)//        //得到雷达头
				{
					RadialData* radialdata = GetBlockDataAsRadial_RadialData(requestLayer,j,k);

					//判断指针正确性
					if ( radialdata==NULL )
					{
						data = -32768;                                           //显示中的第一个参数
						averageData = -32768;                                    //显示中的第二个参数
						return;
					}

					if ( radialdata->AzimuthAngle == temangele )      //当相位角刚好等于其 第k个 径向角时
					{

						//double r1=radialhead->IndexOfFirstRangeBin/radialhead->LengthOfGate;   
						double r1=radialhead->DistanceOfFirstGate;
						//第一距离库的位置（米）    
						double r2=radialhead->DistanceOfFirstGate;	   //第二距离库的位置（米） 也是后面所有的  距离库的位置 

						for (int z  = 0 ;z < radialhead->NumberOfGates ;z++)
						{
							r2 =(r1 + z*radialhead->LengthOfGate);//*cosElevation;
							if (distance == r2 )                  //当恰好在结点上时
							{
								gInt16* pArray = GetBlockDataAsRadial_Data(requestLayer,j,k );          //得到径向上所有的 结点 //注意为什么是 参数变化为 k

									//判断指针正确性
								if ( pArray==NULL )
								{
									data = -32768;                                           //显示中的第一个参数
									averageData = -32768;                                    //显示中的第二个参数
									return;
								}						

								data = (double)(pArray[z])/blok->ScaleOfData;                                           //显示中的第一个参数
								averageData = (double)(pArray[z])/blok->ScaleOfData;                                    //显示中的第二个参数
								return;
							}
							else
							{
								//MessageBox(wxT("error"));
								//if ( r2 >distance )
								//{
								//	gInt16* pArray = GetBlockDataAsRadial_Data(i,j,k );     //注意为什么是 参数变化为 k
								//	data = pArray[z];                                  //显示中的第一个参数
								//	averageData = ( pArray[z]+pArray[z-1])/2;        //显示中的第二个参数
								//	return;
								//}
								//else
								continue;

							}
						}
					}

					if ( radialdata->AzimuthAngle < temangele )     //当第k个 径向角 小于 相位角时 接着下找 
					{
						//pFistRadialdata = radialdata;
						//pFistRadialdata =  GetBlockDataAsRadial_RadialData(i,j,k);
						continue;
					}
					else                      //找到是在第 z-1 与第 z 根径向之间     
					{
						double r1=radialhead->DistanceOfFirstGate;     
						//第一距离库的位置（米）    
						double r2=radialhead->DistanceOfFirstGate;	   //第二距离库的位置（米） 也是后面所有的  距离库的位置 

						for (int z  = 0 ;z < radialhead->NumberOfGates ;z++)       //一个径向上数据个数
						{

							r2 =(r1 + z*radialhead->LengthOfGate);//*cosElevation;

							//这下面的是否在考虑 pArray[nPlace]为-32768

							if (distance == r2 )                  //当恰好在结点上时
							{
								gInt16* pArray;
								gInt16 *pArrayTwo;
								if ( k == 0)
								{
									pArray = GetBlockDataAsRadial_Data(requestLayer,j,k );  //注意为什么是 参数变化为 k-1
									pArrayTwo = GetBlockDataAsRadial_Data(requestLayer,j,k );  //注意为什么是 参数变化为 k
								}
								else
								{
									pArray = GetBlockDataAsRadial_Data(requestLayer,j,k-1 );  //注意为什么是 参数变化为 k-1
									pArrayTwo = GetBlockDataAsRadial_Data(requestLayer,j,k );  //注意为什么是 参数变化为 k
								}

								if ( pArray==NULL || pArrayTwo==NULL )
								{
									data =  -32768;                                           //显示中的第一个参数
									averageData = -32768;                                    //显示中的第二个参数
									return;
								}

								if (pArray[z] == -32768 && pArrayTwo[z]==-32768)
								{
									data =  -32768;                                           //显示中的第一个参数
									averageData = -32768;                                    //显示中的第二个参数
									return;
								}
								data = (double)(pArray[z])/blok->ScaleOfData;                                           //显示中的第一个参数
								averageData = (double)(pArrayTwo[z])/blok->ScaleOfData;                                    //显示中的第二个参数
								return;
							}
							else
							{
								if ( r2 >distance )
								{
									//gInt16* pArray = GetBlockDataAsRadial_Data(i,j,k-1 );     //注意为什么是 参数变化为 k
									//data = pArray[z];                                  //显示中的第一个参数
									//averageData = ( pArray[z]+pArray[z-1])/2;        //显示中的第二个参数
									gInt16* pArray;
									gInt16 *pArrayTwo;
									if ( k == 0)
									{
										pArray = GetBlockDataAsRadial_Data(requestLayer,j,k );  //注意为什么是 参数变化为 k-1
										pArrayTwo = GetBlockDataAsRadial_Data(requestLayer,j,k );  //注意为什么是 参数变化为 k
									}
									else
									{
										pArray = GetBlockDataAsRadial_Data(requestLayer,j,k-1 );  //注意为什么是 参数变化为 k-1
										pArrayTwo = GetBlockDataAsRadial_Data(requestLayer,j,k );  //注意为什么是 参数变化为 k
									}

									if ( pArray==NULL || pArrayTwo==NULL )
									{
										data =  -32768;                                           //显示中的第一个参数
										averageData = -32768;                                    //显示中的第二个参数
										return;
									}


									if (pArray[z] == -32768 && pArrayTwo[z]==-32768)
									{
										data = -32768;                                           //显示中的第一个参数
										averageData = -32768;                                    //显示中的第二个参数
										return;
									}
									//AS by Limh for 当DBZ值为-32766时图中有颜色 20080619
									if ( pArray[z] == -32766)
									{
										data = -32766;                                           //显示中的第一个参数
										averageData = -32766;                                    //显示中的第二个参数
										return;
									}
									//AE by Limh for 当DBZ值为-32766时图中有颜色 20080619
									data = (double)(pArray[z])/blok->ScaleOfData;       //显示中的第一个参数    注意这里为nPlace 还是 nPlace+1
									if ( z == 0)
									{
										averageData = (double)(( pArray[z]+pArray[z]))/blok->ScaleOfData*0.5;     //显示中的第二个参数
										return;

									}
									else
									{
										averageData = (double)(( pArray[z]+pArray[z-1] +pArrayTwo[z] +pArrayTwo[z-1] ))/blok->ScaleOfData*0.25;     //显示中的第二个参数
										return;
									}

								}
								else
								{
									continue;
								}

							}

						}
					}
				}
			}
			else  //AS by Limh for 栅格产品/组网产品时后面的值的求法 20080613
			{
				
				GridHead* pgridhead = GetBlockDataAsGrid_Head(requestLayer,j);

				if ( pgridhead == NULL )
				{
					data = -32766;                                           //显示中的第一个参数
					averageData = -32766;                                    //显示中的第二个参数
					return;
				}

				int nwidth = pgridhead->ResolutionOfRow/pgridhead->ResolutionOfCol;
				int nheight = pgridhead->ResolutionOfCol/pgridhead->ResolutionOfCol;

				double dx = 0.0;
				double dy = 0.0;
				if (  ((double)0) == angle)
				{
					dx = (pgridhead->NumberOfCols) * (pgridhead->ResolutionOfCol ) /2.0;
					dy = (pgridhead->NumberOfRows) * (pgridhead->ResolutionOfRow ) /2.0 - distance;
				}
				else if ( ((double)90) == angle )
				{
					dx = (pgridhead->NumberOfCols) * (pgridhead->ResolutionOfCol ) /2.0 + distance;
					dy = (pgridhead->NumberOfRows) * (pgridhead->ResolutionOfRow ) /2.0;
				}
				else if ( ((double)180) == angle )
				{
					dx = (pgridhead->NumberOfCols) * (pgridhead->ResolutionOfCol ) /2.0;
					dy = (pgridhead->NumberOfRows) * (pgridhead->ResolutionOfRow ) /2.0 - distance;
				}
				else if (  ((double)270) == angle)
				{
					dx = (pgridhead->NumberOfCols) * (pgridhead->ResolutionOfCol ) /2.0 - distance;
					dy = (pgridhead->NumberOfRows) * (pgridhead->ResolutionOfRow ) /2.0 ;
				}
				else
				{
					//double angleTemp =  angle*M_PI/180;
					dx = (pgridhead->NumberOfCols) * (pgridhead->ResolutionOfCol ) /2.0 + distance * sin( angle*M_PI/180 );
					dy = (pgridhead->NumberOfRows) * (pgridhead->ResolutionOfRow ) /2.0  - distance * cos( angle*M_PI/180 );
				}
				
				int nCol = dx / ( pgridhead->ResolutionOfCol );
				int nRow = dy / ( pgridhead->ResolutionOfRow );
				if ( (nRow < 0 ) || (nCol < 0) || (nRow >= pgridhead->NumberOfRows) || (nCol >= pgridhead->NumberOfCols) )
				{
					data = -32768;
					averageData = -32768;  
					return;
				}
				gInt16 *arr = GetBlockDataAsGrid_Data(requestLayer,j,nRow);

                if ( arr==NULL )
                {
					data = -32768;                                           //显示中的第一个参数
					averageData = -32768;                                    //显示中的第二个参数
					return;
                }

				//AS by Limh for 当DBZ值为-32766时图中有颜色 20080619
				if ( arr[nCol] == -32766)
				{
					data = -32766;                                           //显示中的第一个参数
					averageData = -32766;                                    //显示中的第二个参数
					return;
				}
				//AE by Limh for 当DBZ值为-32766时图中有颜色 20080619

				data = (double)(arr[nCol])/blok->ScaleOfData;                                           //显示中的第一个参数

				averageData = -32768;                                    //显示中的第二个参数
			 }
		} //AE by Limh for 栅格产品/组网产品时后面的值的求法 20080613
	//}  //deleted by Limh for 修改文件结构把多个仰角文件存为一个文件 20080827

}

void cDrawProduct::ReadInfo( vector<vec_int>& theArray, int& num_aziumth, int& num_gate, gInt32& elevation, int& height,int requestLayer,int &len_gate,int bmpHei,float zoom,double *rate )
{
	RadarInfor * pRadarInfo = GetRadarInfo();
	PDBlock* blok = GetPDBlock();
	if(blok == NULL)
		return;

	gInt16 scaleOfProd = blok->ScaleOfData;
	//height = (int)pRadarInfo->Altitude;  //雷达高度，added by limei ,08.01
	height = (int)pRadarInfo->Altitude/10;  //雷达高度，added by limei ,08.01//modify by ljg 20081224 for 这三个数据单位有变化。高度单位0.1--〉0.01
	DILayer* layer = GetLayer(requestLayer);  // 解析当前层，modified by limei,08.21
	if(layer == NULL)
		return;
	elevation = layer->HeightOfElevation;

	for (int j = 0;j < layer->NumberOfBlocks ; j++)
	{
		DLIBlock* dblock = GetBlock(requestLayer,j);

		if(dblock->TypeOfDataPackets == BLOCKTYPE_RADIAL)
		{
			RadialHead* radialhead = GetBlockDataAsRadial_Head(requestLayer,j); 

			gInt16 num_radial = radialhead->NumberOfRadials;
			num_gate = radialhead->NumberOfGates;
			len_gate = radialhead->LengthOfGate;   //添加距离库长度，limei,1209
			num_aziumth = num_radial;

			int wight = num_gate*len_gate +radialhead->DistanceOfFirstGate;
			rate[requestLayer] = 2.0*(double)wight/(double)bmpHei/zoom; //每个像素点表示的实现长度

			for (int k = 0; k < num_radial ; k++)
			{
				RadialData* radialdata = GetBlockDataAsRadial_RadialData(requestLayer,j,k);
				gInt16* pArray = GetBlockDataAsRadial_Data(requestLayer,j,k);
				vec_int vecCol;
				for (int z=0; z<num_gate; z++)
				{
					if(pArray[z] == -32768 || pArray[z] == -32767 || pArray[z] == -32766)
					{
						vecCol.push_back(pArray[z]);
					}
					else
					{
						vecCol.push_back(pArray[z]/scaleOfProd);
					}
				}

				theArray.push_back(vecCol);

			}

		}
	}
}

//void OutLineDebug(char* cKey, char* cValue)
//{
//	OutputDebugString("\r\n");
//	OutputDebugString(cKey);
//	OutputDebugString(" : ");
//	OutputDebugString(cValue);
//	OutputDebugString("\r\n");
//}
//
//void OutLineDebug(char* cKey, int iValue)
//{
//	OutputDebugString("\r\n");
//	char numberBuffer[1024] = {0};
//	OutputDebugString(cKey);
//	OutputDebugString(" : ");
//	memset(numberBuffer, sizeof(numberBuffer), 0);
//	_itoa_s(iValue, numberBuffer, sizeof(numberBuffer), 16);
//	OutputDebugString(numberBuffer);
//	OutputDebugString("\r\n");
//}
//
//void OutLineDebug(char* cKey, double dVlaue)
//{
//	int dect(0), sign(0);
//	char numberBuffer[1024] = {0};
//	OutputDebugString("\r\n");
//	OutputDebugString(cKey);
//	OutputDebugString(" : ");
//	_fcvt_s(numberBuffer, sizeof(numberBuffer), dVlaue, 5, &dect, &sign);
//	OutputDebugString(numberBuffer);
//	OutputDebugString("\r\n");
//}

bool cDrawProduct::ReadInfo( proAttr *pAttributeParm, int nLayer /*= -1*/ )
{
	bool bRet=false;
	if(this!=NULL&&GetTypeOfProduct()!=enProductUnkown)
	{
		RadarInfor * pRadarInfo = GetRadarInfo();
		PDBlock* blok = GetPDBlock(); 
		int proCode = (int)blok->ProductCode;
		int numberOfLayers = (int)blok->NumberOfLayers;

		if(nLayer<0)
		{
			if (proCode == 258 || proCode == 501)
			{
				nLayer = numberOfLayers/2;
			}
			else 
				nLayer=0;
		}
		if(blok!=NULL)
		{
			DILayer* layer = GetLayer(nLayer);
			if(layer!=NULL)
			{
				if (proCode == 258 || proCode == 278 || proCode == 281 || proCode == 296) //产品加不上经纬网格的话，问题一般出在这里。
				{
					pAttributeParm->elevation = 0;
				}
				else
				{
					pAttributeParm->elevation = layer->HeightOfElevation*0.01;
				}
			}
			if(layer!=NULL)
			{
				char * achar=pRadarInfo->SiteCode;
				string str = string(achar,/*wxConvUTF8,*/sizeof(pRadarInfo->SiteCode));

				string relsitecode = TransToRadarCode5(str);//add by ljg 20090112 for radarcode changes.
				memcpy_s(pAttributeParm->site_code, 255, relsitecode.data(), relsitecode.length());
				//pAttr->staName = CommonMethod::FindRadarName(pAttr->site_code);

				pAttributeParm->pro_code =proCode;
				pAttributeParm->scan_date = (int)blok->ProductDate;
				pAttributeParm->scan_time = (int)blok->ProductTime;
				pAttributeParm->generation_date = (int)blok->GenerationDateOfProduct;
				pAttributeParm->generation_time = (int)blok->GenerationTimeOfProduct;
				pAttributeParm->altitude = (int)pRadarInfo->Altitude/10;//modify by ljg 20081224 for 这三个数据单位有变化。高度单位0.1--〉0.01
				pAttributeParm->longitude = (int)pRadarInfo->Longitude*10;//经度单位0.0001--〉0.001
				pAttributeParm->latitude = (int)pRadarInfo->Latitude*10;//纬度单位0.0001--〉0.001

				pAttributeParm->operation_mode = TransToOperationalModeInt(blok->OperationalMode);
				pAttributeParm->vcp_mode = TransToVCPsn(blok->VCPMode); //modify by ljg 20081224 for VCP-->VCPMode, trans back it.  
				pAttributeParm->numberoflayers = numberOfLayers;

				if (pAttributeParm->pro_code == 254 || pAttributeParm->pro_code == 255 || pAttributeParm->pro_code == 301) //301-IW-逆风区
				{
					pAttributeParm->param1= int (double(blok->ProductDependent[1])*0.01);//暂时算成是速度退模糊的参数
					pAttributeParm->param2 = blok->ProductDependent[0];

				}
				else
				{
					pAttributeParm->param1= 50;
					pAttributeParm->param2 = 0;
				}
				if (pAttributeParm->pro_code == 258 )
				{
					pAttributeParm->HeightOfElevation = layer->HeightOfElevation*10 + pAttributeParm->altitude; //加上雷达高度 0.1m
				}
				else if (pAttributeParm->pro_code == 268)
				{
					pAttributeParm->HeightOfElevation = layer->HeightOfElevation*100 + pAttributeParm->altitude; //加上雷达高度 0.1m
				}
				else
				{
					pAttributeParm->HeightOfElevation = 0;
				}

				if(pAttributeParm->pro_code==278)
				{
					//CProductDraw_SCT pds;
					//pAttr->HeightOfElevation = pds.GetCountOfStormCells(this);
				}


				bRet=true;
			}
		}
	}
	return bRet;
}

void * cDrawProduct::ReadStructData()
{
	PDBlock* blok = GetPDBlock();
	DILayer* layer = GetLayer(0);
	RadialHead* radialhead = GetBlockDataAsRadial_Head(0,0);
	void* p = GetBlockDataAsStruct_Data(0, 0);
	return p;
}

void cDrawProduct::ReadInformaton( int Layer, int &Elevation,int &NyquistVelocity, int &Azi_Num,int &Gate_Num, int &Num )
{
	RadarInfor * pRadarInfo = GetRadarInfo();
	PDBlock* blok = GetPDBlock();
	if(blok == NULL)
	{
		return;
	}
	DILayer* layer = GetLayer(Layer);  // 解析当前层，modified by limei,08.21
	if(layer == NULL)
	{
		return;
	}
	Elevation = layer->HeightOfElevation;
	Num = blok->ProductDependent[0];
	for (int j = 0;j < layer->NumberOfBlocks ; j++)
	{
		DLIBlock* dblock = GetBlock(Layer,j);

		if(dblock->TypeOfDataPackets == BLOCKTYPE_RADIAL)
		{
			RadialHead* radialhead = GetBlockDataAsRadial_Head(Layer,j); 
			gInt16 num_radial = radialhead->NumberOfRadials;
			Gate_Num = radialhead->NumberOfGates;
			Azi_Num = num_radial;
			NyquistVelocity =  blok->ProductDependent[1];
		}
	}
}

gInt16* cDrawProduct::GetDataV ( int ElevationIndex, int AzimuthIndex )//add by yangys
{
	RadarInfor * pRadarInfo = GetRadarInfo();
	PDBlock* blok = GetPDBlock();
	gInt16* pArray; 

	DILayer* layer = GetLayer(ElevationIndex);
	for (int j = 0;j < layer->NumberOfBlocks ; j++)
	{
		DLIBlock* dblock = GetBlock(ElevationIndex,j);

		if(dblock->TypeOfDataPackets == BLOCKTYPE_RADIAL)
		{
			pArray = GetBlockDataAsRadial_Data(ElevationIndex,j,AzimuthIndex);
		}
	}
	return pArray;
}

void cDrawProduct::SetPara(int NyquistVelocity,int Number )
{
	RadarInfor * pRadarInfo = GetRadarInfo();
	PDBlock* blok = GetPDBlock();
	if(blok == NULL)
	{
		return;
	}
	blok->ProductDependent[0] = Number;
	blok->ProductDependent[1] = NyquistVelocity;
}

void cDrawProduct::GetMaxValue( float * fValue,int layer )
{
	RadarInfor * pRadarInfo = GetRadarInfo();
	PDBlock* blok = GetPDBlock();
	if(blok == NULL)
	{
		return;
	}
	char * achar=pRadarInfo->SiteCode;
	//后面还有好多
}

void cDrawMultiProduct::ReadInfo( int* p,string& regionName ,string &regionSite, string &proName, string &proCName,int layer/*=-1*/ )
//	void cDrawMultiProduct::ReadInfo(int* p,wxString& regionName ,wxString &regionSite, wxString &proName, wxString &proCName,int layer)
{
	AreaInfo * pAreaInfo = GetAreaInfoP();
	MultiDataHead * pMDblock = GetMultiHeadP();
	//MultiGridHead * pMGHead = (MultiGridHead *)GetBlockDataAsGrid_Head(0, 0);

	if(pMDblock == NULL)
	{
		return;
	}

	regionSite = string(pAreaInfo->AreaCode/*, wxConvLibc*/);
	regionName = TransToRadarName(regionSite);
	//regionName = CommonMethod::FindRadarName(regionSite, arrayNum, arrayName);

	p[0] = (int)pMDblock->ProductCode;
	p[1] = (int)pMDblock->ProductDate;
	p[2] = (int)pMDblock->ProductTime;
	p[3] = (int)pMDblock->GenerationDateOfProduct;
	p[4] = (int)pMDblock->GenerationTimeOfProduct;
	p[5] = (int)pAreaInfo->LatitudeOfNorth;
	p[6] = (int)pAreaInfo->LatitudeOfSouth;
	p[7] = (int)pAreaInfo->LongitudeOfWest;
	p[8] = (int)pAreaInfo->LongitudeOfEast;
	p[9] = (int)pAreaInfo->NumberOfRadars;

	//modify by ljg 20090616
	if(GetTypeOfProduct()==enProductGrid)
	{
		MultiGridHead * pMGHead = (MultiGridHead *)GetBlockDataAsGrid_Head(0, 0);
		p[10] = (int)pMGHead->ResolutionOfRow;
		p[11] = (int)pMGHead->ResolutionOfCol;
	}
	else
	{
		p[10] = 0;
		p[11] = 0;
	}

	//if (p[0] == 501&&layer== -1)   ///M by yangys for如果没有传递值，默认是中间层高度
	//{
	//	MultiLayerHead *pLayer = GetLayer(pMDblock->NumberOfLayers/2);
	//	if(pLayer == NULL)
	//		return;
	//	p[12] = (int)pLayer->HeightOfElevation;
	//}
	//else   //否则得到参数指定的层数的高度
	//{
	//	if (layer == -1)
	//	{
	//          layer = 0;
	//	}
	//	MultiLayerHead *pLayer = GetLayer(layer); 
	//	if(pLayer == NULL)
	//		return;
	//	p[12] = (int)pLayer->HeightOfElevation;
	//}
	layer = 0;  //BREF单层保存，对于任何组网产品，都是得到layer=0的高度
	MultiLayerHead *pLayer = GetLayer(layer); 
	if(pLayer == NULL)
		return;
	p[12] = (int)pLayer->HeightOfElevation;
	p[13] = (int)pMDblock->NumberOfLayers;
}

void cDrawMultiProduct::GetDBBlockDate(int iCurLayer,double dLongitude, double dLatitude,double &data,double &averageData)
{
	AreaInfo * pAreaInfo = GetAreaInfoP();
	MultiDataHead *pMDblock = GetMultiHeadP();
	MultiLayerHead* player = GetLayerP(iCurLayer);    //modified by Limh for 根据翻页上的层数去取组网的层数原来是减了一个1 20080725
	MultiGridHead *pMGHead  = (MultiGridHead *)GetBlockDataAsGrid_Head(0, 0);
	//判断指针正确性
	if ( pMDblock==NULL || player==NULL )
	{
		data = -32768;                                           //显示中的第一个参数
		averageData = -32768;                                    //显示中的第二个参数
		return;
	}

	pAreaInfo->LatitudeOfSouth;
	pAreaInfo->LatitudeOfNorth;
	pAreaInfo->LongitudeOfWest;
	pAreaInfo->LongitudeOfEast;
	//double dOne = (pMGHead->ResolutionOfRow)/pMGHead->ResolutionOfRow;
	//double dTwo = (pMGHead->ResolutionOfCol)/pMGHead->ResolutionOfRow;
	double dOne = (pMGHead->ResolutionOfRow)*0.00001;
	double dTwo = (pMGHead->ResolutionOfCol)*0.00001;
	double dLat =  0.0001*( pAreaInfo->LatitudeOfNorth ) - dLatitude;
	double dLon =  dLongitude - ( 0.0001*(pAreaInfo->LongitudeOfWest) );
	//int iLatiData = (int) ( 10*dLat/dOne );
	//int iLongData = (int) ( 10*dLon/dTwo );
	int iLatiData = (int) ( dLat/dOne );
	int iLongData = (int) ( dLon/dTwo );

	gInt16 *arr = GetLineDataP(iCurLayer,iLatiData-1);

	if (arr==NULL)
	{
		data = -32768;                                           //显示中的第一个参数
		averageData = -32768;                                    //显示中的第二个参数
		return;
	}

	//AS by Limh for 当DBZ值为-32766时图中有颜色 20080619
	if ( arr[iLongData-1] == -32766)
	{
		data = -32766;                                           //显示中的第一个参数
		averageData = -32766;                                    //显示中的第二个参数
		return;
	}
	//AE by Limh for 当DBZ值为-32766时图中有颜色 20080619

	data = arr[iLongData-1] /(double)pMDblock->ScaleOfData;
	averageData = -32768;
}

string cDrawMultiProduct::TransToRadarName(string strRadarCode)
{
	//strRadarCode = 	TransToRadarCode5(strRadarCode);//add by ljg 20081223 for radarcode changes.

	string strRadarName = strRadarCode;     //由雷达号得到雷达名
// 	xmlSettings search_xml;
// 	if (!search_xml.LoadSettings(ID_AREA_CENTER))
// 	{
// 		wxMessageBox(_("Load xml files failed!"),_("Tips"));   
// 		return strRadarName;
// 	}
// 	int nTableNum = search_xml.GetTableCount();
// 	string RadarNum;
// 
// 	for(int i=1; i<= nTableNum; i++)
// 	{
// 		xmlTable xmlTab;
// 		if(!xmlTab.SetNode(search_xml.GetTable(i)))
// 		{
// 			return strRadarName;
// 		}
// 		else
// 		{
// 			int nCol = 0;
// 			int nRow = 0;
// 			wxString strName;
// 			if(!xmlTab.GetTableInfo(strName, nRow, nCol))
// 			{
// 				return strRadarName;
// 			}
// 
// 			bool bFind = false;
// 
// 			for(int j=1; j<=nRow; j++)
// 			{
// 				xmlTab.GetTableItem(RadarNum, j,1);
// 
// 				if (RadarNum.compare(strRadarCode) == 0)
// 				{	
// 					xmlTab.GetTableItem(strRadarName, j, 2);
// 
// 					bFind = true;
// 					return strRadarName;					
// 				}
// 			}			
// 		}
// 	}

	return strRadarName;
};

CProductDrawType cDrawMultiProduct::GetTypeOfProduct()
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
};//判断产品绘图类型//add by ljg 20090219 for functions recharge.

void cDrawMultiProduct::GetRange(int *platitude_longitude_array)
{
	AreaInfo * pAreaInfo = GetAreaInfoP();
	if(pAreaInfo)
	{
		platitude_longitude_array[0] = pAreaInfo->LongitudeOfEast;
		platitude_longitude_array[1] = pAreaInfo->LatitudeOfSouth;
		platitude_longitude_array[2] = pAreaInfo->LongitudeOfWest;
		platitude_longitude_array[3] = pAreaInfo->LatitudeOfNorth;
	}

};//add by ljg 20090219 for 获取产品覆盖范围，四角经纬度 

void cDrawMultiProduct::GetMax_MLRM(float &iMaxValue)
{	
	MultiDataHead *pMDblock = GetMultiHeadP();
	if (pMDblock == NULL)
	{
		return;
	}
	int iScaleOfData = pMDblock->ScaleOfData;
	int iNumberOfLayers = pMDblock->NumberOfLayers;

	MultiLayerHead* layer = GetLayerP(0);
	if(layer == NULL)
	{
		return;
	}
	int iNumberOfBlocks = layer->NumberOfBlocks;

	MultiGridHead *pMGHead  = (MultiGridHead *)GetBlockDataAsGrid_Head(0, 0);
	if(pMDblock == NULL)
	{
		return;
	}	
	int iNumberOfRows = pMGHead->NumberOfRows;
	int iNumberOfCols = pMGHead->NumberOfCols;	

	for(int i=0; i<iNumberOfLayers; i++)
	{
		for (int j = 0; j<iNumberOfBlocks; j++)
		{
			for (int ii = 0; ii<iNumberOfRows; ii++)
			{
				gInt16  *arr = (gInt16 *)GetBlockDataAsGrid_Data(i, j, ii);

				for (int jj=0; jj<iNumberOfCols; jj++)
				{					
					float ftemp =((float)(arr[jj])/(float)(iScaleOfData));	

					if (iMaxValue < ftemp)
					{
						iMaxValue = ftemp;
					}

				}
			}			
		}	
	}	
}

float cDrawMultiProduct::GetMaxValue(int Layer)
{

	float iMaxValue = 0.0;
	MultiDataHead *pMDblock = GetMultiHeadP();
	if (pMDblock == NULL)
	{
		return 0;
	}	
	int nProductCode = (int)pMDblock->ProductCode;
	int numberOfLayer = (int)pMDblock->NumberOfLayers;
	if (nProductCode == 501)
	{
		Layer = 0;
	}
	if ( 506==nProductCode )
	{
		this->GetMax_MLRM(iMaxValue);
		return iMaxValue;
	}

	if (Layer==-1)
	{
		if(nProductCode==501)
		{
			Layer = numberOfLayer%2==0?numberOfLayer/2:(numberOfLayer/2+1);
		}
		else
		{
			Layer = 0;
		}
	}

	int iScaleOfData = pMDblock->ScaleOfData;
	MultiLayerHead* layer = GetLayerP(Layer);
	if(layer == NULL)
	{
		return 0;
	}
	int iNumberOfBlocks = layer->NumberOfBlocks;

	MultiGridHead *pMGHead  = (MultiGridHead *)GetBlockDataAsGrid_Head(0, 0);
	if(pMGHead == NULL)
	{
		return 0; 
	}	
	int iNumberOfRows = pMGHead->NumberOfRows;
	int iNumberOfCols = pMGHead->NumberOfCols;	

	for (int j = 0; j<iNumberOfBlocks; j++)
	{
		for (int ii = 0; ii<iNumberOfRows; ii++)
		{
			gInt16  *arr = (gInt16 *)GetBlockDataAsGrid_Data(Layer, j, ii);

			for (int jj=0; jj<iNumberOfCols; jj++)
			{					
				float ftemp =((float)(arr[jj])/(float)(iScaleOfData));	

				if (iMaxValue < ftemp)
				{
					iMaxValue = ftemp;
				}

			}
		}			
	}	
	return iMaxValue;
}

void cDrawMultiProduct::OnGetDiagramParametre(std::vector<vec_int> & theArray,int memberofcn, float cn[][2], double *dGateOrReso, int requestLayer)
{
	MultiDataHead *pMDblock = GetMultiHeadP();
	MultiGridHead *pMGHead = (MultiGridHead *)GetBlockDataAsGrid_Head(0, 0);
	dGateOrReso[0] = pMGHead->ResolutionOfRow/pMGHead->ResolutionOfRow;
	dGateOrReso[1] = pMGHead->ResolutionOfCol/pMGHead->ResolutionOfCol;
	gInt16 nProductcode = pMDblock->ProductCode;

	if (requestLayer == -1)
	{
		if (nProductcode == 501)
		{
			requestLayer = pMDblock->NumberOfLayers/2;
		}
		else
		{
			requestLayer = 0;
		}
	}

	if (nProductcode == 501)
	{
		requestLayer = 0;
	}

	MultiLayerHead* layer = GetLayerP(requestLayer);	

	for (int ii = 0;ii < pMGHead->NumberOfRows ; ii++)
	{	
		gInt16 *arr = GetLineDataP(requestLayer,ii);
		vec_int vecCol;

		for (int jj =0 ;jj <pMGHead->NumberOfCols ; jj++)
		{
			bool bIsIn = true;

			for (int h = 0 ; h < memberofcn ; h++)
			{
				float a = cn[h][0];
				float b = cn[h][1];
				float temp=((float)arr[jj])/pMDblock->ScaleOfData;

				if (temp>=a && temp<=b)
				{
					vecCol.push_back(h); //动态2维数组
					bIsIn = false;

					break;
				}
			}	

			if (bIsIn)
			{
				vecCol.push_back(16);
			}
		}
		theArray.push_back(vecCol);
	}
}	
//AE by wyh for 面积统计（获取组网产品颜色的跨度值）20081020

void cDrawMultiProduct::OnGetDiagramParametre(std::vector<vec_float> &theArray, int requestLayer)
{
	MultiDataHead *pMDblock = GetMultiHeadP();
	MultiGridHead *pMGHead = (MultiGridHead *)GetBlockDataAsGrid_Head(0, 0);

	if (requestLayer == -1)
	{
		gInt16 nProductcode = pMDblock->ProductCode;
		if (nProductcode == 501)
		{
			requestLayer = pMDblock->NumberOfLayers/2;
		}
		else
		{
			requestLayer = 0;
		}
	}

	MultiLayerHead* layer = GetLayerP(requestLayer);	

	for (int ii = 0;ii < pMGHead->NumberOfRows ; ii++)
	{	
		gInt16 *arr = GetLineDataP(requestLayer,ii);
		vec_float vecCol;

		for (int jj =0 ;jj <pMGHead->NumberOfCols ; jj++)
		{			
			float temp = ((float)arr[jj])/pMDblock->ScaleOfData;
			vecCol.push_back(temp);			
		}
		theArray.push_back(vecCol);
	}
}	
//遍历产品，用动态二维数组记录产品颜色的跨度值。
void cDrawProduct::OnGetDiagramParametre(std::vector<vec_int> & theArray,int memberofcn, float cn[][2], double *dGateOrResc, int requestLayer)
{
	PDBlock* blok = GetPDBlock();
	if(blok == NULL)
	{
		return;
	}
	if(requestLayer<0)
	{
		requestLayer=0;
	}

	DILayer* layer = GetLayer(requestLayer);// 解析当前层
	if(layer == NULL)
	{
		return;
	}
	for (int j = 0;j < layer->NumberOfBlocks ; j++)
	{
		DLIBlock* dblock = GetBlock(requestLayer,j);
		if(dblock->TypeOfDataPackets == BLOCKTYPE_RADIAL) //径向产品
		{
			RadialHead* radialhead = GetBlockDataAsRadial_Head(requestLayer,j); 
			dGateOrResc[0] = (double)(radialhead->LengthOfGate)/1000;///radialhead->LengthOfGate;
			gInt16 num_radial=radialhead->NumberOfRadials;

			for (int k=0; k<num_radial; k++)
			{
				RadialData* radialdata = GetBlockDataAsRadial_RadialData(requestLayer,j,k);
				gInt16* pArray = GetBlockDataAsRadial_Data(requestLayer,j,k);
				vec_int vecCol;

				for (int z=0; z<radialhead->NumberOfGates; z++)
				{
					bool bIsIn = true;						

					for (int h = 0 ; h < memberofcn ; h++)
					{
						float a = cn[h][0];
						float b = cn[h][1];
						float temp=((float)pArray[z])/blok->ScaleOfData;	
						if (temp >= a && temp <= b )
						{
							vecCol.push_back(h); //动态2维数组
							bIsIn = false;

							break;
						}
					}			

					if (bIsIn)
					{
						vecCol.push_back(16); //压入数组的信息是16表示该点的信息是空的。
					}
				}
				theArray.push_back(vecCol);
			}
		}  
		else if (dblock->TypeOfDataPackets == BLOCKTYPE_GRID) //栅格产品
		{
			GridHead* pgridhead = GetBlockDataAsGrid_Head(requestLayer,j);
			dGateOrResc[0] = (pgridhead->ResolutionOfRow)/pgridhead->ResolutionOfRow;
			dGateOrResc[1] = (pgridhead->ResolutionOfCol)/pgridhead->ResolutionOfRow;

			for (int ii = 0; ii< pgridhead->NumberOfRows ;ii++)     
			{
				GridRowData* rowdata = GetBlockDataAsGrid_RowData(requestLayer,j,ii);
				vec_int vecCol;

				for (int jj = 0 ;jj < pgridhead->NumberOfCols ; jj++) 
				{					
					gInt16 *arr = GetBlockDataAsGrid_Data(requestLayer,j,ii);

					bool bIsIn = true;

					for (int h = 0 ; h < memberofcn ; h++)
					{
						float a = cn[h][0];
						float b = cn[h][1];
						float temp=((float)arr[jj])/blok->ScaleOfData;

						if (temp>=a && temp<=b)
						{
							vecCol.push_back(h); //动态2维数组
							bIsIn = false;

							break;
						}
					}	

					if (bIsIn)
					{
						vecCol.push_back(16);
					}
				}
				theArray.push_back(vecCol);
			}
		}
		else if (dblock->TypeOfDataPackets == BLOCKTYPE_STRUCT)
		{				

		}
	}
}

int cDrawProduct::GetProCode()
{
	PDBlock* blok = GetPDBlock();
	int numberOfLayer = blok->ProductCode;
	return numberOfLayer;
}
