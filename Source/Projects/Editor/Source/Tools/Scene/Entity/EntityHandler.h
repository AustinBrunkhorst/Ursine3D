#pragma once

#include <NativeJSClass.h>

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

    JSMethod(updateComponentField);

    JSMethod(getChildren);
    JSMethod(getParent);
    JSMethod(setParent);
    JSMethod(getSiblingIndex);
    JSMethod(setSiblingIndex);

    JSMethod(saveAsArchetype);

private:
    ursine::ecs::World *m_world;
    ursine::ecs::EntityUniqueID m_handle;

    ursine::ecs::Entity *getEntity(void);  
} Meta(Enable);