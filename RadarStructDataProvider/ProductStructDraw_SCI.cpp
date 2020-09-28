#include "RadProjTransform.h"
#include "goQuadrilateral.h"

#include <osgText/Text>
#include <osg/Point>

#include <math.h>
#include <vector>

#include <QtCore/QCoreApplication>
#include "QtXml/QXmlStreamReader"
#include <QtCore/QFile>
#include <QDir>
#include <QDate>

#include "Struct_277_SCI.h"
#include "ProductStructDraw_SCI.h"

using namespace std;

CProductStructDraw_SCI::CProductStructDraw_SCI(void)
{
}


CProductStructDraw_SCI::~CProductStructDraw_SCI(void)
{
}

STRUCT_DATA_TYPE CProductStructDraw_SCI::GetTypeOfProduct()
{
	STRUCT_DATA_TYPE n = STRUCT_SCI;
	return n;
}

bool CProductStructDraw_SCI::DumpAll( Layer* layer ,CProductRead* lpProduct, P_COLORTABLE_VECTOR pColorTable, QString filename, int requestLayer /*= 0*/ )
{
	QStringList strList;
	bool bRet = false;
	PDBlock* blok = lpProduct->GetHeadOfProduct();
	if(blok == NULL)
	{
		return false;
	}
	for (int i = 0 ; i < blok->NumberOfLayers ; i++)
	{
		DILayer* layer = lpProduct->GetHeadOfLayer(i);
		if(layer == NULL)
		{
			return false;
		}
		for (int j = 0;j < layer->NumberOfBlocks ; j++)
		{
			DLIBlock* dblock = lpProduct->GetHeadOfBlock(i,j);
			StructHead * structHead = lpProduct->GetHeadOfBlockStruct(i,j);
			gInt16	NumbersOfStruct=structHead->NumbersOfData;		//结构个数
			void * p = lpProduct->GetDataOfBlockStruct(i, j);
			int lastID=0; 
		   //QString SpaceString = "         ";

			if(j==0)
			{
				TAGSSHEAD tagss = *(TAGSSHEAD *) p;
				//strList.append(SpaceString);
				strList.append(tr("风暴段搜索参数："));
				//strList.append(tr("\n"));
				QString Text = tr("");
				//strList.append(SpaceString);
				strList.append(tr("最小反射率阈值（单位：dBZ）："));
				//strList.append(tr("\n"));
				//strList.append(SpaceString);

				for(int k=0;k<15;k++)
				{

					if(tagss.LowThresholdOfReflectivity[k]!=32768&&tagss.LowThresholdOfReflectivity[k]!= 0 )
					{
						Text.append(QString::number(tagss.LowThresholdOfReflectivity[k]));
						//Text.append(Text);
						Text.append(tr("    "));
					}
					else
					{
						continue;
					}

				}
				strList.append(Text);
				//strList.append(tr("\n"));	

				Text = QString::number(tagss.ThresholdOfDropoutDifferenceReflectivity);
				//strList.append(SpaceString);
				strList.append(tr("淘汰反射率差阈值（单位：dBZ）：  ")+Text);
				//strList.append(Text);
				//strList.append(tr("\n"));	

				Text = QString::number(tagss.ThresholdOfDropoutCount);
				//strList.append(SpaceString);
				strList.append(tr("淘汰点数阈值：")+Text);
				//strList.append(Text);
				//strList.append(tr("\n"));	

				Text = QString::number(tagss.ThresholdOfSegmentLength);
				//strList.append(SpaceString);
				strList.append(tr("风暴段长度阈值（单位：KM）：  ")+Text);
				//strList.append(Text);
				//strList.append(tr("\n"));	

				Text = QString::number(tagss.ThresholdOfSeparatedAzimuth);
				//strList.append(SpaceString);
				strList.append(tr("相邻风暴段方位间距阈值（单位：°）：")+Text);
				//strList.append(Text);
				//strList.append(tr("\n"));	

				//strList.append(SpaceString);
				strList.append(tr("风暴分量搜索参数："));
				//strList.append(tr("\n"));	

				Text = QString::number(tagss.ThresholdOfSegmentNumber);
				//strList.append(SpaceString);
				strList.append(tr("最少风暴段个数阈值：  ")+Text);
				//strList.append(Text);
				//strList.append(tr("\n"));		

				Text = QString::number(tagss.ThresholdOfOverlap);
				//strList.append(SpaceString);
				strList.append(tr("相邻风暴段重叠距离阈值（单位：KM）：  ")+Text);
				//strList.append(Text);
				//strList.append(tr("\n"));	

				Text = QString::number(tagss.ThresholdOfComponentArea);
				//strList.append(SpaceString);
				strList.append(tr("最小重叠面积阈值（单位：k㎡）：  ")+Text);
				//strList.append(Text);
				//strList.append(tr("\n"));	

				//strList.append(SpaceString);
				strList.append(tr("风暴单体搜索参数："));
				//strList.append(tr("\n"));	

				//strList.append(SpaceString);
				
				for(int k=0;k<3;k++)
				{
					Text.append(QString::number(tagss.ThresholdOfSearchRadius[k]));
					//strList.append(Text);
					Text.append(tr("     "));
					//Text = Text + tr("     ");
				}
				strList.append(tr("最小关联距离阈值（单位：KM）:  ")+Text);
				//strList.append(tr("\n"));	

				Text = QString::number(tagss.ThresholdOfElevationMerge);
				//strList.append(SpaceString);
				strList.append(tr("风暴合并的仰角阈值（单位：°）：  ")+Text);
				//strList.append(Text);
				//strList.append(tr("\n"));	

				Text = QString::number(tagss.ThresholdOfHorizontalMerge);
				//strList.append(SpaceString);
				strList.append(tr("风暴合并的水平距离阈值（单位：KM）：  ")+Text);
				//strList.append(Text);
				//strList.append(tr("\n"));	

				Text = QString::number(tagss.ThresholdOfHeightMerge);
				//strList.append(SpaceString);
				strList.append(tr("风暴合并的高度阈值（单位：KM）：  ")+Text);
				//strList.append(Text);
				//strList.append(tr("\n"));	

				Text = QString::number(tagss.ThresholdOfHorizontalDelete);
				//strList.append(SpaceString);
				strList.append(tr("相邻风暴体的水平删除阈值（单位：KM）：  ")+Text);
				//strList.append(Text);
				//strList.append(tr("\n"));	

				Text = QString::number(tagss.ThresholdOfDepthDelete);
				//strList.append(SpaceString);
				strList.append(tr("相邻风暴体的高度删除阈值（单位：KM）：  ")+Text);
				//strList.append(Text);
				//strList.append(tr("\n"));	

				//strList.append(tr("\n\n"));
				//strList.append(SpaceString);
				strList.append(tr("风暴单体信息："));
				//strList.append(tr("\n\n"));

			}
			else
			{
				for(int i=0;i<NumbersOfStruct;i++)
				{
					STORM_CELLATTRIBUTES   storm_sell =( (STORM_CELLATTRIBUTES *) p)[i];
					STORM_CELLID SCI = storm_sell.sci;


					if(SCI.id>0)
					{
						if(lastID>SCI.id)
						{
							break;
						}

						QString Text=tr("");
						Text = QString::number( SCI.id);

						//strList.append(SpaceString);
						strList.append(" \n ");
						strList.append( tr("风暴号：")+Text);
						//strList.append(Text);//
						//strList.append(tr("\n"));

						Text = QString::number(SCI.NSC);
						//strList.append(SpaceString);
						strList.append(tr("组成风暴体的风暴分量数量：")+Text);
						//strList.append(Text);//
						//strList.append(tr("\n"));

						Text = QString::number(SCI.MASS);
						//strList.append(SpaceString);
						strList.append(tr("风暴体的质量（单位：吨）：  ")+Text);
						//strList.append(Text);//
						//strList.append(tr("\n"));

						QString VX=tr("");
						QString VY=tr("");
						VX = QString::number(SCI.vx);
						VY = QString::number(SCI.vy);
						//strList.append(SpaceString);
						strList.append(tr("风暴体质量权重中心坐标（单位：KM）：  "));
						//strList.append(tr("\n"));
						//strList.append(SpaceString);
						strList.append(tr("风暴体质量权重中心所在的X坐标（单位：KM）：  ")+VX);
						//strList.append(Text);//
						//strList.append(tr("\n"));

						//strList.append(SpaceString);
						strList.append(tr("风暴体质量权重中心所在的Y坐标（单位：KM）：  ")+VY);
						//strList.append(Text);//
						//strList.append(tr("\n"));

						QString Base=tr("");
						QString Top=tr("");
						Base = QString::number(SCI.base);
						Top = QString::number(SCI.top);
						//strList.append(SpaceString);
						strList.append(tr("风暴体底高度（单位：KM）：  ")+Base);
						//strList.append(Base);//
						//strList.append(tr("\n"));

						//strList.append(SpaceString);
						strList.append(tr("风暴体顶高度（单位：KM）：  ")+Top);
						//strList.append(Top);//
						//strList.append(tr("\n"));

						QString r=tr("");
						QString az=tr("");
						QString h=tr("");
						r = QString::number(SCI.r);
						az = QString::number(SCI.az);
						h = QString::number(SCI.h);
						//strList.append(SpaceString);
						strList.append(tr("风暴体质量权重中心当前位置距雷达距离（单位： KM）：  ")+r);
						//strList.append(r);//
						//strList.append(tr("\n"));

						//strList.append(SpaceString);
						strList.append(tr("风暴体质量权重中心当前位置所在的方位角（单位：°）： ")+az);
						//strList.append(az);//
						//strList.append(tr("\n"));

						//strList.append(SpaceString);
						strList.append(tr("风暴体质量权重中心所在的高度（单位：KM）：  ")+h);
						//strList.append(h);//
						//strList.append(tr("\n"));

						QString VIL=tr("");
						VIL = QString::number(SCI.VIL);
						//strList.append(SpaceString);
						strList.append(tr("风暴体垂直累积液态水含量（单位：kg/㎡）：  ")+VIL);
						//strList.append(VIL);//
						//strList.append(tr("\n"));

						QString depth=tr("");
						depth= QString::number(SCI.depth*0.01);
						//strList.append(SpaceString);
						strList.append(tr("风暴体厚度（单位：KM）：  ")+depth);
						//strList.append(depth);//
						//strList.append(tr("\n"));

						QString maxZ=tr("");
						maxZ= QString::number(SCI.maxZ);
						//strList.append(SpaceString);
						strList.append(tr("最强反射率因子（单位：0.1dBZ）：  ")+maxZ);
						//strList.append(maxZ);//
						//strList.append(tr("\n"));

						QString H_maxZ=tr("");
						H_maxZ= QString::number(SCI.H_maxZ);
						//strList.append(SpaceString);
						strList.append(tr("最大反射率因子高度（单位：KM）：  ")+H_maxZ);
						//strList.append(H_maxZ);//
						//strList.append(tr("\n\n\n"));
						lastID=SCI.id;
					}
				}
			}
		}
	}
	osg::Vec3 vpostion(0,54000000,0.51);
	int height = vpostion.y();
	for (int i = 0 ; i<strList.size() ; i++)
	{
		DrawText(layer,strList.at(i).toStdString(),vpostion,1000);
		vpostion.set(vpostion.x(),height,0.51);
		height = vpostion.y()-3000;
		vpostion.set(vpostion.x(),height,0.51);
	}
	return true;
}


void CProductStructDraw_SCI::DrawText(Layer* layer, const std::string& text,osg::Vec3 positionOffset,float size )
{
	osgText::Text* t = new osgText::Text();
	setlocale( LC_ALL, "chs" );
	int requiredSize = mbstowcs(NULL, text.c_str(), 0); 
	wchar_t* wtext = new wchar_t[requiredSize+1]; 
	mbstowcs(wtext, text.c_str(), requiredSize+1); 
	t->setText(wtext); 
	delete [] wtext; 
	wtext = NULL; 

	t->setPosition( positionOffset );
	t->setAutoRotateToScreen( false );
	t->setCharacterSizeMode( osgText::Text::OBJECT_COORDS_WITH_MAXIMUM_SCREEN_SIZE_CAPPED_BY_FONT_HEIGHT );
	t->setCharacterSize(100000);//100000  
	t->setAxisAlignment( osgText::Text::XY_PLANE );
	t->setAlignment( osgText::Text::LEFT_CENTER );
	osgText::Font* font = osgText::readFontFile("Fonts/simhei.ttf");
	t->setFont(font);

	t->setBackdropColor( osg::Vec4( 0.3 ,  0.3 ,  0.3 , 1 ) );
	t->setBackdropType( osgText::Text::OUTLINE );

	osg::Geode* pGeo = layer->GetLayerNode()->asGeode();
	if(pGeo)
		pGeo->addDrawable(t);
}

QImage CProductStructDraw_SCI::DrawThumbs(CProductRead* lpProduct,P_COLORTABLE_VECTOR pColorTable,float imsize)
{
	QImage image;

	return image;
}
