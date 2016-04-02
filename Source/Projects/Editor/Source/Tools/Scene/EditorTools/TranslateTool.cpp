#include "Precompiled.h"

#include "TranslateTool.h"

#include <EditorConfig.h>
#include <SystemManager.h>
#include <CameraComponent.h>
#include <Model3DComponent.h>

#include "EditorDebugDrawSystems.h"
#include <DebugSystem.h>
#include <Project.h>
#include <ArchetypeData.h>
#include <EditorToolResources.h>

using namespace ursine;
using namespace ecs;
using namespace resources;

TranslateTool::TranslateTool(Editor *editor, World *world)
    : EditorTool( editor, world )
    , m_gizmo( )
    , m_selected( )
    , m_dragging( false )
    , m_hovering( false )
    , m_axisType( -1 )
    , m_snapping( false )
    , m_local( false )
    , m_archetype( editor_resources::ArchetypeTranslateTool )
    , m_toolResources( GetCoreSystem( Editor )->GetProject( )->GetBuiltInResourceManager( ) )
    , m_captured( false )
{
    m_graphics = GetCoreSystem( graphics::GfxAPI );
    m_editorCameraSystem = m_world->GetEntitySystem<EditorCameraSystem>( );
}

void TranslateTool::OnEnable(const EntityHandle &selected)
{
    m_selected = selected;

    if (m_selected)
        enableAxis( );
}

void TranslateTool::OnDisable(void)
{
    m_selected = EntityHandle::Invalid( );
    m_dragging = false;
    m_snapping = false;

    disableAxis( );
}

void TranslateTool::OnSelect(const EntityHandle &entity)
{
    auto prevSelected = m_gizmo.IsValid( );
    
    m_selected = entity;

    if (!prevSelected)
        enableAxis( );

    m_deleteGizmo = false;
}

void TranslateTool::OnDeselect(const EntityHandle &entity)
{
    m_selected = EntityHandle::Invalid( );

    m_deleteGizmo = true;
}


void TranslateTool::OnMouseDown(const MouseButtonArgs& args)
{
    // if not hovering to select a gizmo
    if (!m_hovering)
        return;

    SVec3 pos, basisX, basisY, basisZ;

    SVec3 rayStart, rayDir;

    auto newID = m_graphics->GetMousedOverID( );
    auto entity = m_world->GetEntity( newID );

    if(entity == nullptr)
        return;

    auto camera = m_editorCameraSystem->GetEditorCamera( );
    
    auto worldPos = m_graphics->GetMousedOverWorldPosition( camera->GetCameraHandle( ) );

    getBasis( entity, pos, basisX, basisY, basisZ );

    SVec3 bases[3] = { basisX, basisY, basisZ };

    m_planeNormal = bases[ (m_axisType) % 3 ];
    SVec3 otherAxis = bases[ (m_axisType + 1) % 3 ];
    SVec3 eyeDirection = camera->GetLook( );

    float axisA = eyeDirection.Dot(m_planeNormal);
    float axisB = eyeDirection.Dot( otherAxis );

    if(abs(axisA) < abs(axisB))
    {
        float temp = axisB;
        axisB = axisA;
        axisA = temp;
    }

    // intersect with vector from camera to mouse, and the plane we just created
    rayStart = m_editorCameraSystem->GetEditorCameraEntity( )->GetTransform( )->GetWorldPosition( );
    rayDir = worldPos - rayStart;
    rayDir.Normalize( );

    float w = m_planeNormal.Dot( pos );
    float time = (w - m_planeNormal.Dot(rayStart)) / m_planeNormal.Dot(rayDir);

    m_localPoint = rayStart + rayDir * time;
    m_captured = true;
    m_localOffset = bases[m_axisType - 1].Dot( m_localPoint - pos );
    m_dragging = true;
}

void TranslateTool::OnMouseUp(const MouseButtonArgs& args)
{
    m_captured = false;
    m_dragging = false;
}

void TranslateTool::OnMouseMove(const MouseMoveArgs &args)
{
    if(m_selected == nullptr)
        return;

    if(m_captured)
    {
        SVec3 pos, basisX, basisY, basisZ;

        getBasis(m_selected, pos, basisX, basisY, basisZ);

        SVec3 bases[ 3 ] = { basisX, basisY, basisZ };
        SVec3 dir = bases[ m_axisType - 1 ];

        std::cout << "DIR: " << dir.X( ) << ", " << dir.Y( ) << ", " << dir.Z( ) << std::endl;

        auto camera = m_editorCameraSystem->GetEditorCamera( );
        auto worldPos = m_graphics->GetMousedOverWorldPosition(camera->GetCameraHandle());

        SVec3 rayStart, rayDir;
        rayStart = m_editorCameraSystem->GetEditorCameraEntity( )->GetTransform( )->GetWorldPosition( );
        rayDir = worldPos - rayStart;
        rayDir.Normalize( );

        float w = m_planeNormal.Dot( pos );
        float time = (w - m_planeNormal.Dot(rayStart)) / m_planeNormal.Dot(rayDir);

        SVec3 pt = rayStart + rayDir * time;

        float offset = dir.Dot( pt - pos );

        auto *selectedTransform = m_selected->GetTransform( );

        auto position = selectedTransform->GetWorldPosition( );
        position += dir * (offset - m_localOffset);
        selectedTransform->SetWorldPosition( position );
    }
    //if (m_dragging && m_selected)
    //{
    //    // Project the delta vector onto the screen direction vector
    //    auto b = args.positionDelta;
    //    auto &a = m_screenDir;

    //    // invert the Y
    //    b.Y( ) = -b.Y( );

    //    float dot = a.Dot( b );
    //    auto proj = ( dot / b.LengthSquared( ) ) * b;
    //    auto dist = proj.Length( );
    //    auto selected = m_selected->GetTransform( );
    //    auto gizmo = m_gizmo->GetTransform( );

    //    if (dot < 0.0f)
    //        dist = -dist;
    //    
    //    // Multiply by an arbitrary value so that it feels nice.
    //    // Ideally we would convert the screen space to a world space distance
    //    // and then apply it to the world space direction vector.
    //    dist *= 0.25f;

    //    auto newP = gizmo->GetWorldPosition( ) + m_worldDir * dist * sqrt(m_editorCameraSystem->GetCamZoom( ));

    //    gizmo->SetWorldPosition( newP );

    //    if (m_snapping)
    //    {
    //        for (int i = 0; i < 3; ++i)
    //        {
    //            if (newP[ i ] > 0.0f)
    //                newP[ i ] = float(int(newP[ i ] * 2.0f + 0.5f)) / 2.0f;
    //            else
    //                newP[ i ] = float(int(newP[ i ] * 2.0f - 0.5f)) / 2.0f;
    //        }

    //        selected->SetWorldPosition( newP );
    //    }
    //    else
    //        selected->SetWorldPosition( newP );
    //}
}

void TranslateTool::OnKeyDown(const KeyboardKeyArgs &args)
{
    if (args.key == KEY_G)
        m_local = !m_local;

    if (args.key == KEY_LCONTROL)
        m_snapping = true;
}

void TranslateTool::OnKeyUp(const KeyboardKeyArgs &args)
{
    if (args.key == KEY_CONTROL)
        m_snapping = false;
}

void TranslateTool::OnUpdate(KeyboardManager *kManager, MouseManager *mManager)
{
    if (m_deleteGizmo)
    {
        disableAxis( );
        m_deleteGizmo = false;
    }

    std::cout <<  std::endl;
    std::cout << "IS HOVERING: " << m_hovering << std::endl;
    std::cout << "IS DRAGGING: " << m_dragging << std::endl;
    std::cout << "OFFSET: " << m_localOffset << std::endl;
    std::cout << "AXIS: " << m_axisType << std::endl;

    //m_altDown = kManager->IsDown( KEY_LMENU );

    updateAxis( );
    renderAxis( );
    updateHoverAxis( );
}

void TranslateTool::setDirectionVectors(const SVec3& basisVector, const EntityHandle &selected)
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

void TranslateTool::enableAxis(void)
{
    m_gizmo = m_archetype.Load<ArchetypeData>( m_toolResources )->Instantiate( m_world );

    if (!m_gizmo)
        return;

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

void TranslateTool::disableAxis(void)
{
    if (m_gizmo)
    {
        m_gizmo->Delete( );
    }

    m_gizmo = EntityHandle::Invalid( );
    m_axis = nullptr;
}

void TranslateTool::updateAxis(void)
{
    if (!m_selected|| !m_gizmo)
        return;

    // update the size of the gizmo
    auto gizTrans = m_gizmo->GetTransform( );

    auto camVec = gizTrans->GetWorldPosition() -
        m_editorCameraSystem->GetEditorCameraEntity()->GetTransform()->GetWorldPosition();

    gizTrans->SetWorldScale( SVec3(camVec.Length() * 0.04f) );

    // Update the position of things
    auto selTrans = m_selected->GetTransform( );

    gizTrans->SetWorldPosition( selTrans->GetWorldPosition( ) );

    if (m_local)
        gizTrans->SetWorldRotation( selTrans->GetWorldRotation( ) );
    else
        gizTrans->SetLocalRotation( SQuat::Identity( ) );
}

void TranslateTool::renderAxis()
{
    auto drawer = m_world->GetEntitySystem<DebugSystem>( );

    if ( m_gizmo )
    {
        auto gizTrans = m_gizmo->GetTransform();

        auto camVec = gizTrans->GetWorldPosition() -
            m_editorCameraSystem->GetEditorCameraEntity()->GetTransform()->GetWorldPosition();

        float size = camVec.Length() * 0.18f;

        auto position = gizTrans->GetWorldPosition();

        SVec3 xAxis = SVec3(1, 0, 0), yAxis = SVec3(0, 1, 0), zAxis = SVec3(0, 0, 1);

        if ( m_local )
        {
            xAxis = gizTrans->GetWorldRotation().Rotate(xAxis);
            yAxis = gizTrans->GetWorldRotation().Rotate(yAxis);
            zAxis = gizTrans->GetWorldRotation().Rotate(zAxis);
        }

        Color xColor = Color::Red, yColor = Color::Green, zColor = Color::Blue;

        switch ( m_axisType )
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
        default:
            break;
        }

        // rays
        drawer->DrawRay( position, xAxis, size, xColor, 0, true );
        drawer->DrawRay( position, yAxis, size, yColor, 0, true );
        drawer->DrawRay( position, zAxis, size, zColor, 0, true );

        // two-axis movers
        // xy
        float twoAxisSize = size * 0.2f;

        if ( m_axisType == 6 )
        {
            drawer->DrawLine( position + xAxis * twoAxisSize + yAxis * twoAxisSize,
                xAxis * twoAxisSize + position, Color::Yellow, 0, true );
            drawer->DrawLine( position + xAxis * twoAxisSize + yAxis * twoAxisSize,
                yAxis * twoAxisSize + position, Color::Yellow, 0, true );
        }
        else
        {
            drawer->DrawLine( position + xAxis * twoAxisSize + yAxis * twoAxisSize,
                xAxis * twoAxisSize + position, Color::Red, 0, true );
            drawer->DrawLine( position + xAxis * twoAxisSize + yAxis * twoAxisSize,
                yAxis * twoAxisSize + position, Color::Green, 0, true );
        }

        // zy
        if ( m_axisType == 5 )
        {
            drawer->DrawLine( position + zAxis * twoAxisSize + yAxis * twoAxisSize,
                zAxis * twoAxisSize + position, Color::Yellow, 0, true );
            drawer->DrawLine( position + zAxis * twoAxisSize + yAxis * twoAxisSize,
                yAxis * twoAxisSize + position, Color::Yellow, 0, true );
        }
        else
        {
            drawer->DrawLine( position + zAxis * twoAxisSize + yAxis * twoAxisSize,
                zAxis * twoAxisSize + position, Color::Blue, 0, true );
            drawer->DrawLine( position + zAxis * twoAxisSize + yAxis * twoAxisSize,
                yAxis * twoAxisSize + position, Color::Green, 0, true );
        }

        // xz
        if ( m_axisType == 4 )
        {
            drawer->DrawLine( position + xAxis * twoAxisSize + zAxis * twoAxisSize,
                xAxis * twoAxisSize + position, Color::Yellow, 0, true );
            drawer->DrawLine( position + xAxis * twoAxisSize + zAxis * twoAxisSize,
                zAxis * twoAxisSize + position, Color::Yellow, 0, true );
        }
        else
        {
            drawer->DrawLine( position + xAxis * twoAxisSize + zAxis * twoAxisSize,
                xAxis * twoAxisSize + position, Color::Red, 0, true );
            drawer->DrawLine( position + xAxis * twoAxisSize + zAxis * twoAxisSize,
                zAxis * twoAxisSize + position, Color::Blue, 0, true );
        }
    }
}

void TranslateTool::updateHoverAxis(void)
{
    // get the current entity ID the mouse is over
    auto newID = m_graphics->GetMousedOverID( );
    auto entity = m_world->GetEntity( newID );

    /*if (!entity || m_altDown)
    {
        disableHover( );
        return;
    }*/

    if(m_dragging)
        return;

    if(entity == nullptr)
    {
        m_hovering = false;
        m_axisType = -1;
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
    if (rootName.find( "TranslationGizmo" ) != std::string::npos)
    {
        if (!m_selected)
        {
            m_deleteGizmo = true;
            m_selected = EntityHandle::Invalid( );

            return;
        }

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
        else
        {
            name = entity->GetName( );

            if (name == "zxPlane")
            {
                m_axisType = 4;
                setDirectionVectors( SVec3( 1.0f, 0.0f, 1.0f ), m_selected );
            }
            else if (name == "yzPlane")
            {
                m_axisType = 5;
                setDirectionVectors( SVec3( 0.0f, 1.0f, 1.0f ), m_selected );
            }
            else if (name == "xyPlane")
            {
                m_axisType = 6;
                setDirectionVectors( SVec3( 1.0f, 1.0f, 0.0f ), m_selected );
            }
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
    else
    {
        disableHover();
        m_hovering = false;
        m_axisType = -1;
    }
}

void TranslateTool::disableHover(void)
{
    m_hovering = false;
        
    if (m_axis)
    {
        m_axis->SetColor( m_axisOrigColor );
        m_axis = nullptr;
        m_axisType = 0;
    }
}

void TranslateTool::setEntitySerializationToggle(bool toggle, const EntityHandle &entity)
{
    for (auto &child : entity->GetTransform( )->GetChildren( ))
    {
        setEntitySerializationToggle( toggle, child->GetOwner( ) );
    }

    entity->EnableSerialization( toggle );
}

void TranslateTool::getBasis(EntityHandle obj, ursine::SVec3 &pos, ursine::SVec3 &basisX, ursine::SVec3 &basisY, ursine::SVec3 &basisZ)
{
    if(obj == nullptr)
        return;

    auto *transform = obj->GetTransform( );

    if(transform != nullptr)
    {
        pos = transform->GetWorldPosition( );
        basisX = ursine::SVec3(1, 0, 0);
        basisY = ursine::SVec3(0, 1, 0);
        basisZ = ursine::SVec3(0, 0, 1);
    }
}
