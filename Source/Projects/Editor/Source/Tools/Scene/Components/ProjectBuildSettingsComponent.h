/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** ProjectBuildSettings.h
**
** Author:
** - Austin Brunkhorst - a.brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#pragma once

#include <Component.h>

#include <WorldData.h>

#if defined(__REFLECTION_PARSER__)

#define EMPLACE_FIELD(type, name) 

#else

#define EMPLACE_FIELD(type, name) type name

#endif

#define GETTER_SETTER(type, name)                \
    const type &Get##name(void) { return name; } \
    void Set##name(const type &value)            \
    {                                            \
        name = value;                            \
        NOTIFY_COMPONENT_CHANGED( #name, name ); \
    }                                            \

#define DECLARE_SETTING(type, name)                 \
    EMPLACE_FIELD( type, name );                    \
    EditorField( type name, Get##name, Set##name ); \
    GETTER_SETTER( type, name )                     \

#define DECLARE_RESOURCE_SETTING(type, name)                     \
    EMPLACE_FIELD( ursine::resources::ResourceReference, name ); \
    EditorResourceField( type, name, Get##name, Set##name );     \
    GETTER_SETTER( ursine::resources::ResourceReference, name )  \


class ProjectBuildSettings : public ursine::ecs::Component
{
    NATIVE_COMPONENT;

public:
    ProjectBuildSettings(void);
    ~ProjectBuildSettings(void);
    
    EditorButton(Save, "Save Settings");

    DECLARE_SETTING( bool, launchAfterBuild );

    DECLARE_SETTING( std::string, windowTitle );
    DECLARE_SETTING( bool, windowFullScreen );
    DECLARE_SETTING( bool, windowResizable );
    DECLARE_RESOURCE_SETTING( ursine::resources::WorldData, startingWorld );

private:
    void OnInitialize(void) override;
} Meta(
    Enable, 
    DisableSerialization,
    HiddenInSelector, 
    DisableComponentRemoval
);

#undef EMPLACE_FIELD
#undef GETTER_SETTER
#undef DECLARE_SETTING
#undef DECLARE_RESOURCE_SETTING