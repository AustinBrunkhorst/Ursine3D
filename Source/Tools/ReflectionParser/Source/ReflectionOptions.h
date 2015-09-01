#pragma once

struct ReflectionOptions
{
    std::string targetName;

    std::string sourceFile;

    std::string outputHeaderFile;
    std::string outputSourceFile;

    std::vector<const char *> arguments;
};