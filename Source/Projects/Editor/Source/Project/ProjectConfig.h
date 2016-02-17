#pragma once

#include <FileSystem.h>

struct ProjectConfig
{
    std::string title;
    std::string resourceDirectory;
    std::string buildDirectory;

    Meta(Disable)
    ursine::fs::path rootDirectory;

    ProjectConfig(void) = default;

    static ProjectConfig CreateDefault(const std::string &title);
} Meta(Enable);