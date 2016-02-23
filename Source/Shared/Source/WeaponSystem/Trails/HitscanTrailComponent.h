#pragma once

/* ---------------------------------------------------------------------------
** Team Bear King
** � 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** TrialComponent.h
**
** Author:
** - Chad George - chad.george@digipen.edu
**
** -------------------------------------------------------------------------*/


#include <Component.h>
#include <SVec3.h>
#include <ParticleEmitterComponent.h>

class HitscanTrailComponent : public ursine::ecs::Component
{
    NATIVE_COMPONENT
public:
    
    EditorField(
        float Speed,
        GetSpeed,
        SetSpeed
    );

    EditorField(
        int Segments,
        GetSegments,
        SetSegments
    );

    EditorField(
        int ParticlesToSpawn,
        GetParticlesToSpawn,
        SetParticlesToSpawn
    );

    HitscanTrailComponent(void);
    ~HitscanTrailComponent(void);

    const float GetSpeed(void) const;
    void SetSpeed(const float speed);

    int GetSegments(void) const;
    void SetSegments(const int segments);

    int GetParticlesToSpawn(void) const;
    void SetParticlesToSpawn(const int count);

    void SetVecToEnd(ursine::SVec3 end);

    // end pos
    Meta(Disable)
    ursine::SVec3 m_vecToEnd;

    // velocity to travel ast
    Meta(Disable)
    ursine::SVec3 m_velocity;

    Meta(Disable)
    float m_speed;

    // time that has be alive
    Meta(Disable)
    float m_time;

    // time to reach end
    Meta(Disable)
    float m_timeToReachEnd;

    // number of segments between each step
    Meta(Disable)
    int m_segments;

    // particles to spawn at each segment
    Meta(Disable)
    int m_particlesToSpawn;
    
} Meta(Enable, RequiresComponents(typeof(ursine::ecs::ParticleEmitter)), DisplayName("HitscanTrailComponent"));
