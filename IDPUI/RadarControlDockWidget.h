#ifndef RADARCONTROLDOCKWIDGET_H
#define RADARCONTROLDOCKWIDGET_H
#include <QtGui>
#include "ui_RadarControlDWidget.h"
#include "idpui_global.h"
class OsgEarthBridge;
namespace goto_gis
{
	class Map;
}

class MainWindow;
class IDPUI_EXPORT RadarControlDockWidget :
	public QDockWidget
{
	Q_OBJECT
public:
	RadarControlDockWidget(QWidget *parent = 0);
	~RadarControlDockWidget(void);
	void AddLayersToCmb(goto_gis::Map* map);
	void GroupEnd(bool bEnd);
private:
	void InitSlots();
	void InitLeftDockWidget();
public Q_SLOTS:
	/** @brief œ‘ æ∂‘”¶≤€ */ 
	virtual void setVisible(bool visible);

	void Slot_ChangLayer( int index );

	void Slot_IsoSurface1_Clicked1(bool checked);
	void Slot_IsoSurface1_Clicked2(bool checked);

	void Slot_Section1_Clicked1(bool checked);
	void Slot_Section2_Clicked2(bool checked);
	void Slot_Section3_Clicked3(bool checked);

	void Slot_Slider1Moved(int value);
	void Slot_Slider2Moved(int value);

private:
	//OsgEarthBridge* GetOsgEarthBridge();
	Ui::RadarControlDockWidget ui;
	MainWindow* m_pMW;
	goto_gis::Map* m_pMap;
};

#endif // RADARCONTROLDOCKWIDGET_H