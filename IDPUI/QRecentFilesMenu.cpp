#include <QDebug>
#include <QFileInfo>
#include "IDPApplication.h"
#include "QRecentFilesMenu.h"

static const qint32 RecentFilesMenuMagic = 0xff;

QRecentFilesMenu::QRecentFilesMenu(QWidget * parent): QMenu(parent), m_maxCount(100), m_format(QLatin1String("%d %s"))
{
	connect(this, SIGNAL(triggered(QAction*)), this, SLOT(menuTriggered(QAction*)));
	setMaxCount(m_maxCount);
}

QRecentFilesMenu::QRecentFilesMenu(const QString & title, QWidget * parent)
	: QMenu(title, parent)
	, m_maxCount(100)
	, m_format(QLatin1String("%d %s"))
{
	connect(this, SIGNAL(triggered(QAction*)), this, SLOT(menuTriggered(QAction*)));

	setMaxCount(m_maxCount);
}

void QRecentFilesMenu::addRecentFile(const QString &fileName)
{
	m_files.removeAll(fileName);
	m_files.prepend(fileName);

	while (m_files.size() > maxCount())
		m_files.removeLast();

	updateRecentFileActions();
}

void QRecentFilesMenu::clearMenu()
{
	m_files.clear();
	QFile file(IDPApplication::ExePath()+"/IDP_RecentFiles.txt");
	if (!file.open(QIODevice::Append))
		return;
	file.resize(0);
	file.close();
	updateRecentFileActions();
}

int QRecentFilesMenu::maxCount() const
{
	return m_maxCount;
}

void QRecentFilesMenu::setFormat(const QString &format)
{
	if (m_format == format)
		return;
	m_format = format;

	updateRecentFileActions();
}

const QString & QRecentFilesMenu::format() const
{
	return m_format;
}

void QRecentFilesMenu::saveRecent(const QString &filePath)
{
	QFile file(filePath);
	if (!file.open(QIODevice::Append))
		return;
	file.resize(0);
	foreach(QString s , m_files)
	{
		file.write((s+tr("\r\n")).toLocal8Bit());
	}
	file.close();
}

void QRecentFilesMenu::loadRecent(const QString &filePath)
{
	QFile file(filePath);
	if (!file.open(QIODevice::ReadWrite | QIODevice::Text))
		return;
	QTextStream in(&file);
	while (!in.atEnd()) 
	{
		QString strPath = in.readLine();
		if (tr("") != strPath.trimmed())
		{
			m_files.push_back(strPath);
		}
	}
	file.close();
	updateRecentFileActions();
}

void QRecentFilesMenu::setMaxCount(int count)
{
	m_maxCount = count;

	updateRecentFileActions();
}

void QRecentFilesMenu::menuTriggered(QAction* action)
{
	if (action->data().isValid())
		emit recentFileTriggered(action->data().toString());
}

void QRecentFilesMenu::updateRecentFileActions()
{
	int numRecentFiles = qMin(m_files.size(), maxCount());

	clear();

	for (int i = 0; i < numRecentFiles; ++i) {
		QString strippedName = QFileInfo(m_files[i]).fileName();

		QString text = m_format;
		text.replace(QLatin1String("%d"), QString::number(i + 1));
		text.replace(QLatin1String("%s"), strippedName);

		QAction* recentFileAct = addAction(text);
		recentFileAct->setData(m_files[i]);
	}
	addSeparator();
	addAction(tr("Çå¿ÕÀúÊ·"), this, SLOT(clearMenu()));

	setEnabled(numRecentFiles > 0);
}

