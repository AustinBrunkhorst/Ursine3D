/* ---------------------------------------------------------------------------
** Team Bear King
** ?2016 DigiPen Institute of Technology, All Rights Reserved.
**
** AnimateParticleResetComponent.h
**
** Author:
** - JordanEllis - j.ellis@digipen.edu
**
** -------------------------------------------------------------------------*/

#pragma once

#include <Component.h>

#include <Ease.h>

namespace ursine
{
    namespace ecs
    {
        class ParticleEmitter;
    }
}

class AnimateParticleReset : public ursine::ecs::Component
{
    NATIVE_COMPONENT;

public:

    EditorField(
        float startDuration,
        GetStartDuration,
        SetStartDuration
    );

    EditorField(
        float endDuration,
        GetEndDuration,
        SetEndDuration
    );

    EditorField(
        float timeToChange,
        GetTimeToChange,
        SetTimeToChange
    );

    EditorField(
        bool startOnAwake,
        GetStartOnAwake,
        SetStartOnAwake
    );

    EditorField(
        bool stopOnComplete,
        GetStopOnComplete,
        SetStopOnComplete
    );

    EditorField(
        ursine::ease::Type easeFunction,
        GetEaseFunction,
        SetEaseFunction
    );

    AnimateParticleReset(void);
    ~AnimateParticleReset(void);

    void OnInitialize(void) override;

    float GetStartDuration(void) const;
    void SetStartDuration(float duration);

    float GetEndDuration(void) const;
    void SetEndDuration(float duration);

    float GetTimeToChange(void) const;
    void SetTimeToChange(float duration);

    bool GetStartOnAwake(void) const;
    void SetStartOnAwake(bool flag);

    bool GetStopOnComplete(void) const;
    void SetStopOnComplete(bool flag);

    ursine::ease::Type GetEaseFunction(void) const;
    void SetEaseFunction(ursine::ease::Type type);

    void Play(void);

private:
    float m_startDuration;
    float m_endDuration;
    float m_timeToChange;

    float m_durationTimer;
    float m_changeTimer;

    bool m_startOnAwake;
    bool m_playing;

    bool m_stopOnComplete;

    ursine::ease::Type m_easeFunction;

    ursine::ecs::ParticleEmitter *m_emitter;

    void onUpdate(EVENT_HANDLER(World));

} Meta(
    Enable
) EditorMeta( 
    RequiresComponents( typeof( ursine::ecs::ParticleEmitter ) )
);
