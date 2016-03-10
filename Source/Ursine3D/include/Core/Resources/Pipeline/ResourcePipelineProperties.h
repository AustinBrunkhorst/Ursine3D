#pragma once

#include "Array.h"

#include <boost/regex.hpp>

typedef std::initializer_list<std::string> ExtensionList;
typedef ursine::Array<std::string> FileExclusionList;

// Expression to match all files with the given extensions
#define SYNC_EXPR_ALL_EXTENSIONS(extensions) "^(?!.*[.](" extensions ")$).*$"

class ResourceImporterConfig : public ursine::meta::MetaProperty
{
    META_OBJECT;

public:
    const ursine::Array<std::string> fileExtensions;
    const ursine::meta::Type defaultProcessor;

    ResourceImporterConfig(const std::string &extension, const ursine::meta::Type &defaultProcessor);
    ResourceImporterConfig(const ExtensionList &extensions, const ursine::meta::Type &defaultProcessor);
};

///////////////////////////////////////////////////////////////////////////////

class ResourceProcessorConfig : public ursine::meta::MetaProperty
{
    META_OBJECT;

public:
    const ursine::meta::Type optionsType;

    ResourceProcessorConfig(const ursine::meta::Type &optionsType);
};

///////////////////////////////////////////////////////////////////////////////

class ResourceSyncConfig : public ursine::meta::MetaProperty
{
    META_OBJECT;

public:
    const FileExclusionList exclusionExpressions;

    ResourceSyncConfig(const FileExclusionList &exclusionExpressions);

    Meta(Disable)
    const std::vector<boost::regex> &GetBuiltExclusionExpressions(void) const;

    Meta(Disable)
    bool IsExcluded(const std::string &path) const;
private:
    std::vector<boost::regex> m_exclusionExpressions;
};