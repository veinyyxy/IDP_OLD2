#include "GeneralPolylineBar.h"
#if 0
extern QVector<XFILLPOINT*> ShadeData;			// 填图多边形数组
extern QVector<lineData*> ContourData;						// 等值线线段数组
extern QVector<LineTextValue*> ContourTextValueArray;		// 数字位置数组

GeneralPolyLineBar::GeneralPolyLineBar( void )
{
	pRenderable[0] = 0;
	pRenderable[1] = 0;
}

GeneralPolyLineBar::~GeneralPolyLineBar( void )
{
	Clear();
}


int GeneralPolyLineBar::Draw(QLeaf* container, QList<QColor>* pListColor, int nDim, int nTime, int nLev, float fLat, float fLon, const char *strMode, bool bXY2YX, int nEleNum)
{

	gcmn.sig = 0;
	gacmd("set csmooth on",&gcmn,0);
	gcmn.sig = 0;
	gacmd("set cterp off",&gcmn,0);
	gcmn.sig = 0;
	gacmd("set grads off",&gcmn,0);
	gcmn.sig = 0;
	gacmd("set clab off",&gcmn,0);

	switch(nDim)
	{
	case 0:  // time
		{
			if(nTime < 2)
			{
				//CCIPASMessageBox::CIPASCritical(NULL, //CCIPASMessageBox::CIPAS_OK,tr("错误"),tr("至少包含两个时间"));
				return 1;
			}
			sprintf(m_Command,"set z 1");
			gcmn.sig = 0;
			gacmd(m_Command,&gcmn,0);
			sprintf(m_Command,"set lat %f",fLat);
			gcmn.sig = 0;
			gacmd(m_Command,&gcmn,0);
			sprintf(m_Command,"set lon %f",fLon);
			gcmn.sig = 0;
			gacmd(m_Command,&gcmn,0);
			sprintf(m_Command,"set t 1 %d",nTime);
			gcmn.sig = 0;
			gacmd(m_Command,&gcmn,0);

			if(bXY2YX == true)
			{
				sprintf( m_Command, "set xyrev on");
				gcmn.sig = 0;
				gacmd( m_Command, &gcmn, 0 );
			}

			sprintf(m_Command,"set gxout %s",strMode);
			gcmn.sig = 0;
			gacmd(m_Command,&gcmn,0);

			if (pListColor->count() != nEleNum)
			{
				//CCIPASMessageBox::CIPASCritical(NULL, //CCIPASMessageBox::CIPAS_OK,tr("错误"),tr("元素数量和颜色列表数量不一致"));
				return 1;
			}

			int nIndex;
			char *chrTemp = "abcdefghijklmnopqrstuvwxyz";
			for (nIndex = 0;nIndex < nEleNum;nIndex++)
			{
				if (nIndex == 0)
				{
					sprintf(m_Command,"display v%c",chrTemp[nIndex]);
					gcmn.sig = 0;
					if(gacmd(m_Command,&gcmn,0))
					{
						if(gcmn.str_errorinfo != NULL)
						{
							//CCIPASMessageBox::CIPASCritical(NULL, //CCIPASMessageBox::CIPAS_OK,tr("错误"),gcmn.str_errorinfo);
						}
						return 1;
					}
					if (gcmn.error == true)
					{
						//CCIPASMessageBox::CIPASCritical(NULL, //CCIPASMessageBox::CIPAS_OK,tr("错误"),gcmn.str_errorinfo);
						return 1;
					}

					Dump(container,pListColor->at(nIndex));
				}
				else
				{
					gcmn.sig = 0;
					gacmd("set frame off",&gcmn,0);
					gcmn.sig = 0;
					gacmd("set grid off",&gcmn,0);
					gcmn.sig = 0;
					gacmd("set xlab off",&gcmn,0);

					sprintf(m_Command,"display v%c",chrTemp[nIndex]);
					gcmn.sig = 0;
					if(gacmd(m_Command,&gcmn,0))
					{
						if(gcmn.str_errorinfo != NULL)
						{
							//CCIPASMessageBox::CIPASCritical(NULL, //CCIPASMessageBox::CIPAS_OK,tr("错误"),gcmn.str_errorinfo);
						}
						return 1;
					}
					if (gcmn.error == true)
					{
						//CCIPASMessageBox::CIPASCritical(NULL, //CCIPASMessageBox::CIPAS_OK,tr("错误"),gcmn.str_errorinfo);
						return 1;
					}

					DumpMulti(container,pListColor->at(nIndex));
				}
				ClearExternVector();
			}
		}
		break;

	case 1: // level
		{
			sprintf(m_Command,"set zlog on");
			gcmn.sig = 0;
			gacmd(m_Command,&gcmn,0);

			if(nLev < 2)
			{
				//CCIPASMessageBox::CIPASCritical(NULL, //CCIPASMessageBox::CIPAS_OK,tr("错误"),tr("至少包含两个层"));
				return 1;
			}

			sprintf(m_Command,"set z 1 %d",nLev);
			gcmn.sig = 0;
			gacmd(m_Command,&gcmn,0);
			sprintf(m_Command,"set lat %f",fLat);
			gcmn.sig = 0;
			gacmd(m_Command,&gcmn,0);
			sprintf(m_Command,"set lon %f",fLon);
			gcmn.sig = 0;
			gacmd(m_Command,&gcmn,0);
			sprintf(m_Command,"set t 1");
			gcmn.sig = 0;
			gacmd(m_Command,&gcmn,0);

			if(bXY2YX == true)
			{
				sprintf( m_Command, "set xyrev on");
				gcmn.sig = 0;
				gacmd( m_Command, &gcmn, 0 );

				sprintf(m_Command,"set zlog off");
				gcmn.sig = 0;
				gacmd(m_Command,&gcmn,0);
			}

			sprintf(m_Command,"set gxout %s",strMode);
			gcmn.sig = 0;
			gacmd(m_Command,&gcmn,0);

			if (pListColor->count() != nEleNum)
			{
				//CCIPASMessageBox::CIPASCritical(NULL, //CCIPASMessageBox::CIPAS_OK,tr("错误"),tr("元素数量和颜色列表数量不一致"));
				return 1;
			}

			int nIndex;
			char *chrTemp = "abcdefghijklmnopqrstuvwxyz";
			for (nIndex = 0;nIndex < nEleNum;nIndex++)
			{
				if (nIndex == 0)
				{
					sprintf(m_Command,"display v%c",chrTemp[nIndex]);
					gcmn.sig = 0;
					if(gacmd(m_Command,&gcmn,0))
					{
						if(gcmn.str_errorinfo != NULL)
						{
							//CCIPASMessageBox::CIPASCritical(NULL, //CCIPASMessageBox::CIPAS_OK,tr("错误"),gcmn.str_errorinfo);
						}
						return 1;
					}
					if (gcmn.error == true)
					{
						//CCIPASMessageBox::CIPASCritical(NULL, //CCIPASMessageBox::CIPAS_OK,tr("错误"),gcmn.str_errorinfo);
						return 1;
					}

					Dump(container,pListColor->at(nIndex));
				}
				else
				{
					gcmn.sig = 0;
					gacmd("set frame off",&gcmn,0);
					gcmn.sig = 0;
					gacmd("set grid off",&gcmn,0);
					gcmn.sig = 0;
					gacmd("set xlab off",&gcmn,0);

					sprintf(m_Command,"display v%c",chrTemp[nIndex]);
					gcmn.sig = 0;
					if(gacmd(m_Command,&gcmn,0))
					{
						if(gcmn.str_errorinfo != NULL)
						{
							//CCIPASMessageBox::CIPASCritical(NULL, //CCIPASMessageBox::CIPAS_OK,tr("错误"),gcmn.str_errorinfo);
						}
						return 1;
					}
					if (gcmn.error == true)
					{
						//CCIPASMessageBox::CIPASCritical(NULL, //CCIPASMessageBox::CIPAS_OK,tr("错误"),gcmn.str_errorinfo);
						return 1;
					}

					DumpMulti(container,pListColor->at(nIndex));
				}
				ClearExternVector();
			}
		}
		break;

	case 2:  // lat
		{
			sprintf(m_Command,"set t 1");
			gcmn.sig = 0;
			gacmd(m_Command,&gcmn,0);
			sprintf(m_Command,"set z 1");
			gcmn.sig = 0;
			gacmd(m_Command,&gcmn,0);
			sprintf(m_Command,"set lon %f",fLon);
			gcmn.sig = 0;
			gacmd(m_Command,&gcmn,0);

			if(bXY2YX == true)
			{
				sprintf( m_Command, "set xyrev on");
				gcmn.sig = 0;
				gacmd( m_Command, &gcmn, 0 );
			}

			sprintf(m_Command,"set gxout %s",strMode);
			gcmn.sig = 0;
			gacmd(m_Command,&gcmn,0);

			if (pListColor->count() != nEleNum)
			{
				//CCIPASMessageBox::CIPASCritical(NULL, //CCIPASMessageBox::CIPAS_OK,tr("错误"),tr("元素数量和颜色列表数量不一致"));
				return 1;
			}

			int nIndex;
			char *chrTemp = "abcdefghijklmnopqrstuvwxyz";
			for (nIndex = 0;nIndex < nEleNum;nIndex++)
			{
				if (nIndex == 0)
				{
					sprintf(m_Command,"display v%c",chrTemp[nIndex]);
					gcmn.sig = 0;
					if(gacmd(m_Command,&gcmn,0))
					{
						if(gcmn.str_errorinfo != NULL)
						{
							//CCIPASMessageBox::CIPASCritical(NULL, //CCIPASMessageBox::CIPAS_OK,tr("错误"),gcmn.str_errorinfo);
						}
						return 1;
					}
					if (gcmn.error == true)
					{
						//CCIPASMessageBox::CIPASCritical(NULL, //CCIPASMessageBox::CIPAS_OK,tr("错误"),gcmn.str_errorinfo);
						return 1;
					}

					Dump(container,pListColor->at(nIndex));
				}
				else
				{
					gcmn.sig = 0;
					gacmd("set frame off",&gcmn,0);
					gcmn.sig = 0;
					gacmd("set grid off",&gcmn,0);
					gcmn.sig = 0;
					gacmd("set xlab off",&gcmn,0);

					sprintf(m_Command,"display v%c",chrTemp[nIndex]);
					gcmn.sig = 0;
					if(gacmd(m_Command,&gcmn,0))
					{
						if(gcmn.str_errorinfo != NULL)
						{
							//CCIPASMessageBox::CIPASCritical(NULL, //CCIPASMessageBox::CIPAS_OK,tr("错误"),gcmn.str_errorinfo);
						}
						return 1;
					}
					if (gcmn.error == true)
					{
						//CCIPASMessageBox::CIPASCritical(NULL, //CCIPASMessageBox::CIPAS_OK,tr("错误"),gcmn.str_errorinfo);
						return 1;
					}

					DumpMulti(container,pListColor->at(nIndex));
				}
				ClearExternVector();
			}
		}
		break;

	case 3:  // lon
		{
			sprintf(m_Command,"set t 1");
			gcmn.sig = 0;
			gacmd(m_Command,&gcmn,0);
			sprintf(m_Command,"set z 1");
			gcmn.sig = 0;
			gacmd(m_Command,&gcmn,0);
			sprintf(m_Command,"set lat %f",fLat);
			gcmn.sig = 0;
			gacmd(m_Command,&gcmn,0);

			if(bXY2YX == true)
			{
				sprintf( m_Command, "set xyrev on");
				gcmn.sig = 0;
				gacmd( m_Command, &gcmn, 0 );
			}

			sprintf(m_Command,"set gxout %s",strMode);
			gcmn.sig = 0;
			gacmd(m_Command,&gcmn,0);

			if (pListColor->count() != nEleNum)
			{
				//CCIPASMessageBox::CIPASCritical(NULL, //CCIPASMessageBox::CIPAS_OK,tr("错误"),tr("元素数量和颜色列表数量不一致"));
				return 1;
			}

			int nIndex;
			char *chrTemp = "abcdefghijklmnopqrstuvwxyz";
			for (nIndex = 0;nIndex < nEleNum;nIndex++)
			{
				if (nIndex == 0)
				{
					sprintf(m_Command,"display v%c",chrTemp[nIndex]);
					gcmn.sig = 0;
					if(gacmd(m_Command,&gcmn,0))
					{
						if(gcmn.str_errorinfo != NULL)
						{
							//CCIPASMessageBox::CIPASCritical(NULL, //CCIPASMessageBox::CIPAS_OK,tr("错误"),gcmn.str_errorinfo);
						}
						return 1;
					}
					if (gcmn.error == true)
					{
						//CCIPASMessageBox::CIPASCritical(NULL, //CCIPASMessageBox::CIPAS_OK,tr("错误"),gcmn.str_errorinfo);
						return 1;
					}

					Dump(container,pListColor->at(nIndex));
				}
				else
				{
					gcmn.sig = 0;
					gacmd("set frame off",&gcmn,0);
					gcmn.sig = 0;
					gacmd("set grid off",&gcmn,0);
					gcmn.sig = 0;
					gacmd("set xlab off",&gcmn,0);

					sprintf(m_Command,"display v%c",chrTemp[nIndex]);
					gcmn.sig = 0;
					if(gacmd(m_Command,&gcmn,0))
					{
						if(gcmn.str_errorinfo != NULL)
						{
							//CCIPASMessageBox::CIPASCritical(NULL, //CCIPASMessageBox::CIPAS_OK,tr("错误"),gcmn.str_errorinfo);
						}
						return 1;
					}
					if (gcmn.error == true)
					{
						//CCIPASMessageBox::CIPASCritical(NULL, //CCIPASMessageBox::CIPAS_OK,tr("错误"),gcmn.str_errorinfo);
						return 1;
					}

					DumpMulti(container,pListColor->at(nIndex));
				}
				ClearExternVector();
			}
		}
		break;

	default: break;
	}

	return 0;
}

int GeneralPolyLineBar::Dump()
{
	int num;
	int i,j;
	QVector4D color4D;
	/****************** line ***************/
	num = ContourData.count();
	Line * pLine = new Line[num];
	pRenderable[0] = pLine;

	for(i = 0 ; i < num; i++)
	{
		lineData * pLineInfo = ContourData.at(i);
		for(j= 0 ;j < pLineInfo->arrayData.count();)
		{
			QVector4DArray* pColor = (pLine + i)->getColorArray();
			QVector3DArray* pArray = (pLine + i)->getVectexArray();

			if(qAbs(pLineInfo->nContourValue - 99999999.0f) < 0.0001) 
			{
				color4D = QVector4D(linecolor.redF(),linecolor.greenF(),linecolor.blueF(),1.0f);
			}
			else
			{
				color4D = QVector4D(1.0f,1.0f,1.0f,1.0f);
			}

			for(j= 0 ;j < pLineInfo->arrayData.count();)
			{
				pArray->push_back(QVector3D(pLineInfo->arrayData[j],pLineInfo->arrayData[j+1],pLineInfo->arrayData[j+2]));
				pColor->push_back(color4D);
				j = j + 3;
			}
			(pLine + i)->SetColorArray();
			(pLine + i)->SetBakData();
			container->InsertRenderable2(pLine + i);
		}
	}

	/****************** shaded ***************/
	num = ShadeData.count();
	GISPolygon* pPolygon = new GISPolygon[num];
	pRenderable[1] = pPolygon;
	for (i = 0;i <num;i++)
	{
		const XFILLPOINT *fillpoint = ShadeData.at(i);
		
		QVector4DArray* pColor = (pPolygon + i)->getColorArray();
		QVector3DArray* pArray = (pPolygon + i)->getVectexArray();
		if (fillpoint->n == 4)
		{
			color4D = QVector4D(linecolor.redF(),linecolor.greenF(),linecolor.blueF(),1);
			for (j =0;j< fillpoint->n;j++)
			{
				pColor->push_back(QVector4D(color4D));
				pArray->push_back(QVector3D(fillpoint->pPoint[j].x,fillpoint->pPoint[j].y,0));
			}
		}
		(pPolygon + i)->SetBakData();
		(pPolygon + i)->SetColorArray();
		container->InsertRenderable2(pPolygon + i);
	}

	return 0;
}

#endif