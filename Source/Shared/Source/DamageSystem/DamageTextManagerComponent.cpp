/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** DamageTextManagerComponent.cpp
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** -------------------------------------------------------------------------*/

#include "Precompiled.h"

#include "DamageTextManagerComponent.h"

#include "DamageEvent.h"

#include <SpriteTextComponent.h>
#include <Model3DComponent.h>

NATIVE_COMPONENT_DEFINITION(DamageTextManager);

using namespace ursine;
using namespace ecs;
using namespace resources;

DamageTextManager::DamageTextManager(void)
    : BaseComponent( ) { }

DamageTextManager::~DamageTextManager(void)
{
    // unsubscribe
    auto world = GetOwner( )->GetWorld( );

    if (world)
    {
        world->Listener( this )
            .Off( game::DAMAGE_TEXT_EVENT, &DamageTextManager::onDamageText );
    }
}

void DamageTextManager::OnSceneReady(Scene *scene)
{
    // subscribe
    auto world = GetOwner( )->GetWorld( );

    if (world)
    {
        world->Listener( this )
            .On( game::DAMAGE_TEXT_EVENT, &DamageTextManager::onDamageText );
    }
}

const ResourceReference &DamageTextManager::GetDamageSpriteText(void) const
{
    return m_damageText;
}

void DamageTextManager::SetDamageSpriteText(const ResourceReference &archetype)
{
    m_damageText = archetype;

    NOTIFY_COMPONENT_CHANGED( "damageSpriteText", m_damageText );
}

const ResourceReference &DamageTextManager::GetCritSpriteText(void) const
{
    return m_critText;
}

void DamageTextManager::SetCritSpriteText(const ResourceReference &archetype)
{
    m_critText = archetype;

    NOTIFY_COMPONENT_CHANGED( "critSpriteText", m_critText );
}

const ResourceReference &DamageTextManager::GetInvulnerableSpriteText(void) const
{
    return m_invulnerableText;
}

void DamageTextManager::SetInvulnerableSpriteText(const ResourceReference &archetype)
{
    m_invulnerableText = archetype;

    NOTIFY_COMPONENT_CHANGED( "invulnerableSpriteText", m_invulnerableText );
}

void DamageTextManager::onDamageText(EVENT_HANDLER(World))
{
    EVENT_ATTRS(World, game::DamageEventArgs);

    EntityHandle damageText;

    // load valid damage text archetype
    if ( args->invulnerable )
        damageText = sender->CreateEntityFromArchetype(m_invulnerableText);
    else if (args->crit)
        damageText = sender->CreateEntityFromArchetype( m_critText ); 
    else
        damageText = sender->CreateEntityFromArchetype( m_damageText );

    // set postion
    if (!damageText)
        return;

    auto transform = damageText->GetTransform( );

    transform->SetWorldPosition( args->hitPosition );

    // generate the damage text velocity and start alpha
    DamageText *damageTextComp = damageText->GetComponentInChildren<DamageText>( );
    SpriteText *textComp = damageText->GetComponentInChildren<SpriteText>( );

    UAssert( 
        damageTextComp, 
        "Error: The damage/crit/invulnerable archetype needs a \"DamageText\" Component."
    );

    if (!args->invulnerable)
        textComp->SetText( std::to_string( static_cast<int>( args->damage ) ) );

    textComp->SetOverdraw( true );

    // set text comp render mask
    Model3D* model = args->entityHit->GetComponent<Model3D>( );

    if (!model)
        model = args->entityHit->GetComponentInChildren<Model3D>( );

    if (model)
        textComp->SetRenderMask( model->GetRenderMask( ) );
}
