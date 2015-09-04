#pragma once

struct ReflectionOptions
{
    std::string targetName;

    std::string inputSourceFile;

    std::string outputHeaderFile;
    std::string outputSourceFile;

    std::string templateDirectory;

    std::vector<const char *> arguments;
};