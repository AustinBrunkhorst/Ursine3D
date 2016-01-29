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

    JSMethod(hasComponent);
    JSMethod(addComponent);
    JSMethod(removeComponent);

    JSMethod(componentFieldUpdate);

    JSMethod(componentFieldArrayUpdate);
    JSMethod(componentFieldArrayInsert);
    JSMethod(componentFieldArrayRemove);
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
	ursine::Scene::Handle m_scene;
    ursine::ecs::EntityUniqueID m_handle;
    
    ursine::ecs::Entity *getEntity(void);  
} Meta(Enable);