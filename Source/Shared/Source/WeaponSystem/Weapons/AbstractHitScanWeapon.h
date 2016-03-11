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


#define HitscanEditorFields( )                            \
    EditorField(                                          \
        ursine::physics::RaycastType RaycastType,         \
        GetRaycastType,                                   \
        SetRaycastType                                    \
        );                                                \
                                                          \
    EditorField(                                          \
        float DrawDuration,                               \
        GetDrawDuration,                                  \
        SetDrawDuration                                   \
    );                                                    \
                                                          \
    EditorResourceField(                                  \
        ursine::resources::ArchetypeData,                 \
        shotParticle,                                     \
        GetShotParticle,                                  \
        SetShotParticle                                   \
    );                                                    \
                                                          \
    EditorResourceField(                                  \
        ursine::resources::ArchetypeData,                 \
        trailParticle,                                    \
        GetTrailParticle,                                 \
        SetTrailParticle                                  \
    );                                                    \
                                                          \
    EditorField(                                          \
        bool AlwaysDraw,                                  \
        GetAlwaysDraw,                                    \
        SetAlwaysDraw                                     \
    );                                                    \
                                                          \
    EditorField(                                          \
        bool DebugDraw,                                   \
        GetDebug,                                         \
        SetDebug                                          \
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

    // type of raycast (when to stop)
    ursine::physics::RaycastType m_raycastType;

    // how long to draw
    float m_drawDuration;

    // particle to spawn where shot collide
    ursine::resources::ResourceReference m_shotParticle;

    // particle to use as trail
    ursine::resources::ResourceReference m_trailParticle;

    // draw line
    bool m_alwaysDraw;

    // show debug info
    bool m_debug;

};
