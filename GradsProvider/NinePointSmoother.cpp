#include "NinePointSmoother.h"


NinePointSmoother::NinePointSmoother(void)
{
	isUse = false;
}

NinePointSmoother::~NinePointSmoother(void)
{
}

void NinePointSmoother::Execute(CElement *pElem, double **dArrayValue, double dMissingValue)
{
#if 0
	//(struct gafunc *pfc, struct gastat *pst)
	//struct gagrid *pgr;

	double *buff, *gr, *nw;
	double w,s,mid,sid,cor,missing;
	int i,j,k,rc,siz,p;
	size_t sz;
	char *gru, *nwu, *buffu;
	char *pumask;
	//int *gru,*nwu,*buffu;
	//int *pumask;

	//if (pfc->argnum!=1 && pfc->argnum!=4) {
	//	printf("Error from SMTH9:  Too many or too few args \n");
	//	printf("                   One or 4 arguments expected \n");
	//	return;
	//}
	//if (pst->type!=1) {
	//	printf("Error from SMTH9:  Grid Required\n");
	//	return;
	//}

	//pgr = pst->result.pgr;
	int isiz = pElem->SelectedLon().m_count;
	int jsiz = pElem->SelectedLat().m_count;
	//siz = pgr->isiz * pgr->jsiz;
	siz = isiz*jsiz;

	sz = siz*sizeof(double);
	buff = (double *)malloc(sz);
	if (buff==NULL) {
		printf("Error from SMTH9:  Unable to allocate memory\n");
		return;
	}
	sz = siz*sizeof(char);
	buffu = (char *)malloc(sz);
	//buffu = (int *)malloc(sz);
	if (buffu==NULL) {
		printf("Error from SMTH9:  Unable to allocate memory\n");
		return;
	}

	mid = 1.0; sid = 0.5; cor = 0.3;
	//if (pfc->argnum==4) {
	//	if (/*getdbl(pfc->argpnt[1],&mid)==NULL ||
	//		getdbl(pfc->argpnt[2],&sid)==NULL ||
	//		getdbl(pfc->argpnt[3],&cor)==NULL */
	//		false)  
	//	{
	//		printf("Error from SMTH9:  Invalid Weight Values\n");
	//		printf("  Using defaults:  1.0  0.5  0.3\n");
	//		mid = 1.0; sid = 0.5; cor = 0.3;
	//	}
	//}

	if(*dArrayValue == NULL) 
	{
		printf("data error!!!");
		return;
	}
	missing = dMissingValue;

	//gr = pgr->grid;
	//gru = pgr->umask;

	pumask = (char *)malloc(siz * sizeof(char));
	//pumask = (int *)malloc(siz * sizeof(int));

	gr = *dArrayValue;
	gru = pumask;

	for (i=0; i<siz; i++) {
		double curValue = *gr;
		//qDebug()<<curValue;
		if (abs(missing - curValue) < 0.0001)
		{
			*gru = 0;
		}
		else
		{
			*gru = 1;
		}
		//int curUMASK = *gru;
		//qDebug()<<curUMASK;
		gr++; gru++;
	}

	gr = *dArrayValue;
	gru = pumask;

	nw = buff;
	nwu = buffu;

	k = 0;
	for (j=0; j<jsiz; j++) {
		for (i=0; i<isiz; i++) {
			if (*(gru+k)!=0) {
				s = *(gr+k)*mid;
				w = mid;
				if (i!=0 && *(gru+k-1)!=0) { 
					s = s + *(gr+k-1)*sid; 
					w+=sid;
				}
				if (i!=isiz-1 && *(gru+k+1)!=0) { 
					s = s + *(gr+k+1)*sid; 
					w+=sid;
				}
				if (j!=0) {
					p = k - isiz;
					if (*(gru+p)!=0) { 
						s = s + *(gr+p)*sid; 
						w+=sid;
					}
					if (i!=0 && *(gru+p-1)!=0) { 
						s = s + *(gr+p-1)*cor; 
						w+=cor;
					}
					if (i!=isiz-1 && *(gru+p+1)!=0) { 
						s = s + *(gr+p+1)*cor; 
						w+=cor;
					}
				}
				if (j!=jsiz-1) {
					p = k + isiz;
					if (*(gru+p)!=0) { 
						s = s + *(gr+p)*sid; 
						w+=sid;
					}
					if (i!=0 && *(gru+p-1)!=0) { 
						s = s + *(gr+p-1)*cor; 
						w+=cor;
					}
					if (i!=isiz-1 && *(gru+p+1)!=0) { 
						s = s + *(gr+p+1)*cor; 
						w+=cor;
					}
				}
				*nw = s/w;
				*nwu = 1;
			} 
			else {
				*nw = missing;
				*nwu = 0;
			}
			nw++; nwu++; k++;
		}
	}

	//gr = pgr->grid;
	//gru = pgr->umask;
	gr = *dArrayValue;
	gru = pumask;
	nw = buff;
	nwu = buffu;
	for (i=0; i<siz; i++) {

		//double curDou = *nw;
		//int curMask = *nwu;
		//qDebug()<<curDou;
		//qDebug()<<curMask;

		*gr  = *nw;
		*gru = *nwu; 
		gr++; gru++; nw++; nwu++;
	}

	//free((char*)buff);
	free(buff);
	free(buffu);
#endif
	return;
}