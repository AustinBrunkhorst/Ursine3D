#include "Precompiled.h"

#include "ProjectGameInstaller.h"
#include "Project.h"

#include "Mustache.h"

using namespace ursine;

typedef Mustache::Mustache<std::string> Template;
typedef Mustache::Data<std::string> TemplateData;

namespace
{
    const auto kProjectIconFile = "Installer/Icon.ico";
    const auto kProjectLicenseFile = "Installer/License.txt";

    const auto kDefaultIconFile = "Resources/Installer/DefaultIcon.ico";
    const auto kDefaultLicenseFile = "Resources/Installer/DefaultLicense.txt";

    const auto kInstallScriptTemplateFile = "Resources/Installer/Installer.template.nsi";
    const auto kInstallOptionsFile = "Resources/Installer/InstallOptions.ini";
    const auto kRedistributablesDir = "Resources/Installer/Redistributable";

    const std::vector<fs::path> kRedistributableFiles {
        "D3D11InstallHelper.dll",
        "d3dredist_x64.exe",
        "vcredist140_x64.exe"
    };

    // temp directory where the script and such are written to (in the project folder)
    const auto kTempInstallerDir = "Temp/Installer";
    const auto kTempIconFile = "Icon.ico";
    const auto kTempLicenseFile = "License.txt";
    const auto kTempScriptFile = "Installer.nsi";
    const auto kTempInstallOptionsFile = "InstallOptions.ini";

    const auto kNSISExecutable = "makensis.exe";
    const auto kExecutableExtension = "exe";
    const auto kInstallerPrefix = "Install";

    bool runNSIS(const std::vector<std::string> &arguments);
}

ProjectGameInstaller::ProjectGameInstaller(Project *project)
    : EventDispatcher( this )
    , m_building( false )
    , m_project( project ) { }

void ProjectGameInstaller::Build(
    const fs::path &buildDir,
    const fs::path &outputDir
)
{
    if (m_building)
        return;

    m_building = true;

    m_buildDir = buildDir;
    m_outputDir = outputDir;

    m_buildWorkerThread = std::thread( &ProjectGameInstaller::doBuild, this );

    if (m_buildWorkerThread.joinable( ))
        m_buildWorkerThread.detach( );
}

bool ProjectGameInstaller::IsBuilding(void) const
{
    return m_building;
}

void ProjectGameInstaller::doBuild(void)
{
    Dispatch( PIB_BUILD_START, EventArgs::Empty );

    // note: copy for threading purposes
    auto config = m_project->GetConfig( );
    auto settings = m_project->GetConfig( ).installSettings;
    
    auto projectDir = config.projectFile.parent_path( );
    auto tempProjectDir = projectDir / config.buildDirectory / kTempInstallerDir;

    fs::path redistDir = kRedistributablesDir;
    auto buildRedistDir = m_buildDir / "redist";

    auto tempIconFile = tempProjectDir / kTempIconFile;
    auto tempLicenseFile = tempProjectDir / kTempLicenseFile;
    auto tempInstallOptionsFile = tempProjectDir / kTempInstallOptionsFile;
    auto tempScriptFile = tempProjectDir / kTempScriptFile;

    ProjectGameInstallerBuildCompleteArgs completionEvent;

    TemplateData scriptData;

    try
    {
        auto displayName = getProjectDisplayName( );

        auto executableFile = fs::path( displayName )
            .replace_extension( kExecutableExtension );

        UAssertCatchable( exists( m_buildDir / executableFile ),
            "Build directory missing executable.\nexpected: %s",
            executableFile.string( ).c_str( )
        );

        auto outputInstaller = (m_outputDir / (kInstallerPrefix + displayName))
            .replace_extension( kExecutableExtension );

        scriptData[ "executableFile" ] = executableFile.string( );
        scriptData[ "outputInstaller" ] = outputInstaller.string( );
        scriptData[ "configBuildDirectory" ] = m_buildDir.string( );
        scriptData[ "configGameName" ] = settings.gameName;
        scriptData[ "configPublisher" ] = settings.publisherName;
        scriptData[ "configDisplayName" ] = settings.displayName;
        scriptData[ "configBrandingText" ] = settings.brandingText;
        scriptData[ "configDefaultStartMenu" ] = settings.defaultStartMenuLocation;
        scriptData[ "configVersionMajor" ] = std::to_string( settings.versionMajor );
        scriptData[ "configVersionMinor" ] = std::to_string( settings.versionMinor );
        scriptData[ "configVersionBuild" ] = std::to_string( settings.versionBuild );
        scriptData[ "configVersionDisplayName" ] = settings.versionDisplayName;

        // create the temporary directory if it doesn't exist
        if (!exists( tempProjectDir ))
            create_directories( tempProjectDir );

        fs::path iconFile;

        // detect icon file
        if (exists( projectDir / kProjectIconFile ))
            iconFile = projectDir / kProjectIconFile;
        else
            iconFile = kDefaultIconFile;

        copy_file( iconFile, tempIconFile, fs::copy_option::overwrite_if_exists );

        fs::path licenseFile;

        // detect license file
        if (exists( projectDir / kProjectLicenseFile ))
            licenseFile = projectDir / kProjectLicenseFile;
        else
            licenseFile = kDefaultLicenseFile;

        copy_file( licenseFile, tempLicenseFile, fs::copy_option::overwrite_if_exists );

        copy_file( kInstallOptionsFile, tempInstallOptionsFile, fs::copy_option::overwrite_if_exists );

        scanBuildDirectory( );

        if (!exists( buildRedistDir ))
            create_directories( buildRedistDir );

        for (auto &file : kRedistributableFiles)
            copy_file( redistDir / file, buildRedistDir / file, fs::copy_option::overwrite_if_exists );

        TemplateData buildFileData { TemplateData::Type::List };
        TemplateData buildDirectoryData { TemplateData::Type::List };

        for (auto &file : m_buildFiles)
        {
            TemplateData fileData;

            fileData[ "path" ] = file.string( );

            buildFileData << fileData;
        }

        scriptData[ "buildFile" ] = buildFileData;
           
        for (auto &directory : m_buildDirectories)
        {
            TemplateData directoryData;

            directoryData[ "path" ] = directory.string( );

            buildDirectoryData << directoryData;
        }

        scriptData[ "buildDirectory" ] = buildDirectoryData;

        std::string scriptTemplateText;

        UAssertCatchable( fs::LoadAllText( kInstallScriptTemplateFile, scriptTemplateText ),
            "Unable to load installer script template.\nfile: %s",
            kInstallScriptTemplateFile
        );

        Template script( scriptTemplateText );

        UAssertCatchable( fs::WriteAllText( tempScriptFile.string( ), script.render( scriptData ) ),
            "Unable to write script template file.\nfile: %s",
            tempScriptFile.string( ).c_str( )
        );

        std::vector<std::string> args {
            "\""+ (tempProjectDir / kTempScriptFile).string( ) +"\""
        };

        UAssertCatchable( runNSIS( args ),
            "Failed to run '%s'. Make sure NSIS is installed and added to your path.",
            kNSISExecutable
        );
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

    // cleanup
    try
    {
        remove_all( tempProjectDir );
    }
    catch (...) { }

    try
    {
        remove_all( buildRedistDir );
    } catch (...) { }

    Dispatch( PIB_BUILD_COMPLETE, &completionEvent );

    m_building = false;
}

void ProjectGameInstaller::scanBuildDirectory(void)
{
    fs::RecursiveDirectoryIterator it( m_buildDir );
    fs::RecursiveDirectoryIterator itEnd;

    for (; it != itEnd; ++it)
    {
        auto &entry = it->path( );
        auto relative = fs::MakeRelativePath( m_buildDir, entry );

        if (is_directory( entry ))
        {
            m_buildDirectories.insert( m_buildDirectories.begin( ), relative );
        }
        else
        {
            m_buildFiles.emplace_back( relative );
        }
    }
}

std::string ProjectGameInstaller::getProjectDisplayName(void) const
{
    return fs::SafeFileName( m_project->GetConfig( ).title, '_' );
}

namespace
{
    bool runNSIS(const std::vector<std::string> &arguments)
    {
        std::string argString;

        utils::Join( arguments, " ", argString );

        SHELLEXECUTEINFO shExecInfo;

        shExecInfo.cbSize = sizeof( SHELLEXECUTEINFO );
        shExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
        shExecInfo.hwnd = nullptr;
        shExecInfo.lpVerb = nullptr;
        shExecInfo.lpFile = kNSISExecutable;
        shExecInfo.lpParameters = argString.c_str( );
        shExecInfo.lpDirectory = nullptr;
        shExecInfo.nShow = SW_HIDE;
        shExecInfo.hInstApp = nullptr;

        auto result = ShellExecuteEx( &shExecInfo );

        if (result != TRUE)
            return false;

        WaitForSingleObject( shExecInfo.hProcess, INFINITE );

        CloseHandle( shExecInfo.hProcess );

        return true;
    }
}