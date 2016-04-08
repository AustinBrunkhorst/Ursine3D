#pragma once

#include "ProjectGameInstallerEvent.h"

#include <ResourcePipelineManager.h>

#include <GameSettings.h>
#include <atomic>

class Project;

class ProjectGameInstaller : 
    public ursine::EventDispatcher<ProjectGameInstallerEventType>
{
public:
    ProjectGameInstaller(Project *project);

    void Build(
        const ursine::fs::path &buildDir, 
        const ursine::fs::path &outputFile
    );

    bool IsBuilding(void) const;

private:
    std::atomic_bool m_building;

    std::thread m_buildWorkerThread;

    Project *m_project;

    ursine::fs::path m_buildDir;
    ursine::fs::path m_outputFile;

    ursine::fs::PathList m_buildFiles;
    ursine::fs::PathList m_buildDirectories;

    void doBuild(void);

    void scanBuildDirectory(void);

    std::string getProjectDisplayName(void) const;
};