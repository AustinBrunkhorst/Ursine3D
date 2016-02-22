#pragma once

namespace ursine
{
    enum SceneEventType
    {
        // The active world changed
        SCENE_WORLD_CHANGED
    };

    struct SceneWorldChangedArgs : EventArgs
    {
        // if the world was set through a resource reference,
        // this will contain the resource, otherwise it will be null
        const resources::ResourceReference *reference;

        SceneWorldChangedArgs(const resources::ResourceReference *reference)
            : reference( reference ) { }
    };
}