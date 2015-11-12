#include "Precompiled.h"
#include "EditorIconComponent.h"
#include "CoreSystem.h"
#include "GfxAPI.h"
#include "EntityEvent.h"
#include <Game Engine/Scene/Component/Native Components/Billboard2DComponent.h>

NATIVE_COMPONENT_DEFINITION(EditorIcon);

EditorIcon::EditorIcon() 
    : BaseComponent()
    , m_billboard(nullptr)
{
    
}

EditorIcon::~EditorIcon()
{
    m_billboard->Delete();
}

void EditorIcon::SetIcon(const std::string& text)
{
    m_billboard->GetComponent<ursine::ecs::Billboard2D>()->GetBillboard()->SetTexture(text);
}

void EditorIcon::OnInitialize()
{
    m_billboard = GetOwner()->GetWorld()->CreateEntity();
    m_billboard->SetVisibleInEditor(false);
    m_billboard->AddComponent<ursine::ecs::Billboard2D>();

    GetOwner()->GetTransform()->AddChild(m_billboard->GetTransform());
    m_billboard->GetComponent<ursine::ecs::Billboard2D>()->GetBillboard()->SetDimensions(50, 50);
    m_billboard->GetComponent<ursine::ecs::Billboard2D>()->GetBillboard()->SetEntityUniqueID(GetOwner()->GetUniqueID());
}