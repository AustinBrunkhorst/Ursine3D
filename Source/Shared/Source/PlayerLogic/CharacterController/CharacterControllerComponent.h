/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** CharacterControllerComponent.h
**
** Author:
** - Joshua Shlemmer - joshua.shlemmer@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#pragma once

#include <Component.h>

namespace ursine
{
    namespace ecs
    {
        class SweptController;
    }
}

class CharacterController : public ursine::ecs::Component
{
    NATIVE_COMPONENT;

public:
    friend class CharacterControllerSystem;

    CharacterController(void);
    ~CharacterController(void);

    EditorField(
        float rotateSpeed,
        GetRotateSpeed,
        SetRotateSpeed
    );

    float GetRotateSpeed(void) const;
    void SetRotateSpeed(float rotateSpeed);

    const ursine::Vec2 &GetMoveDirection(void) const;
    void SetMoveDirection(const ursine::Vec2 &moveDir);

    const ursine::Vec2 &GetLookDirection(void) const;
    void SetLookDirection(const ursine::Vec2 &lookDir);

    void OnInitialize(void) override;

private:

    void SetLookDirection(EVENT_HANDLER(game::LOOK_COMMAND));
    void Jump(EVENT_HANDLER(game::JUMP_COMMAND));

    float m_rotateSpeed;
    
    bool m_jump;

    ursine::Vec2 m_moveDir;
    ursine::Vec2 m_lookDir;

} Meta(Enable, DisplayName("CharacterController"), 
       RequiresComponents(typeof(ursine::ecs::SweptController)));
