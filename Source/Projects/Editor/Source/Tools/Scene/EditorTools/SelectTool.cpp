/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
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

SelectTool::SelectTool(Editor* editor)
	: EditorTool( editor )
	, m_currentID( -1 )
	, m_altDown( false )
{
	m_graphics = GetCoreSystem( ursine::graphics::GfxAPI );
	m_world = m_editor->GetProject( )->GetScene( )->GetWorld( );
}

void SelectTool::OnMouseDown(const ursine::MouseButtonArgs& args)
{
	if (m_altDown)
		return;

	// get the current entity ID the mouse if over
	auto newID = m_graphics->GetMousedOverID( );
	
	if (newID == -1 && m_currentID != -1)
	{
		unpickObject( );
		m_currentID = -1;

		return;
	}

	// grab the new object, add the component
	auto obj = m_world->GetEntityUnique( newID );

	if (!obj)
		return;

	if (newID != m_currentID)
	{
		auto disableComponents = obj->GetComponentsInParents<DisableSelection>( );

		if (obj->IsDeleting( ) || 
			obj->HasComponent<DisableSelection>( ) ||
			disableComponents.size( ) != 0)
			return;

		// remove all selected from the currently saved IDs
		if (m_currentID != -1)
			unpickObject( );

		if (!obj->HasComponent<Selected>( ))
		{
			obj->AddComponent<Selected>( );

			m_currentID = newID;
		}
	}
	else
	{
		if (m_currentID != -1)
			unpickObject( );
	}
}

void SelectTool::OnSelect(ursine::ecs::Entity* selected)
{
	m_currentID = selected->GetUniqueID( );
}

void SelectTool::OnUpdate(ursine::KeyboardManager *kManager, ursine::MouseManager *mManager)
{
	if (kManager->IsDown( ursine::KEY_LMENU ))
		m_altDown = true;
	else
		m_altDown = false;
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
