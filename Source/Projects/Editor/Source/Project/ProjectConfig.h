#pragma once

struct ProjectConfig
{
    std::string title;
    std::string resourceDirectory;
    std::string buildDirectory;

    static ProjectConfig CreateDefault(const std::string &title);
} Meta(Enable);