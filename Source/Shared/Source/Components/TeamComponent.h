/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** TeamComponent.h
**
** Author:
** - Joshua Shlemmer - joshua.shlemmer@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

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

	void SetAlive(void);
	void SetDead(void);
	bool IsDead(void);

private:
    int m_teamNumber;

	bool m_alive;
    
} Meta(Enable, DisplayName("Team"));
