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

#include "ShieldFXSystem.h"
#include "ShieldFXComponent.h"
#include "FragmentationComponent.h"

ENTITY_SYSTEM_DEFINITION(ShieldFXSystem);

using namespace ursine;
using namespace ecs;

ShieldFXSystem::ShieldFXSystem(World *world)
    : FilterSystem(world, Filter( ).All<ShieldFX>( ))
{

}

void ShieldFXSystem::Process(const ursine::ecs::EntityHandle & entity)
{
    auto fragmentComponent = entity->GetComponent<ModelFragmenter>( );
    auto shieldComponent = entity->GetComponent<ShieldFX>();
    
    auto shieldState = shieldComponent->GetShieldState( );

    switch(shieldState)
    {
    case ShieldFX::SHIELD_STATE_STABLE:
        break;
    case ShieldFX::SHIELD_STATE_DESTROYING:
        break;
    }
}