/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** ECSFilterSystemTests.cpp
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#define CATCH_CONFIG_RUNNER
#include "../dep/Testing/include/catch.hpp"

#include <EntityManager.h>
#include <Application.h>
#include <World.h>
#include <RigidbodyComponent.h>
#include <AudioEmitterComponent.h>
#include <BodyComponent.h>

#include "TestFilterSystem.h"

using namespace ursine;
using namespace ecs;

int main(int argc, char *argv[])
{
	Application application( argc, argv );

	int result = Catch::Session( ).run( argc, argv );

	return result;
}

TEST_CASE("FilterSystem: Testing Adding, Updating, Removing")
{
	World world;

	std::vector<EntityUniqueID> toBeRemoved;
	int updateCount = 0;

	for (int i = 0; i < 100; ++i)
	{
		auto e = world.CreateEntity( );
		auto value = rand( ) % 4;

		switch (value)
		{
		case 0:
			e->AddComponent<Rigidbody>( );
			break;

		case 1:
			e->AddComponent<Body>( );
			break;

		case 2:
			e->AddComponent<AudioEmitter>( );
			break;

		case 3:
			e->AddComponent<Rigidbody>( );
			e->AddComponent<AudioEmitter>( );
			break;
		}

		world.Update( );

		updateCount += i + 1;

		REQUIRE(TestFilterSystem::numUpdated == updateCount);

		if (rand( ) % 2 == 0)
			toBeRemoved.push_back( e->GetUniqueID( ) );
	}

	updateCount += 100 - toBeRemoved.size( );

	for (auto &id : toBeRemoved)
	{
		auto entity = world.GetEntityUnique( id );
		
		if (entity->HasComponent<Rigidbody>( ))
			entity->RemoveComponent<Rigidbody>( );

		if (entity->HasComponent<AudioEmitter>( ))
			entity->RemoveComponent<AudioEmitter>( );

		if (entity->HasComponent<Body>( ))
			entity->RemoveComponent<Body>( );
	}

	world.Update( );

	REQUIRE(TestFilterSystem::numUpdated == updateCount);
}
