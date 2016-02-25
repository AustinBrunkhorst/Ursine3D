/* ----------------------------------------------------------------------------
** Team Bear King
** ?2016 DigiPen Institute of Technology, All Rights Reserved.
**
** CameraAnimatorSystem.cpp
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** --------------------------------------------------------------------------*/

#include <Precompiled.h>

#include "CameraAnimatorSystem.h"

#include "CameraAnimatorComponent.h"
#include "CameraAnimatorNodeComponent.h"
#include "CameraAnimatorFocusPointComponent.h"

#include <Application.h>

ENTITY_SYSTEM_DEFINITION( CameraAnimatorSystem );

using namespace ursine;
using namespace ecs;

CameraAnimatorSystem::CameraAnimatorSystem(World *world)
    : FilterSystem( world, Filter( ).All<CameraAnimator>( ) )
{

}

void CameraAnimatorSystem::Process(Entity *entity)
{
    auto animator = entity->GetComponent<CameraAnimator>( );

    if (!animator->m_playing)
        return;

    // early out if the node count is < 2
    if (animator->m_nodes.size( ) < 2)
    {
        animator->Reset( );
        return;
    }

    // Every frame get the focus point in the children of the camera animator
    auto focus = entity->GetComponentInChildren<CameraAnimatorFocusPoint>( );

    if (focus)
    {
        animator->m_focusPoint = focus->GetOwner( )->GetTransform( );
    }

    if (animator->m_smoothPath)
    {
        smoothUpdate( animator );
    }
    else
    {
        linearUpdate( animator );
    }
}

void CameraAnimatorSystem::smoothUpdate(CameraAnimator *animator)
{
    auto dt = Application::Instance->GetDeltaTime( );

    // incrment the time
    animator->m_time += dt;
        
    // get the two current nodes
    auto node2 = animator->m_nodes[ animator->m_index ];
    auto node3 = animator->m_nodes[ animator->m_index + 1 ];

    // calculate the t value (based on second node's time)
    auto t = animator->m_time / node3->GetTransitionToTime( );

    // if t value is past the current node
    if (t > 1.0f)
    {
        // decrease t value
        t -= floor( t );

        // increment index
        ++(animator->m_index);

        // if index == nodes.count - 1, playing is done
        if (animator->m_index >= animator->m_nodes.size( ) - 1)
        {
            animator->updateAnimation( animator->m_nodes.back( ) );
            animator->finished( );
            animator->m_time = 0.0f;
            animator->m_index = static_cast<int>( animator->m_nodes.size( ) - 1 );
            return;
        }

        animator->m_time -= node3->GetTransitionToTime( );

        // set the two current nodes
        node2 = animator->m_nodes[ animator->m_index ];
        node3 = animator->m_nodes[ animator->m_index + 1 ];
    }

    // get the previous and next node
    auto node1 = node2;

    if (animator->m_index > 0)
        node1 = animator->m_nodes[ animator->m_index - 1 ];

    auto node4 = node3;

    if (animator->m_index + 2 < animator->m_nodes.size( ))
        node4 = animator->m_nodes[ animator->m_index + 2 ];

    // interpolate between the two current nodes
    animator->updateAnimation( node1, node2, node3, node4, t );
}

void CameraAnimatorSystem::linearUpdate(CameraAnimator *animator)
{
    auto dt = Application::Instance->GetDeltaTime( );

    // incrment the time
    animator->m_time += dt;
        
    // get the two current nodes
    auto node1 = animator->m_nodes[ animator->m_index ];
    auto node2 = animator->m_nodes[ animator->m_index + 1 ];

    // calculate the t value (based on second node's time)
    auto t = animator->m_time / node2->GetTransitionToTime( );

    // if t value is past the current node
    if (t > 1.0f)
    {
        // decrease t value
        t -= floor( t );

        // increment index
        ++(animator->m_index);

        // if index == nodes.count - 1, playing is done
        if (animator->m_index >= animator->m_nodes.size( ) - 1)
        {
            animator->updateAnimation( animator->m_nodes.back( ) );
            animator->finished( );
            animator->m_time = 0.0f;
            animator->m_index = static_cast<int>( animator->m_nodes.size( ) - 1 );
            return;
        }

        animator->m_time -= node2->GetTransitionToTime( );

        // set the two current nodes
        node1 = animator->m_nodes[ animator->m_index ];
        node2 = animator->m_nodes[ animator->m_index + 1 ];
    }

    // interpolate between the two current nodes
    animator->updateAnimation( node1, node2, t );
}

