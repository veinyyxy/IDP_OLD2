#ifndef DISPLAYSETTINGSDIALOG_H
#define DISPLAYSETTINGSDIALOG_H

#include <QtGui>
#include "GeneratedFiles/ui_DisplaySettingsDialog.h"
#include "Map.h"

class MapConfigModel
{
public:
	MapConfigModel(void);
	virtual ~MapConfigModel(void);
	inline void SetBackgroundColor(const QVector4D& v4c){m_v4BackGroundColor = v4c;}
	inline const QVector4D& GetBackgroundColor(){return m_v4BackGroundColor;}
	inline void SetLookedRegion(float fLeft, float fBottom, float fRight, float fTop){
		m_fMapLeft = fLeft;m_fMapRight = fRight;m_fMapTop = fTop;m_fMapBottom = fBottom;}
	inline void SetMapCenter(const QVector3D& v3Center){m_pMapCenter = v3Center;}

	inline void GetLookedRegion(float& fLeft, float& fBottom, float& fRight, float& fTop)
	{
		fLeft	= m_fMapLeft;
		fRight	= m_fMapRight;
		fTop	= m_fMapTop;
		fBottom	= m_fMapBottom;
	}

	inline const QVector3D& GetMapCenter(){return m_pMapCenter;}
private:
	QVector4D m_v4BackGroundColor;
	float m_fMapLeft;
	float m_fMapRight;
	float m_fMapTop;
	float m_fMapBottom;
	QVector3D m_pMapCenter;
};

class MapPropertyControl
{
public:
	MapPropertyControl();
	~MapPropertyControl(void);
	void upDataMapProperty(MapConfigModel* pMCM);
	void upDataToModel(MapConfigModel* pMCM);
};


class DisplaySettingsDialog : public QDialog
{
	Q_OBJECT

public:
	DisplaySettingsDialog(QWidget *parent = 0);
	~DisplaySettingsDialog();
	void StackedWidgetCurrentIndex(int index = 0);
protected Q_SLOTS:
	void Slot_ChangePage(QListWidgetItem *current, QListWidgetItem *previous);
	void Slot_OK();
	void Slot_PushButtonClicked();
private:
	Ui::DisplaySettingsDialog ui;
};

#endif // DISPLAYSETTINGSDIALOG_H
