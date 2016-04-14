/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** HitScanWeapon.h
**
** Author:
** - Chad George - chad.george@digipen.edu
**
** -------------------------------------------------------------------------*/

#pragma once

#include "AbstractWeapon.h"
#include <Raycasting.h>

const float MAX_GUN_DAMAGE = 1440.0f;


#define HitscanEditorFields( )                     \
    EditorField(                                   \
        ursine::physics::RaycastType RaycastType,  \
        GetRaycastType,                            \
        SetRaycastType                             \
        );                                         \
                                                   \
    EditorField(                                   \
        float DrawDuration,                        \
        GetDrawDuration,                           \
        SetDrawDuration                            \
    );                                             \
                                                   \
    EditorResourceField(                           \
        ursine::resources::ArchetypeData,          \
        shotParticle,                              \
        GetShotParticle,                           \
        SetShotParticle                            \
    );                                             \
                                                   \
    EditorResourceField(                           \
        ursine::resources::ArchetypeData,          \
        trailParticle,                             \
        GetTrailParticle,                          \
        SetTrailParticle                           \
    );                                             \
                                                   \
    EditorField(                                   \
        bool AlwaysDraw,                           \
        GetAlwaysDraw,                             \
        SetAlwaysDraw                              \
    );                                             \
                                                   \
    EditorField(                                   \
        bool DebugDraw,                            \
        GetDebug,                                  \
        SetDebug                                   \
    );                                             \
                                                   \
    EditorField(                                   \
        DamageType damageType,                     \
        GetDamageType,                             \
        SetDamageType                              \
    );                                             \
                                                   \
    EditorField(                                   \
        float Damage,                              \
        GetDamageToApply,                          \
        SetDamageToApply                           \
    );                                             \
                                                   \
    EditorField(                                   \
        float CritModifier,                        \
        GetCritModifier,                           \
        SetCritModifier                            \
    );                                                                                 

struct AbstractHitscanWeapon : AbstractWeapon
{

    AbstractHitscanWeapon(void);
    
    ursine::physics::RaycastType GetRaycastType(void) const;
    void SetRaycastType(const ursine::physics::RaycastType type);

    float GetDrawDuration(void) const;
    void SetDrawDuration(const float drawDuration);

    const ursine::resources::ResourceReference &GetShotParticle(void) const;
    void SetShotParticle(const ursine::resources::ResourceReference &particleArchetype);

    const ursine::resources::ResourceReference &GetTrailParticle(void) const;
    void SetTrailParticle(const ursine::resources::ResourceReference &particleArchetype);

    bool GetAlwaysDraw(void) const;
    void SetAlwaysDraw(const bool always);

    bool GetDebug(void) const;
    void SetDebug(const bool debug);

    DamageType GetDamageType(void) const;
    void SetDamageType(DamageType type);
    void ToggleMaxDamage(void);

    float GetDamageToApply(void) const;
    void  SetDamageToApply(float damage);

    float GetCritModifier(void) const;
    void SetCritModifier(float modifier);

    // type of raycast (when to stop)
    ursine::physics::RaycastType m_raycastType;

    // The type of damage being dealt
    DamageType m_damageType;

    // how long to draw
    float m_drawDuration;

    // damage to apply when triggered
    float m_damageToApply;

    // scalar to apply to damage
    float m_critModifier;

    // particle to spawn where shot collide
    ursine::resources::ResourceReference m_shotParticle;

    // particle to use as trail
    ursine::resources::ResourceReference m_trailParticle;

    // draw line
    bool m_alwaysDraw;

    // show debug info
    bool m_debug;

    // toggle for damage cheat
    bool m_maxDamage;
};
