/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** SpawnpointComponent.h
**
** Author:
** - Joshua Shlemmer - joshua.shlemmer@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#pragma once

#include <Component.h>

class Spawnpoint : public ursine::ecs::Component
{
    NATIVE_COMPONENT;

public:

    EditorField(
        int TeamNumber,
        GetTeamNumber,
        SetTeamNumber
        );

    EditorField(
        int RoundSpawnNumber,
        GetRoundSpawnNumber,
        SetRoundSpawnNumber
        );

    Spawnpoint(void);

    int GetTeamNumber(void) const;
    void SetTeamNumber(int teamNo);

    int GetRoundSpawnNumber(void) const;
    void SetRoundSpawnNumber(int spawnNo);

private:
    int m_teamNumber;

    int m_roundSpawnNumber;
    
} Meta(Enable, DisplayName("Spawnpoint"));
