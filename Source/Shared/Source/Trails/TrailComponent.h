#pragma once

/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
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

class TrailComponent : public ursine::ecs::Component
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

    TrailComponent(void);
    ~TrailComponent(void);

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

    // total distance to travel squared
    Meta(Disable)
    float m_distToTravel;

    // distance traveled
    //   - once traveled total dist squared delete obj
    Meta(Disable)
    float m_distTraveled;

    // number of segments between each step
    Meta(Disable)
    int m_segments;

    // particles to spawn at each segment
    Meta(Disable)
    int m_particlesToSpawn;
    
} Meta(Enable, RequiresComponents(typeof(ursine::ecs::ParticleEmitter)), DisplayName("TrailComponent"));
