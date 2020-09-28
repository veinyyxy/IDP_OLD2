////////////////////////////////////////////////////////////////////////////////
#include <tchar.h>
#include "FileSystemWatcher.h"

namespace
{ 
    enum Operation
    {
        CONTINUE_MONITORING,  
        STOP_MONITORING,
    };

    const DWORD FILTER = 
        //FILE_NOTIFY_CHANGE_CREATION | 
        //FILE_NOTIFY_CHANGE_LAST_WRITE |
        //FILE_NOTIFY_CHANGE_SIZE |
        //FILE_NOTIFY_CHANGE_DIR_NAME |
        //FILE_NOTIFY_CHANGE_FILE_NAME;

	FILE_NOTIFY_CHANGE_FILE_NAME| 
		FILE_NOTIFY_CHANGE_DIR_NAME| 
		FILE_NOTIFY_CHANGE_ATTRIBUTES| 
		FILE_NOTIFY_CHANGE_SIZE| 
		FILE_NOTIFY_CHANGE_LAST_WRITE| 
		FILE_NOTIFY_CHANGE_LAST_ACCESS| 
		FILE_NOTIFY_CHANGE_CREATION|
		FILE_NOTIFY_CHANGE_SECURITY;

    const size_t BUFFER_SIZE = sizeof(FILE_NOTIFY_INFORMATION) * 1024;

	LPCWSTR stringToLPCWSTR(std::string orig)
	{
		size_t origsize = orig.length() + 1;
		const size_t newsize = 100;
		size_t convertedChars = 0;
		wchar_t *wcstring = (wchar_t *)malloc(sizeof(wchar_t)*(orig.length()-1));
		mbstowcs_s(&convertedChars, wcstring, origsize, orig.c_str(), _TRUNCATE);
		return wcstring;
	}

    std::string ExtractFileName(const FILE_NOTIFY_INFORMATION* info)
    {
        char fileName[1024] = {0, };
        int count = WideCharToMultiByte(
            CP_ACP, 0, info->FileName, info->FileNameLength / sizeof(WCHAR), 
            fileName, _ARRAYSIZE(fileName) - 1, NULL, NULL);
        fileName[count] = '\0';
        return std::string(fileName);
    }

    std::string AddBackslash(const std::string& path)
    {
        if (path.empty()) return std::string("");
        return path[path.size()-1] != '\\' ? path + std::string("\\") : path;
    }

    std::string Combine(const std::string& dir, const std::string& file)
    {
        return AddBackslash(dir) + file;
    }

    bool IsDirectoryExist(const std::string& path)
    {
        DWORD attr = GetFileAttributes(stringToLPCWSTR(path));
        return (attr != INVALID_FILE_ATTRIBUTES) && (attr & FILE_ATTRIBUTE_DIRECTORY);
    }

}

////////////////////////////////////////////////////////////////////////////////
cFileSystemEvent::cFileSystemEvent(ChangeType type, 
                                   const std::string& directory, 
                                   const std::string& fileName)
: m_ChangeType(type), 
m_Directory(directory), 
m_FileName(fileName)
{
}

////////////////////////////////////////////////////////////////////////////////
cFileSystemEvent::cFileSystemEvent(const std::string& directory, 
                                   const std::string& oldFileName, 
                                   const std::string& newFileName)
: m_ChangeType(RENAMED),
m_Directory(directory), 
m_FileName(oldFileName),
m_NewFileName(newFileName)
{
}

////////////////////////////////////////////////////////////////////////////////
cFileSystemEvent::~cFileSystemEvent()
{
}

////////////////////////////////////////////////////////////////////////////////
std::string cFileSystemEvent::GetFullPath() const
{
    return Combine(m_Directory, m_FileName);
}

////////////////////////////////////////////////////////////////////////////////
std::string cFileSystemEvent::GetOldFullPath() const
{
    return Combine(m_Directory, m_FileName);
}

////////////////////////////////////////////////////////////////////////////////
std::string cFileSystemEvent::GetNewFullPath() const
{
    return Combine(m_Directory, m_NewFileName);
}

////////////////////////////////////////////////////////////////////////////////

struct cFileSystemWatcher::MONITOR : public OVERLAPPED
{
    BYTE                Buffer[BUFFER_SIZE]; 
    int                 Operation;          
    cFileSystemWatcher* Watcher;            
};

////////////////////////////////////////////////////////////////////////////////
cFileSystemWatcher::cFileSystemWatcher()
: m_hDirectory(INVALID_HANDLE_VALUE), m_Monitor(NULL)
{
    ZeroMemory(m_EventHandler, sizeof(PFN_EVENT_HANDLER) * cFileSystemEvent::MAX);
}

////////////////////////////////////////////////////////////////////////////////
cFileSystemWatcher::cFileSystemWatcher(const std::string& directory)
: m_hDirectory(INVALID_HANDLE_VALUE), m_Monitor(NULL)
{
    ZeroMemory(m_EventHandler, sizeof(PFN_EVENT_HANDLER) * cFileSystemEvent::MAX);

    if (!Open(directory))
        Close();
}

////////////////////////////////////////////////////////////////////////////////
cFileSystemWatcher::~cFileSystemWatcher()
{
    Close();
}

////////////////////////////////////////////////////////////////////////////////
bool cFileSystemWatcher::Open(const std::string& directory)
{
    Close();

    bool success = false;

    do 
    {
        m_Path = AddBackslash(directory);
        if (!IsDirectoryExist(m_Path))
            break;

        m_hDirectory = CreateFile(stringToLPCWSTR(m_Path),
            FILE_LIST_DIRECTORY,
            FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
            NULL,
            OPEN_EXISTING,
            FILE_FLAG_BACKUP_SEMANTICS | FILE_FLAG_OVERLAPPED,
            NULL
            );

        if (m_hDirectory == INVALID_HANDLE_VALUE)
            break;

        m_Monitor = reinterpret_cast<MONITOR*>(HeapAlloc(GetProcessHeap(), 0, sizeof(MONITOR)));
        if (!m_Monitor)
            break;

        ZeroMemory(m_Monitor, sizeof(MONITOR));
        m_Monitor->Watcher = this;

        if (!PostChangeNotificationRequest(CONTINUE_MONITORING))
            break;

        success = true;
    } while(0);

    if (!success)
        Close();

    return success;
}

////////////////////////////////////////////////////////////////////////////////
void cFileSystemWatcher::Close()
{
    m_Path = "";

    if (m_hDirectory != INVALID_HANDLE_VALUE)
    {
        CloseHandle(m_hDirectory);
        m_hDirectory = INVALID_HANDLE_VALUE;
    }

    if (m_Monitor)
    {
        HeapFree(GetProcessHeap(), 0, m_Monitor);
        m_Monitor = NULL;
    }
}

////////////////////////////////////////////////////////////////////////////////
bool cFileSystemWatcher::IsGood() const
{
    return m_hDirectory != INVALID_HANDLE_VALUE && m_Monitor != NULL;
}

////////////////////////////////////////////////////////////////////////////////
void cFileSystemWatcher::Heartbeat(DWORD period)
{
    if (IsGood())
    {
        SleepEx(period, TRUE); 
    }
}

////////////////////////////////////////////////////////////////////////////////
void cFileSystemWatcher::SetEventHandler(
    cFileSystemEvent::ChangeType type, cFileSystemWatcher::PFN_EVENT_HANDLER handler)
{
    if (0 <= type && type < cFileSystemEvent::MAX)
        m_EventHandler[type] = handler;
}

////////////////////////////////////////////////////////////////////////////////
bool cFileSystemWatcher::PostChangeNotificationRequest(int operation)
{
    if (!IsGood()) return false;

    m_Monitor->Operation = operation;

    DWORD bytesReturned = 0;
	DWORD bufferSize = static_cast<DWORD>(BUFFER_SIZE);

	if (ReadDirectoryChangesW(m_hDirectory, m_Monitor->Buffer, bufferSize, 
		TRUE, FILTER, &bytesReturned, m_Monitor, FileSystemWatcherCallback))
	{
		return true;
	}
	else
	{
		return false;
	}
}

////////////////////////////////////////////////////////////////////////////////
void cFileSystemWatcher::OnCreate(const std::string& fileName)
{
	if (m_EventHandler[cFileSystemEvent::CREATED])
	{
		cFileSystemEvent evt(cFileSystemEvent::CREATED, m_Path, fileName);
		m_EventHandler[cFileSystemEvent::CREATED](evt);
	}
}

////////////////////////////////////////////////////////////////////////////////
void cFileSystemWatcher::OnChange(const std::string& fileName)
{
    if (m_EventHandler[cFileSystemEvent::CHANGED])
    {
        cFileSystemEvent evt(cFileSystemEvent::CHANGED, m_Path, fileName);
        m_EventHandler[cFileSystemEvent::CHANGED](evt);
    }
}

////////////////////////////////////////////////////////////////////////////////
void cFileSystemWatcher::OnDelete(const std::string& fileName)
{
    if (m_EventHandler[cFileSystemEvent::DELETED])
    {
        cFileSystemEvent evt(cFileSystemEvent::DELETED, m_Path, fileName);
        m_EventHandler[cFileSystemEvent::DELETED](evt);
    }
}

////////////////////////////////////////////////////////////////////////////////
void cFileSystemWatcher::OnRename(const std::string& oldFileName, const std::string& newFileName)
{
    if (m_EventHandler[cFileSystemEvent::RENAMED])
    {
        cFileSystemEvent evt(m_Path, oldFileName, newFileName);
        m_EventHandler[cFileSystemEvent::RENAMED](evt);
    }
}

////////////////////////////////////////////////////////////////////////////////
void CALLBACK cFileSystemWatcher::FileSystemWatcherCallback(
	DWORD errorCode, DWORD bytesTransferred, LPOVERLAPPED overlapped)
{
	cFileSystemWatcher::MONITOR* monitor = 
		reinterpret_cast<cFileSystemWatcher::MONITOR*>(overlapped);
	cFileSystemWatcher* watcher = monitor->Watcher;

	if (errorCode == ERROR_SUCCESS)
    {
        PFILE_NOTIFY_INFORMATION info = NULL;
        size_t offset = 0;
        std::string fileName, oldFileName, newFileName;

        do
        {
			info = reinterpret_cast<PFILE_NOTIFY_INFORMATION>(&monitor->Buffer[offset]);
			offset += info->NextEntryOffset;

			if (info->Action == FILE_ACTION_ADDED)
			{
				fileName = ExtractFileName(info);
				watcher->OnCreate(fileName);
			}
			else if (info->Action == FILE_ACTION_REMOVED)
            {
                fileName = ExtractFileName(info);
                watcher->OnDelete(fileName);
            }
            else if (info->Action == FILE_ACTION_MODIFIED)
            {
                fileName = ExtractFileName(info);
                watcher->OnChange(fileName);
            }
            else if (info->Action == FILE_ACTION_RENAMED_OLD_NAME)
            {
                oldFileName = ExtractFileName(info);
            }
            else if (info->Action == FILE_ACTION_RENAMED_NEW_NAME)
            {
                newFileName = ExtractFileName(info);
                watcher->OnRename(oldFileName, newFileName);
            }
        } while (info->NextEntryOffset != 0);
    }

    if (monitor->Operation == CONTINUE_MONITORING)
        watcher->PostChangeNotificationRequest(CONTINUE_MONITORING);
}