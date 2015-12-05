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
