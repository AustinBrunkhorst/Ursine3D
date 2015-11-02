/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** FileWatcher.h
**
** Author:
** - Matt Yan - m.yan@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#pragma once

#include <string>
#include <vector>
#include <codecvt>
#include <unordered_map>
#include <list>

template<typename T>
class FileWatcher
{
public:
    // what is the watcher watching for?
    enum WatchMode : unsigned
    {
        ON_ACCESS = 0,      // call callback when accessed
        ON_MODIFICATION,    // call callback when contents have been changed
        ON_CREATE,          // call callback when a new file is detected
    };

public:
    /** @brief begins a file watcher, using a class method as the callback
    *
    *  @param path path of directory to watch
    *  @param extension the extension to watch for
    *  @param callback the callback function
    *  @param mode what to watch files for
    *  @param sleepTime how long to sleep in-between scans
    *  @param recursive if we should recursively go through directories
    *  @return pointer to the filewatcher
    */
    static FileWatcher *StartFileWatch( 
        std::string path, 
        std::string extension, 
                void( *callback )(const std::string), WatchMode mode = ON_MODIFICATION, 
                                        unsigned sleepTime = 1000, bool recursive = false );

    /** @brief begins a file watcher, using a class method as the callback
    *
    *  @param path path of directory to watch
    *  @param extension the extension to watch for
    *  @param classFunc the callback function
    *  @param callbackCaller the object calling the callback
    *  @param mode what to watch files for
    *  @param sleepTime how long to sleep in-between scans
    *  @param recursive if we should recursively go through directories
    *  @return pointer to the file watcher
    */
    static FileWatcher *StartFileWatch( std::string path, std::string extension, void(T::*classFunc)(const std::string), 
                                        T *callbackCaller, WatchMode mode = ON_MODIFICATION, unsigned sleepTime = 1000, 
                                        bool recursive = false );

    /** @brief permanently stop a file watcher
    *
    *  @param watcher the watcher to end
    *  @return void
    */
    static void EndFileWatch( FileWatcher **watcher );

private:
    /** @brief constructor for static function callback
    *
    *  @param path path of directory to watch
    *  @param extension the extension to watch for
    *  @param mode what to watch files for
    *  @param sleepTime how long to sleep in-between scans
    *  @param callbackFunc the callback function
    *  @param recursive if we should recursively go through directories
    *  @return void
    */
    FileWatcher( std::string path, std::string extension, WatchMode mode, unsigned sleepTime, void( *callbackFunc )(std::string), bool recursive );

    /** @brief constructor for class method callback
    *
    *  @param path path of directory to watch
    *  @param extension the extension to watch for
    *  @param mode what to watch files for
    *  @param sleepTime how long to sleep in-between scans
    *  @param callbackFunc the callback function
    *  @param callbackCaller the object calling the callback
    *  @param recursive if we should recursively go through directories
    *  @return void
    */
    FileWatcher( std::string path, std::string extension, WatchMode mode, unsigned sleepTime, void(T::*callbackFunc)(std::string), T *callbackCaller, bool recursive );

    /** @brief main entrypoint for thread, if a static function is the callback
    *
    *  @param param the parameter passed to thread
    *  @return exit value
    */
    static DWORD WINAPI staticWatcherMain( LPVOID param );

    /** @brief main entrypoint for thread, if a class method
    *   is the callback
    *
    *  @param param the parameter passed to thread
    *  @return exit value
    */
    static DWORD WINAPI classWatcherMain( LPVOID param );

    /** @brief permenantly stops a file watcher.
    *
    *  This includes stopping the thread
    *
    *  @return void
    */
    void uninit( void );

    /** @brief recursive function to scan a directory for files that require action
    *
    *  Whether or not a file is acted upon is dependent on the WatchMode set in
    *  StartFileWatch
    *
    *  @param directory the directory to scan
    *  @param fileList reference to vector of the files that require action
    *  @return it succeeded or not
    */
    bool scanDirectory( std::wstring directory, std::vector<std::wstring> &fileList );

    /** @brief converts wide string to narrow string
    *
    *  @param wstr wide string to convert
    *  @return converted narrow string
    */
    std::string wToNStr( const std::wstring &wstr );

    /** @brief converts narrow string to wide string
    *
    *  @param str the narrow string to convert
    *  @return converted wide string
    */
    std::wstring nToWStr( const std::string &str );

private:
    HANDLE m_threadHandle;                          // something to keep track of the thread

    DWORD m_threadID;                               // ID of the thread

    std::wstring m_path;                            // path to search in

    std::wstring m_extension;                       // what extension should I look for?

    unsigned m_sleepTime;                           // how long to sleep before checking again

    static unsigned m_watcherCount;                 // how many watchers are currently running

    bool m_recursive;                               // recursive search?

    WatchMode m_mode;                               // watch mode

    T *m_funcCaller;                                // object to call

    std::unordered_map
        <std::wstring, unsigned long long>
        m_fileList;                                 // the files currently watched 

    void(T::*m_classCallback)(const std::string);   // class callback method

    void( *m_staticCallback )(const std::string);   // static callback func
};

#include "FileWatcher.hpp"