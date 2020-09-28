#include "MonitorDialog.h"
#include <QtGui/QApplication>
#include <QtCore/QTextCodec>
#include <QTranslator>

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
	//初始化编码
	QTextCodec::setCodecForLocale(QTextCodec::codecForLocale());
	QTextCodec::setCodecForCStrings(QTextCodec::codecForLocale());
	QTextCodec::setCodecForTr(QTextCodec::codecForLocale());

	QString DBDir = QCoreApplication::applicationDirPath()+"/Plugin";
	qApp->addLibraryPath(DBDir);

	QTranslator qtTranslator;
	qtTranslator.load(QCoreApplication::applicationDirPath()+"/Language/qt_zh_CN.qm");
	app.installTranslator(&qtTranslator);

	if (!QSystemTrayIcon::isSystemTrayAvailable())
	{
		QMessageBox::critical(0, QString::fromLocal8Bit("数据转换批处理程序"),QString::fromLocal8Bit("程序不能在此系统上的任何检测系统托盘."));
		return 1;
	}
	QApplication::setQuitOnLastWindowClosed(false);

	MonitorDialog w;
	w.show();

	return app.exec();
}
