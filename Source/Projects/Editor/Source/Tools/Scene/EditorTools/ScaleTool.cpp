/* ----------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** ScaleTool.cpp
**
** Authors:
** - Jordan Ellis - j.ellis@digipen.edu
** - Matt Yan - m.yan@digipen.edu
** --------------------------------------------------------------------------*/

#include "Precompiled.h"

#include "ScaleTool.h"

#include <EditorConfig.h>
#include <SystemManager.h>
#include <CameraComponent.h>
#include <Model3DComponent.h>
#include <DebugSystem.h>
#include <EditorToolResources.h>

using namespace ursine;
using namespace ecs;

ScaleTool::ScaleTool(Editor *editor, ursine::ecs::World *world)
    : EditorTool( editor, world )
    , m_gizmo( )
    , m_selected( )
    , m_dragging( false )
    , m_hovering( false )
    , m_axisType( -1 )
    , m_snapping( false )
    , m_local( false )
    , m_deleteGizmo( false )
    , m_archetype( editor_resources::ArchetypeScaleTool )
{
    m_graphics = GetCoreSystem( graphics::GfxAPI );
    m_editorCameraSystem = m_world->GetEntitySystem<EditorCameraSystem>( );
}

void ScaleTool::OnEnable(const EntityHandle &selected)
{
    m_selected = selected;

    if (m_selected)
        enableAxis( );
}

void ScaleTool::OnDisable(void)
{
    m_selected = EntityHandle::Invalid( );
    m_dragging = false;
    m_snapping = false;
    
    disableAxis( );
}

void ScaleTool::OnSelect(const EntityHandle &entity)
{
    auto prevSelected = m_gizmo.IsValid( );
    
    m_selected = entity;

    if (!prevSelected)
        enableAxis( );

    m_deleteGizmo = false;
}

void ScaleTool::OnDeselect(const EntityHandle &entity)
{
    m_selected = EntityHandle::Invalid( );
    
    m_deleteGizmo = true;
}

void ScaleTool::OnMouseDown(const MouseButtonArgs& args)
{
    if (m_hovering)
        m_dragging = true;
}

void ScaleTool::OnMouseUp(const MouseButtonArgs& args)
{
    if (m_dragging)
        m_dragging = false;
}

void ScaleTool::OnMouseMove(const MouseMoveArgs& args)
{
    if (m_dragging && m_selected)
    {
        // Project the delta vector onto the screen direction vector
        auto b = args.positionDelta;
        auto &a = m_screenDir;

        // invert the Y
        b.Y( ) = -b.Y( );

        float dot = a.Dot( b );
        auto proj = ( dot / b.LengthSquared( ) ) * b;
        auto dist = proj.Length( );
        auto selected = m_selected->GetTransform( );

        if (dot < 0.0f)
            dist = -dist;
        
        // Multiply by an arbitrary value so that it feels nice.
        // Ideally we would convert the screen space to a world space distance
        // and then apply it to the world space direction vector.
        dist *= 0.5f;

        auto modifier = m_worldDir * dist;

        if (m_local)
            modifier = selected->GetWorldRotation( ).GetInverse( ) * modifier;
        else
            modifier = selected->GetWorldRotation( ) * modifier;

        if (m_uniform)
        {
            auto max = math::Max( modifier.X( ), modifier.Y( ), modifier.Z( ) );
            auto min = math::Min( modifier.X( ), modifier.Y( ), modifier.Z( ) );

            if (abs( max ) > abs( min ))
                modifier = SVec3( max );
            else
                modifier = SVec3( min );
        }

        auto newS = selected->GetLocalScale( ) + modifier;

        if (m_snapping)
        {
            for (int i = 0; i < 3; ++i)
            {
                if (newS[ i ] > 0.0f)
                    newS[ i ] = float(int(newS[ i ] * 2.0f + 0.5f)) / 2.0f;
                else
                    newS[ i ] = float(int(newS[ i ] * 2.0f - 0.5f)) / 2.0f;
            }

            selected->SetLocalScale( newS );
        }
        else
            selected->SetLocalScale( newS );
    }
}

void ScaleTool::OnKeyDown(const KeyboardKeyArgs& args)
{
    if (args.key == KEY_G)
        m_local = !m_local;

    if (args.key == KEY_LCONTROL)
        m_snapping = true;

    if (args.key == KEY_LMENU)
        m_dragging = false;
}

void ScaleTool::OnKeyUp(const KeyboardKeyArgs& args)
{
    if (args.key == KEY_CONTROL)
        m_snapping = false;
}

void ScaleTool::OnUpdate(KeyboardManager* kManager, MouseManager* mManager)
{
    if (m_deleteGizmo)
    {
        disableAxis( );
        m_deleteGizmo = false;
    }

    m_altDown = kManager->IsDown( KEY_LMENU );
    m_uniform = kManager->IsDown( KEY_LSHIFT );

    updateAxis( );

    renderAxis( );

    if (!m_dragging)
    {
        updateHoverAxis( );
    }
}

void ScaleTool::setDirectionVectors(const SVec3& basisVector, const EntityHandle &selected)
{
    if (m_local)
        m_worldDir = selected->GetTransform( )->GetWorldRotation( ) * basisVector;
    else
        m_worldDir = basisVector;

    // Set the screen direction
    auto cam = m_editorCameraSystem->GetEditorCamera( );
    auto worldP = selected->GetTransform( )->GetWorldPosition( );
    auto p1 = cam->WorldToScreen( worldP );
    auto p2 = cam->WorldToScreen( worldP + m_worldDir );
    
    m_screenDir = p2 - p1;
    m_screenDir.Normalize( );
}

void ScaleTool::enableAxis(void)
{
    m_gizmo = m_world->CreateEntityFromArchetype(
        m_archetype
    );

    setEntitySerializationToggle( false, m_gizmo );
    
    // Make it invisible in the hiearchy
    m_gizmo->SetVisibleInEditor( false );

    // Make all models draw over everything
    for (auto &model : m_gizmo->GetComponentsInChildren<ecs::Model3D>( ))
    {
        model->SetOverdraw( true );
        model->SetMaterialData( 4, 0, 0 );
    }
}

void ScaleTool::disableAxis(void)
{
    if (m_gizmo)
    {
        m_gizmo->Delete( );
    }

    m_gizmo = EntityHandle::Invalid( );
    m_axis = nullptr;
}

void ScaleTool::updateAxis(void)
{
    if (!m_selected || !m_gizmo)
        return;

    // update the size of the gizmo
    auto gizTrans = m_gizmo->GetTransform( );

    auto camVec = gizTrans->GetWorldPosition() -
        m_editorCameraSystem->GetEditorCameraEntity()->GetTransform()->GetWorldPosition();

    gizTrans->SetWorldScale( SVec3(camVec.Length() * 0.04f ) );

    auto selTrans = m_selected->GetTransform( );

    gizTrans->SetWorldPosition( selTrans->GetWorldPosition( ) );

    if (m_local)
        gizTrans->SetWorldRotation( selTrans->GetWorldRotation( ) );
    else
        gizTrans->SetLocalRotation( SQuat::Identity( ) );
}

void ScaleTool::renderAxis()
{
    auto drawer = m_world->GetEntitySystem<DebugSystem>( );

    if ( m_gizmo )
    {
        auto gizTrans = m_gizmo->GetTransform( );

        auto camVec = gizTrans->GetWorldPosition( ) -
            m_editorCameraSystem->GetEditorCameraEntity( )->GetTransform( )->GetWorldPosition( );

        float size = camVec.Length( ) * 0.15f;

        auto position = gizTrans->GetWorldPosition( );

        SVec3 xAxis = SVec3( size, 0, 0 ), yAxis = SVec3( 0, size, 0 ), zAxis = SVec3( 0, 0, size );

        if ( m_local )
        {
            xAxis = gizTrans->GetWorldRotation().Rotate(xAxis);
            yAxis = gizTrans->GetWorldRotation().Rotate(yAxis);
            zAxis = gizTrans->GetWorldRotation().Rotate(zAxis);
        }

        Color xColor = Color::Red, yColor = Color::Green, zColor = Color::Blue, mainColor = Color::White;

        switch( m_axisType )
        {
        case 1:
            xColor = Color::Yellow;
            break;
        case 2:
            yColor = Color::Yellow;
            break;
        case 3:
            zColor = Color::Yellow;
            break;
        case 4:
            mainColor = Color::Yellow;
            break;
        default:
            break;
        }

        drawer->DrawLine( position, position + xAxis, xColor, 0, true );
        drawer->DrawLine( position, position + yAxis, yColor, 0, true );
        drawer->DrawLine( position, position + zAxis, zColor, 0, true );

        drawer->DrawCube( position, size * 0.1f, mainColor, 0, true );

        drawer->DrawSphere( position + xAxis, size * 0.1f, xColor, 0, true );
        drawer->DrawSphere( position + yAxis, size * 0.1f, yColor, 0, true );
        drawer->DrawSphere( position + zAxis, size * 0.1f, zColor, 0, true );
    }
}

void ScaleTool::updateHoverAxis(void)
{
    // get the current entity ID the mouse is over
    auto newID = m_graphics->GetMousedOverID( );
    auto entity = m_world->GetEntity( newID );

    if (!entity || m_altDown)
    {
        disableHover( );
        return;
    }

    auto entityTrans = entity->GetTransform( );

    auto root = entityTrans->GetRoot( );

    if (!root)
    {
        disableHover( );
        return;
    }

    auto rootName = root->GetOwner( )->GetName( );

    // if we're clicking on ourselves, set the dragging flag,
    // and the vector we're dragging on
    if (rootName.find( "ScaleGizmo" ) != std::string::npos)
    {
        // Get the gizmo's name (the models are under the parent named the axis' name)
        auto name = entityTrans->GetParent( )->GetOwner( )->GetName( );

        if (name == "xAxis")
        {
            setDirectionVectors( SVec3::UnitX( ), m_selected );
            m_axisType = 1;
        }
        else if (name == "yAxis")
        {
            setDirectionVectors( SVec3::UnitY( ), m_selected );
            m_axisType = 2;
        }
        else if (name == "zAxis")
        {
            setDirectionVectors( SVec3::UnitZ( ), m_selected );
            m_axisType = 3;
        }
        else if(name == "allAxis")
        {
            m_axisType = 4;
            setDirectionVectors( SVec3( 1.0f, 1.0f, 1.0f ), m_selected );
        }
        else
        {
            m_axisType = 0;
            return;
        }

        auto model = entity->GetComponent<Model3D>( );

        if (!model)
            return;

        // If we're hovering over a new axis
        if (m_axis && entity != m_axis->GetOwner( ))
            disableHover( );

        // if the handle is null, store the original color
        if (!m_axis)
            m_axisOrigColor = model->GetColor( );

        // set the axis handle
        m_axis = entity->GetComponent<Model3D>( );

        // set the axis' color
        m_axis->SetColor( Color::Yellow );

        m_hovering = true;
    }
}

void ScaleTool::disableHover(void)
{
    m_hovering = false;
        
    if (m_axis)
    {
        m_axis->SetColor( m_axisOrigColor );
        m_axis = nullptr;
        m_axisType = 0;
    }
}

void ScaleTool::setEntitySerializationToggle(bool toggle, const EntityHandle &entity)
{
    for (auto &child : entity->GetTransform( )->GetChildren( ))
    {
        setEntitySerializationToggle( toggle, child->GetOwner( ) );
    }

    entity->EnableSerialization( toggle );
}
