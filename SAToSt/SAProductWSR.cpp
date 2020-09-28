//////////////////////////////////////////////////////////////////////////
// SAProductPretreatment.cpp
// created by zy 20080401
// SA�״��Ʒ���ݸ�ʽ
//////////////////////////////////////////////////////////////////////////
#include "SAProductWSR.h"
#include <cstdio>
#include <cstdlib>
#include <string>
#include <cmath>

gInt16 IsArchive2;
gInt16 total_tilt_nums;
gInt16 max_nrays;
gInt16 ref_gates;
gInt16 dop_gates;

gInt32 total_radial_nums;
gInt32 m_OffSize;
gInt32 m_StartRadial;
TARCHIVE2 *m_pA2;

SAProductWSR::SAProductWSR(void)
{
	IsArchive2 = 0;
	total_radial_nums=total_tilt_nums = 0;
	ref_gates = dop_gates = 0;
	max_nrays = 0;
}

SAProductWSR::~SAProductWSR(void)
{
	if(total_radial_nums) this->Close();
}

TARCHIVE2 * SAProductWSR::GetTArchiveHead( gInt32 Index )
{
	if((Index * sizeof(TARCHIVE2)) >=(size_t)GetSize () )
		return  ( TARCHIVE2 * ) NULL;

	void * p = NULL;
	GetPointer ( sizeof(TARCHIVE2) * Index+m_OffSize, p);
	return ( TARCHIVE2 * ) p;
}

TARCHIVE2 * SAProductWSR::GetTArchiveHeadP ( gInt32 Index )
{
	return GetTArchiveHead ( Index );
}

//------------------------------��ARCHIVE2�еĻز�ǿ��ת����dataz[][][] ---------------
void SAProductWSR::GetBaseData(GELEVATIONDATAHEAD * pElevHead,gInt16 ***dataz,gInt16 ***datav,gInt16 ***dataw,TILTINFO * pTiltInfo,DATAINFO_TYPE * pDataInfo)
{
	int i,j,k;
	int tilt, LayerIndex;

	//Compensate(szAppPath, prad);
	gUint8 **rppi = new gUint8 *[max_nrays] ;
	for(i=0;i<max_nrays;i++)
		rppi[i] = new gUint8 [ref_gates] ;

	gUint8 **vppi = new gUint8 *[max_nrays] ;
	for(i=0;i<max_nrays;i++)
		vppi[i] = new gUint8 [dop_gates] ;

	gUint8 **wppi = new gUint8 *[max_nrays] ;
	for(i=0;i<max_nrays;i++)
		wppi[i] = new gUint8 [dop_gates] ;

	for(i=0;i<max_nrays;i++)
		memset(rppi[i],0, ref_gates);

	for(i=0;i<max_nrays;i++)
		memset(vppi[i], 0, dop_gates);

	for(i=0;i<max_nrays;i++)
		memset(wppi[i], 0, dop_gates);

	LayerIndex=0;
	for( tilt=0;tilt<total_tilt_nums; tilt++)
	{
		gInt16 iIndex = pTiltInfo[tilt].start;
		if(GetPPiRef(tilt,rppi,pTiltInfo,pDataInfo))
		{
			for(i=0;i<max_nrays;i++)
			{
				for (j=0;j<pDataInfo[iIndex].ReflectivityGates;j++)
				{
					dataz[tilt][i][j] = (gInt16)( rppi[i][j] == 0 ? RADAR_DATA_NOECHO : (rppi[i][j] == 1 ? RADAR_DATA_OUTOFRANGE : ((gInt32)rppi[i][j]- 66) * 50));///36
					//����ֵΪ-32768ʱ����ʾ�޻ز����ݣ���������ȷ�ֵ������ֵΪ-32767ʱ����ʾ����ģ�� ������DBZֵ����λΪ0.1DBZ
				}
			}
			LayerIndex++;
		}
		else
		{
			for(i=0;i<max_nrays;i++)
			{
				for (j=0;j<ref_gates;j++)
				{
					dataz[tilt][i][j] = RADAR_DATA_NOECHO;
				}
			}
		}		
	}

	for( tilt=0;tilt<total_tilt_nums; tilt++)
	{
		if(GetDoppler(tilt,vppi,wppi,pTiltInfo,pDataInfo) ) 
		{
			gInt16 VelReso=pDataInfo[pTiltInfo[tilt].start].VelocityResolution;
			V_Normalization(tilt,pElevHead,vppi, VelReso, datav);
			W_Normalization(tilt,pElevHead, wppi, dataw);
		}
		else
		{
			for(i=0;i<max_nrays;i++)
			{
				for (j=0;j<dop_gates;j++)
				{
					datav[tilt][i][j] = RADAR_DATA_NOECHO;
					dataw[tilt][i][j] = RADAR_DATA_NOECHO;
				}
			}
		}
	}

	for(i=0;i<max_nrays;i++) 
		delete [] rppi[i]; 
	delete [] rppi;

	for(i=0;i<max_nrays;i++) 
		delete [] vppi[i]; 
	delete [] vppi;

	for(i=0;i<max_nrays;i++) 
		delete [] wppi[i]; 
	delete [] wppi;

	rppi = vppi = wppi = NULL;
	
	return ;
}

gInt16 SAProductWSR::RadialStatusCheck(gInt32 iOffSize,gInt16 iSwapon,gInt32 iTotalRadialNums,gInt16 & ElevationNumber,gInt16 & iRays,
									   gInt16 & iRefGates,gInt16 & iDopGates,TILTINFO * pTiltInfo,DATAINFO_TYPE * pDataInfo)
{
	m_OffSize = iOffSize;
	m_pA2 = GetTArchiveHeadP(0);
	total_radial_nums = iTotalRadialNums;
	int i;
	gInt16 tmpMaxRays = 0;

	for( i=0;i<20;i++)
	{
		pTiltInfo[i].start = pTiltInfo[i].end = pTiltInfo[i].total = pTiltInfo[i].bad = 0 ;
	}

	for( i=0;i<total_radial_nums;i++)
	{
		pDataInfo[i].AzimuthAngle = m_pA2[i].AzimuthAngle;                   //36~37��λ�ǣ����뷽ʽ��[��ֵ/8.]*[180./4096.]=�ȣ�
		pDataInfo[i].AzimuthStatus = m_pA2[i].AzimuthStatus;
		pDataInfo[i].DopplerGates = m_pA2[i].DopplerGates;
		pDataInfo[i].ElevationAngle = m_pA2[i].ElevationAngle;                //42~43 (value>>3)*0.043945(degree)  ���� �����뷽ʽ��[��ֵ/8.]*[180./4096.]=�ȣ�
		pDataInfo[i].MessageGenerationDate = m_pA2[i].MessageGenerationDate;          //18~19���������ռ�ʱ��(����,��00:00��ʼ)?
		pDataInfo[i].MessageGenerationTime = m_pA2[i].MessageGenerationTime;          //20~23 //unsigned  long??
		pDataInfo[i].NyquistVelocity = m_pA2[i].NyquistVelocity;
		pDataInfo[i].RadialCollectionTime = m_pA2[i].RadialCollectionTime;           //28~31���������ռ�ʱ��(����,��00:00��ʼ)??
		pDataInfo[i].RadialCollectionDate = m_pA2[i].RadialCollectionDate;           //32~33
		pDataInfo[i].ReflectivityGates = m_pA2[i].ReflectivityGates;
		pDataInfo[i].SpectrumWidthPointer = m_pA2[i].SpectrumWidthPointer;          //68~69 �׿�����ָ�루ƫ���״�������Ϣͷ���ֽ�������ʾ��һ���׿����ݵ�λ��
		pDataInfo[i].VelocityPointer = m_pA2[i].VelocityPointer;               //66~67 �ٶ�����ָ�루ƫ���״�������Ϣͷ���ֽ�������ʾ��һ���ٶ����ݵ�λ��
		pDataInfo[i].VelocityResolution = m_pA2[i].VelocityResolution;            //70~71 2=0.5m/s,4=1.0m/s �������ٶȷֱ��� 2����ʾ0.5��/�� 4����ʾ1.0��/��
		DATAINFO_TYPE tmpData = pDataInfo[i];
		if(iSwapon) 
		{
			byteswap(pDataInfo[i].AzimuthAngle);
			byteswap(pDataInfo[i].AzimuthStatus);
			byteswap(pDataInfo[i].DopplerGates);
			byteswap(pDataInfo[i].ElevationAngle);
			byteswap(pDataInfo[i].MessageGenerationDate);
			byteswap(pDataInfo[i].MessageGenerationTime);
			byteswap(pDataInfo[i].NyquistVelocity);
			byteswap(pDataInfo[i].RadialCollectionDate);
			byteswap(pDataInfo[i].RadialCollectionTime);
			byteswap(pDataInfo[i].ReflectivityGates);
			byteswap(pDataInfo[i].SpectrumWidthPointer);
			byteswap(pDataInfo[i].VelocityPointer);
			byteswap(pDataInfo[i].VelocityResolution);
		}
		tmpData = pDataInfo[i];

		if( pDataInfo[i].AzimuthStatus == 3 )
		{
			tmpMaxRays = 1;
			IsArchive2 = 3;
			pTiltInfo[0].start = i;
			pTiltInfo[0].total = 1;
			m_StartRadial = i;
		}

		else if( IsArchive2 && pDataInfo[i].AzimuthStatus == 0 )
		{
			if (pDataInfo[i].ReflectivityGates>ref_gates&&pDataInfo[i].ReflectivityGates>ref_gates <= 460)
			{
				ref_gates = pDataInfo[i].ReflectivityGates;
			}
			if (pDataInfo[i].DopplerGates>dop_gates &&pDataInfo[i].DopplerGates <=  920)
			{
				dop_gates = pDataInfo[i].DopplerGates;
			}
			tmpMaxRays = 1;
			pTiltInfo[total_tilt_nums].start = i;
			pTiltInfo[total_tilt_nums].total = 1;
		}

		else if( IsArchive2 && pDataInfo[i].AzimuthStatus == 1 )
		{
			tmpMaxRays ++;
			pTiltInfo[total_tilt_nums].total ++;
			if(total_tilt_nums >=20 ) break;
		}

		else if( IsArchive2 && pDataInfo[i].AzimuthStatus == 2 )
		{
			tmpMaxRays ++;
			if (max_nrays<tmpMaxRays)
			{
				max_nrays = tmpMaxRays;
			}			
			pTiltInfo[total_tilt_nums].total ++;
			pTiltInfo[total_tilt_nums].end = i;
			total_tilt_nums++;
		}

		else if( IsArchive2 && pDataInfo[i].AzimuthStatus == 4 )
		{
			tmpMaxRays ++;
			if (max_nrays<tmpMaxRays)
			{
				max_nrays = tmpMaxRays;
			}	
			pTiltInfo[total_tilt_nums].total ++;
			pTiltInfo[total_tilt_nums].end = i;
			total_tilt_nums++;
			IsArchive2 = 4;
		}

		else if(IsArchive2)
		{
			pTiltInfo[total_tilt_nums].bad++;
		}
	}

	// check in ts.total of eack tilt;
	for(i=0;i<total_tilt_nums;i++)
	{
		if(pTiltInfo[i].total < 300 ) 
		{
			total_tilt_nums = i; 
			break;
		}
	}

	ElevationNumber=total_tilt_nums;
	iRays = max_nrays;
	iRefGates = ref_gates;
	iDopGates = dop_gates;

	if(total_tilt_nums<6) IsArchive2 = 0;

	return IsArchive2;
}

void SAProductWSR::Close()
{
	//if( total_radial_nums>0 )
	//{
	//	delete [] m_pA2;
	//	total_radial_nums = 0;
	//}
}

gInt32 SAProductWSR::GetPPiRef(int tilt,gUint8 **rppi,TILTINFO * pTiltInfo,DATAINFO_TYPE * pDataInfo)
{
	if( (pTiltInfo[tilt].end-pTiltInfo[tilt].start) < 359) return 0;

	gInt16 i,j,k,s,iRefGates;

	s= pTiltInfo[tilt].start;
	while (pDataInfo[s].AzimuthStatus >4 )
	{
		s++;
	}
	if( pDataInfo[s].ReflectivityGates<=0) return 0;

	iRefGates = pDataInfo[s].ReflectivityGates;
	//  ����һ����λ��ǿ�ȿ�����Ϊ�����ǲ��ǿ�ȿ���,������Ϊ��������0.01

	for(j=pTiltInfo[tilt].start;j<=pTiltInfo[tilt].end;j++)
	{
		for( i=0; i<iRefGates;i++)
		{
			if( m_pA2[j].dat[i]>1)	m_pA2[j].dat[i]-=2;
		}
	}

	//  bFlag[max_nrays]Ϊת����ǣ�0=��ֵ��n=��Ӧ����ĸ���
	gUint8 *bFlag = new gUint8[max_nrays];
	memset(bFlag,0x00,max_nrays);	

	for(j=pTiltInfo[tilt].start;j<=pTiltInfo[tilt].end;j++)
	{
		if( pDataInfo[j].AzimuthStatus > 4 )
		{
			continue;
		}
		k=(gUint16)(((gUint64)(pDataInfo[j].AzimuthAngle)*180.0/8)/4096+0.5)%360;

		if(!bFlag[k]) 
		{// ֱ�Ӹ�ֵ
			memcpy(rppi[k], m_pA2[j].dat, iRefGates) ;
		}

		else if (bFlag[k]==1)
		{ //ȡƽ��
			for(i=0; i<iRefGates; i++)
				rppi[k][i]=(rppi[k][i]+m_pA2[j].dat[i])/2;
		}

		bFlag[k]++;
	}

	for(  j=0;j<max_nrays;j++) 
	{
		if(bFlag[j] == 0 ) //��һ����λʱ��ֵ
		{ 
			int j1=j-1,j2=j+1;
			if( j1<0) j1+=max_nrays;
			if(j2>=max_nrays) j2-=max_nrays;
			if (bFlag[j1]&&bFlag[j2]) //ȡƽ��
			{
				for(i=0;i<iRefGates;i++)                     
				{
					rppi[j][i]= (rppi[j1][i]+rppi[j2][i])/2;
				}
			}
		}
	}

	delete [] bFlag;
	bFlag = NULL;
	return 1;
}

gInt32 SAProductWSR::GetDoppler(int tilt/*,GELEVATIONDATAHEAD * pElevHead*/,gUint8 **vppi,gUint8 **wppi,TILTINFO * pTiltInfo,DATAINFO_TYPE * pDataInfo)
{
	if( (pTiltInfo[tilt].end-pTiltInfo[tilt].start) < 359) return 0;

	gInt16 i,j,k,s,iDopGates;

	s= pTiltInfo[tilt].start;
	while (pDataInfo[s].AzimuthStatus >4 )
	{
		s++;
	}
	if(pDataInfo[s].DopplerGates<=0) return 0;
	iDopGates = pDataInfo[s].DopplerGates;

	gInt16 vOffset=pDataInfo[s].VelocityPointer-100;
	gInt16 wOffset=pDataInfo[s].SpectrumWidthPointer-100;

	gUint8 *bFlag = new gUint8 [max_nrays];
	memset(bFlag,0x00,max_nrays);	

	for(j=pTiltInfo[tilt].start;j<=pTiltInfo[tilt].end;j++)
	{
		if( pDataInfo[j].AzimuthStatus > 4 ) continue;
		k=(gUint16)(((gUint64)(pDataInfo[j].AzimuthAngle)*180.0/8)/4096+0.5)%360;

		if(bFlag[k]==0) // ֱ�Ӹ�ֵ
		{
			for(i=0;i<iDopGates;i++)
			{
				vppi[k][i]=m_pA2[j].dat[i+vOffset];
			}
			for(i=0;i<iDopGates;i++)
			{
				wppi[k][i]=m_pA2[j].dat[i+wOffset];
			}
		}
		else if (bFlag[k]==1) //ȡƽ��
		{
			for(i=0;i<iDopGates;i++)
			{
				if( vppi[k][i]>1 && m_pA2[j].dat[i+vOffset]>1)
				{
					vppi[k][i]=(vppi[k][i]+m_pA2[j].dat[i+vOffset])/2;
				}
				else if( vppi[k][i]==1 || m_pA2[j].dat[i+vOffset]==1)
				{
					vppi[k][i]=vppi[k][i]+m_pA2[j].dat[i+vOffset]-1;
				}
				else
				{
					vppi[k][i]=vppi[k][i]+m_pA2[j].dat[i+vOffset];
				}
			}

			for(i=0;i<iDopGates;i++)
			{
				if( wppi[k][i]>1 && m_pA2[j].dat[i+wOffset]>1)
				{
					wppi[k][i]=(wppi[k][i]+m_pA2[j].dat[i+wOffset])/2;
				}
				else if( wppi[k][i]==1 || m_pA2[j].dat[i+wOffset]==1 )
				{
					wppi[k][i]=wppi[k][i]+m_pA2[j].dat[i+wOffset]-1;
				}
				else 
				{
					wppi[k][i]=wppi[k][i]+m_pA2[j].dat[i+wOffset];
				}
			}
		}

		bFlag[k]++;
	}

	for(  j=0;j<max_nrays;j++) 
	{
		if(bFlag[j] == 0 ) //��һ����λʱ��ֵ
		{ 
			int j1=j-1,j2=j+1;
			if( j1<0) j1+=max_nrays;
			if(j2>=max_nrays) j2-=max_nrays;
			if (bFlag[j1]&&bFlag[j2]) //ȡƽ��
			{
				for(i=0;i<iDopGates;i++)
				{
					if( vppi[j1][i]>1 && vppi[j2][i]>1)
						vppi[j][i]=(vppi[j1][i]+vppi[j2][i])/2;
					else if( vppi[j1][i]==1 || vppi[j2][i]==1)
						vppi[j][i]=vppi[j1][i]+vppi[j2][i]-1;
					else 
						vppi[j][i]=vppi[j1][i]+vppi[j2][i];
				}


				for(i=0;i<iDopGates;i++)
				{
					if( wppi[j2][i]>1 && wppi[j2][i]>1)
						wppi[j][i]=(wppi[j1][i]+wppi[j2][i])/2;
					else if( wppi[j1][i]==1 || wppi[j2][i]==1)
						wppi[j][i]=wppi[j1][i]+wppi[j2][i]-1;
					else 
						wppi[j][i]=wppi[j1][i]+wppi[j2][i];
				}
			}
		}
	}

	delete [] bFlag;
	bFlag = NULL;
	return 1;
}

//----------- ��һ���ٶ����� --------------------------
void SAProductWSR::V_Normalization(int ilayer,GELEVATIONDATAHEAD * pElevHead,gUint8 **vppi,gInt16 VelReso,gInt16 ***datav)
{
	//�ٶ��������Ͳ����з�������-128��ʾ�޻ز����ٶ�ʹ����ʽ��ʾ��
	//-----------------------------------------------------------
	//              y
	// V = Vmax *  ----    ��ΪԶ���״��ٶȣ���Ϊ�����״��ٶȡ�
	//             127
	//-----------------------------------------------------------
	//	Ϊ��ԭ�״��ٶ�������ͨ�����±任�����ͳһ���ٶ����ݡ�
	//  ���ֱ���Ϊ0.5��/��ʱ���ã�1��ʽת����
	//-------------------------------------------------------------
	//         (x-129)*127
	//     y= -------------   ��1��(����xΪԭ���룬yΪת����Ĵ���)
	//           2Vmax
	//-------------------------------------------------------------
	//  ���ֱ���Ϊ1��/��ʱ���ã�2��ʽת����
	//--------------------------------------------------------------
	//        (x-129)*127                                           
	//     y=-------------    ��2��(����xΪԭ���룬yΪת����Ĵ���) 
	//           Vmax                                              
	//--------------------------------------------------------------
	//  0 ת��Ϊ-128,1 Ҳת��Ϊ-128�����ٶ�ģ�����޻ز�����
	///
	//�ٶȷֱ���VelocityResolution:2=0.5m/s,4=1.0m/s

	double normalization_coefficient = 3175.0*VelReso/pElevHead[ilayer].NyquistVelocity;

	for(int j=0;j<max_nrays;j++)
	{
		for(int i=0;i<pElevHead[ilayer].DopplerGates;i++)
		{
			if(vppi[j][i] > 2 )
			{
				datav[ilayer][j][i] =10*((gInt16)vppi[j][i]-129.0)*normalization_coefficient;
			}
			else
			{
				datav[ilayer][j][i] = RADAR_DATA_NOECHO;
			}
		}
	}

	return ;
}

void SAProductWSR::W_Normalization(int ilayer,GELEVATIONDATAHEAD * pElevHead,gUint8 **wppi,gInt16 ***dataw)
{
	//	�׿�����з������� 0ת��Ϊ-128��1ת��Ϊ-1��
	//	������ֵ���ã�ԭ�״���ٶ��׿�ֵ�ã�3��ʽת��Ϊͳһ��ʽ����ֵ��
	//      ������XΪԭ���룬YΪת����Ĵ��룩	 ��3��
	//-------------------------------------------------------------
	//         (x-129)*127
	//     y= -------------   ��3��(����xΪԭ���룬yΪת����Ĵ���)
	//           2Vmax
	//	
	double normalization_coefficient = 6350.0/pElevHead[ilayer].NyquistVelocity;
	for(int j=0;j<max_nrays;j++)
	{
		for(int i=0;i<pElevHead[ilayer].DopplerGates;i++)
		{
			if(wppi[j][i] == 1 )
			{
				dataw[ilayer][j][i] = -1*100;
			}
			else if(wppi[j][i] >128 )
			{
				dataw[ilayer][j][i] = 10*((gInt16)wppi[j][i]-129.0)*normalization_coefficient;
			}
			else
			{
				dataw[ilayer][j][i] = RADAR_DATA_NOECHO;
			}
		}
	}

	return ;
}