#pragma once

#include "Array.h"

typedef std::initializer_list<std::string> ExtensionList;

class ResourceImporterConfig : public ursine::meta::MetaProperty
{
    META_OBJECT;

public:
    const ursine::Array<std::string> fileExtensions;
    const ursine::meta::Type defaultProcessor;

    ResourceImporterConfig(const std::string &extension, const ursine::meta::Type &defaultProcessor)
        : fileExtensions(ursine::Array<std::string> { extension } )
        , defaultProcessor( defaultProcessor )
    {
        auto &extensions = const_cast<ursine::Array<std::string>&>( fileExtensions );

        ursine::utils::MakeLowerCase( extensions[ 0 ] );
    }

    ResourceImporterConfig(const ExtensionList &extensions, const ursine::meta::Type &defaultProcessor)
        : fileExtensions( extensions )
        , defaultProcessor( defaultProcessor )
    {
        for (auto &extension : const_cast<ursine::Array<std::string>&>( fileExtensions ))
            ursine::utils::MakeLowerCase( extension );
    }
};

class ResourceProcessorConfig : public ursine::meta::MetaProperty
{
    META_OBJECT;

public:
    const ursine::meta::Type optionsType;

    ResourceProcessorConfig(const ursine::meta::Type &optionsType)
        : optionsType( optionsType ) { }
};