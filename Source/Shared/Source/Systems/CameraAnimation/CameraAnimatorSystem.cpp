/* ----------------------------------------------------------------------------
** Team Bear King
** © 2016 DigiPen Institute of Technology, All Rights Reserved.
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

#include <Application.h>

ENTITY_SYSTEM_DEFINITION( CameraAnimatorSystem );

using namespace ursine;
using namespace ecs;

CameraAnimatorSystem::CameraAnimatorSystem(World* world)
	: FilterSystem( world, Filter( ).All<CameraAnimator>( ) )
{

}

void CameraAnimatorSystem::Process(Entity* entity)
{
	auto animator = entity->GetComponent<CameraAnimator>( );

	if (animator->m_playing)
	{
		// early out if the node count is < 2
		if (animator->m_nodes.size( ) < 2)
		{
			animator->Reset( );
			return;
		}

		float dt = Application::Instance->GetDeltaTime( );

		// incrment the time
		animator->m_time += dt;
		
		// get the two current nodes
		auto node1 = animator->m_nodes[ animator->m_index ];
		auto node2 = animator->m_nodes[ animator->m_index + 1 ];

		// calculate the t value (based on second node's time)
		float t = animator->m_time / node2->GetTransitionToTime( );

		// if t value is past the current node
		if (t > 1.0f)
		{
			// decrease t value
			t -= 1.0f;

			// increment index
			++(animator->m_index);

			// if index == nodes.count - 1, playing is done
			if (animator->m_index >= animator->m_nodes.size( ) - 1)
			{
				animator->updateAnimation( *animator->m_nodes.end( ) );
				animator->m_playing = false;
				animator->m_time = 0.0f;
				animator->m_index = static_cast<int>( animator->m_nodes.size( ) - 1 );
				return;
			}

			// set the two current nodes
			node1 = animator->m_nodes[ animator->m_index ];
			node2 = animator->m_nodes[ animator->m_index + 1 ];
		}

		// interpolate between the two current nodes
		animator->updateAnimation( node1, node2, t );
	}
}
