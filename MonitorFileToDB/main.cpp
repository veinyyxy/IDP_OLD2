#include "MonitorDialog.h"
#include <QtGui/QApplication>
#include <QtCore/QTextCodec>
#include <QTranslator>

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
	//��ʼ������
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
		QMessageBox::critical(0, QString::fromLocal8Bit("����ת�����������"),QString::fromLocal8Bit("�������ڴ�ϵͳ�ϵ��κμ��ϵͳ����."));
		return 1;
	}
	QApplication::setQuitOnLastWindowClosed(false);

	MonitorDialog w;
	w.show();

	return app.exec();
}
