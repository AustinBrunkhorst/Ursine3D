/* ----------------------------------------------------------------------------
** Team Bear King
** © 2016 DigiPen Institute of Technology, All Rights Reserved.
**
** PlayerIdComponent.h
**
** Author:
** - Chad George - chad.george@digipen.edu
**
** --------------------------------------------------------------------------*/

#pragma once

#include <Component.h>

class PlayerID : public ursine::ecs::Component
{
    NATIVE_COMPONENT ;

public:

    PlayerID(void);

    EditorField(
        PlayerId,
        GetID,
        SetID
        );

    int GetID(void) const;
    void SetID(const int id);

private:
    int m_id;

} Meta(Enable, DisplayName("PlayerID"));

