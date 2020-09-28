#pragma once
#include <QtCore/QCoreApplication>

class IDPApplication : public QObject
{
private:

public:
	static QString ExePath()
	{
		return QCoreApplication::applicationDirPath();
	};
	static QString Path()
	{
		return ExePath();//ExePath().replace("/bin","");
	};
	static QString ConfigPath()
	{
		return Path()+"/Config/";
	};
	static QString TempPath()
	{
		return Path()+"/Temp/";
	};
	static QString DataPath()
	{
		return Path()+"/Data/";
	}
	static QString FontPath()
	{
		return Path()+"/Font/";
	}
	static QString GEOPath()
	{
		return Path()+"/GEO/";
	}
	static QString LanguagePath()
	{
		return Path()+"/Language/";
	}
	static QString LogPath()
	{
		return Path()+"/Log/";
	}
	static QString LogBackUpPath()
	{
		return Path()+"/LogBackUp/";
	}
	static QString PluginPath()
	{
		return Path()+"/Plugin/";
	}
	static QString ResourcePath()
	{
		return Path()+"/Resource/";
	}
	static QString TemplatePath()
	{
		return Path()+"/Template/";
	}
	static QString ProjectionPath()
	{
		return Path()+"/Projection/";
	}
	static QString VersionPath()
	{
		return Path()+"/Version/";
	}
};