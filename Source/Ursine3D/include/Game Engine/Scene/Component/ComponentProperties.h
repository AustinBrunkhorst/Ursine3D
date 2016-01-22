/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** ComponentProperties.h
**
** Author:
** - Austin Brunkhorst - a.brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

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

/** @brief Disable serialization of this object
 */
struct DisableSerialization : ursine::meta::MetaProperty
{
    META_OBJECT;
};

/** @brief Displays an enum field as a bit mask editor
 */
struct BitMaskEditor : ursine::meta::MetaProperty
{
    META_OBJECT;
};

/** @brief Forces the editor to use a specific type editor for a field
 */
struct ForceEditorType : ursine::meta::MetaProperty
{
    META_OBJECT;

    const std::string type;

    ForceEditorType(const char *type)
        : type( type ) { }
};

/** @brief Field getter only used when interacting with the editor
 */
struct EditorGetter : ursine::meta::MetaProperty
{
    META_OBJECT;

    const std::string getter;

    EditorGetter(const char *getter)
        : getter( getter ) { }
};

/** @brief Field setter only used when interacting with the editor
 */
struct EditorSetter : ursine::meta::MetaProperty
{
    META_OBJECT;

    const std::string setter;

    EditorSetter(const char *setter)
        : setter( setter ) { }
};

/** @brief Declares that this component type requires all 
 * of the listed component types
 */
struct RequiresComponents : ursine::meta::MetaProperty
{
    META_OBJECT;

    Meta(Disable)
    std::vector<ursine::meta::Type> componentTypes;

    template<typename... Types>
    RequiresComponents(Types &&...types)
        : componentTypes( { std::forward( types... ) } ) { }
};

/** @brief Treats a component method as a button in the editor
 */
struct CreateButton : ursine::meta::MetaProperty
{
    META_OBJECT;

    const std::string text;

    CreateButton(const char *text)
        : text( text ) { }
};
