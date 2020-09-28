#include <QLibrary>
#include <QFile>
#include <QTextStream>

#include "../CallRadarAlgorithms/TsCallArithms.h"

#include "mainwindow.h"
#include "sysmainwindow.h"

#include "DataDockWidget.h"

DataDockWidget::DataDockWidget(QWidget *parent)
	: QDockWidget(parent)
{
	ui.setupUi(this);
	m_pCallArithms = new TsCallArithms;

	InitDepArithmsMap();
	InitConfig();

	ui.editBrowserPath->setText(m_defaultReadPath);
	ui.editSavePath->setText(m_defaultSavePath);
	m_pFileSystemModel =new QFileSystemModel;
	m_pFileSystemModel->setNameFilters(m_defaultFilters);
	m_pFileSystemModel->setFilter(QDir::Files|QDir::NoDotAndDotDot);
	m_pFileSystemModel->setNameFilterDisables(false);
	m_pFileSystemModel->setRootPath(ui.editBrowserPath->text());
	ui.listView->setModel(m_pFileSystemModel);
	ui.listView->setRootIndex(m_pFileSystemModel->index(ui.editBrowserPath->text()));

	InitSlots();
}

DataDockWidget::~DataDockWidget()
{
	if (NULL != m_pCallArithms)
	{
		delete m_pCallArithms;
		m_pCallArithms = NULL;
	}
}


void DataDockWidget::InitConfig()
{
	QFile file(QCoreApplication::applicationDirPath()+tr("/Config/RadarDataConfig.ini"));
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
		return;
	QTextStream in(&file);
	int count = 0;
	while (!in.atEnd()) 
	{
		QString line = in.readLine();
		if (! line.isEmpty())
		{
			if (count == 0)
			{
				m_defaultReadPath = line.split("#",QString::SkipEmptyParts).at(1);
			}
			else if (count == 1)
			{
				m_defaultSavePath = line.split("#",QString::SkipEmptyParts).at(1);
			}
			else if (count == 2)
			{
				m_defaultFilters = line.split("#",QString::SkipEmptyParts).at(1).split("=");
			}
			count++;
		}
	}
	file.close();
}

void DataDockWidget::InitSlots()
{
	connect(ui.btnBrowser, SIGNAL(clicked()),this,  SLOT(Slot_PushButtonClicked()));
	connect(ui.btnSave, SIGNAL(clicked()),this,  SLOT(Slot_PushButtonClicked()));
	connect(ui.GenerateButton, SIGNAL(clicked()),this,  SLOT(Slot_PushButtonClicked()));
	connect(ui.listView, SIGNAL(doubleClicked(const QModelIndex &)),this,  SLOT(Slot_DoubleClicked(const QModelIndex &)));
}

void DataDockWidget::Slot_PushButtonClicked()
{
	QPushButton* btn = dynamic_cast<QPushButton*>(sender());
	if (ui.btnBrowser == btn)
	{
		QFileDialog SelFileDia;
		QStringList strSelFiles;
		SelFileDia.setFileMode(QFileDialog::DirectoryOnly);
		SelFileDia.setViewMode(QFileDialog::Detail);
		if (QFileDialog::Accepted == SelFileDia.exec())
		{
			strSelFiles = SelFileDia.selectedFiles();
			ui.editBrowserPath->setText(strSelFiles.at(0));
			ui.listView->setRootIndex(m_pFileSystemModel->setRootPath(ui.editBrowserPath->text()));
		}
	}
	else if (ui.btnSave == btn)
	{
		QString dir = QFileDialog::getExistingDirectory(this, tr("Ñ¡ÔñÂ·¾¶"),m_defaultSavePath,QFileDialog::ShowDirsOnly| QFileDialog::DontResolveSymlinks);
		if (!dir.isEmpty())
		{
			ui.editSavePath->setText(dir);
		}
	}
	else if (ui.GenerateButton == btn)
	{
		QModelIndex  index = ui.listView->currentIndex();
		if (index.isValid())
		{
			QString fileName = m_pFileSystemModel->fileInfo(index).absoluteFilePath();

			QString strArithmsName = ui.RoseArithmsBox->currentText();
			QString strDepArithms = "";
			if (m_DepArithmsMapStr.contains(strArithmsName))
			{
				strDepArithms = m_DepArithmsMapStr[strArithmsName];
			}

			int ret = GS_FALSE;
			vector<string> DetBaseFileNames;
			DetBaseFileNames.push_back(m_pFileSystemModel->fileInfo(index).absoluteFilePath().toStdString());


			QString strRadSite = ui.RadSiteNameComBox->currentText();
			QApplication::setOverrideCursor(Qt::WaitCursor);
			ret = m_pCallArithms->GdCallRoseArithms(strRadSite.toStdString(), DetBaseFileNames, ui.editSavePath->text().toStdString(), strArithmsName.toStdString(), strDepArithms.toStdString(), ui.SelQcRadioBtn->isChecked());
			QString strShowText = strArithmsName;
			if (GS_OK == ret)
			{
				MainWindow* mw = SysMainWindow::GetMainWindow();
				if(m_pCallArithms->ExportFileNameList().size() > 0)
				{
					QString fileName = QString::fromStdString(m_pCallArithms->ExportFileNameList().at(0));
					if (QFile::exists(fileName))
					{
						mw->LoadData(fileName);
					}
				}
			}
			else
			{
				strShowText += "---Ê§°Ü---";
				QMessageBox::information(this,tr("RoseArithms"),QString::fromLocal8Bit(strShowText.toAscii()),QMessageBox::Ok,QMessageBox::Cancel);
			}
			QApplication::restoreOverrideCursor();
		}
	}
}

void DataDockWidget::InitDepArithmsMap()
{
	m_DepArithmsMapStr["Arith_291_HI"] = "Arith_277_SCI";
	m_DepArithmsMapStr["Arith_278_SCT"] = "Arith_277_SCI";
	m_DepArithmsMapStr["Arith_281_SBT"] = "Arith_263_CR";
	m_DepArithmsMapStr["Arith_292_M"] = "Arith_254_V";
	m_DepArithmsMapStr["Arith_271_OHP"] = "Arith_262_HBR_270_QPR";
	m_DepArithmsMapStr["Arith_273_THP"] = "Arith_262_HBR_270_QPR";
	m_DepArithmsMapStr["Arith_274_SHP"] = "Arith_262_HBR_270_QPR";
}

void DataDockWidget::Slot_DoubleClicked( const QModelIndex & index )
{
	
	
}

void DataDockWidget::setVisible( bool visible )
{
	//if (visible)
	/*{
	QAction* action = (QAction*)this->property("Action").toLongLong();
	if (action)
	{
	action->setCheckable(visible);
	action->setChecked(visible);
	}
	}
	QDockWidget::setVisible(visible);*/
}