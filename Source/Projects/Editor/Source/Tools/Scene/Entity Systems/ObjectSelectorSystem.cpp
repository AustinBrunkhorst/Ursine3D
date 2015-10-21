#include "Precompiled.h"
#include "ObjectSelectorSystem.h"

ObjectSelectorSystem::ObjectSelectorSystem(ursine::ecs::World* world)
    : EntitySystem(world)
    , m_xAxis(0)
    , m_yAxis(0)
    , m_zAxis(0)
    , m_graphics(nullptr)
{
    
}

void ObjectSelectorSystem::OnInitialize()
{
    //grab graphics
    m_graphics = GetCoreSystem(ursine::graphics::GfxAPI);

    //construct the 3 axis
}

void ObjectSelectorSystem::OnRemove()
{
    
}

void ObjectSelectorSystem::onRenderHook(EVENT_HANDLER(ursine::ecs::RenderSystem))
{
    
}