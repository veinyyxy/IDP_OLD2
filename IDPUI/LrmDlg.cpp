#include <QDebug>
#include "LrmDlg.h"


LrmDlg::LrmDlg(QWidget* parent):QDialog(parent)
{
	setAttribute(Qt::WA_DeleteOnClose);
	setWindowTitle(tr("分层组合反射率"));
	setContentsMargins(5,5,5,5);
	resize(700,400);

	m_MainHLay = new QVBoxLayout(this);
	m_MainHLay->setContentsMargins(5,5,5,5);
	setLayout(m_MainHLay);
	
	m_MainGrp = new QGroupBox(this);
	m_MainGrp->setTitle("层数和层高");

	m_MainGrpHLay = new QVBoxLayout(m_MainGrp);
	m_MainGrp->setLayout(m_MainGrpHLay);
	

	m_TopGLayout = new QGridLayout();
	QLabel *m_LayNumLab = new QLabel(m_MainGrp);
	m_LayNumLab->setText("层数：");
	QComboBox *m_LayNumCom = new QComboBox(m_MainGrp);
	for(int i = 1;i<16;i++)
	{
		m_LayNumCom->addItem(QString("%1").arg(i));
	}
	QLabel *m_LayTipLab = new QLabel(m_MainGrp);
	m_LayTipLab->setText("每一层地面的海拔高度范围是100~15000米。");

	m_TopGLayout->addWidget(m_LayNumLab,0,0);
	m_TopGLayout->addWidget(m_LayNumCom,0,1);
	m_TopGLayout->addWidget(m_LayTipLab,0,2);

	m_BottonGLayout = new QGridLayout();
	
	m_MainGrpHLay->addLayout(m_TopGLayout);
	m_MainGrpHLay->addLayout(m_BottonGLayout);
	m_MainGrpHLay->setSizeConstraint(QLayout::SetFixedSize);

	m_buttonLay = new QHBoxLayout(this);
	m_buttonLay->setSizeConstraint(QLayout::SetFixedSize);

	m_MainHLay->addWidget(m_MainGrp);
	m_MainHLay->addLayout(m_buttonLay);

	m_MainHLay->setSizeConstraint(QLayout::SetFixedSize);
	LrmConfig(0);
	connect(m_LayNumCom,SIGNAL(currentIndexChanged(int)),this,SLOT(LrmConfig(int)));
}


LrmDlg::~LrmDlg(void)
{
}

void LrmDlg::LrmConfig( int n)
{
	int m_NumOfCom = n+1;
	int m_row = m_NumOfCom/5;
	
	qDeleteAll(m_pWidgetList);
	m_pWidgetList.clear();

	for (int i = 1;i<=m_NumOfCom;i++)
	{
		int cow = 0,rol = 0,d = i;
		if (i>5&&i<=10)
		{
			rol = 2;
			d = d-5;
		}
		if (i>10)
		{
			rol = 4;
			d = d-10;
		}

		QLabel *m_LayLab = new QLabel(m_MainGrp);
		QLineEdit *m_LinEdit = new QLineEdit(m_MainGrp);
		m_LayLab->setText(QString("层%1距地面的海拔高度(米)").arg(i));
		m_LinEdit->setText(QString("%10000").arg(i));
		m_BottonGLayout->addWidget(m_LayLab,d,rol);
		m_BottonGLayout->addWidget(m_LinEdit,d,rol+1);

		m_pWidgetList.push_back(m_LayLab);
		m_pWidgetList.push_back(m_LinEdit);
	}
	m_BottonGLayout->setContentsMargins(0,0,0,0);
	m_MainGrp->setLayout(m_MainGrpHLay);
	m_MainGrpHLay->setSizeConstraint(QLayout::SetFixedSize);

	QPushButton *okButton = new QPushButton(this);
	okButton->setText(tr("确定"));
	QPushButton *CancelButton = new QPushButton(this);
	CancelButton->setText(tr("取消"));
	m_pWidgetList.push_back(okButton);
	m_pWidgetList.push_back(CancelButton);

	m_buttonLay->setSizeConstraint(QLayout::SetFixedSize);
	m_buttonLay->addStretch(1);
	m_buttonLay->addWidget(okButton);
	m_buttonLay->addWidget(CancelButton);

	m_MainHLay->setSizeConstraint(QLayout::SetFixedSize);
	connect(okButton,SIGNAL(clicked()),this,SLOT(OkClicked()));
	connect(CancelButton,SIGNAL(clicked()),this,SLOT(close()));
}

void LrmDlg::OkClicked()
{
	typedef QList<QObject*> QObjectList;
	QObjectList list = m_MainGrp->children ();
	for (int i = 0; i < list.size(); ++i) 
	{
		if (QString (list.at(i)->metaObject()->className())== "QLineEdit")
		{
			QString strText = ((QLineEdit *)list.at(i))->text();
			LayHigh->push_back(strText);
		}
	}
	//setResult(QDialog::Accepted);
	accept();
}
