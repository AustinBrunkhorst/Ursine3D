#pragma once

#include "Meta.h"

/** @brief Makes sure this component can't be removed from an entity in the editor.
*/
struct DisableComponentRemoval : ursine::meta::MetaProperty
{
    META_OBJECT
};

/** @brief Makes sure this component doesn't display in the entity inspector.
*/
struct HiddenInInspector : ursine::meta::MetaProperty
{
    META_OBJECT;
};