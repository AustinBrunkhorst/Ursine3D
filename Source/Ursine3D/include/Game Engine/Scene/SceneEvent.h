#pragma once

#include "ScenePlayState.h"
#include "DeltaTime.h"

namespace ursine
{
    enum SceneEventType
    {
        // The active world changed
        SCENE_WORLD_CHANGED,

        // The current playstate has changed
        SCENE_PLAYSTATE_CHANGED,

        // the scene explicitly stepped a frame while paused
        SCENE_FRAME_STEPPED
    };

    struct SceneWorldChangedArgs : EventArgs
    {
        ecs::World::Handle oldWorld;

        // if the world was set through a resource reference,
        // this will contain the resource, otherwise it will be null
        const resources::ResourceReference *reference;

        SceneWorldChangedArgs(ecs::World::Handle oldWorld, const resources::ResourceReference *reference)
            : oldWorld( oldWorld )
            , reference( reference ) { }
    };

    struct ScenePlayStateChangedArgs : EventArgs
    {
        ScenePlayState oldState;
        ScenePlayState newState;

        ScenePlayStateChangedArgs(ScenePlayState oldState, ScenePlayState newState)
            : oldState( oldState )
            , newState( newState ) { }
    };

    struct SceneFrameSteppedArgs : EventArgs
    {
        DeltaTime dt;

        SceneFrameSteppedArgs(DeltaTime dt)
            : dt( dt ) { }
    };
}