#include "Precompiled.h"

#include "EditorIconComponent.h"

#include <GfxAPI.h>
#include <Billboard2DComponent.h>

NATIVE_COMPONENT_DEFINITION( EditorIcon );

using namespace ursine;

EditorIcon::EditorIcon(void)
    : BaseComponent( )
    , m_billboard( nullptr )
{
}

EditorIcon::~EditorIcon(void)
{
    m_billboard->Delete( );
}

void EditorIcon::SetIcon(const std::string &text)
{
    m_billboard->GetComponent<ecs::Billboard2D>( )->GetBillboard( )->SetTexture( text );
}

void EditorIcon::OnInitialize(void)
{
    auto *owner = GetOwner( );

    m_billboard = owner->GetWorld( )->CreateEntity( );
    m_billboard->EnableSerialization( false );
    m_billboard->SetVisibleInEditor( false );

    auto *billboard = m_billboard
        ->AddComponent<ecs::Billboard2D>( )
        ->GetBillboard( );

    owner->GetTransform( )->AddChild( m_billboard->GetTransform( ) );

    billboard->SetDimensions( 50, 50 );
    billboard->SetEntityUniqueID( owner->GetUniqueID( ) );
}
