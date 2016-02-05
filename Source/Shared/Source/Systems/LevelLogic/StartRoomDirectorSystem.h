/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** StartRoomDirectorSystem.h
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#pragma once

#include <EntitySystem.h>

enum StartRoomDirectorEvents
{
	PLAYERS_SPAWNED,
	ELEVATOR_START,
	ELEVATOR_END
};

class StartRoomDirectorSystem 
	: public ursine::ecs::EntitySystem
	, public ursine::EventDispatcher<StartRoomDirectorEvents>
{
	ENTITY_SYSTEM;

public:
	StartRoomDirectorSystem(ursine::ecs::World *world);

private:
	void OnAfterLoad(void) override;
	void OnRemove(void) override;
} Meta(Enable);