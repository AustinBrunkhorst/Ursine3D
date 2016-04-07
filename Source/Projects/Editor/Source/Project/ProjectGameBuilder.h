#pragma once

#include "ProjectGameBuilderEvent.h"

#include <ResourcePipelineManager.h>

#include <GameSettings.h>
#include <atomic>

class Project;

class ProjectGameBuilder : 
    public ursine::EventDispatcher<ProjectGameBuilderEventType>
{
public:
    ProjectGameBuilder(Project *project);

    void Build(
        const ursine::fs::path &launcherDir, 
        const ursine::fs::path &outputDir
    );

    void Cancel(void);

    bool IsBuilding(void) const;

private:
    typedef std::pair<
        ursine::fs::path,
        ursine::fs::path
    > CopyTarget;

    std::atomic_bool m_building;
    std::atomic_bool m_buildCancelled;

    std::thread m_buildWorkerThread;

    Project *m_project;

    ursine::fs::path m_launcherDir;
    ursine::fs::path m_outputDir;

    std::vector<CopyTarget> m_copyTargets;

    void doBuild(void);
    void cleanupCancel(void);

    void collectCopyTargets(const ursine::fs::path &directory);

    void postProgress(float progress);

    std::string getProjectDisplayName(void) const;
};