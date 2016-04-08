#pragma once

enum ProjectGameInstallerEventType
{
    PIB_BUILD_START,
    PIB_BUILD_COMPLETE
};

struct ProjectGameInstallerBuildCompleteArgs : ursine::EventArgs
{
    bool successful;
    std::string error;
};