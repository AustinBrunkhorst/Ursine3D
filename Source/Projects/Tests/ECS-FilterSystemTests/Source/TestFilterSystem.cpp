/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** TestFilterSystem.cpp
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#include <UrsinePrecompiled.h>

#include "TestFilterSystem.h"
#include <RigidbodyComponent.h>
#include <AudioEmitterComponent.h>
#include <BodyComponent.h>

namespace ursine
{
	namespace ecs
	{
		uint TestFilterSystem::numUpdated = 0;

		ENTITY_SYSTEM_DEFINITION( TestFilterSystem );

		TestFilterSystem::TestFilterSystem(World *world)
			: FilterSystem( world, Filter( ).One<Rigidbody, Body, AudioEmitter>( ) )
		{
			
		}

		void TestFilterSystem::Process(Entity *entity)
		{
			++numUpdated;
		}
	}
}
