/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** CharacterFireControllerComponent.h
**
** Author:
** - Matt Yan - m.yan@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#pragma once

#include <Component.h>
#include <Color.h>

class CharacterFireController : public ursine::ecs::Component
{
    NATIVE_COMPONENT;

public:
    EditorField(
        float fireRate,
        GetFireRate,
        SetFireRate
    );

    EditorField(
        float fireRange,
        GetFireRange,
        SetFireRange
    );

    EditorField(
        float accuracy,
        GetAccuracy,
        SetAccuracy
    );

    EditorField(
        float damage,
        GetDamage,
        SetDamage
    );

    EditorField(
        ursine::SVec3 fireOffset,
        GetFireOffset,
        SetFireOffset
    );

    EditorField(
        bool fireOnTrigger,
        GetTriggerFire,
        SetTriggerFire
    );

    EditorField(
        ursine::Color bulletStartColor,
        GetShotStartColor,
        SetShotStartColor
    );

    EditorField(
        ursine::Color bulletEndColor,
        GetShotEndColor,
        SetShotEndColor
    );

public:
    CharacterFireController(void);
    
    float GetFireRate(void) const;
    void SetFireRate(const float fireRate);

    void Fire(void);

    float GetFireTimer(void) const;
    void DecrementFireTimer(const float dt);

    void SetFireState(const bool fireState);
    bool CanFire(void) const;

    float GetFireRange(void) const;
    void SetFireRange(const float range);

    const ursine::SVec3 &GetFireOffset(void) const;
    void SetFireOffset(const ursine::SVec3 &offset);

    bool GetTriggerFire(void) const;
    void SetTriggerFire(const bool useTriggerFire);

    ursine::Color GetShotStartColor(void) const;
    void SetShotStartColor(const ursine::Color &shotColor);

    ursine::Color GetShotEndColor( void ) const;
    void SetShotEndColor( const ursine::Color &shotColor );

    float GetAccuracy(void) const;
    void SetAccuracy(const float accuracy);

    float GetDamage(void) const;
    void SetDamage(const float damage);

private:
    float m_fireRate;
    float m_fireRange;
    ursine::SVec3 m_fireOffset;
    bool m_triggerFire;

    float m_fireTimer;
    bool m_canFire;

    float m_accuracy;
    float m_damage;

    ursine::Color m_shotStartColor;
    ursine::Color m_shotEndColor;
}Meta( Enable, DisplayName( "CharacterFireController" ) );