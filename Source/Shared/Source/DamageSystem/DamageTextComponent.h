#pragma once

/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** DamageTextComponent.h
**
** Author:
** - Chad George - chad.george@digipen.edu
**
** -------------------------------------------------------------------------*/


#include <Component.h>
#include "Randomizer.h"

namespace ursine
{
    class SVec3;
}


class DamageText : public ursine::ecs::Component
{
    NATIVE_COMPONENT
public:
    EditorField(
        float TotalLifeTime,
        GetTotalLifeTime,
        SetTotalLifeTime
        );

    EditorField(
        ursine::SVec3 Velocity,
        GetVelocity,
        SetVelocity
    );

    EditorField(
        ursine::SVec3 Variance,
        GetVariance,
        SetVariance
    );

    DamageText(void);
    ~DamageText(void);

    void OnInitialize(void) override;

    void GenerateVelocity(void);

    float GetTotalLifeTime(void) const;
    void SetTotalLifeTime(const float lifeTime);

    float GetLifeTime(void) const;
    void IncrementLifeTime(const float lifeTime);

    const ursine::SVec3& GetVelocity(void) const;
    void SetVelocity(const ursine::SVec3& vel);

    const ursine::SVec3 GetVariance(void) const;
    void SetVariance(const ursine::SVec3& var);

    const float GetStartAlpha(void) const;
    void SetStartAlpha(const float alpha);

private:
    // total time that i have been alive
    float m_lifeTime;

    // Total lifetime
    float m_totalLifeTime;

    // base velocity
    ursine::SVec3 m_velocity;

    // variance
    ursine::Randomizer m_varX;
    ursine::Randomizer m_varY;
    ursine::Randomizer m_varZ;

    float m_startAlpha;


} Meta(Enable, DisplayName("DamageText"));


