#ifndef RADARONVTK_H
#define RADARONVTK_H

#include "radaronvtk_global.h"
#include "QT_Include.h"

#include <QMainWindow>
#include <QApplication>

#include <QVTKWidget.h>
#include <vtkSmartPointer.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkColorTransferFunction.h>
#include <vtkPiecewiseFunction.h>
#include <vtkSmartVolumeMapper.h>
#include <vtkColorTransferFunction.h>
#include <vtkPiecewiseFunction.h>
#include <vtkVolumeProperty.h>
#include <vtkMetaImageReader.h>
#include <vtkVolume16Reader.h>
#include <vtkNew.h>
#include <vtkNrrdReader.h>
#include <vtkImageShiftScale.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkRendererCollection.h>
#include <vtkPNGReader.h>
#include <vtkPlaneWidget.h>
#include <vtkPlane.h>
#include <vtkCutter.h>


class vtkWidgetCall : public vtkCommand
{
public:

	static vtkWidgetCall *New()
	{
		return new vtkWidgetCall;
	}
public:
	virtual void Execute(vtkObject *caller, unsigned long eventId, void *callData)
	{
		vtkPlaneWidget *pWidget = vtkPlaneWidget::SafeDownCast(caller);
		pWidget->GetPlane(pPlane);
	}
public:
	vtkPlane *pPlane ;

};

class RADARONVTK_EXPORT RadarOnVTK : public QWidget
{
public:
	RadarOnVTK(QWidget* parent = NULL, Qt::WFlags f = 0);
	~RadarOnVTK();
	void OnFileOpen(const std::string& filename);
	void AddMenuItem();
public Q_SLOTS:
	virtual void setVisible(bool visible);
protected:
	virtual bool winEvent(MSG *m, long *result);
private:
	QString filename;
	QStringList m_imgList;
	QVTKWidget* m_pVTKWidget;

};

#endif // RADARONVTK_H
