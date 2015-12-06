
#include "Precompiled.h"

#include "PlayerAnimationSystem.h"
#include <Components/PlayerAnimationComponent.h>
#include <Game Engine/Scene/Component/Native Components/AnimatorComponent.h>

ENTITY_SYSTEM_DEFINITION(PlayerAnimationSystem);

PlayerAnimationSystem::PlayerAnimationSystem(ursine::ecs::World* world)
    : FilterSystem(world, ursine::ecs::Filter( ).All<PlayerAnimation, ursine::ecs::Animator>( )) 
{
    
}

void PlayerAnimationSystem::Process(ursine::ecs::Entity* entity)
{
    entity->GetComponent<PlayerAnimation>( )->Update( );
}