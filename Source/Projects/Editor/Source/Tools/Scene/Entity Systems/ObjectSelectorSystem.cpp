#include "Precompiled.h"

#include "ObjectSelectorSystem.h"
#include "SelectedComponent.h"

#include "EditorCameraSystem.h"

#include <Vec2.h>
#include <Vec3.h>

#include <RenderableComponent.h>
#include <Model3DComponent.h>

#include <SystemManager.h>
#include <GfxAPI.h>
#include <WorldEvent.h>

using namespace ursine;

ENTITY_SYSTEM_DEFINITION( ObjectSelectorSystem );

ObjectSelectorSystem::ObjectSelectorSystem(ecs::World *world)
    : EntitySystem( world )
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

ecs::Entity *ObjectSelectorSystem::GetCurrentFocus()
{
    return m_world->GetEntityUnique( m_currentID );
}

void ObjectSelectorSystem::OnInitialize()
{
    //grab graphics
    m_graphics = GetCoreSystem( graphics::GfxAPI );
    m_mouseManager = GetCoreSystem( MouseManager );
    m_keyboardManager = GetCoreSystem( KeyboardManager );

    m_editorCameraSystem = m_world->GetEntitySystem( EditorCameraSystem );

    //connect to the mouse events
    m_mouseManager->Listener( this )
        .On( MM_BUTTON_DOWN, &ObjectSelectorSystem::onMouseDown )
        .On( MM_MOVE, &ObjectSelectorSystem::onMouseMove )
        .On( MM_BUTTON_UP, &ObjectSelectorSystem::onMouseUp )
        .On( MM_SCROLL, &ObjectSelectorSystem::onMouseScroll );

    m_keyboardManager->Listener( this )
        .On( KM_KEY_DOWN, &ObjectSelectorSystem::onKeyDown );

    m_world->Listener( this )
        .On( ecs::WorldEventType::WORLD_UPDATE, &ObjectSelectorSystem::onUpdate )
        .On( ecs::WorldEventType::WORLD_ENTITY_COMPONENT_ADDED, &ObjectSelectorSystem::onSelectedAdd );

    //construct the 3 axis
    m_xAxis = m_world->CreateEntity( );
    m_xAxis->SetVisibleInEditor( false );

    m_yAxis = m_world->CreateEntity( );
    m_yAxis->SetVisibleInEditor( false );

    m_zAxis = m_world->CreateEntity( );
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
    m_xAxis->AddComponent<ecs::Renderable>( );
    m_yAxis->AddComponent<ecs::Renderable>( );
    m_zAxis->AddComponent<ecs::Renderable>( );

    m_xAxis->AddComponent<ecs::Model3D>( );
    m_yAxis->AddComponent<ecs::Model3D>( );
    m_zAxis->AddComponent<ecs::Model3D>( );

    //get their models
    auto xHand = m_xAxis->GetComponent<ecs::Renderable>( )->GetHandle( );
    auto yHand = m_yAxis->GetComponent<ecs::Renderable>( )->GetHandle( );
    auto zHand = m_zAxis->GetComponent<ecs::Renderable>( )->GetHandle( );

    auto &xModel = m_graphics->RenderableMgr.GetModel3D( xHand );
    auto &yModel = m_graphics->RenderableMgr.GetModel3D( yHand );
    auto &zModel = m_graphics->RenderableMgr.GetModel3D( zHand );
    {
        xModel.SetModel( "Cylinder" );
        yModel.SetModel( "Cylinder" );
        zModel.SetModel( "Cylinder" );

        xModel.SetMaterial( "Blank" );
        yModel.SetMaterial( "Blank" );
        zModel.SetMaterial( "Blank" );

        xModel.SetColor( Color( 1, 0, 0, 1 ) );
        yModel.SetColor( Color( 0, 1, 0, 1 ) );
        zModel.SetColor( Color( 0, 0, 1, 1 ) );

        xModel.SetMaterialData( 8, 0, 0 );
        yModel.SetMaterialData( 8, 0, 0 );
        zModel.SetMaterialData( 8, 0, 0 );

        xModel.SetOverdraw( true );
        yModel.SetOverdraw( true );
        zModel.SetOverdraw( true );
    }
}

void ObjectSelectorSystem::OnRemove()
{
    m_mouseManager->Listener( this ) 
        .Off( MM_BUTTON_DOWN, &ObjectSelectorSystem::onMouseDown )
        .Off( MM_MOVE, &ObjectSelectorSystem::onMouseMove )
        .Off( MM_BUTTON_UP, &ObjectSelectorSystem::onMouseUp )
        .Off( MM_SCROLL, &ObjectSelectorSystem::onMouseScroll );

    m_keyboardManager->Listener( this )
        .Off( KM_KEY_DOWN, &ObjectSelectorSystem::onKeyDown );
     
    m_world->Listener(this)
        .Off(ecs::WorldEventType::WORLD_UPDATE, &ObjectSelectorSystem::onUpdate)
        .Off(ecs::WorldEventType::WORLD_ENTITY_COMPONENT_ADDED, &ObjectSelectorSystem::onSelectedAdd);

    m_zAxis->Delete( );
    m_xAxis->Delete( );
    m_yAxis->Delete( );
}

// EVENTS ///////////////////////////////////////////////////////////
void ObjectSelectorSystem::onMouseDown(EVENT_HANDLER(MouseManager))
{
    EVENT_ATTRS(MouseManager, MouseButtonArgs);

    // can't click without focus
    if (!(m_editorCameraSystem->HasFocus( ) && m_editorCameraSystem->HasMouseFocus( )))
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

void ObjectSelectorSystem::onMouseMove(EVENT_HANDLER(MouseManager))
{
    EVENT_ATTRS(MouseManager, MouseMoveArgs);

    // can't move mouse on focus
    if (!m_editorCameraSystem->HasFocus( ))
        return;

    auto newID = m_graphics->GetMousedOverID( );

    //some switch for detecting tool type
    if (!(m_keyboardManager->GetModifiers( ) & KMD_ALT))
    {
        //get the editor camera
        graphics::Camera *cam = m_editorCameraSystem->GetEditorCamera( );

        //get the mouse position
        Vec2 screenPos = args->position;

        Vec3 camPos = cam->GetPosition( );

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

void ObjectSelectorSystem::onMouseScroll(EVENT_HANDLER(MouseManager))
{
    // we can scroll only if we have mouse focus
    if (!m_editorCameraSystem->HasMouseFocus( ))
        return;

    if (m_currentID != -1)
        moveToolToEntity( m_currentID );
}

void ObjectSelectorSystem::onKeyDown(EVENT_HANDLER(KeyboardManager))
{
    EVENT_ATTRS(KeyboardManager, KeyboardKeyArgs);

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
    // can't update without focus
    if (!m_editorCameraSystem->HasFocus( ))
        return;

    if (m_currentID == -1)
        return;

    float zoom = m_editorCameraSystem->GetCamZoom( );

    auto obj = m_world->GetEntityUnique( m_currentID );

    Vec3 pos = obj->GetTransform( )->GetWorldPosition( );

    //figure out some color stuff
    Color xAx = Color( 1, 0, 0, 1 );
    Color yAx = Color( 0, 1, 0, 1 );
    Color zAx = Color( 0, 0, 1, 1 );

    moveToolToEntity( m_currentID );

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
    auto xHand = m_xAxis->GetComponent<ecs::Renderable>( )->GetHandle( );
    auto yHand = m_yAxis->GetComponent<ecs::Renderable>( )->GetHandle( );
    auto zHand = m_zAxis->GetComponent<ecs::Renderable>( )->GetHandle( );

    auto &xModel = m_graphics->RenderableMgr.GetModel3D( xHand );
    auto &yModel = m_graphics->RenderableMgr.GetModel3D( yHand );
    auto &zModel = m_graphics->RenderableMgr.GetModel3D( zHand );

    xModel.SetColor( xAx );
    yModel.SetColor( yAx );
    zModel.SetColor( zAx );
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

// UTILITIES ////////////////////////////////////////////////////////

void ObjectSelectorSystem::calculateOffset(Vec2 mousePos)
{
    SVec3 mouse = getMousePosition( mousePos );
}

void ObjectSelectorSystem::updateToolPosition(Vec3 pos)
{
    float zoom = m_editorCameraSystem->GetCamZoom( );
    float scalar = 1;

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

                xTransf->SetWorldRotation( SQuat( 0, SVec3( 1, 0, 0 ) ) );
                yTransf->SetWorldRotation( SQuat( SQuat( 90, SVec3( 0, 0, 2 ) ) ) );
                zTransf->SetWorldRotation( SQuat( 90, SVec3( 1, 0, 0 ) ) );

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
    updateToolPosition( newObj->GetTransform( )->GetWorldPosition( ) );
}

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

// OBJECT TRANSFORMATION / TOOLS ////////////////////////////////////

void ObjectSelectorSystem::setToTranslate()
{
    //get their models
    auto xHand = m_xAxis->GetComponent<ecs::Renderable>( )->GetHandle( );
    auto yHand = m_yAxis->GetComponent<ecs::Renderable>( )->GetHandle( );
    auto zHand = m_zAxis->GetComponent<ecs::Renderable>( )->GetHandle( );

    auto &xModel = m_graphics->RenderableMgr.GetModel3D( xHand );
    auto &yModel = m_graphics->RenderableMgr.GetModel3D( yHand );
    auto &zModel = m_graphics->RenderableMgr.GetModel3D( zHand );

    {
        xModel.SetModel( "Cylinder" );
        yModel.SetModel( "Cylinder" );
        zModel.SetModel( "Cylinder" );

        xModel.SetMaterial( "Blank" );
        yModel.SetMaterial( "Blank" );
        zModel.SetMaterial( "Blank" );

        xModel.SetMaterialData( 8, 0, 0 );
        yModel.SetMaterialData( 8, 0, 0 );
        zModel.SetMaterialData( 8, 0, 0 );

        xModel.SetColor( Color( 1, 0, 0, 1 ) );
        zModel.SetColor( Color( 0, 0, 1, 1 ) );
        yModel.SetColor( Color( 0, 1, 0, 1 ) );

        xModel.SetOverdraw( true );
        yModel.SetOverdraw( true );
        zModel.SetOverdraw( true );

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

void ObjectSelectorSystem::setToScale()
{
    //get their models
    auto xHand = m_xAxis->GetComponent<ecs::Renderable>( )->GetHandle( );
    auto yHand = m_yAxis->GetComponent<ecs::Renderable>( )->GetHandle( );
    auto zHand = m_zAxis->GetComponent<ecs::Renderable>( )->GetHandle( );

    auto &xModel = m_graphics->RenderableMgr.GetModel3D( xHand );
    auto &yModel = m_graphics->RenderableMgr.GetModel3D( yHand );
    auto &zModel = m_graphics->RenderableMgr.GetModel3D( zHand );

    {
        xModel.SetModel( "Cube" );
        yModel.SetModel( "Cube" );
        zModel.SetModel( "Cube" );

        xModel.SetMaterial( "Blank" );
        yModel.SetMaterial( "Blank" );
        zModel.SetMaterial( "Blank" );

        xModel.SetMaterialData( 8, 0, 0 );
        yModel.SetMaterialData( 8, 0, 0 );
        zModel.SetMaterialData( 8, 0, 0 );

        xModel.SetColor( Color( 1, 0, 0, 1 ) );
        zModel.SetColor( Color( 0, 0, 1, 1 ) );
        yModel.SetColor( Color( 0, 1, 0, 1 ) );

        xModel.SetOverdraw( true );
        yModel.SetOverdraw( true );
        zModel.SetOverdraw( true );

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

void ObjectSelectorSystem::setToRotation()
{
    //get their models
    auto xHand = m_xAxis->GetComponent<ecs::Renderable>( )->GetHandle( );
    auto yHand = m_yAxis->GetComponent<ecs::Renderable>( )->GetHandle( );
    auto zHand = m_zAxis->GetComponent<ecs::Renderable>( )->GetHandle( );

    auto &xModel = m_graphics->RenderableMgr.GetModel3D( xHand );
    auto &yModel = m_graphics->RenderableMgr.GetModel3D( yHand );
    auto &zModel = m_graphics->RenderableMgr.GetModel3D( zHand );

    {
        xModel.SetModel( "Ring" );
        yModel.SetModel( "Ring" );
        zModel.SetModel( "Ring" );

        xModel.SetMaterial( "Blank" );
        yModel.SetMaterial( "Blank" );
        zModel.SetMaterial( "Blank" );

        xModel.SetMaterialData( 8, 0, 0 );
        yModel.SetMaterialData( 8, 0, 0 );
        zModel.SetMaterialData( 8, 0, 0 );

        xModel.SetColor( Color( 1, 0, 0, 1 ) );
        zModel.SetColor( Color( 0, 0, 1, 1 ) );
        yModel.SetColor( Color( 0, 1, 0, 1 ) );

        xModel.SetOverdraw( true );
        yModel.SetOverdraw( true );
        zModel.SetOverdraw( true );

        if (m_currentID != -1)
            moveToolToEntity( m_currentID );
    }
}

void ObjectSelectorSystem::updateRotation(const SVec3 &mousePos)
{
    //what axis are we trying to change? x, y, or z?
    SVec3 mouseLockedPos = m_baseMousePos;
    SVec3 mouseFreePos = mousePos;

    SVec3 mouseLockedVec;
    SVec3 mouseFreeVec;

    SQuat newRot;
    auto transf = m_world->GetEntityUnique( m_currentID )->GetTransform( );
    float difference;

    //if x, lock to the x/y of the obj. get vector from xyz of obj.
    switch (m_axis)
    {
        case 0:
            difference = mousePos.X( ) - m_baseMousePos.X( );

            transf->SetLocalRotation( transf->GetLocalRotation( ) * SQuat( 0, -difference, 0 ) );

            break;
        case 1:
            difference = mousePos.X( ) - m_baseMousePos.X( );

            transf->SetLocalRotation( transf->GetLocalRotation( ) * SQuat( -difference, 0, 0 ) );
            break;
        case 2:
            difference = mousePos.Y( ) - m_baseMousePos.Y( );

            transf->SetLocalRotation( transf->GetLocalRotation( ) * SQuat( 0, 0, -difference ) );
            break;
    }


    //so we need to somehow lock onto the plane that we want to rotate on
    //in the case of X, we will want to rotate on the Y/Z Axis
    //however, to get the angle we need to rotate on the plane that the ring exists on (XZ)
    //
}

void ObjectSelectorSystem::hideTool()
{
    m_dragging = false;
    m_axis = -1;

    m_baseTranslation = SVec3( 2000, 2000, 2000 );
    updateToolPosition( m_baseTranslation );
}

SVec3 ObjectSelectorSystem::getMousePosition(const Vec2 &mousePos)
{
    //get the editor camera
    graphics::Camera *cam = m_editorCameraSystem->GetEditorCamera( );

    //get the mouse position
    Vec2 screenPos = mousePos;

    Vec3 camPos = cam->GetPosition( );

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

void ObjectSelectorSystem::updateBases()
{
    if (m_currentID == -1)
        return;

    auto obj = m_world->GetEntityUnique( m_currentID );

    m_baseTranslation = obj->GetTransform( )->GetWorldPosition( );
    m_baseScale = obj->GetTransform( )->GetWorldScale( );
    m_baseRotation = obj->GetTransform( )->GetWorldRotation( );
}
