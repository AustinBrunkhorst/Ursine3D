#pragma once

struct ProjectInstallSettings
{
    std::string gameName;
    std::string publisherName;
    std::string displayName;
    std::string brandingText;
    unsigned versionMajor;
    unsigned versionMinor;
    unsigned versionBuild;
    std::string versionDisplayName;
    std::string defaultStartMenuLocation;

    ProjectInstallSettings(void) = default;
} Meta(Enable);