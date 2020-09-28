#include "VTKInterpolationAlgorithm.h"
//#include "goQuadrilateral.h"
#include <QCoreApplication>
#include <QDate>
#include <QTime>
#include <math.h>
#include <Windows.h>
#include "vtkImageData.h"
#include "vtkColorTransferFunction.h"
#include "vtkVolumeProperty.h"
#include "vtkVolumeRayCastCompositeFunction.h"
#include "vtkVolumeRayCastMapper.h"
#include "vtkVolume.h"
#include "vtkPiecewiseFunction.h"
#define Rm 6371.004; //average value of the Earth radius

InterpolationAlgorithm::InterpolationAlgorithm()
{
	struct_Data = NULL;
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
	m_outData = NULL;
}

InterpolationAlgorithm::~InterpolationAlgorithm()
{
	delete [] m_outData;
}

vtkImageData * InterpolationAlgorithm::GetVolumeData( QHash<int ,QVector4D>colorHash )
{
	ProductBase * pDP = (ProductBase *)m_produceDraw;
	PDBlock* blok = pDP->GetPDBlock();

	if(blok==NULL)
	{
		return NULL;
	}

	int productCode = blok->ProductCode;
	proAttr pAttributeHaHa;
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
			return NULL;
		for (int j = 0;j < layer->NumberOfBlocks ; j++)
		{
			DLIBlock* dblock =pDP-> GetBlock(i,j);
			if(dblock == NULL)
			{
				return NULL;
			}
			if(dblock->TypeOfDataPackets == BLOCKTYPE_RADIAL)
			{
				RadialHead* radialhead = pDP-> GetBlockDataAsRadial_Head(i,j); 

				//double rate = 1.0; /*= 2.0*(double)wight/(double)bitmp.GetHeight()/zoom*/ //精度，需要重新设定
				double rate =  1000.0;
				struct_Data[i].fstgatdis = radialhead->NumberOfGates *radialhead->LengthOfGate*0.0015;/*radialhead->DistanceOfFirstGate;*/
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
	vtkImageData *id=vtkImageData::New();
	id->SetDimensions(m_yNum,m_xNum,m_zNum);
	id->SetOrigin(0,0,0);
	id->AllocateScalars(VTK_UNSIGNED_CHAR,3);

	unsigned char *ptr=(unsigned char *)id->GetScalarPointer(); 
	short tempdata = 0;
	for(int k=0;k<m_zNum;k++)
		for(int i=0;i<m_yNum;i++)
			for(int j=0;j<m_xNum;j++)
			{
				unsigned char temp = outData[k][i][j];
				if (0==temp||1==temp||2==temp||3==temp)
				{
						tempdata = 254;
						*(ptr+k*m_yNum*m_xNum+i*m_xNum+j)=tempdata;
						continue;
				}else
				{
					 tempdata = temp/2-33;
				}
				short key = tempdata/5;
				//qDebug()<< key;
				*(ptr+k*m_yNum*m_xNum+i*m_xNum+j)=key+5;
			}

		return id;
		
		// 流水线
		//vtkRenderer* ren=vtkRenderer::New();
		//ren->AddVolume(volume);

		//vtkRenderWindow* renwin=vtkRenderWindow::New();
		//renwin->AddRenderer(ren);

		//vtkRenderWindowInteractor* iren=vtkRenderWindowInteractor::New();
		//iren->SetRenderWindow(renwin);

		//renwin->Render();
		//iren->Start();
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
					else if(fabs(fAvgEl[ii+1]-fEl)<=fMaxElvDiff)//0.45)
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


