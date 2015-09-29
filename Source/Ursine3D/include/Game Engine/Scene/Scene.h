#pragma once

#include "World.h"

namespace ursine
{
    class Scene
    {
    public:
        ecs::World &GetWorld(void);

    private:
        ecs::World m_world;
    };
}