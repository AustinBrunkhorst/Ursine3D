/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** TrialComponent.h
**
** Author:
** - Chad George - chad.george@digipen.edu
**
** -------------------------------------------------------------------------*/

#pragma once

#include <Component.h>
#include <SVec3.h>
#include <ParticleEmitterComponent.h>

class TrailComponent : public ursine::ecs::Component
{
    NATIVE_COMPONENT;

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

    Meta(Enable)
    TrailComponent(void);
    ~TrailComponent(void);

    const float GetSpeed(void) const;
    void SetSpeed(const float speed);

    int GetSegments(void) const;
    void SetSegments(const int segments);

    int GetParticlesToSpawn(void) const;
    void SetParticlesToSpawn(const int count);

    void SetVecToEnd(ursine::SVec3 end);

private:
    friend class TrailSystem;

    // end pos
    ursine::SVec3 m_vecToEnd;

    // velocity to travel ast
    ursine::SVec3 m_velocity;

    float m_speed;

    // total distance to travel squared
    float m_distToTravel;

    // distance traveled
    //   - once traveled total dist squared delete obj
    float m_distTraveled;

    // number of segments between each step
    int m_segments;

    // particles to spawn at each segment
    int m_particlesToSpawn;
} Meta(
    Enable, 
    WhiteListMethods,
    DisplayName( "TrailComponent" )
) EditorMeta(
    RequiresComponents( typeof( ursine::ecs::ParticleEmitter ) )    
);
