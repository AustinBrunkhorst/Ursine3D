/* ----------------------------------------------------------------------------
** Team Bear King
** © 2016 DigiPen Institute of Technology, All Rights Reserved.
**
** EditorSceneEvent.h
**
** Author:
** - Austin Brunkhorst - a.brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#pragma once

#include "ScenePlayState.h"

#include <EventArgs.h>

enum EditorSceneEventType
{
    EDITOR_SCENE_PLAYSTATE_CHANGED = ursine::SCENE_EVENT_CUSTOM,
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