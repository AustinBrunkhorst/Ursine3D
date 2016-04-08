#include "Precompiled.h"

#include "WaypointUtils.h"

#include "Project.h"

#include "WaypointConnectorSystem.h"

#include <SystemManager.h>
#include <SystemConfig.h>
#include <Editor.h>

using namespace ursine;
using namespace ecs;

namespace
{
    // declare any helper functions here
    WaypointConnectorSystem *getWaypointConSys(void);
}
Meta(Enable, ExposeJavaScript)
JSFunction(WaypointUnidirectionalConnector)
{    
    // intelisense doesn't know that this works but it do
    WaypointConnectorSystem *waypointConSys = getWaypointConSys();

    waypointConSys->SetMode(WaypointConnectorSystem::MODE_UNIDIRECTIONAL);
        
    return CefV8Value::CreateUndefined();
}
Meta(Enable, ExposeJavaScript)
JSFunction(WaypointBidirectionalConnector)
{
    // intelisense doesn't know that this works but it do
    WaypointConnectorSystem *waypointConSys = getWaypointConSys();

    waypointConSys->SetMode(WaypointConnectorSystem::MODE_UNIDIRECTIONAL);

    return CefV8Value::CreateUndefined();
}

Meta(Enable, ExposeJavaScript)
JSFunction(WaypointEnableDrawLines)
{
    // intelisense doesn't know that this works but it do
    WaypointConnectorSystem *waypointConSys = getWaypointConSys();

    waypointConSys->SetShowLines(true);

    return CefV8Value::CreateUndefined();
}

Meta(Enable, ExposeJavaScript)
JSFunction(WaypointDisableDrawLines)
{
    // intelisense doesn't know that this works but it do
    WaypointConnectorSystem *waypointConSys = getWaypointConSys();

    waypointConSys->SetShowLines(false);

    return CefV8Value::CreateUndefined();
}

namespace
{
    // implement any helper functions here
    WaypointConnectorSystem *getWaypointConSys(void)
    {
        auto *editor = GetCoreSystem(Editor);
        auto proj = editor->GetProject();

        auto world = proj->GetScene().GetActiveWorld();

        // intelisense doesn't know that this works but it do
        return world->GetEntitySystem<ecs::WaypointConnectorSystem>();
    }
}
