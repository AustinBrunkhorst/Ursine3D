/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** EditorIconComponent.cpp
**
** Author:
** - Matt Yan - m.yan@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

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

void EditorIcon::SetIcon(ursine::graphics::GfxHND handle)
{
    m_billboard->GetComponent<ecs::Billboard2D>( )->GetBillboard( )->SetTextureHandle( handle );
    GetCoreSystem(graphics::GfxAPI)->ResourceMgr.LoadTexture(handle);

}

void EditorIcon::OnInitialize(void)
{
    auto *owner = GetOwner( );

    m_billboard = owner->GetWorld( )->CreateEntity( );
    GetOwner( )->GetTransform( )->AddChildAlreadyInLocal( m_billboard->GetTransform( ) );
    m_billboard->EnableSerialization( false );
    m_billboard->SetVisibleInEditor( false );

    auto *billboard = m_billboard
        ->AddComponent<ecs::Billboard2D>( )
        ->GetBillboard( );

    billboard->SetDimensions( 50, 50 );
    billboard->SetEntityUniqueID( owner->GetUniqueID( ) );
}
