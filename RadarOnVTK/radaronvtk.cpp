#define vtkRenderingCore_AUTOINIT 4(vtkInteractionStyle,vtkRenderingFreeType,vtkRenderingFreeTypeOpenGL,vtkRenderingOpenGL)
#define vtkRenderingVolume_AUTOINIT 1(vtkRenderingVolumeOpenGL)
#include <QMdiSubWindow>
#include <QGridLayout>
#include <vtkImageShiftScale.h>
#include <vtkProperty.h>
#include <vtkPolyDataMapper.h>
#include <vtkStringArray.h>
#include <vtkImageData.h>
#include "vtkImageData.h"
#include <vtkImageActor.h>
#include <vtkOutlineFilter.h>
#include <vtkCellPicker.h>
#include <vtkCallbackCommand.h>
#include <vtkImageMapToColors.h>
#include <vtkImageFlip.h>
#include "vtkImageResample.h"
#include "vtkImagePlaneWidget.h"
#include "VolumeRadarSingleVTKProvider.h"
#include "vtkVolumeProperty.h"
#include "vtkVolumeRayCastCompositeFunction.h"
#include "vtkVolumeRayCastMapper.h"
#include "radaronvtk.h"

#define IDM_INMDI_VTK 0x0019

RadarOnVTK::RadarOnVTK( QWidget* parent /*= NULL*/, Qt::WFlags f /*= 0*/ ):QWidget(parent)
{
	setAttribute(Qt::WA_DeleteOnClose);
	QGridLayout* gLayout = new QGridLayout(this);
	gLayout->setContentsMargins(0,0,0,0);
	setLayout(gLayout);
	m_pVTKWidget = new QVTKWidget(this);
	gLayout->addWidget(m_pVTKWidget,0,0);
}

RadarOnVTK::~RadarOnVTK()
{

}
#if 0
void RadarOnVTK::OnFileOpen(const std::string& filename)
{
	auto m_pRender1 = vtkSmartPointer<vtkRenderer>::New(); //三维视图; //三维视图
	auto m_pRender2 = vtkSmartPointer<vtkRenderer>::New(); //水平剖面视图; //水平剖面视图
	auto m_pRender3 = vtkSmartPointer<vtkRenderer>::New(); //垂直南北向剖面视图 ; //垂直南北向剖面视图 
	auto m_pRender4 = vtkSmartPointer<vtkRenderer>::New(); //垂直东西向剖面视图; //垂直东西向剖面视图

	m_pVTKWidget->GetRenderWindow()->AddRenderer(m_pRender1);

	m_pRender1->SetViewport(0,0.5,0.5,1);
	m_pRender2->SetViewport(0,0,.5,.5);
	m_pRender3->SetViewport(.5,0,1,.5);
	m_pRender4->SetViewport(.5,0.5,1,1);

	m_pRender1->SetBackground(.1,.1,.1);
	m_pRender2->SetBackground(.3,.3,.3);
	m_pRender3->SetBackground(.4,.4,.4);
	m_pRender4->SetBackground(.2,.2,.2);

	auto interactor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
	interactor->SetRenderWindow(m_pVTKWidget->GetRenderWindow());
	auto style = vtkSmartPointer<vtkInteractorStyleTrackballCamera>::New();
	interactor->SetInteractorStyle(style);
	RadarSingleVTKProvider * provider = new RadarSingleVTKProvider;
	vtkImageData* imagedata = provider->LoadFile(filename);

	//_设定标量值的颜色属性和透明度
	vtkColorTransferFunction *colorTransferFunction= vtkColorTransferFunction::New(); 
	QHash<int,QVector4D> ::iterator iter;
	vtkPiecewiseFunction *opacityTransferFunction = vtkPiecewiseFunction::New(); 
	QHash<int,QVector4D> *colorHash = provider->GetColorTable();
	for (iter= colorHash->begin() ; iter != colorHash->end(); ++iter )
	{
		int key = iter.key();
		QVector4D tempcolor = iter.value();
		colorTransferFunction->AddRGBPoint(key+5, tempcolor.x(),tempcolor.y(),tempcolor.z()); 
		opacityTransferFunction->AddPoint(key+5,0.8);
	}
	opacityTransferFunction->AddPoint(254,0.0);
	colorTransferFunction->AddRGBPoint(254, 0.0,0.0,0.0); 

	//_设定体数据的属性:的不透明性和颜色值映射标量值
	vtkVolumeProperty *volumeProperty = vtkVolumeProperty::New(); 
	volumeProperty->SetColor(colorTransferFunction); 
	volumeProperty->SetScalarOpacity(opacityTransferFunction); 
	volumeProperty->SetInterpolationTypeToNearest();


	//绘制方法:体射线投射
	vtkVolumeRayCastCompositeFunction *compositeFunction=vtkVolumeRayCastCompositeFunction::New();
	//_体数据映射器
	vtkVolumeRayCastMapper *volumeMapper=vtkVolumeRayCastMapper::New();
	volumeMapper->SetInputData((vtkImageData *)imagedata);
	volumeMapper->SetVolumeRayCastFunction(compositeFunction);

	//创建一个 vtkVolume（vtkProp3D 的子类，类似于_vtkActor）对象来处理被映射的体数据和体属性数据
	vtkVolume *volume=vtkVolume::New();
	volume->SetMapper(volumeMapper);
	volume->SetProperty(volumeProperty);
	m_pRender1->AddVolume(volume);

	// An outline provides context around the data.
	//
	auto outlineData =
		vtkSmartPointer<vtkOutlineFilter>::New();
	outlineData->SetInputData(imagedata);
	outlineData->Update();

	auto mapOutline =
		vtkSmartPointer<vtkPolyDataMapper>::New();
	mapOutline->SetInputConnection(outlineData->GetOutputPort());

	auto outline =
		vtkSmartPointer<vtkActor>::New();
	outline->SetMapper(mapOutline);
	outline->GetProperty()->SetColor(0,0,0);
	m_pRender1->AddActor(outline);

	//水平剖面X方向
	vtkPlaneWidget *pWidgetX = vtkPlaneWidget::New();//剖面1;//剖面1
	vtkPlaneWidget *pWidgetY = vtkPlaneWidget::New();//剖面2;//剖面2
	vtkPlaneWidget *pWidgetZ = vtkPlaneWidget::New();//剖面3;//剖面3

	vtkSmartPointer<vtkPlane>pPlaneX= vtkSmartPointer<vtkPlane> ::New(); //挡板1 ; //挡板1
	vtkSmartPointer<vtkPlane>pPlaneY= vtkSmartPointer<vtkPlane> ::New(); //挡板2 ; //挡板2
	vtkSmartPointer<vtkPlane>pPlaneZ= vtkSmartPointer<vtkPlane> ::New(); //挡板3 ; //挡板3
	pWidgetX->SetInteractor(interactor);//交互器，过一会儿设置
	pWidgetX->SetInputData(imagedata);
	pWidgetX->SetResolution(100);
	pWidgetX->SetPlaceFactor(1.0);
	pWidgetX->SetHandleSize(0);
	pWidgetX->GetPlaneProperty()->SetOpacity(0.8);
	pWidgetX->GetPlaneProperty()->SetColor(.9, .4, .4);
	//pWidgetX->GetHandleProperty()->SetColor(0, .4, .7);
	//pWidgetX->GetHandleProperty()->SetLineWidth(1.5);
	pWidgetX->NormalToXAxisOn();
	//pWidgetX->SetRepresentationToOutline();
	pWidgetX->SetCenter(imagedata->GetCenter());
	pWidgetX->PlaceWidget();
	pWidgetX->On();
	pPlaneX->SetOrigin(imagedata->GetCenter());

	vtkCutter *pCutX = vtkCutter::New();
	pCutX->SetCutFunction(pPlaneX);
	pCutX->SetInputData(imagedata);   
	pCutX->Update();

	vtkPolyDataMapper *selectMapperX = vtkPolyDataMapper::New();
	selectMapperX->SetInputConnection(pCutX->GetOutputPort());
	vtkActor *pActorX = vtkActor::New(); 
	pActorX->SetMapper(selectMapperX);
	m_pRender2->AddActor(pActorX);

	vtkWidgetCall *pCallX = vtkWidgetCall::New();
	pCallX->pPlane = pPlaneX; 
	pWidgetX->AddObserver(vtkCommand::EndInteractionEvent, pCallX);


	//垂直剖面Y方向
	pWidgetY->SetInteractor(interactor);//交互器，过一会儿设置
	pWidgetY->SetInputData(imagedata);
	pWidgetY->SetResolution(100);
	pWidgetY->SetPlaceFactor(1.0);
	pWidgetY->GetPlaneProperty()->SetOpacity(0.8);
	pWidgetY->SetHandleSize(0);
	pWidgetY->GetPlaneProperty()->SetColor(.9, .4, .4);
	//pWidgetY->GetHandleProperty()->SetColor(0, .4, .7);
	//pWidgetY->GetHandleProperty()->SetLineWidth(5.5);
	pWidgetY->NormalToYAxisOn();
	//pWidgetY->SetRepresentationToOutline();
	pWidgetY->SetCenter(imagedata->GetCenter());
	pWidgetY->PlaceWidget();
	pWidgetY->On();
	pPlaneY->SetOrigin(imagedata->GetCenter());

	vtkCutter *pCutY = vtkCutter::New();
	pCutY->SetCutFunction(pPlaneY);
	pCutY->SetInputData(imagedata);   
	pCutY->Update();

	vtkPolyDataMapper *selectMapperY = vtkPolyDataMapper::New();
	selectMapperY->SetInputConnection(pCutY->GetOutputPort());
	vtkActor *pActorY= vtkActor::New(); 
	pActorY->SetMapper(selectMapperY);
	m_pRender3->AddActor(pActorY);

	vtkWidgetCall *pCallY = vtkWidgetCall::New();
	pCallY->pPlane = pPlaneY; 
	pWidgetY->AddObserver(vtkCommand::EndInteractionEvent, pCallY);


	//垂直剖面Z方向
	pWidgetZ->SetInteractor(interactor);//交互器，过一会儿设置
	pWidgetZ->SetInputData(imagedata);
	pWidgetZ->SetResolution(100);
	pWidgetZ->GetPlaneProperty()->SetColor(.9, .4, .4);
	pWidgetZ->SetHandleSize(0);
	//pWidgetZ->GetHandleProperty()->SetColor(0, .4, .7);
	//pWidgetZ->GetHandleProperty()->SetLineWidth(5.5);
	pWidgetZ->NormalToZAxisOn();
	//pWidgetZ->SetRepresentationToOutline();
	pWidgetZ->SetCenter(imagedata->GetCenter());
	pWidgetZ->PlaceWidget();
	pWidgetZ->On();
	pPlaneZ->SetOrigin(imagedata->GetCenter());

	vtkCutter *pCut3 = vtkCutter::New();
	pCut3->SetCutFunction(pPlaneZ);
	pCut3->SetInputData(imagedata);   
	pCut3->Update();

	vtkPolyDataMapper *selectMapperZ = vtkPolyDataMapper::New();
	selectMapperZ->SetInputConnection(pCut3->GetOutputPort());
	vtkActor *pActorZ= vtkActor::New(); 
	pActorZ->SetMapper(selectMapperZ);
	m_pRender4->AddActor(pActorZ);

	vtkWidgetCall *pCallZ = vtkWidgetCall::New();
	pCallZ->pPlane = pPlaneZ; 
	pWidgetZ->AddObserver(vtkCommand::EndInteractionEvent, pCallZ);
	m_pVTKWidget->GetRenderWindow()->AddRenderer(m_pRender2);
	m_pVTKWidget->GetRenderWindow()->AddRenderer(m_pRender3);
	m_pVTKWidget->GetRenderWindow()->AddRenderer(m_pRender4);
	interactor->Initialize();

}
#endif

#if 1//vtkimagewidget正交剖面，无法映射颜色
void RadarOnVTK::OnFileOpen(const std::string& filename)
{
	auto m_pRender1 = vtkSmartPointer<vtkRenderer>::New(); //三维视图; //三维视图
	auto m_pRender2 = vtkSmartPointer<vtkRenderer>::New(); //水平剖面视图; //水平剖面视图
	auto m_pRender3 = vtkSmartPointer<vtkRenderer>::New(); //垂直南北向剖面视图 ; //垂直南北向剖面视图 
	auto m_pRender4 = vtkSmartPointer<vtkRenderer>::New(); //垂直东西向剖面视图; //垂直东西向剖面视图

	m_pVTKWidget->GetRenderWindow()->AddRenderer(m_pRender1);

	m_pRender1->SetViewport(0,0.5,0.5,1);
	m_pRender2->SetViewport(0,0,.5,.5);
	m_pRender3->SetViewport(.5,0,1,.5);
	m_pRender4->SetViewport(.5,0.5,1,1);

	m_pRender1->SetBackground(.1,.1,.1);
	m_pRender2->SetBackground(.3,.3,.3);
	m_pRender3->SetBackground(.4,.4,.4);
	m_pRender4->SetBackground(.2,.2,.2);


	auto interactor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
	interactor->SetRenderWindow(m_pVTKWidget->GetRenderWindow());
	auto style = vtkSmartPointer<vtkInteractorStyleTrackballCamera>::New();
	interactor->SetInteractorStyle(style);

	//m_pReader->SetFileDimensionality(3);//设置显示图像的维数
	////reader->SetDataScalarType(VTK_UNSIGNED_CHAR);
	////m_pReader->SetFilePrefix("E:\\IDP\\Release\\Bin\\png\\");//一张，多张SetFilePrefix
	////m_pReader->SetFilePattern("radar%d.png");
	//auto names = vtkSmartPointer<vtkStringArray>::New();
	//foreach(QString name,m_imgList)
	//{
	//	names->InsertNextValue(vtkStdString(name.toStdString().c_str()));

	//}
	//m_pReader->SetFileNames(names);
	//m_pReader->SetDataExtent(0,301,0,301,0,18);
	//m_pReader->SetDataSpacing(50.0, 50.0,50.0);
	//m_pReader->SetFileNameSliceSpacing(1);
	//m_pReader->SetDataOrigin(0.0,0.0,0.0);
	//m_pReader->SetDataScalarTypeToUnsignedChar();
	//m_pReader->SetDataByteOrderToLittleEndian();
	//double range[2];
	//m_pReader->GetOutput()->GetScalarRange(range);
	//auto m_pShift = vtkSmartPointer<vtkImageShiftScale>::New();

	//m_pShift->SetShift(-1.0*range[0]);
	//m_pShift->SetOutputScalarTypeToUnsignedChar();
	//m_pShift->SetInputConnection(m_pReader->GetOutputPort());
	//m_pShift->ReleaseDataFlagOff();
	//m_pShift->Update();

	//auto pColor = vtkSmartPointer<vtkColorTransferFunction>::New(); //第二步:color
	//pColor->AddRGBSegment(0, 1, 1, 1, 255, 1, 1, 1);

	//auto pPiceFun = vtkSmartPointer<vtkPiecewiseFunction>::New();
	//pPiceFun->AddPoint(0.0,  0.0);
	//pPiceFun->AddPoint(36.0,  0.125);
	//pPiceFun->AddPoint(72.0,  0.25);
	//pPiceFun->AddPoint(108.0, 0.375);
	//pPiceFun->AddPoint(144.0, 0.5);
	//pPiceFun->AddPoint(180.0, 0.625);
	//pPiceFun->AddPoint(216.0, 0.75);
	//pPiceFun->AddPoint(255.0, 0.875);


	//auto colorTransferFunction = vtkSmartPointer<vtkColorTransferFunction>::New();
	//colorTransferFunction->AddRGBPoint(0.0,  0.0, 0.0, 0.0);
	//colorTransferFunction->AddRGBPoint(36.0, 1.0, 0.0, 0.0);
	//colorTransferFunction->AddRGBPoint(72.0, 1.0, 1.0, 0.0);
	//colorTransferFunction->AddRGBPoint(108.0, 0.0, 1.0, 0.0);
	//colorTransferFunction->AddRGBPoint(144.0, 0.0, 1.0, 1.0);
	//colorTransferFunction->AddRGBPoint(180.0, 0.0, 0.0, 1.0);
	//colorTransferFunction->AddRGBPoint(216.0, 1.0, 0.0, 1.0);
	//colorTransferFunction->AddRGBPoint(255.0, 1.0, 1.0, 1.0);

	//auto pProperty = vtkSmartPointer<vtkVolumeProperty>::New();//设置属性
	//pProperty->SetColor(colorTransferFunction);
	//pProperty->SetScalarOpacity(pPiceFun);
	//pProperty->IndependentComponentsOff();
	//pProperty->SetInterpolationTypeToLinear();
	//pProperty->ShadeOff();

	//auto volumeMapper = vtkSmartPointer<vtkSmartVolumeMapper>::New();
	//volumeMapper->SetRequestedRenderMode(vtkSmartVolumeMapper::GPURenderMode);
	//volumeMapper->SetInputConnection(m_pReader->GetOutputPort());

	//auto pVolume = vtkSmartPointer<vtkVolume>::New();
	//pVolume->SetProperty(pProperty);
	//pVolume->SetMapper(volumeMapper); //生成体对象
	////m_pRender1->AddVolume(pVolume); //增加体数据到渲染器中渲染
	RadarSingleVTKProvider * provider = new RadarSingleVTKProvider;
	vtkImageData* imagedata = provider->LoadFile(filename);

	//_设定标量值的颜色属性和透明度
	vtkColorTransferFunction *colorTransferFunction= vtkColorTransferFunction::New(); 
	QHash<int,QVector4D> ::iterator iter;
	vtkPiecewiseFunction *opacityTransferFunction = vtkPiecewiseFunction::New(); 
	QHash<int,QVector4D> *colorHash = provider->GetColorTable();
	for (iter= colorHash->begin() ; iter != colorHash->end(); ++iter )
	{
		int key = iter.key();
		QVector4D tempcolor = iter.value();
		colorTransferFunction->AddRGBPoint(key+5, tempcolor.x(),tempcolor.y(),tempcolor.z()); 
		opacityTransferFunction->AddPoint(key+5,0.8);
	}
	opacityTransferFunction->AddPoint(254,0.0);
	colorTransferFunction->AddRGBPoint(254, 0.0,0.0,0.0); 

	//_设定体数据的属性:的不透明性和颜色值映射标量值
	vtkVolumeProperty *volumeProperty = vtkVolumeProperty::New(); 
	volumeProperty->SetColor(colorTransferFunction); 
	volumeProperty->SetScalarOpacity(opacityTransferFunction); 
	volumeProperty->SetInterpolationTypeToNearest();


	//绘制方法:体射线投射
	vtkVolumeRayCastCompositeFunction *compositeFunction=vtkVolumeRayCastCompositeFunction::New();
	//_体数据映射器
	vtkVolumeRayCastMapper *volumeMapper=vtkVolumeRayCastMapper::New();
	volumeMapper->SetInputData((vtkImageData *)imagedata);
	volumeMapper->SetVolumeRayCastFunction(compositeFunction);

	//创建一个 vtkVolume（vtkProp3D 的子类，类似于_vtkActor）对象来处理被映射的体数据和体属性数据
	vtkVolume *volume=vtkVolume::New();
	volume->SetMapper(volumeMapper);
	volume->SetProperty(volumeProperty);
	m_pRender1->AddVolume(volume);

	// An outline provides context around the data.
	//
	auto outlineData =
		vtkSmartPointer<vtkOutlineFilter>::New();
	outlineData->SetInputData(imagedata);
	outlineData->Update();

	auto mapOutline =
		vtkSmartPointer<vtkPolyDataMapper>::New();
	mapOutline->SetInputConnection(outlineData->GetOutputPort());

	auto outline =
		vtkSmartPointer<vtkActor>::New();
	outline->SetMapper(mapOutline);
	outline->GetProperty()->SetColor(0,0,0);
	m_pRender1->AddActor(outline);

	vtkCellPicker* picker = vtkCellPicker::New();
	picker->SetTolerance(0.005);

	auto pImageWidgetX = vtkSmartPointer<vtkImagePlaneWidget>::New();
	pImageWidgetX->SetInteractor(interactor);
	pImageWidgetX->SetKeyPressActivationValue('x');
	pImageWidgetX->SetPicker(picker);
	pImageWidgetX->SetInputData((vtkImageData *)imagedata);
	pImageWidgetX->GetPlaneProperty()->SetColor(1,0,0);
	pImageWidgetX->SetPlaneOrientationToXAxes();
	pImageWidgetX->DisplayTextOn();
	pImageWidgetX->UpdatePlacement();
	pImageWidgetX->PlaceWidget();
	pImageWidgetX->On();

	// Add a 2D image to test the GetReslice method
	//
	auto colorMapX = vtkSmartPointer<vtkImageMapToColors>::New();
	colorMapX->PassAlphaToOutputOff();
	colorMapX->SetActiveComponent(2);
	colorMapX->SetOutputFormatToRGB();
	colorMapX->SetInputData(pImageWidgetX->GetResliceOutput());
	//colorMapX->SetLookupTable( pImageWidgetX->GetColorMap());

	auto imageActorX = vtkSmartPointer<vtkImageActor>::New();
	imageActorX->PickableOn();
	imageActorX->SetInputData(colorMapX->GetOutput());
	m_pRender2->AddActor(imageActorX);

	auto pImageWidgetY = vtkSmartPointer<vtkImagePlaneWidget>::New();
	pImageWidgetY->SetInteractor(interactor);
	pImageWidgetY->SetKeyPressActivationValue('y');
	pImageWidgetY->SetPicker(picker);
	pImageWidgetY->SetInputData((vtkImageData *)imagedata);
	pImageWidgetY->GetPlaneProperty()->SetColor(0,1,0);
	pImageWidgetY->SetPlaneOrientationToYAxes();
	pImageWidgetY->DisplayTextOn();
	pImageWidgetY->UpdatePlacement();
	pImageWidgetY->PlaceWidget();
	pImageWidgetY->On();

	auto colorMapY = vtkSmartPointer<vtkImageMapToColors>::New();
	colorMapY->PassAlphaToOutputOff();
	colorMapY->SetActiveComponent(2);
	colorMapY->SetOutputFormatToLuminance();
	colorMapY->SetInputData(pImageWidgetY->GetResliceOutput());
	//colorMapY->SetLookupTable(pImageWidgetY->GetColorMap());

	auto imageActorY = vtkSmartPointer<vtkImageActor>::New();
	imageActorY->PickableOn();
	imageActorY->SetInputData(colorMapY->GetOutput());
	m_pRender3->AddActor(imageActorY);

	vtkImagePlaneWidget *pImageWidgetZ = vtkImagePlaneWidget::New();
	pImageWidgetZ->SetInteractor(interactor);
	pImageWidgetZ->SetKeyPressActivationValue('z');
	pImageWidgetZ->SetPicker(picker);
	pImageWidgetZ->SetInputData((vtkImageData *)imagedata);
	pImageWidgetZ->GetPlaneProperty()->SetColor(0,0,1);
	pImageWidgetZ->SetPlaneOrientationToZAxes();
	pImageWidgetZ->DisplayTextOn();
	pImageWidgetZ->UpdatePlacement();
	pImageWidgetZ->PlaceWidget();
	pImageWidgetZ->On();

	vtkImageFlip *pFX = vtkImageFlip::New();
	pFX->SetInputData(pImageWidgetX->GetResliceOutput());
	pFX->SetFilteredAxis(1);                          //翻转，如果不加这几句代码，图片就是翻着的

	vtkImageFlip *pFY = vtkImageFlip::New();
	pFY->SetInputData(pFX->GetOutput());
	pFY->SetFilteredAxis(0);
	vtkImageResample *pRes = vtkImageResample::New();
	pRes->SetInputData(pFY->GetOutput());
	pRes->SetInterpolationModeToLinear();
	pRes->SetAxisMagnificationFactor(1, 3);
	auto imageActorx = vtkSmartPointer<vtkImageActor>::New();
	imageActorx->PickableOn();
	imageActorx->SetInputData(pRes->GetOutput());
	m_pRender2->AddActor(imageActorx);
	m_pRender3->AddActor(imageActorx);
	m_pRender4->AddActor(imageActorx);
	//vtkImageViewer *pV = vtkImageViewer::New();
	//pV->SetupInteractor(iren);
	//pV->SetInput(pRes->GetOutput());

	//pV->SetSize(512, 512);
	//pV->GetActor2D()->SetDisplayPosition(0, 200);


	//vtkImageMapToColors *colorMap1 = vtkImageMapToColors::New();
	//colorMap1->PassAlphaToOutputOff(); //use in RGBA
	//colorMap1->SetActiveComponent( 0 );
	//colorMap1->SetOutputFormatToLuminance();
	//colorMap1->SetInputData( (vtkDataSet*)pImageWidget0->GetResliceOutput() );
	//colorMap1->SetLookupTable( (vtkScalarsToColors *)pImageWidget0->GetLookupTable() );
	//vtkImageActor * imageActor1 = vtkImageActor::New();
	//imageActor1->PickableOff();
	//imageActor1->SetInputData( colorMap1->GetOutput() );

	//vtkImageMapToColors *colorMap2 = vtkImageMapToColors::New();
	//colorMap2->PassAlphaToOutputOff(); 
	//colorMap2->SetActiveComponent( 0 ); // for muti-component
	//colorMap2->SetOutputFormatToLuminance();
	//colorMap2->SetInputData( (vtkDataSet*)pImageWidget1->GetResliceOutput() );
	//colorMap2->SetLookupTable( (vtkScalarsToColors *)pImageWidget1->GetLookupTable() );
	//vtkImageActor * imageActor2 = vtkImageActor::New();
	//imageActor2->PickableOff();
	//imageActor2->SetInputData( colorMap2->GetOutput() );

	//vtkImageMapToColors *colorMap3 = vtkImageMapToColors::New();
	//colorMap3->PassAlphaToOutputOff();
	//colorMap3->SetActiveComponent( 0 );
	//colorMap3->SetOutputFormatToLuminance();
	//colorMap3->SetInputData( (vtkDataSet*)pImageWidget2->GetResliceOutput() );
	//colorMap3->SetLookupTable( (vtkScalarsToColors *)pImageWidget2->GetLookupTable());
	//colorMap3->SetLookupTable(planeWidgetX->GetLookupTable());
	//vtkImageActor *imageActor3 = vtkImageActor::New();
	//imageActor3->PickableOff();
	//imageActor3->SetInputData( colorMap3->GetOutput() );
	auto callback = vtkSmartPointer<vtkCallbackCommand>::New();
	callback->SetClientData((void *)m_pRender1);
	m_pVTKWidget->GetRenderWindow()->GetInteractor()->AddObserver(vtkCommand::LeftButtonPressEvent, callback, 1.0);
	m_pVTKWidget->GetRenderWindow()->GetInteractor()->AddObserver(vtkCommand::MouseMoveEvent, callback, 1.0);
	m_pVTKWidget->GetRenderWindow()->GetInteractor()->AddObserver(vtkCommand::LeftButtonReleaseEvent, callback, 1.0);

	m_pVTKWidget->GetRenderWindow()->AddRenderer(m_pRender2);
	m_pVTKWidget->GetRenderWindow()->AddRenderer(m_pRender3);
	m_pVTKWidget->GetRenderWindow()->AddRenderer(m_pRender4);
	interactor->Initialize();
}
#endif

void RadarOnVTK::AddMenuItem()
{
	QMdiSubWindow* subW = (QMdiSubWindow*)this->property("ParentWidget").toLongLong();
	if (subW)
	{
		HMENU hMenu = ::GetSystemMenu(winId(), FALSE);
		if (hMenu != NULL)
		{
			::AppendMenuA(hMenu, MF_SEPARATOR, 0, 0);
			::AppendMenuA(hMenu, MF_STRING, IDM_INMDI_VTK, "窗口锁定");
		}
	}
}


void RadarOnVTK::setVisible( bool visible )
{
	if (visible)
	{
		AddMenuItem();
	}
	QWidget::setVisible(visible);
}

bool RadarOnVTK::winEvent(MSG *m, long *result)
{
	if (m->message == WM_SYSCOMMAND)
	{
		if ((m->wParam) == IDM_INMDI_VTK)
		{
			*result = 0;
			QMdiSubWindow* subW = (QMdiSubWindow*)this->property("ParentWidget").toLongLong();
			if (subW)
			{
				this->setWindowFlags(Qt::Widget);
				subW->setWidget(this);
				subW->showMaximized();
			}
			return (true);
		}
	}
	return (false);
}