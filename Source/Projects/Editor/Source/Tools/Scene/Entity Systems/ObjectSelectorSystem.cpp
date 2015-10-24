#include "Precompiled.h"

#include "ObjectSelectorSystem.h"
#include <Game Engine/Scene/Component/Native Components/RenderableComponent.h>
#include <Game Engine/Scene/Component/Native Components/Model3DComponent.h>
#include <Tools/Scene/Components/SelectedComponent.h>
#include <Vec2.h>
#include <Vec3.h>
#include "EditorCameraSystem.h"
#include <SystemManager.h>
#include <Core/Graphics/API/GfxAPI.h>

using namespace ursine;

ENTITY_SYSTEM_DEFINITION(ObjectSelectorSystem);

ObjectSelectorSystem::ObjectSelectorSystem(ursine::ecs::World* world)
    : EntitySystem(world)
    , m_xAxis(nullptr)
    , m_yAxis(nullptr)
    , m_zAxis(nullptr)
    , m_currentID(-1)
    , m_graphics(nullptr)
    , m_dragging(false)
    , m_axis(-1)
    , m_basePos(Vec3(0, 0, 0))
    , m_translateOffset(0)
{
    
}

ursine::ecs::Entity* ObjectSelectorSystem::GetCurrentFocus()
{
    return m_world->GetEntityUnique(m_currentID);
}

void ObjectSelectorSystem::OnInitialize()
{
    //connect to the mouse events
    GetCoreSystem(ursine::MouseManager)->Listener(this)
        .On(MM_BUTTON_DOWN, &ObjectSelectorSystem::onMouseDown)
        .On(MM_MOVE, &ObjectSelectorSystem::onMouseMove)
        .On(MM_BUTTON_UP, &ObjectSelectorSystem::onMouseUp);

    //grab graphics
    m_graphics = GetCoreSystem(ursine::graphics::GfxAPI);

    //construct the 3 axis
    m_xAxis = m_world->CreateEntity();
    m_yAxis = m_world->CreateEntity();
    m_zAxis = m_world->CreateEntity();

    //get their transforms, set data
    auto xTransf = m_xAxis->GetComponent<ursine::ecs::Transform>();
    auto yTransf = m_yAxis->GetComponent<ursine::ecs::Transform>();
    auto zTransf = m_zAxis->GetComponent<ursine::ecs::Transform>();
    {
        //translation
        xTransf->SetWorldPosition(ursine::SVec3(2, 0, 0));
        yTransf->SetWorldPosition(ursine::SVec3(0, 2, 0));
        zTransf->SetWorldPosition(ursine::SVec3(0, 0, 2));

        //rotation
        xTransf->SetWorldRotation(ursine::SQuat(90, ursine::SVec3(0, 0, 1)));
        yTransf->SetWorldRotation(ursine::SQuat(0, ursine::SVec3(0, 0, 1)));
        zTransf->SetWorldRotation(ursine::SQuat(90, ursine::SVec3(1, 0, 0)));
    }

    //give them all renderables and models
    m_xAxis->AddComponent<ursine::ecs::Renderable>();
    m_yAxis->AddComponent<ursine::ecs::Renderable>();
    m_zAxis->AddComponent<ursine::ecs::Renderable>();

    m_xAxis->AddComponent<ursine::ecs::Model3D>();
    m_yAxis->AddComponent<ursine::ecs::Model3D>();
    m_zAxis->AddComponent<ursine::ecs::Model3D>();

    //get their models
    auto xHand = m_xAxis->GetComponent<ursine::ecs::Renderable>()->GetHandle();
    auto yHand = m_yAxis->GetComponent<ursine::ecs::Renderable>()->GetHandle();
    auto zHand = m_zAxis->GetComponent<ursine::ecs::Renderable>()->GetHandle();

    auto &xModel = m_graphics->RenderableMgr.GetModel3D(xHand);
    auto &yModel = m_graphics->RenderableMgr.GetModel3D(yHand);
    auto &zModel = m_graphics->RenderableMgr.GetModel3D(zHand);
    {

        xModel.SetModel("Cylinder");
        yModel.SetModel("Cylinder");
        zModel.SetModel("Cylinder");

        xModel.SetMaterial("Blank");
        yModel.SetMaterial("Blank");
        zModel.SetMaterial("Blank");

        xModel.SetColor( ursine::Color(1, 0, 0, 1));
        yModel.SetColor( ursine::Color(0, 1, 0, 1));
        zModel.SetColor( ursine::Color(0, 0, 1, 1));

        xModel.SetMaterialData(8, 0, 0);
        yModel.SetMaterialData(8, 0, 0);
        zModel.SetMaterialData(8, 0, 0);

        xModel.SetOverdraw(true);
        yModel.SetOverdraw(true);
        zModel.SetOverdraw(true);
    }
}

void ObjectSelectorSystem::OnRemove()
{
    GetCoreSystem(ursine::MouseManager)->Listener(this)
        .Off(MM_BUTTON_DOWN, &ObjectSelectorSystem::onMouseDown)
        .Off(MM_MOVE, &ObjectSelectorSystem::onMouseMove)
        .Off(MM_BUTTON_UP, &ObjectSelectorSystem::onMouseUp);
}

void ObjectSelectorSystem::onMouseDown(EVENT_HANDLER(ursine::MouseManager))
{
    EVENT_ATTRS(MouseManager, MouseButtonArgs);

    //get the current ID from graphics
    auto newID = m_graphics->GetMousedOverID();

    //make sure it's a valid id
    if (newID == -1)
        return;

    //make sure we aren't selecting one of our axis
    if (newID == m_xAxis->GetUniqueID())
    {
        m_dragging = true;
        m_axis = 0;

        calculateOffset(args->position);

        return;
    }
    else if (newID == m_yAxis->GetUniqueID())
    {
        m_dragging = true;
        m_axis = 1;
        calculateOffset(args->position);
        return;
    }
    else if (newID == m_zAxis->GetUniqueID())
    {
        m_dragging = true;
        m_axis = 2;
        calculateOffset(args->position);
        return;
    }
    else
    {
        m_dragging = false;
        m_axis = -1;
    }

    if (m_dragging)
        return;

    //check to see if it's different than the current one
    if(newID != m_currentID)
    {
        auto oldObj = m_world->GetEntityUnique(m_currentID);
        auto newObj = m_world->GetEntityUnique(newID);

        //since it's a new object, remove the selected component from the current object
        if(oldObj != nullptr && oldObj->GetComponent<Selected>() != nullptr) 
            oldObj->RemoveComponent<Selected>();

        //then, we should add the component to the new object
        if(newObj->GetComponent<Selected>() == nullptr) 
            newObj->AddComponent<Selected>();

        //update id
        m_currentID = newID;

        //save position
        m_basePos = newObj->GetComponent<ursine::ecs::Transform>()->GetWorldPosition();

        updateAxis(m_basePos);
    }
}

void ObjectSelectorSystem::onMouseMove(EVENT_HANDLER(ursine::MouseManager))
{
    EVENT_ATTRS(MouseManager, MouseMoveArgs);

    auto mouseMgr = GetCoreSystem(MouseManager);

    //some switch for detecting tool type
    if(true)
    {
        //get the editor camera
        graphics::Camera *cam = m_world->GetEntitySystem(EditorCameraSystem)->GetEditorCamera();

        //get the mouse position
        Vec2 screenPos = args->position;

        Vec3 camPos = cam->GetPosition();

        //get the mouse world positions
        Vec3 p1 = cam->ScreenToWorld(screenPos, 0.1f);
        Vec3 p2 = cam->ScreenToWorld(screenPos, 1.f);

        //create a vector going out from the eye
        Vec3 mouseVec = p1 - p2;
        mouseVec.Set(mouseVec.X(), mouseVec.Y(), mouseVec.Z());

        //project onto the CURRENT place, which is dependent on the base position
        //x axis, we will treat z as stationary
        //z value will solve to the current z of the position
        //need t value
        //project onto all 3 planes, calculate an x, y, and z

        float timeX = (m_basePos.Z() - camPos.Z()) / mouseVec.Z();
        float timeY = (m_basePos.Z() - camPos.Z()) / mouseVec.Z();
        float timeZ = (m_basePos.X() - camPos.X()) / mouseVec.X();

        float x = camPos.X() + timeX * mouseVec.X();
        float y = camPos.Y() + timeY * mouseVec.Y();
        float z = camPos.Z() + timeZ * mouseVec.Z();

        //we need to take into account that the current point of the transform may not be 0, 0, 0
        if(m_dragging && m_currentID != -1)
        {
            auto transf = m_world->GetEntityUnique(m_currentID)->GetComponent<ecs::Transform>();
            Vec3 pos = m_basePos;
            switch(m_axis)
            {
            case 0:
                pos.SetX(x);
                break;
            case 1:
                pos.SetY(y);
                break;
            case 2:
                pos.SetZ(z);
                break;
            }

            transf->SetWorldPosition(pos + m_translateOffset);
            updateAxis(pos + m_translateOffset);
        }

        
    }
}

void ObjectSelectorSystem::onMouseUp(EVENT_HANDLER(ursine::MouseManager))
{
    m_dragging = false;

    //save position
    if (m_currentID != -1)
    {
        auto newObj = m_world->GetEntityUnique(m_currentID);
        m_basePos = newObj->GetComponent<ursine::ecs::Transform>()->GetWorldPosition();
    }
}

void ObjectSelectorSystem::calculateOffset(ursine::Vec2 mousePos)
{
    //get the editor camera
    graphics::Camera *cam = m_world->GetEntitySystem(EditorCameraSystem)->GetEditorCamera();

    //get the mouse position
    Vec2 screenPos = mousePos;

    Vec3 camPos = cam->GetPosition();

    //get the mouse world positions
    Vec3 p1 = cam->ScreenToWorld(screenPos, 0.1f);
    Vec3 p2 = cam->ScreenToWorld(screenPos, 1.f);

    //create a vector going out from the eye
    Vec3 mouseVec = p1 - p2;
    mouseVec.Set(mouseVec.X(), mouseVec.Y(), mouseVec.Z());

    //project onto the CURRENT place, which is dependent on the base position
    //x axis, we will treat z as stationary
    //z value will solve to the current z of the position
    //need t value
    //project onto all 3 planes, calculate an x, y, and z

    float timeX = (m_basePos.Z() - camPos.Z()) / mouseVec.Z();
    float timeY = (m_basePos.Z() - camPos.Z()) / mouseVec.Z();
    float timeZ = (m_basePos.X() - camPos.X()) / mouseVec.X();

    float x = camPos.X() + timeX * mouseVec.X();
    float y = camPos.Y() + timeY * mouseVec.Y();
    float z = camPos.Z() + timeZ * mouseVec.Z();

    switch(m_axis)
    {
    case 0:
        m_translateOffset = m_basePos - Vec3(x, m_basePos.Y(), m_basePos.Z());
        break;
    case 1:
        m_translateOffset = m_basePos - Vec3(m_basePos.X(), y, m_basePos.Z());
        break;
    case 2:
        m_translateOffset = m_basePos - Vec3(m_basePos.X(), m_basePos.Y(), z);
        break;

    }

    
}

void ObjectSelectorSystem::updateAxis(ursine::Vec3 pos)
{
    //get their transforms, set data
    auto xTransf = m_xAxis->GetComponent<ursine::ecs::Transform>();
    auto yTransf = m_yAxis->GetComponent<ursine::ecs::Transform>();
    auto zTransf = m_zAxis->GetComponent<ursine::ecs::Transform>();
    {
        //translation
        xTransf->SetWorldPosition(ursine::SVec3(2, 0, 0) + pos);
        yTransf->SetWorldPosition(ursine::SVec3(0, 2, 0) + pos);
        zTransf->SetWorldPosition(ursine::SVec3(0, 0, 2) + pos);
    }
}