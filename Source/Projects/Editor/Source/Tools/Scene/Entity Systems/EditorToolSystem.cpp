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

#include "SelectedComponent.h"

using namespace ursine;

ENTITY_SYSTEM_DEFINITION(EditorToolSystem);

EditorToolSystem::EditorToolSystem(ursine::ecs::World* world)
	: EntitySystem( world )
	, m_currentTool( nullptr )
	, m_currentID( -1 )
{
	// m_tools[ KEY_1 ] = new SelectTool( );
	// m_tools[ KEY_2 ] = new TranslateTool( );
	// m_tools[ KEY_3 ] = new ScaleTool( );
	// m_tools[ KEY_4 ] = new RotateTool( );
	// m_tools[ KEY_5 ] = new DuplicateTool( );

	m_currentTool = m_tools[ KEY_1 ];
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
		.On( ecs::WorldEventType::WORLD_ENTITY_COMPONENT_ADDED, &EditorToolSystem::onSelectedAdd )
		.On( ecs::WorldEventType::WORLD_ENTITY_COMPONENT_REMOVED, &EditorToolSystem::onSelectedRemoved );
}

void EditorToolSystem::OnRemove(void)
{
	m_mouseManager->Listener( this )
		.Off( MM_BUTTON_DOWN, &EditorToolSystem::onMouseDown )
		.Off( MM_BUTTON_UP, &EditorToolSystem::onMouseUp )
		.Off( MM_MOVE, &EditorToolSystem::onMouseMove )
		.Off( MM_SCROLL, &EditorToolSystem::onMouseScroll );

	m_keyboardManager->Listener( this )
		.Off( KM_KEY_DOWN, &EditorToolSystem::onKeyDown )
		.Off( KM_KEY_UP, &EditorToolSystem::onKeyUp );

	m_world->Listener( this )
		.Off( ecs::WorldEventType::WORLD_EDITOR_UPDATE, &EditorToolSystem::onUpdate )
		.Off( ecs::WorldEventType::WORLD_ENTITY_COMPONENT_ADDED, &EditorToolSystem::onSelectedAdd )
		.Off( ecs::WorldEventType::WORLD_ENTITY_COMPONENT_REMOVED, &EditorToolSystem::onSelectedRemoved );
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
	EVENT_ATTRS(ursine::MouseManager, MouseButtonArgs);
}

void EditorToolSystem::onMouseMove(EVENT_HANDLER(ursine:MouseManager))
{
	EVENT_ATTRS(ursine::MouseManager, MouseMoveArgs);
}

void EditorToolSystem::onMouseScroll(EVENT_HANDLER(ursine::MouseManager))
{
	EVENT_ATTRS(ursine::MouseManager, MouseScrollArgs);
}

void EditorToolSystem::onKeyDown(EVENT_HANDLER(ursine::KeyboardManager))
{
	EVENT_ATTRS(ursine::KeyboardManager, KeyboardKeyArgs);
}

void EditorToolSystem::onKeyUp(EVENT_HANDLER(ursine::KeyboardManager))
{
	EVENT_ATTRS(ursine::KeyboardManager, KeyboardKeyArgs);
}

void EditorToolSystem::onSelectedAdd(EVENT_HANDLER(ursine::ecs::World))
{
	EVENT_ATTRS(ursine::ecs::World, ursine::ecs::ComponentEventArgs);

	if (args->component->Is<Selected>( ))
	{
		m_currentID = args->entity->GetUniqueID( );
		m_currentTool->OnSelect( args->entity );
	}
}

void EditorToolSystem::onSelectedRemoved(EVENT_HANDLER(ursine::ecs::World))
{
	EVENT_ATTRS(ursine::ecs::World, ursine::ecs::ComponentEventArgs);

	if (args->component->Is<Selected>( ))
	{
		m_currentID = -1;
		m_currentTool->OnDeselect( args->entity );
	}
}