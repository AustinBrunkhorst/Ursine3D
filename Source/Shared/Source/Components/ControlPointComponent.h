/* ----------------------------------------------------------------------------
** Team Bear King
** © 2016 DigiPen Institute of Technology, All Rights Reserved.
**
** ControlPointComponent.h
**
** Author:
** - Chad George - chad.george@digipen.edu
**
** --------------------------------------------------------------------------*/

#pragma once

#include <Component.h>

class ControlPoint : public ursine::ecs::Component
{
    NATIVE_COMPONENT ;

public:

    ControlPoint(void);

    EditorField(
        float Timer,
        GetControlTimer,
        SetControlTimer
        );
    float GetControlTimer(void) const;
    void SetControlTimer(const float time);

    bool GetActive(void) const;

    void DecrementTimer(const float dt);

private:
    // timer for how long control point is active
    float m_controlTimer;
    
    bool m_active;

    void Activate(EVENT_HANDLER(ursine::ecs::ENTITY_COLLISION_PERSISTED));


} Meta(Enable, DisplayName("ControlPoint"));
