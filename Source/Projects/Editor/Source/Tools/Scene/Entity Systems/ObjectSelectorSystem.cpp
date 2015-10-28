#include "Precompiled.h"

#include "ObjectSelectorSystem.h"
#include "EditorCameraSystem.h"
#include "SelectedComponent.h"

#include <SystemManager.h>
#include <RenderableComponent.h>
#include <Model3DComponent.h>
#include <Vec2.h>
#include <Vec3.h>

using namespace ursine;

ENTITY_SYSTEM_DEFINITION( ObjectSelectorSystem );

ObjectSelectorSystem::ObjectSelectorSystem(ecs::World *world)
    : EntitySystem( world )
      , m_xAxis( nullptr )
      , m_yAxis( nullptr )
      , m_zAxis( nullptr )
      , m_currentID( -1 )
      , m_graphics( nullptr )
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
    //connect to the mouse events
    GetCoreSystem( MouseManager )->Listener( this )
        .On( MM_BUTTON_DOWN, &ObjectSelectorSystem::onMouseDown )
        .On( MM_MOVE, &ObjectSelectorSystem::onMouseMove )
        .On( MM_BUTTON_UP, &ObjectSelectorSystem::onMouseUp )
        .On( MM_SCROLL, &ObjectSelectorSystem::onMouseScroll );

    GetCoreSystem( KeyboardManager )->Listener( this )
        .On( KM_KEY_DOWN, &ObjectSelectorSystem::onKeyDown );

    m_world->Listener( this )
        .On( ecs::WorldEventType::WORLD_UPDATE, &ObjectSelectorSystem::onUpdate );

    //grab graphics
    m_graphics = GetCoreSystem( graphics::GfxAPI );

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

    // give them all renderables and models
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

void ObjectSelectorSystem::OnRemove(void)
{
    GetCoreSystem( MouseManager )->Listener( this )
        .Off( MM_BUTTON_DOWN, &ObjectSelectorSystem::onMouseDown )
        .Off( MM_MOVE, &ObjectSelectorSystem::onMouseMove )
        .Off( MM_BUTTON_UP, &ObjectSelectorSystem::onMouseUp )
        .Off( MM_SCROLL, &ObjectSelectorSystem::onMouseScroll );

    GetCoreSystem( KeyboardManager )->Listener( this )
        .Off( KM_KEY_DOWN, &ObjectSelectorSystem::onKeyDown );

    m_zAxis->Delete( );
    m_xAxis->Delete( );
    m_yAxis->Delete( );
}

// EVENTS ///////////////////////////////////////////////////////////
void ObjectSelectorSystem::onMouseDown(EVENT_HANDLER(MouseManager))
{
    EVENT_ATTRS(MouseManager, MouseButtonArgs);

    if (args->button == MBTN_LEFT)
    {
        //update out bases
        updateBases( );

        //get the current ID from graphics
        auto newID = m_graphics->GetMousedOverID( );

        //make sure it's a valid id
        if (newID == -1)
        {
            //if we were pressing alt just ignore this event
            if ((GetCoreSystem(KeyboardManager)->GetModifiers( ) & KMD_ALT))
                return;

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

        //if we are currently dragging, don't try to pick other objects
        if (m_dragging)
            return;

        //check to see if it's different than the current one
        if (newID != m_currentID)
        {
            //unpick old object
            unpickObject( m_currentID );

            //pick new object
            pickObject( newID );

            //update id
            m_currentID = newID;

            //save position
            moveToolToEntity( newID );
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

    auto mouseMgr = GetCoreSystem(MouseManager);
    auto newID = m_graphics->GetMousedOverID();

   

    //some switch for detecting tool type
    if (!(GetCoreSystem(KeyboardManager)->GetModifiers( ) & KMD_ALT))
    {
        //get the editor camera
        graphics::Camera *cam = m_world->GetEntitySystem(EditorCameraSystem)->GetEditorCamera( );

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
<<<<<<< HEAD
            case TOOL_TRANSLATION:
                updateTranslation(SVec3(x, y, z));
                break;
            case TOOL_SCALE:
                updateScale(SVec3(x, y, z));
                break;
            case TOOL_ROTATION:
                updateRotation(SVec3(x, y, z));
                break;
            default:
                break;
=======
                case TOOL_TRANSLATION:
                    updateTranslation( SVec3( x, y, z ) );
                    break;
                case TOOL_SCALE:
                    updateScale( SVec3( x, y, z ) );
                    break;
                case TOOL_ROTATION:
                    break;
                default:
                    break;
>>>>>>> refs/remotes/origin/editor-docking
            }
        }
    }
}

void ObjectSelectorSystem::onMouseUp(EVENT_HANDLER(MouseManager))
{
    m_dragging = false;

    //save position
    if (m_currentID != -1)
    {
        auto newObj = m_world->GetEntityUnique( m_currentID );
    }
}

void ObjectSelectorSystem::onMouseScroll(EVENT_HANDLER(MouseManager))
{
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
    if (m_currentID == -1)
        return;

    float zoom = m_world->GetEntitySystem(EditorCameraSystem)->GetCamZoom( );
    auto obj = m_world->GetEntityUnique( m_currentID );

    Vec3 pos = obj->GetTransform( )->GetWorldPosition( );

    //figure out some color stuff
    Color xAx = Color( 1, 0, 0, 1 );
    Color yAx = Color( 0, 1, 0, 1 );
    Color zAx = Color( 0, 0, 1, 1 );

    //change color to yellow if dragging
    if (m_dragging && !(GetCoreSystem(KeyboardManager)->GetModifiers( ) & KMD_ALT))
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

    //if (m_currentTool == TOOL_TRANSLATION)
    //{
    //    //float length = zoom / 10;

    //    //float arrWidth = (0.3 / 2) * length;
    //    //float arrLength = length - arrWidth;

    //    ////render the lines
    //    //auto gfx = GetCoreSystem(graphics::GfxAPI);

    //    //gfx->DrawingMgr.SetOverdraw(true);

    //    ///////////////////////////////////////////////////////////////
    //    //gfx->DrawingMgr.SetColor(xAx);
    //    ////pole
    //    //gfx->DrawingMgr.DrawLine(pos, pos + SVec3(length, 0, 0));
    //    //
    //    ////arrows
    //    //gfx->DrawingMgr.DrawLine(pos + SVec3(arrLength, arrWidth  , 0         ), pos + SVec3(length, 0, 0));
    //    //gfx->DrawingMgr.DrawLine(pos + SVec3(arrLength, -arrWidth , 0         ), pos + SVec3(length, 0, 0));
    //    //gfx->DrawingMgr.DrawLine(pos + SVec3(arrLength, 0         , arrWidth  ), pos + SVec3(length, 0, 0));
    //    //gfx->DrawingMgr.DrawLine(pos + SVec3(arrLength, 0         , -arrWidth ), pos + SVec3(length, 0, 0));


    //    //gfx->DrawingMgr.SetColor(yAx);
    //    ////pole
    //    //gfx->DrawingMgr.DrawLine(pos, pos + SVec3(0, length, 0));

    //    ////arrows
    //    //gfx->DrawingMgr.DrawLine(pos + SVec3(arrWidth, arrLength, 0), pos + SVec3(0, length, 0));
    //    //gfx->DrawingMgr.DrawLine(pos + SVec3(-arrWidth, arrLength, 0), pos + SVec3(0, length, 0));
    //    //gfx->DrawingMgr.DrawLine(pos + SVec3(0, arrLength, arrWidth), pos + SVec3(0, length, 0));
    //    //gfx->DrawingMgr.DrawLine(pos + SVec3(0, arrLength, -arrWidth), pos + SVec3(0, length, 0));

    //    //gfx->DrawingMgr.SetColor(zAx);
    //    ////pole
    //    //gfx->DrawingMgr.DrawLine(pos, pos + SVec3(0, 0, length));

    //    ////arrows
    //    //gfx->DrawingMgr.DrawLine(pos + SVec3(arrWidth, 0, arrLength ), pos + SVec3(0, 0, length));
    //    //gfx->DrawingMgr.DrawLine(pos + SVec3(-arrWidth, 0, arrLength), pos + SVec3(0, 0, length));
    //    //gfx->DrawingMgr.DrawLine(pos + SVec3(0, arrWidth, arrLength), pos + SVec3(0, 0, length));
    //    //gfx->DrawingMgr.DrawLine(pos + SVec3(0, -arrWidth, arrLength), pos + SVec3(0, 0, length));

    //    //gfx->DrawingMgr.SetOverdraw(false);
    //}
    //else if(m_currentTool == TOOL_SCALE)
    //{
    //    
    //}
}

// UTILITIES ////////////////////////////////////////////////////////

void ObjectSelectorSystem::calculateOffset(Vec2 mousePos)
{
    SVec3 mouse = getMousePosition( mousePos );
}

void ObjectSelectorSystem::updateToolPosition(Vec3 pos)
{
    float zoom = m_world->GetEntitySystem(EditorCameraSystem)->GetCamZoom( );
    float scalar = 1;

    //get their transforms, set data
    auto xTransf = m_xAxis->GetTransform( );
    auto yTransf = m_yAxis->GetTransform( );
    auto zTransf = m_zAxis->GetTransform( );
    {
        switch (m_currentTool)
        {
        case TOOL_TRANSLATION:
<<<<<<< HEAD
            scalar = zoom / 90;
            xTransf->SetWorldScale(SVec3(scalar, zoom / 10.f, scalar));
            yTransf->SetWorldScale(SVec3(scalar, zoom / 10.f, scalar));
            zTransf->SetWorldScale(SVec3(scalar, zoom / 10.f, scalar));

            xTransf->SetWorldPosition(ursine::SVec3(zoom / 20, 0, 0) + pos);
            yTransf->SetWorldPosition(ursine::SVec3(0, zoom / 20, 0) + pos);
            zTransf->SetWorldPosition(ursine::SVec3(0, 0, zoom / 20) + pos);

            //rotation
            xTransf->SetWorldRotation(ursine::SQuat(90, ursine::SVec3(0, 0, 1)));
            yTransf->SetWorldRotation(ursine::SQuat(0, ursine::SVec3(0, 0, 1)));
            zTransf->SetWorldRotation(ursine::SQuat(90, ursine::SVec3(1, 0, 0)));
=======
            scalar = zoom / 80;
            xTransf->SetWorldScale( SVec3( scalar, zoom / 10.f, scalar ) );
            yTransf->SetWorldScale( SVec3( scalar, zoom / 10.f, scalar ) );
            zTransf->SetWorldScale( SVec3( scalar, zoom / 10.f, scalar ) );

            xTransf->SetWorldPosition( SVec3( zoom / 20, 0, 0 ) + pos );
            yTransf->SetWorldPosition( SVec3( 0, zoom / 20, 0 ) + pos );
            zTransf->SetWorldPosition( SVec3( 0, 0, zoom / 20 ) + pos );
>>>>>>> refs/remotes/origin/editor-docking
            break;
        case TOOL_SCALE:
            scalar = zoom / 40;
            xTransf->SetWorldScale( SVec3( scalar, scalar, scalar ) );
            yTransf->SetWorldScale( SVec3( scalar, scalar, scalar ) );
            zTransf->SetWorldScale( SVec3( scalar, scalar, scalar ) );

<<<<<<< HEAD
            xTransf->SetWorldPosition(ursine::SVec3(scalar * 3, 0, 0) + pos);
            yTransf->SetWorldPosition(ursine::SVec3(0, scalar * 3, 0) + pos);
            zTransf->SetWorldPosition(ursine::SVec3(0, 0, scalar * 3) + pos);

            //rotation
            xTransf->SetWorldRotation(ursine::SQuat(90, ursine::SVec3(0, 0, 1)));
            yTransf->SetWorldRotation(ursine::SQuat(0, ursine::SVec3(0, 0, 1)));
            zTransf->SetWorldRotation(ursine::SQuat(90, ursine::SVec3(1, 0, 0)));
=======
            xTransf->SetWorldPosition( SVec3( scalar * 3, 0, 0 ) + pos );
            yTransf->SetWorldPosition( SVec3( 0, scalar * 3, 0 ) + pos );
            zTransf->SetWorldPosition( SVec3( 0, 0, scalar * 3 ) + pos );
>>>>>>> refs/remotes/origin/editor-docking
            break;
        case TOOL_ROTATION:
            scalar = zoom / 5; 
            xTransf->SetWorldScale(SVec3(scalar, scalar, scalar));
            yTransf->SetWorldScale(SVec3(scalar, scalar, scalar));
            zTransf->SetWorldScale(SVec3(scalar, scalar, scalar));

            xTransf->SetWorldRotation(ursine::SQuat(0, ursine::SVec3(1, 0, 0)));
            yTransf->SetWorldRotation(ursine::SQuat(ursine::SQuat(90, ursine::SVec3(0, 0, 2))));
            zTransf->SetWorldRotation(ursine::SQuat(90, ursine::SVec3(1, 0, 0)));

            xTransf->SetWorldPosition(pos);
            yTransf->SetWorldPosition(pos);
            zTransf->SetWorldPosition(pos);
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

    // if it existed and it was selected, unselect
    if (obj != nullptr && !obj->HasComponent<Selected>( ))
        obj->AddComponent<Selected>( );

    m_baseTranslation = obj->GetTransform( )->GetWorldPosition( );
    m_baseScale = obj->GetTransform( )->GetWorldScale( );
}

void ObjectSelectorSystem::unpickObject(const ecs::EntityUniqueID id)
{
    auto obj = m_world->GetEntityUnique( id );

    //if it existed and it was selected, unselect
    if (obj != nullptr && obj->HasComponent<Selected>( ))
        obj->RemoveComponent<Selected>( );
}

// OBJECT TRANSFORMATION / TOOLS ////////////////////////////////////

void ObjectSelectorSystem::setToTranslate(void)
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

void ObjectSelectorSystem::setToScale(void)
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
<<<<<<< HEAD
    case 1: 
        scale.SetY(mousePos.Y( ) - m_baseMousePos.Y( ) + m_baseScale.Y( ));
=======
    case 1:
        scale.SetY( mousePos.Y( ) - m_baseMousePos.Y( ) + m_baseScale.Y( ) );
>>>>>>> refs/remotes/origin/editor-docking
        break;
    case 2:
        scale.SetZ( mousePos.Z( ) - m_baseMousePos.Z( ) + m_baseScale.Z( ) );
        break;
    }


    transf->SetWorldScale( scale );
}

void ObjectSelectorSystem::setToRotation(void)
{
<<<<<<< HEAD
    //get their models
    auto xHand = m_xAxis->GetComponent<ursine::ecs::Renderable>()->GetHandle();
    auto yHand = m_yAxis->GetComponent<ursine::ecs::Renderable>()->GetHandle();
    auto zHand = m_zAxis->GetComponent<ursine::ecs::Renderable>()->GetHandle();

    auto &xModel = m_graphics->RenderableMgr.GetModel3D(xHand);
    auto &yModel = m_graphics->RenderableMgr.GetModel3D(yHand);
    auto &zModel = m_graphics->RenderableMgr.GetModel3D(zHand);

    {

        xModel.SetModel("Ring");
        yModel.SetModel("Ring");
        zModel.SetModel("Ring");

        xModel.SetMaterial("Blank");
        yModel.SetMaterial("Blank");
        zModel.SetMaterial("Blank");

        xModel.SetMaterialData(8, 0, 0);
        yModel.SetMaterialData(8, 0, 0);
        zModel.SetMaterialData(8, 0, 0);

        xModel.SetColor(Color(1, 0, 0, 1));
        zModel.SetColor(Color(0, 0, 1, 1));
        yModel.SetColor(Color(0, 1, 0, 1));

        xModel.SetOverdraw(true);
        yModel.SetOverdraw(true);
        zModel.SetOverdraw(true);

        if (m_currentID != -1) moveToolToEntity(m_currentID);
    }
=======
    float zoom = m_world->GetEntitySystem(EditorCameraSystem)->GetCamZoom( );
>>>>>>> refs/remotes/origin/editor-docking
}

void ObjectSelectorSystem::updateRotation(const SVec3 &mousePos)
{
    //what axis are we trying to change? x, y, or z?
    SVec3 mouseLockedPos = m_baseMousePos;
    SVec3 mouseFreePos = mousePos;

    SVec3 mouseLockedVec;
    SVec3 mouseFreeVec;
  
    SQuat newRot;
    auto transf = m_world->GetEntityUnique(m_currentID)->GetComponent<ecs::Transform>();
    float difference;

    //if x, lock to the x/y of the obj. get vector from xyz of obj.
    switch (m_axis)
    {
    case 0:
        difference = mousePos.X() - m_baseMousePos.X();

        transf->SetLocalRotation(transf->GetLocalRotation() * SQuat(0, -difference, 0));
            
        break;
    case 1:
        difference = mousePos.X() - m_baseMousePos.X();

        transf->SetLocalRotation(transf->GetLocalRotation() * SQuat(-difference, 0, 0));
        break;
    case 2:    
        difference = mousePos.Y() - m_baseMousePos.Y();

        transf->SetLocalRotation(transf->GetLocalRotation() * SQuat(0, 0, -difference));
        break;
    }



    //so we need to somehow lock onto the plane that we want to rotate on
    //in the case of X, we will want to rotate on the Y/Z Axis
    //however, to get the angle we need to rotate on the plane that the ring exists on (XZ)
    //
}

void ObjectSelectorSystem::hideTool(void)
{
    m_dragging = false;
    m_axis = -1;

<<<<<<< HEAD
    m_baseTranslation = SVec3(2000, 2000, 2000);
    updateToolPosition(m_baseTranslation);  
}  
=======
    m_baseTranslation = SVec3( 2000, 2000, 2000 );
    updateToolPosition( m_baseTranslation );
}
>>>>>>> refs/remotes/origin/editor-docking

SVec3 ObjectSelectorSystem::getMousePosition(const Vec2 &mousePos)
{
    //get the editor camera
    graphics::Camera *cam = m_world->GetEntitySystem(EditorCameraSystem)->GetEditorCamera( );

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

void ObjectSelectorSystem::updateBases(void)
{
    if (m_currentID == -1)
        return;

    auto obj = m_world->GetEntityUnique( m_currentID );

<<<<<<< HEAD
    m_baseTranslation = obj->GetComponent<ecs::Transform>( )->GetWorldPosition( );
    m_baseScale = obj->GetComponent<ecs::Transform>( )->GetWorldScale( );
    m_baseRotation = obj->GetComponent<ecs::Transform>()->GetWorldRotation();
}
=======
    m_baseTranslation = obj->GetTransform( )->GetWorldPosition( );
    m_baseScale = obj->GetTransform( )->GetWorldScale( );
}
>>>>>>> refs/remotes/origin/editor-docking
