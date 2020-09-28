#include <QDialog>
#include <QFileDialog>
#include <QSettings>
#include "CipasComposition.h"
#include "CipasExportDlg.h"

CipasExportDlg::CipasExportDlg(CCipasComposition* c,QString inputs,QWidget* parent)
	:mComposition(c)
	,mTemplateName(inputs)
	,QDialog(parent)
{
	SetupUI( this );
	connectSlots();
	InitDlg();
}


CipasExportDlg::~CipasExportDlg(void)
{
}

void CipasExportDlg::InitDlg()
{
	QSettings myQSettings;  
	QString lastSaveName = myQSettings.value( "CIPASUI/lastSaveAsImagName", "" ).toString();
	QFileInfo saveFileInfo( lastSaveName );
	QFileInfo templateInfo( mTemplateName );
	lineEdit_sp->setText(QDir::convertSeparators(saveFileInfo.absolutePath()
		+"/"
		+templateInfo.fileName().left(templateInfo.fileName().length()-4)
		+"."+saveFileInfo.suffix()));
	int mdpi=mComposition->printResolution();
	spinBox->setValue(mdpi);
	int mwidth = ( int )( mdpi * mComposition->paperWidth() / 25.4 );
	int mheight = ( int )( mdpi * mComposition->paperHeight() / 25.4 );
	lineEdit_w->setText(QString::number(mwidth));
	lineEdit_h->setText(QString::number(mheight));

}

void CipasExportDlg::SetupUI(QDialog *dialog)
{
	if (dialog->objectName().isEmpty())
		dialog->setObjectName(QString::fromUtf8("CipasExportDialogBaseClass"));
	dialog->resize(432, 217);
	dialog->setWindowTitle(tr("导出图片"));
	gridLayout_3 = new QGridLayout(dialog);
	gridLayout_3->setSpacing(6);
	gridLayout_3->setContentsMargins(11, 11, 11, 11);
	gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
	groupBox_option = new QGroupBox(dialog);
	groupBox_option->setObjectName(QString::fromUtf8("groupBox_option"));
	groupBox_option->setTitle(tr("选项"));
	formLayout = new QFormLayout(groupBox_option);
	formLayout->setSpacing(6);
	formLayout->setContentsMargins(11, 11, 11, 11);
	formLayout->setObjectName(QString::fromUtf8("formLayout"));
	formLayout->setFieldGrowthPolicy(QFormLayout::AllNonFixedFieldsGrow);
	gridLayout_op = new QGridLayout();
	gridLayout_op->setSpacing(6);
	gridLayout_op->setObjectName(QString::fromUtf8("gridLayout_op"));

	label_r = new QLabel(groupBox_option);
	label_r->setObjectName(QString::fromUtf8("label_r"));
	label_r->setText(tr("分辨率： "));
	gridLayout_op->addWidget(label_r, 0, 0, 1, 1);

	label_w = new QLabel(groupBox_option);
	label_w->setObjectName(QString::fromUtf8("label_w"));
	label_w->setText(tr("宽度：  "));

	gridLayout_op->addWidget(label_w, 1, 0, 1, 1);

	label_hp = new QLabel(groupBox_option);
	label_hp->setObjectName(QString::fromUtf8("label_hp"));
	label_hp->setText(tr("像素"));
	gridLayout_op->addWidget(label_hp, 2, 2, 1, 1);

	lineEdit_h = new QLineEdit(groupBox_option);
	lineEdit_h->setObjectName(QString::fromUtf8("lineEdit_h"));
	lineEdit_h->setDisabled(true);
	gridLayout_op->addWidget(lineEdit_h, 2, 1, 1, 1);

	spinBox = new QSpinBox(groupBox_option);
	spinBox->setObjectName(QString::fromUtf8("spinBox"));
	spinBox->setMinimum(1);
	spinBox->setMaximum(300);
	spinBox->setValue(96);

	gridLayout_op->addWidget(spinBox, 0, 1, 1, 1);

	label_h = new QLabel(groupBox_option);
	label_h->setObjectName(QString::fromUtf8("label_h"));
	label_h->setText(tr("高度："));
	gridLayout_op->addWidget(label_h, 2, 0, 1, 1);

	lineEdit_w = new QLineEdit(groupBox_option);
	lineEdit_w->setObjectName(QString::fromUtf8("lineEdit_w"));
	lineEdit_w->setDisabled(true);
	gridLayout_op->addWidget(lineEdit_w, 1, 1, 1, 1);

	label_wp = new QLabel(groupBox_option);
	label_wp->setObjectName(QString::fromUtf8("label_wp"));
	label_wp->setText(tr("像素"));
	gridLayout_op->addWidget(label_wp, 1, 2, 1, 1);

	label_dpi = new QLabel(groupBox_option);
	label_dpi->setObjectName(QString::fromUtf8("label_dpi"));
	label_dpi->setText(tr("dpi"));
	gridLayout_op->addWidget(label_dpi, 0, 2, 1, 1);


	formLayout->setLayout(0, QFormLayout::LabelRole, gridLayout_op);

	//groupBox_option->setLayout(gridLayout_op);
	gridLayout_3->addWidget(groupBox_option, 0, 0, 1, 1);

	// 下面的部分通用

	groupBox_dlg = new QGroupBox(dialog);
	groupBox_dlg->setObjectName(QString::fromUtf8("groupBox_dlg"));
	groupBox_dlg->setTitle(tr("保存"));
	//formLayout_2 = new QFormLayout(groupBox_dlg);
	//formLayout_2->setSpacing(6);
	//formLayout_2->setContentsMargins(11, 11, 11, 11);
	//formLayout_2->setObjectName(QString::fromUtf8("formLayout_2"));
	gridLayout_sp = new QGridLayout();
	gridLayout_sp->setSpacing(6);
	gridLayout_sp->setObjectName(QString::fromUtf8("gridLayout_sp"));
	label_sp = new QLabel(groupBox_dlg);
	label_sp->setObjectName(QString::fromUtf8("label_sp"));
	label_sp->setText(tr("保存路径："));
	gridLayout_sp->addWidget(label_sp, 0, 0, 1, 1);

	lineEdit_sp = new QLineEdit(groupBox_dlg);
	lineEdit_sp->setObjectName(QString::fromUtf8("lineEdit_sp"));

	gridLayout_sp->addWidget(lineEdit_sp, 0, 1, 1, 4);

	pushButton_dlg = new QPushButton(groupBox_dlg);
	pushButton_dlg->setObjectName(QString::fromUtf8("pushButton_dlg"));
	pushButton_dlg->setText(tr("…"));
	gridLayout_sp->addWidget(pushButton_dlg, 0, 5, 1, 1);


	//formLayout_2->setLayout(0, QFormLayout::LabelRole, gridLayout_sp);

	groupBox_dlg->setLayout(gridLayout_sp);
	gridLayout_3->addWidget(groupBox_dlg, 1, 0, 1, 1);

	buttonBox = new QDialogButtonBox(dialog);
	buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
	buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

	gridLayout_3->addWidget(buttonBox, 2, 0, 1, 1);
	//setLayout(gridLayout_3);
}

void CipasExportDlg::connectSlots()
{
	connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
	connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
	connect(pushButton_dlg,SIGNAL(clicked()),this,SLOT(on_pushButton_dlg_clicked()));
	connect(spinBox,SIGNAL(valueChanged ( int )),this,SLOT(on_spinBox_valueChanged( int )));
	connect(lineEdit_sp, SIGNAL(textChanged(const QString &)),this,SLOT(on_lineEdit_sp_textChanged(const QString &)));
}

QString CipasExportDlg::SaveFileName() const
{
	return lineEdit_sp->text();
}

int CipasExportDlg::SaveDpi() const
{
	return spinBox->value();
}


void CipasExportDlg::on_pushButton_dlg_clicked()
{
	QSettings myQSettings;  // where we keep last used filter in persistent state
	QString lastSaveDir = myQSettings.value( "CIPASUI/lastSaveAsImageDir", "" ).toString();
	QString saveFileName = QFileDialog::getSaveFileName( 0, QString::fromLocal8Bit("保存为图片格式..."), lastSaveDir, "Images (*.png);;Images (*.jpg);;Images (*.bmp);;Images (*.tif);;Images (*.xpm)"/*;;Pdf files (*.pdf);;SVG files (*.svg)*/ );
	if (saveFileName!="")
	{
		lineEdit_sp->setText(QDir::convertSeparators(saveFileName));	
	}
}

void CipasExportDlg::on_okpushButton_clicked()
{

}

void CipasExportDlg::on_spinBox_valueChanged( int i)
{
	int mwidth = ( int )( i * mComposition->paperWidth() / 25.4 );
	int mheight = ( int )( i * mComposition->paperHeight() / 25.4 );
	lineEdit_w->setText(QString::number(mwidth));
	lineEdit_h->setText(QString::number(mheight));
}

void CipasExportDlg::on_lineEdit_sp_textChanged(const QString & text)
{
	QSettings myQSettings; 
	QFileInfo fi( text );
	myQSettings.setValue("CIPASUI/lastSaveAsImageDir",fi.absolutePath());
	myQSettings.setValue("CIPASUI/lastSaveAsImagName", text);	//由于需要把模板名加上所以只取路径
}