#include "On_Profile_Draw.h"
#include <osg/Billboard>
#include <osg/ShapeDrawable>
#include <osg/Shape>
#include <osg/PositionAttitudeTransform>
#include <osg/MatrixTransform>
#include <osgUtil/UpdateVisitor>
#include <osgUtil/CullVisitor>
#include <osg/AutoTransform>
#include <QtCore/QCoreApplication>
#include <osgUtil/SmoothingVisitor>

#include "goSymbolGeode.h"
#include "Layer.h"
#include "gogeometry.h"
#include "CoordinateTransform.h"
#include "goSymbolPoint.h"
#include <QStringList>
#include <QTextStream>
#include <QtCore/QFile>
#include <QGLWidget>
#include <QImage>
#include "helper.h"

#define TOLOCAL8BIT(qstring) \
	((const char*)(qstring.toLocal8Bit()))

class TransformNodeCallback : public osg::NodeCallback
{
public:
	virtual void operator()(osg::Node* node, osg::NodeVisitor* nv)
	{
		static osg::Vec3 localEye(0, 0, 0);
		osgUtil::CullVisitor* pCullVisitor = dynamic_cast<osgUtil::CullVisitor*>(nv);
		osgUtil::UpdateVisitor* pUpdapteVistor = dynamic_cast<osgUtil::UpdateVisitor*>(nv);

		if(pCullVisitor)
		{
			localEye = pCullVisitor->getEyeLocal();
			traverse(node, nv);
			return;
		}

		if(pUpdapteVistor && localEye.z() != 0)
		{
			float fDis = localEye.z() - m_ObjectPos.z();
			osg::Matrix matix;
			matix.setTrans(osg::Vec3(0, 0, fDis));
			osg::MatrixTransform* pMatrixTrans = dynamic_cast<osg::MatrixTransform*>(node);
			pMatrixTrans->setMatrix(matix);
			traverse(node,nv);
		}
	}
	osg::Vec3 m_ObjectPos;

};



osg::Vec3f GetValue(const ProfileData& profile,int i,int j)
{
	float fu = profile.umin+i*(profile.umax-profile.umin)/profile.n_u;
	float fv = profile.vmin+i*(profile.vmax-profile.vmin)/profile.n_v;
	float fz = profile.m_data.at(i*profile.n_v+j);
	return osg::Vec3f(fu,fv,fz);
}
//////////////////////////////////////////////////////////////////////////

On_Profile_Draw::On_Profile_Draw(void)
{
}


On_Profile_Draw::~On_Profile_Draw(void)
{
}

EARTHQUAKE_DATA_TYPE On_Profile_Draw::GetTypeOfProduct()
{
	return EARTHQUAKE_DATA_TYPE::STRUCT_PROFILE;
}

void On_Profile_Draw::LoadFile( const QString& filename )
{
	// vgrids.readVgridData("D:/data/cezhenData/slice/vgrids.in");	
	// readXYZfile(filename);	
	
	 readAscFile(filename);
	 CONTOURLEVELS levels;
	 levels.push_back((profiledata.fmax-profiledata.fmin)/2+profiledata.fmin);
     SearchContour(profiledata,levels);
	 SaveProfileToQImage();

	//SaveProfileToQImage();
}

bool On_Profile_Draw::openDynamicColorMap(ColorVector& cv, QString fname)
{	
	if (fname.isEmpty())
		return false;

	ifstream file(TOLOCAL8BIT(fname));

	if (!file)
		return false;

	QColor rgb;
	cv.clear();
	int r,g,b;
	while ( file ) 
	{		
		file >> r >> g >> b;
		rgb.setRed(r);
		rgb.setGreen(g);
		rgb.setBlue(b);

		file.ignore(1000,'\n');
		if (!file.good())
			break;
		else
		{
			/*rgb.a = 0;
			rgb.r /= 255;
			rgb.g /= 255;
			rgb.b /= 255;
			*/
			cv.push_back(rgb);	
		}
	}

	return true;
}

QColor On_Profile_Draw::ValueToPixel(float fmin,float fmax,float fvalue)
{
	int index = (int)((m_cv.size()-1) * (fvalue - fmin) / (fmax - fmin));
	if (index < 0)
		index = 0;
	if ((unsigned int)index > m_cv.size() - 1)
		index = (int)(m_cv.size() - 1);
	return m_cv[index];
}


bool On_Profile_Draw::readAscFile(const QString& filename)
{
	QFile f(filename);
	if (f.open(QFile::ReadOnly)) 
	{
		QTextStream      in(&f);
		QString strhead;
		int ncols,nrows;
		float xmin,ymax,xmax,ymin,finvalid,cellsize,fvalue;
		float fvaluemax,fvaluemin;
		in>>strhead>>ncols;
		if(strhead!="ncols" && strhead!="NCOLS") return false;
		fvaluemax = -999999;
		fvaluemin = 999999;
		profiledata.m_data.clear();
		in>>strhead>>nrows;
		in>>strhead>>xmin;
		in>>strhead>>ymin;
		in>>strhead>>cellsize;
		in>>strhead>>finvalid;
		xmax = xmin + ncols*cellsize;
		ymax = ymin + nrows*cellsize;
		for (int i=0;i<nrows;i++)
		{
			for (int j=0;j<ncols;j++)
			{
				if(!in.atEnd())
				{
					in>>fvalue;

					if(fvaluemax<fvalue) fvaluemax = fvalue;
					if(fvaluemin>fvalue) fvaluemin = fvalue;

					profiledata.m_data.push_back(fvalue);
				}				
			}
		}
		/*
		if(ncols>0 && nrows>0)
		{
			in>>fvalue;
			fvaluemax = fvaluemin = fvalue;
			while(!in.atEnd() && profiledata.m_data.size()<ncols*nrows)
			{
				in>>fvalue;
				if(fvaluemax<fvalue) fvaluemax = fvalue;
				if(fvaluemin>fvalue) fvaluemin = fvalue;
				
				profiledata.m_data.push_back(fvalue);
			}
		}
		*/

		profiledata.fmin = fvaluemin;
		profiledata.fmax = fvaluemax;
		profiledata.n_u = ncols;
		profiledata.n_v = nrows;
		profiledata.umin = xmin;
		profiledata.vmin = ymin;
		profiledata.umax = xmax;
		profiledata.vmax = ymax;
	}	 

	f.close();
	return true;
}

bool On_Profile_Draw::readXYZfile(const QString& filename)
{
	FILE* fp;
	if ((fp=fopen(filename.toStdString().c_str(),"r"))==NULL)
	{
		return false;
	}

	float lonmax,lonmin,latmax,latmin;
	float fvaluemax,fvaluemin;
	float lon,lat;
	float fvalue;
	int m_lonsize,m_latsize;
	QVector<float> lonlist;
	QVector<float> latlist;
	profiledata.m_data.clear();


	fscanf(fp,"%f %f %f\n",&lon,&lat,&fvalue);
	lonmax = lonmin = lon;
	latmax = latmin = lat;
	fvaluemax = fvaluemin = fvalue;	
	if (!lonlist.contains(lon)) lonlist.push_back(lon);
	if(!latlist.contains(lat)) latlist.push_back(lat);
	//QVector<float> data  ;
	profiledata.m_data.push_back(fvalue);
	//data.push_back(fvalue);

	while(!feof(fp))
	{
		fscanf(fp,"%f %f %f\n",&lon,&lat,&fvalue);
		if (!lonlist.contains(lon)) lonlist.push_back(lon);
		if(!latlist.contains(lat)) latlist.push_back(lat);
		if(fvaluemax<fvalue) fvaluemax = fvalue;
		if(fvaluemin>fvalue) fvaluemin = fvalue;
		profiledata.m_data.push_back(fvalue);
		//data.push_back(fvalue);
	}
	//profiledata.m_data = data;
	fclose(fp);
	qSort(lonlist);
	qSort(latlist);
	m_lonsize = lonlist.size();
	m_latsize = latlist.size();
	profiledata.n_u = m_lonsize;
	profiledata.n_v = m_latsize;
	profiledata.fmax = fvaluemax;
	profiledata.fmin = fvaluemin;
	profiledata.umin = lonlist.front();
	profiledata.umax = lonlist.back();
	profiledata.vmin = latlist.front();
	profiledata.vmax = latlist.back();
	return true;
	/*
	QFile f(filename);
	if (f.open(QFile::ReadOnly)) 
	{
		QTextStream      in(&f);
		QString strlon,strlat,strvalue;
		QStringList strlinelist;
		in>>strlon>>strlat>>strvalue;
		//strlinelist = strline.split(" ");
		float lonmax,lonmin,latmax,latmin;
		float fvaluemax,fvaluemin;
		float lon,lat;
		float fvalue;
		int m_lonsize,m_latsize;
		lonmax = lonmin = strlon.toFloat();
		latmax = latmin = strlat.toFloat();
		fvaluemax = fvaluemin = strvalue.toFloat();
		profiledata.m_data.clear();

		profiledata.m_data.push_back(fvaluemin);
		QVector<float> lonlist;
		QVector<float> latlist;
		lonlist.push_back(lonmax);
		latlist.push_back(latmax);
		int irow = 1;
		QString strline;
		while(!in.atEnd())
		{
			strline = in.readLine();
			strlinelist = strline.split("");
			//in>>strlon>>strlat>>strvalue;
			if (strlon!="" || strlat!="" || strvalue !="")
			{
				lon = strlon.toFloat();
				lat = strlat.toFloat();
				fvalue = strvalue.toFloat();
				if (!lonlist.contains(lon)) lonlist.push_back(lon);
				if(!latlist.contains(lat)) latlist.push_back(lat);
				if(fvaluemax<fvalue) fvaluemax = fvalue;
				if(fvaluemin>fvalue) fvaluemin = fvalue;

				irow++;
				profiledata.m_data.push_back(fvalue);
			}
		}	
		qSort(lonlist);
		qSort(latlist);
		m_lonsize = lonlist.size();
		m_latsize = latlist.size();
		profiledata.n_u = m_lonsize;
		profiledata.n_v = m_latsize;
		profiledata.fmax = fvaluemax;
		profiledata.fmin = fvaluemin;
		profiledata.umin = lonlist.front();
		profiledata.umax = lonlist.back();
		profiledata.vmin = latlist.front();
		profiledata.vmax = latlist.back();
		f.close();
		return true;
	}
	else
	{
		return false;
	}
	*/
}
/*
void On_Profile_Draw::SilceToQImage(string slicetype,float fvalue)
{
	
	VGrid* vgrid = vgrids.GetVGrid(0);
	if (vgrid)
	{		
		vgrid->GetProfile(profiledata,slicetype,fvalue);	
		if (!profiledata.m_data.empty())
		{
			float fmin = profiledata.m_data.front();
			float fmax = fmin;
			for (int i=0;i<profiledata.m_data.size();i++)
			{
				if (profiledata.m_data.at(i)<fmin) fmin = profiledata.m_data.at(i);
				if (profiledata.m_data.at(i)>fmax) fmax = profiledata.m_data.at(i);
			}

			QImage img = QImage(profiledata.n_u,profiledata.n_v,QImage::Format_ARGB32);
			QColor color;
			for (int i=0;i<img.width();i++)
			{
				for (int j=0;j<img.height();j++)
				{
					//img.setPixel(i,j,)
					color = ValueToPixel(fmin,fmax,profiledata.m_data.at(i*img.height()+j));
					img.setPixel(i,j,qRgba(color.red(),color.green(),color.blue(),50));
				}
			}

			img.save("profile.png");
		}
	}
	
}
*/

void On_Profile_Draw::SaveProfileToQImage()
{
	QColor color;
	int r,g,b;

	
	QString colormapname = QCoreApplication::applicationDirPath()+tr("/colormaps/color.map");

	openDynamicColorMap(m_cv,colormapname);

	QImage img = QImage(profiledata.n_u,profiledata.n_v,QImage::Format_ARGB32);
	float fmin,fmax;
	fmin = profiledata.fmin;
	fmax = profiledata.fmax;
	int width = img.width();
	int height = img.height();
	
	for (int i=0;i<img.width();i++)
	{
		for (int j=0;j<img.height();j++)
		{
			//img.setPixel(i,j,)
			color = ValueToPixel(fmin,fmax,profiledata.m_data.at(j*width+i));
		    r = color.red();
			g = color.green();
			b = color.blue();
			img.setPixel(i,j,qRgba(r,g,b,255));
		}
	}

	img.save("c:/slice.png");
	

	//test 
	/*
	FILE* fp;
	QVector<QColor> colorlist;
	QImage img =QImage("c:/color1.png");
	int width = img.width();
	for (int i=0;i<img.height();i++)
	{
		colorlist.push_back(img.pixel(width/2,i));
	}

	if ((fp=fopen("c:/color.map","w"))==NULL)
	{
		return;
	}
	for (int i=0;i<colorlist.size();i++)
	{
		fprintf(fp,"%d %d %d\n",colorlist.at(i).red(),colorlist.at(i).green(),colorlist.at(i).blue());
	}
	fclose(fp);
	*/
	//openDynamicColorMap(m_cv,"c:/color.map");
	/*
	QImage img2 = QImage(100,100,QImage::Format_ARGB32);
	for (int i=0;i<img2.width();i++)
	{
		for (int j=0;j<img2.height();j++)
		{
			color = ValueToPixel(0,100,(i+j)/2);
			r = color.red();
			g = color.green();
			b = color.blue();
			img2.setPixel(i,j,qRgba(r,g,b,255));
		}
	}

	img2.save("c:/slice2.png");
	*/
}

void On_Profile_Draw::SearchContour(const ProfileData& data,CONTOURLEVELS levels)
{
	
	m_contourtracer.SetContourLevels(levels);
	m_contourtracer.SetGrid(data);
	m_contourtracer.BeginSeriesAction();
	m_ContourlinestripArray = m_contourtracer.ToOSGVecArray();

	/*
	for (int i=0;i<linestripArray.size();i++)
	{
		osg::ref_ptr<osg::Geometry> geom = new osg::Geometry;
		osg::Vec3Array* arraysrc = linestripArray.at(i);
		osg::Vec3Array* arraydes = new osg::Vec3Array;

		for (int j=0;j<arraysrc->size();j++)
		{
			arraydes->push_back(LonLat2GL(lyer,arraysrc->at(j)));
		}
		geom->setVertexArray(arraydes);
		osg::Vec4Array* colors = new osg::Vec4Array;
		colors->push_back(osg::Vec4(1.0f,0.0f,0.0f,1.0f)); 
		geom->setColorArray(colors);
		geom->setColorBinding(osg::Geometry::BIND_OVERALL);
		geom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::LINE_STRIP,0,arraydes->size()));
		geode->addDrawable(geom);
	}
   
	return geode;
	*/
}

osg::Geode* On_Profile_Draw::createContourDrawable(Layer* layer)
{
	osg::Geode* geode = new osg::Geode();

	for (int i=0;i<m_ContourlinestripArray.size();i++)
	{
		osg::ref_ptr<osg::Geometry> geom = new osg::Geometry;
		osg::Vec3Array* arraysrc = m_ContourlinestripArray.at(i);
		osg::Vec3Array* arraydes = new osg::Vec3Array;

		for (int j=0;j<arraysrc->size();j++)
		{
			arraydes->push_back(LonLat2GL(layer,arraysrc->at(j),0.1));
		}
		geom->setVertexArray(arraydes);
		osg::Vec4Array* colors = new osg::Vec4Array;
		colors->push_back(osg::Vec4(1.0f,0.0f,0.0f,1.0f)); 
		geom->setColorArray(colors);
		geom->setColorBinding(osg::Geometry::BIND_OVERALL);
		geom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::LINE_STRIP,0,arraydes->size()));
		geode->addDrawable(geom);
	}

	return geode;
}

osg::Geode* On_Profile_Draw::createProfile(Layer* layer,const osg::Vec3& origin,const osg::Vec3& last)
{	
	//unsigned int numColumns = profiledata.n_u;
	//unsigned int numRows = profiledata.n_v;
	float dx,dy;
	dx = last.x() - origin.x();
	dy = last.y() - origin.y();
	osg::Vec3 src1,src2,dst1,dst2;
	src1 = origin;
//	src2 = last;
	LonLatToGL(layer, src1, dst1);
//	LonLatToGL(layer, src2, dst2);

	unsigned int numColumns = 10;
	unsigned int numRows = 10;
	unsigned int r;
	unsigned int c;

	osg::Geode* geode = new osg::Geode();

	// ---------------------------------------
	// Set up a StateSet to texture the objects
	// ---------------------------------------
	osg::StateSet* stateset = new osg::StateSet();

	osg::Image* image = osgDB::readImageFile("c:/slice.png");
	if (image)
	{
		//unsigned char* pdata = pimage->data(10,10);
		osg::Texture2D* texture = new osg::Texture2D;
		texture->setImage(image);
		stateset->setTextureAttributeAndModes(0,texture,osg::StateAttribute::ON); 
	}

	geode->setStateSet( stateset );	
	
	// compute z range of z values of grid data so we can scale it.
	float min_z = FLT_MAX;
	float max_z = -FLT_MAX;

	{
		osg::Geometry* geometry = new osg::Geometry;

		osg::Vec3Array& v = *(new osg::Vec3Array(numColumns*numRows));
		osg::Vec2Array& t = *(new osg::Vec2Array(numColumns*numRows));
		osg::Vec4ubArray& color = *(new osg::Vec4ubArray(1));

		color[0].set(255,255,255,255);

	//	float rowCoordDelta = size.y()/(float)(numRows-1);
	//	float columnCoordDelta = size.x()/(float)(numColumns-1);

		float rowTexDelta = 1.0f/(float)(numRows-1);
		float columnTexDelta = 1.0f/(float)(numColumns-1);

	//	osg::Vec3 pos = origin;
		osg::Vec3 SrcPos,DesPos;
		
		osg::Vec2 tex(0.0f,0.0f);
		int vi=0;
		SrcPos = origin;
		DesPos = dst1;
		for(r=0;r<numRows;++r)
		{
			//pos.x() = origin.x();
			SrcPos.x() = origin.x();
			tex.x() = 0.0f;
			for(c=0;c<numColumns;++c)
			{
				LonLatToGL(layer, SrcPos, DesPos);
				v[vi].set(DesPos.x(),DesPos.y(),DesPos.z()); //+(vertex[r+c*numRows][2]-min_z)*scale_z
				t[vi].set(tex.x(),tex.y());
				//pos.x()+=columnCoordDelta;	
				SrcPos.x() += dx/float(numColumns-1);
				tex.x()+=columnTexDelta;
				++vi;
			}
			//pos.y() += rowCoordDelta;
			SrcPos.y() += dy/float(numRows-1);
			tex.y() += rowTexDelta;
		}

		geometry->setVertexArray(&v);
		geometry->setColorArray(&color);
		geometry->setColorBinding(osg::Geometry::BIND_OVERALL);
		geometry->setTexCoordArray(0,&t);

		for(r=0;r<numRows-1;++r)
		{
			osg::DrawElementsUShort& drawElements = *(new osg::DrawElementsUShort(GL_QUAD_STRIP,2*numColumns));
			geometry->addPrimitiveSet(&drawElements);
			int ei=0;
			for(c=0;c<numColumns;++c)
			{
				drawElements[ei++] = (r+1)*numColumns+c;
				drawElements[ei++] = (r)*numColumns+c;
			}
		}

		geode->addDrawable(geometry);

		osgUtil::SmoothingVisitor sv;
		sv.smooth(*geometry);
	}

	return geode;
}


bool On_Profile_Draw::DumpAll( Layer* layer,P_E_COLORTABLE_VECTOR pColorTable )
{
	osg::Group* pGroup = layer->GetLayerNode()->asGroup();
	if(!pGroup)
	{
		pGroup = new osg::Group;
		layer->SetLayerNode(pGroup);
	}

	pGroup->addChild(createProfile(layer,osg::Vec3(profiledata.umin,profiledata.vmin,layer->LayerZ()),osg::Vec3(profiledata.umax,profiledata.vmax,layer->LayerZ())));
//	pGroup->addChild(createContour(layer,osg::Vec3(profiledata.umin,profiledata.vmin,layer->LayerZ()),osg::Vec3(profiledata.umax,profiledata.vmax,layer->LayerZ()),5));
	pGroup->addChild(createContourDrawable(layer));
	return true;
}

void On_Profile_Draw::LonLatToGL( Layer* layer, osg::Vec3& in, osg::Vec3& out )
{
	osg::Vec3 destVec3;
	layer->CoordTrans()->Transform(in, destVec3);
	double x = destVec3.x();
	double y = destVec3.y();
	double z = layer->LayerZ();
	out.set(x,y,z);
}

osg::Vec3 On_Profile_Draw::LonLat2GL(Layer* layer, osg::Vec3& in,float zshipft)
{
	osg::Vec3 destVec3;
	layer->CoordTrans()->Transform(in, destVec3);
	double x = destVec3.x();
	double y = destVec3.y();
	double z = layer->LayerZ() + zshipft;
	
	return osg::Vec3(x,y,z);
}

