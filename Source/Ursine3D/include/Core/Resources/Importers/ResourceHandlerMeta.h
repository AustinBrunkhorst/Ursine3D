#pragma once

#include "Meta.h"

/** @brief Configures a ResourceHandler
*/
struct ResourceHandlerConfig : ursine::meta::MetaProperty
{
    META_OBJECT

    const std::vector<std::string> extensions;
    const ursine::meta::Type optionsType;

    /** @param extension file extension in which this importer handles.
     *  @param optionsType ResourceImportOptions type this importer expects.
     */
    ResourceHandlerConfig(const std::initializer_list<std::string> &extensions, const ursine::meta::Type &optionsType)
        : extensions( extensions )
        , optionsType( optionsType ) { }
};