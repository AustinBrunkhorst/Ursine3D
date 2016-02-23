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
    EditorField(                                          \
        int SpawnSegments,                                \
        GetSpawnSegments,                                 \
        SetSpawnSegments                                  \
    );                                                    \
                                                          \
    EditorField(                                          \
        std::string ShotParticle,                         \
        GetShotParticle,                                  \
        SetShotParticle                                   \
    );                                                    \
                                                          \
    EditorField(                                          \
        std::string TrailParticle,                        \
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

    int GetSpawnSegments(void) const;
    void SetSpawnSegments(const int segments);

    const std::string& GetShotParticle(void) const;
    void SetShotParticle(const std::string& particleArchetype);

    const std::string& GetTrailParticle(void) const;
    void SetTrailParticle(const std::string& particleArchetype);

    bool GetAlwaysDraw(void) const;
    void SetAlwaysDraw(const bool always);

    bool GetDebug(void) const;
    void SetDebug(const bool debug);

    // type of raycast (when to stop)
    ursine::physics::RaycastType m_raycastType;

    // how long to draw
    float m_drawDuration;

    // number of segments to spawn particles
    int m_spawnSemgennts;

    // particle to spawn where shot collide
    std::string m_shotParticle;

    // particle to use as trail
    std::string m_trailParticle;

    // draw line
    bool m_alwaysDraw;

    // show debug info
    bool m_debug;

};
