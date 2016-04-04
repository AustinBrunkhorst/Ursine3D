#pragma once

enum ProjectGameBuilderEventType
{
    PGB_BUILD_START,
    PGB_BUILD_PROGRESS,
    PGB_BUILD_COMPLETE
};

struct ProjectGameBuildProgressArgs : ursine::EventArgs
{
    float progress;

    ProjectGameBuildProgressArgs(float progress)
        : progress( progress ) { }
};

struct ProjectGameBuildCompleteArgs : ursine::EventArgs
{
    bool successful;
    std::string error;
};