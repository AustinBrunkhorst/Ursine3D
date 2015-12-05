#pragma once

#include <Component.h>

class TeamComponent : public ursine::ecs::Component
{
    NATIVE_COMPONENT;

public:

    EditorField(
        int TeamNumber,
        GetTeamNumber,
        SetTeamNumber
        );

    TeamComponent(void);

    int GetTeamNumber(void) const;

    void SetTeamNumber(int num);

private:
    int m_teamNumber;
    
} Meta(Enable, DisplayName("Team"));
