/* ----------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** SelectTool.h
**
** Authors:
** - Jordan Ellis - j.ellis@digipen.edu
** - Matt Yan - m.yan@digipen.edu
** --------------------------------------------------------------------------*/

#include "Precompiled.h"

#include "SelectTool.h"
#include "SelectedComponent.h"
#include "DisableSelectionComponent.h"

#include <CoreSystem.h>
#include <GfxAPI.h>

SelectTool::SelectTool(Editor* editor, ursine::ecs::World *world)
	: EditorTool( editor, world )
	, m_currentID( -1 )
	, m_altDown( false )
{
	m_graphics = GetCoreSystem( ursine::graphics::GfxAPI );
}

SelectTool::~SelectTool(void)
{
	m_world = nullptr;
}

void SelectTool::OnMouseDown(const ursine::MouseButtonArgs& args)
{
	if (m_altDown)
		return;

	// get the current entity ID the mouse if over
	auto newID = m_graphics->GetMousedOverID( );
	
	// Unpick the previous object if the user clicked off of an entity
	if (newID == -1 && m_currentID != -1)
	{
		unpickObject( );
		m_currentID = -1;

		return;
	}

	// grab the new object, add the component
	auto newObj = m_world->GetEntityUnique( newID );

	if (!newObj)
		return;

	if (newID != m_currentID)
	{
		// Check to see if this object, or it's parents have the "DisableSelection" component
		auto disableComponents = newObj->GetComponentsInParents<DisableSelection>( );

		if (newObj->IsDeleting( ) ||
			newObj->HasComponent<DisableSelection>( ) ||
			disableComponents.size( ) != 0)
			return;

		// If the new object already has the selected component, unpick it
		if (newObj->HasComponent<Selected>( ))
		{
			unpickObject( );
			return;
		}

		// This pointer stores the entity that is to be selected
		ursine::ecs::Entity *toSelect = newObj;

		// grab the new object's root, and see if it is selected
		auto rootObj = newObj->GetRoot( );

		// if it is selected, remove it from root
		if (rootObj->HasComponent<Selected>( ))
			rootObj->RemoveComponent<Selected>( );
		// else, if root is different entity, select that one first
		else if (rootObj != newObj)
			toSelect = rootObj;		

		toSelect->AddComponent<Selected>( );

		m_currentID = toSelect->GetUniqueID( );
	}
	else
	{
		if (m_currentID != -1)
			unpickObject( );
	}
}

void SelectTool::OnSelect(ursine::ecs::Entity* selected)
{
	if (m_currentID != -1)
		unpickObject( );

	m_currentID = selected->GetUniqueID( );
}

void SelectTool::OnUpdate(ursine::KeyboardManager *kManager, ursine::MouseManager *mManager)
{
	m_altDown = kManager->IsDown( ursine::KEY_LMENU );
}

void SelectTool::unpickObject(void)
{
	auto obj = m_world->GetEntityUnique( m_currentID );

    //if it existed and it was selected, unselect
    if (obj != nullptr && obj->HasComponent<Selected>( ))
	{
        obj->RemoveComponent<Selected>( );

		m_currentID = -1;
	}
}
