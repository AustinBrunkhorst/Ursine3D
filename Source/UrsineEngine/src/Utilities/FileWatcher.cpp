/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** FileWatcher.cpp
**
** Author:
** - Matt Yan - m.yan@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#include "UrsinePrecompiled.h"

#include "FileWatcher.h"


template<typename T>
unsigned FileWatcher<T>::m_watcherCount = 0;          // how many watchers are currently running

// begins the file watcher, using a static callback func
template<typename T>
FileWatcher<T> *FileWatcher<T>::StartFileWatch( std::string path, std::string extension, void( *callback )(std::string), WatchMode mode, unsigned sleepTime, bool recursive )
{
    // call proper constuctor
    FileWatcher<T> *fw = new FileWatcher<T>( path, extension, mode, sleepTime, callback, recursive );

    // call thread, using the static callback main
    fw->m_threadHandle = CreateThread( NULL, 0, FileWatcher<T>::staticWatcherMain, fw, 0, &fw->m_threadID );

    // inc counter
    ++m_watcherCount;

    // return file watcher
    return fw;
}

// begins the filewatcher, using a class function
template<typename T>
FileWatcher<T> *FileWatcher<T>::StartFileWatch( std::string path, std::string extension, void(T::*classFunc)(std::string), T *callbackCaller, WatchMode mode, unsigned sleepTime, bool recursive )
{
    // call proper constuctor, using the class callback main
    FileWatcher<T> *fw = new FileWatcher<T>( path, extension, mode, sleepTime, classFunc, callbackCaller, recursive );

    // call thread
    fw->m_threadHandle = CreateThread( NULL, 0, FileWatcher<T>::classWatcherMain, fw, 0, &fw->m_threadID );

    // return file watcher
    return fw;
}

// ends a file watcher
template<typename T>
void FileWatcher<T>::EndFileWatch( FileWatcher **watcher )
{
    // kills the thread
    (*watcher)->uninit( );

    // delete allocated memory
    delete *watcher;

    // set ptr to null
    *watcher = 0;

    // decrement counter
    --m_watcherCount;
}

// constructor, takes the file extension, when to call func (only on new, when modified, or both), a delay to check, fp.
template<typename T>
FileWatcher<T>::FileWatcher( std::string path, std::string extension, WatchMode mode, unsigned sleepTime, void( *callback )(const std::string), bool recursive )
    : m_mode( mode )
    , m_sleepTime( sleepTime )
    , m_staticCallback( callback )
    , m_recursive( recursive )
{
    // save extension and path as wstring
    m_path = nToWStr( path );

    m_extension = nToWStr( extension );


    // do first scan of the directory to init the system
    std::vector<std::wstring> fileList;

    ScanDirectory( m_path, fileList );
}

// constructor, takes the file extension, when to call func (only on new, when modified, or both), a delay to check, method fp, obj to call method
template<typename T>
FileWatcher<T>::FileWatcher( std::string path, std::string extension, WatchMode mode, unsigned sleepTime, void(T::*callbackFunc)(const std::string), T *callbackCaller, bool recursive )
    : m_mode( mode )
    , m_sleepTime( sleepTime )
    , m_classCallback( callbackFunc )
    , m_funcCaller( callbackCaller )
    , m_recursive( recursive )
{
    // save extension and path as wstring
    m_path = nToWStr( path );

    m_extension = nToWStr( extension );


    // do first scan of the directory to init the system
    std::vector<std::wstring> fileList;

    scanDirectory( m_path, fileList );
}

// main function to call
template<typename T>
DWORD WINAPI FileWatcher<T>::staticWatcherMain( LPVOID lpArg )
{
    // get the file watcher
    FileWatcher<T> *current = reinterpret_cast<FileWatcher<T>*>(lpArg);

    while (true)
    {
        // sleep for allotted time
        Sleep( current->m_sleepTime );

        // assert that the directory still exists

        // get a list of all files that NEED to be checked
        std::vector<std::wstring> fileList;
        current->ScanDirectory( current->m_path, fileList );

        // callback on each
        for (auto x : fileList)
        {
            current->m_staticCallback( current->WToNStr( x ) );
        }
    }

    return 0;
}

// main function to call
template<typename T>
DWORD WINAPI FileWatcher<T>::classWatcherMain( LPVOID lpArg )
{
    // get the file watcher
    FileWatcher<T> *current = reinterpret_cast<FileWatcher<T>*>(lpArg);

    while (true)
    {
        // sleep for allotted time
        Sleep( current->m_sleepTime );

        // get a list of all files that NEED to be checked
        std::vector<std::wstring> fileList;
        current->scanDirectory( current->m_path, fileList );

        // callback on each
        for (auto x : fileList)
        {
            ((*current->m_funcCaller).*(current->m_classCallback))(current->wToNStr( x ));
        }
    }

    return 0;
}

// uninitialize
template<typename T>
void FileWatcher<T>::uninit( )
{
    // kill the thread
    TerminateThread( m_threadHandle, 0 );
}

// recursive search
template<typename T>
bool FileWatcher<T>::scanDirectory( std::wstring directory, std::vector<std::wstring> &fileList )
{
    // current handle
    HANDLE handle = INVALID_HANDLE_VALUE;

    // data of current file
    WIN32_FIND_DATA fileData;

    // how to search
    std::wstring currSearchPath = directory + L"\\*";

    // find file, output to ffd
    handle = FindFirstFile( currSearchPath.c_str( ), &fileData );

    // if invalid, get shrekt
    if (handle == INVALID_HANDLE_VALUE)
    {
        return false;
    }

    // else, do everything in there while we still get a file
    do
    {
        // make sure it isn't the . or .. directory for going up or down
        if (wcscmp( fileData.cFileName, L"." ) != 0 && wcscmp( fileData.cFileName, L".." ) != 0)
        {
            // if a directory and we are recursively searching
            if ((fileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
            {
                //attempt to search through next directory
                if (m_recursive && scanDirectory( directory + L"\\" + fileData.cFileName, fileList ))
                {
                    return true;
                }

                return false;
            }
            // else it's a file
            else
            {
                // check to see if it's a valid file
                if (wcsstr( fileData.cFileName, m_extension.c_str( ) ) != nullptr)
                {
                    //get file info
                    struct stat fileInfo;

                    stat( wToNStr( directory + L"\\" + fileData.cFileName ).c_str( ), &fileInfo );

                    //get times
                    unsigned long long lastModify = fileInfo.st_mtime;

                    unsigned long long lastAccess = fileInfo.st_atime;

                    unsigned long long onCreate = fileInfo.st_ctime;

                    unsigned long long timeVal = 0;

                    //handle our mode
                    switch (m_mode)
                    {
                    case ON_CREATE:

                        // if it's not a new file in map, skip
                        if (m_fileList[ fileData.cFileName ] != 0)
                            continue;

                        timeVal = onCreate;

                        break;

                    case ON_MODIFICATION:

                        // if it's zero, it's a new file, add to map but don't call callback on it
                        if (m_fileList[ fileData.cFileName ] == 0)
                        {
                            m_fileList[ fileData.cFileName ] = lastModify;
                        }


                        // if the time is still the same, don't change
                        if (m_fileList[ fileData.cFileName ] == lastModify)
                        {
                            continue;
                        }

                        timeVal = lastModify;

                        break;

                    case ON_ACCESS:

                        // if the current time matches the register, skip
                        if (m_fileList[ fileData.cFileName ] == lastAccess)
                        {
                            continue;
                        }

                        timeVal = lastAccess;

                        break;

                    default:
                        break;
                    }


                    // at this point, the file is added to the list of files that will have the callback called on them
                    m_fileList[ fileData.cFileName ] = timeVal;

                    fileList.push_back( directory + L"\\" + fileData.cFileName );
                }
            }
        }
    } while (FindNextFile( handle, &fileData ) != 0);

    // if no more file found
    if (GetLastError( ) != ERROR_NO_MORE_FILES)
    {
        FindClose( handle );

        return false;
    }

    // close it
    FindClose( handle );

    return true;
}

// convert from wstring to string
template<typename T>
std::string FileWatcher<T>::wToNStr( const std::wstring &widestr )
{
    std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converterX;

    return converterX.to_bytes( widestr );
}

// convert from string to wstring
template<typename T>
std::wstring FileWatcher<T>::nToWStr( const std::string &str )
{
    std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converterX;

    return converterX.from_bytes( str );
}
