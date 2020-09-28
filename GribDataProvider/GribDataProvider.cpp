#include <QDebug>
#include <QTextCodec>

#include "GribDataProvider.h"



GribDataProvider::GribDataProvider():m_prefileName(QString())
{
	papszExtraMDDomains = NULL;
}

GribDataProvider::~GribDataProvider()
{
		CPLFree( pszProjection );
}


/************************************************************************
����˵����װ������Դ�ļ������������ݴ洢��hashelement��Ա������
��      �ߣ���־ѧ
������ڣ�2011��6��17��
************************************************************************/
bool GribDataProvider::LoadFile(const std::string& _filePath)
{
	QString filePath = QString::fromStdString(_filePath);
	GDALDatasetH	hDataset;
	int			i, iBand;
	double		adfGeoTransform[6];
	char		**papszMetadata;
	int                 bComputeMinMax = FALSE, bSample = FALSE;
	int                 bShowGCPs = TRUE, bShowMetadata = TRUE, bShowRAT=TRUE;
	int                 bStats = FALSE, bApproxStats = TRUE, iMDD;
	int                 bShowColorTable = TRUE, bComputeChecksum = FALSE;
	int                 bReportHistograms = FALSE;
	const char          *pszFilename = NULL;

	const char  *pszProjection = NULL;
	OGRCoordinateTransformationH hTransform = NULL;

	if (filePath.isEmpty())
	{
		qDebug() << "Cannot open file!";
		return false;
	}
	// ���ϴδ��ļ���ͬ������ true
	if(filePath == m_prefileName)
	{
		return true;
	}

	GDALAllRegister();   //ע������

	//std::string stdPath = filePath.toStdString();
	//const char * path = stdPath.data();

	QTextCodec *codec = QTextCodec::codecForName("UTF-8");
	QByteArray encodedString = codec->fromUnicode(filePath);
	//QByteArray encodedString = filePath.toLocal8Bit();
	const char * path = encodedString.data();

	hDataset = GDALOpen( path, GA_ReadOnly );  //�򿪼����ļ�
	if (hDataset == NULL)
	{
		qDebug()<<"Can not Open Grib2 file!" << CPLGetLastErrorMsg();
		//CPLError()
		return false;
	}
	pDriver =  GDALGetDatasetDriver( hDataset );
	/* -------------------------------------------------------------------- */
	/*      Get projection.                                                 */
	/* -------------------------------------------------------------------- */
	if(GDALGetProjectionRef(hDataset) != NULL )
	{
		OGRSpatialReferenceH  hSRS;
		pszProjection = (char *) (GDALGetProjectionRef(hDataset));

		hSRS = OSRNewSpatialReference(NULL);
		if( OSRImportFromWkt( hSRS,(char**) &pszProjection ) == CE_None )
		{
			char	*pszPrettyWkt = NULL;
			OSRExportToPrettyWkt( hSRS, &pszPrettyWkt, FALSE );
			pszProjection = NULL;
			pszProjection = pszPrettyWkt ;
		}
		OSRDestroySpatialReference( hSRS );
	}
	double	NumOfLon,NumOfLat;   
	GDALGetGeoTransform(hDataset,adfGeoTransform);
	NumOfLon = GDALGetRasterXSize( hDataset );
	NumOfLat = GDALGetRasterYSize( hDataset );
	double first_latitude   = adfGeoTransform[3] ;         //��ʼγ��
	double last_latitude    = adfGeoTransform[3] + adfGeoTransform[5] * (NumOfLat-1);  //��ֹγ��
	if (first_latitude >= 100 )
	{
		first_latitude = first_latitude - 180;
		last_latitude = last_latitude -180;
	}

	double first_longtitude = adfGeoTransform[0];               //��ʼ����
	double last_longitude   = adfGeoTransform[0] + adfGeoTransform[1] * (NumOfLon-1);        //��ֹ����
	if (first_longtitude >= 0)
	{
		first_longtitude = first_longtitude -180;
		last_longitude = last_longitude -180;
	}
	double incremental_latitude  = adfGeoTransform[5];    //γ������
	double incremental_longitude = adfGeoTransform[1];    //��������

	int numofband =GDALGetRasterCount(hDataset);    //������
	for (int i = 1; i<=numofband; i++)
	{
		Grib2_Layer templayer;
		GDALRasterBandH  hBand;    //ÿһ������
		hBand = GDALGetRasterBand( hDataset, i );
		QString firstName =  QString(GDALGetMetadataItem(hBand,"GRIB_COMMENT",""));
		QString description = QString(GDALGetDescription(hBand));
		QString name = GetLeveName(firstName,description);
		templayer.offset =GDALGetRasterOffset(hDataset,&i);
		templayer.index = i ;	
		templayer.var_name = name;
		templayer.first_latitude = first_latitude;
		templayer.first_longtitude = first_longtitude;
		templayer.last_latitude = last_latitude;
		templayer.last_longitude = last_longitude;
		templayer.incremental_latitude = incremental_latitude;
		templayer.incremental_longitude = incremental_longitude;
		templayer.grib_number_latitude  = NumOfLat;
		templayer.grib_number_longitude = NumOfLon;
		//init element
		CElement *pElement = m_elements.value(name);
		if(pElement)
		{
			QString fileName = filePath;
			fileName = fileName.replace("\\", "/");

			if (pElement->GetFileName() == fileName)
			{
				InitElement(pElement,hBand,&templayer);
				m_indexHash.insert(templayer.index,templayer);
				//if (hBand != NULL)
				//{
				//	CPLFree(hBand);
				//}
				continue;
			}
		}
		pElement = new CElement();
		if(!pElement)
		{
			return false;
		}
		if(InitOtherElement(pElement, hBand,&templayer) /*&& pElement->IsInited()*/)
		{
			pElement->SetFileType("GRIB");
			pElement->SetFileName(filePath);
			m_elements.insert(pElement->name(), pElement);
		}
		else
		{
			// delete 
			delete pElement;
			pElement = 0;
		}
		m_indexHash.insert(templayer.index,templayer);
		//if (hBand != NULL)
		//{
		//	CPLFree(hBand);
		//}

	}
	GDALClose( hDataset );
	CPLCleanupTLS();
	QHash<QString, int>::iterator iter;
	for (HashElement::iterator iter = m_elements.begin(); iter != m_elements.end(); ++iter)
	{
		// ʱ��
		iter.value()->ActualPeriod().m_begin = iter.value()->ActualPeriod().m_hashDate.begin().value();
		iter.value()->ActualPeriod().m_count = iter.value()->ActualPeriod().m_hashDate.size();
		CTimeScale time(1,3);
		iter.value()->ActualPeriod().SetTimeScale(time);
		iter.value()->SelectedPeriod(iter.value()->ActualPeriod());
		//���
		iter.value()->Level().m_selected = iter.value()->Level().m_valus;
		iter.value()->Level().m_count = iter.value()->Level().m_valus.size();
		//��γ�������
		iter.value()->SelectedLat(iter.value()->ActualLat());
		iter.value()->SelectedLon(iter.value()->ActualLon());
		iter.value()->NeedDisplay(true);
		//
		iter.value()->MissingValue(-99999);


	}

	//��Ҫ�ش洢���ݣ��洢��elements��
	SetDeclaration(filePath);//�洢������Ϣ�Լ�Ҫ����Ϣ

	return true;
}
/************************************************************************
����˵�����洢���Ҫ����������
��      �ߣ���־ѧ
������ڣ�2011��6��5��
************************************************************************/
bool GribDataProvider::StoreDetailValues( CElement* pElement )
{
	QString varName = pElement->name();
	QHash<int, float > level = pElement->Level().m_selected;
	QHash<long, QDateTime> dt = pElement->SelectedPeriod().m_hashDate;
	QString fp =pElement->GetFileName();
	int err = 0;

	//std::string stdPath = fp.toStdString();
	//const char * filePath = stdPath.data();
	QTextCodec *codec = QTextCodec::codecForName("UTF-8");
	QByteArray encodedString = codec->fromUnicode(fp);
	const char * filePath = encodedString.data();

	GDALDatasetH	hDataset;
	GDALAllRegister();   //ע������
	hDataset = GDALOpen( filePath, GA_ReadOnly );  //�򿪼����ļ�
	if (hDataset == NULL)
	{
		qDebug()<<"Can not Open Grib2 file!";
		return false;
	}
	long lat_count = pElement->ActualLat().m_count;
	long lon_count = pElement->ActualLon().m_count;
	long lat_select_count = pElement->SelectedLat().m_count;
	long lon_select_count = pElement->SelectedLon().m_count;
	QList<long> list;
	list.append(dt.size());
	list.append(level.size());
	list.append(lon_select_count);
	list.append(lat_select_count);

	CData data(list);
	pElement->Data(data);

	float *pArray = pElement->GetDataArray(0);
	double * pValues = (double *) CPLMalloc(lat_count*lon_count*sizeof(double)); 

	Grib2_index::iterator it;
	int datalevelNum = 0;
	int  cccc = m_indexHash.count();
	for (it = m_indexHash.begin(); it != m_indexHash.end(); ++it)
	{

		for (int i = 0 ;i<lat_count*lon_count;i++)
		{
			pValues[i] = -99999;
		}

		if (it.value().var_name != pElement->name())
		{
			continue;
		}
		if (-1 == pElement->SelectedPeriod().m_hashDate.key((it.value().layer_datetime),-1) )
		{
			continue;
		}
		if (-1 == pElement->Level().m_selected.key((it.value().level),-1))
		{
			continue;;
		}
		int index = it.key();
		GDALRasterBandH  hBand;    //ÿһ������
		hBand = GDALGetRasterBand( hDataset, index );

		if (pElement->ActualLon().m_count == pElement->SelectedLon().m_count && pElement->ActualLat().m_count ==pElement->SelectedLat().m_count)
		{
			pElement->SelectedLon().m_count = it.value().grib_number_longitude; //����������
			pElement->SelectedLat().m_count = it.value().grib_number_latitude;  //γ��������

		}
		float selectLat_begin =  pElement->SelectedLat().m_begin;
		float actualLat_begin =  pElement->ActualLat().m_begin;
		float selectLat_end =    pElement->SelectedLat().m_end;
		float selectLon_begin =  pElement->SelectedLon().m_begin;
		float actualLon_begin =  pElement->ActualLon().m_begin;
		float selectLon_end =    pElement->SelectedLon().m_end;



		if (selectLat_begin == actualLat_begin &&
			selectLon_begin == actualLon_begin &&
			selectLat_end == pElement->ActualLat().m_end &&
			selectLon_end == pElement->ActualLon().m_end )
		{

			GDALRasterIO (hBand,GF_Read,0,0,GDALGetRasterBandXSize(hBand),GDALGetRasterBandYSize(hBand),pValues,lon_count,
				lat_count,((GDALRasterBand*)hBand)->GetRasterDataType(),0,0);

		}else
		{
			int xoff = abs(selectLon_begin -actualLon_begin)/abs(pElement->ActualLon().m_delt) + 1;
			int yoff = abs(selectLat_begin -actualLat_begin)/abs(pElement->ActualLat().m_delt) + 1;
			int xsize = lon_select_count;
			int ysize = lat_select_count;
			GDALRasterIO (hBand,GF_Read,xoff,yoff,xsize,ysize,pValues,lon_select_count,
				lat_select_count,((GDALRasterBand*)hBand)->GetRasterDataType(),0,0);
		}
		for (int i = 0; i<lat_select_count; i++)
		{
			for (int j = 0; j<lon_select_count;j++)
			{
				if (j<lon_select_count/2)
					pArray[i*lon_select_count + j] = pValues[i*lon_select_count + j + lon_select_count/2];
				else
					pArray[i*lon_select_count + j] = pValues[i*lon_select_count + j - lon_select_count/2];
			}
		}
		//FILE *file;
		//file = fopen("e:/gribidata.txt","w");
		//for (int p = 0 ;p<lat_count*lon_count;p++)
		//{

		//	fprintf(file,"%f\n",pArray[p]);
		//}
		//fclose(file);
		pArray += lat_select_count*lon_select_count;

	}
	VSIFree(pValues) ;
	GDALClose( hDataset );
	CPLCleanupTLS();
	return true;

}
/************************************************************************
����˵����д�ļ�
��      �ߣ���־ѧ
������ڣ�2011��6��5��
************************************************************************/	
int GribDataProvider::Write( const QString& fileTypeName )
{
	return 1;

}
/************************************************************************
����˵������ȡҪ��
��      �ߣ���־ѧ
������ڣ�2011��6��5��
************************************************************************/
CElement* GribDataProvider::GetElement( const QString& elementName )
{
	return m_elements.value(elementName);
}
/************************************************************************
����˵����QString to char*
��      �ߣ���־ѧ
������ڣ�2011��6��5��
************************************************************************/
char * GribDataProvider::tochar( QString value, int &len )
{
	char *mycp = (char *)malloc(value.length()+1);
	QChar *data = value.data();
	int strlen = 0;
	while(!data->isNull())
	{
		mycp[strlen] = data->toLatin1();
		++data;
		++strlen;
	}
	mycp[strlen] = '\0';
	len = strlen;
	return mycp;

}
/************************************************************************
����˵������ȡ����Ҫ������
��      �ߣ���־ѧ
������ڣ�2011��6��5��
************************************************************************/
IDataProvider::HashElement GribDataProvider::GetAllElements(const QString&  filePath)const 
{

	QHash<QString, CElement*> ret_elements;
	CElement *pElement = 0;
	QHash<QString, CElement*> :: const_iterator it;

	//for(it = m_elements.constBegin(); it != m_elements.constEnd(); ++it)
	//{
	//	pElement = it.value();
	//	qDebug()<<pElement->GetFileName();
	//	qDebug()<<pElement->name();
	//}

	for(it = m_elements.constBegin(); it != m_elements.constEnd(); ++it)
	{
		pElement = it.value();
		if(pElement && pElement->GetFileName() == filePath)
		{
			ret_elements.insert(pElement->name(), pElement);
		}
	}

	return ret_elements;
}

QStringList GribDataProvider::keys()
{
	QStringList list;
	return list;
}
/************************************************************************
����˵�����³�ʼ��һ��Ҫ��
��      �ߣ���־ѧ
������ڣ�2011��6��5��
************************************************************************/
bool GribDataProvider::InitOtherElement( CElement* pElement, GDALRasterBandH h, Grib2_Layer* layer)
{
	QDateTime datetime;
	double level;                //���
	int forecast_time;          //Ԥ��ʱ��
	QString templevel = QString(GDALGetMetadataItem(h,"GRIB_SHORT_NAME",""));       //���
	if (layer->var_name.contains("DBLL"))
	{
		templevel = templevel.section("-",1,1);
	}else
		templevel = templevel.left(templevel.indexOf("-"));
	level = (templevel.toDouble());
	if (layer->var_name.contains("ISBL"))
	{
		level = level/100;
	}



	QString tempsecond = QString(GDALGetMetadataItem(h,"GRIB_VALID_TIME",""));
	tempsecond = tempsecond.left(tempsecond.indexOf("s"));
	datetime = QDateTime::fromTime_t(tempsecond.toDouble()-8*60*60); //ʱ��

	pElement->ActualLat().m_begin = layer->first_latitude;                 //��ʼγ��
	pElement->ActualLon().m_begin = layer->first_longtitude;               //��ʼ����
	pElement->ActualLat().m_end = layer->last_latitude;                    //��ֹγ��        
	pElement->ActualLon().m_end = layer->last_longitude;                   //��ֹ����   

	if (pElement->name().isEmpty())
	{	
		//��ʼ��������
		pElement->name(layer->var_name);
	}

	//��ʼ��ʱ��
	layer->layer_datetime = datetime;
	CPeriod timeRange;
	if (-1 == timeRange.m_hashDate.key(datetime,-1) )
	{
		timeRange.m_hashDate.insert((timeRange.m_hashDate.size()),datetime);
	}
	pElement->ActualPeriod(timeRange);

	//��ʼ�����
	CLevel lev;
	layer->level =level;
	if (-1 == lev.m_valus.key(level,-1))
	{
		lev.m_valus.insert(lev.m_valus.size(),level);
	}
	pElement->Level(lev);
	//��ʼ��������� ����
	pElement->ActualLat().m_delt = layer->incremental_latitude;                     //γ������ ���ӱ����� ����Ϊ����
	pElement->ActualLon().m_delt = layer->incremental_longitude;                    //�������� �������� ����Ϊ����
	pElement->ActualLon().m_count = layer->grib_number_longitude;
	pElement->ActualLat().m_count = layer->grib_number_latitude;
	return true;

}
/************************************************************************
����˵����������Ҫ�������������
��      �ߣ���־ѧ
������ڣ�2011��6��5��
************************************************************************/
bool GribDataProvider::InitElement( CElement* pElement, GDALRasterBandH h,Grib2_Layer* layer )
{
	QDateTime datetime;
	double level;                //���
	int forecast_time;          //Ԥ��ʱ��
	QString templevel = QString(GDALGetMetadataItem(h,"GRIB_SHORT_NAME",""));       //���
	if (layer->var_name.contains("DBLL"))
	{
		templevel = templevel.section("-",1,1);
	}else
		templevel = templevel.left(templevel.indexOf("-"));

	level = (templevel.toDouble());
	if (layer->var_name.contains("ISBL"))
	{
		level = level/100;
	}
	QString tempsecond = QString(GDALGetMetadataItem(h,"GRIB_VALID_TIME",""));
	tempsecond = tempsecond.left(tempsecond.indexOf("s"));
	datetime = QDateTime::fromTime_t(tempsecond.toDouble()-8*60*60); //ʱ��
	pElement->ActualLat().m_begin = layer->first_latitude;                 //��ʼγ��
	pElement->ActualLon().m_begin = layer->first_longtitude;               //��ʼ����
	pElement->ActualLat().m_end = layer->last_latitude;                    //��ֹγ��        
	pElement->ActualLon().m_end = layer->last_longitude;                   //��ֹ����     
	pElement->ActualLat().m_delt = layer->incremental_latitude;                     //γ������ ���ӱ����� ����Ϊ����
	pElement->ActualLon().m_delt = layer->incremental_longitude;                    //�������� �������� ����Ϊ����

	//��ʼ��ʱ��
	layer->layer_datetime = datetime;
	if (-1 == pElement->ActualPeriod().m_hashDate.key(datetime,-1) )
	{
		pElement->ActualPeriod().m_hashDate.insert((pElement->ActualPeriod().m_hashDate.size()),datetime);
	}

	//��ʼ�����
	layer->level = level;
	if (-1 == pElement->Level().m_valus.key(level,-1))
	{
		pElement->Level().m_valus.insert(pElement->Level().m_valus.size(),level);
	}
	pElement->ActualLon().m_count = layer->grib_number_longitude;
	pElement->ActualLat().m_count = layer->grib_number_latitude;
	return true;
}

bool GribDataProvider::CloseFileAndRemoveElements( const QString& fileName )
{
	CElement *pElement = 0;
	QHash<QString, CElement*>::Iterator it = m_elements.begin();
	while( it != m_elements.end())
	{
		pElement = it.value();
		if(pElement && pElement->GetFileName() == fileName)
		{
			delete  pElement;
			pElement = 0;
			it = m_elements.erase(it);
		}
		else
		{
			++it;
		}
	}
	return true;

}

QString GribDataProvider::GetLeveName( QString &firstName,QString &description )
{
	QString FullName;
	firstName = firstName.left(firstName.indexOf(tr("[")));
	if (description.contains(tr("ISBL")))
	{   		
		FullName = firstName + tr("_ISBL");

	}else if (description.contains(tr("HTGL")))
	{
		if (description.contains(tr("10[m]")))
		{
			FullName = firstName + tr("_HTGL_10");
		} else if (description.contains(tr("2[m]")))
		{
			FullName = firstName + tr("_HTGL_2");
		}else 
		{
			FullName = firstName + tr("_HTGL");
		}

	} 
	else if (description.contains(tr("SFC")))
	{
		if (description.contains(tr("0[-]")))
		{
			FullName = firstName + tr("_SFC_0");
		} else 
		{
			FullName = firstName + tr("_SFC");
		}

	}else if (description.contains(tr("MSL")))
	{
		if (description.contains(tr("0[-]")))
		{
			FullName = firstName + tr("_MSL_0");
		} else 
		{
			FullName = firstName + tr("_MSL");
		}
	} 
	else if (description.contains(tr("DBLL")))
	{
		FullName = firstName + tr("_DBLL");

	}else if (description.contains(tr("CBL")))
	{
		FullName = firstName + tr("_CBL");
	}else

		FullName = firstName;

	return FullName;

}

void GribDataProvider::SetDeclaration( const QString& filePath )
{
	IDataProvider::HashElement curHashEle = GetAllElements(filePath);
	CElement *pcurEleTemp = 0;
	CElement *pEleTemp = 0;
	QStringList displayEleList;
	QHash<QString, CElement*> :: const_iterator it = curHashEle.constBegin();
	for(; it != curHashEle.constEnd(); ++it)
	{
		pcurEleTemp = it.value();
		if(pcurEleTemp && pcurEleTemp->NeedDisplay() == true)
		{
			displayEleList.append(pcurEleTemp->name());
			pEleTemp = it.value();
		}
	}

	QString strdata;
	//�ļ�������Ϣ��ʽ��
	CElement *pEle = new CElement();

	pEle->name(tr("�ļ�����"));
	pEle->SetMicapsDeclaration("�������ͣ�GRIB");

	//CElement *pEleTemp = m_elements.value(QString("lon"));
	if (pEleTemp->ActualLon().m_begin && pEleTemp->ActualLon().m_end)
	{
		strdata = tr("���ݾ��ȷ�Χ��");
		strdata = strdata + QString::number(pEleTemp->ActualLon().m_begin) + QString(" ~ ") + QString::number(pEleTemp->ActualLon().m_end);
		pEle->SetMicapsDeclaration(strdata);
	}

	//pEleTemp = m_elements.value(QString("lat"));
	if (pEleTemp->ActualLat().m_begin && pEleTemp->ActualLat().m_end)
	{
		strdata = tr("����γ�ȷ�Χ��");
		strdata = strdata + QString::number(pEleTemp->ActualLat().m_begin) + QString(" ~ ") + QString::number(pEleTemp->ActualLat().m_end);
		pEle->SetMicapsDeclaration(strdata);
	}

	//pEleTemp = m_elements.value(QString("level"));
	if (!pEleTemp->Level().m_valus.isEmpty())
	{
		strdata = tr("���ݲ�η�Χ��");
		QHash<int, float >::iterator iterBegin,iterEnd;
		if (pEleTemp->Level().m_valus.count() == 1)
		{
			iterBegin = pEleTemp->Level().m_valus.begin();
			strdata = strdata + QString::number(iterBegin.value());
		}
		else if (pEleTemp->Level().m_valus.count() > 1)
		{
			iterBegin = pEleTemp->Level().m_valus.begin();
			iterEnd = pEleTemp->Level().m_valus.end();
			iterEnd--;
			strdata = strdata + QString::number(iterBegin.value()) + QString(" ~ ") + QString::number(iterEnd.value());
		}
		pEle->SetMicapsDeclaration(strdata);
	}

	//pEleTemp = m_elements.value(QString("time"));
	if (pEleTemp->ActualPeriod().m_count >= 1)
	{
		strdata = tr("����ʱ�䷶Χ��");
		if (pEleTemp->ActualPeriod().m_count == 1)
		{
			strdata = strdata + pEleTemp->ActualPeriod().m_begin.date().toString();
		}
		else
		{
			strdata = strdata + pEleTemp->ActualPeriod().m_begin.date().toString() + QString(" ~ ") + pEleTemp->ActualPeriod().m_end.date().toString();
		}
		pEle->SetMicapsDeclaration(strdata);
	}

	strdata = tr("���ݰ���Ԫ�أ�");
	pEle->SetMicapsDeclaration(strdata);
	for (int i = 0;i < displayEleList.count();i++)
	{
		strdata = QString(".............") + displayEleList.at(i);
		pEle->SetMicapsDeclaration(strdata);
	}

	pEle->NeedDisplay(false);
	pEle->SetFileType("GRIB");
	pEle->SetFileName(filePath);
	m_elements.insert(pEle->name(), pEle);
}

//bool GribDataProvider::DataOrdering( long* scan_mode,double *datain,float *dataout,long num_lat,long num_lon,int lon_begin_index,int lat_begin_index )
//{
//	if (0 == scan_mode[4])
//
//	//�ж����У�����i�����Ȼ����У�γ��j������
//
//	if (0 == scan_mode[3]) //����ɨ��
//	{
//		if (0 == scan_mode[4]) //������ݲ�����������
//		{
//			for (int i = 0; i<num_lat; i++)
//			{
//				for (int j = 0; j<num_lon;j++)
//				{
//					if (0 == scan_mode[1] && 0 == scan_mode[0]) //������ �ɱ�����
//					{
//						dataout[i*num_lon + j] = datain[(i+lat_begin_index)*num_lon + j + lon_begin_index];
//					}	
//					else if (1 == scan_mode[1] && 0 == scan_mode[0]) //�ɶ����� �ɱ�����
//					{
//						dataout[i*num_lon + j] = datain[(i+lat_begin_index)*num_lon + num_lon - j - lon_begin_index];
//					}	
//					else if (1 == scan_mode[1] && 1 == scan_mode[0]) //�ɶ����� ������
//					{
//						dataout[i*num_lon + j] = datain[(num_lat - i-lat_begin_index)*num_lon + num_lon - j - lon_begin_index];
//					}	
//					else if (0 == scan_mode[1] && 1 == scan_mode[0]) //������ ������
//					{
//						dataout[i*num_lon + j] = datain[(num_lat - i -lat_begin_index)*num_lon + j + lon_begin_index];
//					}
//				}		
//			}
//
//		
//		}else
//		{
//
//		}
//
//	} 
//	else//����ɨ��
//	{
//		if (0 == scan_mode[4])//������ݲ�����������
//		{
//			for (int i = 0; i<num_lon; i++)
//			{
//				for (int j = 0; j<num_lat;j++)
//				{
//					if (0 == scan_mode[1] && 0 == scan_mode[0]) //������ �ɱ�����
//					{
//						dataout[i*num_lat + j] = datain[(i + lon_begin_index)*num_lat + j + lat_begin_index];
//					}	
//					else if (1 == scan_mode[1] && 0 == scan_mode[0]) //�ɶ����� �ɱ�����
//					{
//						dataout[i*num_lat + j] = datain[(i + lon_begin_index)*num_lat + num_lat - j - lat_begin_index];
//					}	
//					else if (1 == scan_mode[1] && 1 == scan_mode[0]) //�ɶ����� ������
//					{
//						dataout[i*num_lat + j] = datain[(num_lon - i - lon_begin_index)*num_lat + num_lat - j - lat_begin_index];
//					}	
//					else if (0 == scan_mode[1] && 1 == scan_mode[0]) //������ ������
//					{
//						dataout[i*num_lat + j] = datain[(num_lon - i - lon_begin_index)*num_lat + j - lat_begin_index];
//					}
//				}		
//			}
//		}else
//		{
//
//		}
//	}
//	return true;
//}

Q_EXPORT_PLUGIN2(IDataProvider,  GribDataProvider) 