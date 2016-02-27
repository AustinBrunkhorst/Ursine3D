/* ----------------------------------------------------------------------------
** Team Bear King
** ?2016 DigiPen Institute of Technology, All Rights Reserved.
**
** EntityAnimatorSystem.cpp
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** --------------------------------------------------------------------------*/

#include "Precompiled.h"

#include "EntityAnimatorSystem.h"
#include "EntityAnimatorComponent.h"

#include <Application.h>

ENTITY_SYSTEM_DEFINITION( EntityAnimatorSystem );

using namespace ursine;
using namespace ecs;

EntityAnimatorSystem::EntityAnimatorSystem(World *world)
    : FilterSystem( world, Filter( ).All<EntityAnimator>( ) )
{
    
}

void EntityAnimatorSystem::Initialize(void)
{
    m_world->Listener( this )
        .On( WORLD_EDITOR_UPDATE, &FilterSystem::onUpdate );
}

void EntityAnimatorSystem::Process(Entity *entity)
{
    auto animator = entity->GetComponent<EntityAnimator>( );

    if (!animator->m_playing)
        return;

    // Early out if the node count is < 2
    if (animator->keyFrames.Size( ) < 2)
    {
        animator->JumpToStart( );
        return;
    }

    if (animator->m_smoothPath)
        smoothUpdate( animator );
    else
        linearUpdate( animator );
}

void EntityAnimatorSystem::smoothUpdate(EntityAnimator* animator)
{
    auto dt = Application::Instance->GetDeltaTime( );

    auto max = static_cast<int>( animator->keyFrames.Size( ) );

    // incrment the time
    animator->m_time += dt;

    // get the two current nodes
    auto node2 = animator->m_index;
    auto node3 = node2 == max - 1 ? 0 : animator->m_index + 1;

    // calculate the t value (based on second node's time)
    auto t = animator->m_time / animator->keyFrames[ node3 ].delta;

    // if t value is past the current node
    if (t > 1.0f)
    {
        // decrease t value
        t -= floor( t );

        // increment index
        ++(animator->m_index);

        // if index == nodes.count - 1, and looping is turned off, playing is done
        if (animator->m_index >= max - 1 && !animator->m_loop)
        {
            animator->updateAnimation( max - 1 );
            animator->finish( );
            animator->m_time = 0.0f;
            animator->m_index = static_cast<int>( max - 1 );
            return;
        }
        else if (animator->m_index >= max && animator->m_loop)
        {
            animator->m_time = 0.0f;
            animator->m_index = 0;
        }
        else
            animator->m_time -= animator->keyFrames[ node3 ].delta;

        // set the two current nodes
        node2 = animator->m_index;
        node3 = node2 == max - 1 ? 0 : animator->m_index + 1;
    }

    // get the previous and next node
    auto node1 = node2;

    if (node2 > 0)
        node1 = node2 - 1;
    else
        node1 = max - 1;

    auto node4 = node3;

    if (node3 < animator->keyFrames.Size( ) - 1)
        node4 = node3 + 1;
    else
        node4 = 0;

    // interpolate between the two current nodes
    animator->updateAnimation( node1, node2, node3, node4, t );
}

void EntityAnimatorSystem::linearUpdate(EntityAnimator* animator)
{
    auto dt = Application::Instance->GetDeltaTime( );

    auto max = static_cast<int>( animator->keyFrames.Size( ) );

    // incrment the time
    animator->m_time += dt;
        
    // get the two current nodes
    auto node1 = animator->m_index;
    auto node2 = node1 == max - 1 ? 0 : animator->m_index + 1;

    // calculate the t value (based on second node's time)
    auto t = animator->m_time / animator->keyFrames[ node2 ].delta;

    // if t value is past the current node
    if (t > 1.0f)
    {
        // decrease t value
        t -= floor( t );

        // increment index
        ++(animator->m_index);

        // if index == nodes.count - 1, and looping is turned off, playing is done
        if (animator->m_index >= max - 1 && !animator->m_loop)
        {
            animator->updateAnimation( max - 1 );
            animator->finish( );
            animator->m_time = 0.0f;
            animator->m_index = static_cast<int>( max - 1 );
            return;
        }
        else if (animator->m_index >= max && animator->m_loop)
        {
            animator->m_time = 0.0f;
            animator->m_index = 0;
        }
        else
            animator->m_time -= animator->keyFrames[ node2 ].delta;

        // set the two current nodes
        node1 = animator->m_index;
        node2 = node1 == max - 1 ? 0 : animator->m_index + 1;
    }

    // interpolate between the two current nodes
    animator->updateAnimation( node1, node2, t );
}
