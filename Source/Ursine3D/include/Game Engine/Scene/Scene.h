#pragma once

#include "World.h"

namespace ursine
{
    class Scene
    {
    public:
        ecs::World &GetWorld(void);

        void Update(void);
        void Render(void);

    private:
        ecs::World m_world;
    };
}