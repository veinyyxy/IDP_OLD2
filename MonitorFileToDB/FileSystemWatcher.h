#pragma once
#include <QObject>
#include <Windows.h>
#include <string> 
#include <iostream>



class  cFileSystemEvent
{
public:
	enum ChangeType 
	{
		CREATED, 
		CHANGED, 
		DELETED, 
		RENAMED, 
		MAX,
	};


private:
	ChangeType  m_ChangeType;
	std::string m_Directory;  
	std::string m_FileName; 
	std::string m_NewFileName; 


public:
	cFileSystemEvent(ChangeType type, const std::string& directory, const std::string& fileName);
	cFileSystemEvent(const std::string& directory, const std::string& oldFileName, const std::string& newFileName);
	~cFileSystemEvent();


public:
	std::string GetFullPath() const;

	std::string GetOldFullPath() const;

	std::string GetNewFullPath() const;


public:
	ChangeType GetChangeType() const { return m_ChangeType; }
	const std::string& GetDirectory() const { return m_Directory; }
	const std::string& GetFileName() const { return m_FileName; }
	const std::string& GetOldFileName() const { return m_FileName; }
	const std::string& GetNewFileName() const { return m_NewFileName; }
};

////////////////////////////////////////////////////////////////////////////////

class cFileSystemWatcher : public QObject
{
public:
	typedef void (*PFN_EVENT_HANDLER)(const cFileSystemEvent& args);


private:
	struct MONITOR;

	std::string       m_Path;                                
	HANDLE            m_hDirectory;                         
	MONITOR*          m_Monitor;                            
	PFN_EVENT_HANDLER m_EventHandler[cFileSystemEvent::MAX]; 


public:
	cFileSystemWatcher();

	cFileSystemWatcher(const std::string& directory);

	virtual ~cFileSystemWatcher();


public:
	bool Open(const std::string& directory);

	void Close();

	bool IsGood() const;

	void Heartbeat(DWORD period = 1);

	void SetEventHandler(cFileSystemEvent::ChangeType type, PFN_EVENT_HANDLER handler);

	const std::string& GetPath() const { return m_Path; }


private:
	bool PostChangeNotificationRequest(int operation);

	void OnCreate(const std::string& fileName);

	void OnChange(const std::string& fileName);

	void OnDelete(const std::string& fileName);

	void OnRename(const std::string& oldFileName, const std::string& newFileName);

	static void CALLBACK FileSystemWatcherCallback(DWORD, DWORD, LPOVERLAPPED);
};
