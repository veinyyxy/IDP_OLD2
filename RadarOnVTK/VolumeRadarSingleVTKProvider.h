#pragma once
#ifndef RADARSINGLEVTKPROVIDER_H
#define RADARSINGLEVTKPROVIDER_H

#include "radaronvtk_global.h"
#include "QT_Include.h"

#include "RadReadColor_XML.h"
#include "RadReadStationFile.h"

#include "../GDataTypes/GData.h"
#include "../GDataTypes/GDataFiles.h"
#include "../GDataTypes/cDrawProduct.h"
#include "VTKInterpolationAlgorithm.h"
#include "RadProjTransform.h"
class  RADARONVTK_EXPORT RadarSingleVTKProvider
{
public:
	RadarSingleVTKProvider();
	~RadarSingleVTKProvider();
	vtkImageData * LoadFile(const std::string& filename);
	QHash<int ,QVector4D> *GetColorTable(){return &m_pColorValueIndexHash;};
private:
	bool  InitColorTable();//��ʼ����ɫ��
	bool DrawCube();
	static int GetColorIndexKey(int productcode,double fstart);
private:
	GenerationData*        gData;//��������
	cDrawProduct*          pDrawProduct;//�����㷨����
	ProductBase*    pSingleGdata;
	goto_gis::RadProjTransform *proj;
	CRadReadColor_XML      xml_reader;
	QHash<int ,QVector4D> m_pColorValueIndexHash;
	vtkPolyData *cube;
	vtkLookupTable *pColorTable;
	vtkPolyDataMapper *cubeMapper;
	vtkActor *cubeActor;
	vtkCamera *camera;
	vtkRenderer *renderer;
	vtkRenderWindow *renWin;
	vtkRenderWindowInteractor *iren;
	QString                 m_fileName;
	P_COLORTABLE_VECTOR     m_ColorTable;
	InterpolationAlgorithm *m_interpolation;
};

#endif