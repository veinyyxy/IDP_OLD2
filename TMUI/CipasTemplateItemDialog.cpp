#include "CipasTemplateItemDialog.h"
#include <QLineEdit>
#include <QTreeWidgetItem>
#include <QtGui/QPushButton>
#include <QtGui/QLabel>
#include <QGridLayout>


CCipasTemplateItemDialog::CCipasTemplateItemDialog(QTreeWidgetItem *item,QWidget* parent ): QDialog( parent )
{
	setUI(this);
	if (item)
	{
		mLineEdit->setText(item->text(0));
	}
}


CCipasTemplateItemDialog::~CCipasTemplateItemDialog(void)
{
}


void CCipasTemplateItemDialog::setUI(QDialog *dialog)
{
	QGridLayout *gdl=new QGridLayout(dialog);
	QLabel *lb1=new QLabel(dialog);
	lb1->setText(tr("模板名称："));
	mLineEdit=new QLineEdit(dialog);
	mLineEdit->setMinimumWidth(40);
	QPushButton *mPbtnOk=new QPushButton(dialog);
	QPushButton *mPbtnCancle=new QPushButton(dialog);
	mPbtnOk->setText(tr("确定"));
	mPbtnCancle->setText(tr("取消"));
	gdl->addWidget(lb1,0,0,1,1);
	gdl->addWidget(mLineEdit,0,1,1,3);
	gdl->addWidget(mPbtnOk,0,4,1,1);
	gdl->addWidget(mPbtnCancle,0,5,1,1);
	dialog->setLayout(gdl);
	dialog->setWindowTitle(tr("编辑模板名称"));
	dialog->setWindowIcon(QIcon(":/images/thematicMap/mComposer.png" ));
	connect(mPbtnCancle,SIGNAL(clicked()),dialog,SLOT(close()));
	connect(mPbtnOk,SIGNAL(clicked()),dialog,SLOT(accept()));
}

QString CCipasTemplateItemDialog::itemText() const
{
	return mLineEdit->text();
}