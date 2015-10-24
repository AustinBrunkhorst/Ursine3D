#include "Precompiled.h"

#include "SelectedComponent.h"
#include <Game Engine/Scene/Component/Native Components/RenderableComponent.h>
#include <Game Engine/Scene/Component/Native Components/Model3DComponent.h>
#include <Game Engine/Scene/Entity/EntityEvent.h>

using namespace ursine::graphics;

NATIVE_COMPONENT_DEFINITION( Selected );

Selected::Selected(void)
    : BaseComponent( ) 
    , m_selectBox(nullptr) { }

Selected::~Selected()
{
    m_selectBox->Delete();
    GetOwner()->GetComponent<ursine::ecs::Model3D>()->GetModel()->SetMaterialData(0, 0, 0);
}

void Selected::OnInitialize()
{
    m_selectBox = GetOwner()->GetWorld()->CreateEntity("DebugBox");

    m_selectBox->AddComponent<ursine::ecs::Renderable>();
    auto handle = GetCoreSystem(GfxAPI)->RenderableMgr.AddRenderable(ursine::graphics::RENDERABLE_PRIMITIVE);
    auto &prim = GetCoreSystem(GfxAPI)->RenderableMgr.GetPrimitive(handle);
    auto transform = GetOwner()->GetComponent<ursine::ecs::Transform>();

    prim.SetType(Primitive::PRIM_CUBE);

    prim.SetWorldMatrix(transform->GetLocalToWorldMatrix() * ursine::SMat4(1.1, 1.1 ,1.1));

    m_selectBox->GetComponent<ursine::ecs::Renderable>()->SetHandle(handle);

    GetOwner()->GetComponent<ursine::ecs::Model3D>()->GetModel()->SetMaterialData(1, 0, 0);
}
