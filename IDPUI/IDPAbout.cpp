#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>

#include "IDPAbout.h"

IDPAbout::IDPAbout(QWidget *parent ):QDialog(parent),size(400,80)
{
	logo=createPixmapLabel();
	//initUI();
}


IDPAbout::~IDPAbout(void)
{
	delete logo;
}

void IDPAbout::initUI()
{
	QPushButton *pBtn = new QPushButton(QString::fromLocal8Bit("È·¶¨"));
	connect(pBtn, SIGNAL(clicked()), this, SLOT(slotClose()));
	QVBoxLayout *hlayout=new QVBoxLayout;
	hlayout->addWidget(logo);
	QFont font(QString::fromLocal8Bit("Î¢ÈíÑÅºÚ"), 10);
	for (int i=0;i<contents.count();i++)
	{
		QLabel *texts=createContentLabel(contents.at(i));
		texts->setFont(font);
		hlayout->addWidget(texts);
	}
	QHBoxLayout *lay = new QHBoxLayout;
	lay->addWidget(new QLabel("				"));
	lay->addWidget(pBtn);
	hlayout->addLayout(lay);
	this->setLayout(hlayout);
	resize(400,300);
}

void IDPAbout::setHeaderList(const QStringList &ls)
{
	headers=ls;
}

void IDPAbout::setContentList(const QStringList &ls)
{
	contents=ls;
}

void IDPAbout::setIcon(const QIcon &icon)
{
	this->icon = icon;
	updatePixmapLabels();
}

void IDPAbout::setSize(const QSize &size)
{
	if (size != this->size) {
		this->size = size;
		updatePixmapLabels();
	}
}

QLabel *IDPAbout::createHeaderLabel(const QString &text)
{
	QLabel *label = new QLabel(tr("<b>%1</b>").arg(text));
	label->setAlignment(Qt::AlignLeft);
	return label;
}

QLabel *IDPAbout::createContentLabel(const QString &text)
{
	QLabel *label = new QLabel(text);
	label->setAlignment(Qt::AlignLeft);
	return label;
}

QLabel *IDPAbout::createPixmapLabel()
{
	QLabel *label = new QLabel;
	label->setEnabled(true);
	label->setAlignment(Qt::AlignCenter);
	//label->setFrameShape(QFrame::Box);
	//label->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	label->setBackgroundRole(QPalette::Base);
	label->setAutoFillBackground(true);
	//label->setMinimumSize(132, 132);
	label->setMinimumHeight(120);
	return label;
}

void IDPAbout::updatePixmapLabels()
{
	QPixmap pixmap = icon.pixmap(size, QIcon::Normal,  QIcon::Off);
	logo->setStyleSheet("background: transparent;border:0px");
	logo->setPixmap(pixmap);
}

void IDPAbout::slotClose()
{
	this->close();
}

//! [5]
