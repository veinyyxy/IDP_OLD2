#include <QtCore/QCoreApplication>
#include <QtGui/QProgressDialog>
#include "RadarMarchingCubesImp.h"

#include "RadReadStationFile.h"

void RadarDataParser::dumpAllCube(P_COLORTABLE_VECTOR pColorTable,  P_CUBE_DATA_VECTOR pCDV )
{

	RadProjTransform *proj = NULL;
	proj = new RadProjTransform;
	QString destproj = "RADARPROJ" ;
	destproj.append(" ");
	destproj.append(QString::number(m_RadarCentral.y()));
	destproj.append(" ");
	destproj.append( QString::number(m_RadarCentral.x()));
	destproj.append(" ");
	destproj.append(QString::number(m_RadarCentral.z()));

	proj->setSourceCRS(destproj.toStdString());
	proj->setSourceProjName("雷达中心兰勃特投影");

	proj->setDestCRS("+proj=longlat +ellps=WGS84 +datum=WGS84 +no_defs");
	proj->setDesProjName("无投影");

	proj->Elevation(true);


	//QHash<QString,QVector4D> colorHash;
	double maxValue =0;
	PDBlock* blok = GetPDBlock();
	if(blok==NULL)
	{
		return;
	}
	int productCode = blok->ProductCode;
	proAttr pAttributeHaHa;
	cDrawProduct*          pDrawProduct=(cDrawProduct *)this;
	pDrawProduct->ReadInfo(&pAttributeHaHa,0);

	float hhight = QString::number((float)pAttributeHaHa.altitude*0.1,'f',2).toFloat();
	double ra = GetRadialRadius(0)/1000;

	osg::Vec3 ps[4];
	double s1,c1,s2,c2;
	osg::Vec3 p1,p2,p3,p4;
	double dRangJiao = 0.0;
	int iMaxValue = blok->NumberOfLayers;

	if (productCode == 258)
	{
		for (int i = 0 ; i < blok->NumberOfLayers-1 ; i++)
		{
			DILayer* layer = GetLayer(i);
			if(layer == NULL)
				return;
			for (int j = 0;j < layer->NumberOfBlocks ; j++)
			{
				//std::vector<gInt16> vec;
				DLIBlock* dblock = GetBlock(i,j);
				if(dblock == NULL)
				{
					return;
				}
				if(dblock->TypeOfDataPackets == BLOCKTYPE_RADIAL)
				{
					RadialHead* radialhead = GetBlockDataAsRadial_Head(i,j); 

					double rate =  1000.0;
					int wight = radialhead->NumberOfGates*radialhead->LengthOfGate /rate +radialhead->DistanceOfFirstGate/rate;
					for (int k = 0; k <radialhead->NumberOfRadials/*-1*/; k++)        //
					{
						RadialData* radialdata = GetBlockDataAsRadial_RadialData(i,j,k);

						int IC = radialhead->NumberOfGates; //中心点，取距离库数  =  bitmp.GetWidth()/2+offset_x
						int JC = radialhead->NumberOfGates;	///	取距离库数		=  bitmp.GetHeight()/2+offset_y

						p1=osg::Vec3(IC,JC,0);
						p2=osg::Vec3(IC,JC,0);
						p3=osg::Vec3(IC,JC,0);
						p4=osg::Vec3(IC,JC,0);

						ps[0].set(p1);
						ps[1].set(p2);
						ps[2].set(p3);
						ps[3].set(p4);

						double r1=radialhead->DistanceOfFirstGate/rate;
						double r2 = r1;

						s1=sin(radialdata->AzimuthAngle/100.0 * M_PI / 180.0); 
						c1=cos(radialdata->AzimuthAngle/100.0 * M_PI / 180.0);
						s2=sin((radialdata->AzimuthAngle/100.0 + radialdata->DeltaAngle/100.0 )* M_PI / 180.0);
						c2=cos((radialdata->AzimuthAngle/100.0 + radialdata->DeltaAngle/100.0 ) * M_PI / 180.0);
					
						gInt16* pArray = GetBlockDataAsRadial_Data(i,j,k);

						for (int z  = 0 ;z < radialhead->NumberOfGates-1 ;z++)
						{
							gInt16 val = pArray[z];						
							p1=p2;
							p4=p3;
							dRangJiao = layer->HeightOfElevation * 100.0;
							if (val == -32768 || val== -32767) 
							{
								r2 =(r1 + z*radialhead->LengthOfGate/rate)*1000;
								p2.set(r2*s1,r2*c1,0);
								p3.set(r2*s2,r2*c2,0);

								ps[0].set(p1);
								ps[1].set(p2);
								ps[2].set(p3);
								ps[3].set(p4);
								continue;
							}

							r2 =(r1 + z*radialhead->LengthOfGate/rate)*1000;
							p2.set(r2*s1,r2*c1,0);
							p3.set(r2*s2,r2*c2,0);
							ps[0].set(p1);
							ps[1].set(p2);
							ps[2].set(p3);
							ps[3].set(p4);
							if (val == -32766)
							{
								v3Point vp1,vp2,vp3,vp4;
								proj->setProductAngle(dRangJiao);

							//	osg::Vec3 src(ps[0].x()*1000,ps[0].y()*1000,0);
								osg::Vec3 res;
								if(!(proj->Transform(ps[0], res)))
								{
								//	res.set(res.x(),res.y(),(res.z())*1000.0+hhight);
									vp1.x = res.x();
									vp1.y = res.y();
									vp1.z = dRangJiao+hhight;
									vp1.value = pArray[z]/blok->ScaleOfData;
									pCDV->push_back(vp1);

								}

								//osg::Vec3 src1(ps[1].x()*1000,ps[1].y()*1000,0);
								osg::Vec3 res1;
								if(!(proj->Transform(ps[1], res1)))
								{
									//res1.set(res1.x(),res1.y(),(res1.z())*1000.0+hhight);
									vp2.x = res1.x();
									vp2.y = res1.y();
									vp2.z = dRangJiao+hhight;
									vp2.value = pArray[z+1]/blok->ScaleOfData;
									pCDV->push_back(vp2);

								}

								if (radialhead->NumberOfRadials - 1 == k)
								{
									pArray = GetBlockDataAsRadial_Data(i,j,0);
								}else
								{
									pArray = GetBlockDataAsRadial_Data(i,j,k+1);
								}
								

								//osg::Vec3 src2(ps[2].x()*1000,ps[2].y()*1000,0);
								osg::Vec3 res2;
								if(!(proj->Transform(ps[2], res2)))
								{
									//res2.set(res2.x(),res2.y(),(res2.z())*1000.0+hhight);
									vp3.x = res2.x();
									vp3.y = res2.y();
									vp3.z = dRangJiao+hhight;
									vp3.value = pArray[z + 1]/blok->ScaleOfData;
									pCDV->push_back(vp3);

								}

								//osg::Vec3 src3(ps[3].x()*1000,ps[3].y()*1000,0);
								osg::Vec3 res3;
								if(!(proj->Transform(ps[3], res3)))
								{
									//res3.set(res3.x(),res3.y(),(res3.z())*1000.0+hhight);
									vp4.x = res3.x();
									vp4.y = res3.y();
									vp4.z = dRangJiao+hhight;
									vp4.value = pArray[z]/blok->ScaleOfData;
									pCDV->push_back(vp4);

								}

								pArray = GetBlockDataAsRadial_Data(i,j,k);
								dumpPoint(i+1,j,k,z,proj,pCDV);
							}
							else
							{
								//double an = this->GetRadiaElevation(i)*0.01;
								proj->setProductAngle(dRangJiao);
								v3Point vp1,vp2,vp3,vp4;
								//osg::Vec3 src(ps[0].x()*1000,ps[0].y()*1000,0);
								osg::Vec3 res;
								if(!(proj->Transform(ps[0], res)))
								{
									//res.set(res.x(),res.y(),(res.z())*1000.0+hhight);
									vp1.x = res.x();
									vp1.y = res.y();
									vp1.z = dRangJiao+hhight;
									vp1.value = pArray[z]/blok->ScaleOfData;
									pCDV->push_back(vp1);

								}


								//osg::Vec3 src1(ps[1].x()*1000,ps[1].y()*1000,0);
								osg::Vec3 res1;
								if(!(proj->Transform(ps[1], res1)))
								{
									//res1.set(res1.x(),res1.y(),(res1.z())*1000.0+hhight);
									vp2.x = res1.x();
									vp2.y = res1.y();
									vp2.z =dRangJiao+hhight;
									vp2.value = pArray[z+1]/blok->ScaleOfData;
									pCDV->push_back(vp2);

								}


								if (radialhead->NumberOfRadials - 1 == k)
								{
									pArray = GetBlockDataAsRadial_Data(i,j,0);
								}else
								{
									pArray = GetBlockDataAsRadial_Data(i,j,k+1);
								}

								//osg::Vec3 src2(ps[2].x()*1000,ps[2].y()*1000,0);
								osg::Vec3 res2;
								if(!(proj->Transform(ps[2], res2)))
								{
									//res2.set(res2.x(),res2.y(),(res2.z())*1000.0+hhight);
									vp3.x = res2.x();
									vp3.y = res2.y();
									vp3.z = dRangJiao+hhight;
									vp3.value = pArray[z+1]/blok->ScaleOfData;
									pCDV->push_back(vp3);

								}

								//osg::Vec3 src3(ps[3].x()*1000,ps[3].y()*1000,0);
								osg::Vec3 res3;
								if(!(proj->Transform(ps[3], res3)))
								{
									//res3.set(res3.x(),res3.y(),(res3.z())*1000.0+hhight);
									vp4.x = res3.x();
									vp4.y = res3.y();
									vp4.z =dRangJiao+hhight;
									vp4.value = pArray[z]/blok->ScaleOfData;
									pCDV->push_back(vp4);

								}

								pArray = GetBlockDataAsRadial_Data(i,j,k);
								dumpPoint(i+1,j,k,z,proj,pCDV);
							}
						}					
					}
				}	
			}
		}
	} //end if 258
else
	{
		for (int i = 0 ; i < blok->NumberOfLayers-1 ; i++)
		{
			DILayer* layer = GetLayer(i);
			if(layer == NULL)
				return;
			for (int j = 0;j < layer->NumberOfBlocks ; j++)
			{
				DLIBlock* dblock = GetBlock(i,j);
				if(dblock == NULL)
				{
					return;
				}
				if(dblock->TypeOfDataPackets == BLOCKTYPE_RADIAL)
				{
					RadialHead* radialhead = GetBlockDataAsRadial_Head(i,j); 
					double rate =  1000.0;
					int wight = radialhead->NumberOfGates*radialhead->LengthOfGate /rate +radialhead->DistanceOfFirstGate/rate;
					for (int k = 0; k <radialhead->NumberOfRadials/*-1*/; k++)        //
					{
						RadialData* radialdata = GetBlockDataAsRadial_RadialData(i,j,k);

						int IC = radialhead->NumberOfGates; //中心点，取距离库数  =  bitmp.GetWidth()/2+offset_x
						int JC = radialhead->NumberOfGates;	///	取距离库数		=  bitmp.GetHeight()/2+offset_y

						p1=osg::Vec3(IC,JC,0);
						p2=osg::Vec3(IC,JC,0);
						p3=osg::Vec3(IC,JC,0);
						p4=osg::Vec3(IC,JC,0);

						ps[0].set(p1);
						ps[1].set(p2);
						ps[2].set(p3);
						ps[3].set(p4);

						double r1=radialhead->DistanceOfFirstGate/rate;
						double r2 = r1;

						s1=sin(radialdata->AzimuthAngle/100.0 * M_PI / 180.0); 
						c1=cos(radialdata->AzimuthAngle/100.0 * M_PI / 180.0);
						s2=sin((radialdata->AzimuthAngle/100.0 + radialdata->DeltaAngle/100.0 )* M_PI / 180.0);
						c2=cos((radialdata->AzimuthAngle/100.0 + radialdata->DeltaAngle/100.0 ) * M_PI / 180.0);
					
						gInt16* pArray = GetBlockDataAsRadial_Data(i,j,k);

						for (int z  = 0 ;z < radialhead->NumberOfGates-1 ;z++)
						{
							gInt16 val = pArray[z];						
							p1=p2;
							p4=p3;
							dRangJiao = layer->HeightOfElevation / 100.0;
							if (val == -32768 || val== -32767) 
							{
								r2 =(r1 + z*radialhead->LengthOfGate/rate)*1000;
								p2.set(r2*s1,r2*c1,0);
								p3.set(r2*s2,r2*c2,0);
								ps[0].set(p1);
								ps[1].set(p2);
								ps[2].set(p3);
								ps[3].set(p4);
								continue;
							}
							r2 =(r1 + z*radialhead->LengthOfGate/rate)*1000;
							p2.set(r2*s1,r2*c1,0);
							p3.set(r2*s2,r2*c2,0);
							ps[0].set(p1);
							ps[1].set(p2);
							ps[2].set(p3);
							ps[3].set(p4);
// 							if (val != -32768 && val!= -32767)
// 							{
							if (val == -32766)
							{
								v3Point vp1,vp2,vp3,vp4;
								proj->setProductAngle(dRangJiao);

								//osg::Vec3 src(ps[0].x()*1000,ps[0].y()*1000,0);
								osg::Vec3 res;
								if(!(proj->Transform(ps[0], res)))
								{
								//	res.set(res.x(),res.y(),(res.z())*1000.0+hhight);
									vp1.x = res.x();
									vp1.y = res.y();
									vp1.z = (res.z())*1000.0+hhight;
									vp1.value = pArray[z]/blok->ScaleOfData;
									pCDV->push_back(vp1);

								}


								//osg::Vec3 src1(ps[1].x()*1000,ps[1].y()*1000,0);
								osg::Vec3 res1;
								if(!(proj->Transform(ps[1], res1)))
								{
									//res1.set(res1.x(),res1.y(),(res1.z())*1000.0+hhight);
									vp2.x = res1.x();
									vp2.y = res1.y();
									vp2.z = (res1.z())*1000.0+hhight;
									vp2.value = pArray[z+1]/blok->ScaleOfData;
									pCDV->push_back(vp2);

								}


								if (radialhead->NumberOfRadials - 1 == k)
								{
									pArray = GetBlockDataAsRadial_Data(i,j,0);
								}else
								{
									pArray = GetBlockDataAsRadial_Data(i,j,k+1);
								}

								//osg::Vec3 src2(ps[2].x()*1000,ps[2].y()*1000,0);
								osg::Vec3 res2;
								if(!(proj->Transform(ps[2], res2)))
								{
									//res2.set(res2.x(),res2.y(),(res2.z())*1000.0+hhight);
									vp3.x = res2.x();
									vp3.y = res2.y();
									vp3.z = (res2.z())*1000.0+hhight;
									vp3.value = pArray[z + 1]/blok->ScaleOfData;
									pCDV->push_back(vp3);

								}

								//osg::Vec3 src3(ps[3].x()*1000,ps[3].y()*1000,0);
								osg::Vec3 res3;
								if(!(proj->Transform(ps[3], res3)))
								{
									//res3.set(res3.x(),res3.y(),(res3.z())*1000.0+hhight);
									vp4.x = res3.x();
									vp4.y = res3.y();
									vp4.z = (res3.z())*1000.0+hhight;
									vp4.value = pArray[z]/blok->ScaleOfData;
									pCDV->push_back(vp4);

								}

								pArray = GetBlockDataAsRadial_Data(i,j,k);
								dumpPoint(i+1,j,k,z,proj,pCDV);
							}
							else
							{
								//double an = this->GetRadiaElevation(i)*0.01;
								proj->setProductAngle(dRangJiao);
								v3Point vp1,vp2,vp3,vp4;
								//osg::Vec3 src(ps[0].x()*1000,ps[0].y()*1000,0);
								osg::Vec3 res;
								if(!(proj->Transform(ps[0], res)))
								{
									//res.set(res.x(),res.y(),(res.z())*1000.0+hhight);
									vp1.x = res.x();
									vp1.y = res.y();
									vp1.z = (res.z())*1000.0+hhight;
									vp1.value = pArray[z]/blok->ScaleOfData;
									pCDV->push_back(vp1);

								}


								//osg::Vec3 src1(ps[1].x()*1000,ps[1].y()*1000,0);
								osg::Vec3 res1;
								if(!(proj->Transform(ps[1], res1)))
								{
									//res1.set(res1.x(),res1.y(),(res1.z())*1000.0+hhight);
									vp2.x = res1.x();
									vp2.y = res1.y();
									vp2.z =(res1.z())*1000.0+hhight;
									vp2.value = pArray[z+1]/blok->ScaleOfData;
									pCDV->push_back(vp2);

								}


								if (radialhead->NumberOfRadials - 1 == k)
								{
									pArray = GetBlockDataAsRadial_Data(i,j,0);
								}else
								{
									pArray = GetBlockDataAsRadial_Data(i,j,k+1);
								}

								//osg::Vec3 src2(ps[2].x()*1000,ps[2].y()*1000,0);
								osg::Vec3 res2;
								if(!(proj->Transform(ps[2], res2)))
								{
									//res2.set(res2.x(),res2.y(),(res2.z())*1000.0+hhight);
									vp3.x = res2.x();
									vp3.y = res2.y();
									vp3.z = (res2.z())*1000.0+hhight;
									vp3.value = pArray[z+1]/blok->ScaleOfData;
									pCDV->push_back(vp3);

								}

								//osg::Vec3 src3(ps[3].x()*1000,ps[3].y()*1000,0);
								osg::Vec3 res3;
								if(!(proj->Transform(ps[3], res3)))
								{
									//res3.set(res3.x(),res3.y(),(res3.z())*1000.0+hhight);
									vp4.x = res3.x();
									vp4.y = res3.y();
									vp4.z =(res3.z())*1000.0+hhight;
									vp4.value = pArray[z]/blok->ScaleOfData;
									pCDV->push_back(vp4);

								}

								pArray = GetBlockDataAsRadial_Data(i,j,k);
								dumpPoint(i+1,j,k,z,proj,pCDV);
							}

						}
						
					}
				}	
			}
		}
			
	}
	
	return;
}

void RadarDataParser::dumpPoint( int iLayer,int jBlock,int kRad,int zGate,RadProjTransform *proj , P_CUBE_DATA_VECTOR pCDV )
{
	//QHash<QString,QVector4D> colorHash;
	double maxValue =0;
	PDBlock* blok = GetPDBlock();
	if(blok==NULL)
	{
		return;
	}
	int productCode = blok->ProductCode;
	proAttr pAttributeHaHa;
	cDrawProduct*          pDrawProduct=(cDrawProduct *)this;
	pDrawProduct->ReadInfo(&pAttributeHaHa,0);

	float hhight = QString::number((float)pAttributeHaHa.altitude*0.1,'f',2).toFloat();

	DILayer* layer = GetLayer(iLayer);
	if(layer == NULL)
		return;

	DLIBlock* dblock = GetBlock(iLayer,jBlock);
	if(dblock == NULL)
	{
		return;
	}
	double cen_x = m_RadarCentral.x();
	double cen_y = m_RadarCentral.y();
	RadialHead* radialhead = GetBlockDataAsRadial_Head(iLayer,jBlock); 

	double rate =  1000.0/*radialhead->LengthOfGate*/;
	int wight = radialhead->NumberOfGates*radialhead->LengthOfGate /rate +radialhead->DistanceOfFirstGate/rate;

	RadialData* radialdata = GetBlockDataAsRadial_RadialData(iLayer,jBlock,kRad);

	int IC = radialhead->NumberOfGates; //中心点，取距离库数  =  bitmp.GetWidth()/2+offset_x
	int JC = radialhead->NumberOfGates;	///	取距离库数		=  bitmp.GetHeight()/2+offset_y


	double r1=radialhead->DistanceOfFirstGate/rate;
	double r2 = r1;
	double s1,c1,s2,c2;
	s1=sin(radialdata->AzimuthAngle/100.0 * M_PI / 180.0); 
	c1=cos(radialdata->AzimuthAngle/100.0 * M_PI / 180.0);
	s2=sin((radialdata->AzimuthAngle/100.0 + radialdata->DeltaAngle/100.0 )* M_PI / 180.0);
	c2=cos((radialdata->AzimuthAngle/100.0 + radialdata->DeltaAngle/100.0 ) * M_PI / 180.0);

	gInt16* pArray = GetBlockDataAsRadial_Data(iLayer,jBlock,kRad);
	if (productCode ==258)
	{
		double dRangJiao = layer->HeightOfElevation * 100.0;
		r2 =(r1 + zGate*radialhead->LengthOfGate/rate)*1000;

		osg::Vec3 p1,p2,p3,p4;
		if (0 == zGate)
		{			
			p1=osg::Vec3 (IC,JC,0);
			p2=osg::Vec3 (IC,JC,0);
			p3=osg::Vec3 (IC,JC,0);
			p4=osg::Vec3 (IC,JC,0);
		}else
		{
			double r2d = (r1 + (zGate-1)*radialhead->LengthOfGate/rate)*1000;
			p1.set(r2d*s1,r2d*c1,0);
			p4.set(r2d*s2,r2d*c2,0);
			p2.set(r2d*s1,r2d*c1,0);
			p3.set(r2d*s2,r2d*c2,0);

		}
		p2.set(r2*s1,r2*c1,0);
		p3.set(r2*s2,r2*c2,0);

		v3Point vp1,vp2,vp3,vp4;
		proj->setProductAngle(dRangJiao);
		//osg::Vec3 src(p1.x()*1000,p1.y()*1000,0);
		osg::Vec3 res;
		if(!(proj->Transform(p1, res)))
		{
			vp1.x = res.x();
			vp1.y = res.y();
			vp1.z = dRangJiao+hhight;
			vp1.value = pArray[zGate]/blok->ScaleOfData;
			pCDV->push_back(vp1);

		}


		//osg::Vec3 src1(p2.x()*1000,p2.y()*1000,0);
		osg::Vec3 res1;
		if(!(proj->Transform(p2, res1)))
		{
			vp2.x = res1.x();
			vp2.y = res1.y();
			vp2.z = dRangJiao+hhight;
			vp2.value = pArray[zGate+1]/blok->ScaleOfData;
			pCDV->push_back(vp2);

		}

		if (kRad == radialhead->NumberOfRadials - 1)
		{
			pArray = GetBlockDataAsRadial_Data(iLayer,jBlock,0);
		}else
		{
			pArray = GetBlockDataAsRadial_Data(iLayer,jBlock,kRad+1);
		}
		

		//osg::Vec3 src2(p3.x()*1000,p3.y()*1000,0);
		osg::Vec3 res2;
		if(!(proj->Transform(p3, res2)))
		{
			vp3.x = res2.x();
			vp3.y = res2.y();
			vp3.z = dRangJiao+hhight;
			vp3.value = pArray[zGate+1]/blok->ScaleOfData;
			pCDV->push_back(vp3);

		}

	//	osg::Vec3 src3(p4.x()*1000,p4.y()*1000,0);
		osg::Vec3 res3;
		if(!(proj->Transform(p4, res3)))
		{
			vp4.x = res3.x();
			vp4.y = res3.y();
			vp4.z = dRangJiao+hhight;
			vp4.value = pArray[zGate]/blok->ScaleOfData;
			pCDV->push_back(vp4);

		}
		return;
	}
	double dRangJiao = layer->HeightOfElevation / 100.0;
	r2 =(r1 + zGate*radialhead->LengthOfGate/rate)*1000;

	osg::Vec3 p1,p2,p3,p4;
	if (0 == zGate)
	{			
		p1=osg::Vec3 (IC,JC,0);
		p2=osg::Vec3 (IC,JC,0);
		p3=osg::Vec3 (IC,JC,0);
		p4=osg::Vec3 (IC,JC,0);
	}else
	{
		double r2d = (r1 + (zGate-1)*radialhead->LengthOfGate/rate)*1000;
		p1.set(r2d*s1,r2d*c1,0);
		p4.set(r2d*s2,r2d*c2,0);
		p2.set(r2d*s1,r2d*c1,0);
		p3.set(r2d*s2,r2d*c2,0);

	}
	p2.set(r2*s1,r2*c1,0);
	p3.set(r2*s2,r2*c2,0);


	v3Point vp1,vp2,vp3,vp4;
	proj->setProductAngle(dRangJiao);
	//osg::Vec3 src(p1.x()*1000,p1.y()*1000,0);
	osg::Vec3 res;
	if(!(proj->Transform(p1, res)))
	{
		vp1.x = res.x();
		vp1.y = res.y();
		vp1.z = (res.z())*1000.0+hhight;
		vp1.value = pArray[zGate]/blok->ScaleOfData;
		pCDV->push_back(vp1);

	}


	//osg::Vec3 src1(p2.x()*1000,p2.y()*1000,0);
	osg::Vec3 res1;
	if(!(proj->Transform(p2, res1)))
	{
		vp2.x = res1.x();
		vp2.y = res1.y();
		vp2.z = (res1.z())*1000.0+hhight;
		vp2.value = pArray[zGate+1]/blok->ScaleOfData;
		pCDV->push_back(vp2);

	}


	if (kRad == radialhead->NumberOfRadials - 1)
	{
		pArray = GetBlockDataAsRadial_Data(iLayer,jBlock,0);
	}else
	{
		pArray = GetBlockDataAsRadial_Data(iLayer,jBlock,kRad+1);
	}

	//osg::Vec3 src2(p3.x()*1000,p3.y()*1000,0);
	osg::Vec3 res2;
	if(!(proj->Transform(p3, res2)))
	{
		vp3.x = res2.x();
		vp3.y = res2.y();
		vp3.z = (res2.z())*1000.0+hhight;
		vp3.value = pArray[zGate+1]/blok->ScaleOfData;
		pCDV->push_back(vp3);

	}

	//osg::Vec3 src3(p4.x()*1000,p4.y()*1000,0);
	osg::Vec3 res3;
	if(!(proj->Transform(p4, res3)))
	{
		vp4.x = res3.x();
		vp4.y = res3.y();
		vp4.z = (res3.z())*1000.0+hhight;
		vp4.value = pArray[zGate]/blok->ScaleOfData;
		pCDV->push_back(vp4);

	}
}


RadarMarchingCubesImp::RadarMarchingCubesImp( GenerationData* pPB /*= 0*/ ) : m_RadarDataParer((RadarDataParser*)pPB)
{
	m_TriArray  = new osg::Vec3Array;
	m_TriNomArray = new osg::Vec3Array;
}

RadarMarchingCubesImp::~RadarMarchingCubesImp(void)
{
}

void RadarMarchingCubesImp::GetIsosurfaceGeometry( double dIsom, osg::Group& geo )
{
}

void RadarMarchingCubesImp::GetIsosurfaceGeometry( double dIsom, osg::Vec3Array** vertexArray, osg::Vec3Array** normalArray, osg::Vec4Array** colorArray )
{
	m_dIsoValue = dIsom;
	GetCubeData();
	//MarchingCubesMain();

	*vertexArray = m_TriArray.get();
	//*normalArray = m_TriNomArray.get();
	//*colorArray = 0;
}

void RadarMarchingCubesImp::GetCubeData()
{
	if(m_CubeDataBuffer.size() != 0)
		return;
	m_RadarDataParer->SetCentral(m_RadarCentral);
	m_RadarDataParer->dumpAllCube(&m_ColorTable, &m_CubeDataBuffer);
}

void RadarMarchingCubesImp::SetColorTable(COLORTABLE_VECTOR& pColorTable )
{
	m_ColorTable = pColorTable;
}

void RadarMarchingCubesImp::SetRadarCentral( double x, double y, double z )
{
	m_RadarCentral.set(x, y, z);
}

P_CUBE_DATA_VECTOR RadarMarchingCubesImp::GetCubes()
{
	GetCubeData();

	return &m_CubeDataBuffer;
}

Cube* RadarMarchingCubesImp::GetCubes( int* iCount )
{
	return 0;
}
