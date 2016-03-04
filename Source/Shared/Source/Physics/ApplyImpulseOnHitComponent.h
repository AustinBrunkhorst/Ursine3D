/* ---------------------------------------------------------------------------
** Team Bear King
** ?2016 DigiPen Institute of Technology, All Rights Reserved.
**
** ApplyImpulseOnHitComponent.h
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** -------------------------------------------------------------------------*/

#pragma once

#include <Component.h>
#include <SVec3.h>

class ApplyImpulseOnHit : public ursine::ecs::Component
{
    NATIVE_COMPONENT;

public:

    EditorField(
        bool effectSweptController,
        GetEffectSweptController,
        SetEffectSweptController
    );

    EditorField(
        float impulse,
        GetImpulse,
        SetImpulse
    );

    EditorField(
        ursine::SVec3 localDirectionInfluence,
        GetLocalDirectionInfluence,
        SetLocalDirectionInfluence
    );

    EditorField(
        ursine::SVec3 worldDirectionInfluence,
        GetWorldDirectionInfluence,
        SetWorldDirectionInfluence
    );

    ApplyImpulseOnHit(void);
    ~ApplyImpulseOnHit(void);

    bool GetEffectSweptController(void) const;
    void SetEffectSweptController(bool flag);

    float GetImpulse(void) const;
    void SetImpulse(float impulse);

    const ursine::SVec3 &GetLocalDirectionInfluence(void) const;
    void SetLocalDirectionInfluence(const ursine::SVec3 &direction);

    const ursine::SVec3 &GetWorldDirectionInfluence(void) const;
    void SetWorldDirectionInfluence(const ursine::SVec3 &direction);

private:

    void OnInitialize(void) override;

    void onCollision(EVENT_HANDLER(Entity));

    bool m_effectSweptController;

    float m_impulse;

    ursine::SVec3 m_localDirection;
    ursine::SVec3 m_worldDirection;

} Meta(Enable);
