#include "Precompiled.h"

#include "SelectorComponent.h"
#include <Game Engine/Scene/Component/Native Components/TransformComponent.h>

using namespace ursine::graphics;

NATIVE_COMPONENT_DEFINITION(Selector);

Selector::Selector()
    : BaseComponent()
    , m_boxHandle(0)
{
}

Selector::~Selector()
{
    GetCoreSystem(GfxAPI)->RenderableMgr.DestroyRenderable(m_boxHandle);
}

void Selector::OnInitialize()
{
    m_boxHandle = GetCoreSystem(GfxAPI)->RenderableMgr.AddRenderable(ursine::graphics::RENDERABLE_PRIMITIVE);
    auto &prim = GetCoreSystem(GfxAPI)->RenderableMgr.GetPrimitive(m_boxHandle);
    auto transform = GetOwner()->GetComponent<ursine::ecs::Transform>();

    prim.SetType(Primitive::PRIM_CUBE);

    prim.SetWorldMatrix(transform->GetLocalToWorldMatrix());
}