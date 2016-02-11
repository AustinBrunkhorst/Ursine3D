/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
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

	EditorField(
		float yRotationDegrees,
		GetYRotationDegrees,
		SetYRotationDegrees
	);

    Spawnpoint(void);

    int GetTeamNumber(void) const;
    void SetTeamNumber(int teamNo);

    int GetRoundSpawnNumber(void) const;
    void SetRoundSpawnNumber(int spawnNo);

	float GetYRotationDegrees(void) const;
	void SetYRotationDegrees(float degrees);

private:
    int m_teamNumber;

    int m_roundSpawnNumber;

	float m_yRotationDegrees;
    
} Meta(Enable, DisplayName("Spawnpoint"));
