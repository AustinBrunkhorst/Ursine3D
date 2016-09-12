/*---------------------------------------------------------------------------
** Team Bear King
** 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** ShieldFXComponent.cpp
**
** Author:
** - Matt Yan - m.yan@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
**-------------------------------------------------------------------------*/

#include "Precompiled.h"

#include "ShieldFXComponent.h"
#include "FragmentationComponent.h"

using namespace ursine;
using namespace ecs;

NATIVE_COMPONENT_DEFINITION(ShieldFX);


ShieldFX::ShieldFX(void)
    : BaseComponent( )
    , m_shieldState( SHIELD_STATE_STABLE )
{
}

#ifdef URSINE_WITH_EDITOR
void ShieldFX::Stabilize(void)
{
    StabilizeShield( );
}

void ShieldFX::Destroy(void)
{
    DestroyShield( );
}

void ShieldFX::Rebuild(void)
{
    RebuildShield( );
}
#endif

void ShieldFX::OnInitialize(void)
{
}

ShieldFX::ShieldState ShieldFX::GetShieldState(void) const
{
    return m_shieldState;
}

void ShieldFX::SetShieldState(ShieldState state)
{
    m_shieldState = state;
}

Vec2 ShieldFX::GetTextureVelocity(void) const
{
    return m_textureVelocity;
}

void ShieldFX::SetTextureVelocity(const ursine::Vec2& textureVel)
{
    m_textureVelocity = textureVel;

    NOTIFY_COMPONENT_CHANGED( "textureVelocity", textureVel );
}

void ShieldFX::StabilizeShield(void)
{
    auto fragment = GetOwner( )->GetComponent<ModelFragmenter>( );

    fragment->SetTime( 0.0f );
    fragment->PauseFragmentation( );

    m_shieldState = SHIELD_STATE_STABLE;
}

void ShieldFX::DestroyShield(void)
{
    auto fragment = GetOwner( )->GetComponent<ModelFragmenter>( );

    fragment->ResetFragmentation( );
    fragment->SetTime( 0.0f );
    fragment->SetPlayInReverse( false );
    fragment->StartFragmentation( );

    m_shieldState = SHIELD_STATE_DESTROYING;
}

void ShieldFX::RebuildShield(void)
{
    auto fragment = GetOwner()->GetComponent<ModelFragmenter>();

    fragment->ResetFragmentation( );
    fragment->SetTime( fragment->GetMaxTime( ) );
    fragment->SetPlayInReverse( true );
    fragment->StartFragmentation();

    m_shieldState = SHIELD_STATE_REBUILDING;
}
