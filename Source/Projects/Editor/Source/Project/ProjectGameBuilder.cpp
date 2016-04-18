/* ----------------------------------------------------------------------------
** Team Bear King
** © 2016 DigiPen Institute of Technology, All Rights Reserved.
**
** ProjectGameBuilder.cpp
**
** Author:
** - Austin Brunkhorst - a.brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#include "Precompiled.h"

#include "ProjectGameBuilder.h"
#include "Project.h"

#define DELIMIT_PATTERN "|"
#define MATCH_CONTAINS(text) "(" text ")"
#define MATCH_FILE_NAME(name) "((" name ")$)"
#define MATCH_EXTENSION(extension) "(\\.(" extension ")$)"

#define CHECK_CANCELLED if (m_buildCancelled) { cleanupCancel( ); return; }

using namespace ursine;

namespace
{
    // path the the exectuable in a given launcher directory
    const auto kGameLauncherExecutable = "GameLauncher.exe";
    const auto kExecutableExtension = "exe";

    // directory in the output where the build resources are copied
    const auto kGameContentDirectory = "Resources/Content";
    const auto kGameConfigFile = "Resources/Launcher.config";

    const auto kLauncherDirectoryIgnorePattern = boost::regex(
        // Intellij stuff
        MATCH_CONTAINS( "\\.idea" )
    );

    const auto kLauncherFileIgnorePattern = boost::regex(
        // error log stuff
        MATCH_FILE_NAME( "ErrorLog\\.txt" )         DELIMIT_PATTERN
        MATCH_FILE_NAME( "ErrorLog\\.Debug\\.txt" ) DELIMIT_PATTERN

        // Editor stuff
        MATCH_FILE_NAME( "devtools_resources\\.pak" ) DELIMIT_PATTERN

        // haxe source files
        MATCH_EXTENSION( "hx" )                     DELIMIT_PATTERN
        MATCH_EXTENSION( "hxml" )                   DELIMIT_PATTERN

        // preprocessed CSS
        MATCH_EXTENSION( "scss" )                   DELIMIT_PATTERN

        // build artifacts
        MATCH_EXTENSION( "exp" )                    DELIMIT_PATTERN
        MATCH_EXTENSION( "ilk" )                    DELIMIT_PATTERN
        MATCH_EXTENSION( "lib" )                    DELIMIT_PATTERN
        MATCH_EXTENSION( "pdb" )                    DELIMIT_PATTERN

        // Intellij stuff
        MATCH_EXTENSION( "iml" )    
    );
}

ProjectGameBuilder::ProjectGameBuilder(Project *project)
    : EventDispatcher( this )
    , m_building( false )
    , m_project( project ) { }

void ProjectGameBuilder::Build(
    const fs::path &launcherDir,
    const fs::path &outputDir
)
{
    if (m_building)
        return;

    m_building = true;

    m_launcherDir = launcherDir;
    m_outputDir = outputDir / getProjectDisplayName( );

    m_buildCancelled = false;
    m_copyTargets.clear( );

    m_buildWorkerThread = std::thread( &ProjectGameBuilder::doBuild, this );

    if (m_buildWorkerThread.joinable( ))
        m_buildWorkerThread.detach( );
}

void ProjectGameBuilder::Cancel(void)
{
    m_buildCancelled = true;
    m_building = false;

    ProjectGameBuildCompleteArgs cancelEvent;

    cancelEvent.successful = false;
    cancelEvent.error = "Build was cancelled.";

    Dispatch( PGB_BUILD_COMPLETE, &cancelEvent );
}

bool ProjectGameBuilder::IsBuilding(void) const
{
    return m_building;
}

void ProjectGameBuilder::doBuild(void)
{
    Dispatch( PGB_BUILD_START, EventArgs::Empty );

    // note: copy for threading purposes
    auto config = m_project->GetConfig( );
    auto settings = config.buildSettings;

    ProjectGameBuildCompleteArgs completionEvent;

    auto outputExecutablePath = (m_outputDir / getProjectDisplayName( ))
        .replace_extension( kExecutableExtension );

    auto outputConfigPath = m_outputDir / kGameConfigFile;

    try
    {
        // make sure the launcher executable exists
        UAssertCatchable( exists( m_launcherDir / kGameLauncherExecutable ),
            "Launcher build directory missing file '%s'.\ndirectory: %s",
            kGameLauncherExecutable,
            m_launcherDir.string( ).c_str( )
        );

        // make sure the output directory exists
        if (!exists( m_outputDir ))
        {
            UAssertCatchable( create_directories( m_outputDir ),
                "Failed to create output directory.\ndirectory: %s",
                m_outputDir.string( ).c_str( )
            );
        }

        CHECK_CANCELLED;

        collectCopyTargets( m_launcherDir );

        CHECK_CANCELLED;
        
        auto &pipeline = m_project->GetResourcePipeline( );
        auto &db = pipeline.GetDatabase( );

        auto &buildDir = pipeline.GetConfig( ).buildDirectory;

        auto outputContentDir = m_outputDir / kGameContentDirectory;

        for (auto &entry : db)
        {
            auto &buildPath = entry.second->GetBuildFileName( );
            auto relativeBuildDir = fs::MakeRelativePath( buildDir, buildPath );

            m_copyTargets.emplace_back(
                buildPath,
                outputContentDir / relativeBuildDir
            );
        }

        CHECK_CANCELLED;

        auto totalTargets = static_cast<float>( m_copyTargets.size( ) );

        // copy all of the files
        for (size_t i = 0; i < m_copyTargets.size( ); ++i)
        {
            postProgress( (i + 1) / totalTargets );

            auto &target = m_copyTargets[ i ];
            auto &from = target.first;
            auto &to = target.second;

            // make sure the directory exists
            if (!exists( to.parent_path( ) ))
                create_directories( to.parent_path( ) );

            copy_file( from, to, fs::copy_option::overwrite_if_exists );

            CHECK_CANCELLED;
        }

        // rename the executable to respect the project name
        fs::rename( 
            m_outputDir / kGameLauncherExecutable,
            outputExecutablePath 
        );

        auto settingsJson = meta::Type::SerializeJson<GameSettings>( settings );

        // make sure the directory exists
        if (!exists( outputConfigPath.parent_path( ) ))
            create_directories( outputConfigPath.parent_path( ) );

        // write the configuration file
        UAssertCatchable( 
            fs::WriteAllText( 
                outputConfigPath.string( ), 
                settingsJson.dump( false ) 
            ),
            "Unable to write configuration file.\nfile: %s",
            outputConfigPath.string( ).c_str( )
        );

        CHECK_CANCELLED;

        // open the directory in explorer
        utils::OpenPath( m_outputDir.string( ) );

        CHECK_CANCELLED;

        // launch the exectuable if applicable
        if (config.launchAfterBuild)
        {
        #if defined(PLATFORM_WINDOWS)

            SHELLEXECUTEINFO shExecInfo;

            shExecInfo.cbSize = sizeof( SHELLEXECUTEINFO );
            shExecInfo.fMask = 0;
            shExecInfo.hwnd = nullptr;
            shExecInfo.lpVerb = nullptr;
            shExecInfo.lpFile = outputExecutablePath.string( ).c_str( );
            shExecInfo.lpParameters = "";
            shExecInfo.lpDirectory = nullptr;
            shExecInfo.nShow = SW_SHOW;
            shExecInfo.hInstApp = nullptr;

            auto result = ShellExecuteEx( &shExecInfo );

            UAssertCatchable( result == TRUE,
                "Unable to spawn built launcher.\nfile: %s",
                outputExecutablePath.c_str( )
            );

        #endif
        }

        completionEvent.successful = true;
    }
    catch (const AssertionException &e)
    {
        completionEvent.successful = false;
        completionEvent.error = e.GetErrorMessage( );
    }
    catch (fs::filesystem_error &e)
    {
        completionEvent.successful = false;
        completionEvent.error = "File System error ("+ 
            std::to_string( e.code( ).value( ) ) +"): "+ e.what( );
    }
    catch (...)
    {
        completionEvent.successful = false;
        completionEvent.error = "Uncaught exception.";
    }

    // cleanup directories if not successful
    if (!completionEvent.successful)
        cleanupCancel( );

    Dispatch( PGB_BUILD_COMPLETE, &completionEvent );

    m_building = false;
}

void ProjectGameBuilder::cleanupCancel(void)
{
    try
    {
        remove_all( m_outputDir );
    }
    catch (...)
    {
        // do nothing
    }
}

void ProjectGameBuilder::collectCopyTargets(const fs::path &directory)
{
    fs::DirectoryIterator it( directory );
    fs::DirectoryIterator itEnd;

    boost::smatch patternMatch;

    for (; it != itEnd; ++it)
    {
        auto &item = *it;
        auto &path = item.path( );
        auto pathStr = path.string( );

        // recursive directory if it's not excluded
        if (is_directory( item ))
        {
            if (!regex_search( pathStr, patternMatch, kLauncherDirectoryIgnorePattern ))
                collectCopyTargets( path );
        }
        // add file if it's not excluded
        else if (!regex_search( pathStr, patternMatch, kLauncherFileIgnorePattern ))
        {
            auto relativeToRoot = fs::MakeRelativePath( m_launcherDir, path );

            m_copyTargets.emplace_back( path, m_outputDir / relativeToRoot );
        }
    }
}

void ProjectGameBuilder::postProgress(float progress)
{
    ProjectGameBuildProgressArgs e( progress );

    Dispatch( PGB_BUILD_PROGRESS, &e );
}

std::string ProjectGameBuilder::getProjectDisplayName(void) const
{
    return fs::SafeFileName( m_project->GetConfig( ).title, '_' );
}