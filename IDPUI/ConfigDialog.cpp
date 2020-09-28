#include <QStyleFactory>
#include <QDir>
#include <QTextCodec>
#include <QTextStream>

#include "configdialog.h"

ConfigDialog::ConfigDialog(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	ui.gridLayout->setContentsMargins(0,0,0,0);
	ui.gridLayout_2->setContentsMargins(3,3,3,3);
	//setFixedSize(216,280);
	//ui.gridLayout_3->setContentsMargins(0,0,0,0);
	 ui.leftListWidget->setCurrentRow(0);
	setWindowFlags(Qt::Dialog | Qt::WindowSystemMenuHint|Qt::WindowMinMaxButtonsHint);
	QStringList list = QStyleFactory::keys();
	ui.skinListWidget->addItems(list);

	QDir dir(QCoreApplication::applicationDirPath()+"/Skin"); //需要解码文件保存路径
	dir.setFilter(QDir::Files|QDir::NoDotAndDotDot);  
	dir.setSorting(QDir::Time);
	QStringList fileList = dir.entryList();
	ui.qssListWidget->addItems(fileList);


	connect(ui.exitButton, SIGNAL(clicked()),this,  SLOT(Slot_ButtonClicked()));
	connect(ui.useButton, SIGNAL(clicked()),this,  SLOT(Slot_ButtonClicked()));
	connect(ui.leftListWidget,SIGNAL(currentItemChanged(QListWidgetItem*,QListWidgetItem*)),this, SLOT(Slot_ChangePage(QListWidgetItem*,QListWidgetItem*)));

}

ConfigDialog::~ConfigDialog()
{

}



/** 
  * @brief    系统配置槽   
  * @author   易家祥  
  * @param [in] parameter name   parameter description
  * @param [out] parameter name   parameter description
  * @return   ConfigDialog  系统配置槽
  * @note     易家祥
  * @date     2013年3月                               
  * @exception  exception object exception description  
  * @remarks remark text               
  * @deprecated   description            
  * @since when time or version           
  * @see references            
*/ 
void ConfigDialog::Slot_ButtonClicked()
{
	QPushButton* btn = dynamic_cast<QPushButton*>(sender());
	if (btn == ui.exitButton)
	{
		this->close();
	}
	else if (btn == ui.useButton)
	{
		if (ui.skinListWidget->currentItem())
		{
			QString skinText = ui.skinListWidget->currentItem()->text();
			if (!skinText.isEmpty())
			{
				qApp->setStyle(skinText);// 修改默认风格。
				parentWidget()->update();
			}
		}
		
		if (ui.qssListWidget->currentItem())
		{
			QString qssText = ui.qssListWidget->currentItem()->text();
			if (!qssText.isEmpty())
			{
				InitSkin(ui.qssListWidget->currentItem()->text());
			}
		}
	}
} 


void ConfigDialog::InitSkin(QString skinPath)
{
	QFile file(QCoreApplication::applicationDirPath()+ "/Skin/"+skinPath);
	file.open(QFile::ReadOnly);
	QTextStream stream(&file);
	stream.setCodec(QTextCodec::codecForName("UTF-8"));
	QString styleSheet = 	stream.readAll();
	qApp->setStyleSheet(styleSheet);
	file.close();
}

void ConfigDialog::Slot_ChangePage(QListWidgetItem *current, QListWidgetItem *previous)
{
	if (!current)
		current = previous;

	ui.stackedWidget->setCurrentIndex(ui.leftListWidget->row(current));
}
