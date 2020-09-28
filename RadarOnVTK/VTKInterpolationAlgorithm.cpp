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

				//double rate = 1.0; /*= 2.0*(double)wight/(double)bitmp.GetHeight()/zoom*/ //���ȣ���Ҫ�����趨
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
		
		// ��ˮ��
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
	float const fBeamWidth=double(0.95);//������� 0.95��
	double fMaxElvDiff = fBeamWidth/2;  //�������������ٽ�����֮�������ֵ

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

	//	double **el=NULL;			//һ��PPI��ÿ��RADIAL������
	double *fAvgEl=NULL;			//һ��PPI��ƽ������
	short **azIndex=NULL,curAzIdx=-1;//��ͬPPI�ϣ�ÿ����λ��(0,1,...359��)��Ӧ������

	//���Ƿ�Χ
	double minEl = 99, maxEl=-99;//��ɨ��������С����

	double const PAI		=	double(3.14159265);
	double const PI0		=	double(3.14159265/180.);
	double const earth_R	=	double(6371.004);			//����뾶 (km)
	double const earth_Rm=	double(4.0*earth_R/3.0);		//��Ч����뾶 (km)

	//CWriteLogFile WriteLogFile;
	double ratio;
	unsigned int fSR;
	double sinRad = sin(radLat*PI0);
	double cosRad = cos(radLat*PI0);
	//-------------------------------------------------------------------------//
	//------------------- e x e c t u a b l e   c o d e  ----------------------//

	//Ϊ��λ�ǽ�������
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

		//��ʼ����λ������ֵ:-1
		memset(azIndex[i], -1, sizeof(short)*360);
		//��ʼ����PPI��ƽ������:0.0
		fAvgEl[i] = 0.;
		memset(tempFlag,false,sizeof(bool)*361);
		for(int k=0; k<strct_Data[i].num_beam; k++)
		{
			int idx = int(strct_Data[i].azim[k]/10.);	//ת�����͵ķ�λ�� ��0-359�ȣ�  !Whyan 2006.04.28
			if(idx>=360)
			{ 
				idx -= 360;
			}
			//���ͷ�λ�Ƕ�Ӧ������
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
	//��ʼ���״����굽�ѿ�������Ĳ�ֵ����

	for(j=0; j<y_Num; j++)
	{
		//	TRACE("%3d\n", j);
		//����γ��
		y=radLat+(j-y_Num/2)*y_Dlt; //�ӵ�γ���γ
		double yy1 = sinRad*sin(y*PI0);
		double yy2 = cosRad*cos(y*PI0);
		double cosyp = cos(y*PI0);
		for(k=0; k<x_Num; k++)
		{
			//����õ㾭��(degree)
			x=radLon+(k-x_Num/2)*x_Dlt; //����
			//���ˮƽ�����״�̫��,���������
			if(fabs(x-radLon)<0.00001 && fabs(y-radLat)<0.00001)continue;

			s = double(earth_R*acos(yy1 + yy2 * cos(x*PI0-radLon*PI0)) );
			//(r����,��)�����з�λ�ǵ�SINֵ
			sina = double(cosyp * sin(x*PI0-radLon*PI0)/sin(s/earth_R));
			if(fabs(sina)>1) continue;

			//(1): fAz  (r����,��)�����еķ�λ��
			if(y>=radLat)
			{
				if(x>=radLon)
					fAzPI = double(asin(sina));
				else
					fAzPI = double(2*PAI+asin(sina));
			}
			else
				fAzPI = double(PAI-asin(sina));

			fAz = fAzPI/PI0;		//�ɻ���ת��Ϊ��

			//------ Ѱ������ķ�λ�� ---------
			//��1��Ϊ��λ�ķ�λ
			curAzIdx = int(fAz+.5); // (0-359 deg)
			if(curAzIdx>=360)
				curAzIdx-=360;

			//TRACE("y=%5.1f, x=%5.1f, az=%3.0f   ", y, x, fAz);
			belta = double(s/earth_Rm);
			double cosbel = cos(belta);
			double sinbel = sin(belta);
			for(i=0; i<z_Num; i++)
			{
				//��ʼ������
				upElIdx  = -1;
				lowElIdx = -1;

				upRIdx  = -1;
				lowRIdx = -1;

				lowWeight=0;
				upWeight =0;

				bND_Flag = bFold_flag = b_NB_FLag = b_CSD_Flag = false;

				//(2):  fElvPhi  (r����,��)�����е�����
				//z=i*z_Dlt+z_Start;// ����õ�߶�(KM)
				z = double(iHeights[i]/1000.);// ����õ�߶�(KM)
				if(x>=117.8 && x<=118. && y>=26. && y<=26.5 && z==5.)
				{
					int jj;
					jj=0;
				}
				fElPI = double(atan((cosbel-earth_Rm/(earth_Rm+z-radHgt) )/sinbel) );

				fEl   = fElPI/PI0;       //�ɻ���ת��Ϊ��

				//(3):  fSRange (r����,��)�����е�б��Slant range (km)
				fSRange = double(sinbel*(earth_Rm+z-radHgt)/cos(fElPI));

				fMaxElvDiff = Min(double(asin(.5f/fSRange)/PI0),0.01f);

				//����õ�(r����,��)���״������е�λ��
				//������������״���ɨ��Χ֮��,����,������������
				if(fEl<minEl-fMaxElvDiff) continue;
				if(fEl>maxEl+fMaxElvDiff)
				{
					//����ɨ���Ƿ�Χ���ڵ�ä��������Ϊ��Ч��ֵ
					outData[i][j][k] = CODE_INVALID;
					continue;
				}

				//1 ȷ���˸�����״���������Ƿ�Χ lowElIdx: �²����ǣ� upElIdx: �ϲ�����,
				//2 ȷ���˸�����״������е�б��--���������λ��б��
				double cosfe = cos(fEl*PI0);
				double cosfel = cos(fElPI);
				for( ii=0; ii<nCuts-1; ii++)
				{
					//(����)λ�ڵ�������
					if(fabs(fEl-fAvgEl[ii])<=fMaxElvDiff)
					{
						//����ⳤ�Ƿ��� �д����������������һ��
						if(strct_Data[ii].gateWidth==0)
						{
							break;
						}
						ratio = cosfe/cos(fAvgEl[ii]*PI0);
						fSR=int(fSRange*1000*ratio);
						lowRIdx = int(fSR/strct_Data[ii].gateWidth+.5);

						if(lowRIdx < strct_Data[ii].num_gate) lowElIdx = ii;

						//���ô�ֱ�ڲ巽��
						lowWeight  = 1;
						upWeight   = 0;
						break;
					}
					//(����)λ�ڸ�������
					else if(fabs(fAvgEl[ii+1]-fEl)<=fMaxElvDiff)//0.45)
					{
						//����ⳤ�Ƿ��� �д����������������һ��
						if(strct_Data[ii+1].gateWidth==0)
						{
							break;
						}

						ratio = cosfel/cos(fAvgEl[ii+1]*PI0);

						int fSR=int(fSRange*1000*ratio);
						upRIdx = int(fSR/strct_Data[ii+1].gateWidth+.5);

						if(upRIdx < strct_Data[ii+1].num_gate)
							upElIdx = ii+1;

						//���ô�ֱ�ڲ巽��
						lowWeight  = 0;
						upWeight   = 1;
						break;
					}
					//�����������������ɨ����������֮�䣬�������������ڵ�����
					else if(fAvgEl[ii]<fEl && fAvgEl[ii+1]>fEl)
					{
						//����ⳤ�Ƿ��� �д����������������һ��
						if(strct_Data[ii].gateWidth ==0 ||
							strct_Data[ii+1].gateWidth <=0) break;

						ratio = cosfe/cos(fAvgEl[ii]*PI0);
						int fSR=int(fSRange*1000*ratio);
						lowRIdx = int(fSR/strct_Data[ii].gateWidth+.5);

						fSR=int(fSRange*1000*cosfel/cos(fAvgEl[ii+1]*PI0));
						upRIdx = int(fSR/strct_Data[ii+1].gateWidth+.5);

						//����Чб�෶Χ��
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

						//ֻ�е����Ǿ��뷶Χ��Чʱ�� �жϵѿ����������Ƿ񳬳������ǵĲ�����ȷ�Χ
						//������Χ����Ĳ�������
						outData[i][j][k] = CODE_INVALID;

						lowWeight  = (fAvgEl[upElIdx]-fEl)/(fAvgEl[upElIdx]-fAvgEl[lowElIdx]);
						upWeight = (fEl-fAvgEl[lowElIdx])/(fAvgEl[upElIdx]-fAvgEl[lowElIdx]);

						break;
					}
				}//�����������

				//�������������֮��Ĳ�
				fDefBetwLow = fEl-fAvgEl[ii];

				if(lowElIdx!=-1 && upElIdx!=-1)//����������Ч������ͬһ�߶ȵĶ�άˮƽ��ֵ�ʹ�ֱ���Բ�ֵ
				{
					//��ȡ�ڽ����������ϣ���ͬб��λ�õ�ֵ
					upV		=	strct_Data[upElIdx].field[azIndex[upElIdx][curAzIdx]][upRIdx];
					lowV	=	strct_Data[lowElIdx].field[azIndex[lowElIdx][curAzIdx]][lowRIdx];

					lowWeight  = (fAvgEl[upElIdx]-fEl)/(fAvgEl[upElIdx]-fAvgEl[lowElIdx]);
					upWeight = (fEl-fAvgEl[lowElIdx])/(fAvgEl[upElIdx]-fAvgEl[lowElIdx]);

					//����Ŀ�����ֵ
					if( (upV>VALUE_INVALID) && (lowV>VALUE_INVALID) )
					{	//���ô�ֱ�ڲ巽��
						outData[i][j][k]=(unsigned char)((upWeight*upV+lowWeight*lowV)/10.*2 + 66. + .5);   //NVI���� /��ֱ�ڲ�
					}
					else if(lowV > VALUE_INVALID)// && fDefBetwLow<=fMaxElvDiff)
					{	//ȡ�Ͳ����ǵ�ֵ
						outData[i][j][k]=(unsigned char)((lowV*lowWeight)/10.*2. + 66. + .5);
						//if(outData[i][j][k]<ucMinRefOnTop) outData[i][j][k]=CODE_INVALID;
					}
					else if(upV > VALUE_INVALID)
					{	//ȡ�߲����ǵ�ֵ
						outData[i][j][k]=(unsigned char)((upV*upWeight)/10.*2. + 66. + .5);
					}
					else //bND_Flag, bFold_flag, b_NB_FLag
					{	//�ж���ʲô������ֵ
						if(upV==VALUE_INVALID || lowV==VALUE_INVALID)
							bND_Flag = true;
						if(upV==VALUE_RANFOLD || lowV==VALUE_RANFOLD)
							bFold_flag = true;
						if(upV<=VALUE_CLUTTER || lowV<= VALUE_CLUTTER)
							b_CSD_Flag = true;

						if(!bND_Flag && bFold_flag && !b_CSD_Flag)		//��Ϊ����ģ��
							outData[i][j][k] = CODE_RANFOLD;
						else if(!bND_Flag && !bFold_flag && b_CSD_Flag) //��Ϊ������������
							outData[i][j][k] = CODE_CSRED;
						else											//���������Ϊ��������
							outData[i][j][k] = CODE_INVALID;
					}
				}
				else if(lowElIdx != -1) //�Ͳ�б����Ч�����õͲ����ǵ���ֵ
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
						outData[i][j][k] = CODE_CSRED; //������������
				}
				else if(upElIdx != -1) //�߲�б����Ч�����ø߲����ǵ���ֵ
				{
					upV	= strct_Data[upElIdx].field[azIndex[upElIdx][curAzIdx]][upRIdx]; //dBZ*10
					if(upV > VALUE_INVALID)
						outData[i][j][k] = (unsigned char)((upV*upWeight)/10.*2 + 66. + .5);
					else if(upV == VALUE_INVALID)
						outData[i][j][k] = CODE_INVALID;
					else if(upV == VALUE_RANFOLD)
						outData[i][j][k] = CODE_RANFOLD;
					else if(upV <= VALUE_CLUTTER)
						outData[i][j][k] = CODE_CSRED; //������������
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

	//�ͷŷ�λ���������桢���ǻ���
	for(i=0; i<nCuts; i++)
	{
		if(azIndex[i] != NULL)	delete []azIndex[i];
	}
	if(azIndex != NULL)		delete []azIndex;
	if(fAvgEl!=NULL)        delete []fAvgEl;

}


