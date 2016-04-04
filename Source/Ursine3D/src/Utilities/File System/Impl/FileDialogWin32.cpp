#include "UrsinePrecompiled.h"

#if defined(PLATFORM_WINDOWS)

#include "FileDialog.h"

#include <shlobj.h>

using namespace ursine;

namespace
{
    const auto kDefaultOpenTitle = "Open File";
    const auto kDefaultSaveTitle = "Save File";
    const auto kDefaultFolderTitle = "Browse For Folder";

    std::string getCompiledFilter(const std::vector<fs::FileDialogFilter> &filters);

    void runOpenFileDialog(const fs::FileDialogConfig &config, fs::FileDialogResult &output);
    void runMultiOpenFileDialog(const fs::FileDialogConfig &config, fs::FileDialogResult &output);

    void runSaveFileDialog(const fs::FileDialogConfig &config, fs::FileDialogResult &output);

    void runOpenFolderDialog(const fs::FileDialogConfig &config, fs::FileDialogResult &output);
    int CALLBACK folderBrowserCallback(HWND window, UINT message, LPARAM parameter, LPARAM data);

    class WorkingDirectorySaver
    {
    public:
        WorkingDirectorySaver(void)
            : m_initialDirectory( fs::current_path( ) ) { }

        ~WorkingDirectorySaver(void)
        {
            fs::current_path( m_initialDirectory );
        }

    private:
        fs::path m_initialDirectory;
    };
}

namespace ursine
{
    namespace fs
    {
        FileDialogResult FileDialog::platformImplOpen(const FileDialogConfig &config)
        {
            FileDialogResult result;

            switch (config.mode)
            {
                case FDM_OPEN:
                    if (config.allowMultipleFiles)
                        runMultiOpenFileDialog( config, result );
                    else
                        runOpenFileDialog( config, result );
                    break;
                case FDM_SAVE:
                    runSaveFileDialog( config, result );
                    break;
                case FDM_DIRECTORY:
                    runOpenFolderDialog( config, result );
                    break;
            }

            return result;
        }
    }
}

namespace
{
    std::string getCompiledFilter(const std::vector<fs::FileDialogFilter> &filters)
    {
        std::string output;

        auto allFilters = filters;

        // all files always added
        allFilters.emplace_back(
            fs::FileDialogFilter { "All Files", { "*.*" } }
        );

        for (auto &filter : filters)
        {
            // ignore filters with empty titles
            if (filter.title.empty( ))
                continue;

            std::string extensions;
            std::string extensionsDisplay;

            utils::Join( filter.extensions, ";", extensions );
            utils::Join( filter.extensions, ", ", extensionsDisplay );

            auto title = filter.title + " (" + extensionsDisplay + ")";

            // append to the output with null bytes
            output.append( title.c_str( ), title.size( ) + 1 );
            output.append( extensions.c_str( ), extensions.size( ) + 1 );
        }

        // double null terminated
        output.append( 1, '\0' );

        return output;
    }

    void runOpenFileDialog(const fs::FileDialogConfig &config, fs::FileDialogResult &output)
    {
        OPENFILENAME ofn;

        ZeroMemory( &ofn, sizeof( ofn ) );

        ofn.lStructSize = sizeof( ofn );

        ofn.hwndOwner = static_cast<HWND>(
            config.parentWindow ? config.parentWindow->GetPlatformHandle( ) : nullptr
        );

        char filename[ MAX_PATH ] { 0 };

        ofn.lpstrFile = filename;
        ofn.nMaxFile = MAX_PATH;

        if (!config.initialPath.empty( ))
            ofn.lpstrInitialDir = config.initialPath.string( ).c_str( );

        ofn.lpstrTitle = config.windowTitle.empty( ) ?
                kDefaultOpenTitle : config.windowTitle.c_str( );

        ofn.Flags = OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR | OFN_EXPLORER |
                    OFN_ENABLESIZING | OFN_NOTESTFILECREATE;

        if (config.hideReadOnlyFiles)
            ofn.Flags |= OFN_HIDEREADONLY;

        auto filter = getCompiledFilter( config.filters );

        if (!filter.empty( ))
        {
            ofn.lpstrFilter = filter.c_str( );

            // indices into lpstrFilter start at 1.
            ofn.nFilterIndex = config.selectedFilter + 1;
        }

        bool success;

        {
            WorkingDirectorySaver saver;

            success = !!GetOpenFileName( &ofn );
        }

        if (success)
        {
            output.selectedFilterIndex = ofn.nFilterIndex == 0 ? 0 : ofn.nFilterIndex - 1;
            output.selectedFiles.emplace_back( filename );
        }
    }

    void runMultiOpenFileDialog(const fs::FileDialogConfig &config, fs::FileDialogResult &output)
    {
        OPENFILENAME ofn { 0 };

        ZeroMemory( &ofn, sizeof( ofn ) );

        ofn.lStructSize = sizeof( ofn );

        ofn.hwndOwner = static_cast<HWND>(
            config.parentWindow ? config.parentWindow->GetPlatformHandle( ) : nullptr
        );

        std::shared_ptr<char> filename( new char[ UNICODE_STRING_MAX_CHARS ] );

        *filename = 0;

        ofn.lpstrFile = filename.get( );
        ofn.nMaxFile = UNICODE_STRING_MAX_CHARS ;

        if (!config.initialPath.empty( ))
            ofn.lpstrInitialDir = config.initialPath.string( ).c_str( );

        ofn.lpstrTitle = config.windowTitle.empty( ) ?
                kDefaultOpenTitle : config.windowTitle.c_str( );

        ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_EXPLORER |
                    OFN_ALLOWMULTISELECT | OFN_ENABLESIZING | OFN_NOTESTFILECREATE;

        if (config.hideReadOnlyFiles)
            ofn.Flags |= OFN_HIDEREADONLY;

        auto filter = getCompiledFilter( config.filters );

        if (!filter.empty( ))
        {
            ofn.lpstrFilter = filter.c_str( );

            // indices into lpstrFilter start at 1.
            ofn.nFilterIndex = config.selectedFilter + 1;
        }

        bool success;

        {
            WorkingDirectorySaver saver;

            success = !!GetOpenFileName( &ofn );
        }

        if (success)
        {
            fs::PathList files;

            auto *selection = ofn.lpstrFile;

            // empty string indicates end of list
            while (*selection)
            {
                files.emplace_back( selection );

                // skip over filename and null-terminator
                selection += files.back( ).string( ).length( ) + 1;
            }

            if (files.empty( ))
            {
                success = false;
            }
            else if (files.size( ) == 1)
            {
                // when there is one file, it contains the path and filename
                output.selectedFiles.swap( files );
            }
            else
            {
                // otherwise, the first string is the path, and the remainder
                // are filenames
                auto path = files.begin( );

                for (auto it = path + 1; it != files.end( ); ++it)
                    output.selectedFiles.emplace_back( *path / *it );
            }
        }

        if (success)
            output.selectedFilterIndex = ofn.nFilterIndex == 0 ? 0 : ofn.nFilterIndex - 1;
    }

    void runSaveFileDialog(const fs::FileDialogConfig &config, fs::FileDialogResult &output)
    {
        OPENFILENAME ofn;

        ZeroMemory( &ofn, sizeof( ofn ) );

        ofn.lStructSize = sizeof( ofn );

        ofn.hwndOwner = static_cast<HWND>(
            config.parentWindow ? config.parentWindow->GetPlatformHandle( ) : nullptr
        );

        char filename[ MAX_PATH ] { 0 };

        ofn.lpstrFile = filename;
        ofn.nMaxFile = MAX_PATH;

        if (!config.initialPath.empty( ))
            ofn.lpstrInitialDir = config.initialPath.string( ).c_str( );

        ofn.lpstrTitle = config.windowTitle.empty( ) ?
                kDefaultSaveTitle : config.windowTitle.c_str( );

        ofn.Flags = OFN_EXPLORER | OFN_ENABLESIZING | OFN_NOCHANGEDIR |
                                   OFN_PATHMUSTEXIST | OFN_NOTESTFILECREATE;

        if (config.hideReadOnlyFiles)
            ofn.Flags |= OFN_HIDEREADONLY;

        if (config.showOverwritePrompt)
            ofn.Flags |= OFN_OVERWRITEPROMPT;

        auto filter = getCompiledFilter( config.filters );

        if (!filter.empty( ))
        {
            ofn.lpstrFilter = filter.c_str( );

            // indices into lpstrFilter start at 1.
            ofn.nFilterIndex = config.selectedFilter + 1;

            // if a filter is specified and the default file name is changed then append
            // a file extension to the new name.
            ofn.lpstrDefExt = "";
        }

        bool success;

        {
            WorkingDirectorySaver saver;

            success = !!GetSaveFileName( &ofn );
        }

        if (success)
        {
            output.selectedFilterIndex = ofn.nFilterIndex == 0 ? 0 : ofn.nFilterIndex - 1;

            output.selectedFiles.emplace_back( filename );
        }
    }

    void runOpenFolderDialog(const fs::FileDialogConfig &config, fs::FileDialogResult &output)
    {
        char dirBuffer[ MAX_PATH + 1 ] { 0 };

        BROWSEINFO browseInfo { 0 };

        browseInfo.hwndOwner = static_cast<HWND>(
            config.parentWindow ? config.parentWindow->GetPlatformHandle( ) : nullptr
        );

        browseInfo.pszDisplayName = dirBuffer;
        browseInfo.ulFlags = BIF_USENEWUI | BIF_RETURNONLYFSDIRS;

        browseInfo.lpszTitle = config.windowTitle.empty( ) ?
                kDefaultSaveTitle : config.windowTitle.c_str( );

        auto initialPath = config.initialPath.string( );

        if (!initialPath.empty( ))
        {
            // highlight the current value
            browseInfo.lParam = reinterpret_cast<LPARAM>( initialPath.c_str( ) );
            browseInfo.lpfn = &folderBrowserCallback;
        }

        auto list = SHBrowseForFolder( &browseInfo );

        if (list)
        {
            STRRET outDirBuffer;

            ZeroMemory( &outDirBuffer, sizeof( outDirBuffer ) );

            outDirBuffer.uType = STRRET_WSTR;

            IShellFolder *shellFolder;

            if (SHGetDesktopFolder( &shellFolder ) == NOERROR)
            {
                HRESULT hr = shellFolder->GetDisplayNameOf(
                    list,
                    SHGDN_FORPARSING,
                    &outDirBuffer
                );

                if (SUCCEEDED( hr ) && outDirBuffer.uType == STRRET_WSTR)
                {
                    output.selectedFiles.emplace_back( outDirBuffer.pOleStr );

                    CoTaskMemFree( outDirBuffer.pOleStr );
                }
                else
                {
                    // use old way if we don't get what we want
                    char oldOutDirBuffer[ MAX_PATH + 1 ];

                    if (SHGetPathFromIDList( list, oldOutDirBuffer ))
                    {
                        output.selectedFiles.emplace_back( oldOutDirBuffer );
                    }
                }

                shellFolder->Release( );
            }

            CoTaskMemFree( list );
        }
    }

    int CALLBACK folderBrowserCallback(HWND window, UINT message, LPARAM parameter, LPARAM data)
    {
        if (message == BFFM_INITIALIZED)
        {
            // WParam is TRUE since passing a path.
            // data lParam member of the BROWSEINFO structure.
            SendMessage( window, BFFM_SETSELECTION, true, static_cast<LPARAM>( data ) );
        }

        return 0;
    }
}

#endif // defined(PLATFORM_WINDOWS)

