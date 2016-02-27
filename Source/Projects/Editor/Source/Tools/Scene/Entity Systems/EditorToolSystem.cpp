/* ----------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** ObjectSelectorSystem.h
**
** Authors:
** - Jordan Ellis - j.ellis@digipen.edu
** - Matt Yan m.yan@digipen.edu
** --------------------------------------------------------------------------*/

#include "Precompiled.h"

#include "EditorToolSystem.h"

#include "SelectTool.h"
#include "TranslateTool.h"
#include "ScaleTool.h"
#include "RotateTool.h"
#include "DuplicateTool.h"

#include <SelectedComponent.h>

using namespace ursine;

ENTITY_SYSTEM_DEFINITION( EditorToolSystem );

EditorToolSystem::EditorToolSystem(ecs::World *world)
    : EntitySystem( world )
    , m_mouseManager( nullptr )
    , m_keyboardManager( nullptr )
    , m_editorCameraSystem( nullptr )
    , m_currentSelected( -1 )
    , m_currentTool( nullptr )
    , m_selectTool( nullptr )
    , m_dupTool( nullptr )
{

}

EditorToolSystem::~EditorToolSystem(void)
{
    m_currentTool = nullptr;
}

ecs::Entity *EditorToolSystem::GetCurrentFocus(void)
{
    return m_world->GetEntityUnique( m_currentSelected );
}

void EditorToolSystem::ClearSelectedEntities(void)
{
    URSINE_TODO( "@multi selection" );
    
    auto *current = GetCurrentFocus( );

    if (current && current->HasComponent<ecs::Selected>( ))
        current->RemoveComponent<ecs::Selected>( );
}

void EditorToolSystem::OnSceneReady(Scene *scene)
{
    auto editor = GetCoreSystem( Editor );

    m_selectTool = new SelectTool( editor, m_world );
    m_dupTool = new DuplicateTool( editor, m_world );

    m_tools[ KEY_1 ] = m_selectTool;
    m_tools[ KEY_2 ] = new TranslateTool( editor, m_world );
    m_tools[ KEY_3 ] = new ScaleTool( editor, m_world );
    m_tools[ KEY_4 ] = new RotateTool( editor, m_world );
    m_tools[ KEY_5 ] = m_dupTool;

    m_currentTool = m_tools[ KEY_1 ];

    m_mouseManager = GetCoreSystem( MouseManager );
    m_keyboardManager = GetCoreSystem( KeyboardManager );

    m_editorCameraSystem = m_world->GetEntitySystem<EditorCameraSystem>( );

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
    for (auto &p : m_tools)
        delete p.second;

    if (m_mouseManager != nullptr)
    {
        m_mouseManager->Listener( this )
            .Off( MM_BUTTON_DOWN, &EditorToolSystem::onMouseDown )
            .Off( MM_BUTTON_UP, &EditorToolSystem::onMouseUp )
            .Off( MM_MOVE, &EditorToolSystem::onMouseMove )
            .Off( MM_SCROLL, &EditorToolSystem::onMouseScroll );
    }
    
    if (m_keyboardManager != nullptr)
    {
        m_keyboardManager->Listener( this )
            .Off( KM_KEY_DOWN, &EditorToolSystem::onKeyDown )
            .Off( KM_KEY_UP, &EditorToolSystem::onKeyUp );
    }

    m_world->Listener( this )
        .Off( ecs::WorldEventType::WORLD_EDITOR_UPDATE, &EditorToolSystem::onUpdate )
        .Off( ecs::WorldEventType::WORLD_ENTITY_COMPONENT_ADDED, &EditorToolSystem::onSelectedAdd )
        .Off( ecs::WorldEventType::WORLD_ENTITY_COMPONENT_REMOVED, &EditorToolSystem::onSelectedRemoved );
}

void EditorToolSystem::onUpdate(EVENT_HANDLER(ursine::ecs::World))
{
    m_currentTool->OnUpdate( m_keyboardManager, m_mouseManager );

    if (m_currentTool != m_selectTool)
        m_selectTool->OnUpdate( m_keyboardManager, m_mouseManager );
}

void EditorToolSystem::onMouseDown(EVENT_HANDLER(ursine:MouseManager))
{
    EVENT_ATTRS(ursine::MouseManager, MouseButtonArgs);

    // must have focus or mouse focus
    if (!(m_editorCameraSystem->HasFocus( ) && m_editorCameraSystem->HasMouseFocus( )))
        return;

    m_currentTool->OnMouseDown( *args );

    // We always update the select tool
    if (m_currentTool != m_selectTool &&
        (m_currentTool != m_dupTool || args->button == MBTN_RIGHT || m_currentSelected == -1))
        m_selectTool->OnMouseDown( *args );
}

void EditorToolSystem::onMouseUp(EVENT_HANDLER(ursine:MouseManager))
{
    EVENT_ATTRS(ursine::MouseManager, MouseButtonArgs);

    // We always update the select tool
    if (m_currentTool != m_selectTool)
        m_selectTool->OnMouseUp( *args );

    m_currentTool->OnMouseUp( *args );
}

void EditorToolSystem::onMouseMove(EVENT_HANDLER(ursine:MouseManager))
{
    EVENT_ATTRS(ursine::MouseManager, MouseMoveArgs);

    // must have focus or mouse focus
    if (!(m_editorCameraSystem->HasFocus( ) || m_editorCameraSystem->HasMouseFocus( )))
        return;

    // We always update the select tool
    if (m_currentTool != m_selectTool)
        m_selectTool->OnMouseMove( *args );

    m_currentTool->OnMouseMove( *args );
}

void EditorToolSystem::onMouseScroll(EVENT_HANDLER(ursine::MouseManager))
{
    EVENT_ATTRS(ursine::MouseManager, MouseScrollArgs);

    // must have focus or mouse focus
    if (!(m_editorCameraSystem->HasFocus( ) || m_editorCameraSystem->HasMouseFocus( )))
        return;

    m_currentTool->OnMouseScroll( *args );
}

void EditorToolSystem::onKeyDown(EVENT_HANDLER(ursine::KeyboardManager))
{
    EVENT_ATTRS(ursine::KeyboardManager, KeyboardKeyArgs);

    // Check to see if the key pressed is a selection key for one of our tools
    if ((m_editorCameraSystem->HasMouseFocus( ) || m_editorCameraSystem->HasFocus( ))
        && m_tools.end( ) != m_tools.find( args->key ))
    {
        auto tool = m_tools[ args->key ];

        if (tool != m_currentTool)
        {
            m_currentTool->OnDisable( );
            m_currentTool = tool;
            m_currentTool->OnEnable( m_currentSelected );
        }
    }

    // must have focus or mouse focus
    if (!(m_editorCameraSystem->HasFocus( ) && m_editorCameraSystem->HasMouseFocus( )))
        return;

    m_currentTool->OnKeyDown( *args );

    if (m_currentTool != m_selectTool)
        m_selectTool->OnKeyDown( *args );
}

void EditorToolSystem::onKeyUp(EVENT_HANDLER(ursine::KeyboardManager))
{
    EVENT_ATTRS(ursine::KeyboardManager, KeyboardKeyArgs);

    m_currentTool->OnKeyUp( *args );

    if (m_currentTool != m_selectTool)
        m_selectTool->OnKeyUp( *args );
}

void EditorToolSystem::onSelectedAdd(EVENT_HANDLER(ursine::ecs::World))
{
    EVENT_ATTRS(ursine::ecs::World, ursine::ecs::ComponentEventArgs);

    if (args->component->Is<ecs::Selected>( ))
    {
        m_currentSelected = args->entity->GetUniqueID( );
        m_currentTool->OnSelect( args->entity );

        if (m_currentTool != m_selectTool)
            m_selectTool->OnSelect( args->entity );
    }
}

void EditorToolSystem::onSelectedRemoved(EVENT_HANDLER(ursine::ecs::World))
{
    EVENT_ATTRS(ursine::ecs::World, ursine::ecs::ComponentEventArgs);

    if (args->component->Is<ecs::Selected>( ))
    {
        // Wait one frame.  This way we avoid having a dead lock
        // when inside "World.cpp" at line 219. - Jordan

        m_currentSelected = -1;

        m_currentTool->OnDeselect( args->entity );

        if (m_currentTool != m_selectTool)
            m_selectTool->OnDeselect( args->entity );
    }
}
