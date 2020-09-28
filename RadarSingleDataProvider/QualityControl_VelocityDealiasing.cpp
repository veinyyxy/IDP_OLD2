// VelUnfold.cpp: implementation of the QualityControl_VelocityDealiasing class.
// MS by yangys for QualityControl_VelocityDealiasing by 080428
//////////////////////////////////////////////////////////////////////
#include <cstdio>
#include <cmath>
#include "QualityControl_VelocityDealiasing.h"
////////////////////////////////////////////////////
#define MAX_UNFOLD_NUM   5
#define MAX_GATE_DIST2   5
#define MAX_GATE_DIST3   20
#define MAX_GATE_DIST4   80
#define MAX_BEAM_DIST2   3
#define MAX_BEAM_DIST3   6 
#define MAX_BEAM_DIST4   10
#define VELNY_SCALE1     0.80
#define VELNY_SCALE2     0.80
#define VELNY_SCALE3     0.85
#define VELNY_SCALE4     0.90

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

QualityControl_VelocityDealiasing::QualityControl_VelocityDealiasing(void)
{
	Init();
}

QualityControl_VelocityDealiasing::~QualityControl_VelocityDealiasing(void)
{
}
void QualityControl_VelocityDealiasing::Init()
{
	m_missing = RADAR_DATA_NOECHO;
	m_range_fold = RADAR_DATA_OUTOFRANGE;
	m_velny = 0;
	m_scale = 1.0;
	Quality_tol = 0;
	m_layer = 0;
	m_AzimuthNumber = 0;
	m_DopplerGates = 0; 
	m_Elevation = 0;
}
void QualityControl_VelocityDealiasing::unfold_tilt_driver(cDrawProduct *pGData,int El_Tilte)
{
	pGData->ReadInformaton(El_Tilte,m_Elevation,m_velny,m_AzimuthNumber,m_DopplerGates,Quality_tol);
	Quality_tol++;
	if (Quality_tol == 1)
	{
		pGData->SetPara(m_velny * 2,Quality_tol);
	}
	else
	{
		pGData->SetPara(m_velny/(2*(Quality_tol - 1) )* 2*Quality_tol,Quality_tol);
	}
	if(m_Elevation> 1090) 
	{
		return;
	}
	m_layer = El_Tilte;
    m_velny = (int)(m_velny/10.0+0.5);//速率，单位为0.1m/s
	unfold(pGData);
	return ;
}

///////////////////////////////////////////////////////////
// FUNCTION: search_1st_beam()
// DESCRIPTION: 应用最小径向速度查找初始参考径向，确定初始参考径向号和该径向上的平均径向速度
///////////////////////////////////////////////////////////
void QualityControl_VelocityDealiasing::search_1st_beam( cDrawProduct *pGData,int &RadialNum,int &AverageRadialVelocity)
{
   int    Gate_Num_Ref = 40;
   //Flag, 布尔型，环向处理数据标识符（1为顺时针，0为逆时针）；
   int    Flag = 0;
   int    Temp_Radial_Velocity = 0,Gate_Num_Tol = 0,Gate_Num = 0,Max_Gate_Num = 0;
   gFloat  temp = 0.0f;
   int    *Temp_Aver_RV = new int[m_AzimuthNumber];
   int    *Temp_Num_RV = new int [m_AzimuthNumber];
   int  Threshold1 = 0,Threshold2 = 0;
   gInt16     **ppTemp = NULL;

   //////////////////////////////////////////////////////////////////////////空间换时间
   ppTemp = new gInt16* [m_AzimuthNumber];

   for (int i =0; i < m_AzimuthNumber; i++)
   {
	   ppTemp[i] = pGData->GetDataV(m_layer,i);
   }

   //initialize variables
   temp = (gFloat)0.3*(gFloat)m_velny;
   Threshold1 = (int)temp;
   temp = (gFloat)VELNY_SCALE3*(gFloat)m_velny; 
   Threshold2 = (int)temp;
   //get AverageRadialVelocity array for each beam
   RadialNum = -1; 
   AverageRadialVelocity = 0;

   for( int j=0; j < m_AzimuthNumber; j++)
   {
      Gate_Num=0; 
	  Temp_Radial_Velocity=0; 
	  Flag = 0;
	  int Gate_Temp1 = m_DopplerGates - 1; 
      for( int k=4; k < Gate_Temp1; k++)
      {
         if(ppTemp[j][k] <= Threshold1 )   
         {
            Temp_Radial_Velocity +=  ppTemp[j][k];
            Gate_Num  += 1;
         }

         if((ppTemp[j][k] > m_range_fold + 1) && (ppTemp[j][k+1] > m_range_fold + 1)
             && (abs(ppTemp[j][k]-ppTemp[j][k+1]) > Threshold2 ))
         {
             Flag = 1;
             break;
         }
      }
      if(Gate_Num > 10 && Flag == 0 )
      {
         Temp_Aver_RV[j] = Temp_Radial_Velocity/Gate_Num;
         Temp_Num_RV[j] = Gate_Num;
      }
      else
      {
          Temp_Num_RV[j] = 0;
          Temp_Aver_RV[j] = m_missing;
		  
      }
   }
   //find the initial radial velocity beam
   int j11,j22,j33;
   Max_Gate_Num = 0;    
   for( int j=0; j < m_AzimuthNumber; j++)
   {
       j11 = j +1 ;
	   j22 = j +2 ;
	   j33 = j +3 ;
       if( j11 >= m_AzimuthNumber) 
	   {
		   j11 -=  m_AzimuthNumber;
	   }
       if( j22 >= m_AzimuthNumber)
	   {
		   j22 -=  m_AzimuthNumber;
	   }
       if( j33 >= m_AzimuthNumber)
	   {
		   j33 -=  m_AzimuthNumber;
	   }
       if((abs(Temp_Aver_RV[j]) < Threshold1) && (abs(Temp_Aver_RV[j11]) < Threshold1) && 
		   (abs(Temp_Aver_RV[j22]) < Threshold1) && (abs(Temp_Aver_RV[j33]) < Threshold1))
       {
          if( (Temp_Aver_RV[j]<0 && Temp_Aver_RV[j11]<0 &&Temp_Aver_RV[j22] > 0 && Temp_Aver_RV[j33] > 0)||
               (Temp_Aver_RV[j]>0 && Temp_Aver_RV[j11]>0 &&Temp_Aver_RV[j22] < 0 && Temp_Aver_RV[j33] < 0))
          {
              if( Temp_Num_RV[j11] > Max_Gate_Num)
              {
                  Max_Gate_Num = Temp_Num_RV[j11];
                  RadialNum = j11;
                  AverageRadialVelocity = (int) Temp_Aver_RV[j11];
              }
          }
       }
   } 
   if( RadialNum == -1)
   {
	   // while(1)
	   for(;;)
      {
          RadialNum = -1;
		  Gate_Num_Tol = 0;
          AverageRadialVelocity = m_velny;   
		  Max_Gate_Num = 0;
          for( int j=0; j < m_AzimuthNumber; j++)
          {
             Gate_Num=0;  
			 Temp_Radial_Velocity=0;  
			 Flag = 0;
			 int  Gate_Temp2 = m_DopplerGates -2;
             for( int k=4; k < Gate_Temp2; k++)
             {	
                if(ppTemp[j][k] > m_range_fold +1)
                {
					Temp_Radial_Velocity +=  abs(ppTemp[j][k]);
					Gate_Num  += 1;
                }

                if((ppTemp[j][k] > m_range_fold + 1) && (ppTemp[j][k+1] > m_range_fold + 1)
                    && (abs(ppTemp[j][k] - ppTemp[j][k+1]) > Threshold2 ))
                {
                   Flag = 1;
                   break;
                }
             }
             if(Gate_Num > Gate_Num_Ref && Flag == 0 )
             {
                Gate_Num_Tol += 1;
                Temp_Radial_Velocity /= Gate_Num;
                if(Temp_Radial_Velocity < AverageRadialVelocity)
                {
                   RadialNum = j;
                   AverageRadialVelocity = (int)Temp_Radial_Velocity;
                }
             }
          }

          if( (Gate_Num_Tol >= 30) && (AverageRadialVelocity <= Threshold1) )
		  {
               break;
		  }
          else
          {
             Gate_Num_Ref = Gate_Num_Ref - 10;
             if( Gate_Num_Ref == 0 ) 
			 {
				 Gate_Num_Ref = 5;
			 }
             if(Gate_Num_Ref <= 0) 
			 {
				 return;
			 }
          }
      }

      int iodd=0;
      int ieven=0;
      for(int k=4; k < m_DopplerGates; k++)
      {
          if( ppTemp[RadialNum][k] > m_range_fold +1 )
		  {
			  if(ppTemp[RadialNum][k] < 0 )
			  {
				  ieven += 1;
			  }
			  else
			  {
				  iodd += 1;
			  }
		  }
      }

      if(iodd < ieven)
	  {	
         AverageRadialVelocity= - AverageRadialVelocity;
	  }
      else if( iodd == ieven)
	  {
        AverageRadialVelocity=0;
	  }
  }
   /*for (int i = 0; i < Azi_temp; i++)
   {
	   delete [] ppTemp[i];
   }*/
   delete [] ppTemp ;
   ppTemp = NULL;
   delete [] Temp_Aver_RV;
   delete [] Temp_Num_RV;
   Temp_Aver_RV = NULL;
   Temp_Num_RV = NULL;
   return ;
}

///////////////////////////////////////////////////////
// FUNCTION: unfold()
// DESCRIPTION: 调用其它函数对基数据退模糊
///////////////////////////////////////////////////////
void QualityControl_VelocityDealiasing::unfold(cDrawProduct *pGData)

{
   gInt16 *ReferenceAverageRadialVelocity = new gInt16[1000];
   int j0 = 0,j1 = 0;
   int** ProcessMark = NULL;
   chkvel(pGData);
   for( int j = 0; j < m_AzimuthNumber; j++)
   {
	   for( int k = 0; k < 4; k++)
	   {
		   pGData->GetDataV(m_layer,j)[k] =m_missing;
	   }
   }

   ProcessMark = new int* [m_AzimuthNumber];
   for( int j = 0; j < m_AzimuthNumber; j++)   
   {
       ProcessMark[j]  = new int [m_DopplerGates];
       for( int k = 0; k < m_DopplerGates; k++)
       {
          if( pGData->GetDataV(m_layer,j)[k] > m_range_fold+1 )
		  {
             ProcessMark[j][k] = 0;
		  }
          else
		  {
			  ProcessMark[j][k] = 3;
		  }
       }
   }

   int  RadialNum = 0;//初始径向号
   int  AverageRadialVelocity = 0;//平均径向速度的平均值
   search_1st_beam(pGData,RadialNum,AverageRadialVelocity);
   if( RadialNum < 0 )
   {
	   return ;
   }
  for( int k=0; k < m_DopplerGates; k++)
  {
     ReferenceAverageRadialVelocity[k] = AverageRadialVelocity; 	 
  }
  shearb1s_initial(ProcessMark ,RadialNum,ReferenceAverageRadialVelocity,pGData);

  j0 = RadialNum +1 ;
  j1 = RadialNum -1 ;

  if(j0 >= m_AzimuthNumber)
  {
	  j0 -= m_AzimuthNumber;
  }
  if(j1 < 0 )
  {
	  j1 +=  m_AzimuthNumber;
  }

  shearb1s_initial(ProcessMark ,j0,ReferenceAverageRadialVelocity,pGData);
  shearb1s_initial(ProcessMark ,j1,ReferenceAverageRadialVelocity,pGData);
  //change reference beam as RadialNum
  chgbeam(RadialNum,ReferenceAverageRadialVelocity,pGData);
  //unfold all left unfolded beams
  int Azimuth_Temp=RadialNum-m_AzimuthNumber/2;

  if(Azimuth_Temp < 0)
  {
	  Azimuth_Temp +=m_AzimuthNumber;
  }
  unfold1(ProcessMark ,RadialNum,pGData);
  unfold2(2,ProcessMark ,RadialNum,pGData);
  unfold2(3,ProcessMark ,RadialNum,pGData);
  unfold2(4,ProcessMark ,RadialNum,pGData);
  unfold2(4,ProcessMark ,Azimuth_Temp,pGData);
  chkvel(pGData);
  for (int i = 0; i < m_AzimuthNumber; i++)
  {
      delete [] ProcessMark[i];
  }
  delete [] ProcessMark ;
  ProcessMark = NULL;
  delete [] ReferenceAverageRadialVelocity;
  ReferenceAverageRadialVelocity = NULL;
  return ;
}

///////////////////////////////////////////////////////
// FUNCTION: unfold1()
// DESCRIPTION:对初始参考径向以外的径向上的速度值从顺时针和逆时针两个方向上小范围退模糊 
///////////////////////////////////////////////////////
void QualityControl_VelocityDealiasing::
unfold1(int** ProcessMark ,int RadialNum,cDrawProduct *pGData)
{
  int Azimuth_Temp = RadialNum - m_AzimuthNumber/2;
  gInt16  *ptemp = pGData->GetDataV(m_layer,RadialNum);

  if(Azimuth_Temp < 0)
  {
	  Azimuth_Temp +=  m_AzimuthNumber;
  }
  if(Azimuth_Temp > RadialNum)
  {
     for(int j=RadialNum; j<=Azimuth_Temp;j++)
     {
         shearb1s(1,ProcessMark ,j,ptemp,pGData);
         chgbeam(j,ptemp,pGData);
     }
  }
  else
  {
       for( int j=RadialNum; j < m_AzimuthNumber; j++)
       {
           shearb1s(1,ProcessMark ,j,ptemp,pGData);
           chgbeam(j,ptemp,pGData);
       }
       for( int j=0; j <= Azimuth_Temp; j++)
       {
            shearb1s(1,ProcessMark ,j,ptemp,pGData);
            chgbeam(j,ptemp,pGData);
       }
  }
  Azimuth_Temp = RadialNum - m_AzimuthNumber/2;

  if(Azimuth_Temp < 0) 
  {
	  Azimuth_Temp +=  m_AzimuthNumber;
  }

  if(Azimuth_Temp > RadialNum)
  {
     for( int j=RadialNum; j >= 0; j--)
     {
        shearb1s(0,ProcessMark ,j,ptemp,pGData);
        chgbeam(j,ptemp,pGData);
     }
     int temp = m_AzimuthNumber-1;
     for( int j = temp; j >= Azimuth_Temp; j--)
     {
        shearb1s(0,ProcessMark ,j,ptemp,pGData);
        chgbeam( j,ptemp,pGData);
     }
  }
  else
  {
        for( int j=RadialNum; j >= Azimuth_Temp; j--)
        {
            shearb1s(0,ProcessMark ,j,ptemp,pGData);
            chgbeam( j,ptemp,pGData);
        }
  }
  return;
}

///////////////////////////////////////////////////////
// FUNCTION: unfold2()
// DESCRIPTION: 对初始参考径向以外的径向上的速度值，从顺时针和逆时针两个方向上逐步扩展邻近搜索范围，进一步速度退模糊
///////////////////////////////////////////////////////
void QualityControl_VelocityDealiasing::
unfold2(int Round_Num, int** ProcessMark ,int RadialNum,cDrawProduct *pGData)
{
  int Azimuth_Temp =RadialNum - m_AzimuthNumber/2;
  gInt16  *ptemp = pGData->GetDataV(m_layer,RadialNum);

  if(Azimuth_Temp < 0)
  {
	  Azimuth_Temp +=  m_AzimuthNumber;
  }

  if(Azimuth_Temp > RadialNum)
  {
     for(int j=RadialNum; j<=Azimuth_Temp;j++)
     {
         shearb1s1(Round_Num,1,ProcessMark ,j,ptemp,pGData);
         chgbeam( j,ptemp,pGData);
     }
  }
  else
  {
       for( int j=RadialNum; j < m_AzimuthNumber; j++)
       {
           shearb1s1(Round_Num,1,ProcessMark ,j,ptemp,pGData);
           chgbeam(j,ptemp,pGData);
       }
       for( int j=0; j <= Azimuth_Temp; j++)
       {
            shearb1s1(Round_Num,1,ProcessMark ,j,ptemp,pGData);
            chgbeam(j,ptemp,pGData);
       }
  }
  Azimuth_Temp = RadialNum - m_AzimuthNumber/2;

  if(Azimuth_Temp < 0)
  {
	  Azimuth_Temp += m_AzimuthNumber;
  }

  if(Azimuth_Temp > RadialNum)
  {
     for( int j=RadialNum; j >= 0; j--)
     {
        shearb1s1(Round_Num,0,ProcessMark ,j,ptemp,pGData);
        chgbeam(j,ptemp,pGData);
     }

     for( int j=m_AzimuthNumber-1; j >= Azimuth_Temp; j--)
     {
        shearb1s1(Round_Num,0,ProcessMark ,j,ptemp,pGData);
        chgbeam( j,ptemp,pGData);
     }
  }
  else
  {
        for( int j=RadialNum; j >= Azimuth_Temp; j--)
        {
            shearb1s1(Round_Num,0,ProcessMark ,j,ptemp,pGData);
            chgbeam( j,ptemp,pGData);
        }
  }

  return;
}

////////////////////////////////////////////////////////////////////
// FUNCTION: chkvel
// DESCRIPTION: 根据某一点的速度与周围49个点的速度的平均值比对结果确定参考值
////////////////////////////////////////////////////////////////////
void QualityControl_VelocityDealiasing::
chkvel(cDrawProduct *pGData)
{ 
	int AverageRadialVelocity = 0;//所有点的和
	int Azimuth_Sta = 0,Azimuth_End = 0;
	int idigit = 0,itotal = 0;//idigit:有效数据点数itotal:所有点数 
	gInt16**  Temp;
	//为提高运行效率，减少函数调用使用二维数组
	Temp = new gInt16* [m_AzimuthNumber];
	for (int i =0; i < m_AzimuthNumber; i++)
	{
			Temp[i] = pGData->GetDataV(m_layer,i);
	}
	for( int j = 0; j < m_AzimuthNumber; j++)
	{
		Azimuth_Sta=j-3;     
		Azimuth_End=j+3;
		for( int k = 4; k < m_DopplerGates; k++)
		{
			AverageRadialVelocity=0; 
			idigit=0;   
			itotal=0;
			int j1 = Azimuth_Sta;
			if(j1 < 0) 
			{
				j1 = 0;
			}
			for(; j1 <= min(Azimuth_End,m_AzimuthNumber-1);j1++)
			{
				for( int k1=max(k-3,4); k1 <= min(k+3,m_DopplerGates-1);k1++)
				{
					itotal += 1;

					if(Temp[j1][k1]>m_range_fold+1)
					{
						AverageRadialVelocity += Temp[j1][k1];
						idigit += 1;
					}
				}
			}
			if(Azimuth_Sta < 0)
			{
				for(int j1=j-3+m_AzimuthNumber; j1< m_AzimuthNumber; j1++)
				{
					for( int k1=max(k-3,4); k1 <= min(k+3,m_DopplerGates-1);k1++)
					{
						itotal += 1;
						if(Temp[j1][k1] > m_range_fold+1 )
						{
							AverageRadialVelocity += Temp[j1][k1];
							idigit += 1;
						}
					}
				}
			}
			if(Azimuth_End >= m_AzimuthNumber)
			{
				for( int j1=0; j1 <= Azimuth_End-m_AzimuthNumber; j1++)
				{
					for( int k1=max(k-3,4); k1 <= min(k+3,m_DopplerGates-1); k1++)
					{
						itotal += 1;
						if(Temp[j1][k1] > m_range_fold+1)
						{
							AverageRadialVelocity += Temp[j1][k1];
							idigit += 1;
						}
					}
				}
			}

			if(itotal > 30 && Temp[j][k] > m_range_fold+1)
			{
				if(idigit <= itotal/5 + 1)
				{
					Temp[j][k] = m_missing;
					continue;
				}
				if(idigit > 0 )
				{
					AverageRadialVelocity = AverageRadialVelocity/idigit;
					rfold(Temp[j][k],(int)AverageRadialVelocity,1.0);
					continue;
				}

			}
			if(itotal > 30 && Temp[j][k] <= m_range_fold+1 && idigit >= itotal-5 )
			{
				AverageRadialVelocity = AverageRadialVelocity/idigit;
				Temp[j][k] = (int)AverageRadialVelocity;
			}
		}
	}
	delete [] Temp ;
	Temp = NULL;
	return;
}

///////////////////////////////////////////////////////
// FUNCTION:   chgbeam
// DESCRIPTION: 更新参考径向，应用移动平均区域平滑径向上的数据，获取新的参考速度值
///////////////////////////////////////////////////////
void QualityControl_VelocityDealiasing::chgbeam( int AzimuthNumber, gInt16* ReferenceAverageRadialVelocity,cDrawProduct *pGData)
{
	gInt16 * ptemp = pGData->GetDataV(m_layer,AzimuthNumber);
   for( int k=4; k < m_DopplerGates; k++)
   {
        ReferenceAverageRadialVelocity[k] = ptemp[k];

   }
  return;
}

////////////////////////////////////////////////////////////////////
// FUNCTION: shearb1s_initial
// DESCRIPTION: 应用平均值，对初始参考径向上的速度退模糊
////////////////////////////////////////////////////////////////////
void QualityControl_VelocityDealiasing::
shearb1s_initial(int** ProcessMark ,int AzimuthNumber,gInt16* ReferenceAverageRadialVelocity,cDrawProduct *pGData)
{
	gInt16 *ptemp = pGData->GetDataV(m_layer,AzimuthNumber);
   for( int k = 4; k < m_DopplerGates; k ++)
   {
	   if( ProcessMark[AzimuthNumber][k] == 0 && ReferenceAverageRadialVelocity[k] > (m_range_fold+1))   
	   {
		   rfold(ptemp[k],ReferenceAverageRadialVelocity[k],1.0);
		   ProcessMark[AzimuthNumber][k] = 1;
	   }
   }
   return;
}

/////////////////////////////////////////////////////////////////////
// FUNCTION: shearb1s1
// DESCRIPTION: 应用邻近退模糊处理过的3个径向上的速度平均值，对除3个初始参考径向以外的径向上的速度进行扩展范围的退模糊处理 
////////////////////////////////////////////////////////////////////
void QualityControl_VelocityDealiasing::shearb1s1(int Round_Num, bool Flag,int** ProcessMark ,int AzimuthNumber,
		                                          gInt16* ReferenceAverageRadialVelocity,cDrawProduct *pGData )
{
   int Azimuth_Temp = 0,Flag_Temp = 0,Num_Reg = 0;
   int j1 = 0; int j2 = 0; int j3 = 0;
   int Max_Gate_Dist = 0,Max_Beam_Dist = 0;
   gDouble Velny_Scale = 0.0;
   int temp = 0,Threshold1 = 0,Threshold2 = 0;
   gInt16 *ptemp = pGData->GetDataV(m_layer,AzimuthNumber);

   //initialize variables
   Max_Gate_Dist = MAX_GATE_DIST2;  
   Max_Beam_Dist = MAX_BEAM_DIST2;
   Velny_Scale   = VELNY_SCALE2;

   if(Round_Num == 3)
   {
      Max_Gate_Dist = MAX_GATE_DIST3;
      Max_Beam_Dist = MAX_BEAM_DIST3;
      Velny_Scale   = VELNY_SCALE3;
   }

   if(Round_Num == 4)
   {
      Max_Gate_Dist = MAX_GATE_DIST4;
      Max_Beam_Dist = MAX_BEAM_DIST4;
      Velny_Scale   = VELNY_SCALE4;
   } 
 
   //get threshold
   Threshold1 = (int)(VELNY_SCALE1*(gFloat)m_velny);   
   Threshold2 = (int)(Velny_Scale*(gFloat)m_velny);   

   //Flag = 1, for clockwise dorection,
   //Flag = 0, for counter clockwise direction
   //unfold using previous equvalent beam values
   for( int k=4; k< m_DopplerGates; k++)
   {
	   if(ProcessMark[AzimuthNumber][k] == 0)
	   {
		   if(Flag == 1 )
		   {
			   Azimuth_Temp = AzimuthNumber - Max_Beam_Dist;
			   if( Azimuth_Temp < AzimuthNumber)
			   {
				   for(int jj=AzimuthNumber; jj>Azimuth_Temp;jj--)
				   {
					  j1 = jj-1; 
					  j2 = jj-2; 
					  j3 = jj-3;
					   if( j1 < 0 )  
					   {
						   j1 +=  m_AzimuthNumber;
					   }
					   if( j2 < 0 )  
					   {
						   j2 +=  m_AzimuthNumber;
					   }
					   if( j3 < 0 )  
					   {
						   j3 +=  m_AzimuthNumber;
					   }
					   gInt16 *P_Temp1 = pGData->GetDataV(m_layer,j1);
					   gInt16 *P_Temp2 = pGData->GetDataV(m_layer,j2);
					   gInt16 *P_Temp3 = pGData->GetDataV(m_layer,j3);
					   if(ProcessMark[j1][k] == 1 && ProcessMark[j2][k] == 1 && ProcessMark[j3][k] == 1 &&
						   abs(P_Temp1[k] - P_Temp2[k]) < Threshold2 &&
						   abs(P_Temp2[k] - P_Temp3[k]) < Threshold2)
					   {
						   temp=(P_Temp1[k] + P_Temp2[k] + P_Temp3[k])/3;

						   rfold(ptemp[k],temp,1.0);

						   ProcessMark[AzimuthNumber][k] = 2; 

						   break;   
					   }
				   }
			   }
			   else
			   {
				   Flag_Temp = 0;
				   for(int jj=AzimuthNumber; jj>=0;jj--)
				   {
					   j1 = jj-1; 
					   j2 = jj-2; 
					   j3 = jj-3;
					   if( j1 < 0 )
					   {
						   j1 +=  m_AzimuthNumber;
					   }
					   if( j2 < 0 )  
					   {
						   j2 +=  m_AzimuthNumber;
					   }
					   if( j3 < 0 )
					   {
						   j3 +=  m_AzimuthNumber;
					   }

					   gInt16 *P_Temp1 = pGData->GetDataV(m_layer,j1);
					   gInt16 *P_Temp2 = pGData->GetDataV(m_layer,j2);
					   gInt16 *P_Temp3 = pGData->GetDataV(m_layer,j3);
					   if(ProcessMark[j1][k] == 1 && ProcessMark[j2][k] == 1 && ProcessMark[j3][k] == 1 &&
						   abs(P_Temp1[k] - P_Temp2[k]) < Threshold2 &&
						   abs(P_Temp2[k] - P_Temp3[k]) < Threshold2 )
					   {
						   temp=(P_Temp1[k] + P_Temp2[k] +  P_Temp3[k])/3;

						   rfold(ptemp[k],temp,1.0);

						   ProcessMark[AzimuthNumber][k] = 2; 

						   Flag_Temp = 1;

						   break;
					   }
				   }

				   if(Flag_Temp == 0)
				   {
					   for(int jj=m_AzimuthNumber-1; jj>Azimuth_Temp;jj--)
					   {
						   j1 = jj-1; 
						   j2 = jj-2; 
						   j3 = jj-3;
						   if( j1 < 0 )
						   {
							   j1 +=  m_AzimuthNumber;
						   }

						   if( j2 < 0 )  
						   {
							   j2 +=  m_AzimuthNumber;
						   }

						   if( j3 < 0 )  
						   {
							   j3 +=  m_AzimuthNumber;
						   }
						   gInt16 *P_Temp1 = pGData->GetDataV(m_layer,j1);
						   gInt16 *P_Temp2 = pGData->GetDataV(m_layer,j2);
						   gInt16 *P_Temp3 = pGData->GetDataV(m_layer,j3);
						   if(ProcessMark[j1][k] == 1 && ProcessMark[j2][k] == 1 && ProcessMark[j3][k] == 1 &&
							   abs(P_Temp1[k] - P_Temp2[k]) < Threshold2 &&
							   abs(P_Temp2[k] - P_Temp3[k]) < Threshold2)
						   {
							   temp=(P_Temp1[k] + P_Temp2[k] + P_Temp3[k])/3;

							   rfold(ptemp[k],temp,1.0);

							   ProcessMark[AzimuthNumber][k] = 2;
							   break;
						   }
					   }
				   }
			   }
		   }
		   else 
		   {
			   Azimuth_Temp = AzimuthNumber + Max_Beam_Dist;
			   if( Azimuth_Temp > AzimuthNumber)
			   {
				   for(int jj=AzimuthNumber; jj<Azimuth_Temp;jj++)
				   {
					   j1 = jj+1; 
					   j2 = jj+2; 
					   j3 = jj+3;
					   if( j1 >=  m_AzimuthNumber ) 
					   {
						   j1 -=  m_AzimuthNumber;
					   }
					   if( j2 >=  m_AzimuthNumber )
					   {
						   j2 -=  m_AzimuthNumber;
					   }
					   if( j3 >=  m_AzimuthNumber )
					   {
						   j3 -=  m_AzimuthNumber;
					   }
					   gInt16 *P_Temp1 = pGData->GetDataV(m_layer,j1);
					   gInt16 *P_Temp2 = pGData->GetDataV(m_layer,j2);
					   gInt16 *P_Temp3 = pGData->GetDataV(m_layer,j3);
					   if(ProcessMark[j1][k] == 1 && ProcessMark[j2][k] == 1 && ProcessMark[j3][k] == 1 &&
						   abs(P_Temp1[k] - P_Temp2[k]) < Threshold2 &&
						   abs(P_Temp2[k] - P_Temp3[k]) < Threshold2 )
					   {
						   temp=(P_Temp1[k] + P_Temp2[k] + P_Temp3[k])/3;

						   rfold(ptemp[k],temp,1.0);

						   ProcessMark[AzimuthNumber][k] = 2;    
						   break;
					   }
				   }
			   }
			   else
			   {
				   Flag_Temp = 0;
				   for(int jj=AzimuthNumber; jj < m_AzimuthNumber;jj++)
				   {
					   j1 = jj+1; 
					   j2 = jj+2; 
					   j3 = jj+3;
					   if( j1 >=  m_AzimuthNumber ) 
					   {
						   j1 -=  m_AzimuthNumber;
					   }
					   if( j2 >=  m_AzimuthNumber ) 
					   {
						   j2 -=  m_AzimuthNumber;
					   }
					   if( j3 >=  m_AzimuthNumber )
					   {
						   j3 -=  m_AzimuthNumber;
					   }
					   gInt16 *P_Temp1 = pGData->GetDataV(m_layer,j1);
					   gInt16 *P_Temp2 = pGData->GetDataV(m_layer,j2);
					   gInt16 *P_Temp3 = pGData->GetDataV(m_layer,j3);
					   if(ProcessMark[j1][k] == 1 && ProcessMark[j2][k] == 1 && ProcessMark[j3][k] == 1 &&
						   abs(P_Temp1[k] - P_Temp2[k]) < Threshold2 &&
						   abs(P_Temp2[k] - P_Temp3[k]) < Threshold2)
					   {
						   temp=(P_Temp1[k] + P_Temp2[k] + P_Temp3[k])/3;

						   rfold(ptemp[k],temp,1.0);

						   ProcessMark[AzimuthNumber][k] = 2;

						   Flag_Temp = 1; 

						   break;
					   }
				   }

				   if(Flag_Temp == 0)
				   {
					   for(int jj=0; jj<Azimuth_Temp;jj++)
					   {
						   j1 = jj+1; 
						   j2 = jj+2; 
						   j3 = jj+3;
						   if( j1 >=  m_AzimuthNumber )  
						   {
							   j1 -=  m_AzimuthNumber;
						   }
						   if( j2 >=  m_AzimuthNumber ) 
						   {
							   j2 -=  m_AzimuthNumber;
						   }
						   if( j3 >=  m_AzimuthNumber )
						   {
							   j3 -=  m_AzimuthNumber;
						   }
						   gInt16 *P_Temp1 = pGData->GetDataV(m_layer,j1);
						   gInt16 *P_Temp2 = pGData->GetDataV(m_layer,j2);
						   gInt16 *P_Temp3 = pGData->GetDataV(m_layer,j3);
						   if(ProcessMark[j1][k] == 1 && ProcessMark[j2][k] == 1 && ProcessMark[j3][k] == 1 &&
							   abs(P_Temp1[k] - P_Temp2[k]) < Threshold2 &&
							   abs(P_Temp2[k] - P_Temp3[k]) < Threshold2 )
						   {
							   temp=(P_Temp1[k] + P_Temp2[k] + P_Temp3[k])/3;

							   rfold(ptemp[k],temp,1.0);

							   ProcessMark[AzimuthNumber][k] = 2; 
							   break;
						   }
					   }
				   }
			   }
		   }//if(Flag == 1 )

	   }
   }
   //find initial point
   int Gate_First = -1; 
   Num_Reg = 0; 
   int Gate_temp_6 = m_DopplerGates - 6;
   for(int k = Gate_temp_6; k > 4; k--)
   {
	   if( (ProcessMark[AzimuthNumber][k] == 1 || ProcessMark[AzimuthNumber][k] == 2) && 
		   abs(ptemp[k] - ReferenceAverageRadialVelocity[k]) < Threshold1 &&
		   (ProcessMark[AzimuthNumber][k+1] == 1 || ProcessMark[AzimuthNumber][k+1] == 2) && 
		   abs(ptemp[k+1] - ReferenceAverageRadialVelocity[k+1]) < Threshold1 &&
		   (ProcessMark[AzimuthNumber][k+2] == 1 || ProcessMark[AzimuthNumber][k+2] == 2) && 
		   abs(ptemp[k+2] - ReferenceAverageRadialVelocity[k+2]) < Threshold1 &&
		   abs(ptemp[k+1] - ptemp[k]) < Threshold1 && abs(ptemp[k+1] -ptemp[k+2]) < Threshold1 )
	   {
		   Num_Reg++;
		   if(Num_Reg > 2 )
		   {
			   if( Flag == 1 )
			   {
				   j1 = AzimuthNumber-(int)( (gFloat)(600/k) - 0.5);
				   if( j1 < 0 )
				   {
					   j1 +=  m_AzimuthNumber;
				   }
			   }
			   else if( Flag == 0 )
			   {
				   j1 = AzimuthNumber+(int)( (gFloat)(600/k) + 0.5 );

				   if( j1 >=  m_AzimuthNumber ) 
				   {
					   j1 -= m_AzimuthNumber;
				   }
					   
			   }
			   if( ProcessMark[j1][k] <= 2 )
			   { 
				   Gate_First= k + 2;
				   break;
			   }
		   }
	   }
	   else
	   {
		   Num_Reg = 0;
	   }
   }
   if(Gate_First == -1 ) 
   {
	   return;
   }

   //unfold all left values and
   //check if there are any aliased point and unfold it
   for( int k= Gate_First-2; k > 4 ; k--)
   {     
      if( ProcessMark[AzimuthNumber][k]== 0 || ProcessMark[AzimuthNumber][k]== 2)
	  {
		  int Gate_temp_5 = m_DopplerGates - 5;
		  for(int kk = k; kk <= min( k+Max_Gate_Dist, Gate_temp_5); kk++)
		  { 
			  if( ( ProcessMark[AzimuthNumber][kk+1] == 1 || ProcessMark[AzimuthNumber][kk+1] == 2) &&
				  ( ProcessMark[AzimuthNumber][kk+2] == 1 || ProcessMark[AzimuthNumber][kk+2] == 2) &&
				  ( ProcessMark[AzimuthNumber][kk+3] == 1 || ProcessMark[AzimuthNumber][kk+3] == 2) &&
				  abs(ptemp[kk+1] - ptemp[kk+2]) < Threshold2 &&
				  abs(ptemp[kk+2] - ptemp[kk+3]) < Threshold2 )
			  {
				  temp = ( ptemp[kk+1] + ptemp[kk+2] + ptemp[kk+3])/3;

				  rfold(ptemp[k],temp,1.0);

				  ProcessMark[AzimuthNumber][k] = 1;
				  break;
			  }
		  }
	  }
      
        
      if( ProcessMark[AzimuthNumber][k]== 0 || ProcessMark[AzimuthNumber][k]== 2 )
	  {
		  for( int kk = k; kk >= max( k-Max_Gate_Dist,4); kk--)
		  {    
			  if( ( ProcessMark[AzimuthNumber][kk-1] == 1 || ProcessMark[AzimuthNumber][kk-1] == 2) &&
				  ( ProcessMark[AzimuthNumber][kk-2] == 1 || ProcessMark[AzimuthNumber][kk-2] == 2) &&
				  ( ProcessMark[AzimuthNumber][kk-3] == 1 || ProcessMark[AzimuthNumber][kk-3] == 2) &&
				  abs(ptemp[kk-1] - ptemp[kk-2]) < Threshold2 &&
				  abs(ptemp[kk-2] - ptemp[kk-3]) < Threshold2 )
			  {
				  temp = ( ptemp[kk-1] + ptemp[kk-2] + ptemp[kk-3])/3;

				  rfold(ptemp[k],temp,1.0);

				  ProcessMark[AzimuthNumber][k] = 1;

				  break;
			  }
		  }
	  }
      
   }
   int Gate_First2 = Gate_First + 2;
   int Gate_temp_4 = m_DopplerGates - 4;
   for( int k= Gate_First2; k < Gate_temp_4; k++)
   {             
      if( ProcessMark[AzimuthNumber][k]== 0 || ProcessMark[AzimuthNumber][k]== 2 )
	  {
		  for( int kk = k; kk >= max( k-Max_Gate_Dist,4); kk--)
		  {             
			  if( ( ProcessMark[AzimuthNumber][kk-1] == 1 || ProcessMark[AzimuthNumber][kk-1] == 2) &&
				  ( ProcessMark[AzimuthNumber][kk-2] == 1 || ProcessMark[AzimuthNumber][kk-2] == 2) &&
				  ( ProcessMark[AzimuthNumber][kk-3] == 1 || ProcessMark[AzimuthNumber][kk-3] == 2) &&
				  abs(ptemp[kk-1] - ptemp[kk-2]) < Threshold2 &&
				  abs(ptemp[kk-2] - ptemp[kk-3]) < Threshold2 )
			  {              
				  temp = ( ptemp[kk-1] + ptemp[kk-2] + ptemp[kk-3])/3;

				  rfold(ptemp[k],temp,1.0);

				  ProcessMark[AzimuthNumber][k] = 1;
				  break;
			  }
		  }
	  }
      if( ProcessMark[AzimuthNumber][k]== 0 || ProcessMark[AzimuthNumber][k]== 2)
	  {
		  int Dop_Temp = m_DopplerGates-5;
		  for( int kk = k; kk <= min( k+Max_Gate_Dist, Dop_Temp); kk++)
		  { 
			  if( ( ProcessMark[AzimuthNumber][kk+1] == 1 || ProcessMark[AzimuthNumber][kk+1] == 2) &&
				  ( ProcessMark[AzimuthNumber][kk+2] == 1 || ProcessMark[AzimuthNumber][kk+2] == 2) &&
				  ( ProcessMark[AzimuthNumber][kk+3] == 1 || ProcessMark[AzimuthNumber][kk+3] == 2) &&
				  abs(ptemp[kk+1] - ptemp[kk+2]) < Threshold2 &&
				  abs(ptemp[kk+2] - ptemp[kk+3]) < Threshold2 )
			  {
				  temp = ( ptemp[kk+1] + ptemp[kk+2] + ptemp[kk+3])/3;

				  rfold(ptemp[k],temp,1.0);

				  ProcessMark[AzimuthNumber][k] = 1;
				  break;
			  }
		  }
	  }
      
   }
   return;
}

////////////////////////////////////////////////////////////////////
// FUNCTION: shearb1s
// DESCRIPTION: 应用邻近退模糊处理过的3个径向上的速度平均值，对除3个初始参考径向以外的径向上的速度进行小范围退模糊 
//
////////////////////////////////////////////////////////////////////
void QualityControl_VelocityDealiasing::
shearb1s(bool Flag,int** ProcessMark ,int AzimuthNumber,gInt16* ReferenceAverageRadialVelocity,cDrawProduct *pGData)
{
   int Gate_First,j1,j2,j3,Gate_Temp;
   int *ReferenceAverageRadialVelocitytemp = new int[1000];
   int temp,Threshold;
   gInt16 *ptemp = pGData->GetDataV(m_layer,AzimuthNumber);

   //initialize Threshold
   Threshold = (int)(VELNY_SCALE1*(gFloat)m_velny);

   //get previous three beams location
   //Flag = 1, for clockwise dorection,
   //Flag = 0, for counter clockwise direction
   j1 = AzimuthNumber; 
   j2 = AzimuthNumber; 
   j3 = AzimuthNumber;           
   if( Flag == 1 )
   {
      j1 = AzimuthNumber-1; 
	  j2 = AzimuthNumber-2; 
	  j3 = AzimuthNumber-3; 
      if( j1 < 0 )  
	  {
		  j1 +=  m_AzimuthNumber;
	  }
      if( j2 < 0 ) 
	  {
		  j2 +=  m_AzimuthNumber;
	  }
      if( j3 < 0 ) 
	  {
		  j3 +=  m_AzimuthNumber;
	  }
   }
   if( Flag == 0 )
   {
      j1 = AzimuthNumber+1;
	  j2 = AzimuthNumber+2;
	  j3 = AzimuthNumber+3;  
      if( j1 >=  m_AzimuthNumber ) 
	  {
		  j1 -=  m_AzimuthNumber;
	  }
      if( j2 >=  m_AzimuthNumber )  
	  {
		  j2 -=  m_AzimuthNumber;
	  }

      if( j3 >=  m_AzimuthNumber ) 
	  {
		  j3 -=  m_AzimuthNumber;
	  }
   }
   gInt16 *P_Temp1 = pGData->GetDataV(m_layer,j1);
   gInt16 *P_Temp2 = pGData->GetDataV(m_layer,j2);
   gInt16 *P_Temp3 = pGData->GetDataV(m_layer,j3);

   //unfold using previous equvalent beam values
   for( Gate_Temp=4; Gate_Temp < m_DopplerGates; Gate_Temp++)
   {	   
        if( ProcessMark[AzimuthNumber][Gate_Temp] == 0 && ProcessMark[j1][Gate_Temp] == 1 &&
            ProcessMark[j2][Gate_Temp] == 1 && ProcessMark[j3][Gate_Temp] == 1 &&
            abs(P_Temp1[Gate_Temp] - P_Temp2[Gate_Temp]) < Threshold &&
            abs(P_Temp2[Gate_Temp] - P_Temp3[Gate_Temp]) < Threshold) 
        {
            temp=(P_Temp1[Gate_Temp] + P_Temp2[Gate_Temp] + P_Temp3[Gate_Temp])/3;

            rfold(ptemp[Gate_Temp],temp,1.0);

            ProcessMark[AzimuthNumber][Gate_Temp] = 1;
        }
   }
   for( Gate_Temp=4; Gate_Temp < m_DopplerGates; Gate_Temp++)
   {
	   ReferenceAverageRadialVelocitytemp[Gate_Temp] = ptemp[Gate_Temp];
   }
	   
  int Gate_Sta1 = 0,Num_Reg = 0;
  int Gate_Sta2 = m_DopplerGates - 6; 
   
  // while(1)
  for(;;)
   {
      //search initial point
      Gate_First = -1;  
      Gate_Sta1 = Gate_Sta2;  
      Num_Reg = 0;
      for(Gate_Temp = Gate_Sta1; Gate_Temp > 4; Gate_Temp--)
	  {
		  if( ProcessMark[AzimuthNumber][Gate_Temp] == 1 && 
			  abs(ptemp[Gate_Temp]-ReferenceAverageRadialVelocity[Gate_Temp]) < Threshold &&
			  ProcessMark[AzimuthNumber][Gate_Temp+1] == 1 && 
			  abs(ptemp[Gate_Temp+1] - ReferenceAverageRadialVelocity[Gate_Temp+1]) < Threshold && 
			  ProcessMark[AzimuthNumber][Gate_Temp+2] == 1 && 
			  abs(ptemp[Gate_Temp+2] - ReferenceAverageRadialVelocity[Gate_Temp+2]) < Threshold && 
			  abs(ptemp[Gate_Temp+1] - ptemp[Gate_Temp]) < Threshold &&
			  abs(ptemp[Gate_Temp+1] - ptemp[Gate_Temp+2]) < Threshold )
		  {
			  Num_Reg++;
			  if(Num_Reg > 4 )
			  {
				  if( Flag == 1 )
				  {
					  j1 = AzimuthNumber-(int)( (gFloat)(600/Gate_Temp) - 0.5);
					  if( j1 < 0 ) 
					  {
						  j1 +=  m_AzimuthNumber;
					  }
				  }
				  else if( Flag == 0 )
				  {
					  j1 = AzimuthNumber+(int)( (gFloat)(600/Gate_Temp) + 0.5 ); 
					  if( j1 >=  m_AzimuthNumber )  
					  {
						  j1 -=  m_AzimuthNumber;
					  }
				  }

				  if( ProcessMark[j1][Gate_Temp] <= 2 )
				  { 
					  Gate_First= Gate_Temp + 3;
					  break;
				  }
			  }
		  }
		  else  
		  {
			  Num_Reg = 0;
		  }
		  if(Gate_First == -1 )
		  {
			  break;
		  }
	  }
      //unfold using same beam values
      for( Gate_Temp=Gate_First + 2; Gate_Temp < Gate_Sta1; Gate_Temp++)
      {
           if( ProcessMark[AzimuthNumber][Gate_Temp] <= 1 && 
			   ProcessMark[AzimuthNumber][Gate_Temp-1] == 1&& 
			   ProcessMark[AzimuthNumber][Gate_Temp-2] == 1 && 
			   ProcessMark[AzimuthNumber][Gate_Temp-3] == 1)
           {
                ReferenceAverageRadialVelocitytemp[Gate_Temp-2] = 
					( ReferenceAverageRadialVelocitytemp[Gate_Temp-1] +
					ReferenceAverageRadialVelocitytemp[Gate_Temp-2] + 
					ReferenceAverageRadialVelocitytemp[Gate_Temp-3])/3;

                if( abs(ptemp[Gate_Temp-1] - ptemp[Gate_Temp-2]) < Threshold && 
                    abs(ptemp[Gate_Temp-2] - ptemp[Gate_Temp-3]) < Threshold )
                {
                    rfold(ptemp[Gate_Temp],ReferenceAverageRadialVelocity[Gate_Temp-2],1.0);

                    ReferenceAverageRadialVelocitytemp[Gate_Temp] = ptemp[Gate_Temp];
                    ProcessMark[AzimuthNumber][Gate_Temp] = 1;   
                }
           }
           else  
		   {
			   break;
		   }
       }

       for( Gate_Temp = Gate_First-2; Gate_Temp >= 4; Gate_Temp--)
       {
            if(ProcessMark[AzimuthNumber][Gate_Temp] <= 1 &&
			    ProcessMark[AzimuthNumber][Gate_Temp+1] == 1 && 
				ProcessMark[AzimuthNumber][Gate_Temp+2] == 1 && 
				ProcessMark[AzimuthNumber][Gate_Temp+3] == 1)
            {
                ReferenceAverageRadialVelocitytemp[Gate_Temp+2] = 
					(ReferenceAverageRadialVelocitytemp[Gate_Temp+1] +
					ReferenceAverageRadialVelocitytemp[Gate_Temp+2] + 
					ReferenceAverageRadialVelocitytemp[Gate_Temp+3] )/3;

                if( abs(ptemp[Gate_Temp+1] - ptemp[Gate_Temp+2]) < Threshold && 
                    abs(ptemp[Gate_Temp+2] - ptemp[Gate_Temp+3]) < Threshold )
                {
                    rfold(ptemp[Gate_Temp],ReferenceAverageRadialVelocitytemp[Gate_Temp+2],1.0);

                    ReferenceAverageRadialVelocitytemp[Gate_Temp] = ptemp[Gate_Temp];
                    ProcessMark[AzimuthNumber][Gate_Temp] = 1;
                }
            }
            else
            {
                Gate_Sta2 = Gate_Temp;
                break;
            }
       }
       if( Gate_Temp < 6 )
	   {
		   break;
	   }
   }
   delete []ReferenceAverageRadialVelocitytemp;
   ReferenceAverageRadialVelocitytemp = NULL;

   return;
}

///////////////////////////////////////////////////////
// FUNCTION:   rfold
// DESCRIPTION: 在参考平均径向速度的基础上，对单个点的速度值进行退模糊处理
//    
///////////////////////////////////////////////////////
void QualityControl_VelocityDealiasing::
rfold(gInt16& Data_V,int ReferenceAverageRadialVelocity,gFloat Scale)
{
   int Num_Reg=0;
   int temp = 0;
   // while(1)
   for(;;)
   {
        temp=(int)abs(Data_V-ReferenceAverageRadialVelocity);
		
        if(temp > (int)(Scale*(gFloat)m_velny) )
		{
            Num_Reg += 1;
		}
        else
		{
			break;
		}
        if(Num_Reg > 3)
        {	
            Data_V= ReferenceAverageRadialVelocity;
            break;
        }
        if (Quality_tol == 1)
        {
			if(Data_V > ReferenceAverageRadialVelocity)
			{
				Data_V -= 2*m_velny;
			}
			else
			{
				Data_V += 2*m_velny;
			}
        }
		else
		{
			if(Data_V > ReferenceAverageRadialVelocity)
			{
				Data_V -= 2*m_velny/(2*(Quality_tol - 1));
			}
			else
			{
				Data_V += 2*m_velny/(2*(Quality_tol - 1));
			}
		}  
   }
   return;
}
