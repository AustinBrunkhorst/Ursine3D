/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** ObjectSelectorSystem.h
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** - Matt Yan m.yan@digipen.edu
** --------------------------------------------------------------------------*/

#include "Precompiled.h"

#include "EditorToolSystem.h"

using namespace ursine;

ENTITY_SYSTEM_DEFINITION(EditorToolSystem);

EditorToolSystem::EditorToolSystem(ursine::ecs::World* world)
	: EntitySystem( world )
{
}

ursine::ecs::Entity* EditorToolSystem::GetCurrentFocus(void)
{
	return m_world->GetEntityUnique( m_currentID );
}

void EditorToolSystem::OnInitialize(void)
{
	m_mouseManager = GetCoreSystem( MouseManager );
	m_keyboardManager = GetCoreSystem( KeyboardManager );

	m_mouseManager->Listener( this )
		.On( MM_BUTTON_DOWN, &EditorToolSystem::onMouseDown )
		.On( MM_BUTTON_UP, &EditorToolSystem::onMouseUp )
		.On( MM_MOVE, &EditorToolSystem::onMouseMove )
		.On( MM_SCROLL, &EditorToolSystem::onMouseScroll );

	m_keyboardManager->Listener( this )
		.On( KM_KEY_DOWN, &EditorToolSystem::onKeyDown )
		.On( KM_KEY_UP, &EditorToolSystem::onKeyUp );

	m_world->Listener( this )
		.On( ecs::WorldEventType::WORLD_EDITOR_UPDATE, &EditorToolSystem::onUpdate )
		.On( ecs::WorldEventType::WORLD_ENTITY_COMPONENT_ADDED, &EditorToolSystem::onSelectedAdd );
}

void EditorToolSystem::OnRemove(void)
{
}

void EditorToolSystem::onUpdate(EVENT_HANDLER(ursine::ecs::World))
{
}

void EditorToolSystem::onMouseDown(EVENT_HANDLER(ursine:MouseManager))
{
	EVENT_ATTRS(ursine::MouseManager, MouseButtonArgs);
}

void EditorToolSystem::onMouseUp(EVENT_HANDLER(ursine:MouseManager))
{
}

void EditorToolSystem::onMouseMove(EVENT_HANDLER(ursine:MouseManager))
{
}

void EditorToolSystem::onKeyDown(EVENT_HANDLER(ursine::KeyboardManager))
{
}

void EditorToolSystem::onKeyUp(EVENT_HANDLER(ursine::KeyboardManager))
{
}

void EditorToolSystem::onSelectedAdd(EVENT_HANDLER(ursine::ecs::World))
{
}