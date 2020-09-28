#include "AlertList.h"

AlertList::AlertList(void)
{
}

AlertList::~AlertList(void)
{
}
AlertList::AlertList(AlertList & alertlist)
{
	alertmsghead = alertlist.alertmsghead;
	listofeveryarea = alertlist.listofeveryarea;
}
AlertMessageHead *AlertList::GetAertMessageHead()
{
	return &(this->alertmsghead);		
}
void AlertList::SetMsgHead( AlertMessageHead alertmessghead )
{
	alertmsghead = alertmessghead;

}
void AlertList::AddEverycondTail(list<EveryCondAlert>  &listofeverycond ,gInt16 AlertLevelCode,gInt16 TresholdValue[2], EveryLayerAlert *layeralert)
{
	EveryCondAlert everycondalert;
	everycondalert.AlertLevelCode = AlertLevelCode;
	everycondalert.TresholdValue[0] = TresholdValue[0];
	everycondalert.TresholdValue[1] = TresholdValue[1];
	for(int i = 0 ; i < alertmsghead.NumberOfLayer ; i ++ )
		everycondalert.conditionlist.push_back(layeralert[i]);
	listofeverycond.push_back(everycondalert);
}
void AlertList::AddEveryareaTail(AlertArea *alertarea,gInt32 NumberOfAlertCondition,list<EveryCondAlert> *conditionlist, gInt16 MaxAlertLevel)
{	
	EveryAlarmArea alarea;
	alarea.alertarea.first.x = alertarea->first.x;
	alarea.alertarea.first.y = alertarea->first.y;
	alarea.alertarea.second.x = alertarea->second.x;
	alarea.alertarea.second.y = alertarea->second.y;
	alarea.alertarea.thirdth.x = alertarea->thirdth.x;
	alarea.alertarea.thirdth.y = alertarea->thirdth.y;
	alarea.alertarea.fouth.x = alertarea->fouth.x;
	alarea.alertarea.fouth.y = alertarea->fouth.y;
	alarea.NumberOfAlertCondition = NumberOfAlertCondition;
	alarea.listofeverycond = *conditionlist;
	listofeveryarea.push_back(alarea);
	alertmsghead.MaxAlertLevel = MaxAlertLevel;
}
AlertArea * AlertList::GetAlarea(gInt16 Index)
{
	if(Index< 0 || Index >= alertmsghead.NumberOfAlertArea )
		return 0;
	gInt16 count =0;
	list<EveryAlarmArea>::iterator itr;
	for( itr = listofeveryarea.begin() ; itr !=listofeveryarea.end();itr ++)
	{
		if(count == Index)
		{
			//(*itr).alertarea;	
			return &((*itr).alertarea);
		}		
		count ++;
	}
	return &((*--itr).alertarea);
}
gInt16 AlertList::GetNumberConditionofarea(gInt16 IndexArea)
{
	if(IndexArea< 0 || IndexArea >= alertmsghead.NumberOfAlertArea )
		return 0;
	gInt16 count =0;
	list<EveryAlarmArea>::iterator itr;
	for( itr = listofeveryarea.begin() ; itr !=listofeveryarea.end();itr ++)
	{
		if(count == IndexArea)
		{
			//(*itr).NumberOfAlertCondition;
			break;
		}		
		count ++;
	}
	return (*itr).NumberOfAlertCondition;

	return 0;
}
gInt16 AlertList::GetAlertLevel(gInt16 IndexArea,gInt16 IndexNumCond)
{

	if(IndexArea< 0 || IndexArea >= alertmsghead.NumberOfAlertArea )
		return 0;
	gInt16 count =0;
	list<EveryAlarmArea>::iterator itr;
	for( itr = listofeveryarea.begin() ; itr !=listofeveryarea.end();itr ++)
	{
		if(count == IndexArea)
		{
			//(*itr).listofeverycond;
			break;
		}
		count ++;
	}
	list<EveryCondAlert>::iterator itr1;
	list<EveryCondAlert> listcond;
	listcond = (*itr).listofeverycond;

	if(IndexNumCond< 0 )
		return 0;
	gInt16 count1 =0;
	for( itr1 = listcond.begin() ; itr1 !=listcond.end();itr1 ++)
	{
		if(count1 == IndexNumCond && itr1 != listcond.end())
		{
			//(*itr1).AlertLevelCode;
			break;
		}
		count1 ++;
	}
	return (*itr1).AlertLevelCode;

}
gInt16* AlertList::GetThresholdVal(gInt16 IndexArea,gInt16 IndexNumCond)
{
	if(IndexArea< 0 || IndexArea >= alertmsghead.NumberOfAlertArea )
		return 0;
	gInt16 count =0;
	list<EveryAlarmArea>::iterator itr;
	for( itr = listofeveryarea.begin() ; itr !=listofeveryarea.end();itr ++)
	{
		if(count == IndexArea)
		{
			//(*itr).listofeverycond;
			break;
		}
		count ++;
	}
	list<EveryCondAlert>::iterator itr1;
	list<EveryCondAlert> listcond;
	listcond = (*itr).listofeverycond;

	if(IndexNumCond< 0 )
		return 0;
	gInt16 count1 =0;
	for( itr1 = listcond.begin() ; itr1 !=listcond.end();itr1 ++)
	{
		if(count1 == IndexNumCond && itr1 != listcond.end())
		{
			//(*itr1).TresholdValue;
			break;
		}		
		count1 ++;
	}
	return (*itr1).TresholdValue;
}
EveryLayerAlert* AlertList::GetEveryLayerAlert(gInt16 IndexArea,gInt16 IndexNumCond,gInt16 IndexLayerNum)
{

	if(IndexArea< 0 || IndexArea >= alertmsghead.NumberOfAlertArea )
		return 0;
	gInt16 count =0;
	list<EveryAlarmArea>::iterator itr;
	for( itr = listofeveryarea.begin() ; itr !=listofeveryarea.end();itr ++)
	{
		if(count == IndexArea)
		{
			//(*itr).listofeverycond;
			break;
		}
		count ++;
	}
	list<EveryCondAlert>::iterator itr1;
	list<EveryCondAlert> * plistcond;
	plistcond = &((*itr).listofeverycond);

	if(IndexNumCond< 0 )
		return 0;
	gInt16 count1 =0;
	for( itr1 = (*plistcond).begin() ; itr1 !=(*plistcond).end();itr1 ++)
	{
		if(count1 == IndexNumCond && itr1 != (*plistcond).end())
		{
			//(*itr1).conditionlist;
			break;
		}
		count1 ++;
	}
	list<EveryLayerAlert>::iterator itr2;
	list<EveryLayerAlert> * plistlayer;
	plistlayer = &((*itr1).conditionlist);

	if(IndexLayerNum< 0 )
		return 0;
	gInt16 count2 =0;
	for( itr2 = (* plistlayer).begin() ; itr2 !=(* plistlayer).end();itr2 ++)
	{
		if(count2 == IndexLayerNum && itr2 != (* plistlayer).end())
		{
			//*itr2;
			break;
		}
		count2 ++;
	}
	return &(*itr2);

}

list<EveryAlarmArea>& AlertList::GetAlarmAreaList()
{
	return listofeveryarea;
}

GHRESULT AlertList::GetSiteCode(char * pSiteCode)
{
	if (pSiteCode == NULL)
	{
		return GE_INVALIDARG;
	}

	AlertMessageHead * pTempMsagHead = GetAertMessageHead();
	*pSiteCode++ = ( gChar )pTempMsagHead->SiteID[2]; 
	*pSiteCode++ = ( gChar )pTempMsagHead->SiteID[3];
	*pSiteCode++ = ( gChar )pTempMsagHead->SiteID[5]; 
	*pSiteCode++ = ( gChar )pTempMsagHead->SiteID[6];
	*pSiteCode++ = ( gChar )pTempMsagHead->SiteID[7];
	*pSiteCode++ = '\0'; 
	return GS_OK;
}

void AlertList::TransToGData(/*AlertList InList,*/ GenerationData * pOutData)
{
	AlertList& InList=*this;
	list<GenerationData*> Alarmlist;
	GenerationDataStream * pTempOut = (GenerationDataStream *)pOutData;
	list<EveryAlarmArea> plist = InList.GetAlarmAreaList();
	list<EveryCondAlert> everycond;
	list<EveryLayerAlert> everylary;
	list<Point> pointlist;
	list<Point>::iterator pointitr;
	list<EveryLayerAlert> ::iterator everylayitr;
	list<EveryCondAlert>::iterator everyconditr;
	list<EveryAlarmArea>::iterator everyareaitr;

	everyareaitr = plist.begin();

	AlertMessageHead tempHead = InList.alertmsghead;
	GenerationData * MsgHead = new(::std::nothrow) GenerationData;
	MsgHead->Resize(sizeof(tempHead));
	MsgHead->WriteData(&tempHead, sizeof(tempHead));
	Alarmlist.push_back(MsgHead);
	for (;everyareaitr != plist.end();everyareaitr++)//Çø
	{   
		AlarmArea alarmarea;
		memset(&alarmarea, 0, sizeof(alarmarea));
		alarmarea.alertarea= everyareaitr->alertarea;
		alarmarea.NumberOfAlertCondition = everyareaitr->NumberOfAlertCondition;
		GenerationData *pArea = new(::std::nothrow) GenerationData;
		pArea->Resize(sizeof(alarmarea));
		pArea->WriteData(&alarmarea, sizeof(alarmarea));

		list<GenerationData*> plistarea;
		everyconditr = everyareaitr->listofeverycond.begin();
		for (; everyconditr != everyareaitr->listofeverycond.end(); everyconditr++)//¼¶
		{
			AlarmCondition alarmcond;
			memset(&alarmcond, 0, sizeof(alarmcond));
			alarmcond.TresholdValue[0] = everyconditr->TresholdValue[0];
			alarmcond.TresholdValue[1] = everyconditr->TresholdValue[1];
			alarmcond.AlertLevelCode = everyconditr->AlertLevelCode;			
			alarmcond.NumOfAlerts = everyconditr->conditionlist.size();
			GenerationData *pCond = new(::std::nothrow) GenerationData;
			pCond->Resize(sizeof(alarmcond));
			pCond->WriteData(&alarmcond, sizeof(alarmcond));

			list<GenerationData*> plistcond;

			everylayitr = everyconditr->conditionlist.begin();
			for (; everylayitr != everyconditr->conditionlist.end(); everylayitr++)//²ã
			{
				AlarmLayer alarmlayer;
				memset(&alarmlayer, 0 ,sizeof(alarmlayer));
				alarmlayer.LayerNo = everylayitr->LayerNo;
				GenerationData *pLayer = new(::std::nothrow) GenerationData;
				pLayer->Resize(sizeof(alarmlayer));
				pLayer->WriteData(&alarmlayer, sizeof(alarmlayer));

				list<GenerationData*> plist;
				//GenerationDataStream *pGen = new(::std::nothrow) GenerationDataStream;
				GenerationData/*Stream*/ *pGen = new(::std::nothrow) GenerationData/*Stream*/;
				pointitr = everylayitr->layerpointlist.begin();
				//gInt16 nPoint = 0;
				//list<GenerationData*> pglist;

				int pointnum = everylayitr->layerpointlist.size();				
				//GenerationData * pAllPointData = new (::std::nothrow)GenerationData;
				pGen->Resize(pointnum * sizeof(Point));

				for(; pointitr != everylayitr->layerpointlist.end(); pointitr++)
				{
					//GenerationData *pPoint = new(::std::nothrow) GenerationData;
					//pPoint->Resize(sizeof(Point));
					//Point tempPoint = everylayitr->layerpointlist.front();
					//everylayitr->layerpointlist.pop_front();
					//pPoint->WriteData(&(tempPoint), sizeof(Point));
					//pglist.push_back(pPoint);

					//if(pglist.size() == 32000 || everylayitr->layerpointlist.empty())
					//{
					//	GenerationDataStream *pgGen = new(::std::nothrow) GenerationDataStream;
					//	pgGen->CreateFromList(pglist);
					//	pgGen->ReleaseGDataList(pglist);
					//	plist.push_back(pgGen);
					//}
					pGen->WriteData(&(* pointitr), sizeof(Point));
				}
				//pGen->CreateFromList(plist);
				//pGen->ReleaseGDataList(plist);
				//plist.clear();
				GenerationDataStream *pGenlay = new(::std::nothrow) GenerationDataStream;
				plist.push_back(pLayer);
				plist.push_back(pGen);
				pGenlay->CreateFromList(plist);
				pGenlay->ReleaseGDataList(plist);
				plistcond.push_back(pGenlay);

			}
			GenerationDataStream *pGencond = new(::std::nothrow) GenerationDataStream;
			pGencond->CreateFromList(plistcond);
			pGencond->ReleaseGDataList(plistcond);
			plistcond.clear();
			plistcond.push_back(pCond);
			plistcond.push_back(pGencond);
			GenerationDataStream *pGencond1 = new(::std::nothrow) GenerationDataStream;
			pGencond1->CreateFromList(plistcond);
			pGencond1->ReleaseGDataList(plistcond);
			plistarea.push_back(pGencond1);
		}
		GenerationDataStream *pGenarea = new(::std::nothrow) GenerationDataStream;
		pGenarea->CreateFromList(plistarea);
		pGenarea->ReleaseGDataList(plistarea);
		GenerationDataStream *pGenarea1 = new(::std::nothrow) GenerationDataStream;
		plistarea.clear();
		plistarea.push_back(pArea);
		plistarea.push_back(pGenarea);
		pGenarea1->CreateFromList(plistarea);
		pGenarea1->ReleaseGDataList(plistarea);
		Alarmlist.push_back(pGenarea1);
	}
	pTempOut->CreateFromList(Alarmlist);
	pTempOut->ReleaseGDataList(Alarmlist);

}

void AlertList::TransFromGData(/*AlertList * pOutList, */GenerationData * pInData)
{
	AlertList * pOutList=this;
	GenerationDataStream * pTempIn = (GenerationDataStream *)pInData;
	list<GenerationData*> pGenarea1;
	pTempIn->RevertToList(pGenarea1);

	if (pGenarea1.size() <=0)
	{
		return;
	}
	listofeveryarea.clear();
	list<GenerationData*>::iterator AlarmAreaitr = pGenarea1.begin();
	memcpy(&(pOutList->alertmsghead), (*AlarmAreaitr)->GetPointer(0),sizeof(pOutList->alertmsghead));
	
	//list<GenerationData*> pglist;
	list<GenerationData*>::iterator itr_g;
	//GenerationDataStream* lpGenAlarmArea=(GenerationDataStream*)(*(++AlarmAreaitr));
	//lpGenAlarmArea->RevertToList(pglist);
	//for (itr_g= pglist.begin(); itr_g != pglist.end(); itr_g++)
	for (itr_g= ++AlarmAreaitr; itr_g != pGenarea1.end(); itr_g++)
	{
		list<GenerationData*> pglist1;
		list<GenerationData*>::iterator itr_g1;
		((GenerationDataStream*)(*itr_g))->RevertToList(pglist1);
		itr_g1 = pglist1.begin();
		AlarmArea bb;
		bb= *((AlarmArea*)((*itr_g1)->GetPointer(0)));
		EveryAlarmArea kk;
		kk.alertarea = bb.alertarea;
		kk.NumberOfAlertCondition = bb.NumberOfAlertCondition;
		list<EveryCondAlert>& listofeverycond=kk.listofeverycond;
		itr_g1++;
		if(itr_g1!=pglist1.end())
		{
			list<GenerationData*> pglist2;
			((GenerationDataStream*)(*itr_g1))->RevertToList(pglist2);

			list<GenerationData*>::iterator itr_g2;
			for (itr_g2= pglist2.begin();itr_g2 != pglist2.end();itr_g2++)
			{
				list<GenerationData*> pglist3;
				list<GenerationData*>::iterator itr_g3;
				((GenerationDataStream*)(*itr_g2))->RevertToList(pglist3);
				itr_g3 = pglist3.begin();
				AlarmCondition cc;
				cc= *((AlarmCondition*)((*itr_g3)->GetPointer(0)));
				EveryCondAlert tt;
				tt.AlertLevelCode = cc.AlertLevelCode;
				tt.TresholdValue[0] = cc.TresholdValue[0];
				tt.TresholdValue[1] = cc.TresholdValue[1];
				list<EveryLayerAlert>& conditionlist=tt.conditionlist;
				itr_g3++;
				if(itr_g3!=pglist3.end())
				{
					list<GenerationData*> pglist4;
					((GenerationDataStream*)(*itr_g3))->RevertToList(pglist4);
					list<GenerationData*>::iterator itr_g4;
					for (itr_g4=pglist4.begin();itr_g4 != pglist4.end();itr_g4++)
					{
						list<GenerationData*> pglist5;
						list<GenerationData*>::iterator itr_g5;
						((GenerationDataStream*)(*itr_g4))->RevertToList(pglist5);
						itr_g5 = pglist5.begin();
						AlarmLayer dd;
						dd= *((AlarmLayer*)((*itr_g5)->GetPointer(0)));
						EveryLayerAlert dc;
						dc.LayerNo= dd.LayerNo;
						list<Point>& plistpoint=dc.layerpointlist;
						itr_g5++;
						if(itr_g5!=pglist5.end())
						{
							//list<GenerationData*> pglist6;
							//((GenerationDataStream*)(*itr_g5))->RevertToList(pglist6);
							//list<GenerationData*>::iterator itr_g6;
							
							//for (itr_g6=pglist6.begin();itr_g6!= pglist6.end();itr_g6++)
							//{
							//	list<GenerationData*> pglist7;
							//	((GenerationDataStream*)(*itr_g6))->RevertToList(pglist7);
								//list<GenerationData*>::iterator itr_g7;
							//	for(itr_g7=pglist7.begin();itr_g7!= pglist7.end();itr_g7++)
							//	{
							//		Point pp;
							//		pp = *((Point*)(*itr_g7)->GetPointer(0));
							//		plistpoint.push_back(pp);	
							//	}
							//	((GenerationDataStream*)(*itr_g6))->ReleaseGDataList(pglist7);
							//}
							//((GenerationDataStream*)(*itr_g5))->ReleaseGDataList(pglist6);
							Point * pPoint = (Point*)((*itr_g5)->GetPointer(0));
							int PointNum = (*itr_g5)->GetTrueSize()/sizeof(Point);
							for(int nPoint = 0; nPoint < PointNum; nPoint ++)
							{
								plistpoint.push_back(pPoint[nPoint]);	
							}
						}

						((GenerationDataStream*)(*itr_g4))->ReleaseGDataList(pglist5);
						conditionlist.push_back(dc);

					}
					((GenerationDataStream*)(*itr_g3))->ReleaseGDataList(pglist4);
				}

				((GenerationDataStream*)(*itr_g2))->ReleaseGDataList(pglist3);
				listofeverycond.push_back(tt);
			}
			((GenerationDataStream*)(*itr_g1))->ReleaseGDataList(pglist2);
		}
		((GenerationDataStream*)(*itr_g))->ReleaseGDataList(pglist1);
		pOutList->listofeveryarea.push_back(kk);

	}
	pTempIn->ReleaseGDataList(pGenarea1);
}

gInt16 AlertList::GetNumberOfConditions(gInt16 IndexArea)
{
	if(IndexArea< 0 || IndexArea >= alertmsghead.NumberOfAlertArea )
		return 0;
	gInt16 count =0;
	list<EveryAlarmArea>::iterator itr;
	for( itr = listofeveryarea.begin() ; itr !=listofeveryarea.end();itr ++)
	{
		if(count == IndexArea)
		{
			break;
		}		
		count ++;
	}
	return (gInt16)((*itr).listofeverycond.size());
}
