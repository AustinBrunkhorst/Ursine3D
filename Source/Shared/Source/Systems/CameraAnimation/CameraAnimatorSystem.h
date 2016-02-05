/* ----------------------------------------------------------------------------
** Team Bear King
** © 2016 DigiPen Institute of Technology, All Rights Reserved.
**
** CameraAnimatorSystem.h
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** --------------------------------------------------------------------------*/

#pragma once

#include <FilterSystem.h>

class CameraAnimatorSystem : public ursine::ecs::FilterSystem
{
	ENTITY_SYSTEM;

public:
	
	CameraAnimatorSystem(ursine::ecs::World *world);

private:
	
	void Process(ursine::ecs::Entity *entity) override;

} Meta(Enable, AutoAddEntitySystem);
