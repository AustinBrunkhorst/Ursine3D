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
    , m_billboard( )
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

void EditorIcon::SetIcon(graphics::GfxHND handle)
{
    m_billboard->GetComponent<ecs::Billboard2D>( )->GetBillboard( )->SetTextureHandle( handle );

    GetCoreSystem( graphics::GfxAPI )->ResourceMgr.LoadTexture( handle );
}

void EditorIcon::OnInitialize(void)
{
    auto &owner = GetOwner( );

    // look for the editor icon billboard and see if it's already there
    auto child = owner->GetChildByName( "EditorIconBillboard1234" );

    if (child)
    {
        m_billboard = child;
    }
    else
    {
        m_billboard = owner->GetWorld( )->CreateEntity( );
        owner->GetTransform( )->AddChildAlreadyInLocal( m_billboard->GetTransform( ) );
        m_billboard->EnableSerialization( false );
        m_billboard->SetVisibleInEditor( false );
        m_billboard->SetName( "EditorIconBillboard1234" );
        m_billboard->AddComponent<ecs::Billboard2D>( );
    }

    auto *billboard = m_billboard
        ->GetComponent<ecs::Billboard2D>( )
        ->GetBillboard( );

    billboard->SetDimensions( 50, 50 );
    billboard->SetEntityID( owner->GetID( ) );

    m_billboard->GetComponent<ecs::Billboard2D>( )->SetRenderMask( ecs::RenderMask::MEditorTool );
}
