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

class ProjectInstallerSettings : public ursine::ecs::Component
{
    NATIVE_COMPONENT;

public:
    ProjectInstallerSettings(void);
    ~ProjectInstallerSettings(void);
    
    EditorButton( Save, "Save Settings" );

    DECLARE_SETTING( std::string, gameName );
    DECLARE_SETTING( std::string, publisherName );
    DECLARE_SETTING( std::string, displayName );
    DECLARE_SETTING( std::string, brandingText );
    DECLARE_SETTING( unsigned, versionMajor );
    DECLARE_SETTING( unsigned, versionMinor );
    DECLARE_SETTING( unsigned, versionBuild );
    DECLARE_SETTING( std::string, versionDisplayName );
    DECLARE_SETTING( std::string, defaultStartMenuLocation );

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