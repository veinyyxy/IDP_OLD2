#pragma once

#include <QDialog>

#include "Layer.h"
#include "ui_cipascomposerlegendlayersdialogbase.h"


class CCipasComposerLegend;

using namespace goto_gis;
class CCipasComposerLegendLayersDialog :public QDialog,private Ui::CipasComposerLegendLayersDialogBase
{
	Q_OBJECT

public:
	CCipasComposerLegendLayersDialog( CCipasComposerLegend* mLegend, QWidget* parent = 0);
	~CCipasComposerLegendLayersDialog(void);
	inline QListWidget* GetListWidget() {return listWidget;}
	//Layer* selectedLayer();
	protected Q_SLOTS:
		void on_pushButton_clicked();
		void on_pushButtonAdd_clicked();
		void on_pushButtonDel_clicked();
private:
	/**Stores the relation between items and map layer pointers. */
	//QMap<QListWidgetItem*, Layer*> mItemLayerMap;
};

