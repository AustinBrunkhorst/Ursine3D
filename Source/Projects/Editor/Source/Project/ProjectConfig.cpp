#include "Precompiled.h"

#include "ProjectConfig.h"

ProjectConfig ProjectConfig::CreateDefault(const std::string &title)
{
    ProjectConfig config;

    config.title = title;
    config.buildDirectory = "Build";
    config.buildDirectory = "Resources";

    return config;
}