#include <QtGui/QApplication>
#include <QtCore/QTextCodec>
#include <QTranslator>
#include <Windows.h>  
#include <QMessageBox>
#include <TCHAR.h>
#include <QDebug>
#include <imagehlp.h>
#include "mainwindow.h"
#include "MainFrameWindow.h"

// 处理Unhandled Exception的回调函数  
LONG ApplicationCrashHandler(EXCEPTION_POINTERS *pException)  
{     
	// 在这里添加处理程序崩溃情况的代码  
	// 现在很多软件都是弹出一个发送错误报告的对话框  
	// 这里以弹出一个错误对话框并退出程序为例子  
	//  
	//FatalAppExitW(-1,  L"*** Unhandled Exception! ***");  
	QMessageBox::critical(0, QString::fromLocal8Bit("警告"),QString::fromLocal8Bit("*** 系统异常！ ***"));
	return EXCEPTION_CONTINUE_SEARCH;  
} 
int main(int argc, char *argv[])
{
	//try
	//{
		SetUnhandledExceptionFilter((LPTOP_LEVEL_EXCEPTION_FILTER)ApplicationCrashHandler);
		QApplication app(argc, argv);
		
		//#if defined( Q_WS_WIN )
		//	app.setStyle("windowsmodernstyle");
		//#endif

		//初始化编码
		QTextCodec::setCodecForLocale(QTextCodec::codecForLocale());
		QTextCodec::setCodecForCStrings(QTextCodec::codecForLocale());
		QTextCodec::setCodecForTr(QTextCodec::codecForLocale());

		QString DBDir = QCoreApplication::applicationDirPath()+"/Plugin";
		qApp->addLibraryPath(DBDir);

		QTranslator qtTranslator;
		qtTranslator.load(QCoreApplication::applicationDirPath()+"/Language/qt_zh_CN.qm");
		app.installTranslator(&qtTranslator);

#if 0
		MainWindow* mw = new MainWindow;
		mw->showMaximized();
#else
		MainFrameWindow* mfw = MainFrameWindow::GetUIMainFramInstance();
		mfw->showMaximized();
		mfw->LoadToolBoxPlugins("./Config/FrameSystem.xml");
#endif
		
		//mw->menuBar()->addAction("ddd");
		//QMdiArea* mdiArea = new QMdiArea(mw);
		//mw->setCentralWidget(mdiArea);
		//QWidget* view = new QWidget();
		// 
		// QMdiSubWindow* subWindow = mdiArea->addSubWindow(view,Qt::Window);
		// subWindow->setOption(QMdiSubWindow::RubberBandResize, true);
		// subWindow->setOption(QMdiSubWindow::SubWindowOption::AllowOutsideAreaHorizontally, false);
		// subWindow->setOption(QMdiSubWindow::SubWindowOption::AllowOutsideAreaVertically, false);
		// subWindow->setAttribute(Qt::WA_DeleteOnClose);
		// subWindow->showMaximized();
		
		
		return app.exec();
	//}
	//catch (...)
	//{

	//}
}
//
//#include <pluginmanager.h>
//#include <pluginspec.h>
//#include <pluginerroroverview.h>
//
//#include <QString>
//#include <QtCore/QTextCodec>
//#include <QMessageBox>
//#include <QTextStream>
//#include <QApplication>
//#include <QSettings>
//#include <QDir>
//#include <QFileInfo>
////#include <QNetworkProxyFactory>
//#include <QTranslator>
//#include <QLibraryInfo>
//
//using namespace ExtensionSystem;
//
//static const char CORE_PLUGIN_NAME[] = "Core";
//static const char APP_USAGE[] =
//	" [OPTION]... [FILE]...\n"
//	"Options:\n"
//	"    -help                         Display this help\n"
//	"    -version                      Display program version\n"
//	"    -client                       Attempt to connect to already running first instance\n"
//	"    -settingspath <path>          Override the default path where user settings are stored\n"
//	;
//
//static const char ORG_NAME[] = "CSS";
//static const char APP_NAME[] = "IDP";
//
//static const char CONFIG_PATH[] = "/config";
//static const char SHARE_PATH[] = "/share";
//static const char PLUGIN_PATH[] = "/plugins";
//
//static const char HELP_OPTION1[] = "-h";
//static const char HELP_OPTION2[] = "-help";
//static const char HELP_OPTION3[] = "/h";
//static const char HELP_OPTION4[] = "--help";
//static const char VERSION_OPTION[] = "-version";
//static const char CLIENT_OPTION[] = "-client";
//static const char SETTINGS_OPTION[] = "-settingspath";
//
//static inline void toHtml(QString &t)
//{
//	t.replace(QLatin1Char('&'), QLatin1String("&amp;"));
//	t.replace(QLatin1Char('<'), QLatin1String("&lt;"));
//	t.replace(QLatin1Char('>'), QLatin1String("&gt;"));
//	t.insert(0, QLatin1String("<html><pre>"));
//	t.append(QLatin1String("</pre></html>"));
//}
//
//static void displayHelpText(QString t) // No console on Windows.
//{
//	toHtml(t);
//	QMessageBox::information(0, QLatin1String(APP_NAME), t);
//}
//
//static void displayError(const QString &t) // No console on Windows.
//{
//	QMessageBox::critical(0, QLatin1String(APP_NAME), t);
//}
//
//static void printVersion(const PluginSpec *coreplugin)
//{
//	QString version;
//	QTextStream str(&version);
//	str << '\n' << APP_NAME << ' ' << coreplugin->version()<< " based on Qt " << qVersion() << "\n\n";
//	PluginManager::formatPluginVersions(str);
//	str << '\n' << coreplugin->copyright() << '\n';
//	displayHelpText(version);
//}
//
//static void printHelp(const QString &a0)
//{
//	QString help;
//	QTextStream str(&help);
//	str << "Usage: " << a0 << APP_USAGE;
//	const int OptionIndent = 4;
//	const int DescriptionIndent = 34;
//	PluginManager::formatOptions(str, OptionIndent, DescriptionIndent);
//	PluginManager::formatPluginOptions(str, OptionIndent, DescriptionIndent);
//	displayHelpText(help);
//}
//
//static inline QString msgCoreLoadFailure(const QString &why)
//{
//	return qApp->translate("Application", "Failed to load core: %1").arg(why);
//}
//
//static QStringList getPluginPaths()
//{
//	return QStringList() << qApp->applicationDirPath() + QLatin1String(PLUGIN_PATH);
//}
//
//int main(int argc, char **argv)
//{
//	QApplication app(argc, argv);
//
//	//初始化编码
//	QTextCodec::setCodecForLocale(QTextCodec::codecForLocale());
//	QTextCodec::setCodecForCStrings(QTextCodec::codecForLocale());
//	QTextCodec::setCodecForTr(QTextCodec::codecForLocale());
//
//	QTranslator myQtTranslator;
//	myQtTranslator.load(QCoreApplication::applicationDirPath()+"/language/qt_zh_CN.qm");
//	app.installTranslator(&myQtTranslator);
//
//	//加载驱动
//	QString pluginsDir = QCoreApplication::applicationDirPath()+"/ksplugins";
//	qApp->addLibraryPath(pluginsDir);
//
//	//#if defined( Q_WS_WIN )
//	//	app.setStyle( "windowsmodernstyle" );
//	//#endif
//
//	// 确定-settingspath选项
//	QString settingsPath;
//	QStringList arguments = app.arguments();
//	QMutableStringListIterator it(arguments);
//	while (it.hasNext()) {
//		const QString &arg = it.next();
//		if (arg == QLatin1String(SETTINGS_OPTION)) {
//			it.remove();
//			if (it.hasNext()) {
//				settingsPath = QDir::fromNativeSeparators(it.next());
//				it.remove();
//			}
//		}
//	}
//	if (!settingsPath.isEmpty())
//		QSettings::setPath(QSettings::IniFormat, QSettings::UserScope, settingsPath);
//
//	QSettings::setPath(QSettings::IniFormat, QSettings::SystemScope,
//		QCoreApplication::applicationDirPath() + QLatin1String(CONFIG_PATH));
//	QSettings::setDefaultFormat(QSettings::IniFormat);
//
//	QSettings *settings = new QSettings(QSettings::IniFormat, QSettings::UserScope,
//		QLatin1String(ORG_NAME),
//		QLatin1String(APP_NAME));
//	QSettings *globalSettings = new QSettings(QSettings::IniFormat, QSettings::SystemScope,
//		QLatin1String(ORG_NAME),
//		QLatin1String(APP_NAME));
//	PluginManager pluginManager;
//	PluginManager::setFileExtension(QLatin1String("pluginspec"));
//	PluginManager::setGlobalSettings(globalSettings);
//	PluginManager::setSettings(settings);
//
//	// Make sure we honor the system's proxy settings
//	//QNetworkProxyFactory::setUseSystemConfiguration(true);
//
//	// Load qt translation
//	QTranslator qtTranslator;
//	qtTranslator.load("qt_" + QLocale::system().name(),QLibraryInfo::location(QLibraryInfo::TranslationsPath));
//	app.installTranslator(&qtTranslator);
//
//	// Load
//	const QStringList pluginPaths = getPluginPaths();
//	PluginManager::setPluginPaths(pluginPaths);
//
//	QMap<QString, QString> foundAppOptions;
//	if (arguments.size() > 1) {
//		QMap<QString, bool> appOptions;
//		appOptions.insert(QLatin1String(HELP_OPTION1), false);
//		appOptions.insert(QLatin1String(HELP_OPTION2), false);
//		appOptions.insert(QLatin1String(HELP_OPTION3), false);
//		appOptions.insert(QLatin1String(HELP_OPTION4), false);
//		appOptions.insert(QLatin1String(VERSION_OPTION), false);
//		QString errorMessage;
//		if (!PluginManager::parseOptions(arguments, appOptions, &foundAppOptions, &errorMessage)) {
//			displayError(errorMessage);
//			printHelp(QFileInfo(app.applicationFilePath()).baseName());
//			return -1;
//		}
//	}
//
//	QList<ExtensionSystem::PluginSpec*>  plugins = PluginManager::plugins();
//	PluginSpec *coreplugin = 0;
//	foreach (PluginSpec *spec, plugins) {
//		if (spec->name() == QLatin1String(CORE_PLUGIN_NAME)) {
//			coreplugin = spec;
//			break;
//		}
//	}
//	if (!coreplugin) {
//		QString nativePaths = QDir::toNativeSeparators(pluginPaths.join(QLatin1String(",")));
//		const QString reason = qApp->translate("Application", "Could not find 'Core.pluginspec' in %1").arg(nativePaths);
//		displayError(msgCoreLoadFailure(reason));
//		return 1;
//	}
//	if (coreplugin->hasError()) {
//		displayError(msgCoreLoadFailure(coreplugin->errorString()));
//		return 1;
//	}
//	if (foundAppOptions.contains(QLatin1String(VERSION_OPTION))) {
//		printVersion(coreplugin);
//		return 0;
//	}
//	if (foundAppOptions.contains(QLatin1String(HELP_OPTION1))
//		|| foundAppOptions.contains(QLatin1String(HELP_OPTION2))
//		|| foundAppOptions.contains(QLatin1String(HELP_OPTION3))
//		|| foundAppOptions.contains(QLatin1String(HELP_OPTION4))) {
//			printHelp(QFileInfo(app.applicationFilePath()).baseName());
//			return 0;
//	}
//
//	PluginManager::loadPlugins();
//	if (coreplugin->hasError()) {
//		displayError(msgCoreLoadFailure(coreplugin->errorString()));
//		return 1;
//	}
//	if (PluginManager::hasError()) {
//		PluginErrorOverview errorOverview;
//		errorOverview.exec();
//	}
//
//	// shutdown plugin manager on the exit
//	QObject::connect(&app, SIGNAL(aboutToQuit()), &pluginManager, SLOT(shutdown()));
//
//	return app.exec();
//}

