#include "InterpolationAlgorithm.h"
#include "goQuadrilateral.h"
#include <QCoreApplication>
#include <QDate>
#include <QTime>
#include <math.h>
#include <Windows.h>
#define Rm 6371.004; //average value of the Earth radius

InterpolationAlgorithm::InterpolationAlgorithm()
{
	//struct_Data = NULL;
	m_produceDraw = NULL;
	m_xNum = 0;
	m_yNum = 0;
	m_zNum = 0;
	iHeights[0] = 500;
	iHeights[1] = 750;
	iHeights[2] = 1000;
	iHeights[3] = 1250;
	iHeights[4] = 1500;
	iHeights[5] = 1750;
	iHeights[6] = 2000;
	iHeights[7] = 2250;
	iHeights[8] = 2500;
	iHeights[9] = 2750;
	iHeights[10] = 3000;
	iHeights[11] = 3250;
	iHeights[12] = 3500;
	iHeights[13] = 3750;
	iHeights[14] = 4000;
	iHeights[15] = 4250;
	iHeights[16] = 4500;
	iHeights[17] = 4750;
	iHeights[18] = 5000;

	iHeights[19] = 5500;
	iHeights[20] = 6000;
	iHeights[21] = 6500;
	iHeights[22] = 7000;
	iHeights[23] = 7500;
	iHeights[24] = 8000;
	iHeights[25] = 8500;
	iHeights[26] = 9000;
	iHeights[27] = 9500;
	iHeights[28] = 10000;
	m_poutData = NULL;
	
	InitGeometry();
}

InterpolationAlgorithm::~InterpolationAlgorithm()
{
	if (NULL != m_poutData)
	{
		for (int i=0; i<m_eleCnt; i++)
		{
			if (NULL != m_poutData[i])
			{
				for (int j=0; j<radialNumPerLayer; j++)
				{
					if (NULL != m_poutData[i][j])
					{
						delete []m_poutData[i][j];
						m_poutData[i][j] = NULL;
					}

				}
				delete []m_poutData[i];
				m_poutData[i] = NULL;
			}
		}
		delete m_poutData;
		m_poutData = NULL;
	}
}

void InterpolationAlgorithm::DrawImge( QVector<QImage>* imageList,QHash<int ,QVector4D>colorHash )
{
	return;
	SingleDataProvider * pDP = (SingleDataProvider *)m_produceDraw;
	PDBlock* blok = pDP->GetPDBlock();

	if(blok==NULL)
	{
		return;
	}

	int productCode = blok->ProductCode;
	proAttr pAttributeHaHa;
	/*cDrawProduct* pDrawProduct=(cDrawProduct *)this;*/
	m_produceDraw->ReadInfo(&pAttributeHaHa,0);
	float hhight = QString::number((float)pAttributeHaHa.altitude*0.1,'f',2).toFloat()*0.001;
	int m_eleCnt = m_produceDraw->GetCountOfLayer();
	PPISTRUCT *struct_Data = (PPISTRUCT*)malloc (m_eleCnt * sizeof (PPISTRUCT)); 

	osg::Vec3 ps[4];
	double s1,c1,s2,c2;
	osg::Vec3 p1,p2,p3,p4;
	for(int i = 0 ;i<m_eleCnt;i++)
	{		 
		DILayer* layer = pDP->GetLayer(i);
		strcpy(struct_Data[i].radar_name, pAttributeHaHa.staName);
		strcpy(struct_Data[i].data_type,"CC1.0");
		strcpy(struct_Data[i].data_name,pAttributeHaHa.pro_name);


		QDate tDateG(1969,12,31);
		tDateG = tDateG.addDays(pAttributeHaHa.generation_date);

		struct_Data[i].year = tDateG.year();
		struct_Data[i].month = tDateG.month();
		struct_Data[i].day= tDateG.day();

		QTime tTimeG(0,0,0,0);
		tTimeG = tTimeG.addMSecs(pAttributeHaHa.generation_time);

		struct_Data[i].hour= tTimeG.hour();
		struct_Data[i].minute= tTimeG.minute();
		struct_Data[i].second = tTimeG.second();

		struct_Data[i].radlat = pAttributeHaHa.latitude;
		struct_Data[i].radlon = pAttributeHaHa.longitude;
		struct_Data[i].radhgt = pAttributeHaHa.altitude;

		if(layer == NULL)
			return;
		for (int j = 0;j < layer->NumberOfBlocks ; j++)
		{
			DLIBlock* dblock =pDP-> GetBlock(i,j);
			if(dblock == NULL)
			{
				return;
			}
			if(dblock->TypeOfDataPackets == BLOCKTYPE_RADIAL)
			{
				RadialHead* radialhead = pDP-> GetBlockDataAsRadial_Head(i,j); 

				//double rate = 1.0; /*= 2.0*(double)wight/(double)bitmp.GetHeight()/zoom*/ //精度，需要重新设定
				double rate =  1000.0;
				struct_Data[i].fstgatdis = radialhead->NumberOfGates *radialhead->LengthOfGate*0.002;/*radialhead->DistanceOfFirstGate;*/
				struct_Data[i].nyq_vel = radialhead->MaxVelocity;
				struct_Data[i].num_beam = radialhead->NumberOfRadials;
				struct_Data[i].num_gate = radialhead->NumberOfGates;
				struct_Data[i].gateWidth = radialhead->LengthOfGate;
				struct_Data[i].elev_angle = layer->HeightOfElevation*0.1;

				for (int k = 0; k <radialhead->NumberOfRadials; k++)
				{
					RadialData* radialdata = pDP->GetBlockDataAsRadial_RadialData(i,j,k);
					struct_Data[i].elev[k] = struct_Data[i].elev_angle;
					struct_Data[i].azim[k] = radialdata->AzimuthAngle*0.1;

					gInt16* pArray = pDP->GetBlockDataAsRadial_Data(i,j,k);

					for (int z  = 0 ;z < radialhead->NumberOfGates ;z++)
					{
						if (VALUE_ROSEINVALID == pArray[z])
							struct_Data[i].field[k][z] = VALUE_INVALID;
						else
							struct_Data[i].field[k][z] = pArray[z]*0.1;
					}
				}
			}	
		}
	}

	//Set grid number(x,y,z) of single site cartisain ordination. data
	m_xNum = int(struct_Data[1].fstgatdis*KM_TO_DEG*2/(0.01)+1);//SINGLE_LON_NUMS;	
	m_yNum = m_xNum;//SINGLE_LAT_NUMS;
	m_zNum = 19;

	unsigned char ***outData;
	outData=new  unsigned char**[m_zNum];         
	for(int ii=0; ii<m_zNum; ii++)
	{
		outData[ii]=new unsigned char *[m_yNum];
		for(int jj=0; jj<m_yNum; jj++)
		{
			outData[ii][jj]=new unsigned char[m_xNum];
			for(int kk = 0; kk<m_xNum; kk++)
				outData[ii][jj][kk] = CODE_NO_OBSERV;
		}
	}

	PolarToDescartes(m_eleCnt,struct_Data,m_xNum,0.01,m_yNum,0.01,m_zNum,1.0,0.5,outData,struct_Data[0].radlon*0.0001,struct_Data[0].radlat*0.0001,hhight);
	//DRAWIMAGE
	QString pngPath;
	for (int i_num = 0 ; i_num <m_zNum;i_num++)
	{
		QString backPic =  QCoreApplication::applicationDirPath() + "/Config/BackGround.png";
		QImage backimg(backPic);
		QImage image = backimg.copy(0,0,m_xNum,m_yNum);
		for (int j = 0;j <m_yNum;j++)
		{
			for (int k = 0;k<m_xNum;k++)
			{
				if (0==outData[i_num][j][k]||1==outData[i_num][j][k]||2==outData[i_num][j][k]||3==outData[i_num][j][k]) continue;
				short temp = outData[i_num][j][k]/2-33;
				int key = temp/5;

				if (key >colorHash.size() || key < 0)
				{
					continue;
				}

				QVector4D colorVec = colorHash.value(key);
				image.setPixel(k,m_yNum-j-1, qRgb(colorVec.x()*255,colorVec.y()*255,colorVec.z()*255));
			}
		}
		imageList->push_back(image);
		pngPath = QString::number(i_num)+".png";
		image.save(pngPath);
	}
}

void InterpolationAlgorithm::PolarToDescartes(short nCuts, struct PPISTRUCT *strct_Data,
	short x_Num, float x_Dlt, short y_Num, float y_Dlt, short z_Num, float z_Dlt, float z_Start,
	unsigned char ***outData,
	float radLon, float radLat, float radHgt)
{
	//Predefined constant parameters
	float const fBeamWidth=double(0.95);//波束宽度 0.95度
	double fMaxElvDiff = fBeamWidth/2;  //格点的仰角与其临近仰角之间的最大差值

	//Local variables
	int i, j, k, ii;
	double fSRange=0., fAz=0., fEl=0., fAzPI=0., fElPI=0.;

	bool bND_Flag, bFold_flag, b_NB_FLag, b_CSD_Flag;

	int		lowElIdx=0, upElIdx=0, lowRIdx=0, upRIdx=0;
	double	lowWeight=0., upWeight=0.;
	short   lowV=0, upV=0;
	double	fDefBetwLow=0.;

	double belta=0., sina=0., s=0.;
	double x=0., y=0., z=0.;

	//	double **el=NULL;			//一个PPI中每个RADIAL的仰角
	double *fAvgEl=NULL;			//一个PPI的平均仰角
	short **azIndex=NULL,curAzIdx=-1;//不同PPI上，每个方位角(0,1,...359度)对应的索引

	//仰角范围
	double minEl = 99, maxEl=-99;//体扫的最大和最小仰角

	double const PAI		=	double(3.14159265);
	double const PI0		=	double(3.14159265/180.);
	double const earth_R	=	double(6371.004);			//地球半径 (km)
	double const earth_Rm=	double(4.0*earth_R/3.0);		//等效地球半径 (km)

	//CWriteLogFile WriteLogFile;
	double ratio;
	unsigned int fSR;
	double sinRad = sin(radLat*PI0);
	double cosRad = cos(radLat*PI0);
	//-------------------------------------------------------------------------//
	//------------------- e x e c t u a b l e   c o d e  ----------------------//

	//为方位角建立索引
	azIndex = new short *[nCuts];
	fAvgEl = new double[nCuts];

	if(azIndex==NULL || fAvgEl==NULL)
		goto ERRCODE1;

	bool tempFlag[361];
	for(i=0; i<nCuts; i++)
	{
		azIndex[i] = new short [360];
		if(azIndex[i] == NULL)// || el[i] == NULL)
		{
			goto ERRCODE1;
		}

		//初始化方位角索引值:-1
		memset(azIndex[i], -1, sizeof(short)*360);
		//初始化本PPI的平均仰角:0.0
		fAvgEl[i] = 0.;
		memset(tempFlag,false,sizeof(bool)*361);
		for(int k=0; k<strct_Data[i].num_beam; k++)
		{
			int idx = int(strct_Data[i].azim[k]/10.);	//转成整型的方位角 （0-359度）  !Whyan 2006.04.28
			if(idx>=360)
			{ 
				idx -= 360;
			}
			//整型方位角对应的索引
			if(k>0 && azIndex[i][idx] != -1) idx++;
			if(idx>=360) idx -= 360;

			azIndex[i][idx] = k;
			tempFlag[idx] = true;
		}

		//Calculate average elevation of this cut
		fAvgEl[i] = strct_Data[i].elev_angle/10.0f;

		if(minEl >fAvgEl[i]) minEl =fAvgEl[i];
		if(maxEl <fAvgEl[i]) maxEl =fAvgEl[i];
	}
	//开始从雷达坐标到笛卡儿坐标的插值计算

	for(j=0; j<y_Num; j++)
	{
		//	TRACE("%3d\n", j);
		//计算纬度
		y=radLat+(j-y_Num/2)*y_Dlt; //从低纬向高纬
		double yy1 = sinRad*sin(y*PI0);
		double yy2 = cosRad*cos(y*PI0);
		double cosyp = cos(y*PI0);
		for(k=0; k<x_Num; k++)
		{
			//计算该点经度(degree)
			x=radLon+(k-x_Num/2)*x_Dlt; //从西
			//如果水平距离雷达太近,不处理这点
			if(fabs(x-radLon)<0.00001 && fabs(y-radLat)<0.00001)continue;

			s = double(earth_R*acos(yy1 + yy2 * cos(x*PI0-radLon*PI0)) );
			//(r，θ,φ)坐标中方位角的SIN值
			sina = double(cosyp * sin(x*PI0-radLon*PI0)/sin(s/earth_R));
			if(fabs(sina)>1) continue;

			//(1): fAz  (r，θ,φ)坐标中的方位角
			if(y>=radLat)
			{
				if(x>=radLon)
					fAzPI = double(asin(sina));
				else
					fAzPI = double(2*PAI+asin(sina));
			}
			else
				fAzPI = double(PAI-asin(sina));

			fAz = fAzPI/PI0;		//由弧度转化为度

			//------ 寻找最近的方位角 ---------
			//以1度为单位的方位
			curAzIdx = int(fAz+.5); // (0-359 deg)
			if(curAzIdx>=360)
				curAzIdx-=360;

			//TRACE("y=%5.1f, x=%5.1f, az=%3.0f   ", y, x, fAz);
			belta = double(s/earth_Rm);
			double cosbel = cos(belta);
			double sinbel = sin(belta);
			for(i=0; i<z_Num; i++)
			{
				//初始化变量
				upElIdx  = -1;
				lowElIdx = -1;

				upRIdx  = -1;
				lowRIdx = -1;

				lowWeight=0;
				upWeight =0;

				bND_Flag = bFold_flag = b_NB_FLag = b_CSD_Flag = false;

				//(2):  fElvPhi  (r，θ,φ)坐标中的仰角
				//z=i*z_Dlt+z_Start;// 计算该点高度(KM)
				z = double(iHeights[i]/1000.);// 计算该点高度(KM)
				if(x>=117.8 && x<=118. && y>=26. && y<=26.5 && z==5.)
				{
					int jj;
					jj=0;
				}
				fElPI = double(atan((cosbel-earth_Rm/(earth_Rm+z-radHgt) )/sinbel) );

				fEl   = fElPI/PI0;       //由弧度转化为度

				//(3):  fSRange (r，θ,φ)坐标中的斜距Slant range (km)
				fSRange = double(sinbel*(earth_Rm+z-radHgt)/cos(fElPI));

				fMaxElvDiff = Min(double(asin(.5f/fSRange)/PI0),0.01f);

				//计算该点(r，θ,φ)在雷达坐标中的位置
				//如果仰角落在雷达体扫范围之外,跳过,不处理这个格点
				if(fEl<minEl-fMaxElvDiff) continue;
				if(fEl>maxEl+fMaxElvDiff)
				{
					//对于扫覆盖范围以内的盲区，设置为无效数值
					outData[i][j][k] = CODE_INVALID;
					continue;
				}

				//1 确定此格点在雷达坐标的仰角范围 lowElIdx: 下层仰角， upElIdx: 上层仰角,
				//2 确定此格点在雷达坐标中的斜距--采用最近方位的斜距
				double cosfe = cos(fEl*PI0);
				double cosfel = cos(fElPI);
				for( ii=0; ii<nCuts-1; ii++)
				{
					//(近似)位于低仰角上
					if(fabs(fEl-fAvgEl[ii])<=fMaxElvDiff)
					{
						//距离库长非法， 有错误情况，不处理这一点
						if(strct_Data[ii].gateWidth==0)
						{
							break;
						}
						ratio = cosfe/cos(fAvgEl[ii]*PI0);
						fSR=int(fSRange*1000*ratio);
						lowRIdx = int(fSR/strct_Data[ii].gateWidth+.5);

						if(lowRIdx < strct_Data[ii].num_gate) lowElIdx = ii;

						//采用垂直内插方法
						lowWeight  = 1;
						upWeight   = 0;
						break;
					}
					//(近似)位于高仰角上
					else if(fabs(fAvgEl[ii+1]-fEl)<=fMaxElvDiff)
					{
						//距离库长非法， 有错误情况，不处理这一点
						if(strct_Data[ii+1].gateWidth==0)
						{
							break;
						}

						ratio = cosfel/cos(fAvgEl[ii+1]*PI0);

						int fSR=int(fSRange*1000*ratio);
						upRIdx = int(fSR/strct_Data[ii+1].gateWidth+.5);

						if(upRIdx < strct_Data[ii+1].num_gate)
							upElIdx = ii+1;

						//采用垂直内插方法
						lowWeight  = 0;
						upWeight   = 1;
						break;
					}
					//如果格点的仰角落在体扫的两个仰角之间，记下这两个相邻的仰角
					else if(fAvgEl[ii]<fEl && fAvgEl[ii+1]>fEl)
					{
						//距离库长非法， 有错误情况，不处理这一点
						if(strct_Data[ii].gateWidth ==0 ||
							strct_Data[ii+1].gateWidth <=0) break;

						ratio = cosfe/cos(fAvgEl[ii]*PI0);
						int fSR=int(fSRange*1000*ratio);
						lowRIdx = int(fSR/strct_Data[ii].gateWidth+.5);

						fSR=int(fSRange*1000*cosfel/cos(fAvgEl[ii+1]*PI0));
						upRIdx = int(fSR/strct_Data[ii+1].gateWidth+.5);

						//在有效斜距范围内
						if(lowRIdx < strct_Data[ii].num_gate)
							lowElIdx = ii;
						else
						{
							lowWeight = 0;
							upWeight=1;
							break;
						}
						if(upRIdx < strct_Data[ii+1].num_gate)
							upElIdx = ii+1;
						else
						{
							lowWeight = 1;
							upWeight=0;
							break;
						}

						//只有低仰角距离范围有效时， 判断笛卡儿坐标格点是否超出低仰角的波束宽度范围
						//波束范围以外的不做处理
						outData[i][j][k] = CODE_INVALID;

						lowWeight  = (fAvgEl[upElIdx]-fEl)/(fAvgEl[upElIdx]-fAvgEl[lowElIdx]);
						upWeight = (fEl-fAvgEl[lowElIdx])/(fAvgEl[upElIdx]-fAvgEl[lowElIdx]);

						break;
					}
				}//检查仰角落区

				//计算格点个低仰角之间的差
				fDefBetwLow = fEl-fAvgEl[ii];

				if(lowElIdx!=-1 && upElIdx!=-1)//两层仰角有效，进行同一高度的二维水平插值和垂直线性插值
				{
					//获取邻近两个仰角上，相同斜距位置的值
					upV		=	strct_Data[upElIdx].field[azIndex[upElIdx][curAzIdx]][upRIdx];
					lowV	=	strct_Data[lowElIdx].field[azIndex[lowElIdx][curAzIdx]][lowRIdx];

					lowWeight  = (fAvgEl[upElIdx]-fEl)/(fAvgEl[upElIdx]-fAvgEl[lowElIdx]);
					upWeight = (fEl-fAvgEl[lowElIdx])/(fAvgEl[upElIdx]-fAvgEl[lowElIdx]);

					//计算目标格点的值
					if( (upV>VALUE_INVALID) && (lowV>VALUE_INVALID) )
					{	//采用垂直内插方法
						outData[i][j][k]=(unsigned char)((upWeight*upV+lowWeight*lowV)/10.*2 + 66. + .5);   //NVI方法 /垂直内插
					}
					else if(lowV > VALUE_INVALID)// && fDefBetwLow<=fMaxElvDiff)
					{	//取低层仰角的值
						outData[i][j][k]=(unsigned char)((lowV*lowWeight)/10.*2. + 66. + .5);
						//if(outData[i][j][k]<ucMinRefOnTop) outData[i][j][k]=CODE_INVALID;
					}
					else if(upV > VALUE_INVALID)
					{	//取高层仰角的值
						outData[i][j][k]=(unsigned char)((upV*upWeight)/10.*2. + 66. + .5);
					}
					else //bND_Flag, bFold_flag, b_NB_FLag
					{	//判断是什么特殊数值
						if(upV==VALUE_INVALID || lowV==VALUE_INVALID)
							bND_Flag = true;
						if(upV==VALUE_RANFOLD || lowV==VALUE_RANFOLD)
							bFold_flag = true;
						if(upV<=VALUE_CLUTTER || lowV<= VALUE_CLUTTER)
							b_CSD_Flag = true;

						if(!bND_Flag && bFold_flag && !b_CSD_Flag)		//均为距离模糊
							outData[i][j][k] = CODE_RANFOLD;
						else if(!bND_Flag && !bFold_flag && b_CSD_Flag) //均为休整过的数据
							outData[i][j][k] = CODE_CSRED;
						else											//其它情况视为许无数据
							outData[i][j][k] = CODE_INVALID;
					}
				}
				else if(lowElIdx != -1) //低层斜距有效，采用低层仰角的数值
				{
					lowV = strct_Data[lowElIdx].field[azIndex[lowElIdx][curAzIdx]][lowRIdx];
					if(lowV > VALUE_INVALID )//&& fDefBetwLow<=fMaxElvDiff)
					{
						outData[i][j][k] = (unsigned char)((lowV*lowWeight)/10.*2 + 66. + .5);
						//if(outData[i][j][k]<ucMinRefOnTop) outData[i][j][k]=CODE_INVALID;

					}
					else if(lowV == VALUE_INVALID)
						outData[i][j][k] = CODE_INVALID;
					else if(lowV == VALUE_RANFOLD)
						outData[i][j][k] = CODE_RANFOLD;
					else if(lowV <= VALUE_CLUTTER)
						outData[i][j][k] = CODE_CSRED; //做过质量控制
				}
				else if(upElIdx != -1) //高层斜距有效，采用高层仰角的数值
				{
					upV	= strct_Data[upElIdx].field[azIndex[upElIdx][curAzIdx]][upRIdx]; //dBZ*10
					if(upV > VALUE_INVALID)
						outData[i][j][k] = (unsigned char)((upV*upWeight)/10.*2 + 66. + .5);
					else if(upV == VALUE_INVALID)
						outData[i][j][k] = CODE_INVALID;
					else if(upV == VALUE_RANFOLD)
						outData[i][j][k] = CODE_RANFOLD;
					else if(upV <= VALUE_CLUTTER)
						outData[i][j][k] = CODE_CSRED; //做过质量控制
				}
				else
				{
					outData[i][j][k] = CODE_INVALID;////
				}

			}// end for i(0-num_Z)
		} //end for k(0-num_X)
	}// end for j(0-num_Y)

	goto ERRCODE1;

ERRCODE1:

	//释放方位角索引缓存、仰角缓存
	for(i=0; i<nCuts; i++)
	{
		if(azIndex[i] != NULL)	delete []azIndex[i];
	}
	if(azIndex != NULL)		delete []azIndex;
	if(fAvgEl!=NULL)        delete []fAvgEl;

}

void InterpolationAlgorithm::Cutting(osg::Vec3& pp1, osg::Vec3& pp2, osg::Vec3& pp3, osg::Vec3& pp4, osg::Node* pClipNode,QHash<int,QVector4D>colorHash,goto_gis::CoordinateTransform* proj_earth)
{
	static bool bAddDrawable = true;
	double const earth_R	=	double(6371.004);	
	double const earth_Rm=	double(4.0*earth_R/3.0);		//等效地球半径 (km)
	int const C_iImgHgt = 40;
	SingleDataProvider * singleDP = (SingleDataProvider *)m_produceDraw;
	PDBlock* blok = singleDP->GetPDBlock();
	if(blok==NULL)
	{
		return;
	}
	int productCode = blok->ProductCode;
	int numberOfLayers = blok->NumberOfLayers;
	double  heightaz[20]={0.5,1.5,2.4,3.4,4.3,6.0,9.0,14.6};
	RadarInfor * pRadarInfor = singleDP->GetRadarInfo();

	for(int i=0; i< numberOfLayers; i++)
	{
		DILayer * pLayer = singleDP->GetLayer(i);
    	 heightaz[i]= pLayer->HeightOfElevation/100.0;
	}
	
	if(pRadarInfor ==NULL)
		return;

	double mAltitude = pRadarInfor->Altitude/100000.0; 

	proAttr pAttributeHaHa;
	m_produceDraw->ReadInfo(&pAttributeHaHa,0);
	hhight = QString::number((float)pAttributeHaHa.altitude*0.1,'f',2).toFloat()*0.001;
	if (1== m_eleCnt||0 == m_eleCnt)
	{
		return;
	}
	double lon = pAttributeHaHa.longitude * 0.0001;
	double lat = pAttributeHaHa.latitude  * 0.0001;
	RadProjTransform *proj = NULL;
	osg::Vec3 p =osg::Vec3(lon,lat,hhight);
	proj = new RadProjTransform;
	QString destproj = "RADARPROJ" ;
	destproj.append(" ");
	destproj.append(QString::number(p.x()));
	destproj.append(" ");
	destproj.append( QString::number(p.y()));
	destproj.append(" ");
	destproj.append(QString::number(p.z()));

	proj->setSourceCRS("+proj=longlat +ellps=WGS84 +datum=WGS84 +no_defs");
	proj->setSourceProjName("无投影");
	proj->setDestCRS(destproj.toStdString());
	proj->setDesProjName("雷达中心兰勃特投影");
	proj->Elevation(true);
	osg::Vec3 pp1_out=proj->Transform(pp1);//逆变
	osg::Vec3 pp2_out=proj->Transform(pp2);//逆变
	osg::Vec3 pointA,pointB;
#if 0
	osg::ref_ptr<osg::Vec3Array > pointA_array = new osg::Vec3Array;
	osg::ref_ptr<osg::Vec3Array > pointB_array = new osg::Vec3Array;

	coordTransform(pp1,pp2,pointA_array,pointB_array);//剖面线与雷达圈交点
	pointA = pointA_array->at(0);
	pointB = pointB_array->at(0);

#endif
	double R= singleDP->GetDataRadius(0)/1000;//半径 
	pointA = osg::Vec3(pp1_out.x()/1000.0,pp1_out.y()/1000.0,pp1_out.z()/1000.0);
	pointB = osg::Vec3(pp2_out.x()/1000.0,pp2_out.y()/1000.0,pp2_out.z()/1000.0);

	double angle = getAngle(circlePoint,pointA,pointB);//获得oa，ob夹角(角度非弧度)
	osg::Vec3 pointNorth = osg::Vec3(0,R,hhight);

	double azimuth_a = GetAzimuth(pointA);
	double azimuth_b = GetAzimuth(pointB);

	double disOA = Getdistans(osg::Vec3(0,0,0),pointA);
	double disOB = Getdistans(osg::Vec3(0,0,0),pointB);
	if (angle >= 180)
	{
		angle = 360 -angle;
	}
	double disAtoB = Getdistans(pointA,pointB);
	QSize imageSize((int)disAtoB, C_iImgHgt);
	QImage image(imageSize, QImage::Format::Format_ARGB32_Premultiplied);
	image.fill(QColor(1, 1, 1, 1));
	QRgb value_color;

	if (angle >= 180)
	{
		angle = 360 -angle;
	}

	int img_x = 0,img_y =0;
	for (int dis = 1,img_x = 0; dis <int(disAtoB);dis++,img_x++)
	{
		double disOtoP;
		double cosOAC = (disOA * disOA +disAtoB * disAtoB -disOB *disOB)/(2 * disOB *disAtoB);
		if(cosOAC < -1 && cosOAC > -2)
			cosOAC = -1;
		else if(cosOAC > 1 && cosOAC < 2)
			cosOAC = 1;

		disOtoP = sqrt(disOB*disOB +dis * dis -2*disOB *dis *cosOAC);
		double angleP,angleDest;
		if (disOtoP == 0.0)
		{
			angleP = 0;
		}
		else
		{
			double tmepcos = (disOB - dis * cosOAC)/disOtoP;
			if(tmepcos < -1 && tmepcos > -2)
				tmepcos = -1;
			else if(tmepcos > 1 && tmepcos < 2)
				tmepcos = 1;
			angleP = acos(tmepcos)*180/M_PI;
		}
			
		if (azimuth_b >=azimuth_a && abs(azimuth_b-azimuth_a)< 180)
		{
			angleDest = abs(azimuth_b-angleP);
		}else if (azimuth_b >=azimuth_a && abs(azimuth_b-azimuth_a)>= 180)
		{
			angleDest = abs(azimuth_b+angleP);
		}else if (azimuth_b <azimuth_a && abs(azimuth_b-azimuth_a)>= 180)
		{
			angleDest = abs(azimuth_b-angleP);
		}else if (azimuth_b <azimuth_a && abs(azimuth_b-azimuth_a)< 180)
		{
			angleDest = abs(azimuth_b+angleP);
		}
		double eij=0;
		double  az=0;
		double  l = 0;
	    int valcout =0;
		for (double dis_j = 0.0,img_y=0; dis_j<20.0; dis_j += 0.5,img_y++)
		{
			double discha = cos(disOtoP/earth_Rm) - earth_Rm/(earth_Rm+dis_j-mAltitude);
			double cosd = sin(disOtoP/earth_Rm);

			 eij = (180.0*atan(discha/cosd))/M_PI;
			 //////////////计算出的仰角eij起始段为负值，此处跳过这段无意义的反演/////////////
			 if( eij < 0.4)  continue;
			az = angleDest;

			double sind = sin(disOtoP/earth_Rm);
			double disRm =(earth_Rm + dis_j - mAltitude);
			l = sind*disRm/cos(atan(discha/cosd));
			int xx =0;
			int yy = int(az+.5);
			if (yy>=radialNumPerLayer)continue;
			int zz = int (l*1000/LengthOfGate);
			if (zz >= numOfGates)continue;
			gInt16 val =VALUE_ROSEINVALID;
			if(heightaz[0]-eij> 0.01 && eij >0.0)
			{
				val = m_poutData[xx][yy][zz];
			}
			else if( eij - heightaz[0] > 0.01 && heightaz[numberOfLayers-1]-eij >0.01)
			{
				float downWgt=0,upWgt=0;
				WHATELEVALID validType = NOTVALID;
				for(int azindex =0; azindex < numberOfLayers-1; azindex ++)
				{
					if (fabs(eij -heightaz[azindex]) < .5)
					{
						xx = azindex;
						downWgt = 1;
						validType = LOWVALID;
						break;
					}
					else if (fabs(eij -heightaz[azindex+1]) < .5)
					{
						xx = azindex+1;
						upWgt = 1;
						validType = UPVALID;
						break;
					}
					else if(eij>heightaz[azindex] && eij<heightaz[azindex+1])
					{
						xx = azindex;
						downWgt = ((eij- heightaz[xx])/(heightaz[xx+1]-heightaz[xx]));
						upWgt =  (( heightaz[xx+1] - eij)/(heightaz[xx+1]-heightaz[xx]));
						validType = DOUBLEVALID;
						break;
					}
				}
				gInt16 valdown = m_poutData[xx][yy][zz];	
				gInt16 valup = m_poutData[xx][yy][zz];
				switch (validType)
				{
				case LOWVALID:
					valup = m_poutData[xx+1][yy][zz];
					if(valdown != VALUE_ROSEINVALID)
					{
						val = downWgt*valdown;
					}
					break;
				case UPVALID:
					if (valup != VALUE_ROSEINVALID)
					{
						val = upWgt*valup;
					}
					break;
				case DOUBLEVALID:
					//插值
					valup = m_poutData[xx+1][yy][zz];
					if((valdown!=VALUE_ROSEINVALID) && (valup!=VALUE_ROSEINVALID))
					{
						val = downWgt*valdown + upWgt*valup;
					}
					else if(valdown != VALUE_ROSEINVALID)
					{
						val = downWgt*valdown;
					}
					else if(valup != VALUE_ROSEINVALID)
					{
						val = upWgt*valup;
					}
					break;
				default:
					break;
				}
			}	
			
			float value =0;
			if (val != VALUE_ROSEINVALID)
			{
				osg::Vec4 tempcolor;	
				value = ((float)val)/blok->ScaleOfData;
				int key = value/5;
				if(key > 0)
					valcout+=1;

				if (key >colorHash.size()||key < 0)
				{
					continue;
				}

				QVector4D tempcolor_QT = colorHash.value(key);;
				value_color = qRgb(tempcolor_QT.x()*255 ,tempcolor_QT.y()*255,tempcolor_QT.z()*255);
				image.setPixel(img_x, C_iImgHgt-img_y+1, value_color);
			}
		}
	}

	//system("pause");
	delete proj;
	proj = NULL;
	///////////////////////////////////////////////////////////////////////
	pVertexArray->clear();
	osg::Vec3 src(pp1.x(),pp1.y(),pp1.z()+20000);
	osg::Vec3 res1;
	if(!(proj_earth->Transform(src, res1)))
	{
		pVertexArray->push_back(res1);
	}
	osg::Vec3 src2(pp1/*.x(),pp1.y(),pp1().z()+200*/);
	osg::Vec3 res2;
	if(!(proj_earth->Transform(src2, res2)))
	{
		pVertexArray->push_back(res2);
	}

	osg::Vec3 src3(pp2/*.x(),pp1.y(),pp1().z()+200*/);
	osg::Vec3 res3;
	if(!(proj_earth->Transform(src3, res3)))
	{
		pVertexArray->push_back(res3);
	}
	osg::Vec3 src4(pp2.x(),pp2.y(),pp2.z()+20000);
	osg::Vec3 res4;
	if(!(proj_earth->Transform(src4, res4)))
	{
		pVertexArray->push_back(res4);
	}
	//QImage to osg::Image
	if(32 != image.depth()) return;
	image = QGLWidget::convertToGLFormat(image);//容易出问题崩溃
	unsigned char* data = new unsigned char[image.byteCount()];
	for (int i = 0; i< image.byteCount();i++)
	{
		data[i] = image.bits()[i];
	}
	osg::ref_ptr<osg::Image> image_osg   = new osg::Image;
	image_osg->setImage(image.width(), image.height(), 1, 4, GL_RGBA, GL_UNSIGNED_BYTE, data, osg::Image::USE_NEW_DELETE, 1);
	//end QImage to osg::Image
	pTexture2D->setImage(image_osg);
	if(bAddDrawable)
	{
		pClipNode->asGeode()->addDrawable(pGeom);
		bAddDrawable = false;
	}

	pGeom->dirtyBound();
	pGeom->dirtyDisplayList();
}

//void InterpolationAlgorithm::Cutting( osg::Vec3& pp1, osg::Vec3& pp2, osg::Vec3& pp3, osg::Vec3& pp4, osg::Node* pClipNode,QHash<int,QVector4D>colorHash,goto_gis::CoordinateTransform* proj_earth)
//{
//	static bool bAddDrawable = true;
//
//	SingleDataProvider * singleDP = (SingleDataProvider *)m_produceDraw;
//	PDBlock* blok = singleDP->GetPDBlock();
//	if(blok==NULL)
//	{
//		return;
//	}
//	int productCode = blok->ProductCode;
//
//	proAttr pAttributeHaHa;
//	m_produceDraw->ReadInfo(&pAttributeHaHa,0);
//	hhight = QString::number((float)pAttributeHaHa.altitude*0.1,'f',2).toFloat()*0.001;
//	layerCount        = m_produceDraw->GetCountOfLayer();
//	if (1== layerCount||0 == layerCount)
//	{
//		return;
//	}
//	double lon = pAttributeHaHa.longitude * 0.0001;
//	double lat = pAttributeHaHa.latitude  * 0.0001;
//	RadProjTransform *proj = NULL;
//	osg::Vec3 p =osg::Vec3(lon,lat,hhight);
//	proj = new RadProjTransform;
//	QString destproj = "RADARPROJ" ;
//	destproj.append(" ");
//	destproj.append(QString::number(p.x()));
//	destproj.append(" ");
//	destproj.append( QString::number(p.y()));
//	destproj.append(" ");
//	destproj.append(QString::number(p.z()));
//
//	proj->setSourceCRS("+proj=longlat +ellps=WGS84 +datum=WGS84 +no_defs");
//	proj->setSourceProjName("无投影");
//
//	proj->setDestCRS(destproj.toStdString());
//	proj->setDesProjName("雷达中心兰勃特投影");
//
//	proj->Elevation(true);
//	osg::Vec3 pp1_out=proj->Transform(pp1,POSITIVE_DIRECTION);
//	osg::Vec3 pp2_out=proj->Transform(pp2,POSITIVE_DIRECTION);
//	//delete proj;
//	//proj = NULL;
//	osg::Vec3 pointA,pointB;
//#if 0
//	osg::ref_ptr<osg::Vec3Array > pointA_array = new osg::Vec3Array;
//	osg::ref_ptr<osg::Vec3Array > pointB_array = new osg::Vec3Array;
//
//	coordTransform(pp1,pp2,pointA_array,pointB_array);//剖面线与雷达圈交点
//	pointA = pointA_array->at(0);
//	pointB = pointB_array->at(0);
//
//#endif
//
//
//	double R= singleDP->GetDataRadius(0)/1000;//半径 
//	
//	pointA = osg::Vec3(pp1_out.x()/1000.0,pp1_out.y()/1000.0,pp1_out.z()/1000.0);
//	pointB = osg::Vec3(pp2_out.x()/1000.0,pp2_out.y()/1000.0,pp2_out.z()/1000.0);
//
//	double angle = getAngle(circlePoint,pointA,pointB);//获得oa，ob夹角(角度非弧度)
//	osg::Vec3 pointNorth = osg::Vec3(0,R,hhight);
//
//	double azimuth_a = GetAzimuth(pointA);
//	double azimuth_b = GetAzimuth(pointB);
//
//	double disOA = Getdistans(osg::Vec3(0,0,0),pointA);
//	double disOB = Getdistans(osg::Vec3(0,0,0),pointB);
//	if (angle >= 180)
//	{
//		angle = 360 -angle;
//	}
////	double disAtoB = sqrt(2*disOA*disOB -2*disOA*disOB*cos(angle));
//	double disAtoB = Getdistans(pointA,pointB);
//	QSize imageSize((int)disAtoB, 151);
//	QImage image(imageSize, QImage::Format::Format_ARGB32_Premultiplied);
//	image.fill(QColor(1, 1, 1, 0));
//	QRgb value_color;
//
//	if (angle >= 180)
//	{
//		angle = 360 -angle;
//	}
//
//	
//	int img_x = 0,img_y =0;
//	for(int dis = 1,img_x = 0; dis <int(disAtoB);dis++,img_x++ )
//	{
//		double disOtoP;
//		double cosOAC = (disOA * disOA +disAtoB * disAtoB -disOB *disOB)/(2 * disOB *disAtoB);
//		if(cosOAC < -1 && cosOAC > -2)
//			cosOAC = -1;
//		else if(cosOAC > 1 && cosOAC < 2)
//			cosOAC = 1;
//
//		disOtoP = sqrt(disOB*disOB +dis * dis -2*disOB *dis *cosOAC);
//		double angleP,angleDest;
//		if (disOtoP == 0.0)
//		{
//			angleP = 0;
//		}else
//		{
//			double tmepcos = (disOB - dis * cosOAC)/disOtoP;
//			if(tmepcos < -1 && tmepcos > -2)
//				tmepcos = -1;
//			else if(tmepcos > 1 && tmepcos < 2)
//				tmepcos = 1;
//			angleP = acos(tmepcos)*180/M_PI;
//		}
//			
//		if (azimuth_b >=azimuth_a && abs(azimuth_b-azimuth_a)< 180)
//		{
//			angleDest = azimuth_b-angleP;
//		}else if (azimuth_b >=azimuth_a && abs(azimuth_b-azimuth_a)>= 180)
//		{
//			angleDest = azimuth_b+angleP;
//		}else if (azimuth_b <azimuth_a && abs(azimuth_b-azimuth_a)>= 180)
//		{
//			angleDest = azimuth_b-angleP;
//		}else if (azimuth_b <azimuth_a && abs(azimuth_b-azimuth_a)< 180)
//		{
//			angleDest = azimuth_b+angleP;
//		}
//		double lon,lat;
//		proj->arcS_of_gps(angleDest,disOtoP,lon,lat);
//
//		for (double dis_j = 0.5,img_y=0; dis_j<20.0/*img_y<elevationMap.size()*/; dis_j += 0.5,img_y++)
//		{
//			double az ,l,h;
//			proj->gps_to_tilt(lon,lat,/*elevationMap.value(img_y)/100*/dis_j,az,l,h);
//			int xx = GetElevationNumMap(dis_j)/*img_y*/;
//			int yy = az * radialNumPerLayer/360;
//			if (yy>=radialNumPerLayer)continue;
//			int zz = l *1000/LengthOfGate ;
//			if (zz >= numOfGates)continue;
//
//			gInt16 val = m_outData[xx*radialNumPerLayer +yy *numOfGates +zz];
//			if (val != -32768 && val!= -32767)
//			{
//				osg::Vec4 tempcolor;
//				if ( -32766 == val )
//				{
//					int key = -32766;
//					tempcolor.set(0.5f,0,0.5f,1);
//					value_color = qRgb(128,128,255);
//					image.setPixel(img_x,img_y, value_color);
//				}//end if(var ==-32766)
//				else
//				{
//					float value =((float)val)/blok->ScaleOfData;									
//					int key = value/5;
//
//					if (key >colorHash.size()||key < 0)
//					{
//						continue;
//					}
//
//					QVector4D tempcolor_QT = colorHash.value(key);;
//					value_color = qRgb(tempcolor_QT.x()*255 ,tempcolor_QT.y()*255,tempcolor_QT.z()*255);
//					image.setPixel(img_x,img_y, value_color);
//				}
//			}// end if(val != -32768 && val!= -32767)
//		}
//	}
//	delete proj;
//	proj = NULL;
//	///////////////////////////////////////////////////////////////////////
//	pVertexArray->clear();
//	osg::Vec3 src(pp1.x(),pp1.y(),pp1.z()+20000);
//	osg::Vec3 res1;
//	if(!(proj_earth->Transform(src, res1)))
//	{
//		pVertexArray->push_back(res1);
//	}
//	osg::Vec3 src2(pp1/*.x(),pp1.y(),pp1().z()+200*/);
//	osg::Vec3 res2;
//	if(!(proj_earth->Transform(src2, res2)))
//	{
//		pVertexArray->push_back(res2);
//	}
//
//	osg::Vec3 src3(pp2/*.x(),pp1.y(),pp1().z()+200*/);
//	osg::Vec3 res3;
//	if(!(proj_earth->Transform(src3, res3)))
//	{
//		pVertexArray->push_back(res3);
//	}
//	osg::Vec3 src4(pp2.x(),pp2.y(),pp2.z()+20000);
//	osg::Vec3 res4;
//	if(!(proj_earth->Transform(src4, res4)))
//	{
//		pVertexArray->push_back(res4);
//	}
//	//QImage to osg::Image
//	if(32 != image.depth()) return;
//	image = QGLWidget::convertToGLFormat(image);//容易出问题崩溃
//	unsigned char* data = new unsigned char[image.byteCount()];
//	for (int i = 0; i< image.byteCount();i++)
//	{
//		data[i] = image.bits()[i];
//	}
//	osg::ref_ptr<osg::Image> image_osg   = new osg::Image;
//	image_osg->setImage(image.width(), image.height(), 1, 4, GL_RGBA, GL_UNSIGNED_BYTE, data, osg::Image::USE_NEW_DELETE, 1);
//	//end QImage to osg::Image
//	pTexture2D->setImage(image_osg);
//	if(bAddDrawable)
//	{
//		pClipNode->asGeode()->addDrawable(pGeom);
//		bAddDrawable = false;
//	}
//
//	pGeom->dirtyBound();
//	pGeom->dirtyDisplayList();
//}

void InterpolationAlgorithm::DataPrepar()
{
	SingleDataProvider * pDP = (SingleDataProvider *)m_produceDraw;
	PDBlock* blok = pDP->GetPDBlock();

	if(blok==NULL)
	{
		return;
	}

	int productCode = blok->ProductCode;
	m_eleCnt = m_produceDraw->GetCountOfLayer();
	radialNumPerLayer = pDP->GetBlockDataAsRadial_Head(0,0)->NumberOfRadials;
	numOfGates        = pDP->GetBlockDataAsRadial_Head(0,0)->NumberOfGates;
	LengthOfGate = pDP->GetBlockDataAsRadial_Head(0,0)->LengthOfGate;

	m_poutData=new gInt16**[m_eleCnt]; 
	for(int ii=0; ii<m_eleCnt; ii++)
	{
		m_poutData[ii]=new gInt16 *[radialNumPerLayer];
		for(int jj=0; jj<radialNumPerLayer/**/; jj++)
		{
			m_poutData[ii][jj]=new gInt16[numOfGates];
			for(int kk = 0; kk<numOfGates; kk++)
				m_poutData[ii][jj][kk] = VALUE_ROSEINVALID/**/;
		}
	}
	for(int i = 0 ;i<m_eleCnt;i++)
	{		 
		DILayer* layer = pDP->GetLayer(i);
		if(layer == NULL)
			return;
		for (int j = 0;j < layer->NumberOfBlocks ; j++)
		{
			DLIBlock* dblock =pDP-> GetBlock(i,j);
			if(dblock == NULL)
			{
				return;
			}
			if(dblock->TypeOfDataPackets == BLOCKTYPE_RADIAL)
			{
				RadialHead* radialhead = pDP-> GetBlockDataAsRadial_Head(i,j); 
				for (int k = 0; k <radialhead->NumberOfRadials; k++)
				{
					RadialData* radialdata = pDP->GetBlockDataAsRadial_RadialData(i,j,k);
					gInt16* pArray = pDP->GetBlockDataAsRadial_Data(i,j,k);
					memcpy(m_poutData[i][k], pArray, sizeof(gInt16)*radialhead->NumberOfGates);
				}
			}	
		}
	}
}
///////////////////////////////////////////////////////////////////////////
//将剖面线两端坐标（经纬度）转换为雷达覆盖区域内的有效坐标（平面坐标）
///////////////////////////////////////////////////////////////////////////
void InterpolationAlgorithm::coordTransform( osg::Vec3& pp1_in, osg::Vec3& pp2_in,osg::Vec3& pp1_out, osg::Vec3& pp2_out )
{
	SingleDataProvider * singleDP = (SingleDataProvider *)m_produceDraw;
	PDBlock* blok = singleDP->GetPDBlock();
	proAttr pAttributeHaHa;
	m_produceDraw->ReadInfo(&pAttributeHaHa,0);
	double lon = pAttributeHaHa.longitude * 0.0001;
	double lat = pAttributeHaHa.latitude  * 0.0001;
	RadProjTransform *proj = NULL;
	osg::Vec3 p =osg::Vec3(lon,lat,hhight);
	proj = new RadProjTransform;
	QString destproj = "RADARPROJ" ;
	destproj.append(" ");
	destproj.append(QString::number(p.x()));
	destproj.append(" ");
	destproj.append( QString::number(p.y()));
	destproj.append(" ");
	destproj.append(QString::number(p.z()));

	proj->setSourceCRS("+proj=longlat +ellps=WGS84 +datum=WGS84 +no_defs");
	proj->setDestCRS(destproj.toStdString());
	proj->setDesProjName("兰伯特投影");

	proj->Elevation(true);

	circlePoint =proj->Transform(p);//逆变
	pp1_out=proj->Transform(pp1_in);//逆变
	pp1_out.set(pp1_out.x()/1000.0,pp1_out.y()/1000.0,pp1_out.z());

	pp2_out=proj->Transform(pp2_in);//逆变
	pp2_out.set(pp2_out.x()/1000.0,pp2_out.y()/1000.0,pp2_out.z());

	double dis_AO = Getdistans(osg::Vec3(0,0,0),pp1_out);
	double dis_BO = Getdistans(osg::Vec3(0,0,0),pp2_out);

	float k , b;

	k = (pp2_out.y() - pp1_out.y())/(pp2_out.x() - pp1_out.x());//剖面线斜率
	b = pp1_out.y() - k * pp1_out.x();//剖面斜距

	float A = k,B =-1.0,C = b;
	float M = fabs(C)/sqrt(A*A+B*B);//

	double R= singleDP->GetDataRadius(0)/1000.0;//半径 
	osg::Vec3 pointA, pointB;
	float xa1,ya1,xb1,yb1;

	if (M >= R)
	{
		return;
	}
	else if (M == 0)
	{
	}
	else if ( M < R)
	{
		float ma = k*k +1;
		float mb = 2*k*b;
		float mc = b*b - R*R;//circlePoint.x() * circlePoint.x() + (b - circlePoint.y()) * (b - circlePoint.y()) - R*R;
		double mdet = mb*mb - 4*ma*mc;

		if (mdet >= 0.0)
		{
			xa1 = (-mb + sqrt(mdet))/(2*ma);
			xb1 = (-mb - sqrt(mdet))/(2*ma);

			ya1 = k*xa1 + b;
			yb1 = k*xb1 + b;

		}
		pointA.set(xa1,ya1,0);
		pointB.set(xb1,yb1,0);
	}

	if (dis_AO > R)
	{
		pp1_out.set(pointA);
	}

	if (dis_BO > R)
	{
		pp2_out.set(pointB);
	}


}

double InterpolationAlgorithm::getAngle(osg::Vec3 pSrc, osg::Vec3 p1, osg::Vec3 p2)
{
	double angle = 0.0f; // 夹角
	
	// 向量Vector a的(x, y)坐标
	double va_x = p1.x() - pSrc.x();
	double va_y = p1.y() - pSrc.y();
	
	// 向量b的(x, y)坐标
	double vb_x = p2.x() - pSrc.x();
	double vb_y = p2.y() - pSrc.y();
	
	double productValue = (va_x * vb_x) + (va_y * vb_y);  // 向量的乘积
	double va_val = sqrt(va_x * va_x + va_y * va_y);  // 向量a的模
	double vb_val = sqrt(vb_x * vb_x + vb_y * vb_y);  // 向量b的模
	double cosValue = productValue / (va_val * vb_val);      // 余弦公式
	
	// acos的输入参数范围必须在[-1, 1]之间，否则会"domain error"
	// 对输入参数作校验和处理
	if(cosValue < -1 && cosValue > -2)
		cosValue = -1;
	else if(cosValue > 1 && cosValue < 2)
		cosValue = 1;
	
	// acos返回的是弧度值，转换为角度值
	angle = acos(cosValue) * 180 / M_PI; 
	
	return angle;
}

double InterpolationAlgorithm::GetAzimuth( osg::Vec3 p )
{
	double azimuth = 0 ;
	if (p.x()>0&&p.y()>0)//几何坐标第一象限，雷达第一象限
	{
		return azimuth = 90 - atan(p.y()/p.x()) * 180 /M_PI;

	}else if (p.x()>0&&p.y()<0)//几何坐标第四象限，雷达第二象限
	{
		return azimuth = 90 - atan(p.y()/p.x())* 180 /M_PI;

	}else if (p.x()<0&&p.y()<0)//几何坐标第三象限，雷达第三象限
	{
		return azimuth = 270 - atan(p.y()/p.x())* 180 /M_PI;

	}else if (p.x()<0&&p.y()>0)//几何坐标第二象限，雷达第四象限
	{
		return azimuth = 270 - atan(p.y()/p.x())* 180 /M_PI;

	}else if (p.x()==0&&p.y()>0)
	{
		return 0;
	}else if (p.x()==0&&p.y()<0)
	{
		return 180;
	}
	else if (p.x()<0&&p.y()==0)
	{
		return 270;
	}
	else if (p.x()>0&&p.y()==0)
	{
		return 90;
	}
}

int InterpolationAlgorithm::GetElevationNumMap( double ele )
{
	QMap<int ,float > tempMap;
	QMap<int ,float >::iterator it;
	QList<float> temparray;

	for(it = elevationMap.begin();it != elevationMap.end();it++)
	{
		tempMap.insert(it.key(),abs(it.value()/100.0 - ele));
		temparray.push_back(abs(it.value()/100.0 - ele));
	}

	float minvalue=99999;

	for (int i = 0; i< temparray.size();i++)
	{
		minvalue = min(temparray.at(i),minvalue);

	}

	QList<int> returnlist = tempMap.keys(minvalue);

	if (returnlist.isEmpty())
	{
		return 0;
	}else
		return returnlist.at(0);
}

double InterpolationAlgorithm::Getdistans( osg::Vec3 p1, osg::Vec3 p2 )
{
	return sqrt((p2.y() - p1.y()) *(p2.y() - p1.y())  +(p2.x() - p1.x()) *(p2.x() - p1.x()) );
}

void InterpolationAlgorithm::InitGeometry()
{
	pGeom = new osg::Geometry;
	pVertexArray = new osg::Vec3Array;
	texcoords = new osg::Vec2Array;
	texcoords->push_back(osg::Vec2(0.0f,1.0f));
	texcoords->push_back(osg::Vec2(0.0f,0.0f));
	texcoords->push_back(osg::Vec2(1.0f,0.0f));
	texcoords->push_back(osg::Vec2(1.0f,1.0f));
	pColorArray = new osg::Vec4Array;
	pColorArray->push_back(osg::Vec4(1.0f, 1,1,1));
	pColorArray->push_back(osg::Vec4(1.0f, 1,1,1));
	pColorArray->push_back(osg::Vec4(1.0f, 1,1,1));
	pColorArray->push_back(osg::Vec4(1.0f, 1,1,1));
	
	pGeom->setVertexArray(pVertexArray);
	pGeom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::QUADS,0,4));
	pGeom->setTexCoordArray(0, texcoords);
	pGeom->setColorArray(pColorArray);
	pGeom->setColorBinding(osg::Geometry::BIND_PER_VERTEX);
	
	pStateSet = new osg::StateSet;
	///////////////////////////////////设置纹理///////////////////////////////////////
	pTexture2D = new osg::Texture2D;
	pTexture2D->setFilter(osg::Texture2D::MIN_FILTER, osg::Texture2D::LINEAR);
	pTexture2D->setFilter(osg::Texture2D::MAG_FILTER, osg::Texture2D::LINEAR);
	pTexture2D->setDataVariance(osg::Object::DYNAMIC);
	pStateSet->setTextureAttributeAndModes(0,pTexture2D,osg::StateAttribute::ON);
	pStateSet->setMode(GL_BLEND, osg::StateAttribute::ON);
	pGeom->setStateSet(pStateSet);
}


