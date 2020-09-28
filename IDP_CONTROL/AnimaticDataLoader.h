#pragma once

class AnimaticControlerInterface;
class QString;
class QStringList;

class LoadFileCallback
{
public:
	virtual void LoadingFile(QString fileName, int iFileNum) = 0;
};

class AnimaticDataLoader
{
public:
	virtual AnimaticControlerInterface* LoadAnimaticFromFileList(const QStringList& fineList) = 0;
	virtual AnimaticControlerInterface* LoadAnimaticFromFile(const QString& fileName) = 0;
	virtual void* GetResult() = 0;
};

