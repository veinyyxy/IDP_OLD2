#include <osg/Node>
#include <osg/Geometry>
#include <osg/Notify>
#include <osg/Texture3D>
#include <osg/Texture1D>
#include <osg/ImageSequence>
#include <osg/TexGen>
#include <osg/Geode>
#include <osg/Billboard>
#include <osg/PositionAttitudeTransform>
#include <osg/ClipNode>
#include <osg/AlphaFunc>
#include <osg/TexGenNode>
#include <osg/TexEnv>
#include <osg/TexEnvCombine>
#include <osg/Material>
#include <osg/PrimitiveSet>
#include <osg/Endian>
#include <osg/BlendFunc>
#include <osg/BlendEquation>
#include <osg/TransferFunction>
#include <osg/MatrixTransform>

#include <osgDB/Registry>
#include <osgDB/ReadFile>
#include <osgDB/WriteFile>
#include <osgDB/FileUtils>
#include <osgDB/FileNameUtils>

#include <osgGA/EventVisitor>
#include <osgGA/TrackballManipulator>
#include <osgGA/FlightManipulator>
#include <osgGA/KeySwitchMatrixManipulator>

#include <osgUtil/CullVisitor>

#include <osgViewer/Viewer>
#include <osgViewer/ViewerEventHandlers>

#include <osgManipulator/TabBoxDragger>
#include <osgManipulator/TabPlaneTrackballDragger>
#include <osgManipulator/TrackballDragger>

#include <osg/io_utils>

#include <algorithm>
#include <iostream>

#include <osg/ImageUtils>
#include <osgVolume/Volume>
#include <osgVolume/VolumeTile>
#include <osgVolume/RayTracedTechnique>
#include <osgVolume/FixedFunctionTechnique>

#include "osgVolumeImp.h"

enum ShadingModel
{
	Standard,
	Light,
	Isosurface,
	MaximumIntensityProjection
};

osg::Image* createTexture3D(osg::ImageList& imageList,
	unsigned int numComponentsDesired,
	int s_maximumTextureSize,
	int t_maximumTextureSize,
	int r_maximumTextureSize,
	bool resizeToPowerOfTwo)
{

	if (numComponentsDesired==0)
	{
		return osg::createImage3DWithAlpha(imageList,
			s_maximumTextureSize,
			t_maximumTextureSize,
			r_maximumTextureSize,
			resizeToPowerOfTwo);
	}
	else
	{
		GLenum desiredPixelFormat = 0;
		switch(numComponentsDesired)
		{
		case(1) : desiredPixelFormat = GL_LUMINANCE; break;
		case(2) : desiredPixelFormat = GL_LUMINANCE_ALPHA; break;
		case(3) : desiredPixelFormat = GL_RGB; break;
		case(4) : desiredPixelFormat = GL_RGBA; break;
		}

		return osg::createImage3D(imageList,
			desiredPixelFormat,
			s_maximumTextureSize,
			t_maximumTextureSize,
			r_maximumTextureSize,
			resizeToPowerOfTwo);
	}
}

class TestSupportOperation: public osg::GraphicsOperation
{
public:

	TestSupportOperation():
	  osg::GraphicsOperation("TestSupportOperation",false),
		  supported(true),
		  errorMessage(),
		  maximumTextureSize(256) {}

	  virtual void operator () (osg::GraphicsContext* gc)
	  {
		  OpenThreads::ScopedLock<OpenThreads::Mutex> lock(mutex);

		  glGetIntegerv( GL_MAX_3D_TEXTURE_SIZE, &maximumTextureSize );

		  osg::notify(osg::NOTICE)<<"Max texture size="<<maximumTextureSize<<std::endl;
	  }

	  OpenThreads::Mutex  mutex;
	  bool                supported;
	  std::string         errorMessage;
	  GLint               maximumTextureSize;
};

osgVolumeImp::osgVolumeImp(void)
{
}


osgVolumeImp::~osgVolumeImp(void)
{
}

void osgVolumeImp::InputImage(osg::ImageList& pImages, int maximumTextureSize )
{
	int s_maximumTextureSize(maximumTextureSize), 
		t_maximumTextureSize(maximumTextureSize), 
		r_maximumTextureSize(maximumTextureSize);
	size_t count = pImages.size();

	if (count > 0)
	{
		osg::Image* image = createTexture3D(pImages, 0, s_maximumTextureSize, t_maximumTextureSize, r_maximumTextureSize, false);
		if (image)
		{
			images.push_back(image);
		}
	}
}

void osgVolumeImp::GetVolume( osg::Group* pVolumeNode )
{
	osg::ref_ptr<osg::TransferFunction1D> transferFunction;
	std::string tranferFunctionFile;

	float alphaFunc=0.02f;

	ShadingModel shadingModel = Standard;

	float xSize=0.0f, ySize=0.0f, zSize=0.0f;

	enum RescaleOperation
	{
		NO_RESCALE,
		RESCALE_TO_ZERO_TO_ONE_RANGE,
		SHIFT_MIN_TO_ZERO
	};

	RescaleOperation rescaleOperation = RESCALE_TO_ZERO_TO_ONE_RANGE;

	bool resizeToPowerOfTwo = false;

	unsigned int numComponentsDesired = 0;

	bool useShader = true;

	bool gpuTransferFunction = true;

	double sampleDensityWhenMoving = 0.0;


	double sequenceLength = 10.0;

	std::string endian, raw_filename;


	Images::iterator sizeItr = images.begin();
	int image_s = (*sizeItr)->s();
	int image_t = (*sizeItr)->t();
	int image_r = (*sizeItr)->r();
	++sizeItr;


	osg::ref_ptr<osgVolume::ImageDetails> details = dynamic_cast<osgVolume::ImageDetails*>(images.front()->getUserData());
	osg::ref_ptr<osg::RefMatrix> matrix = details ? details->getMatrix() : dynamic_cast<osg::RefMatrix*>(images.front()->getUserData());

	if (!matrix)
	{
		if (xSize==0.0) xSize = static_cast<float>(image_s);
		if (ySize==0.0) ySize = static_cast<float>(image_t);
		if (zSize==0.0) zSize = static_cast<float>(image_r);

		matrix = new osg::RefMatrix(xSize, 0.0,   0.0,   0.0,
			0.0,   ySize, 0.0,   0.0,
			0.0,   0.0,   zSize, 0.0,
			0.0,   0.0,   0.0,   1.0);
	}

	osg::ref_ptr<osg::Image> image_3d = 0;

	if (images.size()==1)
	{
		osg::notify(osg::NOTICE)<<"Single image "<<images.size()<<" volumes."<<std::endl;
		image_3d = images.front();
	}
	else
	{
		osg::notify(osg::NOTICE)<<"Creating sequence of "<<images.size()<<" volumes."<<std::endl;

		osg::ref_ptr<osg::ImageSequence> imageSequence = new osg::ImageSequence;
		imageSequence->setLength(sequenceLength);
		image_3d = imageSequence.get();
		for(Images::iterator itr = images.begin();
			itr != images.end();
			++itr)
		{
			imageSequence->addImage(itr->get());
		}
		imageSequence->play();
	}

	osg::ref_ptr<osgVolume::Volume> volume = new osgVolume::Volume;
	osg::ref_ptr<osgVolume::VolumeTile> tile = new osgVolume::VolumeTile;
	volume->addChild(tile.get());

	osg::ref_ptr<osgVolume::ImageLayer> layer = new osgVolume::ImageLayer(image_3d.get());

	switch(rescaleOperation)
	{
	case(NO_RESCALE):
		break;

	case(RESCALE_TO_ZERO_TO_ONE_RANGE):
		{
			layer->rescaleToZeroToOneRange();
			break;
		}
	case(SHIFT_MIN_TO_ZERO):
		{
			layer->translateMinToZero();
			break;
		}
	};

	matrix = new osg::RefMatrix(
		1,     0.0,   0.0,   0.0,
		0.0,   1,     0.0,   0.0,
		0.0,   0.0,   1,     0.0,
		0.0,   0.0,   0.0,  1);

	layer->setLocator(new osgVolume::Locator(*matrix));

	tile->setLocator(new osgVolume::Locator(*matrix));

	tile->setLayer(layer.get());

	tile->setEventCallback(new osgVolume::PropertyAdjustmentCallback());

	if (useShader)
	{

		osgVolume::SwitchProperty* sp = new osgVolume::SwitchProperty;
		sp->setActiveProperty(0);

		osgVolume::AlphaFuncProperty* ap = new osgVolume::AlphaFuncProperty(alphaFunc);
		osgVolume::SampleDensityProperty* sd = new osgVolume::SampleDensityProperty(0.005);
		osgVolume::SampleDensityWhenMovingProperty* sdwm = sampleDensityWhenMoving!=0.0 ? new osgVolume::SampleDensityWhenMovingProperty(sampleDensityWhenMoving) : 0;
		osgVolume::TransparencyProperty* tp = new osgVolume::TransparencyProperty(1.0);
		osgVolume::TransferFunctionProperty* tfp = transferFunction.valid() ? new osgVolume::TransferFunctionProperty(transferFunction.get()) : 0;

		{
			// Standard
			osgVolume::CompositeProperty* cp = new osgVolume::CompositeProperty;
			cp->addProperty(ap);
			cp->addProperty(sd);
			cp->addProperty(tp);
			if (sdwm) cp->addProperty(sdwm);
			if (tfp) cp->addProperty(tfp);

			sp->addProperty(cp);
		}

		{
			// Light
			osgVolume::CompositeProperty* cp = new osgVolume::CompositeProperty;
			cp->addProperty(ap);
			cp->addProperty(sd);
			cp->addProperty(tp);
			cp->addProperty(new osgVolume::LightingProperty);
			if (sdwm) cp->addProperty(sdwm);
			if (tfp) cp->addProperty(tfp);

			sp->addProperty(cp);
		}

		{
			// Isosurface
			osgVolume::CompositeProperty* cp = new osgVolume::CompositeProperty;
			cp->addProperty(sd);
			cp->addProperty(tp);
			cp->addProperty(new osgVolume::IsoSurfaceProperty(alphaFunc));
			if (sdwm) cp->addProperty(sdwm);
			if (tfp) cp->addProperty(tfp);

			sp->addProperty(cp);
		}

		{
			// MaximumIntensityProjection
			osgVolume::CompositeProperty* cp = new osgVolume::CompositeProperty;
			cp->addProperty(ap);
			cp->addProperty(sd);
			cp->addProperty(tp);
			cp->addProperty(new osgVolume::MaximumIntensityProjectionProperty);
			if (sdwm) cp->addProperty(sdwm);
			if (tfp) cp->addProperty(tfp);

			sp->addProperty(cp);
		}

		switch(shadingModel)
		{
		case(Standard):                     sp->setActiveProperty(0); break;
		case(Light):                        sp->setActiveProperty(1); break;
		case(Isosurface):                   sp->setActiveProperty(2); break;
		case(MaximumIntensityProjection):   sp->setActiveProperty(3); break;
		}
		layer->addProperty(sp);


		tile->setVolumeTechnique(new osgVolume::RayTracedTechnique);
	}
	else
	{
		layer->addProperty(new osgVolume::AlphaFuncProperty(alphaFunc));
		tile->setVolumeTechnique(new osgVolume::FixedFunctionTechnique);
	}


	if (volume.valid())
	{
		pVolumeNode->addChild(volume.get());
	}
}
