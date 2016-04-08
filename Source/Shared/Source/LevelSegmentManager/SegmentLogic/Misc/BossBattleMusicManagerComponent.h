/* ---------------------------------------------------------------------------
** Team Bear King
** ?2016 DigiPen Institute of Technology, All Rights Reserved.
**
** BossBattleMusicManagerComponent.h
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** -------------------------------------------------------------------------*/

#pragma once

#include <Component.h>

#include <AudioItemEventData.h>
#include <AudioEmitterComponent.h>

#include "LevelSegmentManagerComponent.h"

class BossBattleMusicManager : public ursine::ecs::Component
{
    NATIVE_COMPONENT;

public:

    EditorResourceField(
        ursine::resources::AudioItemEventData,
        startMusicEvent,
        GetStartMusicEvent,
        SetStartMusicEvent
    );

    EditorResourceField(
        ursine::resources::AudioItemEventData,
        stopMusicEvent,
        GetStopMusicEvent,
        SetStopMusicEvent
    );

    BossBattleMusicManager(void);

    void OnSceneReady(ursine::Scene *scene) override;

    const ursine::resources::ResourceReference &GetStartMusicEvent(void) const;
    void SetStartMusicEvent(const ursine::resources::ResourceReference &startMusicEvent);

    const ursine::resources::ResourceReference &GetStopMusicEvent(void) const;
    void SetStopMusicEvent(const ursine::resources::ResourceReference &stopMusicEvent);

private:

    void onSegmentChange(EVENT_HANDLER(LevelSegmentManager));

    void onDeath(EVENT_HANDLER(ursine::ecs::Entity));

    void setCurrentState(LevelSegments segment);

    ursine::resources::ResourceReference m_startMusic;
    ursine::resources::ResourceReference m_stopMusic;

} Meta(
    Enable,
    RequiresComponents(
        typeof( ursine::ecs::AudioEmitter )
    )
);
