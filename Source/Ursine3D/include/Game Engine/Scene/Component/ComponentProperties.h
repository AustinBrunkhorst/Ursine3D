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

#pragma region Global Properties

/** @brief Disable serialization of this object
*/
struct DisableSerialization : ursine::meta::MetaProperty
{
    META_OBJECT;
};

#pragma endregion

#pragma region Editor Specific Properties

#if defined(URSINE_WITH_EDITOR)

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

/** @brief Makes sure this component doesn't display in the "Add Component" dialog.
*/
struct HiddenInSelector : ursine::meta::MetaProperty
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

    const std::string typeName;

    ForceEditorType(const ursine::meta::Type &type)
        : typeName( type.GetName( ) ) { }

    ForceEditorType(const char *editorName)
        : typeName( editorName ) { }
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
 *         of the listed component types
 */
struct RequiresComponents : ursine::meta::MetaProperty
{
    META_OBJECT;

    ursine::Array<ursine::meta::Type> componentTypes;

    template<typename... Types>
    RequiresComponents(Types &&...types)
        : componentTypes( { std::forward<Types>( types )... } ) { }
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

/** @brief Modifies the field to be restricted to the input range
 *         specified by min and max. This only applies to number types.
 */
struct InputRange : ursine::meta::MetaProperty
{
    META_OBJECT;

    const double min;
    const double max;
    const double step;
    const std::string format;

    /** @param min Minimum allowed value.
     *  @param max Maximum allowed value.
     *  @param step Optional input step precision. 
     *         A negative value ensures a nice default.
     *  @param format Optional format to display. "{{value}}" is replaced with the calculated value
     */
    InputRange(double min, double max, double step = -1.0, const std::string &format = "{{value.toPrecision( 4 )}}")
        : min( min )
        , max( max )
        , step( step )
        , format( format ) { }
};

/** @brief Enables mutli-line editor support on string fields
*/
struct MultiLineEditor : ursine::meta::MetaProperty
{
    META_OBJECT;
};

/** @brief Annotates a symbol so that the default tooltip is overriden
*/
struct Annotation : ursine::meta::MetaProperty
{
    META_OBJECT;

    const std::string text;

    Annotation(const char *text)
        : text( text ) { }
};

#endif

#pragma endregion
