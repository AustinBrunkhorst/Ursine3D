#pragma once

#include "DeltaTime.h"

namespace ursine
{
    enum SceneEventType
    {
        // The active world changed
        SCENE_WORLD_CHANGED,

        SCENE_EVENT_CUSTOM,

    #if defined(URSINE_WITH_EDITOR)

        // A resource has been rebuilt
        SCENE_RESOURCE_MODIFIED = 0x800

    #endif
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

#if defined(URSINE_WITH_EDITOR)

    struct SceneResourceModifiedArgs : EventArgs
    {
        GUID resourceGUID;

        SceneResourceModifiedArgs(const GUID &resourceGUID)
            : resourceGUID( resourceGUID ) { }
    };

#endif
}
