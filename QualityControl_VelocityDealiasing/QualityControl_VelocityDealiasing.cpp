// VelUnfold.cpp: implementation of the QualityControl_VelocityDealiasing class.
// MS by yangys for QualityControl_VelocityDealiasing by 080428
//////////////////////////////////////////////////////////////////////
#ifndef IARITHMS_LIB
	#define IARITHMS_LIB
#endif
#include <cstdio>
#include <cmath>
#include "../GDataTypes/IBase.h"
#include "QualityControl_VelocityDealiasing.h"
#include "../GDataTypes/CJulMsGMT.h"
#include <fstream>
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
}

QualityControl_VelocityDealiasing::~QualityControl_VelocityDealiasing(void)
{
}
GHRESULT QualityControl_VelocityDealiasing::Initialize()
{
	m_pGenerateProductList = NULL;
	m_pOutputProductList = NULL;
	return GS_OK;
}
GHRESULT QualityControl_VelocityDealiasing::UnInitialize()
{
	return GS_OK;
}
GHRESULT QualityControl_VelocityDealiasing::LoadData(void *pValue,ReadParam * ReadParameter)
{
	if ( ReadParameter == NULL )
	{
		return GE_INVALIDARG;
	}
	ReadP=ReadParameter;
	if ( pValue == NULL )
	{
		return GE_INVALIDARG;
	}

	m_pGenerateProductList = (GXList< GenerationData > *)pValue;//取得输入数据列表
	return GS_OK;
}
GHRESULT QualityControl_VelocityDealiasing::Execute()
{
	ofstream logfile;
	logfile.open("./Log/QualityControl_VelocityDealiasing.txt",ios::app);

	gUint16 iJulDay;
	gInt32 iMilliSecond,iYear,iMonth,iDay,iHour,iMinute,iSecond,iMSecond;
	CJulMsGMT::GetLocalJulMs(iJulDay, iMilliSecond );
	CJulMsGMT::Jul2Greg(iJulDay,iYear,iMonth,iDay);
	CJulMsGMT::MilliSecond2Time(iMilliSecond,iHour,iMinute,iSecond,iMSecond);
	logfile<<endl<<iHour<<":"<<iMinute<<":"<<iSecond<<"	"<<iYear<<"-"<<iMonth<<"-"<<iDay<<endl;

	logfile<<"QC_VelocityDealiasing：  Started: "<<endl;

	logfile<<"QC_VelocityDealiasing：  Get Input Product. "<<endl;
	StandardRadarData * pGData = &(*(m_pGenerateProductList->GetHeadAs<StandardRadarData>()));
	logfile<<"QC_VelocityDealiasing：  Send in and Read Parameters."<<endl;
 	char * pTempName = NULL;
 	(*ReadP)("Quality_tol",pTempName);
	if(pTempName == NULL)
		return GE_FAIL;
	Quality_tol = atoi(pTempName);

	pTempName = NULL;
	(*ReadP)("El_Tilte",pTempName);  //modify by dqc 20080923
	if(pTempName == NULL)
		return GE_FAIL;
	gInt32 El_Tilte = atoi(pTempName);

	logfile<<"QC_VelocityDealiasing：  Call Function: unfold_tilt_driver(pGData,El_Tilte). "<<endl;
	unfold_tilt_driver(pGData,El_Tilte); 
	logfile<<"QC_VelocityDealiasing：  Function Finished. "<<endl;

	logfile<<"QC_VelocityDealiasing：  GetElevationHeadP(). "<<endl;
	if (Quality_tol == 1)
	{
		pGData->GetElevationHeadP(El_Tilte)->NyquistVelocity = pGData->GetElevationHeadP(El_Tilte)->NyquistVelocity * 2 ;
	}
	else
	{
		pGData->GetElevationHeadP(El_Tilte)->NyquistVelocity = pGData->GetElevationHeadP(El_Tilte)->NyquistVelocity/(2 * (Quality_tol - 1)) * (2*Quality_tol);
	}

	logfile<<"QC_VelocityDealiasing：  Create Output Product."<<endl;
	m_pOutputProductList->AddTail();
	StandardRadarData *pOutData =  &(*(m_pOutputProductList->GetHeadAs<StandardRadarData>()));

	logfile<<"QC_VelocityDealiasing：  Get Head and Elevation Head Information And Write Into Output Product."<<endl;
	GRADARDATAHEADER * pGDataHead = new GRADARDATAHEADER;
	gUint16 ElevationNumber = pGData->GetHeadP()->ElevationCount;//gInt16(1);
	pGDataHead->ElevationCount = ElevationNumber;
	//pGDataHead->ID = pGData->GetHeadP()->ID;
	//pGDataHead->maxAziNum = pGData->GetHeadP()->maxAziNum;
	//pGDataHead->maxRefgatenum = pGData->GetElevationHeadP(El_Tilte)->ReflectivityGates;

	//memcpy(pGDataHead->IndexOfElevationData ,pGData->GetHeadP()->IndexOfElevationData,sizeof(gInt32)*20);

	//pGDataHead->maxVelgatenum = pGData->GetElevationHeadP(El_Tilte)->DopplerGates;  
	
	pGDataHead->RadarHeight = pGData->GetHeadP()->RadarHeight;
	pGDataHead->RadarLat = pGData->GetHeadP()->RadarLat;
	pGDataHead->RadarLon = pGData->GetHeadP()->RadarLon;

	memcpy(pGDataHead->ScanMode, pGData->GetHeadP()->ScanMode, sizeof(gInt8)*2);
	memcpy(pGDataHead->SiteCode , pGData->GetHeadP()->SiteCode,sizeof(gInt8)*8);
	//memcpy(pGDataHead->SiteName,pGData->GetHeadP()->SiteName,sizeof(gInt8)*18);
	memcpy(pGDataHead->RadarType, pGData->GetHeadP()->RadarType, sizeof(gInt8)*4);
	pGDataHead->VCPTime = pGData->GetHeadP()->VCPTime;
	pGDataHead->VCPDate = pGData->GetHeadP()->VCPDate;
	memcpy(pGDataHead->VCPMode, pGData->GetHeadP()->VCPMode, sizeof(gInt8)*4) ;

	//pOutData->WriteHead(pGDataHead);
	

	GELEVATIONDATAHEAD * pGEleDataHead = new GELEVATIONDATAHEAD[ElevationNumber];
	for (int i=0;i<pGData->GetHeadP()->ElevationCount;i++)
	{
		pGEleDataHead[i].SNOfElevationData = i+1;
		pGEleDataHead[i].UnambiguousRange = pGData->GetElevationHeadP(i) -> UnambiguousRange;
		pGEleDataHead[i].NyquistVelocity = pGData->GetElevationHeadP(i) -> NyquistVelocity;
		pGEleDataHead[i].ReflectivityResolution =pGData->GetElevationHeadP(i)->ReflectivityResolution;//未知数据，置0
		pGEleDataHead[i].VelocityResolution = pGData->GetElevationHeadP(i) -> VelocityResolution;
		pGEleDataHead[i].SpectrumWidthOfVelocityResolution = pGData->GetElevationHeadP(i)->SpectrumWidthOfVelocityResolution;//未知数据，置0
		pGEleDataHead[i].FirstGateRangeOfRef = pGData->GetElevationHeadP(i) -> FirstGateRangeOfRef;
		pGEleDataHead[i].FirstGateRangeOfDoppler = pGData->GetElevationHeadP(i) -> FirstGateRangeOfDoppler;
		pGEleDataHead[i].ReflectivityGates = pGData->GetElevationHeadP(i)->ReflectivityGates;
		pGEleDataHead[i].DopplerGates = pGData->GetElevationHeadP(i)->DopplerGates;
		pGEleDataHead[i].Elevation = pGData->GetElevationHeadP(i)->Elevation;//单位0.01度;
		pGEleDataHead[i].AzimuthNumber = pGData->GetElevationHeadP(i)->AzimuthNumber;
		//pGEleDataHead = pGData->GetElevationHeadP(i);
		//pOutData->WriteElevationHead(i,pGEleDataHead);
	}
	GHRESULT gResult;
	if((gResult = pOutData->WriteHeadAndElevationHead( pGDataHead,pGEleDataHead,ElevationNumber)) != GS_OK)
	{
		delete pGDataHead;
		pGDataHead = NULL;
		delete []pGEleDataHead;
		pGEleDataHead = NULL;
		return gResult;
	}
	
	delete pGDataHead;
	pGDataHead = NULL;
	delete []pGEleDataHead;
	pGEleDataHead = NULL;

	/*GELEVATIONDATAHEAD *pGEleDataHead;
	pGEleDataHead = pGData->GetElevationHeadP(El_Tilte);
	pOutData->WriteElevationHead(0,pGEleDataHead);*/

	logfile<<"QC_VelocityDealiasing：  Loop:k=0-->ElevationCount and i=0-->AzimuthNumber(360) to Write RadarData to Output Product."<<endl;
	for (int k=0;k< pGData->GetHeadP()->ElevationCount;k++)
	{
		for( int i = 0 ;i< pGData->GetElevationHeadP(k)->AzimuthNumber; i++ )
		{
			RVWDATAHEAD *pOutRVWHead = pOutData->GetRadarDataP(k, i);
			RVWDATAHEAD *pInRVWHead = pGData->GetRadarDataP(k, i);
			pOutRVWHead->Azimuth = pInRVWHead->Azimuth;
			pOutRVWHead->Date = pInRVWHead->Date;
			pOutRVWHead->Elevation = pInRVWHead->Elevation;
			pOutRVWHead->Time = pInRVWHead->Time;
			gInt16 *pOutRData = pOutData->GetRadarDataR(k, i);
			gInt16 *pOutVData = pOutData->GetRadarDataV(k, i);
			gInt16 *pOutWData = pOutData->GetRadarDataW(k, i);
			gInt16 *pInRData = pGData->GetRadarDataR(k, i);
			gInt16 *pInVData = pGData->GetRadarDataV(k, i);
			gInt16 *pInWData = pGData->GetRadarDataW(k, i);
			for(int j =0; j< pGData->GetElevationHeadP(k)->ReflectivityGates; j++)
				pOutRData[j] = pInRData[j];
			for(int j =0; j< pGData->GetElevationHeadP(k)->DopplerGates; j++)
			{
				pOutVData[j] = pInVData[j];
				pOutWData[j] = pInWData[j];
			}			
		}
	}
	logfile<<"QC_VelocityDealiasing：  Loop Finished."<<endl;
	logfile<<"QC_VelocityDealiasing：  Finished."<<endl;
	logfile.close();

	return GS_OK;  
}
GHRESULT QualityControl_VelocityDealiasing::OutPutData(void * pValue)
{
	m_pOutputProductList=(GXList< GenerationData > *)pValue;
	if (m_pOutputProductList==NULL)
	{
		return GS_FALSE;
	}

	return GS_OK;
}

//下面是私有函数实现
void QualityControl_VelocityDealiasing::
unfold_tilt_driver(StandardRadarData *pGData,gInt32 El_Tilte)
{
	if(pGData->GetElevationHeadP(El_Tilte)->Elevation > 1090) 
	{
		return;
	}
    m_scale       = 1.0;                          
    m_missing     = RADAR_DATA_NOECHO;
    m_range_fold  = RADAR_DATA_OUTOFRANGE;
	
    m_velny = (gInt16)(pGData->GetElevationHeadP(El_Tilte)->NyquistVelocity/10.0+0.5);//速率，单位为0.1m/s
	
	unfold(pGData,El_Tilte);

	return ;
}

///////////////////////////////////////////////////////////
// FUNCTION: search_1st_beam()
// DESCRIPTION: 应用最小径向速度查找初始参考径向，确定初始参考径向号和该径向上的平均径向速度
///////////////////////////////////////////////////////////
void QualityControl_VelocityDealiasing::
search_1st_beam( StandardRadarData *pGData,gInt32 El_Tilte, gInt32 &RadialNum,gInt16 &AverageRadialVelocity)
{
   gInt32    Gate_Num_Ref = 40;
   //Flag, 布尔型，环向处理数据标识符（1为顺时针，0为逆时针）；
   gInt32    Flag = 0;
   gInt32    Temp_Radial_Velocity = 0,Gate_Num_Tol = 0,Gate_Num = 0,Max_Gate_Num = 0;
   gFloat  temp = 0.0f;
   gInt32 Azi_temp = pGData->GetElevationHeadP(El_Tilte)->AzimuthNumber;
   gInt32 Gate_Temp = pGData->GetElevationHeadP(El_Tilte)->DopplerGates;
   gInt32    *Temp_Aver_RV = new gInt32[Azi_temp];
   gInt32    *Temp_Num_RV = new gInt32 [Azi_temp];
   gInt16  Threshold1 = 0,Threshold2 = 0;
   gInt16  **ppTemp = NULL;

   //////////////////////////////////////////////////////////////////////////空间换时间
   ppTemp = new gInt16* [Azi_temp];
   /*for( int j = 0; j < Azi_temp; j++)   
   {
	   ppTemp[j]  = new gInt16 [Gate_Temp];
   }*/
   for (int i =0; i < Azi_temp; i++)
   {
	   ppTemp[i] = pGData->GetRadarDataV(El_Tilte,i);
   }

   //initialize variables
   temp = (gFloat)0.3*(gFloat)m_velny;
   Threshold1 = (gInt16)temp;
   temp = (gFloat)VELNY_SCALE3*(gFloat)m_velny; 
   Threshold2 = (gInt16)temp;
   //get AverageRadialVelocity array for each beam
   RadialNum = -1; 
   AverageRadialVelocity = 0;

   for( int j=0; j < Azi_temp; j++)
   {
      Gate_Num=0; 
	  Temp_Radial_Velocity=0; 
	  Flag = 0;
	  gInt32 Gate_Temp1 = Gate_Temp - 1; 
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
   gInt32 j11,j22,j33;
   Max_Gate_Num = 0;    
   for( int j=0; j < Azi_temp; j++)
   {
       j11 = j +1 ;
	   j22 = j +2 ;
	   j33 = j +3 ;
       if( j11 >= Azi_temp) 
	   {
		   j11 -=  Azi_temp;
	   }
       if( j22 >= Azi_temp)
	   {
		   j22 -=  Azi_temp;
	   }
       if( j33 >= Azi_temp)
	   {
		   j33 -=  Azi_temp;
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
                  AverageRadialVelocity = (gInt16) Temp_Aver_RV[j11];
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
          for( int j=0; j < Azi_temp; j++)
          {
             Gate_Num=0;  
			 Temp_Radial_Velocity=0;  
			 Flag = 0;
			 gInt32  Gate_Temp2 = Gate_Temp -2;
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
                   AverageRadialVelocity = (gInt16)Temp_Radial_Velocity;
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

      gInt32 iodd=0;
      gInt32 ieven=0;
      for(int k=4; k < Gate_Temp; k++)
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
void QualityControl_VelocityDealiasing::
unfold(StandardRadarData *pGData,gInt32 El_Tilte)

{
   gInt16 *ReferenceAverageRadialVelocity = new gInt16[920];
   gInt32 j0 = 0,j1 = 0;
   gInt16** ProcessMark = NULL;
   gInt32 Azi_Temp = pGData->GetElevationHeadP(El_Tilte)->AzimuthNumber;
   gInt32 Gate_Temp = pGData->GetElevationHeadP(El_Tilte)->DopplerGates;
   chkvel(pGData,El_Tilte);
   for( int j = 0; j < Azi_Temp; j++)
   {
	   for( int k = 0; k < 4; k++)
	   {
		   pGData->GetRadarDataV(El_Tilte,j)[k] =m_missing;
	   }
   }

   ProcessMark = new gInt16* [Azi_Temp];
   for( int j = 0; j < Azi_Temp; j++)   
   {
       ProcessMark[j]  = new gInt16 [Gate_Temp];
       for( int k = 0; k < Gate_Temp; k++)
       {
          if( pGData->GetRadarDataV(El_Tilte,j)[k] > m_range_fold+1 )
		  {
             ProcessMark[j][k] = 0;
		  }
          else
		  {
			  ProcessMark[j][k] = 3;
		  }
       }
   }

   gInt32  RadialNum = 0;//初始径向号
   gInt16  AverageRadialVelocity = 0;//平均径向速度的平均值
   search_1st_beam(pGData,El_Tilte,RadialNum,AverageRadialVelocity);
   if( RadialNum < 0 )
   {
	   return ;
   }
  for( int k=0; k < Gate_Temp; k++)
  {
     ReferenceAverageRadialVelocity[k] = AverageRadialVelocity; 	 
  }
  shearb1s_initial(ProcessMark ,RadialNum,ReferenceAverageRadialVelocity,pGData,El_Tilte);

  j0 = RadialNum +1 ;
  j1 = RadialNum -1 ;

  if(j0 >= Azi_Temp)
  {
	  j0 -= Azi_Temp;
  }
  if(j1 < 0 )
  {
	  j1 +=  Azi_Temp;
  }

  shearb1s_initial(ProcessMark ,j0,ReferenceAverageRadialVelocity,pGData,El_Tilte);
  shearb1s_initial(ProcessMark ,j1,ReferenceAverageRadialVelocity,pGData,El_Tilte);
  //change reference beam as RadialNum
  chgbeam(RadialNum,ReferenceAverageRadialVelocity,pGData,El_Tilte);
  //unfold all left unfolded beams
  gInt32 Azimuth_Temp=RadialNum-Azi_Temp/2;

  if(Azimuth_Temp < 0)
  {
	  Azimuth_Temp +=Azi_Temp;
  }
  unfold1(ProcessMark ,RadialNum,pGData,El_Tilte);
  unfold2(2,ProcessMark ,RadialNum,pGData,El_Tilte);
  unfold2(3,ProcessMark ,RadialNum,pGData,El_Tilte);
  unfold2(4,ProcessMark ,RadialNum,pGData,El_Tilte);
  unfold2(4,ProcessMark ,Azimuth_Temp,pGData,El_Tilte);
  chkvel(pGData,El_Tilte);
  for (int i = 0; i < Azi_Temp; i++)
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
unfold1(gInt16** ProcessMark ,gInt32 RadialNum,StandardRadarData *pGData,gInt32 El_Tilte)
{
	gInt32 Azi_Num_Temp = pGData->GetElevationHeadP(El_Tilte)->AzimuthNumber;
  gInt32 Azimuth_Temp = RadialNum - Azi_Num_Temp/2;
  gInt16 *ptemp = pGData->GetRadarDataV(El_Tilte,RadialNum);

  if(Azimuth_Temp < 0)
  {
	  Azimuth_Temp +=  Azi_Num_Temp;
  }
  if(Azimuth_Temp > RadialNum)
  {
     for(int j=RadialNum; j<=Azimuth_Temp;j++)
     {
         shearb1s(1,ProcessMark ,j,ptemp,pGData,El_Tilte);
         chgbeam(j,ptemp,pGData,El_Tilte);
     }
  }
  else
  {
       for( int j=RadialNum; j < Azi_Num_Temp; j++)
       {
           shearb1s(1,ProcessMark ,j,ptemp,pGData,El_Tilte);
           chgbeam(j,ptemp,pGData,El_Tilte);
       }
       for( int j=0; j <= Azimuth_Temp; j++)
       {
            shearb1s(1,ProcessMark ,j,ptemp,pGData,El_Tilte);
            chgbeam(j,ptemp,pGData,El_Tilte);
       }
  }
  Azimuth_Temp = RadialNum - Azi_Num_Temp/2;

  if(Azimuth_Temp < 0) 
  {
	  Azimuth_Temp +=  Azi_Num_Temp;
  }

  if(Azimuth_Temp > RadialNum)
  {
     for( int j=RadialNum; j >= 0; j--)
     {
        shearb1s(0,ProcessMark ,j,ptemp,pGData,El_Tilte);
        chgbeam(j,ptemp,pGData,El_Tilte);
     }
     gInt32 temp = Azi_Num_Temp-1;
     for( int j = temp; j >= Azimuth_Temp; j--)
     {
        shearb1s(0,ProcessMark ,j,ptemp,pGData,El_Tilte);
        chgbeam( j,ptemp,pGData,El_Tilte);
     }
  }
  else
  {
        for( int j=RadialNum; j >= Azimuth_Temp; j--)
        {
            shearb1s(0,ProcessMark ,j,ptemp,pGData,El_Tilte);
            chgbeam( j,ptemp,pGData,El_Tilte);
        }
  }
  return;
}

///////////////////////////////////////////////////////
// FUNCTION: unfold2()
// DESCRIPTION: 对初始参考径向以外的径向上的速度值，从顺时针和逆时针两个方向上逐步扩展邻近搜索范围，进一步速度退模糊
///////////////////////////////////////////////////////
void QualityControl_VelocityDealiasing::
unfold2(gInt32 Round_Num, gInt16** ProcessMark ,gInt32 RadialNum,StandardRadarData *pGData,gInt32 El_Tilte)
{
	gInt32 Azimuth_Num_Temp = pGData->GetElevationHeadP(El_Tilte)->AzimuthNumber;
  gInt32 Azimuth_Temp =RadialNum - Azimuth_Num_Temp/2;
  gInt16 *ptemp = pGData->GetRadarDataV(El_Tilte,RadialNum);

  if(Azimuth_Temp < 0)
  {
	  Azimuth_Temp +=  Azimuth_Num_Temp;
  }

  if(Azimuth_Temp > RadialNum)
  {
     for(int j=RadialNum; j<=Azimuth_Temp;j++)
     {
         shearb1s1(Round_Num,1,ProcessMark ,j,ptemp,pGData,El_Tilte);
         chgbeam( j,ptemp,pGData,El_Tilte);
     }
  }
  else
  {
       for( int j=RadialNum; j < Azimuth_Num_Temp; j++)
       {
           shearb1s1(Round_Num,1,ProcessMark ,j,ptemp,pGData,El_Tilte);
           chgbeam(j,ptemp,pGData,El_Tilte);
       }
       for( int j=0; j <= Azimuth_Temp; j++)
       {
            shearb1s1(Round_Num,1,ProcessMark ,j,ptemp,pGData,El_Tilte);
            chgbeam(j,ptemp,pGData,El_Tilte);
       }
  }
  Azimuth_Temp = RadialNum - Azimuth_Num_Temp/2;

  if(Azimuth_Temp < 0)
  {
	  Azimuth_Temp += Azimuth_Num_Temp;
  }

  if(Azimuth_Temp > RadialNum)
  {
     for( int j=RadialNum; j >= 0; j--)
     {
        shearb1s1(Round_Num,0,ProcessMark ,j,ptemp,pGData,El_Tilte);
        chgbeam(j,ptemp,pGData,El_Tilte);
     }

     for( int j=Azimuth_Num_Temp-1; j >= Azimuth_Temp; j--)
     {
        shearb1s1(Round_Num,0,ProcessMark ,j,ptemp,pGData,El_Tilte);
        chgbeam( j,ptemp,pGData,El_Tilte);
     }
  }
  else
  {
        for( int j=RadialNum; j >= Azimuth_Temp; j--)
        {
            shearb1s1(Round_Num,0,ProcessMark ,j,ptemp,pGData,El_Tilte);
            chgbeam( j,ptemp,pGData,El_Tilte);
        }
  }

  return;
}

////////////////////////////////////////////////////////////////////
// FUNCTION: chkvel
// DESCRIPTION: 根据某一点的速度与周围49个点的速度的平均值比对结果确定参考值
////////////////////////////////////////////////////////////////////
void QualityControl_VelocityDealiasing::
chkvel(StandardRadarData *pGData,gInt32 El_Tilte)
{ 
	gInt32 AverageRadialVelocity = 0;//所有点的和
	gInt32 Azimuth_Sta = 0,Azimuth_End = 0;
	gInt32 idigit = 0,itotal = 0;//idigit:有效数据点数itotal:所有点数 
	gInt16 **Temp;
	gInt32 Azi_Num_Temp = pGData->GetElevationHeadP(El_Tilte)->AzimuthNumber;
	gInt32 Gate_Num_Temp = pGData->GetElevationHeadP(El_Tilte)->DopplerGates;
	//为提高运行效率，减少函数调用使用二维数组
	Temp = new gInt16* [Azi_Num_Temp];
	for (int i =0; i < Azi_Num_Temp; i++)
	{
			Temp[i] = pGData->GetRadarDataV(El_Tilte,i);
	}
	for( int j = 0; j < Azi_Num_Temp; j++)
	{
		Azimuth_Sta=j-3;     
		Azimuth_End=j+3;
		for( int k = 4; k < Gate_Num_Temp; k++)
		{
			AverageRadialVelocity=0; 
			idigit=0;   
			itotal=0;
			gInt32 j1 = Azimuth_Sta;
			if(j1 < 0) 
			{
				j1 = 0;
			}
			for(; j1 <= min(Azimuth_End,Azi_Num_Temp-1);j1++)
			{
				for( int k1=max(k-3,4); k1 <= min(k+3,Gate_Num_Temp-1);k1++)
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
				for(int j1=j-3+Azi_Num_Temp; j1< Azi_Num_Temp; j1++)
				{
					for( int k1=max(k-3,4); k1 <= min(k+3,Gate_Num_Temp-1);k1++)
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
			if(Azimuth_End >= Azi_Num_Temp)
			{
				for( int j1=0; j1 <= Azimuth_End-Azi_Num_Temp; j1++)
				{
					for( int k1=max(k-3,4); k1 <= min(k+3,Gate_Num_Temp-1); k1++)
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
					rfold(Temp[j][k],(gInt16)AverageRadialVelocity,1.0);
					continue;
				}

			}
			if(itotal > 30 && Temp[j][k] <= m_range_fold+1 && idigit >= itotal-5 )
			{
				AverageRadialVelocity = AverageRadialVelocity/idigit;
				Temp[j][k] = (gInt16)AverageRadialVelocity;
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
void QualityControl_VelocityDealiasing::
chgbeam( gInt32 AzimuthNumber, gInt16* ReferenceAverageRadialVelocity,StandardRadarData *pGData,gInt32 El_Tilte)
{
	gInt32 temp = pGData->GetElevationHeadP(El_Tilte)->DopplerGates;
	gInt16 * ptemp = pGData->GetRadarDataV(El_Tilte,AzimuthNumber);
   for( int k=4; k < temp; k++)
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
shearb1s_initial(gInt16** ProcessMark ,gInt32 AzimuthNumber,gInt16* ReferenceAverageRadialVelocity,StandardRadarData *pGData,gInt32 El_Tilte)
{
	gInt32 temp = pGData->GetElevationHeadP(El_Tilte)->DopplerGates;
	gInt16 *ptemp = pGData->GetRadarDataV(El_Tilte,AzimuthNumber);
   for( int k = 4; k < temp; k ++)
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
void QualityControl_VelocityDealiasing::
shearb1s1(gInt32 Round_Num, bool Flag,gInt16** ProcessMark ,gInt32 AzimuthNumber,
		 gInt16* ReferenceAverageRadialVelocity,StandardRadarData *pGData,gInt32 El_Tilte )
{
   gInt32 Azimuth_Temp = 0,Flag_Temp = 0,Num_Reg = 0;
   gInt32 j1 = 0; gInt32 j2 = 0; gInt32 j3 = 0;
   gInt32 Max_Gate_Dist = 0,Max_Beam_Dist = 0;
   gDouble Velny_Scale = 0.0;
   gInt16 temp = 0,Threshold1 = 0,Threshold2 = 0;
   gInt32 Azi_temp = pGData->GetElevationHeadP(El_Tilte)->AzimuthNumber;
   gInt32 Gate_temp = pGData->GetElevationHeadP(El_Tilte)->DopplerGates;
   gInt16 *ptemp = pGData->GetRadarDataV(El_Tilte,AzimuthNumber);

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
   Threshold1 = (gInt16)(VELNY_SCALE1*(gFloat)m_velny);   
   Threshold2 = (gInt16)(Velny_Scale*(gFloat)m_velny);   

   //Flag = 1, for clockwise dorection,
   //Flag = 0, for counter clockwise direction
   //unfold using previous equvalent beam values
   for( int k=4; k< Gate_temp; k++)
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
						   j1 +=  Azi_temp;
					   }
					   if( j2 < 0 )  
					   {
						   j2 +=  Azi_temp;
					   }
					   if( j3 < 0 )  
					   {
						   j3 +=  Azi_temp;
					   }
					   gInt16 *P_Temp1 = pGData->GetRadarDataV(El_Tilte,j1);
					   gInt16 *P_Temp2 = pGData->GetRadarDataV(El_Tilte,j2);
					   gInt16 *P_Temp3 = pGData->GetRadarDataV(El_Tilte,j3);
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
						   j1 +=  Azi_temp;
					   }
					   if( j2 < 0 )  
					   {
						   j2 +=  Azi_temp;
					   }
					   if( j3 < 0 )
					   {
						   j3 +=  Azi_temp;
					   }

					   gInt16 *P_Temp1 = pGData->GetRadarDataV(El_Tilte,j1);
					   gInt16 *P_Temp2 = pGData->GetRadarDataV(El_Tilte,j2);
					   gInt16 *P_Temp3 = pGData->GetRadarDataV(El_Tilte,j3);
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
					   for(int jj=Azi_temp-1; jj>Azimuth_Temp;jj--)
					   {
						   j1 = jj-1; 
						   j2 = jj-2; 
						   j3 = jj-3;
						   if( j1 < 0 )
						   {
							   j1 +=  Azi_temp;
						   }

						   if( j2 < 0 )  
						   {
							   j2 +=  Azi_temp;
						   }

						   if( j3 < 0 )  
						   {
							   j3 +=  Azi_temp;
						   }
						   gInt16 *P_Temp1 = pGData->GetRadarDataV(El_Tilte,j1);
						   gInt16 *P_Temp2 = pGData->GetRadarDataV(El_Tilte,j2);
						   gInt16 *P_Temp3 = pGData->GetRadarDataV(El_Tilte,j3);
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
					   if( j1 >=  Azi_temp ) 
					   {
						   j1 -=  Azi_temp;
					   }
					   if( j2 >=  Azi_temp )
					   {
						   j2 -=  Azi_temp;
					   }
					   if( j3 >=  Azi_temp )
					   {
						   j3 -=  Azi_temp;
					   }
					   gInt16 *P_Temp1 = pGData->GetRadarDataV(El_Tilte,j1);
					   gInt16 *P_Temp2 = pGData->GetRadarDataV(El_Tilte,j2);
					   gInt16 *P_Temp3 = pGData->GetRadarDataV(El_Tilte,j3);
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
				   for(int jj=AzimuthNumber; jj < Azi_temp;jj++)
				   {
					   j1 = jj+1; 
					   j2 = jj+2; 
					   j3 = jj+3;
					   if( j1 >=  Azi_temp ) 
					   {
						   j1 -=  Azi_temp;
					   }
					   if( j2 >=  Azi_temp ) 
					   {
						   j2 -=  Azi_temp;
					   }
					   if( j3 >=  Azi_temp )
					   {
						   j3 -=  Azi_temp;
					   }
					   gInt16 *P_Temp1 = pGData->GetRadarDataV(El_Tilte,j1);
					   gInt16 *P_Temp2 = pGData->GetRadarDataV(El_Tilte,j2);
					   gInt16 *P_Temp3 = pGData->GetRadarDataV(El_Tilte,j3);
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
						   if( j1 >=  Azi_temp )  
						   {
							   j1 -=  Azi_temp;
						   }
						   if( j2 >=  Azi_temp ) 
						   {
							   j2 -=  Azi_temp;
						   }
						   if( j3 >=  Azi_temp )
						   {
							   j3 -=  Azi_temp;
						   }
						   gInt16 *P_Temp1 = pGData->GetRadarDataV(El_Tilte,j1);
						   gInt16 *P_Temp2 = pGData->GetRadarDataV(El_Tilte,j2);
						   gInt16 *P_Temp3 = pGData->GetRadarDataV(El_Tilte,j3);
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
   gInt32 Gate_First = -1; 
   Num_Reg = 0; 
   gInt32 Gate_temp_6 = Gate_temp - 6;
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
				   j1 = AzimuthNumber-(gInt32)( (gFloat)(600/k) - 0.5);
				   if( j1 < 0 )
				   {
					   j1 +=  Azi_temp;
				   }
			   }
			   else if( Flag == 0 )
			   {
				   j1 = AzimuthNumber+(gInt32)( (gFloat)(600/k) + 0.5 );

				   if( j1 >=  Azi_temp ) 
				   {
					   j1 -= Azi_temp;
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
		  gInt32 Gate_temp_5 = Gate_temp - 5;
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
   gInt32 Gate_First2 = Gate_First + 2;
   gInt32 Gate_temp_4 = Gate_temp - 4;
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
		  gInt32 Dop_Temp = Gate_temp-5;
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
shearb1s(bool Flag,gInt16** ProcessMark ,gInt32 AzimuthNumber,gInt16* ReferenceAverageRadialVelocity,StandardRadarData *pGData,gInt32 El_Tilte)
{
   gInt32 Gate_First,j1,j2,j3,Gate_Temp;
   gInt16 *ReferenceAverageRadialVelocitytemp = new gInt16[920];
   gInt16 temp,Threshold;
   gInt32 Azi_Num_Temp = pGData->GetElevationHeadP(El_Tilte)->AzimuthNumber;
   gInt32 Gate_Num_Temp = pGData->GetElevationHeadP(El_Tilte)->DopplerGates;
   gInt16 *ptemp = pGData->GetRadarDataV(El_Tilte,AzimuthNumber);

   //initialize Threshold
   Threshold = (gInt16)(VELNY_SCALE1*(gFloat)m_velny);

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
		  j1 +=  Azi_Num_Temp;
	  }
      if( j2 < 0 ) 
	  {
		  j2 +=  Azi_Num_Temp;
	  }
      if( j3 < 0 ) 
	  {
		  j3 +=  Azi_Num_Temp;
	  }
   }
   if( Flag == 0 )
   {
      j1 = AzimuthNumber+1;
	  j2 = AzimuthNumber+2;
	  j3 = AzimuthNumber+3;  
      if( j1 >=  Azi_Num_Temp ) 
	  {
		  j1 -=  Azi_Num_Temp;
	  }
      if( j2 >=  Azi_Num_Temp )  
	  {
		  j2 -=  Azi_Num_Temp;
	  }

      if( j3 >=  Azi_Num_Temp ) 
	  {
		  j3 -=  Azi_Num_Temp;
	  }
   }
   gInt16 *P_Temp1 = pGData->GetRadarDataV(El_Tilte,j1);
   gInt16 *P_Temp2 = pGData->GetRadarDataV(El_Tilte,j2);
   gInt16 *P_Temp3 = pGData->GetRadarDataV(El_Tilte,j3);

   //unfold using previous equvalent beam values
   for( Gate_Temp=4; Gate_Temp < Gate_Num_Temp; Gate_Temp++)
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
   for( Gate_Temp=4; Gate_Temp < Gate_Num_Temp; Gate_Temp++)
   {
	   ReferenceAverageRadialVelocitytemp[Gate_Temp] = ptemp[Gate_Temp];
   }
	   
  gInt32 Gate_Sta1 = 0,Num_Reg = 0;
  gInt32 Gate_Sta2 = Gate_Num_Temp - 6; 
   
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
					  j1 = AzimuthNumber-(gInt32)( (gFloat)(600/Gate_Temp) - 0.5);
					  if( j1 < 0 ) 
					  {
						  j1 +=  Azi_Num_Temp;
					  }
				  }
				  else if( Flag == 0 )
				  {
					  j1 = AzimuthNumber+(gInt32)( (gFloat)(600/Gate_Temp) + 0.5 ); 
					  if( j1 >=  Azi_Num_Temp )  
					  {
						  j1 -=  Azi_Num_Temp;
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
rfold(gInt16& Data_V,gInt16 ReferenceAverageRadialVelocity,gFloat Scale)
{
   gInt32 Num_Reg=0;
   gInt16 temp = 0;
   // while(1)
   for(;;)
   {
        temp=(gInt16)abs(Data_V-ReferenceAverageRadialVelocity);
		
        if(temp > (gInt16)(Scale*(gFloat)m_velny) )
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

extern "C" DLLEXP_GSYS IArith *Create_arith (void)//用于创建QualityControl_VelocityDealiasing接口
{
	return new QualityControl_VelocityDealiasing;
}
//////////////////////////////////////////////////////////