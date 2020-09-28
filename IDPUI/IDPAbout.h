#pragma once
#include <QIcon>
#include <QLabel>
#include <QDialog>
#include <QStringList>


class  IDPAbout : public QDialog
{
	Q_OBJECT
public:
	IDPAbout(QWidget *parent = 0);
	~IDPAbout(void);
	void setIcon(const QIcon &icon);
	void setSize(const QSize &size);
	void setHeaderList(const QStringList &ls);
	void setContentList(const QStringList &ls);
	void initUI();

private slots:
	void slotClose();
private:
	QLabel *createHeaderLabel(const QString &text);
	QLabel *createContentLabel(const QString &text);
	QLabel *createPixmapLabel();
	void updatePixmapLabels();

	
	QStringList headers;
	QStringList contents;
	QIcon icon;
	QSize size;
	QLabel *logo;
};

