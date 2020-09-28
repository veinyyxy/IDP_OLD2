#include "CressmanInterpolation.h"


CressmanInterpolation::CressmanInterpolation(void)
{
}

CressmanInterpolation::~CressmanInterpolation(void)
{
}

void CressmanInterpolation::Execute(/*CressmanParam *structParam,*/CData *pDataArray, double **dArrayValue)
{
#if 0
	float *rads;   //[5] = {10.0, 7.0, 4.0, 2.0, 1.0};
	double *gr, *nw, *newbuf, *p1, *p2, *p3, *p4,*lev,*tim,*stnlon,*stnlat;
	float *stnval;
	double lon,lat,lnmin,lnmax,lnscl,ltmin,ltmax,ltscl;
	double sum;
	double e1,e2,e,wsum,x,y,xmin,xmax,ymin,ymax,d,d2,rad,radpower2,w;
	int *flgbuf, *ii,*work,*pumask;
	int i,j,p,siz,icnt,irad,radflg,isiz,jsiz,inum,stnnum;
	double fgsum=-1e20,fundef;
	int sumu,*gru,*nwu,*newbufu;
	size_t sz;

	/* Check for user provided radii */
	irad = 5;
	radflg = 0;

	/* Set up scaling for converting lon-lat to grid units */

	//isiz = structParam->isiz;
	//jsiz = structParam->jsiz;
	//lnmin = structParam->lonmin;
	//lnmax = structParam->lonmax;
	//ltmin  = structParam->latmin;
	//ltmax = structParam->latmax;
	//irad = structParam->nrad;
	//rads = structParam->rads;
	//fundef = structParam->fUndef;

	isiz = m_cressmanParam.isiz;
	jsiz = m_cressmanParam.jsiz;
	lnmin = m_cressmanParam.lonmin;
	lnmax = m_cressmanParam.lonmax;
	ltmin  = m_cressmanParam.latmin;
	ltmax = m_cressmanParam.latmax;
	irad = m_cressmanParam.nrad;
	rads = m_cressmanParam.rads;
	fundef = m_cressmanParam.fUndef;
	stnnum = pDataArray[0].DataBlockDimensionSize(0,0);
	siz = isiz * jsiz;


	//isiz = 141;
	//jsiz = 81;
	//lnmin = 70.0;
	//lnmax = 140.0;
	//ltmin =15.0;
	//ltmax = 55.0;
	//stnnum = 1813;
	//fundef = -999.9;

	lnscl = (lnmax-lnmin)/((double)isiz-1); //步长
	ltscl = (ltmax-ltmin)/((double)jsiz-1); //步长

	work = (int*)malloc(sizeof(int)*stnnum);
	stnlon= (double*)malloc(sizeof(double)*stnnum);
	stnlat = (double*)malloc(sizeof(double)*stnnum);
	lev      = (double*)malloc(sizeof(double)*stnnum);
	tim     =  (double*)malloc(sizeof(double)*stnnum);
	pumask = (int *)malloc(siz * sizeof(int));

	if(*dArrayValue != NULL)
	{
		free(*dArrayValue);
		*dArrayValue = NULL;
	}

	*dArrayValue = (double *)malloc(siz * sizeof(double));
	stnval = pDataArray[2].GetDataArray(0);
	for(inum = 0; inum < stnnum; inum++)
	{
		stnlon[inum] = pDataArray[0].GetDataArray(0)[inum];
		stnlat[inum] = pDataArray[1].GetDataArray(0)[inum];
	}

	//FILE *fp;
	//fp = fopen("E:\\Liu\\Temp Workspace\\Cressman-test\\lonlatdata.txt","r");
	//float f1 = 0,f2 = 0,f3 = 0;
	//i = 0;
	//while(!feof(fp))
	//{
	//	fscanf(fp,"%f ,%f ,%f\n",&f1,&f2,&f3);
	//	stnval[i] = f3;
	//	stnlon[i] = f1;
	//	stnlat[i] = f2;
	//	i++;
	//}
	//fclose(fp);

	/* Now loop through each stn report and convert stn lat/lon to grid coordinates */
	sum = 0.0;
	icnt = 0;
	for(inum = 0; inum < stnnum; inum++)
	{
		lon =stnlon[inum];
		lat = stnlat[inum];
		if (lon<lnmin) lon+=360.0;
		else if (lon>lnmax) lon-=360.0;
		stnlon[inum] = (lon-lnmin)/lnscl;
		stnlat[inum] = (lat-ltmin)/ltscl;
		i = (int)stnlon[inum]; //计算格网索引
		j = (int)stnlat[inum];
		if (i<0 || i>(isiz-1) || j<0 || j>(jsiz-1) ||fabs(stnval[inum] - fundef) < 1e-6)
		{
			work[inum] = -999;
		}
		else
		{
			work[inum] = j*isiz + i; //位移
			sum = sum + stnval[inum];
			icnt++;
			i = (int)stnlon[inum];
			j = (int)stnlat[inum];
			//通过格网索引以及下面的差值用于得到实际的站点经纬度
			lev[inum] =stnlon[inum] - (double)i; //经度与其整数的差值
			tim[inum] = stnlat[inum] - (double)j; //纬度与其整数的差值
		}
	}
	if (icnt<2) {
		printf("Warning from OACRES:  Less than two stations\n");
		printf("    Grid will be all missing values\n");
		sumu = 0;
	}
	else {
		sum = sum/((double)icnt); //计算站点的平均值
		sumu = 1;
	}

	/* Need some buffer space */
	sz = sizeof(double)*siz;
	newbuf = (double *)malloc(sz);
	if (newbuf==NULL) {
		goto err;
	}
	sz = sizeof(int)*siz;
	newbufu = (int *)malloc(sz);
	if (newbufu==NULL) {
		free(newbuf);
		goto err;
	}
	sz = sizeof(int)*siz;
	flgbuf = (int *)malloc(sz);
	if (flgbuf==NULL) {
		free(newbuf);
		free(newbufu);
		goto err;
	}

	/* Initial grid values are average of station reports */
	if (fabs(fgsum) < 1e20 ) {
		sum=fgsum;
		sumu=1;
	}
	gr = *dArrayValue;
	gru = pumask;
	nw = newbuf;
	nwu = newbufu;
	ii = flgbuf;
	for (i=0; i<siz; i++) {
		if (sumu==1) {
			*gr = sum; *gru = 1;
			*nw = sum; *nwu = 1;
		}
		else {
			*gru = 0;
			*nwu = 0;
		}
		*ii = 0;
		ii++;
		gr++; gru++;
		nw++; nwu++;
	}
	if (sumu==0) goto retrn;

	/* Perform the objective analysis */
	for (p=0; p<irad; p++)
	{
		rad = rads[p];
		radpower2 = rad*rad;
		gr = *dArrayValue;
		nw = newbuf;
		nwu = newbufu;
		ii = flgbuf;
		for (j=0; j<jsiz; j++) {
			y = (double)j;
			ymin = y - rad;
			ymax = y + rad;
			for (i=0; i<isiz; i++) {
				x = (double)i;
				xmin = x - rad;
				xmax = x + rad;
				sum = 0.0; wsum = 0.0;

				for (inum = 0; inum < stnnum; inum++)
				{
					if (work[inum] ==-999 || stnlon[inum] < xmin ||stnlon[inum] > xmax ||stnlat[inum] < ymin || stnlat[inum] > ymax || (d = hypot(x-stnlon[inum],y-stnlat[inum])) > rad )
					{
					}
					else
					{
						d2 = d*d; //两点距离的平方
// 						p1 = *dArrayValue + work[inum]; //对指针进行位移运算
// 						p2 = p1+1;
// 						p4 = p1 + isiz;
// 						p3 = p4 + 1;
// 						e1 = *p1 + ( (*p2 - *p1)* lev[inum] );
// 						e2 = *p4 + ( (*p3 - *p4)* lev[inum] );
// 						e = e1 + ( (e2 - e1)* tim[inum] ); 
// 						e = stnval[inum] - e; //得到站点值与平均值的差值
						e = pDataArray[2].GetDataArray(0)[inum];
						w = (radpower2-d2)/(radpower2+d2); //计算权重						
						sum += e*w;
						wsum += w;
						/********************
						       p1,p2
						       p4,p3
						**********************/
					}
				}
				if (wsum>1e-6) {
				//	*nw = *gr + sum/wsum;
					*nw = sum/wsum;
					*nwu = 1;
				}
				else if (p==2) *ii = 1;
				nw++; nwu++; gr++; ii++;
			}
		}
		nw  = newbuf;
		nwu = newbufu;
		gr = *dArrayValue;
		gru = pumask;
		for (i=0; i<siz; i++) {
			if (*nwu==1) {
				*gr = *nw;
				*gru = 1;
			}
			else {
				*gru = 0;
			}
			gr++; gru++; nw++; nwu++;
		}
	}
	ii = flgbuf;
	gru = pumask;
	for (i=0; i<siz; i++) {
		if (*ii) *gru = 0;
		gru++; ii++;
	}
	gr = *dArrayValue;
	for (i=0; i<siz; i++)
	{
		if(pumask[i] == 0)
			gr[i] = fundef;
	}

	//gr  = pgr->grid;
	//gru = pgr->umask;

retrn:

	free(stnlon);
	free(stnlat);
	free(lev);
	free(tim);
	free(work);
	free(pumask);
	free(newbuf);
	free(newbufu);
	free(flgbuf);
	return;

err:
	free(stnlon);
	free(stnlat);
	free(lev);
	free(tim);
	free(work);
	free(pumask);
	free(newbuf);
	free(newbufu);
	free(flgbuf);
	printf("Error in OACRES:  Unable to allocate memory\n");
	return;
#endif
}

bool CressmanInterpolation::SetCressmanParam( QString cellSizeList,QStringList noValueList,QStringList radList,QStringList rangeList )
{
#if 1
	if (!noValueList.empty())
	{
		m_cressmanParam.fUndef = noValueList[0].toFloat();
	}

	m_cressmanParam.lonmin = rangeList[0].toFloat();
	m_cressmanParam.lonmax = rangeList[1].toFloat();
	m_cressmanParam.latmin = rangeList[2].toFloat();
	m_cressmanParam.latmax = rangeList[3].toFloat();

	m_cressmanParam.isiz = (int)((m_cressmanParam.lonmax - m_cressmanParam.lonmin)/cellSizeList.toFloat());
	m_cressmanParam.jsiz = (int)((m_cressmanParam.latmax - m_cressmanParam.latmin)/cellSizeList.toFloat());

	//m_cressmanParam.isiz = cellSizeList[0].toInt();
	//m_cressmanParam.jsiz = cellSizeList[1].toInt();

	m_cressmanParam.nrad = 5;
	int i;
	for (i = 0;i < 5;i++)
	{
		m_cressmanParam.rads[i] = radList[i].toInt();
	}
#endif
	return true;
}

#if 0
void CressmanInterpolation::SetElementInfo( CElement *pElem/*, CressmanParam *structParam*/ )
{
#if 1
	//if(pElem == NULL|| structParam == NULL)
	//{
	//	printf("POINTER IS NULL. ERROR.\n");
	//	return;
	//}

	//pElem->ResetSelectedLat(structParam->latmin,structParam->latmax);
	//pElem->ResetSelectedLon(structParam->lonmin,structParam->lonmax);
	//pElem->SelectedLon().m_count = structParam->isiz;
	//pElem->SelectedLat().m_count = structParam->jsiz;
	//pElem->SelectedLon().m_delt = (structParam->lonmax - structParam->lonmin) / (structParam->isiz - 1);
	//pElem->SelectedLat().m_delt = (structParam->latmax - structParam->latmin) / (structParam->jsiz - 1);

	//QDateTime date = pElem->ActualPeriod().m_hashDate.value(0);
	//pElem->ResetSelectedPeriod(date,date);
	//CTimeScale timescale(1,3);
	//pElem->SelectedPeriod().SetTimeScale(timescale);     // 设置时间尺度 默认1dy
	////pElem->MissingValue(structParam->fUndef);

	if(pElem == NULL )
	{
		printf("POINTER IS NULL. ERROR.\n");
		return;
	}

	pElem->ResetSelectedLat(m_cressmanParam.latmin,m_cressmanParam.latmax);
	pElem->ResetSelectedLon(m_cressmanParam.lonmin,m_cressmanParam.lonmax);
	pElem->SelectedLon().m_count = m_cressmanParam.isiz;
	pElem->SelectedLat().m_count = m_cressmanParam.jsiz;
	pElem->SelectedLon().m_delt = (m_cressmanParam.lonmax - m_cressmanParam.lonmin) / (m_cressmanParam.isiz - 1);
	pElem->SelectedLat().m_delt = (m_cressmanParam.latmax - m_cressmanParam.latmin) / (m_cressmanParam.jsiz - 1);

	QDateTime date = pElem->ActualPeriod().m_hashDate.value(0);
	pElem->ResetSelectedPeriod(date,date);
	CTimeScale timescale(1,3);
	pElem->SelectedPeriod().SetTimeScale(timescale);     // 设置时间尺度 默认1dy
	//pElem->MissingValue(structParam->fUndef);
#endif
}
#endif