/* ----------------------------------------------------------------------------
** Team Bear King
** ?2016 DigiPen Institute of Technology, All Rights Reserved.
**
** PlayerSpawnPointComponent.h
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
** --------------------------------------------------------------------------*/

#pragma once

#include <Component.h>

class PlayerSpawnPoint : public ursine::ecs::Component
{
    NATIVE_COMPONENT;

public:

    PlayerSpawnPoint(void);

    EditorField(
        std::string playerArchetype,
        GetPlayerArchetype,
        SetPlayerArchetype
    );

    const std::string &GetPlayerArchetype(void) const;
    void SetPlayerArchetype(const std::string &archetype);

private:
    std::string m_archetype;

} Meta(Enable);
