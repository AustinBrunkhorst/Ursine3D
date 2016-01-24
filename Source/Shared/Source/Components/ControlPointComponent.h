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
    ~ControlPoint(void);

    void OnInitialize(void) override;

    EditorField(
        float Timer,
        GetControlTimer,
        SetControlTimer
        );
    float GetControlTimer(void) const;
    void SetControlTimer(const float time);


    // timer for how long control point is active
    Meta( Disable )
    float m_controlTimer;
    
    // Is control point active
    Meta( Disable )
    bool m_active;

    void Activate(EVENT_HANDLER(ursine::ecs::ENTITY_COLLISION_PERSISTED));


} Meta(Enable, DisplayName("ControlPoint"));
