#include "UrsinePrecompiled.h"

#include "Scene.h"

namespace ursine
{
    ecs::World &Scene::GetWorld(void)
    {
        return m_world;
    }
}