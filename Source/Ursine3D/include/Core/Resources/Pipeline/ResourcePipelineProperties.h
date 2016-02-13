#pragma once

#include "Array.h"

class ResourceImporterConfig : public ursine::meta::MetaProperty
{
public:
    const ursine::Array<std::string> fileExtensions;
    const ursine::meta::Type defaultProcessor;

    ResourceImporterConfig(const std::string &&extension, const ursine::meta::Type &defaultProcessor)
        : fileExtensions(ursine::Array<std::string> { move( extension ) } )
        , defaultProcessor( defaultProcessor ) { }

    ResourceImporterConfig(const std::initializer_list<std::string> &&extensions, const ursine::meta::Type &defaultProcessor)
        : fileExtensions( move( extensions ) )
        , defaultProcessor( defaultProcessor ) { }
};

class ResourceProcessorConfig : public ursine::meta::MetaProperty
{
public:
    const ursine::meta::Type optionsType;

    ResourceProcessorConfig(const ursine::meta::Type &optionsType)
        : optionsType( optionsType ) { }
};