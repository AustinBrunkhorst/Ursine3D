
/* ----------------------------------------------------------------------------
** Team Bear King
** © 2016 DigiPen Institute of Technology, All Rights Reserved.
**
** DamageTextSystem.cpp
**
** Author:
** - Chad George - chad.george@digipen.edu
**
** --------------------------------------------------------------------------*/

#include "Precompiled.h"
#include "DamageTextSystem.h"
#include "Application.h"
#include "GameEvents.h"
#include "SpriteTextComponent.h"
#include "TransformComponent.h"
#include "Model3DComponent.h"
#include "DamageTextComponent.h"

ENTITY_SYSTEM_DEFINITION( DamageTextSystem ) ;

using namespace ursine;
using namespace ursine::ecs;

namespace
{
} // unnamed namespace


//////////////////////////////
////  Base Weapon System  ////
//////////////////////////////

DamageTextSystem::DamageTextSystem(ursine::ecs::World* world) :
    EntitySystem( world ),
    m_critText( WORLD_ARCHETYPE_PATH"DamageText/CritText.uatype" ),
    m_normText( WORLD_ARCHETYPE_PATH"DamageText/NormText.uatype" )
{
}

DamageTextSystem::~DamageTextSystem(void)
{
    m_world->Listener(this)
        .Off( ursine::ecs::WORLD_UPDATE, &DamageTextSystem::onUpdate )
        .Off( game::DAMAGE_TEXT_EVENT, &DamageTextSystem::onDamageText );
}

void DamageTextSystem::OnInitialize(void)
{
    m_world->Listener(this)
        .On( ursine::ecs::WORLD_UPDATE, &DamageTextSystem::onUpdate )
        .On( game::DAMAGE_TEXT_EVENT, &DamageTextSystem::onDamageText );
}

void DamageTextSystem::onUpdate(EVENT_HANDLER(World))
{
    float dt = Application::Instance->GetDeltaTime( );

    for ( auto damageText : m_damageTexts )
    {
        updateDamageText(damageText.first, damageText.second, dt);
    }
}

void DamageTextSystem::onDamageText(void* _sender, const ursine::EventArgs* _args)
{
    EVENT_ATTRS(ursine::ecs::Entity, game::DamageEventArgs);

    createDamageText(args);
}

void DamageTextSystem::updateDamageText(ursine::ecs::Entity* damageText, DamageText* damageTextComp, float dt)
{
    ursine::ecs::Transform* transform = m_transforms[ damageText ];
    ursine::ecs::SpriteText* spriteText = m_spriteTexts[ damageText ];

    // upate lifetime
    damageTextComp->IncrementLifeTime( dt );

    if ( damageTextComp->GetLifeTime( ) > damageTextComp->GetTotalLifeTime( ) )
    {
        deleteDamageText(damageText);
        return;
    }

    // update position
    transform->SetWorldPosition(transform->GetWorldPosition( ) + damageTextComp->GetVelocity( ) * dt);

    // upate alpha
    float percentage = 1.0f - damageTextComp->GetLifeTime( ) / damageTextComp->GetTotalLifeTime( );

    Color color = spriteText->GetColor( );
    color.a = damageTextComp->GetStartAlpha( ) * percentage;

    spriteText->SetColor(color);
}

void DamageTextSystem::createDamageText(const game::DamageEventArgs* args)
{
    Entity* damageText;

    // load valid damage text archetype
    if (args->crit)
        damageText = m_world->CreateEntityFromArchetype( m_critText );
    else
        damageText = m_world->CreateEntityFromArchetype( m_normText );

    // set postion
    auto transform = damageText->GetTransform( );
    transform->SetWorldPosition( args->hitPosition );

    // set sprite text to damage done
    SpriteText* textComp = damageText->GetComponent< SpriteText >( );

    textComp->SetText( std::to_string( static_cast<int>( args->damage ) ) );
    textComp->SetOverdraw( true );

    // generate the damage text velocity and start alpha
    DamageText* damageTextComp = damageText->GetComponent< DamageText >( );
    damageTextComp->GenerateVelocity( );
    damageTextComp->SetStartAlpha( textComp->GetColor( ).a );

    // set text comp render mask
    Model3D* model = args->entityHit->GetComponent<Model3D>( );

    if (model)
    {
        model->GetRenderMask( );
        /////////////////////////////////////////////////////////////////////////////
        //// @matt - set bit mask here //////////////////////////////////////////////
        /////////////////////////////////////////////////////////////////////////////
    }

    // get id for hashing comps needed for updating
    m_spriteTexts[ damageText ] = textComp;
    m_damageTexts[ damageText ] = damageTextComp;
    m_transforms[ damageText ] = transform;
}

void DamageTextSystem::deleteDamageText(ursine::ecs::Entity* textObject)
{
    m_spriteTexts.erase( textObject );
    m_damageTexts.erase( textObject );
    m_transforms.erase( textObject );

    textObject->Delete( );
}

