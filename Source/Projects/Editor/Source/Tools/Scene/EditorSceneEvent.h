#pragma once

#include "ScenePlayState.h"

#include <EventArgs.h>

enum EditorSceneEventType
{
    EDITOR_SCENE_PLAYSTATE_CHANGED = ursine::SCENE_EVENT_COUNT,
    EDITOR_SCENE_FRAME_STEPPED
};

struct ScenePlayStateChangedArgs : ursine::EventArgs
{
    ScenePlayState oldState;
    ScenePlayState newState;

    ScenePlayStateChangedArgs(ScenePlayState oldState, ScenePlayState newState)
        : oldState( oldState )
        , newState( newState ) { }
};

struct SceneFrameSteppedArgs : ursine::EventArgs
{
    ursine::DeltaTime dt;

    SceneFrameSteppedArgs(ursine::DeltaTime dt)
        : dt( dt ) { }
};