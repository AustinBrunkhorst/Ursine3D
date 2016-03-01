/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** EntityHandler.h
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#pragma once

#include <NativeJSClass.h>

#include <mutex>

namespace ursine
{
    class Scene;
}

class EntityHandler : public ursine::NativeJSClass
{
    JAVASCRIPT_CLASS;

public:
    JSConstructor(EntityHandler);

    JSMethod(isValid);
    JSMethod(isRemovalEnabled);
    JSMethod(isHierarchyChangeEnabled);
    JSMethod(isVisibleInEditor);

    JSMethod(remove);

    JSMethod(getName);
    JSMethod(setName);
    
    JSMethod(inspect);
    JSMethod(inspectComponent);

    JSMethod(hasComponent);
    JSMethod(addComponent);
    JSMethod(removeComponent);

    JSMethod(componentSet);
    JSMethod(componentFieldUpdate);

    JSMethod(componentFieldArrayUpdate);
    JSMethod(componentFieldArrayInsert);
    JSMethod(componentFieldArrayRemove);
    JSMethod(componentFieldArraySwap);
    JSMethod(componentFieldArrayGetLength);

    JSMethod(componentButtonInvoke);

    JSMethod(getChildren);
    JSMethod(getParent);
    JSMethod(setParent);
    JSMethod(getSiblingIndex);
    JSMethod(setSiblingIndex);

    JSMethod(saveAsArchetype);
    JSMethod(clone);

private:
    const ursine::ecs::EntityHandle &getHandle(void);

    ursine::Scene *m_scene;
    ursine::ecs::World *m_world;
    ursine::ecs::EntityHandle m_handle;
} Meta(Enable);