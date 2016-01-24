﻿/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** ObjectSelectorSystem.cpp
**
** Author:
** - Austin Brunkhorst - a.brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#include "Precompiled.h"

#include "ObjectSelectorSystem.h"
#include "SelectedComponent.h"

#include "EditorCameraSystem.h"

#include <Vec2.h>
#include <Vec3.h>

#include <Model3DComponent.h>
#include <RigidbodyComponent.h>
#include <BodyComponent.h>
#include <CameraComponent.h>

#include <SystemManager.h>
#include <GfxAPI.h>
#include <WorldEvent.h>
#include <Game Engine/Scene/Component/Native Components/Physics/BoxColliderComponent.h>
#include <Game Engine/Scene/Component/Native Components/CameraComponent.h>

using namespace ursine;

ENTITY_SYSTEM_DEFINITION( ObjectSelectorSystem );

ObjectSelectorSystem::ObjectSelectorSystem(ecs::World *world)
    : FilterSystem( world, ecs::Filter( ).All<ecs::Rigidbody, Selected>( ) )
    , m_xAxis( nullptr )
    , m_yAxis( nullptr )
    , m_zAxis( nullptr )
    , m_currentID( -1 )
    , m_graphics( nullptr )
    , m_mouseManager( nullptr )
    , m_keyboardManager( nullptr )
    , m_editorCameraSystem( nullptr )
    , m_dragging( false )
    , m_axis( -1 )
    , m_currentTool( TOOL_TRANSLATION )
    , m_baseTranslation( Vec3( 0, 0, 0 ) )
    , m_offset( 0 )
    , m_baseScale( Vec3( 1, 1, 1 ) )
    , m_baseMousePos( Vec3( 0, 0, 0 ) ) { }

ecs::Entity *ObjectSelectorSystem::GetCurrentFocus(void)
{
    return m_world->GetEntityUnique( m_currentID );
}

void ObjectSelectorSystem::OnInitialize(void)
{
    FilterSystem::SetUpdateType( ecs::WorldEventType::WORLD_EDITOR_UPDATE );
    FilterSystem::OnInitialize( );

    //grab graphics
    m_graphics = GetCoreSystem( graphics::GfxAPI );
    m_mouseManager = GetCoreSystem( MouseManager );
    m_keyboardManager = GetCoreSystem( KeyboardManager );

    m_editorCameraSystem = m_world->GetEntitySystem( EditorCameraSystem );

    //connect to the mouse events
    m_mouseManager->Listener( this )
        .On( MM_BUTTON_DOWN, &ObjectSelectorSystem::onMouseDown )
        .On( MM_BUTTON_UP, &ObjectSelectorSystem::onMouseUp );

    m_keyboardManager->Listener( this )
        .On( KM_KEY_DOWN, &ObjectSelectorSystem::onKeyDown );

    m_world->Listener( this )
        .On( ecs::WorldEventType::WORLD_EDITOR_UPDATE, &ObjectSelectorSystem::onUpdate )
        .On( ecs::WorldEventType::WORLD_EDITOR_UPDATE, &ObjectSelectorSystem::onMouseUpdate )
        .On( ecs::WorldEventType::WORLD_ENTITY_COMPONENT_ADDED, &ObjectSelectorSystem::onSelectedAdd );

    //construct the 3 axis
    m_xAxis = m_world->CreateEntity( );
    m_xAxis->EnableSerialization( false );
    m_xAxis->SetVisibleInEditor( false );

    m_yAxis = m_world->CreateEntity( );
    m_yAxis->EnableSerialization( false );
    m_yAxis->SetVisibleInEditor( false );

    m_zAxis = m_world->CreateEntity( );
    m_zAxis->EnableSerialization( false );
    m_zAxis->SetVisibleInEditor( false );

    //get their transforms, set data
    auto xTransf = m_xAxis->GetTransform( );
    auto yTransf = m_yAxis->GetTransform( );
    auto zTransf = m_zAxis->GetTransform( );
    {
        //translation
        xTransf->SetWorldPosition( SVec3( 2000, 0, 0 ) );
        yTransf->SetWorldPosition( SVec3( 0, 2000, 0 ) );
        zTransf->SetWorldPosition( SVec3( 0, 0, 2000 ) );

        //rotation
        xTransf->SetWorldRotation( SQuat( 90, SVec3( 0, 0, 1 ) ) );
        yTransf->SetWorldRotation( SQuat( 0, SVec3( 0, 0, 1 ) ) );
        zTransf->SetWorldRotation( SQuat( 90, SVec3( 1, 0, 0 ) ) );

        xTransf->SetWorldScale( SVec3( 0.1f, 1.1f, 0.1f ) );
        yTransf->SetWorldScale( SVec3( 0.1f, 1.1f, 0.1f ) );
        zTransf->SetWorldScale( SVec3( 0.1f, 1.1f, 0.1f ) );
    }

    //give them all renderables and models
    auto xModel = m_xAxis->AddComponent<ecs::Model3D>( );
    auto yModel = m_yAxis->AddComponent<ecs::Model3D>( );
    auto zModel = m_zAxis->AddComponent<ecs::Model3D>( );

    {
        xModel->SetModelResourceName( "Cylinder" );
        yModel->SetModelResourceName( "Cylinder" );
        zModel->SetModelResourceName( "Cylinder" );

        xModel->SetMaterial( "Blank" );
        yModel->SetMaterial( "Blank" );
        zModel->SetMaterial( "Blank" );

        xModel->SetColor( Color( 1, 0, 0, 1 ) );
        yModel->SetColor( Color( 0, 1, 0, 1 ) );
        zModel->SetColor( Color( 0, 0, 1, 1 ) );

        xModel->SetMaterialData( 8, 0, 0 );
        yModel->SetMaterialData( 8, 0, 0 );
        zModel->SetMaterialData( 8, 0, 0 );

        xModel->SetOverdraw( true );
        yModel->SetOverdraw( true );
        zModel->SetOverdraw( true );
    }
}

void ObjectSelectorSystem::OnRemove(void)
{
    FilterSystem::OnRemove( );

    m_mouseManager->Listener( this )
        .Off( MM_BUTTON_DOWN, &ObjectSelectorSystem::onMouseDown )
        .Off( MM_BUTTON_UP, &ObjectSelectorSystem::onMouseUp );

    m_keyboardManager->Listener( this )
        .Off( KM_KEY_DOWN, &ObjectSelectorSystem::onKeyDown );
     
    m_world->Listener(this)
        .Off( ecs::WorldEventType::WORLD_EDITOR_UPDATE, &ObjectSelectorSystem::onUpdate )
        .Off( ecs::WorldEventType::WORLD_EDITOR_UPDATE, &ObjectSelectorSystem::onMouseUpdate )
        .Off( ecs::WorldEventType::WORLD_ENTITY_COMPONENT_ADDED, &ObjectSelectorSystem::onSelectedAdd );

    m_zAxis->Delete( );
    m_xAxis->Delete( );
    m_yAxis->Delete( );
}

void ObjectSelectorSystem::Process(ecs::Entity *entity)
{
    if (!(m_keyboardManager->GetModifiers( ) & KMD_ALT) &&
        GetCoreSystem(MouseManager)->IsButtonDown( MBTN_LEFT ) &&
        m_dragging)
    {
        // zero out all selected rigidbodies' velocity and angular velocity
        auto rigidbody = entity->GetComponent<ecs::Rigidbody>( );

		rigidbody->SetVelocity( SVec3::Zero( ) );
		rigidbody->SetAngularVelocity( SVec3::Zero( ) );
		rigidbody->SetAwake( );
    }
}

/////////////////////////////////////////////////////////////////////
// EVENTS ///////////////////////////////////////////////////////////
void ObjectSelectorSystem::onMouseDown(EVENT_HANDLER(MouseManager))
{
    EVENT_ATTRS(MouseManager, MouseButtonArgs);

    // can't click without mouse focus
    if (!m_editorCameraSystem->HasMouseFocus( ))
        return;

    if (m_keyboardManager->GetModifiers( ) & KMD_ALT)
        return;

    if (args->button == MBTN_LEFT)
    {
        //update out bases
        updateBases( );

        //get the current ID from graphics
        auto newID = m_graphics->GetMousedOverID( );

        //make sure it's a valid id
        if (newID == -1)
        {
            hideTool( );
            unpickObject( m_currentID );
            m_currentID = -1;

            return; 
        }

        //check to see if we are selecting any of our axis
        if (newID == m_xAxis->GetUniqueID( ))
        {
            m_dragging = true;
            m_axis = 0;

            m_baseMousePos = getMousePosition( args->position );

            return;
        }
        else if (newID == m_yAxis->GetUniqueID( ))
        {
            m_dragging = true;
            m_axis = 1;

            m_baseMousePos = getMousePosition( args->position );

            return;
        }
        else if (newID == m_zAxis->GetUniqueID( ))
        {
            m_dragging = true;
            m_axis = 2;

            m_baseMousePos = getMousePosition( args->position );

            return;
        }
        else
        {
            m_axis = -1;
            m_dragging = false;
        }

        //if we are currently dragging, don't try to pick other objects
        if (m_dragging)
            return;

        //check to see if it's different than the current one
        if (newID != m_currentID)
        {
            //remove all selected from the currently saved IDs
            unpickObject( m_currentID );

            //grab the new object, add the component
            auto obj = m_world->GetEntityUnique( newID );

            //if it existed and it was selected, unselect
            if (obj != nullptr && obj->GetComponent<Selected>( ) == nullptr)
                obj->AddComponent<Selected>( );
        }
    }
    else if (args->button == MBTN_MIDDLE)
    {
        //update out bases
        updateBases( );
        m_baseMousePos = getMousePosition( args->position );
        m_dragging = true;
    }
}

void ObjectSelectorSystem::onMouseUpdate(EVENT_HANDLER(ursine::ecs::World))
{
    // can't move mouse on focus
    if (!m_editorCameraSystem->HasFocus( ))
        return;

    auto *cameraEntity = m_editorCameraSystem->GetEditorCameraEntity( );

    SVec3 worldMousePos = cameraEntity->GetComponent<ecs::Camera>()->GetMouseWorldPosition();

    //some switch for detecting tool type
    if (!(m_keyboardManager->GetModifiers( ) & KMD_ALT))
    {
        //get the editor camera
        ecs::Camera *cam = m_editorCameraSystem->GetEditorCamera( );

        // we need to calculate mouse position w/ respect to the current window
        float width, height;
		auto dimensions = cam->GetViewportSize( );
		width = dimensions.X( );
		height = dimensions.Y( );
        
        //get the mouse position
        Vec2 screenPos = GetCoreSystem(MouseManager)->GetPosition( );

        SVec3 camPos = cam->GetOwner( )->GetTransform( )->GetWorldPosition( );

        //get the mouse world positions
        SVec3 p1 = cam->ScreenToWorld( screenPos, 0.1f );
        SVec3 p2 = cam->ScreenToWorld( screenPos, 1.f );

        //create a vector going out from the eye
        SVec3 mouseVec = p1 - p2;

        //project onto the CURRENT place, which is dependent on the base position
        //x axis, we will treat z as stationary
        //z value will solve to the current z of the position
        //need t value
        //project onto all 3 planes, calculate an x, y, and z

        float timeX = (m_baseTranslation.Z( ) - camPos.Z( )) / mouseVec.Z( );
        float timeY = (m_baseTranslation.Z( ) - camPos.Z( )) / mouseVec.Z( );
        float timeZ = (m_baseTranslation.X( ) - camPos.X( )) / mouseVec.X( );

        float x = camPos.X( ) + timeX * mouseVec.X( );
        float y = camPos.Y( ) + timeY * mouseVec.Y( );
        float z = camPos.Z( ) + timeZ * mouseVec.Z( );

        //we need to take into account that the current point of the transform may not be 0, 0, 0
        if (m_dragging && m_currentID != -1)
        {
            switch (m_currentTool)
            {
                case TOOL_TRANSLATION:
                    updateTranslation( SVec3( x, y, z ) );
                    break;
                case TOOL_SCALE:
                    updateScale( SVec3( x, y, z ) );
                    break;
                case TOOL_ROTATION:
                    updateRotation( SVec3( x, y, z ) );
                    break;
                default:
                    break;
            }
        }
    }
}

void ObjectSelectorSystem::onMouseUp(EVENT_HANDLER(MouseManager))
{
    m_dragging = false;
}

void ObjectSelectorSystem::onKeyDown(EVENT_HANDLER(KeyboardManager))
{
    EVENT_ATTRS(KeyboardManager, KeyboardKeyArgs);

    // must have focus or mouse focus
    if (!(m_editorCameraSystem->HasFocus( ) || m_editorCameraSystem->HasMouseFocus( )))
        return;

    if (args->key == KEY_1)
    {
        m_dragging = false;
        m_currentTool = TOOL_TRANSLATION;
        setToTranslate( );
    }
    else if (args->key == KEY_2)
    {
        m_dragging = false;
        m_currentTool = TOOL_SCALE;
        setToScale( );
    }
    else if (args->key == KEY_3)
    {
        m_dragging = false;
        m_currentTool = TOOL_ROTATION;
        setToRotation( );
    }
}

void ObjectSelectorSystem::onUpdate(EVENT_HANDLER(ecs::World))
{
    if (m_currentID == -1)
        return;

    moveToolToEntity( m_currentID );

    // can't update without focus
    if (!m_editorCameraSystem->HasFocus( ))
        return;

    float zoom = m_editorCameraSystem->GetCamZoom( );

    auto obj = m_world->GetEntityUnique( m_currentID );

    //figure out some color stuff
    Color xAx = Color( 1, 0, 0, 1 );
    Color yAx = Color( 0, 1, 0, 1 );
    Color zAx = Color( 0, 0, 1, 1 );

    if(m_currentTool == TOOL_ROTATION )
    {
        xAx = yAx;
        yAx = Color( 1, 0, 0, 1 );
    }

    //change color to yellow if dragging
    if (m_dragging && !(m_keyboardManager->GetModifiers( ) & KMD_ALT))
    {
        switch (m_axis)
        {
            case 0:
                xAx = Color( 1, 1, 0, 1 );
                break;
            case 1:
                yAx = Color( 1, 1, 0, 1 );
                break;
            case 2:
                zAx = Color( 1, 1, 0, 1 );
                break;
            default:
                break;
        }
    }

    //set color in models
    auto xModel = m_xAxis->GetComponent<ecs::Model3D>( );
    auto yModel = m_yAxis->GetComponent<ecs::Model3D>( );
    auto zModel = m_zAxis->GetComponent<ecs::Model3D>( );

    xModel->SetColor( xAx );
    yModel->SetColor( yAx );
    zModel->SetColor( zAx );
}

void ObjectSelectorSystem::onSelectedAdd(EVENT_HANDLER(ecs::World))
{
    EVENT_ATTRS(ecs::World, ecs::ComponentEventArgs);

    auto comp = args->component;

    //if the object added was a selected component
    if (comp->Is<Selected>( ))
    {
        m_currentID = comp->GetOwner( )->GetUniqueID( );

        //select the new object
        pickObject( m_currentID );
    }
}

/////////////////////////////////////////////////////////////////////
// UTILITIES ////////////////////////////////////////////////////////

void ObjectSelectorSystem::updateToolPosition(Vec3 pos)
{
    float zoom = m_editorCameraSystem->GetCamZoom( );
    float scalar = 1;

    ecs::Entity *obj = nullptr;
    
    if( m_currentID != -1 )
        obj = m_world->GetEntityUnique( m_currentID );

    //get their transforms, set data
    auto xTransf = m_xAxis->GetTransform( );
    auto yTransf = m_yAxis->GetTransform( );
    auto zTransf = m_zAxis->GetTransform( );
    {
        switch (m_currentTool)
        {
            case TOOL_TRANSLATION:
                scalar = zoom / 90;
                xTransf->SetWorldScale( SVec3( scalar, zoom / 10.f, scalar ) );
                yTransf->SetWorldScale( SVec3( scalar, zoom / 10.f, scalar ) );
                zTransf->SetWorldScale( SVec3( scalar, zoom / 10.f, scalar ) );

                xTransf->SetWorldPosition( SVec3( zoom / 20, 0, 0 ) + pos );
                yTransf->SetWorldPosition( SVec3( 0, zoom / 20, 0 ) + pos );
                zTransf->SetWorldPosition( SVec3( 0, 0, zoom / 20 ) + pos );
                
                //rotation
                xTransf->SetWorldRotation( SQuat( 90, SVec3( 0, 0, 1 ) ) );
                yTransf->SetWorldRotation( SQuat( 0, SVec3( 0, 0, 1 ) ) );
                zTransf->SetWorldRotation( SQuat( 90, SVec3( 1, 0, 0 ) ) );
                break;
            case TOOL_SCALE:
                scalar = zoom / 40;
                xTransf->SetWorldScale( SVec3( scalar, scalar, scalar ) );
                yTransf->SetWorldScale( SVec3( scalar, scalar, scalar ) );
                zTransf->SetWorldScale( SVec3( scalar, scalar, scalar ) );

                xTransf->SetWorldPosition( SVec3( scalar * 3, 0, 0 ) + pos );
                yTransf->SetWorldPosition( SVec3( 0, scalar * 3, 0 ) + pos );
                zTransf->SetWorldPosition( SVec3( 0, 0, scalar * 3 ) + pos );

                //rotation
                xTransf->SetWorldRotation( SQuat( 90, SVec3( 0, 0, 1 ) ) );
                yTransf->SetWorldRotation( SQuat( 0, SVec3( 0, 0, 1 ) ) );
                zTransf->SetWorldRotation( SQuat( 90, SVec3( 1, 0, 0 ) ) );
                break;
            case TOOL_ROTATION:
                scalar = zoom / 5;
                xTransf->SetWorldScale( SVec3( scalar, scalar, scalar ) );
                yTransf->SetWorldScale( SVec3( scalar, scalar, scalar ) );
                zTransf->SetWorldScale( SVec3( scalar, scalar, scalar ) );

                // we need to calculate stuff to rotate our rotation tool properly

                // default rotation if we don't have an object
                if ( obj == nullptr )
                {
                    xTransf->SetWorldRotation( SQuat( 0, SVec3( 1, 0, 0 ) ) );
                    yTransf->SetWorldRotation( SQuat( 90, SVec3( 0, 0, 2 ) ) );
                    zTransf->SetWorldRotation( SQuat( 90, SVec3( 1, 0, 0 ) ) );
                }
                else
                {
                    auto *entityTransform = obj->GetTransform( );

                    // calculate the rotations
                    auto up = SVec3( 0, 1, 0 );
                    auto right = SVec3( 1, 0, 0 );
                    auto forward = SVec3( 0, 0, 1 );

                    auto entityUp = entityTransform->GetUp( );
                    auto entityRight = entityTransform->GetRight( );
                    auto entityForward = entityTransform->GetForward( );

                    SQuat upRot, rightRot, forwardRot;
                    upRot.SetFromTo( up, entityUp );
                    rightRot.SetFromTo( right, entityRight );
                    forwardRot.SetFromTo( forward, entityForward );

                    xTransf->SetWorldRotation( upRot );
                    yTransf->SetWorldRotation( rightRot * SQuat( 90, SVec3( 0, 0, 2 ) ) );
                    zTransf->SetWorldRotation( forwardRot * SQuat( 90, SVec3( 1, 0, 0 ) ) );
                }

                xTransf->SetWorldPosition( pos );
                yTransf->SetWorldPosition( pos );
                zTransf->SetWorldPosition( pos );
                break;
            default:
                break;
        }
    }
}

void ObjectSelectorSystem::moveToolToEntity(const ecs::EntityUniqueID id)
{
    auto newObj = m_world->GetEntityUnique( id );
    
    if (!newObj)
        return;

    updateToolPosition( newObj->GetTransform( )->GetWorldPosition( ) );
}

/////////////////////////////////////////////////////////////////////
// PICKING //////////////////////////////////////////////////////////

void ObjectSelectorSystem::pickObject(const ecs::EntityUniqueID id)
{
    auto obj = m_world->GetEntityUnique( id );

    auto transf = obj->GetTransform( );

    m_baseTranslation = transf->GetWorldPosition( );
    m_baseScale = transf->GetWorldScale( );
    m_baseRotation = transf->GetWorldRotation( );

    //save position
    moveToolToEntity( id );
}

void ObjectSelectorSystem::unpickObject(const ecs::EntityUniqueID id)
{
    auto obj = m_world->GetEntityUnique( id );

    //if it existed and it was selected, unselect
    if (obj != nullptr && obj->GetComponent<Selected>( ) != nullptr)
        obj->RemoveComponent<Selected>( );
}

/////////////////////////////////////////////////////////////////////
// OBJECT TRANSFORMATION / TOOLS ////////////////////////////////////

// TRANSLATION ////////////////////////
void ObjectSelectorSystem::setToTranslate()
{
    //get their models
    auto xModel = m_xAxis->GetComponent<ecs::Model3D>( );
    auto yModel = m_yAxis->GetComponent<ecs::Model3D>( );
    auto zModel = m_zAxis->GetComponent<ecs::Model3D>( );

    {
        xModel->SetModelResourceName( "Cylinder" );
        yModel->SetModelResourceName( "Cylinder" );
        zModel->SetModelResourceName( "Cylinder" );

        xModel->SetMaterial( "Blank" );
        yModel->SetMaterial( "Blank" );
        zModel->SetMaterial( "Blank" );

        xModel->SetMaterialData( 8, 0, 0 );
        yModel->SetMaterialData( 8, 0, 0 );
        zModel->SetMaterialData( 8, 0, 0 );

        xModel->SetColor( Color( 1, 0, 0, 1 ) );
        zModel->SetColor( Color( 0, 0, 1, 1 ) );
        yModel->SetColor( Color( 0, 1, 0, 1 ) );

        xModel->SetOverdraw( true );
        yModel->SetOverdraw( true );
        zModel->SetOverdraw( true );

        if (m_currentID != -1)
            moveToolToEntity( m_currentID );
    }
}

void ObjectSelectorSystem::updateTranslation(const SVec3 &mousePos)
{
    auto transf = m_world->GetEntityUnique( m_currentID )->GetTransform( );
    Vec3 pos = m_baseTranslation;

    Vec3 total = mousePos - m_baseMousePos;

    switch (m_axis)
    {
        case 0:
            pos.SetX( mousePos.X( ) - m_baseMousePos.X( ) + m_baseTranslation.X( ) );
            break;
        case 1:
            pos.SetY( mousePos.Y( ) - m_baseMousePos.Y( ) + m_baseTranslation.Y( ) );
            break;
        case 2:
            pos.SetZ( mousePos.Z( ) - m_baseMousePos.Z( ) + m_baseTranslation.Z( ) );
            break;
    }

    transf->SetWorldPosition( pos );
    updateToolPosition( pos );
}

// SCALE //////////////////////////////
void ObjectSelectorSystem::setToScale()
{
    //get their models
    auto xModel = m_xAxis->GetComponent<ecs::Model3D>( );
    auto yModel = m_yAxis->GetComponent<ecs::Model3D>( );
    auto zModel = m_zAxis->GetComponent<ecs::Model3D>( );

    {
        xModel->SetModelResourceName( "Cube" );
        yModel->SetModelResourceName( "Cube" );
        zModel->SetModelResourceName( "Cube" );

        xModel->SetMaterial( "Blank" );
        yModel->SetMaterial( "Blank" );
        zModel->SetMaterial( "Blank" );

        xModel->SetMaterialData( 8, 0, 0 );
        yModel->SetMaterialData( 8, 0, 0 );
        zModel->SetMaterialData( 8, 0, 0 );

        xModel->SetColor( Color( 1, 0, 0, 1 ) );
        zModel->SetColor( Color( 0, 0, 1, 1 ) );
        yModel->SetColor( Color( 0, 1, 0, 1 ) );

        xModel->SetOverdraw( true );
        yModel->SetOverdraw( true );
        zModel->SetOverdraw( true );

        if (m_currentID != -1)
            moveToolToEntity( m_currentID );
    }
}

void ObjectSelectorSystem::updateScale(const SVec3 &mousePos)
{
    auto transf = m_world->GetEntityUnique( m_currentID )->GetTransform( );
    Vec3 scale = transf->GetWorldScale( );

    switch (m_axis)
    {
        case 0:
            scale.SetX( mousePos.X( ) - m_baseMousePos.X( ) + m_baseScale.X( ) );
            break;
        case 1:
            scale.SetY( mousePos.Y( ) - m_baseMousePos.Y( ) + m_baseScale.Y( ) );
            break;
        case 2:
            scale.SetZ( mousePos.Z( ) - m_baseMousePos.Z( ) + m_baseScale.Z( ) );
            break;
    }


    transf->SetWorldScale( scale );
}

// ROTATION ///////////////////////////
void ObjectSelectorSystem::setToRotation()
{
    //get their models
    auto xModel = m_xAxis->GetComponent<ecs::Model3D>( );
    auto yModel = m_yAxis->GetComponent<ecs::Model3D>( );
    auto zModel = m_zAxis->GetComponent<ecs::Model3D>( );

    {
        xModel->SetModelResourceName( "Ring" );
        yModel->SetModelResourceName( "Ring" );
        zModel->SetModelResourceName( "Ring" );

        xModel->SetMaterial( "Blank" );
        yModel->SetMaterial( "Blank" );
        zModel->SetMaterial( "Blank" );

        xModel->SetMaterialData( 8, 0, 0 );
        yModel->SetMaterialData( 8, 0, 0 );
        zModel->SetMaterialData( 8, 0, 0 );

        xModel->SetColor( Color( 1, 0, 0, 1 ) );
        zModel->SetColor( Color( 0, 0, 1, 1 ) );
        yModel->SetColor( Color( 0, 1, 0, 1 ) );

        xModel->SetOverdraw( true );
        yModel->SetOverdraw( true );
        zModel->SetOverdraw( true );

        if (m_currentID != -1)
            moveToolToEntity( m_currentID );
    }
}

void ObjectSelectorSystem::updateRotation(const SVec3 &mousePos)
{
    auto transf = m_world->GetEntityUnique( m_currentID )->GetTransform( );
    float difference;

    // get camera position
    auto *cameraEntity = m_editorCameraSystem->GetEditorCameraEntity( );
    auto *camera = cameraEntity->GetComponent<ecs::Camera>( );
    auto cameraPos = cameraEntity->GetTransform( )->GetWorldPosition( );

    float sign = 0;

    //if x, lock to the x/y of the obj. get vector from xyz of obj.
    switch (m_axis)
    {
        case 0:
            difference = mousePos.X( ) - m_baseMousePos.X( );
            sign = camera->GetLook( ).Dot( transf->GetRight( ) ) < 0 ? -1.f : 1.f;

            transf->SetLocalRotation( transf->GetLocalRotation( ) * SQuat( 0, -difference * sign, 0 ) );

            break;
        case 1:
            difference = mousePos.Z( ) - m_baseMousePos.Z( );
            sign = camera->GetLook( ).Dot( transf->GetForward( ) ) < 0 ? -1.f : 1.f;
            transf->SetLocalRotation( transf->GetLocalRotation( ) * SQuat( -difference * sign, 0, 0 ) );
            break;
        case 2:
            difference = mousePos.Y( ) - m_baseMousePos.Y( );
            sign = camera->GetLook( ).Dot( transf->GetUp( ) ) < 0 ? -1.f : 1.f;
            transf->SetLocalRotation( transf->GetLocalRotation( ) * SQuat( 0, 0, -difference * sign ) );
            break;
    }

    m_baseMousePos = mousePos;

    updateBases( );

    //so we need to somehow lock onto the plane that we want to rotate on
    //in the case of X, we will want to rotate on the Y/Z Axis
    //however, to get the angle we need to rotate on the plane that the ring exists on (XZ)
    //
}

/////////////////////////////////////////////////////////////////////
// MISC METHODS /////////////////////////////////////////////////////
void ObjectSelectorSystem::hideTool(void)
{
    m_dragging = false;
    m_axis = -1;

    m_baseTranslation = SVec3( 2000, 2000, 2000 );
    updateToolPosition( m_baseTranslation );
}

SVec3 ObjectSelectorSystem::getMousePosition(const Vec2 &mousePos)
{
    //get the editor camera
    ecs::Camera *cam = m_editorCameraSystem->GetEditorCamera( );

    //get the mouse position
    Vec2 screenPos = mousePos;

    Vec3 camPos = cam->GetOwner( )->GetTransform( )->GetWorldPosition( );

    //get the mouse world positions
    Vec3 p1 = cam->ScreenToWorld( screenPos, 0.1f );
    Vec3 p2 = cam->ScreenToWorld( screenPos, 1.f );

    //create a vector going out from the eye
    Vec3 mouseVec = p1 - p2;
    mouseVec.Set( mouseVec.X( ), mouseVec.Y( ), mouseVec.Z( ) );

    //project onto the CURRENT place, which is dependent on the base position
    //x axis, we will treat z as stationary
    //z value will solve to the current z of the position
    //need t value
    //project onto all 3 planes, calculate an x, y, and z

    float timeX = (m_baseTranslation.Z( ) - camPos.Z( )) / mouseVec.Z( );
    float timeY = (m_baseTranslation.X( ) - camPos.X( )) / mouseVec.X( );
    float timeZ = (m_baseTranslation.X( ) - camPos.X( )) / mouseVec.X( );

    float x = camPos.X( ) + timeX * mouseVec.X( );
    float y = camPos.Y( ) + timeY * mouseVec.Y( );
    float z = camPos.Z( ) + timeZ * mouseVec.Z( );

    return SVec3( x, y, z );
}

void ObjectSelectorSystem::updateBases(void)
{
    if (m_currentID == -1)
        return;

    auto obj = m_world->GetEntityUnique( m_currentID );

    URSINE_TODO( "Multi select, handle unselected objects" );
    if (!obj)
    {
        m_currentID = -1;

        return;
    }

    m_baseTranslation = obj->GetTransform( )->GetWorldPosition( );
    m_baseScale = obj->GetTransform( )->GetWorldScale( );
    m_baseRotation = obj->GetTransform( )->GetWorldRotation( );
}

ursine::SVec3 ObjectSelectorSystem::LinePlaneIntersection(
    const ursine::SVec3 &pointOnPlane,
    const ursine::SVec3& axis1,
    const ursine::SVec3& axis2,
    const ursine::SVec3& vector,
    const ursine::SVec3& vectorStart
    )
{
    // we'll need a normal to this plane
    SVec3 planeNormal = SVec3::Cross( axis1, axis2 );
    planeNormal.Normalize( );

    // if invalid, return infinity
    if ( planeNormal.Dot( vector ) == 0 )
        return SVec3( 0, 0, 0 );

    // plane equation
    // (some given point - point on plane) . normal = 0 IF it is on the plane

    // line equation
    // newPoint = oldPoint + scalar * vector;
    float scalar = ((vectorStart - pointOnPlane).Dot( planeNormal )) / (vector.Dot( planeNormal ));

    return vectorStart + vector * scalar;
}

ursine::SVec3 ObjectSelectorSystem::ProjectPointToLine(
    const ursine::SVec3& linePoint1,
    const ursine::SVec3& linePoint2,
    const ursine::SVec3& point
    )
{
    SVec3 vec1 = point - linePoint1;
    SVec3 vec2 = linePoint2 - linePoint1;

    return linePoint1 + (vec1.Dot( vec2 ) / vec2.Dot( vec2 )) * vec2;
}