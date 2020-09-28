#include "SingleDataProvider.h"
#include "goQuadrilateral.h"
#include "goGeometry.h"
#include <QtCore/QDebug>
#include <QtGui/QImage>
#include <QtCore/QFileInfo>
#include <qtcore/QCoreApplication>


SingleDataProvider::SingleDataProvider()
{
}

SingleDataProvider::~SingleDataProvider() 
{}

CProductDrawType SingleDataProvider::GetTypeOfProduct()
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

int SingleDataProvider::GetCountOfLayer()
{
	bool bFlag=false;
	int nCount=0;
	gInt16 nProductCode=0;
	CProductDrawType nType=GetTypeOfProduct();
	GetProductCode(nProductCode);
	
	if(nType==enProductStruct)
	{
		/*CProductDraw* p=GetInstanceOfProductDraw(nProductCode);
		if(p!=NULL)
		{
			nCount=p->GetCountOfLayer((CProductRead*)this);
			bFlag=true;
			ReleaseInstanceOfProductDraw(p);
		}*/
	}

	if(bFlag!=true)
	{
		PDBlock* blok = GetPDBlock();
		if(blok!=NULL )
		{
			nCount=blok->NumberOfLayers;
		}
	}
	return nCount;
}

void SingleDataProvider::ProdDataProvider( int requestLayer,double &maxValue,QHash<int,QVector4D>colorHash, Layer &pLayer,RadProjTransform *proj)
{
	CProductDrawType nType=this->GetTypeOfProduct();
	if(nType!=enProductUnkown)
	{
		int n = this->GetCountOfLayer();
		if (requestLayer<0)
		{
			gInt16 nProductCode=0;
			GetProductCode(nProductCode);
			if (nProductCode==258)
			{
				requestLayer = n/2;
			}
			else
			{
				requestLayer = 0;
			}
		}

		switch(nType)
		{
		case enProductRadial:
			GetRadialData(pLayer,proj,requestLayer,colorHash,maxValue);
			break;
		case enProductGrid:
			SinGetGridData(pLayer,proj,requestLayer,colorHash,maxValue);
			break;
		case enProductStruct:
		case enProductUnkown:
		default:
			break;
		}
	}
}

void SingleDataProvider::GetRadialData( Layer &pLayer,RadProjTransform *proj, int requestLayer,QHash<int ,QVector4D>colorHash,double &maxValue )
{
	PDBlock* blok = GetPDBlock();
	if(blok==NULL)
	{
		return;
	}
	int productcode = blok->ProductCode;
	proAttr pAttributeHaHa;
	cDrawProduct*          pDrawProduct=(cDrawProduct *)this;
	pDrawProduct->ReadInfo(&pAttributeHaHa,0);

	float hhight = QString::number((float)pAttributeHaHa.altitude*0.1,'f',2).toFloat();

	float lat = pAttributeHaHa.latitude/10000.0;
	float lon = pAttributeHaHa.longitude/10000.0;

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

	double step;
	bool ifkey = false;
	if (251==productcode||252==productcode||262==productcode||263==productcode||280==productcode||282==productcode||283==productcode
		||284==productcode||288==productcode||278==productcode||291==productcode||292==productcode||294==productcode||293==productcode)
	{
		step = 5.0;
		ifkey = true;
	}else if (254==productcode||255==productcode||301==productcode)
	{
		proAttr pAttributeHaHa;
		cDrawProduct*          pDrawProduct=(cDrawProduct *)this;
		if (pDrawProduct != NULL)
		{
			pDrawProduct->ReadInfo(&pAttributeHaHa,requestLayer);
		}
		step = 15*pAttributeHaHa.param1/100;
		ifkey = true;
	}else if (256==productcode||257==productcode)
	{
		step = 1;
		ifkey = true;
	}else if (267 == productcode)
	{
		step = 0.5;
		ifkey = true;
	}else if (269 == productcode)
	{
		step = 2;
		ifkey = true;
	}

	Layer::MgoGeometry* pMG = pLayer.GetMgoGeometry();
	osg::Vec3 ps[4];
	double s1,c1,s2,c2;
	osg::Vec3 p1,p2,p3,p4;
	 
	if (productcode == 258 )
	{
		int layerHight = GetRadiaElevation(requestLayer) /**100*/;
		//for (int i = requestLayer ; i < blok->NumberOfLayers ; i++)
		//{
			//qDebug()<<"Layer"<< i; 
			DILayer* layer = GetLayer(requestLayer);
			if(layer == NULL)
				return;
			for (int j = 0;j < layer->NumberOfBlocks ; j++)
			{
				//std::vector<gInt16> vec;
				//qDebug()<<"Layer"<< i <<"block"<<layer->NumberOfBlocks; 
				DLIBlock* dblock = GetBlock(requestLayer,j);
				if(dblock == NULL)
				{
					return;
				}
				if(dblock->TypeOfDataPackets == BLOCKTYPE_RADIAL)
				{
					RadialHead* radialhead = GetBlockDataAsRadial_Head(requestLayer,j); 
					double rate =  1000.0;
					for (int k = 0; k <radialhead->NumberOfRadials; k++)        //
					{
						RadialData* radialdata = GetBlockDataAsRadial_RadialData(requestLayer,j,k);

						int IC = radialhead->NumberOfGates;
						int JC = radialhead->NumberOfGates;

						p1=osg::Vec3(IC,JC,0);
						p2=osg::Vec3(IC,JC,0);
						p3=osg::Vec3(IC,JC,0);
						p4=osg::Vec3(IC,JC,0);

						ps[0].set(p1);
						ps[1].set(p2);
						ps[2].set(p3);
						ps[3].set(p4);

						double r1 = radialhead->DistanceOfFirstGate/rate;
						double r2 = r1;

						s1=sin(radialdata->AzimuthAngle/100.0 * M_PI / 180.0); 
						c1=cos(radialdata->AzimuthAngle/100.0 * M_PI / 180.0);
						s2=sin((radialdata->AzimuthAngle/100.0 + radialdata->DeltaAngle/100.0 )* M_PI / 180.0);
						c2=cos((radialdata->AzimuthAngle/100.0 + radialdata->DeltaAngle/100.0 ) * M_PI / 180.0);
						gInt16* pArray = GetBlockDataAsRadial_Data(requestLayer,j,k);

						for (int z  = 0 ;z < radialhead->NumberOfGates ;z++)
						{
							gInt16 val = pArray[z];	
#if 1						
							p1 = p2;
							p4 = p3;
							if (val == -32768 || val== -32767) 
							{
								r2 =(r1 + z*radialhead->LengthOfGate/rate);
								p2.set(r2*s1,r2*c1,0);
								p3.set(r2*s2,r2*c2,0);
								ps[0].set(p1);
								ps[1].set(p2);
								ps[2].set(p3);
								ps[3].set(p4);
								continue;
							}
							if (z>0)
							{
								float tempValue =((float)val)/blok->ScaleOfData;
								int key_temp = tempValue/5;

								int index_ex = z-1;
								gInt16 val_ex = pArray[index_ex];
								float tempValue_ex = ((float)val_ex)/blok->ScaleOfData;
								int key_ex = tempValue_ex/5;

								if (key_temp == key_ex &&z != (radialhead->NumberOfGates -1)) continue;

							}
#endif
							maxValue = val>maxValue ? val:maxValue;
							osg::Vec4 tempcolor;
							r2 =(r1 + z*radialhead->LengthOfGate/rate);
							p2.set(r2*s1,r2*c1,0);
							p3.set(r2*s2,r2*c2,0);

							ps[0].set(p1);
							ps[1].set(p2);
							ps[2].set(p3);
							ps[3].set(p4);

							if ( -32766 == val )
							{
								int key = -32766;
								tempcolor.set(0.5f,0,0.5f,1);
								temp_array = dataMap.value(key);
								temp_color_array = colorMap.value(key);
								for(int ip = 0; ip<4; ip++)
								{
									osg::Vec3 src(ps[ip].x()*1000,ps[ip].y()*1000,0);
									osg::Vec3 res;
									if(!(proj->Transform(src, res)))
									{
										res.set(res.x(),res.y(),layerHight+hhight);
										temp_array->push_back(res);
										temp_color_array->push_back(tempcolor);
									}
								}
							}//end if(var == -32766)
							else
							{
								float value =((float)val)/blok->ScaleOfData;	
								maxValue = value>maxValue ? value:maxValue;
								osg::Vec4 tempcolor;
								int key = value/5;

								if (key >colorHash.size()||key < 0)
								{
									continue;
								}

								QVector4D tempcolor_QT = colorHash.value(key);;
								tempcolor.set(tempcolor_QT.x() ,tempcolor_QT.y(),tempcolor_QT.z(),1.0);
								temp_array = dataMap.value(key);
								
								if (NULL ==temp_array)
								{
									//qDebug()<<key;
									continue;
								}
								//osg::ref_ptr <osg::Vec4Array> temp_color_arr = colorMap.value(key);
								temp_color_array = colorMap.value(key);
								for(int ip = 0; ip<4; ip++)
								{
									osg::Vec3 src(ps[ip].x()*1000,ps[ip].y()*1000,0);
									osg::Vec3 res;
									if(!(proj->Transform(src, res)))
									{
										res.set(res.x(),res.y(),layerHight+hhight);
										temp_array->push_back(res);
										temp_color_array->push_back(tempcolor);
									}
								}
							}//end else
						}//end for gates
					}
				}	
			}
			QMap<int,osg::ref_ptr <osg::Vec3Array>> ::iterator it;
			int pNum = 0;
			for (it= dataMap.begin() ; it != dataMap.end(); ++it )
			{
				goQuadrilateral* gpl = new goQuadrilateral();
				gpl->SetCoordinateTransform(pLayer.CoordTrans());
				gpl->SetOsgArray(it.value());
				gpl->SetOsgColorArray(*(colorMap.value(it.key())));
				gpl->SetParentLayer(&pLayer);


				pMG->insert(Layer::MgoGeometryPair(pNum,gpl));
				pNum++;
			}

	}//end if productcood ==258
else 
	{
		DILayer* layer = GetLayer(requestLayer);
		if(layer == NULL)
			return;
		for (int j = 0;j < layer->NumberOfBlocks ; j++)
		{
			DLIBlock* dblock = GetBlock(requestLayer,j);
			if(dblock == NULL)
			{
				return;
			}
			if(dblock->TypeOfDataPackets == BLOCKTYPE_RADIAL)
			{
				RadialHead* radialhead = GetBlockDataAsRadial_Head(requestLayer,j); 
				double rate =  1000.0/*radialhead->LengthOfGate*/;
				for (int k = 0; k <radialhead->NumberOfRadials; k++)
				{
					RadialData* radialdata = GetBlockDataAsRadial_RadialData(requestLayer,j,k);

					int IC = radialhead->NumberOfGates; 
					int JC = radialhead->NumberOfGates;	
					p1=osg::Vec3(IC,JC,0);
					p2=osg::Vec3(IC,JC,0);
					p3=osg::Vec3(IC,JC,0);
					p4=osg::Vec3(IC,JC,0);

					ps[0].set(p1);
					ps[1].set(p2);
					ps[2].set(p3);
					ps[3].set(p4);

					double r1= radialhead->DistanceOfFirstGate/*/rate*/ ;

					double r2 = r1;

					s1=sin(radialdata->AzimuthAngle/100.0 * M_PI / 180.0); 
					c1=cos(radialdata->AzimuthAngle/100.0 * M_PI / 180.0);
					s2=sin((radialdata->AzimuthAngle/100.0 + radialdata->DeltaAngle/100.0 )* M_PI / 180.0);
					c2=cos((radialdata->AzimuthAngle/100.0 + radialdata->DeltaAngle/100.0 ) * M_PI / 180.0);

					gInt16* pArray = GetBlockDataAsRadial_Data(requestLayer,j,k);

					for (int z  = 0 ;z < radialhead->NumberOfGates ;z++)
					{
						gInt16 val = pArray[z];	
						p1 = p2;
						p4 = p3;
#if 1
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

						if (z>0)
						{
							float tempValue =((float)val)/blok->ScaleOfData;
							int key_temp = tempValue/5;

							int index_ex = z-1;
							gInt16 val_ex = pArray[index_ex];
							float tempValue_ex = ((float)val_ex)/blok->ScaleOfData;
							int key_ex = 0;
							if (ifkey)
							{
								key_ex = tempValue_ex/step;
							}else
							{
								key_ex = GetColorIndexKey(productcode,tempValue_ex);
							}

							if (key_temp == key_ex &&z != (radialhead->NumberOfGates -1)) continue;

						}
#endif
						maxValue = val>maxValue ? val:maxValue;
						osg::Vec4 tempcolor;
						r2 =(r1 + z*radialhead->LengthOfGate/rate)*1000;
						p2.set(r2*s1,r2*c1,0);
						p3.set(r2*s2,r2*c2,0);
						ps[0].set(p1);
						ps[1].set(p2);
						ps[2].set(p3);
						ps[3].set(p4);

						if ( -32766 == val )
						{

							int key = -32766;
							tempcolor.set(0.5f,0,0.5f,1);
							temp_array = dataMap.value(key);
							temp_color_array = colorMap.value(key);
							for(int ip = 0; ip<4; ip++)
							{
								osg::Vec3 res;
								if(!(proj->Transform(ps[ip], res)))
								{
									res.set(res.x(),res.y(),(res.z())*1000.0+hhight);
									temp_array->push_back(res);
									temp_color_array->push_back(tempcolor);
								}
							}
						}//end if(var == -32766)
						else
						{
							float value =((float)val)/blok->ScaleOfData;	
							maxValue = value>maxValue ? value:maxValue;
							int key = 0;
							if (ifkey)
							{
								key = value/step;
							}else
							{
								key = GetColorIndexKey(productcode,value);
							}

							//if (key >colorHash.size()||key < 0)
							//{
							//	continue;
							//}

							QVector4D tempcolor_QT = colorHash.value(key);;
							tempcolor.set(tempcolor_QT.x() ,tempcolor_QT.y(),tempcolor_QT.z(),1.0);
							temp_array = dataMap.value(key);
							temp_color_array = colorMap.value(key);
							if (NULL ==temp_array)
							{
								continue;
							}
							for(int ip = 0; ip<4; ip++)
							{
								osg::Vec3 res;
								if(!(proj->Transform(ps[ip], res)))
								{
									res.set(res.x(),res.y(),(res.z())*1000.0+hhight);
									temp_array->push_back(res);
									temp_color_array->push_back(tempcolor);
								}
							}						
						}
					}
				}
			}	
		}
		QMap<int,osg::ref_ptr <osg::Vec3Array>> ::iterator it;
		int pNum = 0;
		for (it= dataMap.begin() ; it != dataMap.end(); ++it )
		{
			goQuadrilateral* gpl = new goQuadrilateral();
			gpl->SetCoordinateTransform(pLayer.CoordTrans());
			gpl->SetOsgArray(it.value());
			gpl->SetOsgColorArray(*(colorMap.value(it.key())));
			gpl->SetParentLayer(&pLayer);
			pMG->insert(Layer::MgoGeometryPair(pNum,gpl));
			pNum++;
		}
	}
	
	
}

void SingleDataProvider::SinGetGridData(Layer &pLayer, RadProjTransform *proj,int requestLayer,QHash<int,QVector4D>colorHash,double &maxValue)
{
	PDBlock* blok = GetPDBlock();
	if(blok==NULL)
	{
		return;
	}

	int productcode = blok->ProductCode;
	proAttr pAttributeHaHa;
	cDrawProduct*          pDrawProduct=(cDrawProduct *)this;
	pDrawProduct->ReadInfo(&pAttributeHaHa,0);
	float hhight = QString::number((float)pAttributeHaHa.altitude*0.1,'f',2).toFloat();
	QMap <int,osg::ref_ptr <osg::Vec3Array>> dataMap;
	QMap <int,osg::ref_ptr <osg::Vec4Array>> colorMap;
	osg::ref_ptr <osg::Vec3Array> temp_array = NULL  ;//数据数组
	osg::ref_ptr <osg::Vec4Array> temp_color_array =NULL;//颜色数组

	double step;
	bool ifkey = false;
	if (251==productcode||252==productcode||258==productcode||262==productcode||263==productcode||280==productcode||282==productcode||283==productcode
		||284==productcode||288==productcode||278==productcode||291==productcode||292==productcode||294==productcode||293==productcode)
	{
		step = 5.0;
		ifkey = true;
	}else if (254==productcode||255==productcode||301==productcode)
	{
		proAttr pAttributeHaHa;
		cDrawProduct*          pDrawProduct=(cDrawProduct *)this;
		if (pDrawProduct != NULL)
		{
			pDrawProduct->ReadInfo(&pAttributeHaHa,requestLayer);
		}
		step = 15*pAttributeHaHa.param1/100;
		ifkey = true;
	}else if (256==productcode||257==productcode)
	{
		step = 1;
		ifkey = true;
	}else if (267 == productcode)
	{
		step = 0.5;
		ifkey = true;
	}else if (269 == productcode)
	{
		step = 2;
		ifkey = true;
	}
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
	Layer::MgoGeometry* pMG = pLayer.GetMgoGeometry();

	if(requestLayer<0)
	{
		requestLayer=0;
	}
	DILayer* layer = GetLayer(requestLayer);
	if(layer == NULL)
		return;
	double cosElevation;
	if (layer->HeightOfElevation == -1 )
	{
		cosElevation = 1;
	}
	else
	{
		cosElevation =cos(layer->HeightOfElevation/100.0 * M_PI / 180);
	}

	for (int j = 0;j < layer->NumberOfBlocks ; j++)
	{
		DLIBlock* dblock = GetBlock(requestLayer,j);
		if(dblock == NULL)
		{
			return;
		}
		if (dblock->TypeOfDataPackets == BLOCKTYPE_GRID)
		{
			GridHead* pgridhead = GetBlockDataAsGrid_Head(requestLayer,j);
			int center = pgridhead->NumberOfCols/2;
			float nwidth ;    
			float nheight ;
			if(productcode == 294)                   
			{
					 
				nwidth = 1;
				nheight = 1;
				//m_radius =nwidth; 
			}
			else
			{
				nwidth = pgridhead->ResolutionOfRow/1000.0;
				nheight = pgridhead->ResolutionOfCol/1000.0;
				///*m_radius*/ =nwidth; 
			}              
			
			for (int ii = 0; ii< pgridhead->NumberOfRows ;ii++)
			{
				GridRowData* rowdata = GetBlockDataAsGrid_RowData(requestLayer,j,ii);

				int r1 = nwidth;				
									
				osg::Vec3 p1(0,ii*nheight,0);
				osg::Vec3 p2(0,ii*nheight,0);
				osg::Vec3 p3(0,ii*nheight,0);
				osg::Vec3 p4(0,(ii+1)*nheight,0);

				osg::Vec3 ps[4];
				gInt16 *arr = GetBlockDataAsGrid_Data(requestLayer,j,ii);
				for (int jj = 0 ;jj < pgridhead->NumberOfCols ; jj++)
				{
					p1=p2;
					p4=p3;
					if(arr[jj]==-32768 || arr[jj]==-32767 /*|| -32766 == arr[jj]*/ )
					{
						r1 = jj * nwidth;
						p2.set(r1-center* nwidth,center* nwidth-ii*nheight,0);
						p3.set(r1-center* nwidth,center* nwidth-(ii+1)*nheight,0);

						ps[0].set(p1);
						ps[1].set(p2);
						ps[2].set(p3);
						ps[3].set(p4);
						continue;
					}
					r1 = jj * nwidth;
					p2.set(r1-center* nwidth,center* nwidth-ii*nheight,0);
					p3.set(r1-center* nwidth,center* nwidth-(ii+1)*nheight,0);

					ps[0].set(p1);
					ps[1].set(p2);
					ps[2].set(p3);
					ps[3].set(p4);
						
					float temp = -32768;
					//逆风区产品不一样，产品号301，缩写IW
					if(productcode == 301)
					{
						if((arr[jj]>= -2700) && (arr[jj]<= 2700))  //正常值
						{
							temp = ((float)arr[jj])/blok->ScaleOfData;
						}
						else if(arr[jj] < -15000) 
						{
							arr[jj] = arr[jj] + 15000;
							temp = ((float)arr[jj])/blok->ScaleOfData; 
						}
						else if(arr[jj] > 15000)  
						{
							arr[jj] = arr[jj] - 15000;
							temp = ((float)arr[jj])/blok->ScaleOfData;
						}
						else if((arr[jj]<= -2700 && arr[jj]>=-15000)||(arr[jj]<= 15000 && arr[jj]>=2700)) //在-15000~-2700之间以及2700~15000之间的值也按照正常数据进行处理
						{
							temp = ((float)arr[jj])/blok->ScaleOfData;
						}
					}
					else
					{							         
						if(productcode == 294)                 
						{
							temp = (float)arr[jj];   //294产品不需要除以放大倍数，因为本身值很小，所以需要放大显示
						}
						else
						{
							temp = ((float)arr[jj])/blok->ScaleOfData;
						}     
					}
					maxValue = temp>maxValue ? temp:maxValue;
					osg::Vec4 tempcolor;
					if ( -32766 == temp )
					{
						int key = -32766;
						tempcolor.set(0.5f,0,0.5f,1);
						osg::ref_ptr <osg::Vec3Array> temp_data_arr = dataMap.value(key);
						osg::ref_ptr <osg::Vec4Array> temp_color_arr = colorMap.value(key);
						for(int ip = 0; ip<4; ip++)
						{
							osg::Vec3 src(ps[ip].x()*1000,ps[ip].y()*1000,0);
							osg::Vec3 res;
							if(!(proj->Transform(src, res)))
							{
								res.set(res.x(),res.y(),(res.z())*1000.0+hhight);
								temp_data_arr->push_back(res);
								temp_color_arr->push_back(tempcolor);
							}
						}
					}
					else
					{

						int key = 0;
						if (ifkey)
						{
							key = temp/step;
						}else
						{
							key = GetColorIndexKey(productcode,temp);
						}
						
						QVector4D tempcolor_QT = colorHash.value(key);
						tempcolor.set(tempcolor_QT.x() ,tempcolor_QT.y(),tempcolor_QT.z(),0.7);
						osg::ref_ptr <osg::Vec3Array> temp_data_arr = dataMap.value(key);
						osg::ref_ptr <osg::Vec4Array> temp_color_arr = colorMap.value(key);
						if(!temp_color_arr||!temp_data_arr)
						{
							continue;
						}
						for(int ip = 0; ip<4; ip++)
						{
							osg::Vec3 src(ps[ip].x()*1000,ps[ip].y()*1000,0);
							osg::Vec3 res;
							if(!(proj->Transform(src, res)))
							{
								res.set(res.x(),res.y(),(res.z())*1000.0+hhight);
								temp_data_arr->push_back(res);
								temp_color_arr->push_back(tempcolor);
							}
						}
					} 
				}
			}
		}
	}
	QMap<int,osg::ref_ptr <osg::Vec3Array>> ::iterator it;
	int pNum = 0;
	for (it= dataMap.begin() ; it != dataMap.end(); ++it )
	{
		goQuadrilateral* gpl = new goQuadrilateral();
		gpl->SetCoordinateTransform(pLayer.CoordTrans());
		gpl->SetOsgArray(it.value());
		gpl->SetOsgColorArray(*(colorMap.value(it.key())));
		gpl->SetParentLayer(&pLayer);


		pMG->insert(Layer::MgoGeometryPair(pNum,gpl));
		pNum++;
	}
}

double SingleDataProvider::GetDataRadius( int ilayer )
{
	RadialGridDataList DataList;

	CProductDrawType nType=this->GetTypeOfProduct();
	if(nType!=enProductUnkown)
	{
		int n = this->GetCountOfLayer();
		if (ilayer<0)
		{
			gInt16 nProductCode=0;
			GetProductCode(nProductCode);
			if (nProductCode==258)
			{
				ilayer = n/2;
			}
			else
			{
				ilayer = 0;
			}
		}

		switch(nType)
		{
		case enProductRadial:
			return GetRadialRadius(ilayer);
			break;
		case enProductGrid:
			return GetGridRadius(ilayer);
			break;
		case enProductStruct:
			return 0;
		case enProductUnkown:
			return 0;
		default:
			return 0;

			break;
		}
	}else
		return 0;
}

double SingleDataProvider::GetRadialRadius( int ilayer )
{
	double radius;
	PDBlock* blok = GetPDBlock();
	if(blok==NULL)
	{
		return 0;
	}
	if(ilayer<0)
	{
		ilayer=0;
	}
	DILayer* layer = GetLayer(ilayer);
	if(layer == NULL)
		return 0;
	DLIBlock* dblock = GetBlock(ilayer,0);
	if(dblock == NULL)
	{
		return 0;
	}
	if(dblock->TypeOfDataPackets == BLOCKTYPE_RADIAL)
	{
		RadialHead* radialhead = GetBlockDataAsRadial_Head(ilayer,0); 
		radius = (radialhead->NumberOfGates*radialhead->LengthOfGate + radialhead->DistanceOfFirstGate);
	}
	return radius;
}

double SingleDataProvider::GetGridRadius( int ilayer )
{
	double radius;
	PDBlock* blok = GetPDBlock();
	if(blok==NULL)
	{
		return 0;
	}


	if(ilayer<0)
	{
		ilayer=0;
	}

	DILayer* layer = GetLayer(ilayer);
	if(layer == NULL)
		return 0;

	DLIBlock* dblock = GetBlock(ilayer,0);
	if(dblock == NULL)
	{
		return 0;
	}
	if (dblock->TypeOfDataPackets == BLOCKTYPE_GRID)
	{
		GridHead* pgridhead = GetBlockDataAsGrid_Head(ilayer,0);
		radius = pgridhead->NumberOfCols * pgridhead->ResolutionOfCol / 2.0; 
	}
	return radius;
}

int SingleDataProvider::ProductCode()
{
	PDBlock* blok = GetPDBlock();
	if(blok==NULL)
	{
		return 0;
	}

	int productCode = blok->ProductCode;
	return productCode;
}

double SingleDataProvider::GetRadiaResolution(int ilayer)
{
	RadialGridDataList DataList;
	double resolution;
	CProductDrawType nType=this->GetTypeOfProduct();
	if(nType!=enProductUnkown)
	{
		int n = this->GetCountOfLayer();
		if (ilayer<0)
		{
			gInt16 nProductCode=0;
			GetProductCode(nProductCode);
			if (nProductCode==258)
			{
				ilayer = n/2;
			}
			else
			{
				ilayer = 0;
			}
		}

		switch(nType)
		{
		case enProductRadial:
			{
				
				PDBlock* blok = GetPDBlock();
				if(blok==NULL)
				{
					return 0;
				}
				if(ilayer<0)
				{
					ilayer=0;
				}
				DILayer* layer = GetLayer(ilayer);
				if(layer == NULL)
					return 0;

				DLIBlock* dblock = GetBlock(ilayer,0);
				if(dblock == NULL)
				{
					return 0;
				}
				if(dblock->TypeOfDataPackets == BLOCKTYPE_RADIAL)
				{
					RadialHead* radialhead = GetBlockDataAsRadial_Head(ilayer,0); 
					resolution = radialhead->LengthOfGate;
				}
				return resolution;
			break;	
			}

		case enProductGrid:
			{
				PDBlock* blok = GetPDBlock();
				if(blok==NULL)
				{
					return 0;
				}


				if(ilayer<0)
				{
					ilayer=0;
				}

				DILayer* layer = GetLayer(ilayer);
				if(layer == NULL)
					return 0;

				DLIBlock* dblock = GetBlock(ilayer,0);
				if(dblock == NULL)
				{
					return 0;
				}
				if (dblock->TypeOfDataPackets == BLOCKTYPE_GRID)
				{
					GridHead* pgridhead = GetBlockDataAsGrid_Head(ilayer,0);
					resolution = pgridhead->ResolutionOfCol;
				}
				return resolution;
				break;
			}

			
		case enProductStruct:
			return 0;
		case enProductUnkown:
			return 0;
		default:
			return 0;

			break;
		}
	}
	else
		return 0;
}

double SingleDataProvider::GetRadiaElevation( int ilayer )
{
	double radius;
	PDBlock* blok = GetPDBlock();
	if(blok==NULL)
	{
		return 0;
	}
	if(ilayer<0)
	{
		ilayer=0;
	}
	DILayer* layer = GetLayer(ilayer);
	return layer->HeightOfElevation;
}

void SingleDataProvider::DrawPic( QString& filePath,int requestLayer,P_COLORTABLE_VECTOR pColor)
{
	CProductDrawType nType=this->GetTypeOfProduct();
	if(nType!=enProductUnkown)
	{
		int n = this->GetCountOfLayer();
		if (requestLayer<0)
		{
			gInt16 nProductCode=0;
			GetProductCode(nProductCode);
			if (nProductCode==258)
			{
				requestLayer = n/2;
			}
			else
			{
				requestLayer = 0;
			}
		}

		switch(nType)
		{
		case enProductRadial:
			DrawRadialPic(filePath,requestLayer,pColor);
			break;
		case enProductGrid:
			DrawGridPic(filePath,requestLayer,pColor);
			break;
		case enProductStruct:
		case enProductUnkown:
		default:
			break;
		}
	}
}

void SingleDataProvider::DrawRadialPic( QString& filePath,int requestLayer,P_COLORTABLE_VECTOR pColorTable)
{
	PDBlock* blok = GetPDBlock();
	if(blok==NULL)
	{
		return;
	}
	int productCode = blok->ProductCode;
	double ra = GetRadialRadius(0)/1000;
	for (int i = requestLayer ; i < blok->NumberOfLayers ; i++)
	{
		QString backPic =  QCoreApplication::applicationDirPath() + "/Config/BackGround.png";
		QImage backimg(backPic);
		QImage image = backimg.copy(0,0,2*ra,2*ra);
		QRgb value;
		value = qRgb(255,255,255);
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

				//double rate = 1.0; /*= 2.0*(double)wight/(double)bitmp.GetHeight()/zoom*/ //精度，需要重新设定
				double rate =  1000.0/*radialhead->LengthOfGate*/;
				int wight = radialhead->NumberOfGates*radialhead->LengthOfGate /rate +radialhead->DistanceOfFirstGate/rate;
				for (int k = 0; k <radialhead->NumberOfRadials; k++)        //
				{
					RadialData* radialdata = GetBlockDataAsRadial_RadialData(i,j,k);

					int IC = radialhead->NumberOfGates; //中心点，取距离库数  =  bitmp.GetWidth()/2+offset_x
					int JC = radialhead->NumberOfGates;	///	取距离库数		=  bitmp.GetHeight()/2+offset_y

					osg::Vec3 p1(IC,JC,0);
					osg::Vec3 p2(IC,JC,0);
					osg::Vec3 p3(IC,JC,0);
					osg::Vec3 p4(IC,JC,0);

					osg::Vec3 ps[4];
					double s1,c1,s2,c2;
					ps[0].set(p1.x(),p1.y(),0);
					ps[1].set(p2.x(),p2.y(),0);
					ps[2].set(p3.x(),p3.y(),0);
					ps[3].set(p4.x(),p4.y(),0);

					double r1=radialhead->DistanceOfFirstGate/rate;
					double r2 = r1;

					s1=sin(radialdata->AzimuthAngle/100.0 * M_PI / 180.0); 
					c1=cos(radialdata->AzimuthAngle/100.0 * M_PI / 180.0);
					s2=sin((radialdata->AzimuthAngle/100.0 + radialdata->DeltaAngle/100.0 )* M_PI / 180.0);
					c2=cos((radialdata->AzimuthAngle/100.0 + radialdata->DeltaAngle/100.0 ) * M_PI / 180.0);

					gInt16* pArray = GetBlockDataAsRadial_Data(i,j,k);

					for (int z  = 0 ;z < radialhead->NumberOfGates ;z++)
					{
						r2 =(r1 + z*radialhead->LengthOfGate/rate);

						p1=p2;
						p4=p3;
						p2.set(r2*s1,r2*c1,0);
						p3.set(r2*s2,r2*c2,0);

						gInt16 val = pArray[z];						
						if (val != -32768 && val!= -32767)
						{
							if (val == -32766)
							{
								for (int ic = 0;ic < pColorTable->count();ic++)
								{
									P_COLORNODE_PRODUCT color = pColorTable->at(ic);
									if (val >= color->fStart && val <= color->fEnd)
									{
										value = qRgb(color->nRed,color->nGreen,color->nBlue);
										for(int ip = 0; ip<4; ip++)
										{
											int xx = (int)(ps[ip].x()+ra);
											int yy = 2*ra - (int)(ps[ip].y()+ra);
											image.setPixel(xx,yy, value);
											image.setPixel(xx,yy+1, value);
											image.setPixel(xx+1,yy, value);
											image.setPixel(xx+1,yy+1, value);

										}

										break;
									}
									else if ( -32766 == val )
									{
										value = qRgb(127,0,127);
										for(int ip = 0; ip<4; ip++)
										{
											int xx = (int)(ps[ip].x()+ra);
											int yy = 2*ra - (int)(ps[ip].y()+ra);
											image.setPixel(xx,yy, value);
											image.setPixel(xx,yy+1, value);
											image.setPixel(xx+1,yy, value);
											image.setPixel(xx+1,yy+1, value);
										}
										break;
									}

								}

							}
							else
							{
								float valu =((float)val)/blok->ScaleOfData;									
								for (int id = 0;id < pColorTable->count();id++)
								{
									P_COLORNODE_PRODUCT color = pColorTable->at(id);
									if (valu >= color->fStart && valu<= color->fEnd)
									{
										value = qRgb(color->nRed,color->nGreen,color->nBlue);
										for(int ip = 0; ip<4; ip++)
										{
											int xx = (int)(ps[ip].x()+ra);
											int yy = 2*ra - (int)(ps[ip].y()+ra);
											image.setPixel(xx,yy, value);
											image.setPixel(xx,yy+1, value);
											image.setPixel(xx+1,yy, value);
											image.setPixel(xx+1,yy+1, value);
										}
										break;
									}
									else if ( -32766 == valu )
									{
										value = qRgb(127,0,127);
										for(int ip = 0; ip<4; ip++)
										{
											int xx = (int)(ps[ip].x()+ra);
											int yy = 2*ra - (int)(ps[ip].y()+ra);
											image.setPixel(xx,yy, value);
											image.setPixel(xx,yy+1, value);
											image.setPixel(xx+1,yy, value);
											image.setPixel(xx+1,yy+1, value);
										}
										break;
									}

								}
							}
						}
						ps[0].set(p1.x(),p1.y(),0);
						ps[1].set(p2.x(),p2.y(),0);
						ps[2].set(p3.x(),p3.y(),0);
						ps[3].set(p4.x(),p4.y(),0);

					}
				}
			}	
		}
		//QString tt = QString(filePath.c_str);
		image.save(filePath);
		break;		
	}
}

void SingleDataProvider::DrawGridPic( QString& filePath,int requestLayer,P_COLORTABLE_VECTOR pColorTable )
{
	PDBlock* blok = GetPDBlock();
	if(blok==NULL)
	{
		return;
	}
	double ra = GetRadialRadius(0)/1000;
	int productCode = blok->ProductCode;

	if(requestLayer<0)
	{
		requestLayer=0;
	}

	for (int i = requestLayer ; i < blok->NumberOfLayers ; i++)
	{
		QString backPic =  QCoreApplication::applicationDirPath() + "/Config/BackGround.png";
		QImage backimg(backPic);
		QImage image = backimg.copy(0,0,2*ra,2*ra);
		QRgb value;
		value = qRgb(255,255,255);
		//QString filepath();
		//filepath.replace("layer***","layer"+QString::number(i));
		DILayer* layer = GetLayer(i);
		if(layer == NULL)
			return;
		double cosElevation;
		if (layer->HeightOfElevation == -1 )
		{
			cosElevation = 1;
		}
		else
		{
			cosElevation =cos(layer->HeightOfElevation/100.0 * M_PI / 180);
		}

		for (int j = 0;j < layer->NumberOfBlocks ; j++)
		{
			DLIBlock* dblock = GetBlock(i,j);
			if(dblock == NULL)
			{
				return;
			}
			if (dblock->TypeOfDataPackets == BLOCKTYPE_GRID)
			{
				GridHead* pgridhead = GetBlockDataAsGrid_Head(i,j);
				int center = pgridhead->NumberOfCols/2;
				float nwidth ;    
			    float nheight ;
				if(productCode == 294)                   
				{
					 
					nwidth = 1;
				    nheight = 1;
					
				}
				else
				{
					nwidth = pgridhead->ResolutionOfRow/1000.0;
				    nheight = pgridhead->ResolutionOfCol/1000.0;

				}              
			
				for (int ii = 0; ii< pgridhead->NumberOfRows ;ii++)
				{
					GridRowData* rowdata = GetBlockDataAsGrid_RowData(i,j,ii);

					int r1 = nwidth;				
									
					osg::Vec3 p1(0,ii*nheight,0);
					osg::Vec3 p2(0,ii*nheight,0);
					osg::Vec3 p3(0,ii*nheight,0);
					osg::Vec3 p4(0,(ii+1)*nheight,0);

					osg::Vec3 ps[4];

					for (int jj = 0 ;jj < pgridhead->NumberOfCols ; jj++)
					{
						r1 = jj * nwidth;
						p1=p2;
						p4=p3;

						p2.set(r1-center* nwidth,center* nwidth-ii*nheight,0);
						p3.set(r1-center* nwidth,center* nwidth-(ii+1)*nheight,0);

						ps[0].set(p1.x(),p1.y(),0);
						ps[1].set(p2.x(),p2.y(),0);
						ps[2].set(p3.x(),p3.y(),0);
						ps[3].set(p4.x(),p4.y(),0);

						gInt16 *arr = GetBlockDataAsGrid_Data(i,j,ii);
						float temp = -32768;
					    if(arr[jj]!=-32768 && arr[jj]!=-32767)
						{
						   //逆风区产品不一样，产品号301，缩写IW
							if(productCode == 301)
							{
								if((arr[jj]>= -2700) && (arr[jj]<= 2700))  //正常值
								{
									temp = ((float)arr[jj])/blok->ScaleOfData;
								}
								else if(arr[jj] < -15000) 
								{
									arr[jj] = arr[jj] + 15000;
									temp = ((float)arr[jj])/blok->ScaleOfData; 
								}
								else if(arr[jj] > 15000)  
								{
									arr[jj] = arr[jj] - 15000;
									temp = ((float)arr[jj])/blok->ScaleOfData;
								}
								else if((arr[jj]<= -2700 && arr[jj]>=-15000)||(arr[jj]<= 15000 && arr[jj]>=2700)) //在-15000~-2700之间以及2700~15000之间的值也按照正常数据进行处理
								{
									 temp = ((float)arr[jj])/blok->ScaleOfData;
								}
							}
							else
							{							         
								if(productCode == 294)                 
								{
									temp = (float)arr[jj];   //294产品不需要除以放大倍数，因为本身值很小，所以需要放大显示
								}
								else
								{
									temp = ((float)arr[jj])/blok->ScaleOfData;
								}     
							}
							
							for (int id = 0;id < pColorTable->count();id++)
							{
								P_COLORNODE_PRODUCT color = pColorTable->at(id);
								if (temp >= color->fStart && temp<= color->fEnd)
								{
									value = qRgb(color->nRed,color->nGreen,color->nBlue);
									for(int ip = 0; ip<4; ip++)
									{
										int xx = (int)(ps[ip].x()+ra);
										int yy = 2*ra - (int)(ps[ip].y()+ra);
										image.setPixel(xx,yy, value);
										image.setPixel(xx,yy+1, value);
										image.setPixel(xx+1,yy, value);
										image.setPixel(xx+1,yy+1, value);

									}
									break;
								}
								else if ( -32766 == temp )
								{
									value = qRgb(color->nRed,color->nGreen,color->nBlue);
									for(int ip = 0; ip<4; ip++)
									{
										int xx = (int)(ps[ip].x()+ra);
										int yy = 2*ra - (int)(ps[ip].y()+ra);
										image.setPixel(xx,yy, value);
										image.setPixel(xx,yy+1, value);
										image.setPixel(xx+1,yy, value);
										image.setPixel(xx+1,yy+1, value);

									}
									break;
								}

							}
						}
					}
				}
			}
		}
		image.save(filePath);
		break;
	}
}

int SingleDataProvider::GetColorIndexKey( int productcode,double fstart )
{
	double step = 0.0;
	/*	if (251==productcode||252==productcode||258==productcode||262==productcode||263==productcode||280==productcode||282==productcode||283==productcode
	||284==productcode||288==productcode||278==productcode||291==productcode||292==productcode||294==productcode||293==productcode)
	{
	step = 5.0;
	return (int)(fstart/step);
	}else if (254==productcode||255==productcode||301==productcode)
	{
	proAttr pAttributeHaHa;
	cDrawProduct*          pDrawProduct=(cDrawProduct *)this;
	if (pDrawProduct != NULL)
	{
	pDrawProduct->ReadInfo(&pAttributeHaHa,requestlayer);
	}
	//-------------用最大不模糊速度修改  速度产品颜色表fstart fend值
	if (pAttributeHaHa.pro_code == 254 || pAttributeHaHa.pro_code == 255 || pAttributeHaHa.pro_code == 301) //301-IW-逆风区
	{
	m_nParam = pAttributeHaHa.param1;
	}
	step = 15*m_nParam/100;
	return (int)(fstart/step );
	}else if (256==productcode||257==productcode)
	{
	step = 1;
	return (int)(fstart/ step);
	}else if (267 == productcode)
	{
	step = 0.5;
	return (int)(fstart/ step);
	}else if (269 == productcode)
	{
	step = 2;
	return (int)(fstart/ step);
	}else */
	if (264 == productcode||265 == productcode)
	{
		if (fstart<0)
		{
			return 99999;
		}
		if (fstart<3.0)
		{
			step = 3;
			return 1;		
		}
		if (fstart<10.0)
		{
			step = 1;
			return (int)(fstart/ step) +10;
		}

		step = 2;
		return (int)(fstart/ step) +20;

	}else if (266 == productcode)
	{
		if (fstart<2.01)
		{
			step = 1;
			return (int)(fstart/ step);
		}
		if (fstart<5)
		{
			step = 3;
			return 11;
		}
		if (fstart<21)
		{
			step = 5;
			return(int)(fstart/ step) +20;
		}
		if (fstart<71)
		{
			step = 10;
			return (int)(fstart/ step) +30;
		}
		step = 30;
		return (int)(fstart/ step) +40;
	}else if (267 == productcode)
	{
		if (fstart<0.1)
		{
			return 99999;
		}
		if (fstart<0.5)
		{
			step = 1;
			return 1;
		}
		if (fstart<1)
		{
			step = 3;
			return 11;
		}
		if (fstart<5)
		{
			step = 0.5;
			return (int)(fstart/ step) +20;
		}
		if (fstart<10)
		{
			step = 1;
			return (int)(fstart/ step) +30;
		}
		return 41;
	}else if (270 == productcode||300 == productcode||298 == productcode||299 == productcode)
	{
		if (fstart<0.1)
		{
			return 99999;
		}
		if (fstart<0.31)
		{
			step = 0.1;
			return (int)(fstart/ step);
		}
		if (fstart<0.5)
		{
			step = 0.2;
			return 11;
		}
		if (fstart<1.0)
		{
			step =0.5;
			return 21;
		}
		if (fstart<2.0)
		{
			step = 1;
			return 31;
		}
		if (fstart<5)
		{
			step = 3;
			return 41;
		}
		if (fstart<10)
		{
			step = 5;
			return 51;
		}
		if (fstart<20)
		{
			step = 10;
			return 61;
		}
		if (fstart<50)
		{
			step = 20;
			return 71;
		}
		step = 50;
		return (int)(fstart/ step) +80;
	}else if (271 == productcode||272 == productcode||273 == productcode||274 == productcode||297 == productcode)
	{
		if(fstart<0.1)
		{
			return 99999; 
		}
		if (fstart<1.0)
		{
			//step = 0.9;
			return 1;
		}
		if (fstart<2.0)
		{
			step = 1;
			return 6;
		}
		if (fstart<5.0)
		{
			step = 2;
			return 11;
		}
		if (fstart<20.1)
		{
			step = 5;
			return (int)(fstart/ step) +20;
		}
		if (fstart<50.1)
		{
			step = 10;
			return (int)(fstart/ step) +30;
		}
		if (fstart<80.1)
		{
			step = 30;
			return (int)(fstart/ step) +40;
		}
		if (fstart<100.1)
		{
			step = 20;
			return (int)(fstart/ step) +50;
		}
		step = 50;
		return (int)(fstart/ step) +60;
		
	}
}

double SingleDataProvider::GetValue( double lon,double lat,double height,RadProjTransform *proj )
{
	CProductDrawType nType=this->GetTypeOfProduct();
	if(nType!=enProductUnkown)
	{
		int n = this->GetCountOfLayer();
		if (height<0)
		{
			gInt16 nProductCode=0;
			GetProductCode(nProductCode);
			if (nProductCode==258)
			{
				height = n/2;
			}
			else
			{
				height = 0;
			}
		}

		switch(nType)
		{
		case enProductRadial:
			return GetRadialValue(lon,lat,height,proj);
			break;
		case enProductGrid:
			return GetGridValue(lon,lat,height,proj);
			break;
		case enProductStruct:
		case enProductUnkown:
		default:
			return 0.0;
			break;
		}
	}
return 0.0;
}

double SingleDataProvider::GetRadialValue( double lon,double lat,double height,RadProjTransform *proj )
{
	PDBlock* blok = GetPDBlock();
	if(blok==NULL)
	{
		return 0.0;
	}
	int productcode = blok->ProductCode;
	double layerHight = GetRadiaElevation(int(height))*0.01;
	double Az; //雷达方位度数返回值
	double L; //雷达探测距离返回值(千米)
	double H;
	proj->gps_to_tilt(lon,lat,layerHight,Az,L,H);
	RadialHead* radialhead = GetBlockDataAsRadial_Head(int(height),0);

	int k = Az/360.0 * radialhead->NumberOfRadials;
	if (k > radialhead->NumberOfRadials)
	{
		return 0.0;
	}
	int z =( L - radialhead->DistanceOfFirstGate/1000.0)*1000/(radialhead->LengthOfGate);

	if (z>radialhead->NumberOfGates)
	{
		return 0.0;
	}

	gInt16* pArray = GetBlockDataAsRadial_Data(int(height),0,k);
	double val = pArray[z];

	return ((double)val)/blok->ScaleOfData;
}

double SingleDataProvider::GetGridValue( double lon,double lat,double height,RadProjTransform *proj )
{
	PDBlock* blok = GetPDBlock();
	if(blok==NULL)
	{
		return 0.0;
	}
	GridHead* pgridhead = GetBlockDataAsGrid_Head(int(height),0);
	double x;
	double y;
	int productcode = blok->ProductCode;
	proj->gps_to_tilt(lon,lat,height,x,y);
	if (x>pgridhead->NumberOfRows||y>pgridhead->NumberOfRows||x<0||y<0)
	{
		return 0.0;
	}
	gInt16 *arr = GetBlockDataAsGrid_Data(int(height),0,x);
	float temp = arr[int(y)];
	//逆风区产品不一样，产品号301，缩写IW
	if(productcode == 301)
	{
		if((temp= -2700) && (temp<= 2700))  //正常值
		{
			temp = ((float)temp)/blok->ScaleOfData;
		}
		else if(temp < -15000) 
		{
			temp = temp + 15000;
			temp = ((float)temp)/blok->ScaleOfData; 
		}
		else if(temp > 15000)  
		{
			temp= temp - 15000;
			temp = ((float)temp)/blok->ScaleOfData;
		}
		else if((temp<= -2700 && temp>=-15000)||(temp<= 15000 && temp>=2700)) //在-15000~-2700之间以及2700~15000之间的值也按照正常数据进行处理
		{
			temp = ((float)temp)/blok->ScaleOfData;
		}
	}
	else
	{							         
		if(productcode == 294)                 
		{
			return temp;   //294产品不需要除以放大倍数，因为本身值很小，所以需要放大显示
		}
		else
		{
			temp = ((float)temp)/blok->ScaleOfData;
		}     
	}

	return temp;
}
